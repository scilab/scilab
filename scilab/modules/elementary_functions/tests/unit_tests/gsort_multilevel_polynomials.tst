// ===================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// ===================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->
//
//====================================================================
//          Tests of multilevel gsort() for polynomials
//====================================================================
/*
We sort polynomials,
 - first: by degree, with degree()
 - second: by highest coefficient, by a custom function.
*/
function c = get_degreeI(p,i)
    tmp = coeff(p(:))
    [nl,nc] = size(tmp)
    if i <> %inf then
        tmp = tmp(:,i+1)
    else
        tmp = tmp'((0:nl-1)'*nc + max(0,degree(p(:)))+1) // highest coeff
    end
    c = matrix(tmp, size(p))
endfunction
crit = list(degree, list(get_degreeI, %inf));

// Initial array
// -------------
x = poly(0,"x");
p = [
  "[2*x,1+2*x+x^2,1+2*x+2*x^2,2+x,2*x+x^2,0*x;"
  "2+x^2,2*x+x^2,0,2*x^2,1+x+2*x^2,2*x+2*x^2;"
  "1+2*x^2,x+x^2,2*x^2,1+2*x,2+2*x,2+2*x+x^2;"
  "2*x+x^2,x,1+x+x^2,2*x,x+x^2,x+2*x^2;"
  "1+2*x+x^2,2+x+x^2,1+2*x+x^2,2*x+2*x^2,2,2+x+2*x^2;"
  "x,2*x,2+2*x+x^2,2+2*x+x^2,1,2+x];"
 ];
p = evstr(p);
/*
  2x         1 +2x +x²  1 +2x +2x²  2 +x       2x +x²     0
  2 +x²      2x +x²     0           2x²        1 +x +2x²  2x +2x²
  1 +2x²     x +x²      2x²         1 +2x      2 +2x      2 +2x +x²
  2x +x²     x          1 +x +x²    2x         x +x²      x +2x²
  1 +2x +x²  2 +x +x²   1 +2x +x²   2x +2x²    2          2 +x +2x²
  x          2x         2 +2x +x²   2 +2x +x²  1          2 +x
*/

// "g" multilevel sorting
// ----------------------
[r, k] = gsort(p, "g", ["i" "i"], crit);
ref = [
  "[0*x,2+x,2+2*x,x+x^2,2*x+x^2,2*x^2;"
  "0*x,2+x,2+x^2,2+x+x^2,x+x^2,2*x+2*x^2;"
  "1,2*x,2*x+x^2,1+x+x^2,2+2*x+x^2,1+x+2*x^2;"
  "2,2*x,1+2*x+x^2,1+2*x+x^2,1+2*x^2,2*x+2*x^2;"
  "x,1+2*x,1+2*x+x^2,2+2*x+x^2,1+2*x+2*x^2,x+2*x^2;"
  "x,2*x,2*x+x^2,2+2*x+x^2,2*x^2,2+x+2*x^2]"
  ];
ref = evstr(ref);
/*
  0  2 +x   2 +2x      x +x²      2x +x²      2x²
  0  2 +x   2 +x²      2 +x +x²   x +x²       2x +2x²
  1  2x     2x +x²     1 +x +x²   2 +2x +x²   1 +x +2x²
  2  2x     1 +2x +x²  1 +2x +x²  1 +2x²      2x +2x²
  x  1 +2x  1 +2x +x²  2 +2x +x²  1 +2x +2x²  x +2x²
  x  2x     2x +x²     2 +2x +x²  2x²         2 +x +2x²
*/
kref = [
  14  19  27   9  25  20
  31  36   2  11  28  23
  30   1   4  16  33  26
  29  12   5  17   3  32
   6  21   7  18  13  34
  10  22   8  24  15  35
  ];
assert_checkequal(r, ref);
assert_checkequal(k, kref);
//
[r, k] = gsort(p, "g", ["i" "d"], crit);
ref = [
  "[0*x,1+2*x,2+x,1+x+2*x^2,1+2*x+x^2,1+2*x+x^2;"
  "0*x,2*x,1+2*x^2,2*x+2*x^2,1+2*x+x^2,2+2*x+x^2;"
  "2,2+2*x,1+2*x+2*x^2,x+2*x^2,2*x+x^2,2+2*x+x^2;"
  "1,x,2*x^2,2+x+2*x^2,x+x^2,2*x+x^2;"
  "2*x,x,2*x^2,2+x^2,2+x+x^2,x+x^2;"
  "2*x,2+x,2*x+2*x^2,2*x+x^2,1+x+x^2,2+2*x+x^2]"
  ];
