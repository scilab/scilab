// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// ascii to string
// ---------------
ref  = [97 98 99 ;100 101 102];
code = asciimat(["a" "b" "c";"d" "e" "f"]);
assert_checkequal(ref, code);

ref  = [97 122 98 99 ;100 101 121 102];
code = asciimat(["az" "b" "c";"d" "ey" "f"]);
assert_checkequal(ref, code);

// 3D hypermat
m = matrix(97:120,4,3,2);
t = asciimat(m);
ref = ["aei" "mqu"
       "bfj" "nrv"
       "cgk" "osw"
       "dhl" "ptx"
       ];
assert_checkequal(t, ref);

// 12D hypermat
h = cat(12,m,m);
t = asciimat(h);
assert_checkequal(t, cat(11,ref,ref));

// string to ascii
// ---------------
// not managed in scilab 5
scilabversion = getversion("scilab");
if scilabversion(1) >= 6
    ref  = matrix(97:120,2,3,4);
    a    = matrix(strsplit(ascii(97:120),1:23),2,3,4);
    code = asciimat(a);
    assert_checkequal(ref, code);
end
//
ref = ["azerty";"ytreza"];
str = asciimat([97 122 101 114 116 121; 121 116 114 101 122 97]);
assert_checkequal(ref, str);

ref = ["bdf" "hjl" "npr" "tvx";"ceg" "ikm" "oqs" "uwy"];
a   = matrix(97+(1:2*3*4),2,3,4);
str = asciimat(a);
assert_checkequal(ref, str);

// With UTF8 (limited application)
t = "àâãäéèêëìîïòôöùûü";
assert_checkequal(asciimat(asciimat(t)), t);
assert_checkequal(asciimat(asciimat([t t])), t+t);
assert_checkequal(asciimat(asciimat([t;t])), [t;t]);


