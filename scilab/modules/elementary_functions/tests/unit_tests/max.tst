// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

Ntest = 1;
// test max(A) : A matrix
// maxi is compared with sort

for i = 1:Ntest,
    m = 100;n = 200;a = rand(m,n);
    [am,ak] = max(a); [a1,ak1] = gsort(a);
    assert_checkequal(am, a1(1,1));
    assert_checkequal(ak(1) + m * (ak(2) - 1), ak1(1,1));
end

// test max(A) : A vector

for i = 1:Ntest,
    m = 100;a = rand(1,m);
    [am,ak] = max(a); [a1,ak1] = gsort(a);
    assert_checkequal(am, a1(1,1));
    assert_checkequal(ak, ak1(1,1));
end

// test mini

for i=1:Ntest,
    m = 100;n = 200;a = rand(m,n);
    [am,ak] = max(-a); [ami,aki] = min(a);
    assert_checkequal(am, -ami);
    assert_checkequal(ak, aki);
end

for i = 1:Ntest,
    m = 100;a = rand(1,m);
    [am,ak] = max(-a); [ami,aki] = min(a);
    assert_checkequal(am, -ami);
    assert_checkequal(ak, aki);
end

// test max(A1,A....) compared with usual maxi + feval

m = 10;
n = 20;
A1 = rand(m,n);
A2 = rand(m,n);
A3 = rand(m,n);
A4 = rand(m,n);
function [y]=f(i,j)
    [y,k]=max([A1(i,j),A2(i,j),A3(i,j),A4(i,j)]);y=y+%i*k;
endfunction
A = feval(1:m,1:n,f);
Am = real(A);Ak = imag(A);
[Am1, Ak1] = max(A1, A2, A3, A4);

assert_checkequal(Am1, Am);
assert_checkequal(Ak1, Ak);

// test max(list(A1,A2,..)) compared to max(A1,..)

[Al, Akl] = max(list(A1, A2, A3, A4));

assert_checkequal(Al, Am);
assert_checkequal(Akl, Ak);

// test max(A,'c') and max(A,'r')
// max(A,'r') returns a row vector which contains max for each column

[Am, Akm] = max(A1, "r");
nc = size(A1, "c");
Am1 = [];
Ak1 = [];
for i = 1:nc
    [am, ak] = max(A1(:,i));
    Am1 = [Am1, am];
    Ak1 = [Ak1, ak];
end
assert_checkequal(Am, Am1);
assert_checkequal(Akm, Ak1);

// max(A,'c');

[Am, Akm] = max(A1, "c");
nc = size(A1, "r");
Am1 = [];
Ak1 = [];
for i = 1:nc
    [am, ak] = max(A1(i,:));
    Am1 = [Am1; am];
    Ak1 = [Ak1; ak];
end

assert_checkequal(Am, Am1);
assert_checkequal(Akm, Ak1);

