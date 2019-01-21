// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

[v, ka, kb] = intersect([],[]);
assert_checkequal(v, []);
assert_checkequal(ka, []);
assert_checkequal(kb, []);

[v,ka,kb] = intersect([1 2],[3 4]);
assert_checkequal(v, []);
assert_checkequal(ka, []);
assert_checkequal(kb, []);


A = [ 1 8 4 5 2 1];
B = [ 9 7 4 2 1 4];

[v,ka,kb] = intersect(A,B);
if or(A(ka) <> B(kb)) then pause,end
if or(A(ka) <> v) then pause,end
if or(v<>intersect(A,B))  then pause,end
if (or(v<>[1,2,4])) then pause,end

// With texts
// ----------
A = 'a'+string(A);
B = 'b'+string(B);
[v,ka,kb] = intersect(A,B);
if or(A(ka) <> B(kb)) then pause,end
if or(A(ka) <> v) then pause,end
if or(v<>intersect(A,B))  then pause,end
if v <> [] then pause,end


A = [ "elt1" "elt3" "elt4"];
B = [ "elt5" "elt1" "elt3"];
[v, ka, kb] = intersect(A,B);
if or(A(ka) <> B(kb)) then pause,end
if or(A(ka) <> v) then pause,end
if or(v<>intersect(A,B))  then pause,end
if (or(v<>["elt1","elt3"])) then pause,end

A = [ "elt1" "elt3" "elt4"];
B = [ "elt5" "elt6" "elt2" "elt1" "elt3"];

[v, ka, kb] = intersect(A,B);
if or(A(ka) <> B(kb)) then pause,end
if or(A(ka) <> v) then pause,end
if or(v<>intersect(A,B))  then pause,end
if (or(v<>["elt1","elt3"])) then pause,end

// UTF-8
A = strsplit("هو برنامج علمي كبير ""Scilab""")'
B = strsplit("فهو حر ومفتوح")'
assert_checkequal(intersect(A,B),[" "  "ر"  "م"  "ه"  "و"]);

// with integers
// ------------
A = int16([ 1 8 4 5 2 1]);
B = int16([ 9 7 4 2 1 4]);

[v, ka, kb]=intersect(A,B);
if or(A(ka) <> B(kb)) then pause,end
if or(A(ka) <> v) then pause,end
if or(v<>intersect(A,B))  then pause,end
if (or(v<>int16([1,2,4]))) then pause,end

A = uint8([ 1 8 4 5 2 1]);
B = uint8([ 9 7 4 2 1 4]);

[v, ka, kb]=intersect(A,B);
if or(A(ka) <> B(kb)) then pause,end
if or(A(ka) <> v) then pause,end
if or(v<>intersect(A,B))  then pause,end
if (or(v<>uint8([1,2,4]))) then pause,end

// With orientation: Common rows or columns
// ----------------------------------------
A = [0,0,1,1 1;
      0,1,1,1,1;
      2,0,1,1,1;
      0,2,2,2,2;
      2,0,1,1,1;
      0,0,1,1,3];
B = [1,0,1;
     1,0,2;
     1,2,3;
     2,0,4;
     1,2,5;
     3,0,6];
[v, ka, kb] = intersect(A,B,'c');
assert_checkequal(v, A(:,ka));
assert_checkequal(A(:,ka), B(:,kb));
assert_checkequal(intersect(A,B,'c'), v);
assert_checkequal(v, [0,1;0,1;2,1;0,2;2,1;0,3]);

