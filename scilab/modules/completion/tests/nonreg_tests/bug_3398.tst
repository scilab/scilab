// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3398 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3398
//
// <-- Short Description -->
// A bug in the completion:
// it removes the line. It should add it at the end instead instead.

// <-- INTERACTIVE TEST -->

exec le<TAB>

// it should be :
-->exec le
Fonction Scilab:
legendre length

Macro Scilab:
leastsq legend legends leqe leqr lev levin lex_sort

Champ d'un handle graphique:
legend_location
                    