// ========================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// ========================================================================
//
//<-- CLI SHELL MODE -->
//<-- NO CHECK REF -->
//
// ==========================
// CHECKING FORMAT OF RESULTS
// ==========================
[h,j,b,i] = histc([]);
assert_checkequal(list(h,j,b,i), list([],[],[],[]));
d = rand(2,9);
[h,j,b,i] = histc(d, 6);
assert_checkequal(size(h),[1,6]);
assert_checkequal(size(j),[1,5]);
assert_checkequal(size(b),[1,7]);
assert_checkequal(size(i),[2,9]);
[h,j,b,i] = histc(d, [-%inf 0.3 0.5 0.6 %inf]);
assert_checkequal(size(h),[1,4]);
assert_checkequal(size(b),[1,5]);
d = grand(2,9,"uin",0,9);
[h,j,b,i] = histc(d, [1 3 5], "discrete");
assert_checkequal(size(h),[1,3]);
assert_checkequal(size(j),[1,5]);
assert_checkequal(size(b),[1,3]);
assert_checkequal(size(i),[2,9]);
c = d + %i*d;
[h,j,b,i] = histc(c, 4);
assert_checkequal(size(h),[1,4]);
assert_checkequal(size(j),[1,5]);
assert_checkequal(size(b),[1,5]);
assert_checkequal(size(i),[2,9]);
d = int8(d);
[h,j,b,i] = histc(d, 6);
assert_checkequal(size(h),[1,6]);
assert_checkequal(size(j),[1,2]);
assert_checkequal(size(b),[1,7]);
assert_checkequal(size(i),[2,9]);
[h,j,b,i] = histc(d, [0 3 5 8], "discrete");
assert_checkequal(size(h),[1,4]);
assert_checkequal(size(j),[1,1]);
assert_checkequal(size(b),[1,4]);
assert_checkequal(size(i),[2,9]);
d = sparse(double(d));
[h,j,b,i] = histc(d, 6);
assert_checkequal(size(h),[1,6]);
assert_checkequal(size(j),[1,5]);
assert_checkequal(size(b),[1,7]);
assert_checkequal(size(i),[2,9]);
[h,j,b,i] = histc(d, [1 3 5], "discrete");
assert_checkequal(size(h),[1,3]);
assert_checkequal(size(j),[1,5]);
assert_checkequal(size(b),[1,3]);
assert_checkequal(size(i),[2,9]);
d = d + %i*d;
[h,j,b,i] = histc(d, 4);
assert_checkequal(size(h),[1,4]);
assert_checkequal(size(j),[1,5]);
assert_checkequal(size(b),[1,5]);
assert_checkequal(size(i),[2,9]);
// Texts
t = matrix(asciimat(grand(20,1,"uin",ascii("a"), ascii("z"))), 2, 10);
[h,j,b,i] = histc(t, ["e" "i" "o" "u"]);
assert_checkequal(size(h),[1,3]);
assert_checkequal(size(j),[1,3]);
assert_checkequal(size(b),[1,4]);
assert_checkequal(size(i),[2,10]);
[h,j,b,i] = histc(t, ["e" "i" "o" "u"], "discrete");
assert_checkequal(size(h),[1,4]);
assert_checkequal(size(j),[1,3]);
assert_checkequal(size(b),[1,4]);
assert_checkequal(size(i),[2,10]);
// Polynomials
z = %z;
p = [-z z z z z 2*z 2*z 2*z z^2 z^2];
[h,j,b,i] = histc(p);
assert_checkequal(size(h),[1,4]);
assert_checkequal(size(j),[1,5]);
assert_checkequal(size(b),[1,4]);
assert_checkequal(size(i),[1,10]);
p = [z+%inf -z z z z z 2*z 2*z 2*z z^2 z^2];
[h,j,b,i] = histc(p);
assert_checkequal(size(h),[1,5]);
assert_checkequal(size(j),[1,5]);
assert_checkequal(size(b),[1,5]);

