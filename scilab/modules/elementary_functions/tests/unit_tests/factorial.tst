// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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

