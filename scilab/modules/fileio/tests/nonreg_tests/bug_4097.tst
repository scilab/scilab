// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4097 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4097
//
// <-- Short Description -->
// fscanfMat() crashes Scilab. It doesn't occur with scilab-5.0.3

n  = 50;
a  = rand(n,n,"u");
fd = mopen(TMPDIR+"/bug_4097.mat","w");

for i=1:n ,
  for j=1:n, mfprintf(fd,"%5.2f ",a(i,j));end;
  mfprintf(fd,'\n');
end

mclose(fd);

b = fscanfMat(TMPDIR+"/bug_4097.mat");

if size(b)<> [50,50] then pause,end