// =====================================================
// CHECKING RESULTS WITH DECIMAL REAL OR COMPLEX NUMBERS
// =====================================================
assert_checkequal(histc(37), 1);
d = [1 1 1 2 2 3 4 4 5 5 5 6 6 7 8 8 9 9 9];
N = size(d,"*");
// Default binning
[h,j,b,i] = histc(d);
href = [6 5 3 5];
assert_checkequal(h, href);
assert_checkequal(b, 1:2:9);
assert_checkequal(i, [1 1 1 1 1 1 2 2 2 2 2 3 3 3 4 4 4 4 4]);
assert_checkequal(histc(d,,"countsNorm"), href/N);
assert_checkequal(histc(d,,"density"), href/2);
assert_checkequal(histc(d,,"densityNorm"), href/2/N);
// Fixed number of bins
[h,j,b,i] = histc(d,3);
href = [6 7 6];
assert_checkequal(h, href);
assert_checkequal(b, [3 11 19 27]/3);
assert_checkequal(i, [1 1 1 1 1 1 2 2 2 2 2 2 2 3 3 3 3 3 3]);
assert_checkequal(histc(d, 3,"countsNorm"), href/N);
assert_checkalmostequal(histc(d, 3,"density"), href/(8/3));
assert_checkalmostequal(histc(d, 3,"densityNorm"), href/(8/3)/N);
// Fixed bins width
[h,j,b,i] = histc(d,-2.5);
href = [5 6 3 5];
assert_checkequal(h, href);
assert_checkequal(b, (0:4)*2.5);
assert_checkequal(i, [1 1 1 1 1 2 2 2 2 2 2 3 3 3 4 4 4 4 4]);
assert_checkequal(histc(d, -2.5, "countsNorm"), href/N);
assert_checkalmostequal(histc(d, -2.5, "density"), href/2.5);
// Automatic Sturges binning
[h,j,b,i] = histc(d,"sturges");
href = [5 1 5 2 1 5];
assert_checkequal(h, href);
assert_checkalmostequal(b, [3 7 11 15 19 23 27]/3);
assert_checkequal(i, [1 1 1 1 1 2 3 3 3 3 3 4 4 5 6 6 6 6 6]);
assert_checkequal(histc(d,"sturges","countsNorm"), href/N);
assert_checkalmostequal(histc(d,"sturges","density"), href/(4/3));
assert_checkalmostequal(histc(d,"sturges","densityNorm"), href/(4/3)/N);
// Automatic Freedman-Diaconis binning
[h,j,b,i] = histc(d, "freediac");
href = [11 8];
assert_checkequal(h, href);
assert_checkalmostequal(b, [1 5 9]);
assert_checkequal(i, [1 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2]);
assert_checkequal(histc(d, "freediac", "countsNorm"), href/N);
assert_checkalmostequal(histc(d, "freediac", "density"), href/4);
assert_checkalmostequal(histc(d, "freediac", "densityNorm"), href/4/N);
// Explicit bins edges, without outsiders
sb = [1 4 6 9];
[h,j,b,i] = histc(d,sb);
href = [8 5 6];
assert_checkequal(h, href);
assert_checkalmostequal(b, sb);
assert_checkequal(i, [1 1 1 1 1 1 1 1 2 2 2 2 2 3 3 3 3 3 3]);
assert_checkequal(histc(d, sb, "countsNorm"), href/N);
assert_checkalmostequal(histc(d, sb, "density"), href./diff(sb));
assert_checkalmostequal(histc(d, sb, "densityNorm"), href/N./diff(sb));
// Explicit bins edges, with marginal bins
sb = [-%inf 3 5 7 %inf];
[h,j,b,i] = histc(d,sb);
href = [6 5 3 5];
assert_checkequal(h, href);
assert_checkalmostequal(b, [1 3 5 7 9]);
assert_checkequal(i, [1 1 1 1 1 1 2 2 2 2 2 3 3 3 4 4 4 4 4]);
assert_checkequal(histc(d, sb, "countsNorm"), href/N);
assert_checkalmostequal(histc(d, sb, "density"), href/2);
assert_checkalmostequal(histc(d, sb, "densityNorm"), href/N/2);
// Explicit bins edges, with useless/empty extra marginal bins
sb = [-%inf 1 3 5 7 10 %inf];
[h,j,b,i] = histc(d,sb);
href = [6 5 3 5];
assert_checkalmostequal(b, [1 3 5 7 10]);
assert_checkequal(i, [1 1 1 1 1 1 2 2 2 2 2 3 3 3 4 4 4 4 4]);
assert_checkequal(histc(d, sb, "countsNorm"), href/N);
assert_checkalmostequal(histc(d, sb, "density"), href./[2 2 2 3]);
assert_checkalmostequal(histc(d, sb, "densityNorm"), href/N./[2 2 2 3]);
// Explicit bins edges, with outsiders
// d = [1 1 1 2 2 3 4 4 5 5 5 6 6 7 8 8 9 9 9];
sb = [2 5.5 7];
[h,j,b,i] = histc(d,sb);
href = [8 3];
jref = [3 5 0 0 0];
assert_checkequal(h, href);
assert_checkequal(j, jref);
assert_checkalmostequal(b, sb);
assert_checkequal(i, [0 0 0 1 1 1 1 1 1 1 1 2 2 2 0 0 0 0 0]);
[h, j] = histc(d, sb, "countsNorm");
assert_checkequal(h, href/11);
assert_checkequal(j, jref/11);
[h, j] = histc(d, sb, "countsNorm,normWith: leftout");
assert_checkequal(h, href/14);
assert_checkequal(j, jref/14);
[h, j] = histc(d, sb, "countsNorm,normWith: rightout");
assert_checkequal(h, href/16);
assert_checkequal(j, jref/16);
[h, j] = histc(d, sb, "countsNorm,normWith: out");
assert_checkequal(h, href/19);
assert_checkequal(j, jref/19);
[h, j] = histc(d, sb, "countsNorm,normWith: all");
assert_checkequal(h, href/19);
assert_checkequal(j, jref/19);
[h, j] = histc(d, sb, "density");
assert_checkalmostequal(h, href./diff(sb));
assert_checkalmostequal(j, jref);
[h, j] = histc(d, sb, "densityNorm");
assert_checkalmostequal(h, href./diff(sb)/11);
assert_checkalmostequal(j, jref/11);
[h, j] = histc(d, sb, "densityNorm,normWith: leftout");
assert_checkalmostequal(h, href./diff(sb)/14);
assert_checkalmostequal(j, jref/14);
[h, j] = histc(d, sb, "densityNorm,normWith: rightout");
assert_checkalmostequal(h, href./diff(sb)/16);
assert_checkalmostequal(j, jref/16);
[h, j] = histc(d, sb, "densityNorm,normWith: all");
assert_checkalmostequal(h, href./diff(sb)/19);
assert_checkalmostequal(j, jref/19);

