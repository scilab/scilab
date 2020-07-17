// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - ESI Group - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16401 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=16401
//
// <-- Short Description -->
// Scilab crash at exit after declaring a global klass variable
//

jimport java.lang.String

global String
clear String

// crashed at exit

