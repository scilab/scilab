// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

jimport("java.util.ArrayList")

l = ArrayList.new(jvoid);

// insert
l($+1) = 1;
l($+1) = 2;
l(2*$-1) = 3;
l($+1) = 4;

assert_checkequal(l.size(), int32(4));

// extract
assert_checkequal(l(2*$-6), 2);

