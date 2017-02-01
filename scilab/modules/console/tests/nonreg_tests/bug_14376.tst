// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- INTERACTIVE TEST -->
// <-- Non-regression test for bug 14376 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14376
//
// <-- Short Description -->
// input() is broken: \n introduced before prompting; multiple prompts; missin assignment; "%" "\n" "\t" no longer supported in messages...

t = input("Message: ","s");
// write a string then enter
// Must be prompted only one time
