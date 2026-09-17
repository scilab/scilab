// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//
// <-- NO CHECK REF -->
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 3851 -->
//

//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/3851
//
// <-- Short Description -->
// root_properties: get(0, <property>) works only for
// "screensize_px" and "screendepth" properties. All other
// properties presented in the help file yield to an error.

ierr = execstr('r = get(0, ""UID"")', 'errcatch');
if ierr <> 0 then pause,end

ierr = execstr('r = get(0, ""Children"")', 'errcatch');
if ierr <> 0 then pause,end

ierr = execstr('r = get(0, ""pointer_location"")', 'errcatch');
if ierr <> 0 then pause,end

ierr = execstr('r = get(0, ""screensize"")', 'errcatch');
if ierr <> 0 then pause,end

ierr = execstr('r = get(0, ""screendepth"")', 'errcatch');
if ierr <> 0 then pause,end

ierr = execstr('r = get(0, ""ShowHiddenHandles"")', 'errcatch');
if ierr <> 0 then pause,end

ierr = execstr('r = get(0, ""ShowHiddenProperties"")', 'errcatch');
if ierr <> 0 then pause,end

ierr = execstr('r = get(0, ""UseDeprecatedSkin"")', 'errcatch');
if ierr <> 0 then pause,end

ierr = execstr('r = get(0, ""Units"")', 'errcatch');
if ierr <> 0 then pause,end

ierr = execstr('r = get(0, ""user_data"")', 'errcatch');
if ierr <> 0 then pause,end

ierr = execstr('r = get(0, ""tag"")', 'errcatch');
if ierr <> 0 then pause,end
