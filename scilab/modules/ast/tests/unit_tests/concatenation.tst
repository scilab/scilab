// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

function checkCallOverload(mat)
    assert_checkerror(mat, [], 999);
endfunction

ldouble = [1 2 3];
lbool   = [%t %f %t];
lint    = [int32(1) int32(2) int32(3)];
lint16  = [int16(1) int16(2) int16(3)];
lpoly   = [%s %s^2 %s];
lsparse = [sparse(1) sparse(2) sparse(3)];
lspb    = [sparse(%t) sparse(%f) sparse(%t)];
lstring = ["Scilab" "Enterprises" "2014"];
sta.x   = 12;
lsta    = [sta sta sta];

// list, tlist, mlist
checkCallOverload("[list(1) list(2)]");

// Double
assert_checkequal([ldouble ldouble], [1 2 3 1 2 3]);
assert_checkequal([ldouble lbool], [ldouble double(lbool)]);
checkCallOverload("[ldouble lint]");
checkCallOverload("[ldouble lint16]");
assert_checkequal([ldouble lpoly], [(ldouble + 0*%s) lpoly]);
assert_checkequal([ldouble lsparse], [sparse(ldouble) lsparse]);
checkCallOverload("[ldouble lspb]");
checkCallOverload("[ldouble lstring]");
checkCallOverload("[ldouble lsta]");

assert_checkequal([ldouble ; ldouble], matrix([1 1 2 2 3 3], 2, 3));
assert_checkequal([ldouble ; lbool], [ldouble ; double(lbool)]);
checkCallOverload("[ldouble ; lint]");
checkCallOverload("[ldouble ; lint16]");
assert_checkequal([ldouble ; lpoly], [(ldouble + 0*%s) ; lpoly]);
assert_checkequal([ldouble ; lsparse], [sparse(ldouble) ; lsparse]);
checkCallOverload("[ldouble ; lspb]");
checkCallOverload("[ldouble ; lstring]");
checkCallOverload("[ldouble ; lsta]");

// Bool
assert_checkequal([lbool ldouble], [double(lbool) ldouble]);
assert_checkequal([lbool lbool], [%t %f %t %t %f %t]);
checkCallOverload("[lbool lint]");
checkCallOverload("[lbool lint16]");
checkCallOverload("[lbool lpoly]");
checkCallOverload("[lbool lsparse]");
assert_checkequal([lbool lspb], [sparse(lbool) lspb]);
checkCallOverload("[lbool lstring]");
checkCallOverload("[lbool lsta]");

assert_checkequal([lbool ; ldouble], [double(lbool) ; ldouble]);
assert_checkequal([lbool ; lbool], matrix([%t %t %f %f %t %t], 2, 3));
checkCallOverload("[lbool ; lint]");
checkCallOverload("[lbool ; lint16]");
checkCallOverload("[lbool ; lpoly]");
checkCallOverload("[lbool ; lsparse]");
assert_checkequal([lbool ; lspb], [sparse(lbool) ; lspb]);
checkCallOverload("[lbool ; lstring]");
checkCallOverload("[lbool ; lsta]");

// int
checkCallOverload("[lint ldouble]");
checkCallOverload("[lint lbool]");
assert_checkequal([lint lint], int32([1 2 3 1 2 3]));
checkCallOverload("[lint lint16]");
checkCallOverload("[lint lpoly]");
checkCallOverload("[lint lsparse]");
checkCallOverload("[lint lspb]");
checkCallOverload("[lint lstring]");
checkCallOverload("[lint lsta]");

checkCallOverload("[lint ; ldouble]");
checkCallOverload("[lint ; lbool]");
assert_checkequal([lint ; lint], int32(matrix([1 1 2 2 3 3], 2, 3)));
checkCallOverload("[lint ; lint16]");
checkCallOverload("[lint ; lpoly]");
checkCallOverload("[lint ; lsparse]");
checkCallOverload("[lint ; lspb]");
checkCallOverload("[lint  lstring]");
checkCallOverload("[lint ; lsta]");

// polynom
assert_checkequal([lpoly ldouble], [%s %s^2 %s 1 2 3]);
checkCallOverload("[lpoly lbool]");
checkCallOverload("[lpoly lint]");
checkCallOverload("[lpoly lint16]");
assert_checkequal([lpoly lpoly], [%s %s^2 %s %s %s^2 %s]);
checkCallOverload("[lpoly lsparse]");
checkCallOverload("[lpoly lspb]");
checkCallOverload("[lpoly lstring]");
checkCallOverload("[lpoly lsta]");

assert_checkequal([lpoly ; ldouble], matrix([%s 1 %s^2 2 %s 3], 2, 3));
checkCallOverload("[lpoly ; lbool]");
checkCallOverload("[lpoly ; lint]");
checkCallOverload("[lpoly ; lint16]");
assert_checkequal([lpoly ; lpoly], matrix([%s %s %s^2 %s^2 %s %s], 2, 3));
checkCallOverload("[lpoly ; lsparse]");
checkCallOverload("[lpoly ; lspb]");
checkCallOverload("[lpoly ; lstring]");
checkCallOverload("[lpoly ; lsta]");

