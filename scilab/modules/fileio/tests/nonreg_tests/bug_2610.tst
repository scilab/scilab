//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// test bug 2610

write(TMPDIR + '/foo.txt',rand(500,3)) 
fd=mopen(TMPDIR + '/foo.txt');
A=mgetl(fd,300);
if size(A)<>[300,1] then pause, end

A=mgetl(fd,300);
if size(A)<>[200,1] then pause, end

A=mgetl(fd,300);
if size(A)<>[0,0] then pause, end
mclose(fd);
mdelete(TMPDIR + '/foo.txt')
