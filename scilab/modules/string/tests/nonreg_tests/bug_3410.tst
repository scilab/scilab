// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3410 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3410
//
// <-- Short Description -->

if length(sparse(eye(2,2))) <> 2 then pause,end
if length(sparse(eye(12,2))) <> 12 then pause,end
if length(sparse(eye(2,12))) <> 12 then pause,end
if length(sparse(1)) <> 1 then pause,end

if with_module('umfpack') then
  A = ReadHBSparse(SCI+"/modules/umfpack/examples/bcsstk24.rsa");
  if length(A) <> 3562 then pause,end
end  
