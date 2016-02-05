// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) ????-2008 - ENPC
// Copyright (C) 2008 - DIGITEO - Allan CORNET
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//================================ complex matrix ================================================
x = [3  1  5 ; 2 9 8];
y = [2  4  1 ; 4 1 3];
c = x + y * %i;

ref_ind = [    4.    5.    3.  ;    6.    2.    1.  ];
ref_values = [     9. + %i      5. + %i      1. + 4.*%i  ;    8. + 3.*%i    2. + 4.*%i    3. + 2.*%i ];
[v,ind] = gsort(c);
assert_checkequal(ref_ind, ind);
assert_checkequal(ref_values, v);

[v1,ind1] = gsort(abs(c));
[v2,ind2] = gsort(c);
assert_checkequal(ind1, ind2);

A = [18 21 10 7 5];
B = [1  3  22 8 4];
y = complex(A,B);
[a,b] = gsort(y);
assert_checkequal(b, [3 2 1 4 5]);
assert_checkequal(y(b), a);

assert_checkfalse(execstr("[a,b] = gsort(y,''l'')", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: [''g'' ''r'' ''c'' ''lc'' ''lr''] expected.\n"), "gsort", 2);
assert_checkerror("[a,b] = gsort(y,''l'')", refMsg);

ierr = execstr("[a,b] = gsort(y,''g'');","errcatch");
assert_checkequal(ierr, 0);

ierr = execstr("[a,b] = gsort(y,''r'');","errcatch");
assert_checkequal(ierr, 0);

ierr = execstr("[a,b] = gsort(y,''c'');","errcatch");
assert_checkequal(ierr, 0);

ierr = execstr("[a,b] = gsort(y,''lc'');","errcatch");
assert_checkequal(ierr, 0);

ierr = execstr("[a,b] = gsort(y,''lr'');","errcatch");
assert_checkequal(ierr, 0);
//================================ sparse vectors ================================================
sp = sparse([1,2,4,5,3,10]);
ref = sparse([10,5,4,3,2,1]);
A = gsort(sp);
assert_checkequal(ref, A);

sp = sparse([1,2;4,5;3,10]);
assert_checkfalse(execstr("A = gsort(sp)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: sparse vectors expected.\n"), "gsort", 1);
assert_checkerror("A = gsort(sp)", refMsg);
//================================ Matrix of string case ===========================================
N = 4;
P = 20;
a = int(10*rand(N,P,"u"));

//-----Global sort

[a1,ind] = gsort(string(a),"g");
[a2,ind2] = gsort(a,"g");
assert_checkalmostequal(norm(evstr(a1)- matrix(a(ind),N,P)), %eps, [], %eps);

// a or string(a) are in the same order in the previous example
assert_checkalmostequal(norm(evstr(a1)- a2), %eps, [], %eps);

//-- rows
[a1,ind1]=gsort(string(a),"r");
[a2,ind2]=gsort(a,"r");
assert_checkalmostequal(norm(ind1-ind2), %eps, [], %eps);

// a or string(a) are in the same order in the previous example
assert_checkalmostequal(norm(evstr(a1)- a2), %eps, [], %eps);


//--columns
[a1,ind1] = gsort(string(a),"c");
[a2,ind2] = gsort(a,"c");
assert_checkalmostequal(norm(ind1-ind2), %eps, [], %eps);

// a or string(a) are in the same order in the previous example
assert_checkalmostequal(norm(evstr(a1)- a2), %eps, [], %eps);

//----sort the rows of a in lexicographic order

N1 = 4;P1 = 3;
alr = [1,2,2;
1,2,1;
1,1,2;
1,1,1];
// a random permutation
[ax,perm] = gsort(rand(1,N1,"u"));
a = alr(perm,:);
[a1,ind] = gsort(string(a),"lr");
[a2,ind] = gsort(a,"lr") ;

assert_checkalmostequal(norm(evstr(a1)- matrix(a(ind,:),N1,P1)), %eps, [], %eps);

// a or string(a) are in the same order in the previous example
assert_checkalmostequal(norm(evstr(a1)- a2), %eps, [], %eps);


//----sort the columns of a in lexicographic order
N1 = 3;P1 = 4;
alr = alr';
// a random permutation
[ax,perm] = gsort(rand(1,P1,"u"));
a = alr(:,perm);

[a1,ind] = gsort(string(a),"lc");
[a2,ind] = gsort(a,"lc") ;

assert_checkalmostequal(norm(evstr(a1)- matrix(a(:,ind),N1,P1)), %eps, [], %eps);

// a or string(a) are in the same order in the previous example
assert_checkalmostequal(norm(evstr(a1)- a2), %eps, [], %eps);

//================================ Matrix of double case ===========================================
N = 4;P = 20;Nrand = 100;

a = matrix(grand(1,"prm",(1:N*P)'),N,P);

//-----Global sort --------------------------------

[a1,ind]=gsort(a,"g");
assert_checkalmostequal(norm(a1- matrix(N*P:-1:1,N,P)), %eps, [], %eps);
assert_checkalmostequal(norm(a1- matrix(a(ind),N,P)), %eps, [], %eps);

for i=1:Nrand
    b = int(10*rand(N,P,"u"));
    [b1,ind] = gsort(b,"g");
    assert_checkalmostequal(norm(b1- matrix(b(ind),N,P)), %eps, [], %eps);
    assert_checktrue(or(b1(1:$-1)-b1(2:$) >= 0));
end

//increasing values

[a1,ind] = gsort(a,"g","i");
assert_checkalmostequal(norm(a1- matrix(1:N*P,N,P)), %eps, [], %eps);
assert_checkalmostequal(norm(a1- matrix(a(ind),N,P)), %eps, [], %eps);

for i=1:100
    b = int(10*rand(N,P,"u"));
    [b1,ind] = gsort(b,"g","i");
    assert_checkalmostequal(norm(b1- matrix(b(ind),N,P)), %eps, [], %eps);
    assert_checktrue(or(b1(1:$-1)-b1(2:$) <= 0));
end

//----sort each column of a ('r' means that the row indice is used for sorting)

[a1,ind] = gsort(a,"r");
nc = size(a,"c");
test = [];
for i = 1:nc;
    test = [test, matrix(a(ind(:,i),i),N,1)];
end
assert_checkalmostequal(norm(a1- test), %eps, [], %eps);

test = [];
for i = 1:nc;
    test = [test, gsort(a(:,i),"g")];
end
assert_checkalmostequal(norm(a1- test), %eps, [], %eps);

for i = 1:Nrand
    b = int(10*rand(N,P,"u"));
    [b1,ind] = gsort(b,"r");
    assert_checkequal(or(b1(2:$,:) - b1(1:$-1,:) > 0), %f);
    test = [];
    for j = 1:nc
        test = [test, matrix(b(ind(:,j),j),N,1)];
    end
    assert_checkalmostequal(norm(b1- test), %eps, [], %eps);
end

//increasing values

[a1,ind] = gsort(a,"r","i");
nc = size(a,"c");
test = [];
for i = 1:nc
    test = [test, matrix(a(ind(:,i),i),N,1)];
end
assert_checkalmostequal(norm(a1- test), %eps, [], %eps);

test = [];
for i = 1:nc
    test = [test, gsort(a(:,i),"g","i")];
end
assert_checkalmostequal(norm(a1- test), %eps, [], %eps);

//----sort each row of a ('c' means that the column indice is used for sorting)
[a1,ind] = gsort(a,"c");
nr = size(a,"r");
test = [];
for i = 1:nr
    test = [test; matrix(a(i,ind(i,:)),1,P)];
end
assert_checkalmostequal(norm(a1- test), %eps, [], %eps);

test = [];
for i = 1:nr
    test = [test; gsort(a(i,:),"g")];
end
assert_checkalmostequal(norm(a1- test), %eps, [], %eps);

for i = 1:Nrand
    b = int(10*rand(N,P,"u"));
    [b1,ind] = gsort(b,"c");
    assert_checkequal(or(b1(:,2:$) - b1(:,1:$-1) > 0), %f);
    test = [];
    for j = 1:nr
        test = [test; b(j,ind(j,:))];
    end
    assert_checkalmostequal(norm(b1- test), %eps, [], %eps);
end

//increasing
[a1,ind] = gsort(a,"c","i");
nr = size(a,"r");
test = [];
for i = 1:nr
    test = [test; matrix(a(i,ind(i,:)),1,P)];
end
assert_checkalmostequal(norm(a1- test), %eps, [], %eps);

test = [];
for i = 1:nr
    test = [test; gsort(a(i,:),"g","i")];
end
assert_checkalmostequal(norm(a1- test), %eps, [], %eps);


//----sort the rows of a in lexicographic order
//    i.e a(k,:) < a(l,:) if there's a number j
//    such that a(k,j) < a(l,j) or a(k,p)=a(l,p) for p in [1,j-1];
//
vmax = 4;
N1 = 4;P1 = 3;
alr = [1,2,2;
1,2,1;
1,1,2;
1,1,1];
// a random permutation
[ax,perm] = gsort(rand(1, N1, "u"));
a = alr(perm,:);

[a1,ind] = gsort(a,"lr");
assert_checkalmostequal(norm(a1 - alr), %eps, [], %eps);
assert_checkalmostequal(norm(a1 - matrix(a(ind,:),N1,P1)), %eps, [], %eps);

[a2,ind2] = gsort(a*[100;10;1],"g");
assert_checkalmostequal(norm(ind2 - ind), %eps, [], %eps);

for i = 1:Nrand
    b = int(vmax * rand(N,P,"u"));
    [b1,ind] = gsort(b,"lr");
    assert_checkalmostequal(norm(b1 - b(ind,:)), %eps, [], %eps);
    v = b1 * ((vmax + 1) ^ [P - 1 : -1 : 0])';
    assert_checktrue(or(v(2:$) - v(1:$-1) <= 0));
end

// increasing
[a1,ind] = gsort(a,"lr","i");
assert_checkalmostequal(norm(a1 - alr(N1:-1:1,:)), %eps, [], %eps);
assert_checkalmostequal(norm(a1 - matrix(a(ind,:),N1,P1)), %eps, [], %eps);

[a2,ind2] = gsort(a * [100;10;1],"g","i");
assert_checkalmostequal(norm(ind2 - ind), %eps, [], %eps);

for i = 1:Nrand
    b = int(vmax * rand(N,P,"u"));
    [b1,ind] = gsort(b,"lr","i");
    assert_checkalmostequal(norm(b1 - b(ind,:)), %eps, [], %eps);
    v= b1*((vmax+1)^[P-1:-1:0])';
    assert_checktrue(or(v(2:$) - v(1:$-1) >= 0));
end

//----sort the columns of a in lexicographic order
N1 = 3;P1 = 4;
alr = alr';
// a random permutation
[ax,perm] = gsort(rand(1,P1,"u"));
a = alr(:,perm);

[a1,ind] = gsort(a,"lc");
assert_checkalmostequal(norm(a1 - alr), %eps, [], %eps);
assert_checkalmostequal(norm(a1- matrix(a(:,ind),N1,P1)), %eps, [], %eps);

[a2,ind2] = gsort([100,10,1]*a,"g");
assert_checkalmostequal(norm(ind2 - ind), %eps, [], %eps);

for i = 1:Nrand
    b = int(vmax * rand(N1,P1,"u"));
    [b1,ind] = gsort(b,"lc");
    assert_checkalmostequal(norm(b1 - b(:,ind)), %eps, [], %eps);
    v = ((vmax+1)^[N1-1:-1:0])*b1;
    assert_checktrue(or(v(2:$) - v(1:$-1) <= 0));
end

//increasing
[a1,ind] = gsort(a,"lc","i");

assert_checkalmostequal(norm(a1 - alr(:,P1:-1:1)), %eps, [], %eps);
assert_checkalmostequal(norm(a1 - matrix(a(:,ind),N1,P1)), %eps, [], %eps);

[a2,ind2] = gsort([100,10,1]*a,"g","i");
assert_checkalmostequal(norm(ind2 - ind), %eps, [], %eps);

for i = 1:Nrand
    b = int(vmax * rand(N,P,"u"));
    [b1,ind] = gsort(b,"lc","i");
    assert_checkalmostequal(norm(b1 - b(:,ind)), %eps, [], %eps);
    v = ((vmax+1)^[N-1:-1:0])*b1;
    assert_checktrue(or(v(2:$) - v(1:$-1) >= 0));
end

a = [1,1,1,1,2,2,3,3,2,2,3,3,4,4,4,4,4,4,4,4,5,5,5,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,9,9,9,7,9,10,..
10,10,10,11,11,11,12,13,13,13,13,14,14,12,12,14,14,14,14,14,14,15,15,15,15,16,17,18;
3,4,4,5,3,8,4,4,7,8,8,8,5,5,8,9,10,10,9,8,6,10,10,10,10,11,8,8,13,13,9,9,13,13,10,10,13,..
13,14,15,15,12,14,11,11,15,15,15,15,16,17,14,14,17,17,15,15,13,13,17,17,18,18,19,19,16,..
16,19,19,19,18,19]';
[b,ind] = gsort(a,"lr","i");
t = b(2:$,:) - b(1:$-1,:);
assert_checktrue(or(t(:,1) >= 0));
assert_checktrue(or(t(find(t(:,1)==0),2)>=0));
assert_checkequal(a(ind,:), b);

for k = 1:30
    p = grand(1,"prm",(1:size(a,1))');
    [b,ind] = gsort(a(p,:),"lr","i");
    t = b(2:$,:) - b(1:$-1,:);
    assert_checktrue(or(t(:,1) >= 0));
    assert_checktrue(or(t(find(t(:,1)==0),2)>=0));
    assert_checkequal(a(p(ind),:), b);
end

[b,ind] = gsort(a,"lr","d");
t = b(1:$-1,:) - b(2:$,:);
assert_checktrue(or(t(:,1) >= 0));
assert_checktrue(or(t(find(t(:,1)==0),2)>=0));
assert_checkequal(a(ind,:), b);

for k = 1:30
    p = grand(1,"prm",(1:size(a,1))');
    [b,ind] = gsort(a(p,:),"lr","d");
    t = b(1:$-1,:) - b(2:$,:);
    assert_checktrue(or(t(:,1) >= 0));
    assert_checktrue(or(t(find(t(:,1)==0),2)>=0));
    assert_checkequal(a(p(ind),:), b);
end

a = b;
a([10 60],:) = a([60 10],:);
[b,ind] = gsort(a,"lr","i");
t = b(2:$,:) - b(1:$-1,:);
assert_checktrue(or(t(:,1) >= 0));
assert_checktrue(or(t(find(t(:,1)==0),2)>=0));
assert_checkequal(a(ind,:), b);

[b,ind] = gsort(a,"lr","d");
t = b(1:$-1,:) - b(2:$,:);
assert_checktrue(or(t(:,1) >= 0));
assert_checktrue(or(t(find(t(:,1)==0),2)>=0));
assert_checkequal(a(ind,:), b);
//================================ Matrix of int32 or uint32 case =======================================
N = 4;P = 20;Nrand = 100; vmax = 4;

a = int32(matrix(grand(1,"prm",(1:N*P)'),N,P));

//-----Global sort --------------------------------
[a1,ind] = gsort(a,"g");
assert_checkequal(a1, int32(matrix(N * P:-1:1,N,P)));
assert_checkequal(a1, matrix(a(ind),N,P));

for i = 1:Nrand
    b = int32(10 * rand(N,P,"u"));
    [b1,ind] = gsort(b,"g");
    assert_checkequal(b1, matrix(b(ind),N,P));
    assert_checktrue(or(b1(1:$-1) - b1(2:$) >= 0));
end

//increasing values
[a1,ind] = gsort(a,"g","i");
assert_checkequal(a1, int32(matrix(1:N*P,N,P)));
assert_checkequal(a1, matrix(a(ind),N,P));

for i = 1:100
    b = int32(10*rand(N,P,"u"));
    [b1,ind]=gsort(b,"g","i");
    assert_checkequal(b1, matrix(b(ind),N,P));
    assert_checktrue(or(b1(1:$-1) - b1(2:$) <= 0));
end

//----sort each column of a ('r' means that the row indice is used for sorting)
[a1,ind] = gsort(a,"r");
nc = size(a,"c");
test = [];
for i = 1:nc
    test = [test, matrix(a(ind(:,i),i),N,1)];
end
assert_checkequal(a1, test);

test = [];
for i =1:nc
    test = [test, gsort(a(:,i),"g")];
end
assert_checkequal(a1, test);

for i = 1:Nrand
    b = int32(10*rand(N,P,"u"));
    [b1,ind] = gsort(b,"r");
    assert_checkequal(or(b1(2:$,:) - b1(1:$-1,:) > 0), %f);
    test = [];
    for j = 1:nc
        test = [test, matrix(b(ind(:,j),j),N,1)];
    end
    assert_checkequal(b1, test);
end

//increasing values
[a1,ind] = gsort(a,"r","i");
nc = size(a,"c");
test = [];
for i = 1:nc
    test = [test, matrix(a(ind(:,i),i),N,1)];
end
assert_checkequal(a1, test);

test = [];
for i = 1:nc
    test = [test, gsort(a(:,i),"g","i")];
end
assert_checkequal(a1, test);

//----sort each row of a ('c' means that the column indice is used for sorting)
[a1,ind] = gsort(a,"c");
nr = size(a,"r");
test = [];
for i = 1:nr
    test = [test; matrix(a(i,ind(i,:)),1,P)];
end
assert_checkequal(a1, test);

test = [];
for i = 1:nr ;
    test = [test; gsort(a(i,:),"g")];
end
assert_checkequal(a1, test);

for i = 1:Nrand
    b = int32(10 * rand(N,P,"u"));
    [b1,ind] = gsort(b,"c");
    assert_checkequal(or(b1(:,2:$) - b1(:,1:$-1) > 0), %f);
    test = [];
    for j = 1:nr
        test = [test; b(j,ind(j,:))];
    end
    assert_checkequal(b1, test);
end

//increasing
[a1,ind] = gsort(a,"c","i");
nr = size(a,"r");
test = [];
for i = 1:nr
    test = [test; matrix(a(i,ind(i,:)),1,P)];
end
assert_checkequal(a1, test);
test = [];
for i = 1:nr
    test = [test; gsort(a(i,:),"g","i")];
end
assert_checkequal(a1, test);

//----sort the rows of a in lexicographic order
//    i.e a(k,:) < a(l,:) if there's a number j
//    such that a(k,j) < a(l,j) or a(k,p)=a(l,p) for p in [1,j-1];
//
N1=4;P1=3;
alr=[1,2,2;
1,2,1;
1,1,2;
1,1,1];
// a random permutation
[ax,perm] = gsort(rand(1,N1,"u"));
a = int32(alr(perm,:));

[a1,ind] = gsort(a,"lr");

assert_checkequal(a1, int32(alr));
assert_checkequal(a1, matrix(a(ind,:),N1,P1));

[a2,ind2] = gsort(a*[100;10;1],"g");
assert_checkequal(ind2, ind);
///////////////////////
for i = 1:Nrand
    b = int32(vmax*rand(N,P,"u"));
    [b1,ind] = gsort(b,"lr");
    assert_checkequal(b1, b(ind,:));
    v = double(b1)*((vmax+1)^[P-1:-1:0])';
    assert_checktrue(or(v(2:$) - v(1:$-1) <= 0));
end

// increasing
[a1,ind] = gsort(a,"lr","i");
assert_checkequal(a1, int32(alr(N1:-1:1,:)));
assert_checkequal(a1, matrix(a(ind,:),N1,P1));

[a2,ind2] = gsort(a*[100;10;1],"g","i");
assert_checkequal(ind2, ind);

for i = 1:Nrand
    b = int(vmax * rand(N,P,"u"));
    [b1,ind] = gsort(b,"lr","i");
    assert_checkequal(b1, b(ind,:));
    v = double(b1)*((vmax+1)^[P-1:-1:0])';
    assert_checktrue(or(v(2:$) - v(1:$-1) >= 0));
end

//----sort the columns of a in lexicographic order
N1 = 3;P1 = 4;
alr = alr';
// a random permutation
[ax,perm] = gsort(rand(1,P1,"u"));
a = int32(alr(:,perm));

[a1,ind] = gsort(a,"lc");
assert_checkequal(a1, int32(alr));
assert_checkequal(a1, matrix(a(:,ind),N1,P1));

[a2,ind2] = gsort([100,10,1]*a,"g");
assert_checkequal(ind2, ind);

for i = 1:Nrand
    b = int(vmax*rand(N1,P1,"u"));
    [b1,ind] = gsort(b,"lc");
    assert_checkequal(b1, b(:,ind));
    v= ((vmax+1)^[N1-1:-1:0])*double(b1);
    assert_checktrue(or(v(2:$) - v(1:$-1) <= 0));
end

//increasing
[a1,ind] = gsort(a,"lc","i");
assert_checkequal(a1, int32(alr(:,P1:-1:1)));
assert_checkequal(a1, matrix(a(:,ind),N1,P1));

[a2,ind2] = gsort([100,10,1] * a,"g","i");
assert_checkequal(ind2, ind);

for i = 1:Nrand
    b = int(vmax*rand(N,P,"u"));
    [b1,ind] = gsort(b,"lc","i");
    assert_checkequal(b1, b(:,ind));
    v = ((vmax+1)^[N-1:-1:0])*b1;
    assert_checktrue(or(v(2:$) - v(1:$-1) >= 0));
end

a = int32([1,1,1,1,2,2,3,3,2,2,3,3,4,4,4,4,4,4,4,4,5,5,5,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,9,9,9,7,9,10,..
10,10,10,11,11,11,12,13,13,13,13,14,14,12,12,14,14,14,14,14,14,15,15,15,15,16,17,18;
3,4,4,5,3,8,4,4,7,8,8,8,5,5,8,9,10,10,9,8,6,10,10,10,10,11,8,8,13,13,9,9,13,13,10,10,13,..
13,14,15,15,12,14,11,11,15,15,15,15,16,17,14,14,17,17,15,15,13,13,17,17,18,18,19,19,16,..
16,19,19,19,18,19])';
[b,ind] = gsort(a,"lr","i");
t = b(2:$,:) - b(1:$-1,:);
assert_checktrue(or(t(:,1) >= 0));
assert_checktrue(or(t(find(t(:,1)==0),2) >= 0));
assert_checkequal(a(ind,:), b);

for k = 1:30
    p = grand(1,"prm",(1:size(a,1))');
    [b,ind] = gsort(a(p,:),"lr","i");
    t = b(2:$,:) - b(1:$-1,:);
    assert_checktrue(or(t(:,1) >= 0));
    assert_checktrue(or(t(find(t(:,1)==0),2) >= 0));
    assert_checkequal(a(p(ind),:), b);
end

[b,ind] = gsort(a,"lr","d");
t = b(1:$-1,:) - b(2:$,:);
assert_checktrue(or(t(:,1) >= 0));
assert_checktrue(or(t(find(t(:,1)==0),2) >= 0));
assert_checkequal(a(ind,:), b);

for k = 1:30
    p = grand(1,"prm",(1:size(a,1))');
    [b,ind] = gsort(a(p,:),"lr","d");
    t = b(1:$-1,:) - b(2:$,:);
    assert_checktrue(or(t(:,1) >= 0));
    assert_checktrue(or(t(find(t(:,1)==0),2) >= 0));
    assert_checkequal(a(p(ind),:), b);
end

a = b;
a([10 60],:) = a([60 10],:);
[b,ind] = gsort(a,"lr","i");
t = b(2:$,:) - b(1:$-1,:);
assert_checktrue(or(t(:,1) >= 0));
assert_checktrue(or(t(find(t(:,1)==0),2) >= 0));
assert_checkequal(a(ind,:), b);

[b,ind] = gsort(a,"lr","d");
t = b(1:$-1,:) - b(2:$,:);
assert_checktrue(or(t(:,1) >= 0));
assert_checktrue(or(t(find(t(:,1)==0),2) >= 0));
assert_checkequal(a(ind,:), b);

//================================ Matrix of int16 or uint16 case =======================================
// Copyright Inria
// test of gsort for int16 matrices:
N = 4;P = 20;Nrand = 100; vmax = 4;

a = int16(matrix(grand(1,"prm",(1:N*P)'),N,P));

//-----Global sort --------------------------------
[a1,ind] = gsort(a,"g");
assert_checkequal(a1, int16(matrix(N*P:-1:1,N,P)));
assert_checkequal(a1, matrix(a(ind),N,P));

for i = 1:Nrand
    b = int16(10*rand(N,P,"u"));
    [b1,ind]=gsort(b,"g");
    assert_checkequal(b1, matrix(b(ind),N,P));
    assert_checktrue(or(b1(1:$-1) - b1(2:$) >= 0));
end

//increasing values
[a1,ind] = gsort(a,"g","i");
assert_checkequal(a1, int16(matrix(1:N*P,N,P)));
assert_checkequal(a1, matrix(a(ind),N,P));

for i = 1:100
    b = int16(10*rand(N,P,"u"));
    [b1,ind] = gsort(b,"g","i");
    assert_checkequal(b1, int16(matrix(b(ind),N,P)));
    assert_checktrue(or(b1(1:$-1) - b1(2:$) <= 0));
end

//----sort each column of a ('r' means that the row indice is used for sorting)
[a1,ind] = gsort(a,"r");
nc = size(a,"c");
test = [];
for i = 1:nc
    test = [test, matrix(a(ind(:,i),i),N,1)];
end
assert_checkequal(a1, test);

test = [];
for i = 1:nc
    test = [test, gsort(a(:,i),"g")];
end
assert_checkequal(a1, test);

for i = 1:Nrand
    b = int16(10*rand(N,P,"u"));
    [b1,ind] = gsort(b,"r");
    assert_checktrue(or( b1(2:$,:) - b1(1:$-1,:) <= 0));
    test = [];
    for j = 1:nc
        test = [test, matrix(b(ind(:,j),j),N,1)];
    end
    assert_checkequal(b1, test);
end

//increasing values
[a1,ind] = gsort(a,"r","i");
nc = size(a,"c");
test = [];
for i = 1:nc
    test = [test, matrix(a(ind(:,i),i),N,1)];
end
assert_checkequal(a1, test);

test =[];
for i = 1:nc ;
    test = [test, gsort(a(:,i),"g","i")];
end
assert_checkequal(a1, test);

//----sort each row of a ('c' means that the column indice is used for sorting)
[a1,ind] = gsort(a,"c")   ;
nr = size(a,"r");
test = [];
for i = 1:nr
    test = [test; matrix(a(i,ind(i,:)),1,P)];
end
assert_checkequal(a1, test);

test = [];
for i = 1:nr
    test = [test; gsort(a(i,:),"g")];
end
assert_checkequal(a1, test);

for i = 1:Nrand
    b = int16(10*rand(N,P,"u"));
    [b1,ind] = gsort(b,"c");
    assert_checktrue(or(b1(:,2:$) - b1(:,1:$-1) <= 0));
    test = [];
    for j = 1:nr
        test = [test; b(j,ind(j,:))];
    end
    assert_checkequal(b1, test);
end

//increasing
[a1,ind] = gsort(a,"c","i");
nr = size(a,"r");
test = [];
for i = 1:nr
    test = [test; matrix(a(i,ind(i,:)),1,P)];
end
assert_checkequal(a1, test);

test = [];
for i = 1:nr ;
    test = [test; gsort(a(i,:),"g","i")];
end
assert_checkequal(a1, test);

//----sort the rows of a in lexicographic order
//    i.e a(k,:) < a(l,:) if there's a number j
//    such that a(k,j) < a(l,j) or a(k,p)=a(l,p) for p in [1,j-1];
//
N1 = 4;P1 = 3;
alr = [1,2,2;
1,2,1;
1,1,2;
1,1,1];
// a random permutation
[ax,perm] = gsort(rand(1,N1,"u"));
a = int16(alr(perm,:));

[a1,ind] = gsort(a,"lr");

assert_checkequal(a1, int16(alr));
assert_checkequal(a1, matrix(a(ind,:),N1,P1));

[a2,ind2]=gsort(a*[100;10;1],"g");
assert_checkequal(ind2, ind);
///////////////////////
for i = 1:Nrand
    b = int16(vmax * rand(N,P,"u"));
    [b1,ind] = gsort(b,"lr");
    assert_checkequal(b1, b(ind,:));
    v= double(b1)*((vmax+1)^[P-1:-1:0])';
    assert_checktrue(or(v(2:$) - v(1:$-1) <= 0));
end

// increasing
[a1,ind] = gsort(a,"lr","i");
assert_checkequal(a1, int16(alr(N1:-1:1,:)));
assert_checkequal(a1, matrix(a(ind,:),N1,P1));

[a2,ind2] = gsort(a*[100;10;1],"g","i");
assert_checkequal(ind2, ind);

for i = 1:Nrand
    b = int(vmax*rand(N,P,"u"));
    [b1,ind] = gsort(b,"lr","i");
    assert_checkequal(b1, b(ind,:));
    v = double(b1)*((vmax+1)^[P-1:-1:0])';
    assert_checktrue(or(v(2:$) - v(1:$-1) >= 0));
end

//----sort the columns of a in lexicographic order
N1 = 3;P1 = 4;
alr = alr';
// a random permutation
[ax,perm] = gsort(rand(1,P1,"u"));
a = int16(alr(:,perm));

[a1,ind] = gsort(a,"lc");
assert_checkequal(a1, int16(alr));
assert_checkequal(a1, matrix(a(:,ind),N1,P1));

[a2,ind2] = gsort([100,10,1]*a,"g");
assert_checkequal(ind2, ind);

for i = 1:Nrand
    b = int(vmax*rand(N1,P1,"u"));
    [b1,ind] = gsort(b,"lc");
    assert_checkequal(b1, b(:,ind));
    v= ((vmax+1)^[N1-1:-1:0])*double(b1);
    assert_checktrue(or(v(2:$) - v(1:$-1) <= 0));
end

//increasing
[a1,ind] = gsort(a,"lc","i");

assert_checkequal(a1, int16(alr(:,P1:-1:1)));
assert_checkequal(a1, matrix(a(:,ind),N1,P1));

[a2,ind2] = gsort([100,10,1]*a,"g","i");
assert_checkequal(ind2, ind);

for i = 1:Nrand
    b = int(vmax*rand(N,P,"u"));
    [b1,ind] = gsort(b,"lc","i");
    assert_checkequal(b1, b(:,ind));
    v = ((vmax+1)^[N-1:-1:0])*b1;
    assert_checktrue(or(v(2:$) - v(1:$-1) >= 0));
end

a = int16([1,1,1,1,2,2,3,3,2,2,3,3,4,4,4,4,4,4,4,4,5,5,5,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,9,9,9,7,9,10,..
10,10,10,11,11,11,12,13,13,13,13,14,14,12,12,14,14,14,14,14,14,15,15,15,15,16,17,18;
3,4,4,5,3,8,4,4,7,8,8,8,5,5,8,9,10,10,9,8,6,10,10,10,10,11,8,8,13,13,9,9,13,13,10,10,13,..
13,14,15,15,12,14,11,11,15,15,15,15,16,17,14,14,17,17,15,15,13,13,17,17,18,18,19,19,16,..
16,19,19,19,18,19])';
[b,ind] = gsort(a,"lr","i");
t = b(2:$,:) - b(1:$-1,:);
assert_checktrue(or(t(:,1) >= 0));
assert_checktrue(or(t(find(t(:,1)==0),2) >= 0));
assert_checkequal(a(ind,:), b);

for k = 1:30
    p = grand(1,"prm",(1:size(a,1))');
    [b,ind] = gsort(a(p,:),"lr","i");
    t = b(2:$,:) - b(1:$-1,:);
    assert_checktrue(or(t(:,1) >= 0));
    assert_checktrue(or(t(find(t(:,1)==0),2) >= 0));
    assert_checkequal(a(p(ind),:), b);
end

[b,ind] = gsort(a,"lr","d");
t = b(1:$-1,:) - b(2:$,:);
assert_checktrue(or(t(:,1) >= 0));
assert_checktrue(or(t(find(t(:,1)==0),2) >= 0));
assert_checkequal(a(ind,:), b);

for k = 1:30
    p = grand(1,"prm",(1:size(a,1))');
    [b,ind] = gsort(a(p,:),"lr","d");
    t = b(1:$-1,:) - b(2:$,:);
    assert_checktrue(or(t(:,1) >= 0));
    assert_checktrue(or(t(find(t(:,1)==0),2) >= 0));
    assert_checkequal(a(p(ind),:), b);
end

a = b;
a([10 60],:) = a([60 10],:);
[b,ind] = gsort(a,"lr","i");
t = b(2:$,:) - b(1:$-1,:);
assert_checktrue(or(t(:,1) >= 0));
assert_checktrue(or(t(find(t(:,1)==0),2) >= 0));
assert_checkequal(a(ind,:), b);

[b,ind] = gsort(a,"lr","d");
t = b(1:$-1,:) - b(2:$,:);
assert_checktrue(or(t(:,1) >= 0));
assert_checktrue(or(t(find(t(:,1)==0),2) >= 0));
assert_checkequal(a(ind,:), b);

a = uint16([1,1,1,1,2,2,3,3,2,2,3,3,4,4,4,4,4,4,4,4,5,5,5,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,9,9,9,7,9,10,..
10,10,10,11,11,11,12,13,13,13,13,14,14,12,12,14,14,14,14,14,14,15,15,15,15,16,17,18;
3,4,4,5,3,8,4,4,7,8,8,8,5,5,8,9,10,10,9,8,6,10,10,10,10,11,8,8,13,13,9,9,13,13,10,10,13,..
13,14,15,15,12,14,11,11,15,15,15,15,16,17,14,14,17,17,15,15,13,13,17,17,18,18,19,19,16,..
16,19,19,19,18,19]-5)';
[b,ind] = gsort(a,"lr","i");
t = b(2:$,:) < b(1:$-1,:);
assert_checkfalse(or(t(:,1)));
assert_checkfalse(or(t(b(2:$,1)==b(1:$-1,1),2)));
assert_checkequal(a(ind,:), b);

for k = 1:30
    p = grand(1,"prm",(1:size(a,1))');
    [b,ind] = gsort(a(p,:),"lr","i");
    t = b(2:$,:) < b(1:$-1,:);
    assert_checkfalse(or(t(:,1)));
    assert_checkfalse(or(t(b(2:$,1)==b(1:$-1,1),2)));
    assert_checkequal(a(p(ind),:), b);
end

[b,ind] = gsort(a,"lr","d");
t = b(2:$,:) > b(1:$-1,:);
assert_checkfalse(or(t(:,1)));
assert_checkfalse(or(t(b(2:$,1)==b(1:$-1,1),2)));
assert_checkequal(a(ind,:), b);

for k = 1:30
    p = grand(1,"prm",(1:size(a,1))');
    [b,ind] = gsort(a(p,:),"lr","d");
    t = b(2:$,:) > b(1:$-1,:);
    assert_checkfalse(or(t(:,1)));
    assert_checkfalse(or(t(b(2:$,1)==b(1:$-1,1),2)));
    assert_checkequal(a(p(ind),:), b);
end

a = b;
a([10 60],:) = a([60 10],:);
[b,ind] = gsort(a,"lr","i");
t = b(2:$,:) < b(1:$-1,:);
assert_checkfalse(or(t(:,1)));
assert_checkfalse(or(t(b(2:$,1)==b(1:$-1,1),2)));
assert_checkequal(a(ind,:), b);

[b,ind] = gsort(a,"lr","d");
t = b(2:$,:) > b(1:$-1,:);
assert_checkfalse(or(t(:,1)));
assert_checkfalse(or(t(b(2:$,1)==b(1:$-1,1),2)));
assert_checkequal(a(ind,:), b);

//================================ Matrix of int8 or uint8 case =======================================
// test of gsort for int8 matrices:
N = 4;P = 20;Nrand = 100; vmax = 4;

a = int8(matrix(grand(1,"prm",(1:N*P)'),N,P));

//-----Global sort --------------------------------
[a1,ind] = gsort(a,"g");
assert_checkequal(a1, int8(matrix(N*P:-1:1,N,P)));
assert_checkequal(a1, matrix(a(ind),N,P));

for i = 1:Nrand
    b = int8(10*rand(N,P,"u"));
    [b1,ind] = gsort(b,"g");
    assert_checkequal(b1, matrix(b(ind),N,P));
    assert_checktrue(or(b1(1:$-1) - b1(2:$) >= 0));
end

//increasing values
[a1,ind] = gsort(a,"g","i");

assert_checkequal(a1, int8(matrix(1:N*P,N,P)));
assert_checkequal(a1, matrix(a(ind),N,P));

for i = 1:Nrand
    b = int8(10*rand(N,P,"u"));
    [b1,ind] = gsort(b,"g","i");
    assert_checkequal(b1, matrix(b(ind),N,P));
    assert_checktrue(or(b1(1:$-1) - b1(2:$) <= 0));
end

//----sort each column of a ('r' means that the row indice is used for sorting)
[a1,ind] = gsort(a,"r");
nc = size(a,"c");
test = [];
for i = 1:nc
    test = [test, matrix(a(ind(:,i),i),N,1)];
end
assert_checkequal(a1, test);
test = [];
for i = 1:nc
    test = [test, gsort(a(:,i),"g")];
end
assert_checkequal(a1, test);

for i = 1:Nrand
    b = int8(10*rand(N,P,"u"));
    [b1,ind] = gsort(b,"r");
    assert_checkequal(or(b1(2:$,:) - b1(1:$-1,:) > 0), %f);
    test = [];
    for j = 1:nc
        test = [test, matrix(b(ind(:,j),j),N,1)];
    end
    assert_checkequal(b1, test);
end

//increasing values
[a1,ind] = gsort(a,"r","i");
nc = size(a,"c");
test = [];
for i = 1:nc
    test = [test, matrix(a(ind(:,i),i),N,1)];
end
assert_checkequal(a1, test);

test =[];
for i = 1:nc ;
    test = [test, gsort(a(:,i),"g","i")];
end
assert_checkequal(a1, int8(test));

//----sort each row of a ('c' means that the column indice is used for sorting)
[a1,ind] = gsort(a,"c");
nr = size(a,"r");
test = [];
for i = 1:nr
    test = [test; matrix(a(i,ind(i,:)),1,P)];
end
assert_checkequal(a1, test);

test = [];
for i = 1:nr
    test = [test; gsort(a(i,:),"g")];
end
assert_checkequal(a1, test);

for i = 1:Nrand
    b = int8(10*rand(N,P,"u"));
    [b1,ind] = gsort(b,"c");
    assert_checktrue(or(b1(:,2:$) - b1(:,1:$-1) <= 0));
    test = [];
    for j = 1:nr
        test = [test; b(j,ind(j,:))];
    end
    assert_checkequal(b1, test);
end

//increasing
[a1,ind] = gsort(a,"c","i");
nr = size(a,"r");
test = [];
for i = 1:nr
    test = [test; matrix(a(i,ind(i,:)),1,P)];
end
assert_checkequal(a1, test);
test = [];
for i = 1:nr ;
    test = [test; gsort(a(i,:),"g","i")];
end
assert_checkequal(a1, test);

//----sort the rows of a in lexicographic order
//    i.e a(k,:) < a(l,:) if there's a number j
//    such that a(k,j) < a(l,j) or a(k,p)=a(l,p) for p in [1,j-1];
//
N1 = 4;P1 = 3;
alr = [1,2,2;
1,2,1;
1,1,2;
1,1,1];
// a random permutation
[ax,perm] = gsort(rand(1,N1,"u"));
a = int8(alr(perm,:));

[a1,ind] = gsort(a,"lr");
assert_checkequal(a1, int8(alr));
assert_checkequal(a1, matrix(a(ind,:),N1,P1));

[a2,ind2] = gsort(a*[100;10;1],"g");
assert_checkequal(ind2, ind);
///////////////////////
for i = 1:Nrand
    b = int8(vmax*rand(N,P,"u"));
    [b1,ind] = gsort(b,"lr");
    assert_checkequal(b1, b(ind,:));
    v= double(b1)*((vmax+1)^[P-1:-1:0])';
    assert_checktrue(or(v(2:$) - v(1:$-1) <= 0));
end

// increasing
[a1,ind] = gsort(a,"lr","i");
assert_checkequal(a1, int8(alr(N1:-1:1,:)));
assert_checkequal(a1, matrix(a(ind,:),N1,P1));

[a2,ind2]=gsort(a*[100;10;1],"g","i");
assert_checkequal(ind2, ind);

for i = 1:Nrand
    b = int(vmax*rand(N,P,"u"));
    [b1,ind] = gsort(b,"lr","i");
    assert_checkequal(b1, b(ind,:));
    v = double(b1)*((vmax+1)^[P-1:-1:0])';
    assert_checktrue(or(v(2:$) - v(1:$-1) >= 0));
end

//----sort the columns of a in lexicographic order
N1 = 3;P1 = 4;
alr = alr';
// a random permutation
[ax,perm] = gsort(rand(1,P1,"u"));
a = int8(alr(:,perm));

[a1,ind] = gsort(a,"lc");
assert_checkequal(a1, int8(alr));
assert_checkequal(a1, matrix(a(:,ind),N1,P1));

[a2,ind2]=gsort([100,10,1]*a,"g");
assert_checkequal(ind2, ind);

for i = 1:Nrand
    b = int(vmax*rand(N1,P1,"u"));
    [b1,ind] = gsort(b,"lc");
    assert_checkequal(b1, b(:,ind));
    v= ((vmax+1)^[N1-1:-1:0])*double(b1);
    assert_checktrue(or(v(2:$) - v(1:$-1) <= 0));
end

//increasing
[a1,ind] = gsort(a,"lc","i");

assert_checkequal(a1, int8(alr(:,P1:-1:1)));
assert_checkequal(a1, matrix(a(:,ind),N1,P1));

[a2,ind2]=gsort([100,10,1]*a,"g","i");
assert_checkequal(ind2, ind);

for i = 1:Nrand
    b = int(vmax*rand(N,P,"u"));
    [b1,ind] = gsort(b,"lc","i");
    assert_checkequal(b1, b(:,ind));
    v= ((vmax+1)^[N-1:-1:0])*b1;
    assert_checktrue(or(v(2:$) - v(1:$-1) >= 0));
end

a = int8([1,1,1,1,2,2,3,3,2,2,3,3,4,4,4,4,4,4,4,4,5,5,5,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,9,9,9,7,9,10,..
10,10,10,11,11,11,12,13,13,13,13,14,14,12,12,14,14,14,14,14,14,15,15,15,15,16,17,18;
3,4,4,5,3,8,4,4,7,8,8,8,5,5,8,9,10,10,9,8,6,10,10,10,10,11,8,8,13,13,9,9,13,13,10,10,13,..
13,14,15,15,12,14,11,11,15,15,15,15,16,17,14,14,17,17,15,15,13,13,17,17,18,18,19,19,16,..
16,19,19,19,18,19])';
[b,ind] = gsort(a,"lr","i");
t = b(2:$,:) - b(1:$-1,:);
assert_checktrue(or(t(:,1) <= 0));
assert_checktrue(or(t(find(t(:,1)==0),2)>=0));
assert_checkequal(a(ind,:), b);

for k = 1:30
    p = grand(1,"prm",(1:size(a,1))');
    [b,ind] = gsort(a(p,:),"lr","i");
    t = b(2:$,:) - b(1:$-1,:);
    assert_checktrue(or(t(:,1) <= 0));
    assert_checktrue(or(t(find(t(:,1)==0),2)>=0));
    assert_checkequal(a(p(ind),:), b);
end

[b,ind]=gsort(a,"lr","d");
t=b(1:$-1,:)-b(2:$,:);
assert_checktrue(or(t(:,1) <= 0));
assert_checktrue(or(t(find(t(:,1)==0),2)>=0));
assert_checkequal(a(ind,:), b);

for k = 1:30
    p = grand(1,"prm",(1:size(a,1))');
    [b,ind] = gsort(a(p,:),"lr","d");
    t = b(1:$-1,:) - b(2:$,:);
    assert_checktrue(or(t(:,1) <= 0));
    assert_checktrue(or(t(find(t(:,1)==0),2)>=0));
    assert_checkequal(a(p(ind),:), b);
end

a = b;
a([10 60],:) = a([60 10],:);
[b,ind] = gsort(a,"lr","i");
t = b(2:$,:) - b(1:$-1,:);
assert_checktrue(or(t(:,1) <= 0));
assert_checktrue(or(t(find(t(:,1)==0),2)>=0));


assert_checkequal(a(ind,:), b);

[b,ind] = gsort(a,"lr","d");
t = b(1:$-1,:) - b(2:$,:);
assert_checktrue(or(t(:,1) <= 0));
assert_checktrue(or(t(find(t(:,1)==0),2)>=0));
assert_checkequal(a(ind,:), b);

N = 4;P = 20;Nrand = 100; vmax = 4;

a = uint32(matrix(grand(1,"prm",(1:N*P)'),N,P));

//-----Global sort --------------------------------

[a1,ind] = gsort(a,"g");
assert_checkequal(a1, uint32(matrix(N*P:-1:1,N,P)));
assert_checkequal(a1, matrix(a(ind),N,P));

for i = 1:Nrand
    b = uint32(10*rand(N,P,"u"));
    [b1,ind] = gsort(b,"g");
    assert_checkequal(b1, matrix(b(ind),N,P));
    assert_checktrue(or(b1(1:$-1) - b1(2:$) >= 0));
end

//increasing values
[a1,ind] = gsort(a,"g","i");
assert_checkequal(a1, uint32(matrix(1:N*P,N,P)));
assert_checkequal(a1, matrix(a(ind),N,P));

//----sort each column of a ('r' means that the row indice is used for sorting)
[a1,ind] = gsort(a,"r");
nc = size(a,"c");
test = [];
for i = 1:nc ;
    test = [test, matrix(a(ind(:,i),i),N,1)];
end
assert_checkequal(a1, test);
test = [];
for i = 1:nc ;
    test = [test, gsort(a(:,i),"g")];
end
assert_checkequal(a1, test);

//increasing values
[a1,ind] = gsort(a,"r","i");
nc = size(a,"c");
test = [];
for i = 1:nc ;
    test = [test, matrix(a(ind(:,i),i),N,1)];
end
assert_checkequal(a1, test);

test = [];
for i = 1:nc ;
    test = [test, gsort(a(:,i),"g","i")];
end
assert_checkequal(a1, test);

//----sort each row of a ('c' means that the column indice is used for sorting)
[a1,ind] = gsort(a,"c");
nr = size(a,"r");
test = [];
for i = 1:nr
    test = [test; matrix(a(i,ind(i,:)),1,P)];
end
assert_checkequal(a1, test);

test = [];
for i = 1:nr
    test = [test; gsort(a(i,:),"g")];
end
assert_checkequal(a1, test);

//increasing
[a1,ind] = gsort(a,"c","i");
nr = size(a,"r");
test = [];
for i = 1:nr
    test = [test; matrix(a(i,ind(i,:)),1,P)];
end
assert_checkequal(a1, test);
test = [];
for i = 1:nr
    test = [test; gsort(a(i,:),"g","i")];
end
assert_checkequal(a1, test);

//----sort the rows of a in lexicographic order
//    i.e a(k,:) < a(l,:) if there's a number j
//    such that a(k,j) < a(l,j) or a(k,p)=a(l,p) for p in [1,j-1];
//
N1 = 4;P1 = 3;
alr = [1,2,2;
1,2,1;
1,1,2;
1,1,1];
// a random permutation
[ax,perm] = gsort(rand(1,N1,"u"));
a = uint32(alr(perm,:));

[a1,ind] = gsort(a,"lr");
assert_checkequal(a1, uint32(alr));
assert_checkequal(a1, matrix(a(ind,:),N1,P1));

[a2,ind2]=gsort(a*[100;10;1],"g");
assert_checkequal(ind2, ind);
///////////////////////
for i = 1:Nrand
    b = uint32(vmax*rand(N,P,"u"));
    [b1,ind] = gsort(b,"lr");
    assert_checkequal(b1, b(ind,:));
    v = double(b1)*((vmax+1)^[P-1:-1:0])';
    assert_checktrue(or(v(2:$)-v(1:$-1) <= 0));
end

// increasing
[a1,ind] = gsort(a,"lr","i");
assert_checkequal(a1, uint32(alr(N1:-1:1,:)));
assert_checkequal(a1, matrix(a(ind,:),N1,P1));

[a2,ind2]=gsort(a*[100;10;1],"g","i");
assert_checkequal(ind2, ind);

for i = 1:Nrand
    b = int(vmax*rand(N,P,"u"));
    [b1,ind] = gsort(b,"lr","i");
    assert_checkequal(b1, b(ind,:));
    v= double(b1)*((vmax+1)^[P-1:-1:0])';
    assert_checktrue(or(v(2:$)-v(1:$-1) >= 0));
end

//----sort the columns of a in lexicographic order
N1 = 3;P1 = 4;
alr = alr';
// a random permutation
[ax,perm] = gsort(rand(1,P1,"u"));
a = uint32(alr(:,perm));

[a1,ind] = gsort(a,"lc");
assert_checkequal(a1, uint32(alr));
assert_checkequal(a1, matrix(a(:,ind),N1,P1));

[a2,ind2] = gsort([100,10,1]*a,"g");
assert_checkequal(ind2, ind);

for i = 1:Nrand
    b = int(vmax * rand(N1,P1,"u"));
    [b1,ind] = gsort(b,"lc");
    assert_checkequal(b1, b(:,ind));
    v= ((vmax+1)^[N1-1:-1:0])*double(b1);
    assert_checktrue(or(v(2:$)-v(1:$-1) <= 0));
end

//increasing
[a1,ind] = gsort(a,"lc","i");
assert_checkequal(a1, uint32(alr(:,P1:-1:1)));
assert_checkequal(a1, matrix(a(:,ind),N1,P1));

[a2,ind2] = gsort([100,10,1] * a,"g","i");
assert_checkequal(ind2, ind);

for i = 1:Nrand
    b = int(vmax * rand(N,P,"u"));
    [b1,ind] = gsort(b,"lc","i");
    assert_checkequal(b1, b(:,ind));
    v= ((vmax+1)^[N-1:-1:0])*b1;
    assert_checktrue(or(v(2:$)-v(1:$-1) >= 0));
end

a = uint32([1,1,1,1,2,2,3,3,2,2,3,3,4,4,4,4,4,4,4,4,5,5,5,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,9,9,9,7,9,10,..
10,10,10,11,11,11,12,13,13,13,13,14,14,12,12,14,14,14,14,14,14,15,15,15,15,16,17,18;
3,4,4,5,3,8,4,4,7,8,8,8,5,5,8,9,10,10,9,8,6,10,10,10,10,11,8,8,13,13,9,9,13,13,10,10,13,..
13,14,15,15,12,14,11,11,15,15,15,15,16,17,14,14,17,17,15,15,13,13,17,17,18,18,19,19,16,..
16,19,19,19,18,19]-5)';
[b,ind] = gsort(a,"lr","i");
t = b(2:$,:) < b(1:$-1,:);
assert_checkfalse(or(t(:,1)));
assert_checkfalse(or(t(b(2:$,1)==b(1:$-1,1),2)));
assert_checkequal(a(ind,:),b);

for k = 1:30
    p = grand(1,"prm",(1:size(a,1))');
    [b,ind] = gsort(a(p,:),"lr","i");
    t = b(2:$,:) < b(1:$-1,:);
    assert_checkfalse(or(t(:,1)));
    assert_checkfalse(or(t(b(2:$,1)==b(1:$-1,1),2)));
    assert_checkequal(a(p(ind),:),b);
end

[b,ind] = gsort(a,"lr","d");
t = b(2:$,:) > b(1:$-1,:);
assert_checkfalse(or(t(:,1)));
assert_checkfalse(or(t(b(2:$,1)==b(1:$-1,1),2)));
assert_checkequal(a(ind,:),b);

for k = 1:30
    p = grand(1,"prm",(1:size(a,1))');
    [b,ind] = gsort(a(p,:),"lr","d");
    t = b(2:$,:) > b(1:$-1,:);
    assert_checkfalse(or(t(:,1)));
    assert_checkfalse(or(t(b(2:$,1)==b(1:$-1,1),2)));
    assert_checkequal(a(p(ind),:),b);
end

a = b;
a([10 60],:) = a([60 10],:);
[b,ind] = gsort(a,"lr","i");
t = b(2:$,:) < b(1:$-1,:);
assert_checkfalse(or(t(:,1)));
assert_checkfalse(or(t(b(2:$,1)==b(1:$-1,1),2)));
assert_checkequal(a(ind,:),b);

[b,ind] = gsort(a,"lr","d");
t = b(2:$,:) > b(1:$-1,:);
assert_checkfalse(or(t(:,1)));
assert_checkfalse(or(t(b(2:$,1)==b(1:$-1,1),2)));
assert_checkequal(a(ind,:),b);

// test of gsort for uuint16 matrices:
N = 4;P = 20;Nrand = 100; vmax = 4;

a = uint16(matrix(grand(1,"prm",(1:N*P)'),N,P));

//-----Global sort --------------------------------

[a1,ind] = gsort(a,"g");
assert_checkequal(a1, uint16(matrix(N*P:-1:1,N,P)));
assert_checkequal(a1, matrix(a(ind),N,P));

for i = 1:Nrand
    b = uint16(10 * rand(N,P,"u"));
    [b1,ind] = gsort(b,"g");
    assert_checkequal(b1, matrix(b(ind),N,P));
    assert_checktrue(or(b1(1:$-1) - b1(2:$) >= 0));
end

//increasing values
[a1,ind] = gsort(a,"g","i");
assert_checkequal(a1, uint16(matrix(1:N*P,N,P)));
assert_checkequal(a1, matrix(a(ind),N,P));

//----sort each column of a ('r' means that the row indice is used for sorting)
[a1,ind] = gsort(a,"r");
nc = size(a,"c");
test = [];
for i = 1:nc
    test = [test, matrix(a(ind(:,i),i),N,1)];
end
assert_checkequal(a1, test);
test = [];
for i = 1:nc
    test = [test, gsort(a(:,i),"g")];
end
assert_checkequal(a1, test);

//increasing values
[a1,ind] = gsort(a,"r","i");
nc = size(a,"c");
test = [];
for i = 1:nc
    test = [test, matrix(a(ind(:,i),i),N,1)];
end
assert_checkequal(a1, test);

test = [];
for i = 1:nc
    test = [test, gsort(a(:,i),"g","i")];
end
assert_checkequal(a1, test);

//----sort each row of a ('c' means that the column indice is used for sorting)
[a1,ind] = gsort(a,"c");
nr = size(a,"r");
test = [];
for i = 1:nr
    test = [test; matrix(a(i,ind(i,:)),1,P)];
end
assert_checkequal(a1, test);

test = [];
for i = 1:nr
    test = [test; gsort(a(i,:),"g")];
end
assert_checkequal(a1, test);

//increasing
[a1,ind] = gsort(a,"c","i");
nr = size(a,"r");
test = [];
for i = 1:nr
    test = [test; matrix(a(i,ind(i,:)),1,P)];
end
assert_checkequal(a1, test);

test = [];
for i = 1:nr
    test = [test; gsort(a(i,:),"g","i")];
end
assert_checkequal(a1, test);

//----sort the rows of a in lexicographic order
//    i.e a(k,:) < a(l,:) if there's a number j
//    such that a(k,j) < a(l,j) or a(k,p)=a(l,p) for p in [1,j-1];
//
N1 = 4;P1 = 3;
alr = [1,2,2;
1,2,1;
1,1,2;
1,1,1];
// a random permutation
[ax,perm] = gsort(rand(1,N1,"u"));
a = uint16(alr(perm,:));

[a1,ind] = gsort(a,"lr");
assert_checkequal(a1, uint16(alr));
assert_checkequal(a1, matrix(a(ind,:),N1,P1));

[a2,ind2] = gsort(a * [100;10;1],"g");
assert_checkequal(ind2, ind);
///////////////////////
for i = 1:Nrand
    b = uint16(vmax*rand(N,P,"u"));
    [b1,ind] = gsort(b,"lr");
    assert_checkequal(b1, b(ind,:));
    v= double(b1)*((vmax+1)^[P-1:-1:0])';
    assert_checktrue(or(v(2:$)-v(1:$-1) <= 0));
end

// increasing
[a1,ind] = gsort(a,"lr","i");
assert_checkequal(a1, uint16(alr(N1:-1:1,:)));
assert_checkequal(a1, matrix(a(ind,:),N1,P1));

[a2,ind2] = gsort(a * [100;10;1],"g","i");
assert_checkequal(ind2, ind);

for i = 1:Nrand
    b = int(vmax * rand(N,P,"u"));
    [b1,ind] = gsort(b,"lr","i");
    assert_checkequal(b1, b(ind,:));
    v = double(b1) * ((vmax+1)^[P-1:-1:0])';
    assert_checktrue(or(v(2:$)-v(1:$-1) >= 0));
end

//----sort the columns of a in lexicographic order
N1 = 3;P1 = 4;
alr = alr';
// a random permutation
[ax,perm] = gsort(rand(1,P1,"u"));
a = uint16(alr(:,perm));

[a1,ind] = gsort(a,"lc");
assert_checkequal(a1, uint16(alr));
assert_checkequal(a1, matrix(a(:,ind),N1,P1));

[a2,ind2] = gsort([100,10,1] * a,"g");
assert_checkequal(ind2, ind);

for i = 1:Nrand
    b = int(vmax * rand(N1,P1,"u"));
    [b1,ind] = gsort(b,"lc");
    assert_checkequal(b1, b(:,ind));
    v = ((vmax+1)^[N1-1:-1:0]) * double(b1);
    assert_checktrue(or(v(2:$)-v(1:$-1) <= 0));
end

//increasing
[a1,ind] = gsort(a,"lc","i");

assert_checkequal(a1, uint16(alr(:,P1:-1:1)));
assert_checkequal(a1, matrix(a(:,ind),N1,P1));

[a2,ind2] = gsort([100,10,1] * a,"g","i");
assert_checkequal(ind2, ind);

vmax = 4;
for i = 1:Nrand
    b = int(vmax * rand(N,P,"u"));
    [b1,ind] = gsort(b,"lc","i");
    assert_checkequal(b1, b(:,ind));
    v = ((vmax+1)^[N-1:-1:0])*b1;
    assert_checktrue(or(v(2:$)-v(1:$-1) >= 0));
end

////////////////////////////////////////////////////////////////////////
// test of gsort for uint8 matrices:
N = 4;P = 20;Nrand = 100; vmax = 4;

a = uint8(matrix(grand(1,"prm",(1:N*P)'),N,P));

//-----Global sort --------------------------------

[a1,ind] = gsort(a,"g");
assert_checkequal(a1, uint8(matrix(N*P:-1:1,N,P)));
assert_checkequal(a1, matrix(a(ind),N,P));

for i = 1:Nrand
    b = uint8(10*rand(N,P,"u"));
    [b1,ind] = gsort(b,"g");
    assert_checkequal(b1, matrix(b(ind),N,P));
    assert_checktrue(or(b1(1:$-1)-b1(2:$) >= 0));
end

//increasing values
[a1,ind] = gsort(a,"g","i");
assert_checkequal(a1, uint8(matrix(1:N*P,N,P)));
assert_checkequal(a1, matrix(a(ind),N,P));

//----sort each column of a ('r' means that the row indice is used for sorting)
[a1,ind] = gsort(a,"r");
nc = size(a,"c");
test = [];
for i = 1:nc
    test = [test, matrix(a(ind(:,i),i),N,1)];
end
assert_checkequal(a1, test);

test = [];
for i = 1:nc
    test = [test, gsort(a(:,i),"g")];
end
assert_checkequal(a1, test);

//increasing values
[a1,ind] = gsort(a,"r","i");
nc = size(a,"c");
test = [];
for i = 1:nc
    test = [test, matrix(a(ind(:,i),i),N,1)];
end
assert_checkequal(a1, test);

test = [];
for i = 1:nc ;
    test = [test, gsort(a(:,i),"g","i")];
end
assert_checkequal(a1, test);

//----sort each row of a ('c' means that the column indice is used for sorting)
[a1,ind] = gsort(a,"c");
nr = size(a,"r");
test = [];
for i = 1:nr;
    test = [test; matrix(a(i,ind(i,:)),1,P)];
end
assert_checkequal(a1, test);

test = [];
for i = 1:nr
    test = [test; gsort(a(i,:),"g")];
end
assert_checkequal(a1, test);

//increasing
[a1,ind] = gsort(a,"c","i");
nr = size(a,"r");
test = [];
for i = 1:nr
    test = [test; matrix(a(i,ind(i,:)),1,P)];
end
assert_checkequal(a1, test);

test = [];
for i = 1:nr
    test = [test; gsort(a(i,:),"g","i")];
end
assert_checkequal(a1, test);

//----sort the rows of a in lexicographic order
//    i.e a(k,:) < a(l,:) if there's a number j
//    such that a(k,j) < a(l,j) or a(k,p)=a(l,p) for p in [1,j-1];
//
N1 = 4;P1 = 3;
alr = [1,2,2;
1,2,1;
1,1,2;
1,1,1];
// a random permutation
[ax,perm] = gsort(rand(1,N1,"u"));
a = uint8(alr(perm,:));

[a1,ind] = gsort(a,"lr");
assert_checkequal(a1, uint8(alr));
assert_checkequal(a1, matrix(a(ind,:),N1,P1));

[a2,ind2] = gsort(a*[100;10;1],"g");
assert_checkequal(ind2, ind);
///////////////////////
for i = 1:Nrand
    b = uint8(vmax*rand(N,P,"u"));
    [b1,ind] = gsort(b,"lr");
    assert_checkequal(b1, b(ind,:));
    v = double(b1)*((vmax+1)^[P-1:-1:0])';
    assert_checktrue(or(v(2:$)-v(1:$-1) <= 0));
end

// increasing
[a1,ind] = gsort(a,"lr","i");
assert_checkequal(a1, uint8(alr(N1:-1:1,:)));
assert_checkequal(a1, matrix(a(ind,:),N1,P1));

[a2,ind2] = gsort(a*[100;10;1],"g","i");
assert_checkequal(ind2, ind);

for i = 1:Nrand
    b = int(vmax*rand(N,P,"u"));
    [b1,ind] = gsort(b,"lr","i");
    assert_checkequal(b1, b(ind,:));
    v= double(b1)*((vmax+1)^[P-1:-1:0])';
    assert_checktrue(or(v(2:$)-v(1:$-1) >= 0));
end

//----sort the columns of a in lexicographic order
N1 = 3;P1 = 4;
alr = alr';
// a random permutation
[ax,perm] = gsort(rand(1,P1,"u"));
a = uint8(alr(:,perm));

[a1,ind] = gsort(a,"lc");
assert_checkequal(a1, uint8(alr));
assert_checkequal(a1, matrix(a(:,ind),N1,P1));

[a2,ind2] = gsort([100,10,1]*a,"g");
assert_checkequal(ind2, ind);

for i = 1:Nrand
    b = int(vmax * rand(N1,P1,"u"));
    [b1,ind] = gsort(b,"lc");
    assert_checkequal(b1, b(:,ind));
    v = ((vmax+1)^[N1-1:-1:0])*double(b1);
    assert_checktrue(or(v(2:$)-v(1:$-1) <= 0));
end

//increasing
[a1,ind] = gsort(a,"lc","i");
assert_checkequal(a1, uint8(alr(:,P1:-1:1)));
assert_checkequal(a1, matrix(a(:,ind),N1,P1));

[a2,ind2] = gsort([100,10,1]*a,"g","i");
assert_checkequal(ind2, ind);

for i = 1:Nrand
    b = int(vmax*rand(N,P,"u"));
    [b1,ind] = gsort(b,"lc","i");
    assert_checkequal(b1, b(:,ind));
    v= ((vmax+1)^[N-1:-1:0])*b1;
    assert_checktrue(or(v(2:$)-v(1:$-1) >= 0));
end

a = uint8([1,1,1,1,2,2,3,3,2,2,3,3,4,4,4,4,4,4,4,4,5,5,5,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,9,9,9,7,9,10,..
10,10,10,11,11,11,12,13,13,13,13,14,14,12,12,14,14,14,14,14,14,15,15,15,15,16,17,18;
3,4,4,5,3,8,4,4,7,8,8,8,5,5,8,9,10,10,9,8,6,10,10,10,10,11,8,8,13,13,9,9,13,13,10,10,13,..
13,14,15,15,12,14,11,11,15,15,15,15,16,17,14,14,17,17,15,15,13,13,17,17,18,18,19,19,16,..
16,19,19,19,18,19]-5)';
[b,ind] = gsort(a,"lr","i");
t = b(2:$,:) < b(1:$-1,:);
assert_checkfalse(or(t(:,1)));
assert_checkfalse(or(t(b(2:$,1)==b(1:$-1,1),2)));
assert_checkequal(a(ind,:),b);

for k = 1:30
    p = grand(1,"prm",(1:size(a,1))');
    [b,ind] = gsort(a(p,:),"lr","i");
    t = b(2:$,:) < b(1:$-1,:);
    assert_checkfalse(or(t(:,1)));
    assert_checkfalse(or(t(b(2:$,1)==b(1:$-1,1),2)));
    assert_checkequal(a(p(ind),:),b);
end

[b,ind]=gsort(a,"lr","d");
t=b(2:$,:) > b(1:$-1,:);
assert_checkfalse(or(t(:,1)));
assert_checkfalse(or(t(b(2:$,1)==b(1:$-1,1),2)));
assert_checkequal(a(ind,:),b);

for k = 1:30
    p = grand(1,"prm",(1:size(a,1))');
    [b,ind] = gsort(a(p,:),"lr","d");
    t = b(2:$,:) > b(1:$-1,:);
    assert_checkfalse(or(t(:,1)));
    assert_checkfalse(or(t(b(2:$,1)==b(1:$-1,1),2)));
    assert_checkequal(a(p(ind),:),b);
end

a = b;
a([10 60],:) = a([60 10],:);
[b,ind] = gsort(a,"lr","i");
t = b(2:$,:) < b(1:$-1,:);
assert_checkfalse(or(t(:,1)));
assert_checkfalse(or(t(b(2:$,1)==b(1:$-1,1),2)));
assert_checkequal(a(ind,:),b);

[b,ind] = gsort(a,"lr","d");
t = b(2:$,:) > b(1:$-1,:);
assert_checkfalse(or(t(:,1)));
assert_checkfalse(or(t(b(2:$,1)==b(1:$-1,1),2)));
assert_checkequal(a(ind,:),b);

//testing gsort with Nan's
b = gsort([1 2 %nan 3 4],"g","i");
assert_checkequal(b(1:4), (1:4));
assert_checkequal(find(isnan(b)), 5);

b = gsort([1 2 %nan 1 3 ],"g","i");
assert_checkequal(b(1:4), [1 1 2 3]);
assert_checkequal(find(isnan(b)), 5);

b = gsort([1 2 %nan 1 3 ],"g","d");
assert_checkequal(b(2:$), [3 2 1 1]);
assert_checkequal(find(isnan(b)), 1);

b = gsort([1 2 %nan 1 3 %nan 2 3],"g","d");
assert_checkequal(b(3:$), [3,3,2,2,1,1]);
assert_checkequal(find(isnan(b)), [1 2]);

b = gsort([1 2 %nan 1 3 %nan 2 3],"g","i");
assert_checkequal(b(1:$-2), [1,1,2,2,3,3]);
assert_checkequal(find(isnan(b)), [7 8]);

m = [1 2 %nan;1 3 %nan;1 2 3];
b = gsort(m,"lr","i");
assert_checkequal(sci2exp(b,0), "[1,2,3;1,2,%nan;1,3,%nan]");

b = gsort(m,"lr","d");
assert_checkequal(sci2exp(b,0), "[1,3,%nan;1,2,%nan;1,2,3]");

m = m(:,[3 1 2]);
b = gsort(m,"lc","i");
assert_checkequal(sci2exp(b,0), "[1,2,%nan;1,3,%nan;1,2,3]");

b = gsort(m,"lc","d");
assert_checkequal(sci2exp(b,0), "[%nan,2,1;%nan,3,1;3,2,1]");

//testing gsort with Inf's
b = gsort([1 2 %inf 3 4],"g","i");
assert_checkequal(b(1:4), (1:4));
assert_checkequal(find(isinf(b)), 5);

b=gsort([1 2 %inf 1 3 ],"g","i");
assert_checkequal(b(1:4), [1 1 2 3]);
assert_checkequal(find(isinf(b)), 5);

b=gsort([1 2 %inf 1 3 ],"g","d");
assert_checkequal(b(2:$), [3 2 1 1]);
assert_checkequal(find(isinf(b)), 1);

b=gsort([1 2 %inf 1 3 %inf 2 3],"g","d");
assert_checkequal(b(3:$), [3,3,2,2,1,1]);
assert_checkequal(find(isinf(b)), [1 2]);

b=gsort([1 2 %inf 1 3 %inf 2 3],"g","i");
assert_checkequal(b(1:$-2), [1,1,2,2,3,3]);
assert_checkequal(find(isinf(b)), [7 8]);

m = [1 2 %inf;1 3 %inf;1 2 3];
b = gsort(m,"lr","i");
assert_checkequal(sci2exp(b,0), "[1,2,3;1,2,%inf;1,3,%inf]");

b = gsort(m,"lr","d");
assert_checkequal(sci2exp(b,0), "[1,3,%inf;1,2,%inf;1,2,3]");

m = m(:,[3 1 2]);
b = gsort(m,"lc","i");
assert_checkequal(sci2exp(b,0), "[1,2,%inf;1,3,%inf;1,2,3]");

b = gsort(m,"lc","d");
assert_checkequal(sci2exp(b,0), "[%inf,2,1;%inf,3,1;3,2,1]");

//gsort
a = [5 1 3 2 4];
assert_checkequal(gsort(a), [5 4 3 2 1]);
assert_checkequal(gsort(a + 0), [5 4 3 2 1]);

[s,k] = gsort(a);
assert_checkequal(k, [1 5 3 4 2]);
assert_checkequal(s, [5 4 3 2 1]);

[s,k]=gsort(a+0);
assert_checkequal(k, [1 5 3 4 2]);
assert_checkequal(s, [5 4 3 2 1]);

a = string([5 1 3 2 4]);
assert_checkequal(gsort(a,"g","i"), string(1:5));

[s,k]=gsort(a,"g","i");
assert_checkequal(k, [2 4 3 5 1]);
assert_checkequal(s, string(1:5));

a = [];
assert_checkequal(gsort(a), []);
[s,k]=gsort(a);
assert_checkequal(s, []);
assert_checkequal(k, []);

// gsort with sparses (macro: %sp_gsort):
b = [5 1 3 2 4;6 1 2 4 1];
B = sparse(b);

// opts = 'r' and direction = 'd' or 'i'
[s,k] = gsort(b(:),"r","d");
[s1,k1] = gsort(B(:),"r","d");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

[s,k] = gsort(b(:),"r","i");
[s1,k1] = gsort(B(:),"r","i");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

[s,k] = gsort(b(:)',"r","d");
[s1,k1] = gsort(B(:)',"r","d");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

[s,k] = gsort(b(:)',"r","i");
[s1,k1] = gsort(B(:)',"r","i");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

// opts = 'c' and direction = 'd' or 'i'
[s,k] = gsort(b(:),"c","d");
[s1,k1] = gsort(B(:),"c","d");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

[s,k] = gsort(b(:),"c","i");
[s1,k1] = gsort(B(:),"c","i");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

[s,k] = gsort(b(:)',"c","d");
[s1,k1] = gsort(B(:)',"c","d");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

[s,k] = gsort(b(:)',"c","i");
[s1,k1] = gsort(B(:)',"c","i");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

// opts = 'g' and direction = 'd' or 'i'
[s,k] = gsort(b(:),"g","d");
[s1,k1] = gsort(B(:),"g","d");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

[s,k] = gsort(b(:),"g","i");
[s1,k1] = gsort(B(:),"g","i");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

[s,k] = gsort(b(:)',"g","d");
[s1,k1] = gsort(B(:)',"g","d");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

[s,k] = gsort(b(:)',"g","i");
[s1,k1] = gsort(B(:)',"g","i");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

// opts = 'lr' and direction = 'd' or 'i'
[s,k] = gsort(b(:),"lr","d");
[s1,k1] = gsort(B(:),"lr","d");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

[s,k] = gsort(b(:),"lr","i");
[s1,k1] = gsort(B(:),"lr","i");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

[s,k] = gsort(b(:)',"lr","d");
[s1,k1] = gsort(B(:)',"lr","d");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

[s,k] = gsort(b(:)',"lr","i");
[s1,k1] = gsort(B(:)',"lr","i");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

// opts = 'lc' and direction = 'd' or 'i'
[s,k] = gsort(b(:),"lc","d");
[s1,k1] = gsort(B(:),"lc","d");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

[s,k] = gsort(b(:),"lc","i");
[s1,k1] = gsort(B(:),"lc","i");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

[s,k] = gsort(b(:)',"lc","d");
[s1,k1] = gsort(B(:)',"lc","d");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

[s,k] = gsort(b(:)',"lc","i");
[s1,k1] = gsort(B(:)',"lc","i");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);


// gsort with complex sparses :
A = [1 -%i;%i 0];
A1 = sparse(A);

c = gsort(A(:),"g","d");
d = full(gsort(A1(:),"g","d"));
assert_checkequal(c,d);

c = gsort(A(:),"g","i");
d = full(gsort(A1(:),"g","i"));
assert_checkequal(c,d);

c = gsort(A(:),"r","d");
d = full(gsort(A1(:),"r","d"));
assert_checkequal(c,d);

c = gsort(A(:),"r","i");
d = full(gsort(A1(:),"r","i"));
assert_checkequal(c,d);

c = gsort(A(:),"c","d");
d = full(gsort(A1(:),"c","d"));
assert_checkequal(c,d);

c = gsort(A(:),"c","i");
d = full(gsort(A1(:),"c","i"));
assert_checkequal(c,d);

B = [1 1+%i 4; -2*%i 3 3-%i];
B1 = sparse(B);

c = gsort(B(:),"g","d");
d = full(gsort(B1(:),"g","d"));
assert_checkequal(c,d);

c = gsort(B(:),"g","i");
d = full(gsort(B1(:),"g","i"));
assert_checkequal(c,d);

c = gsort(B(:),"r","d");
d = full(gsort(B1(:),"r","d"));
assert_checkequal(c,d);

c = gsort(B(:),"r","i");
d = full(gsort(B1(:),"r","i"));
assert_checkequal(c,d);

c = gsort(B(:),"c","d");
d = full(gsort(B1(:),"c","d"));
assert_checkequal(c,d);

c = gsort(B(:),"c","i");
d = full(gsort(B1(:),"c","i"));
assert_checkequal(c,d);

C = [-%i 3*%i;4 9;-2*%i 7];
C1 = sparse(C);

[s,k] = gsort(C(:),"g","d");
[s1,k1] = gsort(C1(:),"g","d");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

[s,k] = gsort(C(:),"g","i");
[s1,k1] = gsort(C1(:),"g","i");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

[s,k] = gsort(C(:),"r","d");
[s1,k1] = gsort(C1(:),"r","d");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

[s,k] = gsort(C(:),"r","i");
[s1,k1] = gsort(C1(:),"r","i");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

[s,k] = gsort(C(:),"c","d");
[s1,k1] = gsort(C1(:),"c","d");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

[s,k] = gsort(C(:),"c","i");
[s1,k1] = gsort(C1(:),"c","i");
assert_checkequal(full(s1),s);
assert_checkequal(k1,k);

//================================ hypermatrix ================================================

A = rand(6, 4, 2, 8);
// ----- decreasing -----
// 'g'
[hs hind]   = gsort(A);
[s ind]     = gsort(A(:));

assert_checkequal(hs(:), s);
assert_checkequal(hind(:), ind);

hs = gsort(A);
assert_checkequal(hs(:), s);

// 'c' 'r' 'lc' 'lr'
[hsc hindc]   = gsort(A, "c");
[hsr hindr]   = gsort(A, "r");
[hslc hindlc] = gsort(A, "lc");
[hslr hindlr] = gsort(A, "lr");

a       = matrix(A, 6, 4, 16); // 2 * 8 = 16
sc      = zeros(a);
indc    = zeros(a);
sr      = zeros(a);
indr    = zeros(a);
slc     = zeros(a);
indlc   = zeros(1, 4, 16);
slr     = zeros(a);
indlr   = zeros(6, 1, 16);

for i = 1:16
    [sc(:,:,i) indc(:,:,i)]   = gsort(a(:,:,i),"c");
    [sr(:,:,i) indr(:,:,i)]   = gsort(a(:,:,i),"r");
    [slc(:,:,i) indlc(:,:,i)] = gsort(a(:,:,i),"lc");
    [slr(:,:,i) indlr(:,:,i)] = gsort(a(:,:,i),"lr");
end

sc      = matrix(sc, 6, 4, 2, 8);
indc    = matrix(indc, 6, 4, 2, 8);
sr      = matrix(sr, 6, 4, 2, 8);
indr    = matrix(indr, 6, 4, 2, 8);
slc     = matrix(slc, 6, 4, 2, 8);
indlc   = matrix(indlc, 1, 4, 2, 8);
slr     = matrix(slr, 6, 4, 2, 8);
indlr   = matrix(indlr, 6, 1, 2, 8);

assert_checkequal(hsc, sc);
assert_checkequal(hindc, indc);
assert_checkequal(hsr, sr);
assert_checkequal(hindr, indr);
assert_checkequal(hslc, slc);
assert_checkequal(hindlc, indlc);
assert_checkequal(hslr, slr);
assert_checkequal(hindlr, indlr);

// whitout indices
hsc  = gsort(A, "c");
hsr  = gsort(A, "r");
hslc = gsort(A, "lc");
hslr = gsort(A, "lr");

assert_checkequal(hsc, sc);
assert_checkequal(hsr, sr);
assert_checkequal(hslc, slc);
assert_checkequal(hslr, slr);

// ----- increasing -----
// 'g'
[hs hind]   = gsort(A, "g", "i");
[s ind]     = gsort(A(:), "g", "i");

assert_checkequal(hs(:), s);
assert_checkequal(hind(:), ind);

hs = gsort(A, "g", "i");
assert_checkequal(hs(:), s);

// 'c' 'r' 'lc' 'lr'
[hsc hindc]   = gsort(A, "c", "i");
[hsr hindr]   = gsort(A, "r", "i");
[hslc hindlc] = gsort(A, "lc", "i");
[hslr hindlr] = gsort(A, "lr", "i");

a       = matrix(A, 6, 4, 16); // 2 * 8 = 16
sc      = zeros(a);
indc    = zeros(a);
sr      = zeros(a);
indr    = zeros(a);
slc     = zeros(a);
indlc   = zeros(1, 4, 16);
slr     = zeros(a);
indlr   = zeros(6, 1, 16);

for i = 1:16
    [sc(:,:,i) indc(:,:,i)]   = gsort(a(:,:,i),"c", "i");
    [sr(:,:,i) indr(:,:,i)]   = gsort(a(:,:,i),"r", "i");
    [slc(:,:,i) indlc(:,:,i)] = gsort(a(:,:,i),"lc", "i");
    [slr(:,:,i) indlr(:,:,i)] = gsort(a(:,:,i),"lr", "i");
end

sc      = matrix(sc, 6, 4, 2, 8);
indc    = matrix(indc, 6, 4, 2, 8);
sr      = matrix(sr, 6, 4, 2, 8);
indr    = matrix(indr, 6, 4, 2, 8);
slc     = matrix(slc, 6, 4, 2, 8);
indlc   = matrix(indlc, 1, 4, 2, 8);
slr     = matrix(slr, 6, 4, 2, 8);
indlr   = matrix(indlr, 6, 1, 2, 8);

assert_checkequal(hsc, sc);
assert_checkequal(hindc, indc);
assert_checkequal(hsr, sr);
assert_checkequal(hindr, indr);
assert_checkequal(hslc, slc);
assert_checkequal(hindlc, indlc);
assert_checkequal(hslr, slr);
assert_checkequal(hindlr, indlr);

// whitout indices
hsc  = gsort(A, "c", "i");
hsr  = gsort(A, "r", "i");
hslc = gsort(A, "lc", "i");
hslr = gsort(A, "lr", "i");

assert_checkequal(hsc, sc);
assert_checkequal(hsr, sr);
assert_checkequal(hslc, slc);
assert_checkequal(hslr, slr);
