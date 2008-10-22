// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// test bug 2059

fd = mopen(SCI+'/modules/io/tests/nonreg_tests/bug_2059.txt','rt');

ierr = execstr("dataReadFromFile = mgetl(fd,100)","errcatch");
if ierr <> 0 then pause,end

close(fd);

if size(dataReadFromFile,'*') <> 4 then pause,end