ref = evstr(ref);
/*
  0   1 +2x  2 +x        1 +x +2x²  1 +2x +x²  1 +2x +x²
  0   2x     1 +2x²      2x +2x²    1 +2x +x²  2 +2x +x²
  2   2 +2x  1 +2x +2x²  x +2x²     2x +x²     2 +2x +x²
  1   x      2x²         2 +x +2x²  x +x²      2x +x²
  2x  x      2x²         2 +x²      2 +x +x²   x +x²
  2x  2 +x   2x +2x²     2x +x²     1 +x +x²   2 +2x +x²
*/
kref = [
  14  21  36  26   5  17
  31  22   3  32   7  18
  29  27  13  34   8  24
  30   6  15  35   9  25
   1  10  20   2  11  28
  12  19  23   4  16  33
  ];
assert_checkequal(r, ref);
assert_checkequal(k, kref);

// "r" and "c" multilevel sorting
// ------------------------------
[r, k] = gsort(p, "c", ["i" "i"], crit);
ref = [
  "[0*x,2+x,2*x,1+2*x+x^2,2*x+x^2,1+2*x+2*x^2;"
  "0*x,2+x^2,2*x+x^2,2*x^2,1+x+2*x^2,2*x+2*x^2;"
  "1+2*x,2+2*x,x+x^2,2+2*x+x^2,1+2*x^2,2*x^2;"
  "x,2*x,2*x+x^2,1+x+x^2,x+x^2,x+2*x^2;"
  "2,1+2*x+x^2,2+x+x^2,1+2*x+x^2,2*x+2*x^2,2+x+2*x^2;"
  "1,x,2+x,2*x,2+2*x+x^2,2+2*x+x^2]"
  ];
ref = evstr(ref);
/*
  0      2 +x       2x        1 +2x +x²  2x +x²     1 +2x +2x²
  0      2 +x²      2x +x²    2x²        1 +x +2x²  2x +2x²
  1 +2x  2 +2x      x +x²     2 +2x +x²  1 +2x²     2x²
  x      2x         2x +x²    1 +x +x²   x +x²      x +2x²
  2      1 +2x +x²  2 +x +x²  1 +2x +x²  2x +2x²    2 +x +2x²
  1      x          2 +x      2x         2 +2x +x²  2 +2x +x²
*/
kref = [
  6  4  1  2  5  3
  3  1  2  4  5  6
  4  5  2  6  1  3
  2  4  1  3  5  6
  5  1  2  3  4  6
  5  1  6  2  3  4
  ];
assert_checkequal(r, ref);
assert_checkequal(k, kref);
//
[r, k] = gsort(p, "c", ["d" "i"], crit);
ref = [
  "[1+2*x+x^2,2*x+x^2,1+2*x+2*x^2,2+x,2*x,0*x;"
  "2+x^2,2*x+x^2,2*x^2,1+x+2*x^2,2*x+2*x^2,0*x;"
  "x+x^2,2+2*x+x^2,1+2*x^2,2*x^2,1+2*x,2+2*x;"
  "2*x+x^2,1+x+x^2,x+x^2,x+2*x^2,x,2*x;"
  "1+2*x+x^2,2+x+x^2,1+2*x+x^2,2*x+2*x^2,2+x+2*x^2,2;"
  "2+2*x+x^2,2+2*x+x^2,x,2+x,2*x,1]"
  ];
ref = evstr(ref);
/*
  1 +2x +x²  2x +x²     1 +2x +2x²  2 +x       2x         0
  2 +x²      2x +x²     2x²         1 +x +2x²  2x +2x²    0
  x +x²      2 +2x +x²  1 +2x²      2x²        1 +2x      2 +2x
  2x +x²     1 +x +x²   x +x²       x +2x²     x          2x
  1 +2x +x²  2 +x +x²   1 +2x +x²   2x +2x²    2 +x +2x²  2
  2 +2x +x²  2 +2x +x²  x           2 +x       2x         1
*/
kref = [
  2  5  3  4  1  6
  1  2  4  5  6  3
  2  6  1  3  4  5
  1  3  5  6  2  4
  1  2  3  4  6  5
  3  4  1  6  2  5
  ];
