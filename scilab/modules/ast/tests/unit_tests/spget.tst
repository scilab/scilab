// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

// <-- CLI SHELL MODE -->

//get from old module sparse (Scilab 5.4)

//-------------------------------------------------------------
//test de la primitives spget
//--------------------------------------------------------------
sp=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
[ij,v]=spget(sp);
assert_checkequal(ij, [1,3;1,5;1,6;2,1;2,4;4,3;4,4;5,1;6,6]);
assert_checkequal(v, [3;2;1;5;4;7;6;8;9]);
zer=sparse([],[],[6,6]);[ij,v]=spget(zer);
assert_checkequal(ij, []);
assert_checkequal(v, []);

sp=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],[%t %t %t %t %t %t %t %t %t],[6 6]);
[ij,v,s]=spget(sp);
assert_checkequal(ij, [1,3;1,5;1,6;2,1;2,4;4,3;4,4;5,1;6,6]);
assert_checktrue(v);
assert_checkequal(s, [6,6]);