// With Nan and Inf
// ----------------
// continuous: [leftout, rightout, zeros, nan, inf]
//d = [1 1 1 2 2 3 4 4 5 5 5 6 6 7 8 8 9 9 9];
d = [%nan %inf d %nan %nan -%inf];
N = size(d,"*");
sb = [2 4.5 7];
[h,j,b,i] = histc(d, sb);
href = [5 6];
jref = [3 5 0 3 2];
assert_checkequal(h, href);
assert_checkequal(j, jref);
assert_checkequal(i, [0 0 0 0 0 1 1 1 1 1 2 2 2 2 2 2 0 0 0 0 0 0 0 0]);
[h, j] = histc(d, sb,"countsNorm");
assert_checkequal(h, href/11);
assert_checkequal(j, jref/11);
[h, j] = histc(d, sb,"countsNorm,normWith: nan");
assert_checkequal(h, href/14);
assert_checkequal(j, jref/14);
[h, j] = histc(d, sb,"countsNorm,normWith: inf");
assert_checkequal(h, href/13);
assert_checkequal(j, jref/13);
[h, j] = histc(d, sb,"countsNorm,normWith: inf nan");
assert_checkequal(h, href/16);
assert_checkequal(j, jref/16);
[h, j] = histc(d, sb,"countsNorm,normWith: leftout nan");
assert_checkequal(h, href/17);
assert_checkequal(j, jref/17);
[h, j] = histc(d, sb,"countsNorm,normWith: rightout inf");
assert_checkequal(h, href/18);
assert_checkequal(j, jref/18);
[h, j] = histc(d, sb,"countsNorm,normWith: out inf");
assert_checkequal(h, href/21);
assert_checkequal(j, jref/21);
[h, j] = histc(d, sb,"countsNorm,normWith: all");
assert_checkequal(h, href/24);
assert_checkequal(j, jref/24);

[h, j] = histc(d, sb,"densityNorm");
assert_checkequal(h, href/11/2.5);
assert_checkequal(j, jref/11);
[h, j] = histc(d, sb,"densityNorm,normWith: nan");
assert_checkequal(h, href/14/2.5);
assert_checkequal(j, jref/14);
[h, j] = histc(d, sb,"densityNorm,normWith: inf");
assert_checkequal(h, href/13/2.5);
assert_checkequal(j, jref/13);
[h, j] = histc(d, sb,"densityNorm,normWith: inf nan");
assert_checkequal(h, href/16/2.5);
assert_checkequal(j, jref/16);
[h, j] = histc(d, sb,"densityNorm,normWith: leftout nan");
assert_checkequal(h, href/17/2.5);
assert_checkequal(j, jref/17);
[h, j] = histc(d, sb,"densityNorm,normWith: rightout inf");
assert_checkequal(h, href/18/2.5);
assert_checkequal(j, jref/18);
[h, j] = histc(d, sb,"densityNorm,normWith: all");
assert_checkequal(h, href/24/2.5);
assert_checkequal(j, jref/24);

