#ifndef FMHASH_H
#define FMHASH_H

#ifndef FMNONNULL // The value cannot be NULL
#define FMNONNULL
#endif

#include <stddef.h> // size_t, NULL
size_t fmhash(FMNONNULL const void* p, size_t len, size_t seed);

#endif // FMHASH_H

#ifdef FM_TEST
#define FMHASH_IMPL
#endif

// Code got from https://github.com/nothings/stb/blob/master/stb_ds.h
// at commit 1ee679ca2ef753a528db5ba6801e1067b40481b8
// The code bellow is published as public domain, so no problem here.
// Some minor code changes were added to avoid compilation warnings.
#ifdef FMHASH_IMPL

#define FMHS_STBDS_SIZE_T_BITS ((sizeof(size_t)) * 8)
#define FMHS_STBDS_SIPHASH_C_ROUNDS 1
#define FMHS_STBDS_SIPHASH_D_ROUNDS 1
#define FMHS_STBDS_ROTATE_LEFT(val, n)                                                 \
    (((val) << (n)) | ((val) >> (FMHS_STBDS_SIZE_T_BITS - (n))))
#define FMHS_STBDS_ROTATE_RIGHT(val, n)                                                \
    (((val) >> (n)) | ((val) << (FMHS_STBDS_SIZE_T_BITS - (n))))

static size_t fmhs_stbds_siphash_bytes(const void* p, size_t len, size_t seed) {
    const unsigned char* d = (const unsigned char*)p;
    size_t i, j;
    size_t v0, v1, v2, v3, data;

    // hash that works on 32- or 64-bit registers without knowing which we have
    // (computes different results on 32-bit and 64-bit platform) derived from siphash,
    // but on 32-bit platforms very different as it uses 4 32-bit state not 4 64-bit
    v0 = ((((size_t)0x736f6d65 << 16) << 16) + 0x70736575) ^ seed;
    v1 = ((((size_t)0x646f7261 << 16) << 16) + 0x6e646f6d) ^ ~seed;
    v2 = ((((size_t)0x6c796765 << 16) << 16) + 0x6e657261) ^ seed;
    v3 = ((((size_t)0x74656462 << 16) << 16) + 0x79746573) ^ ~seed;

#define FMHS_STBDS_SIPROUND()                                                          \
    do {                                                                               \
        v0 += v1;                                                                      \
        v1 = FMHS_STBDS_ROTATE_LEFT(v1, 13);                                           \
        v1 ^= v0;                                                                      \
        v0 = FMHS_STBDS_ROTATE_LEFT(v0, FMHS_STBDS_SIZE_T_BITS / 2);                   \
        v2 += v3;                                                                      \
        v3 = FMHS_STBDS_ROTATE_LEFT(v3, 16);                                           \
        v3 ^= v2;                                                                      \
        v2 += v1;                                                                      \
        v1 = FMHS_STBDS_ROTATE_LEFT(v1, 17);                                           \
        v1 ^= v2;                                                                      \
        v2 = FMHS_STBDS_ROTATE_LEFT(v2, FMHS_STBDS_SIZE_T_BITS / 2);                   \
        v0 += v3;                                                                      \
        v3 = FMHS_STBDS_ROTATE_LEFT(v3, 21);                                           \
        v3 ^= v0;                                                                      \
    } while (0)

    for (i = 0; i + sizeof(size_t) <= len; i += sizeof(size_t), d += sizeof(size_t)) {
        data = (size_t)(d[0] | (d[1] << 8) | (d[2] << 16) | (d[3] << 24));
        data |= (size_t)(d[4] | (d[5] << 8) | (d[6] << 16) | (d[7] << 24))
                << 16 << 16; // discarded if size_t == 4

        v3 ^= data;
        for (j = 0; j < FMHS_STBDS_SIPHASH_C_ROUNDS; ++j)
            FMHS_STBDS_SIPROUND();
        v0 ^= data;
    }
    data = len << (FMHS_STBDS_SIZE_T_BITS - 8);
    switch (len - i) {
    case 7U:
        data |= (size_t)((d[6] << 24U) << 24U); // fall through
    case 6U:
        data |= (size_t)((d[5] << 20U) << 20U); // fall through
    case 5U:
        data |= (size_t)((d[4] << 16U) << 16U); // fall through
    case 4U:
        data |= (size_t)(d[3] << 24U); // fall through
    case 3U:
        data |= (size_t)(d[2] << 16U); // fall through
    case 2U:
        data |= (size_t)(d[1] << 8U); // fall through
    case 1U:
        data |= (size_t)d[0]; // fall through
    case 0U:
        break;
    default:
        break;
    }
    v3 ^= data;
    for (j = 0; j < FMHS_STBDS_SIPHASH_C_ROUNDS; ++j)
        FMHS_STBDS_SIPROUND();
    v2 ^= 0xff;
    for (j = 0; j < FMHS_STBDS_SIPHASH_D_ROUNDS; ++j)
        FMHS_STBDS_SIPROUND();

    // slightly stronger since v0^v3 in above cancels out final round operation?
    // I tweeted at the authors of SipHash about this but they didn't reply
    return v1 ^ v2 ^ v3;
}

