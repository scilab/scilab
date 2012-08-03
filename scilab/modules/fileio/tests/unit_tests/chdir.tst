// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

a = chdir(TMPDIR);
if ~a then pause, end

b = chdir(SCI);
if ~b then pause, end

wm = warning('query');
warning('off');
c = chdir(pathconvert(SCI+"/nowhere"));
warning(wm);

if c then pause, end

ierr = execstr('chdir(1)','errcatch');
if ierr <> 999 then pause,end