// CATEGORIAL MODE
// The order of binsValues matters and is kept
// a) sorted, no duplicate:
d = [
1  7  4  4  1  4  3  4  8  6  8  2  3  4  6
1  6  2  7  7  3  8  7  4  3  4  4  1  5  7
];
[h,j,b,i] = histc(d, [1 2 4 7] , "discrete");
href = [4 2 8 5];
assert_checkequal(h, href);
assert_checkequal(b, [1 2 4 7]);
iref = [
1  4  3  3  1  3  0  3  0  0  0  2  0  3  0
1  0  2  4  4  0  0  4  3  0  3  3  1  0  4
];
assert_checkequal(i, iref);
// b) sorted main bins values, but duplicates
[h,j,b,i] = histc(d, [1 2 1 4 2 7 4] , "discrete");
assert_checkequal(h, href);
assert_checkequal(b, [1 2 4 7]);
assert_checkequal(i, iref);

// c) puzzled bins values, no duplicates:
[h,j,b,i] = histc(d, [2 7 1 4] , "discrete");
href = [2 5 4 8];
assert_checkequal(h, href);
assert_checkequal(b, [2 7 1 4]);
iref = [
3  2  4  4  3  4  0  4  0  0  0  1  0  4  0
3  0  1  2  2  0  0  2  4  0  4  4  3  0  2
];
assert_checkequal(i, iref);
// d) puzzled and duplicates bins values:
[h,j,b,i] = histc(d, [2 7 2 7 1 7 2 4 1 4] , "discrete");
assert_checkequal(h, href);
assert_checkequal(b, [2 7 1 4]);
assert_checkequal(i, iref);

// e) bins with %inf and %nan
d = [
1 1 1 1 1 2 2 2 2 3 3 3 4 4 5
6 7 7 8 8 8 9 9 9 9 0 0 0 0 0
];
uns = [1 1 1 1 1];
d = [d ; [-uns uns uns*0]*%inf];
//   1    1    1    1    1    2   2   2   2   3   3   3   4   4   5
//   6    7    7    8    8    8   9   9   9   9   0   0   0   0   0
//  -Inf -Inf -Inf -Inf -Inf Inf Inf Inf Inf Inf Nan Nan Nan Nan Nan
[h,j,b,i] = histc(d, [1 2 4 7 %nan -%inf] , "discrete");
bref = [1  2  4  7 -%inf]
href = [5  4  2  2  5 ];
jref = [17 0  5  5 10 ];
iref = [
1  1  1  1  1  2  2  2  2  0  0  0  3  3  0
0  4  4  0  0  0  0  0  0  0  0  0  0  0  0
5  5  5  5  5  0  0  0  0  0  0  0  0  0  0
];
assert_checkequal(h, href);
assert_checkequal(j, jref);
assert_checkequal(b, bref);
assert_checkequal(i, iref);
// e.2) = normalized by counted
[h,j,b,i] = histc(d, [1 2 4 7 %nan -%inf] , "discrete,countsNorm");
assert_checkequal(h, href/sum(href));
assert_checkequal(j, jref/sum(href));
// e.3) = normalized by all
options = "discrete,countsNorm,normwith: all";
[h,j,b,i] = histc(d, [1 2 4 7 %nan -%inf], options);
assert_checkequal(h, href/size(d,"*"));
assert_checkequal(j, jref/size(d,"*"));
// e.3) = normalized including out
options = "discrete,countsNorm,normwith: out";
[h,j,b,i] = histc(d, [1 2 4 7 %nan -%inf], options);
assert_checkequal(h, href/(sum(href)+jref(1)));
assert_checkequal(j, jref/(sum(href)+jref(1)));
// e.4) normalized including Inf: should change nothing, since Inf and -Inf
//      are already included.
options = "discrete,countsNorm,normwith: inf";
[h,j,b,i] = histc(d, [1 2 4 7 %nan -%inf], options);
assert_checkequal(h, href/(sum(href)+5));
assert_checkequal(j, jref/(sum(href)+5));
// e.5) normalized including Nan
options = "discrete,countsNorm,normwith: nan";
[h,j,b,i] = histc(d, [1 2 4 7 %nan -%inf], options);
assert_checkequal(h, href/(sum(href)+jref(4)));
assert_checkequal(j, jref/(sum(href)+jref(4)));

// =============================
// WITH SPARSE MATRIX OF NUMBERS
// =============================


