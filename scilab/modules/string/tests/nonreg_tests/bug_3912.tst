// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3912 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3912
//
// Short description:
// strsubst used with regular expression may return very stange result.
// spaces at beginning of a string were removed.
//==============================================================
t = "      IF ( INFORM .NE. 0 ) STOP";
ref = "      IF ( INFORM .NE. 0 ) then";
r = strsubst(t,'/stop/i','then','r');
if r <> ref then pause,end

t = "IF ( INFORM .NE. 0 ) STOP";
ref = "IF ( INFORM .NE. 0 ) then";
r = strsubst(t,'/stop/i','then','r');
if r <> ref then pause,end

t = "IF ( INFORM .NE. 0 ) STOP";
ref = "IF ( test .NE. 0 ) STOP";
r = strsubst(t,'/inform/i','test','r');
if r <> ref then pause,end
