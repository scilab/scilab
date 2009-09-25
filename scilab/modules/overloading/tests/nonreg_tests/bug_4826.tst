// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4826 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4826
//
// <-- Short Description -->
// The type of the 'dims' entry of an hypermat is not always the same.

a=[1 2;3 4];
A=a(:,:,[1 1]);
if typeof(A.dims)<>"int32" then pause,end
if size(A.dims,1)<>1 then pause,end

a=int8([1 2;3 4]);
A=a(:,:,[1 1]);
if typeof(A.dims)<>"int32" then pause,end
if size(A.dims,1)<>1 then pause,end


a(1,1)=a(1,1)+%s;
A=a(:,:,[1 1]);
if typeof(A.dims)<>"int32" then pause,end
if size(A.dims,1)<>1 then pause,end

a=['1' '2';'3' '4'];
A=a(:,:,[1 1]);
if typeof(A.dims)<>"int32" then pause,end
if size(A.dims,1)<>1 then pause,end
