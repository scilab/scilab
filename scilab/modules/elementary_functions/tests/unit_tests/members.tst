// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- ENGLISH IMPOSED -->
//

// Vectors
[nb, loc] = members([], [], "last");
assert_checkequal(nb , []);
assert_checkequal(loc, []);
[nb, loc] = members([], []);
assert_checkequal(nb , []);
assert_checkequal(loc, []);

[nb, loc] = members([1 2], [3 4], "last");
assert_checkequal(nb , [0 0]);
assert_checkequal(loc, [0 0]);
[nb, loc] = members([1 2], [3 4]);
assert_checkequal(nb , [0 0]);
assert_checkequal(loc, [0 0]);

[nb, loc] = members([1 2; 3 4], [5 6; 7 8], "last");
assert_checkequal(nb , [0 0; 0 0]);
assert_checkequal(loc, [0 0; 0 0]);
[nb, loc] = members([1 2; 3 4], [5 6; 7 8]);
assert_checkequal(nb , [0 0; 0 0]);
assert_checkequal(loc, [0 0; 0 0]);

[nb, loc] = members(string([1 2; 3 4]), string([5 6; 7 8]), "last");
assert_checkequal(nb , [0 0; 0 0]);
assert_checkequal(loc, [0 0; 0 0]);
[nb, loc] = members(string([1 2; 3 4]), string([5 6; 7 8]));
assert_checkequal(nb , [0 0; 0 0]);
assert_checkequal(loc, [0 0; 0 0]);

A = [1 8 4 5 2 1];
S = [9 7 4 2 1 4];

[nb, loc] = members(A, S, "last");
assert_checkequal(nb , [1 0 2 0 1 1]);
assert_checkequal(loc, [5 0 6 0 4 5]);
[nb, loc] = members(A, S);
assert_checkequal(nb , [1 0 2 0 1 1]);
assert_checkequal(loc, [5 0 3 0 4 5]);


// Strings
A = 'a'+string(A);
S = 's'+string(S);

[nb, loc] = members(A, S, "last");
assert_checkequal(nb , [0 0 0 0 0 0]);
assert_checkequal(loc, [0 0 0 0 0 0]);
[nb, loc] = members(A, S);
assert_checkequal(nb , [0 0 0 0 0 0]);
assert_checkequal(loc, [0 0 0 0 0 0]);

A = ["elt1" "elt3" "elt4"];
S = ["elt5" "elt1" "elt3"];

[nb, loc] = members(A, S, "last");
assert_checkequal(nb , [1 1 0]);
assert_checkequal(loc, [2 3 0]);
[nb, loc] = members(A, S);
assert_checkequal(nb , [1 1 0]);
assert_checkequal(loc, [2 3 0]);

A = ["elt1" "elt3" "elt4"];
S = ["elt5" "elt6" "elt2" "elt1" "elt3"];

[nb, loc] = members(A, S, "last");
assert_checkequal(nb , [1 1 0]);
assert_checkequal(loc, [4 5 0]);
[nb, loc] = members(A, S);
assert_checkequal(nb , [1 1 0]);
assert_checkequal(loc, [4 5 0]);


// Integers
A = int16([1 8 4 5 2 1]);
S = int16([9 7 4 2 1 4]);

[nb, loc] = members(A, S, "last");
assert_checkequal(nb , [1 0 2 0 1 1]);
assert_checkequal(loc, [5 0 6 0 4 5]);
[nb, loc] = members(A, S);
assert_checkequal(nb , [1 0 2 0 1 1]);
assert_checkequal(loc, [5 0 3 0 4 5]);

A = uint8([1 8 4 5 2 1]);
S = uint8([9 7 4 2 1 4]);

[nb, loc] = members(A, S, "last");
assert_checkequal(nb , [1 0 2 0 1 1]);
assert_checkequal(loc, [5 0 6 0 4 5]);
[nb, loc] = members(A, S);
assert_checkequal(nb , [1 0 2 0 1 1]);
assert_checkequal(loc, [5 0 3 0 4 5]);


