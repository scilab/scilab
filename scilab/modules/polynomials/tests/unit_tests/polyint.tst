// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON - Le Mans Université
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

s = %s;
P = s^[0 1 ; 2 3];

// SINGLE INTEGRATION
// ==================
assert_checkequal(polyint(s), s^2/2);
Ref0 = s^[1 2 ; 3 4] ./ [1 2 ; 3 4];
assert_checkequal(polyint(P), Ref0);
Ref = [0, -s+0.5*s^2 ; -s+s^3/3,-s+0.25*s^4];
assert_checkequal(polyint(P-1), Ref);

// With consts
// -----------
assert_checkequal(polyint(P, 1), Ref0+1);
assert_checkequal(polyint(P, [1 2 ; 3 4]), Ref0+[1 2 ; 3 4]);
assert_checkequal(polyint(P, list([1 2 ; 3 4])), Ref0+[1 2 ; 3 4]);


// MULTIPLE INTEGRATIONS
// =====================
assert_checkequal(polyint(s,list(-1,1)), 1-s+s^3/6);

o = list(); o(5) = 0;   // undefined elements #1-4
assert_checkequal(polyint(1+0*s, o), s^5/factorial(5));

Ref = [-2+2*s+1.5*s^2 ; 1+2*s+0.5*s^2-s^3/6 ; 2*s-s^2+s^4/12];
assert_checkequal(polyint([3 ; 1-s ; s^2-2], list(2,[-2 1 0]')), Ref);
Ref = [-2 + 1.5*s^2 ; 1 + 0.5*s^2 - s^3/6 ; -s^2 + s^4/12];
assert_checkequal(polyint([3 ; 1-s ; s^2-2], list(,[-2 1 0]')), Ref);


// ERROR MESSAGES
// ==============
assert_checkerror("polyint(1,2,3)", _("Wrong number of input arguments."));
msg = msprintf(_("%s: Argument #%d: Polynomial expected.\n"), "polyint",1);
assert_checkerror("polyint(3)", msg);
msg = _("%s: Argument #%d: numbers or list of numbers expected.\n");
assert_checkerror("polyint(%s, %f)", msprintf(msg, "polyint",2));
msg = _("%s: Argument #%d(%d): Decimal or complex number expected.\n");
assert_checkerror("polyint(%s, list(3,%f))", msprintf(msg, "polyint",2,2));
msg = _("%s: Argument #%d(%d): Array of size [%s] expected.\n");
assert_checkerror("polyint(%s, [1 2])", msprintf(msg, "polyint",2,1,"1 1"));
assert_checkerror("polyint(%s, list([1 2]))", msprintf(msg, "polyint",2,1,"1 1"));
assert_checkerror("polyint(%s, list([1 2],4))", msprintf(msg, "polyint",2,1,"1 1"));
assert_checkerror("polyint(%s, list(4,,[1 2]))", msprintf(msg, "polyint",2,3,"1 1"));
