// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// stdout
write(%io(2), "Hello World")

//stdin
ierr = execstr("write(%io(1), ""Hello World"");","errcatch");
assert_checkequal(ierr, 999);