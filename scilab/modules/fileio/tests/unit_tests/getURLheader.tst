// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Kartik Gupta
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

// ============================================================================
// Unitary tests for getURLheader function
// ============================================================================

filename = getURLheader("www.google.co.in","google");
assert_checkequal(filename,"google");
assert_checktrue(isfile(filename));
assert_checktrue(isfile(filename+".header"));
deletefile(filename);
deletefile(filename+".header");


getURLheader("www.scilab.org/product/man/derivative.html");
assert_checktrue(isfile("derivative.html"));
assert_checktrue(isfile("derivative.html.header"));
deletefile("derivative.html");
deletefile("derivative.html.header");