A = A'; B = B';
[v, ka, kb] = intersect(A, B, 'r');
assert_checkequal(v, A(ka,:));
assert_checkequal(A(ka,:), B(kb,:));
assert_checkequal(intersect(A,B,'r'), v);
assert_checkequal(v, [0,1;0,1;2,1;0,2;2,1;0,3]');

A32 = uint32(A);
B32 = uint32(B);
[v, ka, kb] = intersect(A32, B32, 'r');
assert_checkequal(v, A32(ka,:));
assert_checkequal(A32(ka,:), B32(kb,:));
assert_checkequal(intersect(A32,B32,'r'), v);
assert_checkequal(v, uint32([0,1;0,1;2,1;0,2;2,1;0,3]'));

// with Nan
// --------
assert_checkequal(intersect(%nan,%nan), []);
assert_checkequal(intersect([1 -2 %nan 3 6], [%nan 1:3]), [1 3]);

[A, B] = (A.', B.');
A($) = %nan;
B(6,1) = %nan;
[v,ka,kb] = intersect(A,B,'c');
assert_checkequal(v, A(:,ka));
assert_checkequal(A(:,ka), B(:,kb));
assert_checkequal(intersect(A,B,'c'), v);
assert_checkequal(v, [0;0;2;0;2;0]);

// With complex numbers
// --------------------
m = [
  "[1+%i,1+%i,1,0,0,%i,1,1,1,0,1+%i,0,%i,%i,%i,1+%i,1+%i,0,1,1,1,0,0,%i;"
  "1,0,%i,0,%i,1,1,0,%i,1,0,0,1,1,1+%i,1,1,%i,1+%i,%i,1,0,1+%i,0]"
  ];
m = evstr(m).';
m1 = m(1:$/2,:);
m2 = m($/2+1:$,:);
/*         m1           row#           m2
   1. + i     1. + 0.i    1    0. + i     1. + 0.i
   1. + i     0. + 0.i    2    0. + i     1. + 0.i
   1. + 0.i   0. + i      3    0. + i     1. + i
   0. + 0.i   0. + 0.i    4    1. + i     1. + 0.i
   0. + 0.i   0. + i      5    1. + i     1. + 0.i
   0. + i     1. + 0.i    6    0. + 0.i   0. + i
   1. + 0.i   1. + 0.i    7    1. + 0.i   1. + i
   1. + 0.i   0. + 0.i    8    1. + 0.i   0. + i
   1. + 0.i   0. + i      9    1. + 0.i   1. + 0.i
   0. + 0.i   1. + 0.i   10    0. + 0.i   0. + 0.i
   1. + i     0. + 0.i   11    0. + 0.i   1. + i
   0. + 0.i   0. + 0.i   12    0. + i     0. + 0.i
*/
// by element
[x, y] = (m1(1:6,1), m2(1:6,2));
i = intersect(x, y);
ref = [1 %i 1+%i];

assert_checkequal(i, ref);
[i, k1, k2] = intersect(x, y);
assert_checkequal(i, ref);
assert_checkequal(i, x(k1).');
assert_checkequal(i, y(k2).');
assert_checkequal(k1, [3 6 1]);
assert_checkequal(k2, [1 6 3]);

// "r": common rows
i = intersect(m1,m2,"r");
ref = evstr(["[0,0,1,1,%i,1+%i;";"0,%i,1,%i,1,1]"]).';
/*
   0. + 0.i   0. + 0.i
   0. + 0.i   0. + i
   1. + 0.i   1. + 0.i
   1. + 0.i   0. + i
   0. + i     1. + 0.i
   1. + i     1. + 0.i
*/
assert_checkequal(i, ref);
[i, k1, k2] = intersect(m1,m2,"r");
assert_checkequal(i, ref);
assert_checkequal(i, m1(k1,:));
assert_checkequal(i, m2(k2,:));
assert_checkequal(k1, [4  5  7  3  6  1]);
assert_checkequal(k2, [10 6  9  8  1  4]);

// "c": common columns
[m1, m2, ref] = (m1.', m2.', ref.');
i = intersect(m1, m2, "c");
assert_checkequal(i, ref);
[i, k1, k2] = intersect(m1, m2, "c");
assert_checkequal(i, ref);
assert_checkequal(i, m1(:,k1));
assert_checkequal(i, m2(:,k2));
assert_checkequal(k1, [4  5  7  3  6  1]);
assert_checkequal(k2, [10 6  9  8  1  4]);
