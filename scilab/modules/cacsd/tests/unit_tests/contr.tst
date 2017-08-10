//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge Steer
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
assert_checkfalse(execstr("contr()", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d to %d expected.\n"), "contr", 2, 3); 
assert_checkerror("contr()", refMsg);

A = ones(3,3) + 0 * %i;
B = ones(3,3) * 2;
assert_checkfalse(execstr("contr(A, B)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "contr", 1); 
assert_checkerror("contr(A, B)", refMsg);

A = ones(3,3);
assert_checkfalse(execstr("contr(A, complex(B))", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "contr", 2); 
assert_checkerror("contr(A, complex(B))", refMsg);

assert_checkfalse(execstr("contr(sparse(A), B)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "contr", 1); 
assert_checkerror("contr(sparse(A), B)", refMsg);

assert_checkfalse(execstr("contr(A, sparse(B))", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "contr", 2); 
assert_checkerror("contr(A, sparse(B))", refMsg);

a = [0.8604043 , 0.0070020 , 0.6223373 , -1.356213 , -4.2301775
     0.159714 ,  0.0857791 , -0.2367751 , 0.4958580 , 0.6398817
     -4.3054931 , -0.0365878 , 2.1784911 , 0.0314793 , 2.3728994
     -3.7115779 , -0.5370809 , 2.4291716 , -0.6352663 , 2.6972781
     6.3580966 , 0.1377712 , -4.0461243 , -0.5192899 , -4.0394083];
b = [-0.0532544 , -0.1494083 , -0.0098619
     -0.0355030 , -0.0162722 , 0.1045365
     -0.1301775 , 0.1736686 , -0.0611440
     0.1834320 , 0.1757396 , -0.5956607
     0.1775148 , -0.1186391 , 0.1439842];
c = [2 , 7 , -2 , 5 , 1
     0 , -1 , 3 , 0 , 2];
d = [1 , 0 , 0
     0 , 0 , 0];

assert_checkequal(contr(a, b), 4);

[n,U]=contr(a,b);a1=U'*a*U;
assert_checkalmostequal(abs(spec(a1(n+1:$,n+1:$))), 0, [], 1e-7);

[n,U,ind,V,Ac,Bc]=contr(a,b);
assert_checkequal(n, 4);
assert_checkalmostequal(norm(Ac(n+1:$,:),1), 0, [], 1e-7);
assert_checkalmostequal(norm(U'*a*U-Ac,1), 0, [], 1e-6);
assert_checkalmostequal(norm(U'*b*V-Bc), 0, [], 1e-10);

assert_checkfalse(execstr("contr(a,1)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong values for input arguments #%d and #%d.\n"), "contr", 1, 2); 
assert_checkerror("contr(a,1)", refMsg);

assert_checkfalse(execstr("contr(a+%s,b)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "contr", 1); 
assert_checkerror("contr(a+%s,b)", refMsg);

assert_checkfalse(execstr("contr(a,b,''ee'')", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A real scalar expected.\n"), "contr", 3); 
assert_checkerror("contr(a,b,''ee'')", refMsg);

assert_checkequal(contr(a',c',0.0001), 3);


