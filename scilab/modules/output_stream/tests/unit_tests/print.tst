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
msg = msprintf(_("%s: Wrong file descriptor: %d.\n"), "print", 5);
assert_checkerror("print(%io(1), ""Hello World"")", msg);
