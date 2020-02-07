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
//             Tests of multilevel gsort() for texts
//====================================================================

t = [
  "cc"    "ca"    "ab"    "bbca"  "b"     "ccbc"  "aab"   "bca"
  "ac"    "bba"   "aba"   "bb"    "a"     "cac"   "b"     "b"
  "aaaa"  "ac"    "b"     "bbca"  "bb"    "bc"    "aa"    "ca"
  "c"     "ba"    "cbb"   "a"     "aab"   "abbb"  "ac"    "c"
  "cbb"   "b"     "cabb"  "bccc"  "aba"   "acb"   "acb"   "b"
  "cba"   "cc"    "a"     "abbb"  "ab"    "cc"    "bba"   "caaa"
  ];

// "g" multilevel sorting
// ----------------------
[r, k] = gsort(t, "g", ["i" "i"], list(length,:));
ref = [
  "a"  "b"   "ab"  "bb"  "cc"   "acb"  "cba"   "bbca"
  "a"  "b"   "ab"  "bb"  "cc"   "acb"  "cbb"   "bbca"
  "a"  "b"   "ac"  "bc"  "aab"  "bba"  "cbb"   "bccc"
  "b"  "c"   "ac"  "ca"  "aab"  "bba"  "aaaa"  "caaa"
  "b"  "c"   "ac"  "ca"  "aba"  "bca"  "abbb"  "cabb"
  "b"  "aa"  "ba"  "cc"  "aba"  "cac"  "abbb"  "ccbc"
  ];
kref = [
  18  38  13  20  12  35   6  19
  22  44  30  27  36  41   5  21
  26  47   2  33  28   8  16  23
  11   4   9   7  37  42   3  48
  15  46  40  45  14  43  24  17
  25  39  10   1  29  32  34  31
  ];
assert_checkequal(r, ref);
assert_checkequal(k, kref);
//
[r, k] = gsort(t, "g", ["i" "d"], list(length,:));
ref = [
  "c"  "b"   "cc"  "bb"  "ab"   "bca"  "aba"   "bccc"
  "c"  "b"   "cc"  "ba"  "aa"   "bba"  "aab"   "bbca"
  "b"  "a"   "ca"  "ac"  "cbb"  "bba"  "aab"   "bbca"
  "b"  "a"   "ca"  "ac"  "cbb"  "acb"  "ccbc"  "abbb"
  "b"  "a"   "bc"  "ac"  "cba"  "acb"  "cabb"  "abbb"
  "b"  "cc"  "bb"  "ab"  "cac"  "aba"  "caaa"  "aaaa"
  ];
kref = [
   4  44  12  27  30  43  29  23
  46  47  36  10  39   8  28  19
  11  18   7   2   5  42  37  21
  15  22  45   9  16  35  31  24
  25  26  33  40   6  41  17  34
  38   1  20  13  32  14  48   3
  ];
assert_checkequal(r, ref);
assert_checkequal(k, kref);

// "r" and "c" multilevel sorting
// ------------------------------
[r, k] = gsort(t, "c", ["i" "i"], list(length,:));
ref = [
  "b"  "ab"  "ca"   "cc"   "aab"  "bca"  "bbca"  "ccbc"
  "a"  "b"   "b"    "ac"   "bb"   "aba"  "bba"   "cac"
  "b"  "aa"  "ac"   "bb"   "bc"   "ca"   "aaaa"  "bbca"
  "a"  "c"   "c"    "ac"   "ba"   "aab"  "cbb"   "abbb"
  "b"  "b"   "aba"  "acb"  "acb"  "cbb"  "bccc"  "cabb"
  "a"  "ab"  "cc"   "cc"   "bba"  "cba"  "abbb"  "caaa"
  ];
kref = [
  5  3  2  1  7  8  4  6
  5  7  8  1  4  3  2  6
  3  7  2  5  6  8  1  4
  4  1  8  7  2  5  3  6
  2  8  5  6  7  1  4  3
  3  5  2  6  7  1  4  8
  ];
