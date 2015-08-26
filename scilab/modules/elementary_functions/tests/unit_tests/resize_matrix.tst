// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

myMat =[     0.6029980    4.2470508    3.2442814    3.7427533
1.4276821    2.6285304    4.9615955    2.0520295
4.3037573    4.9656049    0.2502099    3.0422632  ];

myMat = resize_matrix( myMat, 3, 3 ) // reduce the matrix size
refMat=[0.602998,4.2470508,3.2442814;
1.4276821,2.6285304,4.9615955;
4.3037573,4.9656049,0.2502099];

assert_checkequal(size(myMat), size(refMat));

assert_checkequal(myMat, refMat);

refMat=int32([0,4,3,0;
1,2,4,0;
4,4,0,0;
0,0,0,0]);

myMatInteger = resize_matrix( myMat, 4, 4, "int32" ) // create an integer matrix
assert_checkequal(myMatInteger, refMat);
booleanMat=resize_matrix( refMat, 3, 5 , "boolean");
assert_checkequal(booleanMat, [%f %t %t %f %f; %t %t %t %f %f; %t %t %f %f %f]);

myMatBoolean = resize_matrix( myMat, 2, 2, "boolean" );
assert_checkequal(myMatBoolean, [%t %t; %t %t] );
assert_checkequal(size(myMatBoolean), [2, 2]);
myMatBoolean = resize_matrix( myMatBoolean, 3, 5 );
assert_checkequal(myMatBoolean, [%t %t %f %f %f; %t %t %f %f %f; %f %f %f %f %f]);
assert_checkequal(size(myMatBoolean), [3, 5]);

myMatBoolean(1:4:9) = %t;
myMatBoolean = resize_matrix(myMatBoolean, 3, 3);
assert_checkequal(myMatBoolean, [%t %t %f; %t %t %f; %f %f %t]);

// string matrix
myMatString = ["Scilab","the";"Open Source","Scientific";"Software","Package"];
myMatString = resize_matrix( myMatString, 3, 1 );

assert_checkequal(myMatString, ["Scilab";"Open Source";"Software"]);