static size_t fmhs_stbds_hash_bytes(const void* p, size_t len, size_t seed) {
    const unsigned char* d = p;

    if (len == 4) {
        size_t hash = (size_t)(d[0] | (d[1] << 8) | (d[2] << 16) | (d[3] << 24));
        // HASH32-BB  Bob Jenkin's presumably-accidental version of Thomas Wang hash
        // with rotates turned into shifts. Note that converting these back to rotates
        // makes it run a lot slower, presumably due to collisions, so I'm not really
        // sure what's going on.
        hash ^= seed;
        hash = (hash ^ 61) ^ (hash >> 16);
        hash = hash + (hash << 3);
        hash = hash ^ (hash >> 4);
        hash = hash * 0x27d4eb2d;
        hash ^= seed;
        hash = hash ^ (hash >> 15);
        return (((size_t)hash << 16 << 16) | hash) ^ seed;
    } else if (len == 8 && sizeof(size_t) == 8) {
        size_t hash = (size_t)(d[0] | (d[1] << 8) | (d[2] << 16) | (d[3] << 24));

        // avoid warning if size_t == 4
        hash |= (size_t)(d[4] | (d[5] << 8) | (d[6] << 16) | (d[7] << 24)) << 16 << 16;
        hash ^= seed;
        hash = (~hash) + (hash << 21);
        hash ^= FMHS_STBDS_ROTATE_RIGHT(hash, 24);
        hash *= 265;
        hash ^= FMHS_STBDS_ROTATE_RIGHT(hash, 14);
        hash ^= seed;
        hash *= 21;
        hash ^= FMHS_STBDS_ROTATE_RIGHT(hash, 28);
        hash += (hash << 31);
        hash = (~hash) + (hash << 18);
        return hash;
    } else {
        return fmhs_stbds_siphash_bytes(p, len, seed);
    }
}

size_t fmhash(const void* p, size_t len, size_t seed) {
    return fmhs_stbds_hash_bytes(p, len, seed);
}
#endif // FMHASH_IMPL

#ifdef FM_TEST

// Avoiding test collision
#undef FM_TEST
#define FMHS_IMPL
#include "fmhs.h"
#define FM_TEST

#include <stdio.h>
int test_ok = 1;
#define xstr(a) str(a)
#define str(a) #a
#define TEST(func, ctx)                                                                \
    test_ok = 1;                                                                       \
    printf("TESTING:" xstr(func) " ");                                                 \
    fflush(stdout);                                                                    \
    ctx;                                                                               \
    test_ok ? printf(" OK!\n") : printf("\nTEST FAIL!\n");                             \
    test_ok ? (void)0 : exit(EXIT_FAILURE);                                            \
    (void)0

#define ASSERT(expr, msg)                                                              \
    do {                                                                               \
        if (!(expr)) {                                                                 \
            fprintf(stderr, "\n%s", msg);                                              \
            fflush(stderr);                                                            \
            test_ok = 0;                                                               \
        }                                                                              \
    } while (0)

int cmp(const void* item1ptr, const void* item2ptr, size_t itemsize) {
    (void)itemsize;
    return (int)(*(const short*)item1ptr - *(const short*)item2ptr);
}
int main(void) {
    TEST(fmhash, {
        fmhs_t hs = {0};
        fmhs_new(&hs, sizeof(short), cmp, fmhash, 33, 256);
        fmhs_insert(&hs, &(short){1});
        fmhs_insert(&hs, &(short){-2});
        fmhs_insert(&hs, &(short){-3});
        fmhs_insert(&hs, &(short){-7});
        fmhs_delete(&hs);
    });
    return 0;
}

#endif
