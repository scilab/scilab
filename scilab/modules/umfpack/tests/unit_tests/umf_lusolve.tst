// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for umf_lusolve function
// =============================================================================

assert_checkfalse(execstr("umf_lusolve()"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d to %d expected.\n"), "umf_lusolve", 2, 4);
assert_checkerror("umf_lusolve()", refMsg);

assert_checkfalse(execstr("umf_lusolve(1,1)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A pointer expected.\n"), "umf_lusolve", 1);
assert_checkerror("umf_lusolve(1,1)", refMsg);

B = rand(3,3)*%i;
Lup = umf_lufact(sparse(B));
assert_checkfalse(execstr("umf_lusolve(Lup, speye(3,3))"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A real or complex column vector or matrix expected.\n"), "umf_lusolve", 2);
assert_checkerror("umf_lusolve(Lup, speye(3,3))", refMsg);

assert_checkfalse(execstr("umf_lusolve(Lup, eye(3,3), ""Cx=d"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong input argument #%d: ''%s'' or ''%s'' expected.\n"), "umf_lusolve", 3, "Ax=b", "A''x=b");
assert_checkerror("umf_lusolve(Lup, eye(3,3), ""Cx=d"")", refMsg);

assert_checkfalse(execstr("umf_lusolve(Lup, eye(3,3), ""Ax=b"", ""titi"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A sparse matrix expected.\n"), "umf_lusolve", 4);
assert_checkerror("umf_lusolve(Lup, eye(3,3), ""Ax=b"", ""titi"")", refMsg);

x = umf_lusolve(Lup, eye(3,3));
assert_checkalmostequal(x, inv(B));
umf_ludel(Lup);

spB = sparse(B);
Lup = umf_lufact(spB);
b = ones(size(spB,1),1);
// use umf_lusolve for solving Ax=b
x = umf_lusolve(Lup,b);
firstNorm=norm(spB*x - b);

// now the same thing with iterative refiment
x = umf_lusolve(Lup,b,"Ax=b",spB);
secondNorm=norm(spB*x - b);

assert_checkalmostequal(firstNorm, secondNorm);
umf_ludel(Lup);
