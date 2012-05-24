// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- CLI SHELL MODE -->


x = factorial ( 1 );
assert_checkequal ( x , 1 );
x = factorial ( 2 );
assert_checkequal ( x , 2 );
x = factorial ( [1 2 3 4] );
assert_checkequal ( x , [1 2 6 24] );
x = factorial ( 170 );
assert_checkalmostequal ( x , 7.25741561530799896739e306 , 10 * %eps );
// Test with a matrix
n = [
1 2 3
4 5 6
7 8 9
];
x = factorial ( n );
e = [
    1.       2.        6.       
    24.      120.      720.     
    5040.    40320.    362880.  
];
assert_checkequal ( x , e );
// Test with an hypermatrix
clear n;
clear e;
n(1,1,1,1:2)=[1 2];
x = factorial ( n );
e(1,1,1,1:2)=[1 2];
assert_checkequal ( x , e );
clear n;
clear e;

