// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// Scalar
assert_checkequal(gsort(%t), %t);
assert_checkequal(gsort(%f), %f);

// Vector
b = [%f %t %f %t ];
assert_checkequal(gsort(b),  [%t %t %f %f]);
assert_checkequal(gsort(b'), [%t %t %f %f]');
assert_checkequal(gsort(b,"g","i"),  [%f %f %t %t]);
assert_checkequal(gsort(b',"g","i"), [%f %f %t %t]');

assert_checkequal(gsort(b,"r"), b);
assert_checkequal(gsort(b,"r","i"), b);
assert_checkequal(gsort(b',"c"),    b');
assert_checkequal(gsort(b',"c","i"), b');

assert_checkequal(gsort(b,"lr"), b);
assert_checkequal(gsort(b,"lr","i"), b);
assert_checkequal(gsort(b',"lc"),    b');
assert_checkequal(gsort(b',"lc","i"), b');

// Matrix
b = [%f,%t,%t,%t,%t;%f,%f,%f,%f,%t;%t,%f,%t,%f,%f]; // (3x5)
assert_checkequal(gsort(b), [%t,%t,%t,%f,%f;%t,%t,%f,%f,%f;%t,%t,%f,%f,%f]);
assert_checkequal(gsort(b,"g","i"), [%f,%f,%f,%t,%t;%f,%f,%f,%t,%t;%f,%f,%t,%t,%t]);

ref = [%t,%t,%t,%t,%t;%f,%f,%t,%f,%t;%f,%f,%f,%f,%f];
assert_checkequal(gsort(b,"r"), ref);
assert_checkequal(gsort(b,"r","i"), ref($:-1:1,:));
ref = [%t,%t,%t,%t,%f;%t,%f,%f,%f,%f;%t,%t,%f,%f,%f];
assert_checkequal(gsort(b,"c"), ref);
assert_checkequal(gsort(b,"c","i"), ref(:,$:-1:1));

ref = [%t,%f,%t,%f,%f;%f,%t,%t,%t,%t;%f,%f,%f,%f,%t];
assert_checkequal(gsort(b,"lr"), ref);
assert_checkequal(gsort(b,"lr","i"), ref($:-1:1,:));
ref = [%t,%t,%t,%t,%f;%t,%f,%f,%f,%f;%f,%t,%f,%f,%t];
assert_checkequal(gsort(b,"lc"), ref);
assert_checkequal(gsort(b,"lc","i"), ref(:,$:-1:1));

// Hypermatrix
b = cat(3,[%f %t %f %t ; %t %t %t %f ; %f %t %f %t], ..
          [%t %t %f %f ; %t %t %f %f ; %f %t %f %t]);
s = [3 4 2];
// "g"
ref = matrix([%t,%t,%t,%t,%t,%t,%t,%t,%t,%t,%t,%t,%t,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f],s);
assert_checkequal(gsort(b), ref);
ref = matrix([%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%t,%t,%t,%t,%t,%t,%t,%t,%t,%t,%t,%t,%t],s);
assert_checkequal(gsort(b,"g","i"), ref);
// "r"
ref = matrix([%t,%f,%f,%t,%t,%t,%t,%f,%f,%t,%t,%f,%t,%t,%f,%t,%t,%t,%f,%f,%f,%t,%f,%f],s);
assert_checkequal(gsort(b,"r"), ref);
ref = matrix([%f,%f,%t,%t,%t,%t,%f,%f,%t,%f,%t,%t,%f,%t,%t,%t,%t,%t,%f,%f,%f,%f,%f,%t],s);
assert_checkequal(gsort(b,"r","i"), ref);
// "c"
ref = matrix([%t,%t,%t,%t,%t,%t,%f,%t,%f,%f,%f,%f,%t,%t,%t,%t,%t,%t,%f,%f,%f,%f,%f,%f],s);
assert_checkequal(gsort(b,"c"), ref);
ref = matrix([%f,%f,%f,%f,%t,%f,%t,%t,%t,%t,%t,%t,%f,%f,%f,%f,%f,%f,%t,%t,%t,%t,%t,%t],s);
assert_checkequal(gsort(b,"c","i"), ref);
// "lr"
ref = matrix([%t,%f,%f,%t,%t,%t,%t,%f,%f,%f,%t,%t,%t,%t,%f,%t,%t,%t,%f,%f,%f,%f,%f,%t],s);
assert_checkequal(gsort(b,"lr"), ref);
ref = matrix([%f,%f,%t,%t,%t,%t,%f,%f,%t,%t,%t,%f,%f,%t,%t,%t,%t,%t,%f,%f,%f,%t,%f,%f],s);
assert_checkequal(gsort(b,"lr","i"), ref);
// "lc"
ref = matrix([%t,%t,%t,%t,%f,%t,%f,%t,%f,%f,%t,%f,%t,%t,%t,%t,%t,%f,%f,%f,%t,%f,%f,%f],s);
assert_checkequal(gsort(b,"lc"), ref);
ref = matrix([%f,%t,%f,%f,%t,%f,%t,%f,%t,%t,%t,%t,%f,%f,%f,%f,%f,%t,%t,%t,%f,%t,%t,%t],s);
assert_checkequal(gsort(b,"lc","i"), ref);

// ========
// K output
// ========
// Matrix
// ------
// With some equal rows and cols, to check that initial order is kept
b = [%f %t %f %t ; %t %t %t %f ; %f %t %f %t];
// "r"
[?,k] = gsort(b, "r");
assert_checkequal(k, [2,1,2,1;1,2,1,3;3,3,3,2]);
[?,k] = gsort(b, "r", "i");
assert_checkequal(k, [1,1,1,2;3,2,3,1;2,3,2,3]);
// "c"
[?,k] = gsort(b, "c");
assert_checkequal(k, [2,4,1,3;1,2,3,4;2,4,1,3]);
[?,k] = gsort(b, "c", "i");
assert_checkequal(k, [1,3,2,4;4,1,2,3;1,3,2,4]);
// "lr"
[?,k] = gsort(b, "lr");
assert_checkequal(k, [2 1 3]');
[?,k] = gsort(b, "lr", "i");
assert_checkequal(k, [1 3 2]');
// "lc"
[?,k] = gsort(b, "lc");
assert_checkequal(k, [2 4 1 3]);
[?,k] = gsort(b, "lc", "i");
assert_checkequal(k, [1 3 4 2]);

// Hypermatrix
// -----------
b = cat(3,[%f %t %f %t ; %t %t %t %f ; %f %t %f %t], ..
          [%t %t %f %f ; %t %t %f %f ; %f %t %f %t]);
s = [3 4 2];
// "r"
[?,k] = gsort(b, "r");
assert_checkequal(k, matrix([2,1,3,1,2,3,2,1,3,1,3,2,1,2,3,1,2,3,1,2,3,3,1,2],s));
[?,k] = gsort(b, "r", "i");
assert_checkequal(k, matrix([1,3,2,1,2,3,1,3,2,2,1,3,3,1,2,1,2,3,1,2,3,1,2,3],s));
// "c"
[?,k] = gsort(b, "c");
assert_checkequal(k, matrix([2,1,2,4,2,4,1,3,1,3,4,3,1,1,2,2,2,4,3,3,1,4,4,3],s));
[?,k] = gsort(b, "c", "i");
assert_checkequal(k, matrix([1,4,1,3,1,3,2,2,2,4,3,4,3,3,1,4,4,3,1,1,2,2,2,4],s));
// "lr"
[?,k] = gsort(b, "lr");
assert_checkequal(k, cat(3,[2 1 3]', [1 2 3]'));
[?,k] = gsort(b, "lr", "i");
assert_checkequal(k, cat(3,[1 3 2]', [3 1 2]') );
// "lc"
[?,k] = gsort(b, "lc");
assert_checkequal(k, cat(3,[2 4 1 3], [2 1 4 3]));
[?,k] = gsort(b, "lc", "i");
assert_checkequal(k, cat(3,[1 3 4 2], [3 4 1 2]));
