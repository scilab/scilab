// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//
// <-- CLI SHELL MODE -->

a=rand(3,4);
spz = spzeros(a);
assert_checkequal(size(spz), size(a));

spz = spzeros(69, 73);
assert_checkequal(size(spz), [69 73]);

spz = spzeros(["abc"; "def"]);
assert_checkequal(size(spz), [2 1]);

err = execstr("spzeros(list(1,2))", "errcatch");
assert_checktrue(err <> 0);

err = execstr("spzeros("""", """")", "errcatch");
assert_checktrue(err <> 0);