// Complexes
A = [1 8 4 5 2 1]*%i;
S = [9 7 4 2 1 4]*%i;

[nb, loc] = members(A, S, "last");
assert_checkequal(nb , [1 0 2 0 1 1]);
assert_checkequal(loc, [5 0 6 0 4 5]);
[nb, loc] = members(A, S);
assert_checkequal(nb , [1 0 2 0 1 1]);
assert_checkequal(loc, [5 0 3 0 4 5]);


// Matrices
A = [ 5 0 1 4 1 ;
      0 5 3 1 9 ;
      2 0 1 6 1 ;
      0 2 2 2 2 ;
      2 0 8 1 7 ;
      6 7 1 9 3 ];
S = [4 8 1 ;
     1 0 2 ;
     6 2 3 ;
     2 9 4 ;
     1 2 5 ;
     3 0 6 ];
loc_ref  = [ 17 12 13 16 13 ;
             12 17 15 13 10 ;
             14 12 13 18 13 ;
             12 14 14 14 14 ;
             14 12  7 13  0 ;
             18  0 13 10 15 ];
loc2_ref = [ 17  8  2  1  2 ;
              8 17  6  2 10 ;
              4  8  2  3  2 ;
              8  4  4  4  4 ;
              4  8  7  2  0 ;
              3  0  2  10 6 ];
nb_ref   = [ 1 2 3 2 3 ;
             2 1 2 3 1 ;
             4 2 3 2 3 ;
             2 4 4 4 4 ;
             4 2 1 3 0 ;
             2 0 3 1 2 ];

[nb, loc] = members(A, S, "last");
assert_checkequal(nb , nb_ref);
assert_checkequal(loc, loc_ref);
[nb, loc2] = members(A, S);
assert_checkequal(nb  , nb_ref);
assert_checkequal(loc2, loc2_ref);
assert_checktrue(loc >= loc2);
A_for_later = A;
S_for_later = S;
loc_ref_for_later = loc_ref;
loc2_ref_for_later = loc2_ref;
nb_ref_for_later = nb_ref;

A = A';
S = S';
loc_ref  = [ 15 17 14 17 14 18 ;
             17 15 17 14 17  0 ;
             13 16 13 14  2 13 ;
             12 13 18 14 13 11 ;
             13 11 13 14  0 16 ];
loc2_ref = [ 15  5  6  5  6  7 ;
              5 15  5  6  5  0 ;
              3  9  3  6  2  3 ;
              1  3  7  6  3 11 ;
              3 11  3  6  0  9 ];
nb_ref   = [1 2 4 2 4 2 ;
            2 1 2 4 2 0 ;
            3 2 3 4 1 3 ;
            2 3 2 4 3 1 ;
            3 1 3 4 0 2 ];

[nb, loc] = members(A, S, "last");
assert_checkequal(nb , nb_ref);
assert_checkequal(loc, loc_ref);
[nb, loc2] = members(A, S);
assert_checkequal(nb  , nb_ref);
assert_checkequal(loc2, loc2_ref);
assert_checktrue(loc >= loc2);


// Matrices of Integers
A = uint32(A);
S = uint32(S);

[nb, loc] = members(A, S, "last");
assert_checkequal(nb , nb_ref);
assert_checkequal(loc, loc_ref);
[nb, loc2] = members(A, S);
assert_checkequal(nb  , nb_ref);
assert_checkequal(loc2, loc2_ref);
assert_checktrue(loc >= loc2);


// Matrices of Strings
A = string(A_for_later);
S = string(S_for_later);
loc_ref  = loc_ref_for_later;
loc2_ref = loc2_ref_for_later;
nb_ref   = nb_ref_for_later;

[nb, loc] = members(A, S, "last");
assert_checkequal(nb , nb_ref);
assert_checkequal(loc, loc_ref);
[nb, loc2] = members(A, S);
assert_checkequal(nb  , nb_ref);
assert_checkequal(loc2, loc2_ref);
assert_checktrue(loc >= loc2);


// Matrices of Complexes
A = A_for_later*%i;
S = S_for_later*%i;

