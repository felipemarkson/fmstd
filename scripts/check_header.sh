value=($(cksum fmstd.h))
value=${value[0]}
./scripts/assembly_header.sh
value2=($(cksum fmstd.h))
value2=${value2[0]}
echo $(expr $value - $value2)
exit $(expr $value - $value2)