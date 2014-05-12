//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


// <-- Non-regression test for bug 3135 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3135
//
// <-- Short Description -->
//Problem with extraction and hypermatrices, especially when using patterns with a dollar

a=zeros(3,15,10);
for i=1:15
  if or(size(a(:,i:3:$,:))<>size(a(:,i:3:15,:))) then pause,end
end
for i=1:15
  if or(size(a(:,i:2:$,:))<>size(a(:,i:2:15,:))) then pause,end
end
for i=1:15
  if or(size(a(:,i:$,:))<>size(a(:,i:15,:))) then pause,end
end
