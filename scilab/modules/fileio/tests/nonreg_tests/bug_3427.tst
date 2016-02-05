//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3427 -->
//
// <-- Short Description -->
//-->mgetl(TMPDIR+"/"+myFile,'r')
//mgetl: Wrong type for argument 2: Real or complex matrix expected.
//=> Wrong error report.
//The second input argument should be an integer scalar.
//Nothing else.
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3427
//

ierr = execstr("mgetl(''SCI/etc/scilab.start'',''r'');","errcatch");
if ierr <> 999 then pause,end
err_msg = lasterror();
ref_msg = msprintf(gettext("%s: Wrong type for input argument #%d: An integer value expected.\n"),"mgetl",2);

if err_msg <> ref_msg then pause,end

ierr = execstr("mgetl(''SCI/etc/scilab.start'',[5 5]);","errcatch");
if ierr <> 999 then pause,end
err_msg = lasterror();
ref_msg = msprintf(gettext("%s: Wrong size for input argument #%d: An integer value expected.\n"),"mgetl",2);

if err_msg <> ref_msg then pause,end
