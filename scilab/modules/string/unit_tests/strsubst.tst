
//strsubst
if strsubst('abc,abd,aa,bxe',',',';')<>'abc;abd;aa;bxe'  then pause,end
if strsubst('abc,abd,aa,bxe',',',emptystr())<>'abcabdaabxe'  then pause,end
if strsubst(',abc,abd,aa,bxe',',',emptystr())<>'abcabdaabxe'  then pause,end
if strsubst('abc',',',';')<>'abc'  then pause,end
