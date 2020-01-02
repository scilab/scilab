// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14978
//
// <-- Short Description -->
// input(message) never interpreted 'x' as an existing variable.
// input(message) displayed the returning prompt on the same line as the input

x = 20;
z = input("Message. Please enter ''x'':")
// Then enter 'x' (with not quote) and press <enter>
// It should display the following:

--> z = input("Message. Please enter ''x'':")
Message. Please enter 'x':
x
 z  = 

   20.

// Sometimes we are prompted several times: http://bugzilla.scilab.org/14376