[nb, loc] = members(A, S, "last");
assert_checkequal(nb , nb_ref);
assert_checkequal(loc, loc_ref);
[nb, loc2] = members(A, S);
assert_checkequal(nb  , nb_ref);
assert_checkequal(loc2, loc2_ref);
assert_checktrue(loc >= loc2);


// Hypermatrices
A = A_for_later;
A(:, :, 2) = A;
S = S_for_later;
loc_ref(:, :, 2)  = loc_ref;
loc2_ref(:, :, 2) = loc2_ref;
nb_ref(:, :, 2)   = nb_ref;

[nb, loc] = members(A, S, "last");
assert_checkequal(nb , nb_ref);
assert_checkequal(loc, loc_ref);
[nb, loc2] = members(A, S);
assert_checkequal(nb  , nb_ref);
assert_checkequal(loc2, loc2_ref);
assert_checktrue(loc(:, :, 1) >= loc2(:, :, 1));
assert_checktrue(loc(:, :, 2) >= loc2(:, :, 2));


// Hypermatrices of Integers
A_int = uint32(A);
S_int = uint32(S);

[nb, loc] = members(A_int, S_int, "last");
assert_checkequal(nb , nb_ref);
assert_checkequal(loc, loc_ref);
[nb, loc2] = members(A_int, S_int);
assert_checkequal(nb  , nb_ref);
assert_checkequal(loc2, loc2_ref);
assert_checktrue(loc(:, :, 1) >= loc2(:, :, 1));
assert_checktrue(loc(:, :, 2) >= loc2(:, :, 2));


// Hypermatrices of Strings
A_str = string(A);
S_str = string(S);

[nb, loc] = members(A_str, S_str, "last");
assert_checkequal(nb , nb_ref);
assert_checkequal(loc, loc_ref);
[nb, loc2] = members(A_str, S_str);
assert_checkequal(nb  , nb_ref);
assert_checkequal(loc2, loc2_ref);
assert_checktrue(loc(:, :, 1) >= loc2(:, :, 1));
assert_checktrue(loc(:, :, 2) >= loc2(:, :, 2));


// Hypermatrices of Complexes
A_img = A.*%i;
S_img = S*%i;

[nb, loc] = members(A_img, S_img, "last");
assert_checkequal(nb , nb_ref);
assert_checkequal(loc, loc_ref);
[nb, loc2] = members(A_img, S_img);
assert_checkequal(nb  , nb_ref);
assert_checkequal(loc2, loc2_ref);
assert_checktrue(loc(:, :, 1) >= loc2(:, :, 1));
assert_checktrue(loc(:, :, 2) >= loc2(:, :, 2));


// Polynomials
z = %z;
A_pol = [z (1-z)^2 ; -4 %i*z ];
S_pol = [2  %i*z -z  3-z  z  z^3 z];
loc_ref  = [7 0 ; 0 2];
loc2_ref = [5 0 ; 0 2];
nb_ref   = [2 0 ; 0 1];

[nb, loc] = members(A_pol, S_pol, "last");
assert_checkequal(nb, nb_ref);
assert_checkequal(loc, loc_ref);
[nb, loc2] = members(A_pol, S_pol);
assert_checkequal(nb, nb_ref);
assert_checkequal(loc2, loc2_ref);
assert_checktrue(loc >= loc2);


// Row-wise and column-wise search
// With reals
A_cols = A_for_later;
S_cols = S_for_later;
S_cols(:, 2) = A_for_later(:, 2);
loc_ref = [0 2 0 0 0];
nb_ref  = [0 1 0 0 0];
[nb, loc] = members(A_cols, S_cols, "cols");
assert_checkequal(nb, nb_ref);
assert_checkequal(loc, loc_ref);
A_rows = A_cols';
S_rows = S_cols';
[nb, loc] = members(A_rows, S_rows, "rows");
assert_checkequal(nb, nb_ref);
assert_checkequal(loc, loc_ref);
// With integers
A_cols = int16(A_cols);
S_cols = int16(S_cols);
[nb, loc] = members(A_cols, S_cols, "cols");
assert_checkequal(nb, nb_ref);
assert_checkequal(loc, loc_ref);
A_rows = A_cols';
S_rows = S_cols';
[nb, loc] = members(A_rows, S_rows, "rows");
assert_checkequal(nb, nb_ref);
assert_checkequal(loc, loc_ref);
// With strings
A_cols = string(A_cols);
S_cols = string(S_cols);
[nb, loc] = members(A_cols, S_cols, "cols");
assert_checkequal(nb, nb_ref);
assert_checkequal(loc, loc_ref);
A_rows = A_cols';
S_rows = S_cols';
[nb, loc] = members(A_rows, S_rows, "rows");
assert_checkequal(nb, nb_ref);
assert_checkequal(loc, loc_ref);


