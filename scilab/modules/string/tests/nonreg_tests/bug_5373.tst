// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- Non-regression test for bug 5373 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5373
//
// <-- Short Description -->
// isalphanum() doesn't support non ascii chars

//======================================================================================== 
ref = [%T];
res = isalphanum("é");
if ref <> res then pause,end
//======================================================================================== 
ref = [%T %T %T %T %T %T %F %T %F %T %T %T %T %T %T];
res = isalphanum("scilab é scilab");
if ~and(ref == res) then pause,end
//======================================================================================== 


