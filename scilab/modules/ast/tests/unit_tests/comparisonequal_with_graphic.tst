// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Sylvain GENIN
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- TEST WITH GRAPHIC -->

//handle
handle1 = gcf();
handle2 = gca();

assert_checkequal(handle1 == handle1, %t);
assert_checkequal(handle1 == handle2, %f);

delete(handle1);

