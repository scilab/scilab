// =============================================================================
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
// =============================================================================
// <-- CLI SHELL MODE -->
// =============================================================================
content = ["a b c d e"; ..
"1 2 3 4 5"; ..
"2 3 4 5 6"; ..
"3 4 5 6 7"; ..
"4 5 6 7 8"; ..
"5 6 7 8 9"; ..
"6 7 8 9 10"]
filename=TMPDIR+"/foo.csv";
mputl(content,filename);

a=read_csv(filename)
b=read_csv(filename,[],[],"string")
assert_checkequal(a, b);

a2=csvRead(filename)
b2=csvRead(filename,[],[],"double")
assert_checkequal(a, b);

b3=csvRead(filename,[],[],"string")
assert_checkequal(a,b3);
assert_checkequal(b,b3);