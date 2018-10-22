// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI-Group - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15624 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15624
//
// <-- Short Description -->
// parser shoud not allow fun(,) or fun(x,) for internals

msgerr = [msprintf(gettext("Function not defined for given argument type(s),\n"));
          msprintf(gettext("  check arguments or define function %s for overloading.\n"), "%0_max")];
assert_checkerror ("max(,)", msgerr);
assert_checkerror ("max(1,)", msgerr);
assert_checkerror ("max(,1)", msgerr);
