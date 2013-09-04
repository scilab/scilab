// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// stdout
print(%io(2), "Hello World")

//stdin
try
    print(%io(1), "Hello World")
catch
    ierr = lasterror();
    assert_checkequal(ierr, 49);
end