// ******************** WITH SPARSES ********************
// check error messages
A = [1 2;3 4];
A = sparse(A);
assert_checkfalse(execstr("[m,i] = max(A,''c'')", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of output argument: %d expected.\n"), "%sp_max", 1);
assert_checkerror("[m,i] = max(A,''c'')", refMsg);

A = [1 2;3 4];
A = sparse(A);
assert_checkfalse(execstr("m = max(A,full(A))", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A sparse matrix or a character expected.\n"), "%sp_max", 2);
assert_checkerror("m = max(A,full(A))", refMsg);

B = sparse([1 2]);
assert_checkfalse(execstr("m = max(A,B)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size of input argument #%d: Same size as input argument #%d expected.\n"), "%sp_max", 2, 1);
assert_checkerror("m = max(A,B)", refMsg);

B = B';
assert_checkfalse(execstr("m = max(A,B)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size of input argument #%d: Same size as input argument #%d expected.\n"), "%sp_max", 2, 1);
assert_checkerror("m = max(A,B)", refMsg);

B = [6 7;8 9];
C = [2 4; 6 8];
assert_checkfalse(execstr("[m,i] = max(A,B,C)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of output argument: %d expected.\n"), "%sp_max", 1);
assert_checkerror("[m,i] = max(A,B,C)", refMsg);


assert_checkfalse(execstr("m = max(A,sparse(B),C)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A sparse matrix expected.\n"), "%sp_max", 3);
assert_checkerror("m = max(A,sparse(B),C)", refMsg);

L = list(A,sparse(B),C);
assert_checkfalse(execstr("m = max(L)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d (List element: %d): A sparse matrix expected.\n"), "%sp_max", 1, 3);
assert_checkerror("m = max(L)", refMsg);

C = sparse([1 2]);
L = list(A,sparse(B),C);
assert_checkfalse(execstr("m = max(L)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size of input argument #%d (List element: %d): Same size as input argument #%d expected.\n"), "%sp_max", 1, 3, 1);
assert_checkerror("m = max(L)", refMsg);

assert_checkfalse(execstr("m = max(A,sparse(B),C)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size of input argument #%d: Same size as input argument #%d expected.\n"), "%sp_max", 3, 1);
assert_checkerror("m = max(A,sparse(B),C)", refMsg);


a = -1 * rand(3,2);
a(2) = 0;
A = sparse(a);
[m, i] = max(a);
[m1, i1] = max(A);
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(a');
[m1, i1] = max(A');
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(a(:));
[m1, i1] = max(A(:));
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(a(:)');
[m1, i1] = max(A(:)');
assert_checkequal(m1,m);
assert_checkequal(i1,i);

a = [-3 -4;0 -1;-5 -6];
A = sparse(a);
[m, i] = max(a);
[m1, i1] = max(A);
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(a');
[m1, i1] = max(A');
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(a(:));
[m1, i1] = max(A(:));
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(a(:)');
[m1, i1] = max(A(:)');
assert_checkequal(m1,m);
assert_checkequal(i1,i);

b = [-5 0;-1 -6];
B = sparse(b);
[m, i] = max(b);
[m1, i1] = max(B);
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(b');
[m1, i1] = max(B');
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(b(:));
[m1, i1] = max(B(:));
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(b(:)');
[m1, i1] = max(B(:)');
assert_checkequal(m1,m);
assert_checkequal(i1,i);

s = rand(3,2);
s(2) = 0;
S = sparse(s);
[m, i] = min(s);
[m1, i1] = min(S);
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = min(s');
[m1, i1] = min(S');
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = min(s(:));
[m1, i1] = min(S(:));
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = min(s(:)');
[m1, i1] = min(S(:)');
assert_checkequal(m1,m);
assert_checkequal(i1,i);

v = [1 4;7 6];
V = sparse(v);
[m, i] = max(v);
[m1, i1] = max(V);
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(v');
[m1, i1] = max(V');
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(v(:));
[m1, i1] = max(V(:));
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(v(:)');
[m1, i1] = max(V(:)');
assert_checkequal(m1,m);
assert_checkequal(i1,i);

// max(A1, A2,..., An)
A1 = sprand(3,3,3);
A2 = sprand(3,3,3);
A3 = sprand(3,3,3);
m = max(full(A1), full(A2), full(A3));
m1 = max(A1, A2, A3);
assert_checkequal(full(m1), m);

// with list
m = max(list(full(A1), full(A2), full(A3)));
m1 = max(list(A1, A2, A3));
assert_checkequal(full(m1), m);

// min(A1, A2,..., An)
A1 = sprand(3,3,3);
A2 = sprand(3,3,3);
A3 = sprand(3,3,3);
m = min(full(A1), full(A2), full(A3));
m1 = min(A1, A2, A3);
assert_checkequal(full(m1), m);

// with list
m = min(list(full(A1), full(A2), full(A3)));
m1 = min(list(A1, A2, A3));
assert_checkequal(full(m1), m);

for i = 1:Ntest,
    m = 100;a = rand(1,m);
    [am,ak] = max(-a); [ami,aki] = min(a);
    assert_checkequal(am, -ami);
    assert_checkequal(ak, aki);
end

// test max(A1,A....) compared with usual maxi + feval

m = 10;
n = 20;
A1 = sprand(m,n,10);
A2 = sprand(m,n,10);
A3 = sprand(m,n,10);
A4 = sprand(m,n,10);
function [y]=f(i,j)
    [y,k]=max([A1(i,j),A2(i,j),A3(i,j),A4(i,j)]);y=y+%i*k;
endfunction
A = feval(1:m,1:n,f);
Am = real(A);Ak = imag(A);
Am1 = max(A1, A2, A3, A4);

assert_checkequal(Am1, sparse(Am));

// test max(list(A1,A2,..)) compared to max(A1,..)

Al = max(list(A1, A2, A3, A4));
assert_checkequal(Al, sparse(Am));

// test max(A,'c') and max(A,'r')
// max(A,'r') returns a row vector which contains max for each column

[Am] = max(A1, "r");
nc = size(A1, "c");
Am1 = [];
for i = 1:nc
    [am] = max(A1(:,i));
    Am1 = [Am1, am];
end
assert_checkequal(Am, sparse(Am1));

// max(A,'c');

[Am] = max(A1, "c");
nc = size(A1, "r");
Am1 = [];
for i = 1:nc
    [am] = max(A1(i,:));
    Am1 = [Am1; am];
end
assert_checkequal(Am, sparse(Am1));

// test min(A1,A....) compared with usual maxi + feval

m = 10;
n = 20;
A1 = sprand(m,n,10);
A2 = sprand(m,n,10);
A3 = sprand(m,n,10);
A4 = sprand(m,n,10);
function [y]=g(i,j)
    [y,k]=min([A1(i,j),A2(i,j),A3(i,j),A4(i,j)]);y=y+%i*k;
endfunction
A = feval(1:m,1:n,g);
Am = real(A);Ak = imag(A);
Am1 = min(A1, A2, A3, A4);

assert_checkequal(Am1, sparse(Am));

// test min(list(A1,A2,..)) compared to min(A1,..)

Al = min(list(A1, A2, A3, A4));
assert_checkequal(Al, sparse(Am));

// test min(A,'c') and min(A,'r')
// min(A,'r') returns a row vector which contains min for each column

[Am] = min(A1, "r");
nc = size(A1, "c");
Am1 = [];
for i = 1:nc
    [am] = min(A1(:,i));
    Am1 = [Am1, am];
end
assert_checkequal(Am, sparse(Am1));

// min(A,'c');

[Am] = min(A1, "c");
nc = size(A1, "r");
Am1 = [];
for i = 1:nc
    [am] = min(A1(i,:));
    Am1 = [Am1; am];
end
assert_checkequal(Am, sparse(Am1));