// Shuffle option
// With reals
A_cols = A_for_later;
S_cols = S_for_later;
S_cols(:, 2) = A_for_later(:, 2);
temp = S_cols(1, 2); S_cols(1, 2) = S_cols(6, 2); S_cols(6, 2) = temp; // Switching two elements of S
S_cols = [S_cols S_cols(:, 2)]; // To get two column occurrences of A in S, for "later" to be relevant
loc_ref  = [0 2 0 0 0];
loc2_ref = [0 4 0 0 0];
nb_ref   = [0 2 0 0 0];
[nb, loc] = members(A_cols, S_cols, "cols", "shuffle");
assert_checkequal(nb, nb_ref);
assert_checkequal(loc, loc_ref);
[nb, loc2] = members(A_cols, S_cols, "cols", "shuffle", "last");
assert_checkequal(nb, nb_ref);
assert_checkequal(loc2, loc2_ref);
A_rows = A_cols';
S_rows = S_cols';
[nb, loc] = members(A_rows, S_rows, "rows", "shuffle");
assert_checkequal(nb, nb_ref);
assert_checkequal(loc, loc_ref);
[nb, loc2] = members(A_rows, S_rows, "rows", "shuffle", "last");
assert_checkequal(nb, nb_ref);
assert_checkequal(loc2, loc2_ref);
// With integers
A_cols = int16(A_cols);
S_cols = int16(S_cols);
[nb, loc] = members(A_cols, S_cols, "cols", "shuffle");
assert_checkequal(nb, nb_ref);
assert_checkequal(loc, loc_ref);
[nb, loc2] = members(A_cols, S_cols, "cols", "shuffle", "last");
assert_checkequal(nb, nb_ref);
assert_checkequal(loc2, loc2_ref);
A_rows = A_cols';
S_rows = S_cols';
[nb, loc] = members(A_rows, S_rows, "rows", "shuffle");
assert_checkequal(nb, nb_ref);
assert_checkequal(loc, loc_ref);
[nb, loc2] = members(A_rows, S_rows, "rows", "shuffle", "last");
assert_checkequal(nb, nb_ref);
assert_checkequal(loc2, loc2_ref);
// With strings
A_cols = string(A_cols);
S_cols = string(S_cols);
[nb, loc] = members(A_cols, S_cols, "cols", "shuffle");
assert_checkequal(nb, nb_ref);
assert_checkequal(loc, loc_ref);
[nb, loc2] = members(A_cols, S_cols, "cols", "shuffle", "last");
assert_checkequal(nb, nb_ref);
assert_checkequal(loc2, loc2_ref);
A_rows = A_cols';
S_rows = S_cols';
[nb, loc] = members(A_rows, S_rows, "rows", "shuffle");
assert_checkequal(nb, nb_ref);
assert_checkequal(loc, loc_ref);
[nb, loc2] = members(A_rows, S_rows, "rows", "shuffle", "last");
assert_checkequal(nb, nb_ref);
assert_checkequal(loc2, loc2_ref);



// IEEE
// Normal mode
A = [ 0 0 1 1 1 ;
      0 2 2 2 %nan ;
      2 0 1 1 1 ;
      0 0 1 %inf -%inf];
S = [1 0 1 ;
     2 0 4 ;
     1 2 5 ;
  %inf -%inf 6];