// sparse
assert_checkequal([lsparse ldouble], sparse([1 2 3 1 2 3]));
checkCallOverload("[lsparse lbool]");
checkCallOverload("[lsparse lint]");
checkCallOverload("[lsparse lint16]");
checkCallOverload("[lsparse lpoly]");
assert_checkequal([lsparse lsparse], sparse([1 2 3 1 2 3]));
checkCallOverload("[lsparse lspb]");
checkCallOverload("[lsparse lstring]");
checkCallOverload("[lsparse lsta]");

assert_checkequal([lsparse ; ldouble], sparse(matrix([1 1 2 2 3 3], 2, 3)));
checkCallOverload("[lsparse  ;lbool]");
checkCallOverload("[lsparse ; lint]");
checkCallOverload("[lsparse ; lint16]");
checkCallOverload("[lsparse ; lpoly]");
assert_checkequal([lsparse ; lsparse], sparse(matrix([1 1 2 2 3 3], 2, 3)));
checkCallOverload("[lsparse ; lspb]");
checkCallOverload("[lsparse ; lstring]");
checkCallOverload("[lsparse ; lsta]");

// boolean sparse
checkCallOverload("[lspb ldouble]");
assert_checkequal([lspb lbool], sparse([%t %f %t %t %f %t]));
checkCallOverload("[lspb lint]");
checkCallOverload("[lspb lint16]");
checkCallOverload("[lspb lpoly]");
checkCallOverload("[lspb lsparse]");
assert_checkequal([lspb lspb], sparse([%t %f %t %t %f %t]));
checkCallOverload("[lspb lstring]");
checkCallOverload("[lspb lsta]");

checkCallOverload("[lspb ; ldouble]");
assert_checkequal([lspb ; lbool], sparse(matrix([%t %t %f %f %t %t], 2 ,3)));
checkCallOverload("[lspb ; lint]");
checkCallOverload("[lspb ; lint16]");
checkCallOverload("[lspb ; lpoly]");
checkCallOverload("[lspb ; lsparse]");
assert_checkequal([lspb ; lspb], sparse(matrix([%t %t %f %f %t %t], 2 ,3)));
checkCallOverload("[lspb ; lstring]");
checkCallOverload("[lspb ; lsta]");

// string
checkCallOverload("[lstring ldouble]");
checkCallOverload("[lstring lbool]");
checkCallOverload("[lstring lint]");
checkCallOverload("[lstring lint16]");
checkCallOverload("[lstring lpoly]");
checkCallOverload("[lstring lsparse]");
checkCallOverload("[lstring lspb]");
assert_checkequal([lstring lstring], ["Scilab" "Enterprises" "2014" "Scilab" "Enterprises" "2014"]);
checkCallOverload("[lstring lsta]");

checkCallOverload("[lstring ; ldouble]");
checkCallOverload("[lstring ; lbool]");
checkCallOverload("[lstring ; lint]");
checkCallOverload("[lstring ; lint16]");
checkCallOverload("[lstring ; lpoly]");
checkCallOverload("[lstring ; lsparse]");
checkCallOverload("[lstring ; lspb]");
assert_checkequal([lstring ; lstring], matrix(["Scilab" "Scilab" "Enterprises" "Enterprises" "2014" "2014"], 2, 3));
checkCallOverload("[lstring ; lsta]");

// struct
stb.y = 23;
stc.x = 56;
lstb  = [stb stb stb];
lstc  = [stc stc stc];

checkCallOverload("[lsta ldouble]");
checkCallOverload("[lsta lbool]");
checkCallOverload("[lsta lint]");
checkCallOverload("[lsta lint16]");
checkCallOverload("[lsta lpoly]");
checkCallOverload("[lsta lsparse]");
checkCallOverload("[lsta lspb]");
checkCallOverload("[lsta lstring]");
assert_checkequal([lsta lsta], [sta sta sta sta sta sta]);
assert_checkerror("[lsta ; lstb]", msprintf(_("%s: Field names mismatch.\n"),"%st_c_st"), 10000);

checkCallOverload("[lsta ; ldouble]");
checkCallOverload("[lsta ; lbool]");
checkCallOverload("[lsta ; lint]");
checkCallOverload("[lsta ; lint16]");
checkCallOverload("[lsta ; lpoly]");
checkCallOverload("[lsta ; lsparse]");
checkCallOverload("[lsta ; lspb]");
checkCallOverload("[lsta ; lstring]");
assert_checkequal([lsta ; lsta], matrix([sta sta sta sta sta sta], 2, 3));
assert_checkerror("[lsta ; lstb]", msprintf(_("%s: Field names mismatch.\n"),"%st_c_st"), 10000);

// ImplicitList
checkCallOverload("[1:$ 2]");
checkCallOverload("[1:$ 1:2]");
checkCallOverload("[1:$ int8(2)]");
checkCallOverload("[1:2 1:$]");
checkCallOverload("[1:$ 1:$]");
assert_checkequal([1:3 1:3], [1 2 3 1 2 3]);

checkCallOverload("[1:$ ; 2]");
checkCallOverload("[1:$ ; 1:2]");
checkCallOverload("[1:$ ; int8(2)]");
checkCallOverload("[1:2 ; 1:$]");
checkCallOverload("[1:$ ; 1:$]");
assert_checkequal([1:3 ; 1:3], matrix([1 1 2 2 3 3], 2, 3));