assert_checkequal(r, ref);
assert_checkequal(k, kref);
//
[r, k] = gsort(t, "c", ["d" "i"], list(length,:));
ref = [
  "bbca"  "ccbc"  "aab"  "bca"  "ab"   "ca"   "cc"  "b"
  "aba"   "bba"   "cac"  "ac"   "bb"   "a"    "b"   "b"
  "aaaa"  "bbca"  "aa"   "ac"   "bb"   "bc"   "ca"  "b"
  "abbb"  "aab"   "cbb"  "ac"   "ba"   "a"    "c"   "c"
  "bccc"  "cabb"  "aba"  "acb"  "acb"  "cbb"  "b"   "b"
  "abbb"  "caaa"  "bba"  "cba"  "ab"   "cc"   "cc"  "a"
  ];
kref = [
  4  6  7  8  3  2  1  5
  3  2  6  1  4  5  7  8
  1  4  7  2  5  6  8  3
  6  5  3  7  2  4  1  8
  4  3  5  6  7  1  2  8
  4  8  7  1  5  2  6  3
  ];
assert_checkequal(r, ref);
assert_checkequal(k, kref);
//
[r, k] = gsort(t, "r", ["i" "i"], list(length,:));
ref = [
  "c"     "b"    "a"     "a"     "a"    "bc"    "b"    "b"
  "ac"    "ac"   "b"     "bb"    "b"    "cc"    "aa"   "b"
  "cc"    "ba"   "ab"    "abbb"  "ab"   "acb"   "ac"   "c"
  "cba"   "ca"   "aba"   "bbca"  "bb"   "cac"   "aab"  "ca"
  "cbb"   "cc"   "cbb"   "bbca"  "aab"  "abbb"  "acb"  "bca"
  "aaaa"  "bba"  "cabb"  "bccc"  "aba"  "ccbc"  "bba"  "caaa"
  ];
kref = [
  4  5  6  4  2  3  2  2
  2  3  3  2  1  6  3  5
  1  4  1  6  6  5  4  4
  6  1  2  1  3  2  1  3
  5  6  4  3  4  4  5  1
  3  2  5  5  5  1  6  6
  ];
assert_checkequal(r, ref);
assert_checkequal(k, kref);
//
[r, k] = gsort(t, "r", ["d" "i"], list(length,:));
ref = [
  "aaaa"  "bba"  "cabb"  "abbb"  "aab"  "abbb"  "aab"  "caaa"
  "cba"   "ac"   "aba"   "bbca"  "aba"  "ccbc"  "acb"  "bca"
  "cbb"   "ba"   "cbb"   "bbca"  "ab"   "acb"   "bba"  "ca"
  "ac"    "ca"   "ab"    "bccc"  "bb"   "cac"   "aa"   "b"
  "cc"    "cc"   "a"     "bb"    "a"    "bc"    "ac"   "b"
  "c"     "b"    "b"     "a"     "b"    "cc"    "b"    "c"
  ];
kref = [
  3  2  5  6  4  4  1  6
  6  3  2  1  5  1  5  1
  5  4  4  3  6  5  6  3
  2  1  1  5  3  2  3  2
  1  6  6  2  2  3  4  5
  4  5  3  4  1  6  2  4
  ];
assert_checkequal(r, ref);
assert_checkequal(k, kref);

// Lexicographic multilevel sorting
// --------------------------------
t = [
  "aa"  "bb"  "b"   "aa"  "b"   "b"   "bb"  "bb"
  "b"   "b"   "b"   "aa"  "a"   "a"   "ab"  "a"
  "bb"  "b"   "bb"  "b"   "a"   "b"   "ba"  "b"
  "b"   "b"   "ba"  "a"   "b"   "b"   "ba"  "bb"
  "ba"  "aa"  "ba"  "ba"  "ba"  "ab"  "a"   "aa"
  "b"   "a"   "bb"  "a"   "aa"  "b"   "ab"  "b"
  "b"   "ab"  "aa"  "ba"  "ab"  "b"   "a"   "ba"
  "b"   "aa"  "bb"  "aa"  "bb"  "a"   "bb"  "a"
    ];
