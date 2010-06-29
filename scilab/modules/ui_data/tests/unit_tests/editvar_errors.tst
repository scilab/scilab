// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//editvar

// <-- ENGLISH IMPOSED -->

ierr = execstr("editvar();", "errcatch")
lasterror()

ierr = execstr("editvar(1,2,3);", "errcatch")
lasterror()

ierr = execstr("editvar([]);", "errcatch")
lasterror()

ierr = execstr("editvar(12);", "errcatch")
lasterror()

clear __ghost
ierr = execstr("editvar(""__ghost"");", "errcatch")
lasterror()

