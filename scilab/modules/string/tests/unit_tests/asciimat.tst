// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
// Copyright (C) 2020-2021 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// Empty
assert_checkequal(asciimat([]), []);
assert_checkequal(asciimat(["" ""]), []);
assert_checkequal(asciimat(["";""]), []);
assert_checkequal(asciimat(["" "";"" ""]), []);

// US-ASCII
// ========
// Scalar
assert_checkequal(asciimat(""), []);
assert_checkequal(asciimat("a"), 97);
assert_checkequal(asciimat("abc"), 97:99);
assert_checkequal(asciimat(0), "");
assert_checkequal(asciimat(97), "a");
assert_checkequal(asciimat(97:99), "abc");

// Mat with all lengths equal
t = ["ab" ; " c" ; "d "];
ref  = [ 97  98 ; 32 99 ; 100 32];
assert_checkequal(asciimat(t), ref);
assert_checkequal(asciimat(ref), ["ab" ; " c" ; "d"]);

t = ["a" "b" "c" ; "d" "e" "f"];
ref  = [ 97  98  99 ;
        100 101 102];
assert_checkequal(asciimat(t), ref);
assert_checkequal(asciimat(ref), ["abc" ; "def"]);

// Matrix with various lengths, including "" :
//   * Matlab & Octave DELETE ""
//   * Octave PADDS with ascii(32)
t = ["a" ; "ab" ; ""; "  " ; "ab d"];
ref  = [ 97 32 32 32
         97 98 32 32
         32 32 32 32
         32 32 32 32
         97 98 32 100 ];
assert_checkequal(asciimat(t), ref);
    // Matlab and Octave trim trailing spaces ascii(32)
assert_checkequal(asciimat(ref), ["a"; "ab"; "" ; "" ; "ab d"]);

// Hypermat
t = [t, flipdim(t,1)];
t = cat(3, t,t);
ref = [97,97,98,32,100;97,98,32,32,32;32,32,32,32,32;32,32,97,98,32;97,98,32,100,97];
ref = cat(3, ref, ref);
assert_checkequal(asciimat(t), ref);
tmp = ["aab d" ; "ab" ; "" ; "  ab" ; "ab da"];
assert_checkequal(asciimat(ref), cat(3, tmp,tmp));

// UTF-8
// =====
// ascimmat() uses Octave's encoding = UTF-8 + trims trailing spaces
t1 = "àâãäéèêë";
t2 = "ìîïòôöùûü";
assert_checkequal(asciimat(asciimat(t2)), t2);
assert_checkequal(asciimat(asciimat([t1 t2])), t1+t2);
assert_checkequal(asciimat(asciimat([t1 ; t2])), [t1 ; t2]);
assert_checkequal(asciimat(asciimat([t1 t2 ; t2 t1])), [t1+t2 ; t2+t1]);

// Scalar
assert_checkequal(asciimat("α"), [206 177]);
assert_checkequal(asciimat("βcδ"), [206 178 99 206 180]);
assert_checkequal(asciimat("abc"), 97:99);

x = ["a €" "bδ " ; "   " "45ε"]
// for x(:)
ref = [97  32 226 130 172
       32  32  32  32  32
       98 206 180  32  32
       52  53 206 181  32 ];
assert_checkequal(asciimat(x(:)), ref);
assert_checkequal(asciimat(ref), ["a €" "" "bδ" "45ε"]');

// for x matrix
ref = [97 32 226 130  172  98  206  180   32
       32 32  32  52   53  206 181   32   32 ];
assert_checkequal(asciimat(x), ref);
assert_checkequal(asciimat(ref), ["a €bδ" ; "   45ε"]);

// 3D hypermat
x2 = ["αab" "βδc" ; "ε4" ""];
h = cat(3, x, x2)
/* h  = 
(:,:,1)
  "a €"  "bδ "
  "   "  "45ε"
(:,:,2)
  "αab"  "βδc"
  "ε4"   ""   
*/
ref = cat(3, [97  32 226 130 172  98  206 180  32.
              32  32  32  52  53 206  181  32  32 ],..
            [206 177  97  98 206 178  206 180  99
             206 181  52  32  32  32   32  32  32 ]);
assert_checkequal(asciimat(h), ref);
refS = cat(3,["a €bδ";"   45ε"],["αabβδc";"ε4"]);
assert_checkequal(asciimat(ref), refS);

// 12D hypermat
h = cat(12, ["€" "bδ"], ["" "45ε"]);
ref = cat(12, [226  130  172  98  206 180 ],..
              [ 52   53  206 181   32  32 ]);
assert_checkequal(asciimat(h), ref);
assert_checkequal(asciimat(ref), cat(12,"€bδ","45ε"));
