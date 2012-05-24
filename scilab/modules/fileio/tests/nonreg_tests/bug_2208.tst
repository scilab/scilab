// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// test bug 2208

fd = mopen(TMPDIR+'/bug_2208.txt','wt');
if mtell(fd) <> 0 then pause,end

m = rand(10,1);
t = mputl(string(m),fd);
if t <> %t then pause,end

r = mtell(fd);
mclose(fd);

INFO = fileinfo(TMPDIR+'/bug_2208.txt');
if INFO(1) <> r then pause,end



