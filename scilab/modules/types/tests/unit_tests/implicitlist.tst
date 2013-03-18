// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

// normal cases
assert_checkequal(1:3, [1 2 3]);
assert_checkequal(1:1:3, [1 2 3]);
assert_checkequal(1:2:3, [1 3]);

assert_checkequal(3:-1:1, [3 2 1]);
assert_checkequal(3:-2:1, [3 1]);

assert_checkequal(3:1, []);
assert_checkequal(3:2:1, []);
assert_checkequal(1:0:3, []);

// no finite cases
assert_checkequal(1:%inf, %nan);
assert_checkequal(1:%nan, %nan);
assert_checkequal(1:-%inf, []);
assert_checkequal(1:%inf:10, %nan);
assert_checkequal(1:%nan:10, %nan);
assert_checkequal(1:-%inf:10, []);

assert_checkequal(%inf:1, []);
assert_checkequal(%nan:1, %nan);
assert_checkequal(-%inf:1, %nan);
assert_checkequal(10:%inf:1, []);
assert_checkequal(10:%nan:1, %nan);
assert_checkequal(10:-%inf:1, %nan);
