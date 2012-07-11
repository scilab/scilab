// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2009 - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4467 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4467
//
// <-- Short Description -->
// getshortpathname , getlongpathname do not manage matrix of strings

if getos() == 'Windows' then

[r1,b1] = getshortpathname([TMPDIR,SCI;SCI,TMPDIR]);
if size(r1,'*') <> 4 then pause,end
if ~and(b1 == %t) then pause,end

[r2,b2] = getlongpathname(r1);
if size(r2,'*') <> 4 then pause,end
if ~and(b2 == %t) then pause,end

if ~and(r1<>r2) then pause,end
if ~and(b1 == b2) then pause,end

[r3,b3] = getshortpathname(["/My_tmp/file_1";"/My_tmp/file_2"]);
if size(r3,'*') <> 2 then pause,end
if ~and(b3 == %f) then pause,end

[r4,b4] =getlongpathname(["/My_tmp/file_1";"/My_tmp/file_2"]);
if size(r4,'*') <> 2 then pause,end
if ~and(b4 == %f) then pause,end

if ~and(r3 == r4) then pause,end
if ~and(b3 == b4) then pause,end

[r5,b5] =getshortpathname([SCI;"/My_tmp/file_2"]);
if size(r5,'*') <> 2 then pause,end
if ~and(b5 == [%t;%f]) then pause,end

[r6,b6] =getlongpathname([SCI;"/My_tmp/file_2"]);
if size(r6,'*') <> 2 then pause,end
if ~and(b6 == [%t;%f]) then pause,end

end
