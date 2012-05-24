// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2976 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2976
//
v = "1.0";
ierr = execstr('r = regexp(v, ''/[1-9]+\.[1-9]+'') ','errcatch');
if ierr <> 999 then pause,end
if exists('r') <> 0 then pause,end

ierr = execstr('r = regexp(v, ''/[1-9]+\.[1-9]/'') ','errcatch');
if ierr <> 0 then pause,end
if exists('r') <> 1 then pause,end
if r <> [] then pause,end