// =====================
// WITH ENCODED INTEGERS
// =====================
assert_checkequal(histc(int16(37)), 1);

// ==========
// WITH TEXTS
// ==========
// Discrete bins
// -------------
assert_checkequal(histc("abcd"), 1);
assert_checkequal(histc("abcd",["a" "b"]), 1);
assert_checkequal(histc("cbcd",["a" "b"]), 0);
assert_checkequal(histc(["a" "c" "a" "a" "b" "c"]), [3 1 2]);

t = [
"c" "n" "h" "i" "b" "i" "f" "i" "p" "l" "p" "d" "f" "i" "l"
"b" "m" "e" "o" "o" "f" "p" "o" "h" "f" "h" "h" "c" "k" "o"
"p" "f" "k" "a" "j" "o" "j" "d" "h" "h" "n" "m" "o" "l" "n"
"h" "b" "o" "l" "j" "n" "o" "i" "g" "i" "a" "a" "j" "d" "p"
];
// With default discrete bins:
[h,j,b,i] = histc(t);
assert_checkequal(h, [3 3 2 3 1 5 1 7 6 4 2 4 2 4 8 5]);
assert_checkequal(j, [0 0 0]);
assert_checkequal(b, asciimat((97:112)')');
iref = [
3   14  8   9   2   9   6   9   16  12  16  4   6   9   12
2   13  5   15  15  6   16  15  8   6   8   8   3   11  15
16  6   11  1   10  15  10  4   8   8   14  13  15  12  14
8   2   15  12  10  14  15  9   7   9   1   1   10  4   16
];
assert_checkequal(i, iref);
// With given discrete bins WITHOUT "" bins:
t2 = t;
t2([7 13 19 26 32 39 43]) = "";
// --> t2  =
// c  n  h     b  i  f  i  p  l  p  d  f  i  l
// b  m  e  o  o  f     o  h  f  h  h  c  k  o
// p     k  a     o  j  d  h        m  o  l  n
// h  b  o  l  j  n  o     g  i  a  a  j  d  p
//
// b =  '' a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p
// h =  7  3  3  2  3  1  4  1  6  4  3  2  4  2  3  8  4
href = [3 1 4 8];
N = sum(href);
jref = [37 0 7];
bref = ["a" "e" "i" "o"];
iref = [
0  0  0  0  0  3  0  3  0  0  0  0  0  3  0
0  0  2  4  4  0  0  4  0  0  0  0  0  0  4
0  0  0  1  0  4  0  0  0  0  0  0  4  0  0
0  0  4  0  0  0  4  0  0  3  1  1  0  0  0
];
[h,j,b,i] = histc(t2, bref, "discrete");
assert_checkequal(h, href);
assert_checkequal(j, jref);
assert_checkequal(i, iref);
// With given discrete bins WITHOUT "" bins: Normalized
[h,j,b,i] = histc(t2, bref, "discrete,countsNorm");
assert_checkequal(h, href/16);
assert_checkequal(j, jref/16);
[h,j,b,i] = histc(t2, bref, "discrete,countsNorm,normWith: empty");
assert_checkequal(h, href/(16+7));
assert_checkequal(j, jref/(16+7));
[h,j,b,i] = histc(t2, bref, "discrete,countsNorm,normWith: all");
assert_checkequal(h, href/60);
assert_checkequal(j, jref/60);
[h,j,b,i] = histc(t2, bref, "discrete,countsNorm,normWith: out");
assert_checkequal(h, href/(N+jref(1)));
assert_checkequal(j, jref/(N+jref(1)));
// With given discrete bins WITH "" bins: Normalized
href = [3 1 4 8 7];
N = sum(href);
jref = [37 0 7];
[h,j,b,i] = histc(t2, [bref ""], "discrete,countsNorm");
assert_checkequal(h, href/N);
assert_checkequal(j, jref/N);
[h,j,b,i] = histc(t2, [bref ""], "discrete,countsNorm,normWith: empty");
assert_checkequal(h, href/N);
assert_checkequal(j, jref/N);
[h,j,b,i] = histc(t2, [bref ""], "discrete,countsNorm,normWith: out");
assert_checkequal(h, href/60);
assert_checkequal(j, jref/60);

// Continuous bins. Data WITHOUT ""
// --------------------------------
[h,j,b,i] = histc(t, ["a" "c" "e" "g" "i" "k" "m" "o" "q"]);
assert_checkequal(h, [8  4  6  13  6  6  12  5]);
assert_checkequal(j, [0 0 0]);
iref = [
1  7  4  4  1  4  3  4  8  6  8  2  3  4  6
1  6  2  7  7  3  8  7  4  3  4  4  1  5  7
8  3  5  1  5  7  5  2  4  4  7  6  7  6  7
4  1  7  6  5  7  7  4  3  4  1  1  5  2  8
];
assert_checkequal(i, iref);
// With continuous bins and outsiders:
[h,j,b,i] = histc(t, ["c" "e" "g" "i" "k" "m"]);
assert_checkequal(h, [6 6 13 6 6]);
assert_checkequal(j, [6 17 0]);
iref = [
1  0  3  3  0  3  2  3  0  5  0  1  2  3  5
0  5  1  0  0  2  0  0  3  2  3  3  1  4  0
0  2  4  0  4  0  4  1  3  3  0  5  0  5  0
3  0  0  5  4  0  0  3  2  3  0  0  4  1  0
];
assert_checkequal(i, iref);
// With continuous and marginal bins: "" <=> -inf , "~~" <=> Inf (regular ascii)
[h,j,b,i] = histc(t, ["" "c" "e" "g" "i" "k" "m" "~~"]);
assert_checkequal(h, [8 4 6 13 6 6 17]);
assert_checkequal(j, [0 0 0]);
iref = [
1  7  4  4  1  4  3  4  7  6  7  2  3  4  6
1  6  2  7  7  3  7  7  4  3  4  4  1  5  7
7  3  5  1  5  7  5  2  4  4  7  6  7  6  7
4  1  7  6  5  7  7  4  3  4  1  1  5  2  7
];
assert_checkequal(i, iref);

// Continuous bins. Data WITH ""
// ------------------------------
// t2  =
// c  n  h     b  i  f  i  p  l  p  d  f  i  l
// b  m  e  o  o  f     o  h  f  h  h  c  k  o
// p     k  a     o  j  d  h        m  o  l  n
// h  b  o  l  j  n  o     g  i  a  a  j  d  p
//
// b =  '' a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p
// h =  7  3  3  2  3  1  4  1  6  4  3  2  4  2  3  8  4
[h,j,b,i] = histc(t2, ["e" "f" "g" "h" "i" "j"]);
href = [5 1 6 4 3]; N = sum(href);
jref = [11  23  7];
[h,j,b,i] = histc(t2, ["e" "f" "g" "h" "i" "j"],"countsNorm,normWith: leftout");
assert_checkequal(h, href/(N+jref(1)));
assert_checkequal(j, jref/(N+jref(1)));
[h,j,b,i] = histc(t2, ["e" "f" "g" "h" "i" "j"],"countsNorm,normWith: rightout");
assert_checkequal(h, href/(N+jref(2)));
assert_checkequal(j, jref/(N+jref(2)));
[h,j,b,i] = histc(t2, ["e" "f" "g" "h" "i" "j"],"countsNorm,normWith: out");
assert_checkequal(h, href/sum([N jref(1:2)]));
assert_checkequal(j, jref/sum([N jref(1:2)]));
[h,j,b,i] = histc(t2, ["e" "f" "g" "h" "i" "j"],"countsNorm,normWith: empty");
assert_checkequal(h, href/(N+jref(3)));
assert_checkequal(j, jref/(N+jref(3)));
[h,j,b,i] = histc(t2, ["e" "f" "g" "h" "i" "j"],"countsNorm,normWith: out empty");
assert_checkequal(h, href/sum([N jref]));
assert_checkequal(j, jref/sum([N jref]));
[h,j,b,i] = histc(t2, ["e" "f" "g" "h" "i" "j"],"countsNorm,normWith: all");
assert_checkequal(h, href/sum([N jref]));
assert_checkequal(j, jref/sum([N jref]));


// ================
// WITH POLYNOMIALS
// ================
assert_checkequal(histc(%z), 1);
assert_checkequal(histc([%z %z 2+%z]), [2 1]);
assert_checkequal(histc([%z %z 2+%z],,"countsnorm"), [2 1]/3);
assert_checkequal(histc([%z %z 2+%z %nan],,"countsnorm"), [2 1]/3);
assert_checkequal(histc([%z %z 2+%z %nan],,"countsnorm,normWith: Nan"), [2 1]/4);  // A VOIR
// Data order is kept
assert_checkequal(histc([2+%z %z %z ]), [1 2]);


// ==============================
// CHECKING ERRORS AND TOLERANCES
// ==============================
t = ["c" "n" "h" "i" "b" "i" "f" "i" "p" "l" "p" "d" "f" "i" "l"];
// Wrong number of arguments
msg = _("%s: Wrong number of input arguments: %d to %d expected.\n");
msg = msprintf(msg, "histc", 1, 3);
assert_checkerror("histc()", msg);
// Too many input arguments
assert_checkfalse(execstr("histc(1,2,3,4)", "errcatch")==0);
// Too many output arguments
assert_checkfalse(execstr("[a,b,c,d,e] = histc(rand(1,100))", "errcatch")==0);
// Wrong Data types
msg = _("%s: Argument #%d: %s\n");
msg = msprintf(msg, "histc", 1, "data array missing");
assert_checkerror("histc(,,3)", msg);
msg = _("histc: Data in argument #1: Numbers or polynomials or texts expected.");
assert_checkerror("histc(rand(1,100)<0.5)", msg);       // boolean
assert_checkerror("histc(sprand(1,100,0.1)>0.5)", msg); // sparse booleans
assert_checkerror("histc(list(%pi, 1, %e))", msg);      // list
assert_checkerror("histc({1, 2, 3, 4})", msg);          // cells
s(1:3).r = %pi;
assert_checkerror("histc(s)", msg);                     // structs
// Number of bins must be integer > 0:
msg = _("%s: Argument #%d: non-zero decimal integer expected.\n");
msg = msprintf(msg, "histc", 2);
assert_checkerror("histc(rand(1,100), 1.5)", msg);
assert_checkerror("histc(rand(1,100), 0)", msg);
assert_checkerror("histc(rand(1,100), %inf)", msg);
assert_checkerror("histc(rand(1,100), %nan)", msg);
// Bins width must not be %inf
msg = _("%s: Argument #%d: decimal number > -Inf expected.\n");
msg = msprintf(msg, "histc", 2);
assert_checkerror("histc(rand(1,100), -%inf)", msg);

// Mismatching Data and binsEdges types
msg = _("%s: Arguments #%d and #%d: Same types expected.\n")
msg = msprintf(msg, "histc", 1, 2);
assert_checkerror("histc(t, 1:3)", msg);
assert_checkerror("histc(rand(1,100), [""a"" ""b"" ""c""])", msg);
assert_checkerror("histc(int8(rand(1,100)*100), [""a"" ""b"" ""c""])", msg);
// Mismatching Data and binsValues types
assert_checkerror("histc(t, 1:3, ""discrete"")", msg);
assert_checkerror("histc(rand(1,100), [""a"" ""b""], ""discrete"")", msg);
assert_checkerror("histc(int8(rand(1,100)*100), [""a"" ""b""], ""discrete"")", msg);
// Wrong algo name
msg = msprintf(_("%s: Argument #%d: wrong value for binning algo"),"histc",2);
assert_checkerror("histc(rand(1,100), ""foo"")", msg);
// Unapplicable binning modes:
msg = msprintf( _("%s: Argument #2: Please provide bins edges or values or leave choosing default bins.\n"), "histc");
assert_checkerror("histc(t, ""sqrt"")", msg);
assert_checkerror("histc(t, 3)", msg);
assert_checkerror("histc(t, -3)", msg);
// Wrong options type
msg = _("%s: Argument #%d: Text expected.\n");
msg = msprintf(msg, "histc", 3);
assert_checkerror("histc(rand(1,100),,%t)", msg);
assert_checkerror("histc(rand(1,100),,1)", msg);
assert_checkerror("histc(rand(1,100),,int8(1))", msg);
// Unexisting options are ignored
assert_checkequal(execstr("histc(rand(1,1000),,""foo"")", "errcatch"), 0);
// Unrelevant options are ignored
tmp = "histc(t,,""density"")";
assert_checkequal(execstr(tmp, "errcatch"), 0);
// Duplicate options are ignored
tmp = "histc(grand(1,100,""uin"",0,20),,""discrete,discrete"")";
assert_checkequal(execstr(tmp, "errcatch"), 0);
// Useless option is ignored
tmp = "histc([%z 2*%z %z],,""discrete"")";
assert_checkequal(execstr(tmp, "errcatch"), 0);
// Useless normWith: flags are ignored
tmp = "histc(rand(1,100),,""counts,normWith: inf"")";
assert_checkequal(execstr(tmp, "errcatch"), 0);
// Bad normWith: flags are ignored
tmp = "histc(rand(1,100),,""countsNorm,normWith: foo"")";
assert_checkequal(execstr(tmp, "errcatch"), 0);

// ===========================================================================
//                                  SCILAB 5.5
// ===========================================================================
rand("seed", 0);
d = rand(1, 10000, "normal");
[cfC, indC] = histc(20, d);

refCF = [
   0.0002572096
   0.00102883842
   0.00514419209
   0.00925954575
   0.02700700845
   0.07356194682
   0.12860480213
   0.20576768341
   0.30376454264
   0.36472321885
   0.39713162899
   0.36317996123
   0.28987522401
   0.19033510716
   0.11008571063
   0.06121588582
   0.02674979884
   0.01028838417
   0.00282930565
   0.00128604802
]';
assert_checkequal(size(indC), [1 10000]);
assert_checkalmostequal(cfC, refCF,1e-7);
[cfC, indC] = histc(int32(20), d);
assert_checkequal(size(indC), [1 10000]);
assert_checkalmostequal(cfC, refCF, 1e-7);

// Same test, without normalization
[cfC, indC] = histc(20, d, normalization=%f);

refCF = [
1
4
20
36
105
286
500
800
1181
1418
1544
1412
1127
740
428
238
104
40
11
5 ]';
assert_checkequal(size(indC), [1 10000]);
assert_checkequal(cfC, refCF);
[cfC, indC] = histc(int16(20), d, normalization=%f);
assert_checkequal(size(indC), [1 10000]);
assert_checkequal(cfC, refCF);


// With x instead of n as first argument
cfC = histc([-5 0 5], d);
refCF = [0.4991 0.5009]/5;
assert_checkequal(cfC, refCF);
cfC = histc(int8([-5 0 5]), d);
assert_checkequal(cfC, refCF);
cfC = histc(int16([-5 0 5]), d);
assert_checkequal(cfC, refCF);
cfC = histc(int32([-5 0 5]), d);
assert_checkequal(cfC, refCF);
assert_checkequal(sum(cfC)*5, 1);
cfC = histc([-5 0 5], d, %f);
assert_checkequal(sum(cfC), 10000); // -5 > d > 5, and d has 10000 elements.



lambda = 2;
grand("setsd", 0);
D = grand(100000, 1, "exp", 1/lambda);
[cfC, indC] = histc(40, D);

refCF = [
   1.6987118D+00
   1.2440861D+00
   9.0843138D-01
   6.5661150D-01
   4.8963575D-01
   3.5287585D-01
   2.5718177D-01
   1.9447913D-01
   1.4149098D-01
   9.7271105D-02
   7.3615681D-02
   5.0401824D-02
   3.8416409D-02
   2.8512672D-02
   2.1447585D-02
   1.4634823D-02
   1.0660711D-02
   8.2005472D-03
   5.8665453D-03
   4.2895170D-03
   4.1633547D-03
   1.3877849D-03
   1.4508660D-03
   9.4621698D-04
   7.5697358D-04
   7.5697358D-04
   4.4156792D-04
   3.1540566D-04
   2.5232453D-04
   1.8924340D-04
   6.3081132D-05
   1.8924340D-04
   1.2616226D-04
   6.3081132D-05
   6.3081132D-05
   0.0000000D+00
   0.0000000D+00
   6.3081132D-05
   0.0000000D+00
   6.3081132D-05
 ]';
assert_checkequal(size(indC), [100000 1]);
assert_checkalmostequal(cfC, refCF, 1e-7);
[cfC, indC] = histc(int8(40), D);
assert_checkequal(size(indC), [100000 1]);
assert_checkalmostequal(cfC, refCF, 1e-7);

// Same test, without normalization
[cfC, indC] = histc(40, D, normalization=%f);
assert_checkequal(size(indC), [100000 1]);

refCF = [
26929
19722
14401
10409
7762
5594
4077
3083
2243
1542
1167
799
609
452
340
232
169
130
93
68
66
22
23
15
12
12
7
5
4
3
1
3
2
1
1
0
0
1
0
1 ]';
assert_checkequal(size(indC), [100000 1]);
assert_checkalmostequal(cfC, refCF);
[cfC, indC] = histc(int32(40), D, normalization=%f);
assert_checkequal(size(indC), [100000 1]);
assert_checkalmostequal(cfC, refCF);

// With x instead of n as first argument
cfC = histc([0 7], D);
refCF = 1/7;
assert_checkalmostequal(cfC, refCF);
cfC = histc(int8([0 7]), D);
assert_checkalmostequal(cfC, refCF);
cfC = histc(int16([0 7]), D);
assert_checkalmostequal(cfC, refCF);
cfC = histc(int32([0 7]), D);
assert_checkalmostequal(cfC, refCF);
cfC = histc([0 7], D, %f);
assert_checkequal(cfC, 100000); // 0 > D > 7, and D has 100000 elements.