assert_checkequal(r, ref);
assert_checkequal(k, kref);
//
[r, k] = gsort(p, "r", ["i" "i"], crit);
ref = [
  "[x,x,0*x,2+x,1,0*x;"
  "2*x,2*x,1+x+x^2,1+2*x,2,2+x;"
  "2+x^2,1+2*x+x^2,1+2*x+x^2,2*x,2+2*x,2+2*x+x^2;"
  "2*x+x^2,2*x+x^2,2+2*x+x^2,2+2*x+x^2,2*x+x^2,2*x+2*x^2;"
  "1+2*x+x^2,x+x^2,1+2*x+2*x^2,2*x^2,x+x^2,x+2*x^2;"
  "1+2*x^2,2+x+x^2,2*x^2,2*x+2*x^2,1+x+2*x^2,2+x+2*x^2]"
  ];
ref = evstr(ref);
/*
  x          x          0           2 +x       1          0
  2x         2x         1 +x +x²    1 +2x      2          2 +x
  2 +x²      1 +2x +x²  1 +2x +x²   2x         2 +2x      2 +2x +x²
  2x +x²     2x +x²     2 +2x +x²   2 +2x +x²  2x +x²     2x +2x²
  1 +2x +x²  x +x²      1 +2x +2x²  2x²        x +x²      x +2x²
  1 +2x²     2 +x +x²   2x²         2x +2x²    1 +x +2x²  2 +x +2x²
*/
kref = [
  6  4  2  1  6  1
  1  6  4  3  5  6
  2  1  5  4  3  3
  4  2  6  6  1  2
  5  3  1  2  4  4
  3  5  3  5  2  5
  ];
assert_checkequal(r, ref);
assert_checkequal(k, kref);
//
[r, k] = gsort(p, "r", ["d" "i"], crit);
ref = [
  "[2+x^2,1+2*x+x^2,1+x+x^2,2+2*x+x^2,2*x+x^2,2+2*x+x^2;"
  "2*x+x^2,2*x+x^2,1+2*x+x^2,2*x^2,x+x^2,2*x+2*x^2;"
  "1+2*x+x^2,x+x^2,2+2*x+x^2,2*x+2*x^2,1+x+2*x^2,x+2*x^2;"
  "1+2*x^2,2+x+x^2,1+2*x+2*x^2,2+x,2+2*x,2+x+2*x^2;"
  "x,x,2*x^2,1+2*x,1,2+x;"
  "2*x,2*x,0*x,2*x,2,0*x]"
  ];
ref = evstr(ref);
/*
  2 +x²      1 +2x +x²  1 +x +x²    2 +2x +x²  2x +x²     2 +2x +x²
  2x +x²     2x +x²     1 +2x +x²   2x²        x +x²      2x +2x²
  1 +2x +x²  x +x²      2 +2x +x²   2x +2x²    1 +x +2x²  x +2x²
  1 +2x²     2 +x +x²   1 +2x +2x²  2 +x       2 +2x      2 +x +2x²
  x          x          2x²         1 +2x      1          2 +x
  2x         2x         0           2x         2          0
*/
kref = [
  2  1  4  6  1  3
  4  2  5  2  4  2
  5  3  6  5  2  4
  3  5  1  1  3  5
  6  4  3  3  6  6
  1  6  2  4  5  1
  ];
assert_checkequal(r, ref);
assert_checkequal(k, kref);

// Lexicographic multilevel sorting
// --------------------------------
p = [
  "[2+2*x,2+2*x,2+x,1+x,1+2*x,1+2*x;"
  "0*x,x,2*x,2,2+x,1+x;"
  "2+2*x,1+2*x,x,1,0*x,2+x;"
  "2,1+2*x,1+2*x,x,x,1+2*x;"
  "1,2,1+x,2,1,0*x;"
  "1+x,2*x,1,1,2+2*x,x;"
  "2+x,0*x,2+x,0*x,2*x,x]"
  ];
p = evstr(p);
/*
  2 +2x  2 +2x  2 +x   1 +x  1 +2x  1 +2x
  0      x      2x     2     2 +x   1 +x
  2 +2x  1 +2x  x      1     0      2 +x
  2      1 +2x  1 +2x  x     x      1 +2x
  1      2      1 +x   2     1      0

  1 +x   2x     1      1     2 +2x  x
  2 +x   0      2 +x   0     2x     x
*/
[r, k] = gsort(p, "lr", ["i" "i"], crit);
ref = [
  "[0*x,x,2*x,2,2+x,1+x;"
  "1,2,1+x,2,1,0*x;"
  "2,1+2*x,1+2*x,x,x,1+2*x;"
  "2+x,0*x,2+x,0*x,2*x,x;"
  "1+x,2*x,1,1,2+2*x,x;"
  "2+2*x,1+2*x,x,1,0*x,2+x;"
  "2+2*x,2+2*x,2+x,1+x,1+2*x,1+2*x]"
  ];
