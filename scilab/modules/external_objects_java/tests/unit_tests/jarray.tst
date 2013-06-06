// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

a = jarray("java.lang.String", 2, 2, 3);
// array insertion
a(0, 0, 2) = "Hi Jims !";
a(1, 0, 2) = "Hi Jims again !!"
