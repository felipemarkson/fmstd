rm -f fm_std.h
echo -e "/*
Automaticaly created.
fm_std.h - Felipe Monteiro's C standard library

Check the [repostiory](https://github.com/felipemarkson/fm_std) for more details.
---
LICENSE

$(cat ./LICENSE)
---
*/" >> fm_std.h

cat  \
src/dependency.h \
src/common.h \
src/darray.h \
src/dstr.h \
>> fm_std.h