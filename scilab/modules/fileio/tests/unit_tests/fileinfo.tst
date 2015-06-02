// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

[x,err] = fileinfo(SCI);
if err <> 0 then pause,end
if size(x,"*") <> 13 then pause,end

[x,err] = fileinfo("My_file_not_exist");
if err == 0 then pause,end
if x <> [] then pause,end

ierr = execstr("fileinfo([SCI,SCIHOME])","errcatch");
if ierr <> 0 then pause,end

FILES = [SCI;"My_file_not_exist";SCIHOME;TMPDIR];
[X,ERRS] = fileinfo(FILES);

if    or(isnan(X(1,:))) then pause,end
if ~ and(isnan(X(2,:))) then pause,end
if    or(isnan(X(3,:))) then pause,end
if    or(isnan(X(4,:))) then pause,end

if size(X(1,:),"*") <> 13 then pause,end
if size(FILES,"*") <> size(ERRS,"*") then pause,end
