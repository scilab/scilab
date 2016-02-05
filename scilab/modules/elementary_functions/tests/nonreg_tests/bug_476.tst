// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Serge Steer - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 476 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/476
//
// <-- Short Description -->
// find did not work when there are most lhs arguments than matrix dimensions

[i,j,k]=find([%t %f %f %t %f]);
if or(i<>1)|or(k<>1)|or(j<>[1 4]) then pause,end

[i,j,k,l]=find([%t %f %f %t %f]);
if or(i<>1)|or(k<>1)|or(l<>1)|or(j<>[1 4]) then pause,end

[i,j,k]=find(matrix([%t %f %f %t %f],1,1,-1));
if or(i<>1)|or(j<>1)|or(k<>[1 4]) then pause,end

[i,j]=find(matrix([%t %f %f %t %f],1,1,-1));
if or(i<>1)|or(j<>[1 4]) then pause,end

[i,j,k,l]=find(matrix([%t %f %f %t %f],1,1,-1));
if or(i<>1)|or(j<>1)|or(k<>[1 4])|or(l<>1) then pause,end