ref = evstr(ref);
/*
  0      x      2x     2     2 +x   1 +x
  1      2      1 +x   2     1      0
  2      1 +2x  1 +2x  x     x      1 +2x
  2 +x   0      2 +x   0     2x     x
  1 +x   2x     1      1     2 +2x  x
  2 +2x  1 +2x  x      1     0      2 +x
  2 +2x  2 +2x  2 +x   1 +x  1 +2x  1 +2x
*/
assert_checkequal(r, ref);
assert_checkequal(k', [2  5  4  7  6  3  1]);

//
[r, k] = gsort(p, "lr", ["d" "i"], crit);
ref = [
  "[1+x,2*x,1,1,2+2*x,x;"
  "2+x,0*x,2+x,0*x,2*x,x;"
  "2+2*x,2+2*x,2+x,1+x,1+2*x,1+2*x;"
  "2+2*x,1+2*x,x,1,0*x,2+x;"
  "1,2,1+x,2,1,0*x;"
  "2,1+2*x,1+2*x,x,x,1+2*x;"
  "0*x,x,2*x,2,2+x,1+x]"
  ];
ref = evstr(ref);
/*
  1 +x   2x     1      1     2 +2x  x
  2 +x   0      2 +x   0     2x     x
  2 +2x  2 +2x  2 +x   1 +x  1 +2x  1 +2x
  2 +2x  1 +2x  x      1     0      2 +x
  1      2      1 +x   2     1      0
  2      1 +2x  1 +2x  x     x      1 +2x
  0      x      2x     2     2 +x   1 +x
*/
assert_checkequal(r, ref);
assert_checkequal(k', [6  7  1  3  5  4  2]);
//
[r, k] = gsort(p, "lc", ["i" "i"], crit);
ref = [
  "[1+x,2+x,2+2*x,1+2*x,1+2*x,2+2*x;"
  "2,2*x,0*x,2+x,1+x,x;"
  "1,x,2+2*x,0*x,2+x,1+2*x;"
  "x,1+2*x,2,x,1+2*x,1+2*x;"
  "2,1+x,1,1,0*x,2;"
  "1,1,1+x,2+2*x,x,2*x;"
  "0*x,2+x,2+x,2*x,x,0*x]"
  ];
ref = evstr(ref);
/*
  1 +x  2 +x   2 +2x  1 +2x  1 +2x  2 +2x
  2     2x     0      2 +x   1 +x   x
  1     x      2 +2x  0      2 +x   1 +2x
  x     1 +2x  2      x      1 +2x  1 +2x
  2     1 +x   1      1      0      2
  1     1      1 +x   2 +2x  x      2x
  0     2 +x   2 +x   2x     x      0
*/
assert_checkequal(r, ref);
assert_checkequal(k, [4  3  1  5  6  2]);
//
[r, k] = gsort(p, "lc", ["d" "i"], crit);
ref = [
  "[2+x,1+x,1+2*x,2+2*x,1+2*x,2+2*x;"
  "2*x,2,1+x,x,2+x,0*x;"
  "x,1,2+x,1+2*x,0*x,2+2*x;"
  "1+2*x,x,1+2*x,1+2*x,x,2;"
  "1+x,2,0*x,2,1,1;"
  "1,1,x,2*x,2+2*x,1+x;"
  "2+x,0*x,x,0*x,2*x,2+x]"
  ];
ref = evstr(ref);
/*
  2 +x   1 +x  1 +2x  2 +2x  1 +2x  2 +2x
  2x     2     1 +x   x      2 +x   0
  x      1     2 +x   1 +2x  0      2 +2x
  1 +2x  x     1 +2x  1 +2x  x      2
  1 +x   2     0      2      1      1
  1      1     x      2x     2 +2x  1 +x
  2 +x   0     x      0      2x     2 +x
*/
assert_checkequal(r, ref);
assert_checkequal(k, [3  4  6  2  5  1]);