loc_ieee_ref  = [6 6 9 9 9 ;
                 6 7 7 7 0 ;
                 7 6 9 9 9 ;
                 6 6 9 4 8 ];
loc2_ieee_ref = [5 5 1 1 1 ;
                 5 2 2 2 0 ;
                 2 5 1 1 1 ;
                 5 5 1 4 8 ];
nb_ieee_ref   = [2 2 3 3 3 ;
                 2 2 2 2 0 ;
                 2 2 3 3 3 ;
                 2 2 3 1 1 ];

[nb, loc] = members(A, S, "last");
assert_checkequal(nb , nb_ieee_ref);
assert_checkequal(loc, loc_ieee_ref);
[nb, loc2] = members(A, S);
assert_checkequal(nb  , nb_ieee_ref);
assert_checkequal(loc2, loc2_ieee_ref);
assert_checktrue(loc >= loc2);


// Row-wise and column-wise modes
A_cols(:, 1) = "%nan";      // Introducing a %nan column
A_cols(1, 2) = "%inf";      // Adding some %inf elements
A_cols(2, 2) = "-%inf";
S_cols(:, 1) = "%nan";      // Introducing a %nan column
S_cols(1, [2 4]) = "%inf";  // Adding the same %inf elements to S
S_cols(2, [2 4]) = "-%inf";
S_cols(3, [2 4]) = "7";     // Adjusting, for shuffle option
A_rows(1, :) = "%nan";      // Same operations, by rows
A_rows(2, 1) = "%inf";
A_rows(2, 2) = "-%inf";
S_rows(1, :) = "%nan";
S_rows([2 4], 1) = "%inf";
S_rows([2 4], 2) = "-%inf";
S_rows([2 4], 3) = "7";
loc_ref(1) = 1; // %nan columns are common to A and S, in first position
nb_ref(1)  = 1;

[nb, loc] = members(A_cols, S_cols, "cols", "shuffle");
assert_checkequal(nb , nb_ref);
assert_checkequal(loc, loc_ref);
[nb, loc] = members(A_rows, S_rows, "rows", "shuffle");
assert_checkequal(nb , nb_ref);
assert_checkequal(loc, loc_ref);



// =============================================================================



// Error checks
A = [1 2 3; 4 5 6];
refMsg = msprintf(_("%s: Wrong number of input argument(s): at least %d expected.\n"), "members", 2);
assert_checkerror("[nb, loc] = members(A);", refMsg);
S = string(A);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: expected same type as first argument.\n"), "members", 2);
assert_checkerror("[nb, loc] = members(A, S, ""last"");", refMsg);
S = int16(A);
assert_checkerror("[nb, loc] = members(A, S, ""last"");", refMsg);
S = uint8(A);
assert_checkerror("[nb, loc] = members(A, S);", refMsg);
S = [7 8 9; 10 %nan 12];
refMsg = msprintf(_("%s: Wrong value for argument #%d: Must not contain NaN.\n"), "members", 2);
assert_checkerror("[nb, loc] = members(A, S);", refMsg);
A = rand(2, 3, 2);
S = [7 8; 10 11];
refMsg = msprintf(_("%s: Wrong type for argument #%d: Matrix expected.\n"), "members", 1);
assert_checkerror("[nb, loc] = members(A, S, ""rows"");", refMsg);
A = [1 2 3; 4 5 6; 7 8 9];
S = rand(2, 3, 2);
refMsg = msprintf(_("%s: Wrong type for argument #%d: Matrix expected.\n"), "members", 2);
assert_checkerror("[nb, loc] = members(A, S, ""rows"");", refMsg);
S = [7 8; 10 11];
refMsg = msprintf(_("%s: Incompatible input arguments #%d and #%d: Same number of columns expected.\n"), "members", 1, 2);
assert_checkerror("[nb, loc] = members(A, S, ""rows"");", refMsg);
refMsg = msprintf(_("%s: Incompatible input arguments #%d and #%d: Same number of rows expected.\n"), "members", 1, 2);
assert_checkerror("[nb, loc] = members(A, S, ""cols"");", refMsg);