[r, k] = gsort(t, "lr", ["i" "i"], list(length,:));
ref = [
  "b"   "a"   "bb"  "a"   "aa"  "b"   "ab"  "b"
  "b"   "b"   "b"   "aa"  "a"   "a"   "ab"  "a"
  "b"   "b"   "ba"  "a"   "b"   "b"   "ba"  "bb"
  "b"   "aa"  "bb"  "aa"  "bb"  "a"   "bb"  "a"
  "b"   "ab"  "aa"  "ba"  "ab"  "b"   "a"   "ba"
  "aa"  "bb"  "b"   "aa"  "b"   "b"   "bb"  "bb"
  "ba"  "aa"  "ba"  "ba"  "ba"  "ab"  "a"   "aa"
  "bb"  "b"   "bb"  "b"   "a"   "b"   "ba"  "b"
  ];
assert_checkequal(r, ref);
assert_checkequal(k', [6  2  4  8  7  1  5  3]);

//
[r, k] = gsort(t, "lr", ["d" "i"], list(length,:));
ref = [
  "aa"  "bb"  "b"   "aa"  "b"   "b"   "bb"  "bb"
  "ba"  "aa"  "ba"  "ba"  "ba"  "ab"  "a"   "aa"
  "bb"  "b"   "bb"  "b"   "a"   "b"   "ba"  "b"
  "b"   "aa"  "bb"  "aa"  "bb"  "a"   "bb"  "a"
  "b"   "ab"  "aa"  "ba"  "ab"  "b"   "a"   "ba"
  "b"   "a"   "bb"  "a"   "aa"  "b"   "ab"  "b"
  "b"   "b"   "ba"  "a"   "b"   "b"   "ba"  "bb"
  "b"   "b"   "b"   "aa"  "a"   "a"   "ab"  "a"
  ];
assert_checkequal(r, ref);
assert_checkequal(k', [1  5  3  8  7  6  4  2]);
//
[r, k] = gsort(t, "lc", ["i" "i"], list(length,:));
ref = [
  "b"   "b"   "b"   "aa"  "aa"  "bb"  "bb"  "bb"
  "a"   "a"   "b"   "b"   "aa"  "a"   "b"   "ab"
  "a"   "b"   "bb"  "bb"  "b"   "b"   "b"   "ba"
  "b"   "b"   "ba"  "b"   "a"   "bb"  "b"   "ba"
  "ba"  "ab"  "ba"  "ba"  "ba"  "aa"  "aa"  "a"
  "aa"  "b"   "bb"  "b"   "a"   "b"   "a"   "ab"
  "ab"  "b"   "aa"  "b"   "ba"  "ba"  "ab"  "a"
  "bb"  "a"   "bb"  "b"   "aa"  "a"   "aa"  "bb"
  ];
assert_checkequal(r, ref);
assert_checkequal(k, [5  6  3  1  4  8  2  7]);
//
[r, k] = gsort(t, "lc", ["d" "i"], list(length,:));
ref = [
  "aa"  "aa"  "bb"  "bb"  "bb"  "b"   "b"   "b"
  "aa"  "b"   "ab"  "a"   "b"   "a"   "a"   "b"
  "b"   "bb"  "ba"  "b"   "b"   "a"   "b"   "bb"
  "a"   "b"   "ba"  "bb"  "b"   "b"   "b"   "ba"
  "ba"  "ba"  "a"   "aa"  "aa"  "ba"  "ab"  "ba"
  "a"   "b"   "ab"  "b"   "a"   "aa"  "b"   "bb"
  "ba"  "b"   "a"   "ba"  "ab"  "ab"  "b"   "aa"
  "aa"  "b"   "bb"  "a"   "aa"  "bb"  "a"   "bb"
  ];
assert_checkequal(r, ref);
assert_checkequal(k, [4  1  7  8  2  5  6  3]);
