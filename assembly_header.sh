rm -f fmstd.h
echo -e "/*
Automaticaly created.
fmstd.h - Felipe Monteiro's C standard library

Check the [repostiory](https://github.com/felipemarkson/fmstd) for more details.
---
LICENSE

$(cat ./LICENSE)
---
*/" >> fmstd.h

cat  \
src/dependency.h \
src/common.h \
src/darray.h \
src/dstr.h \
src/strv.h \
>> fmstd.h

gcc -DIMPLEMENT_FMALL -c fmstd.h
rm -f fmstd.h.gch