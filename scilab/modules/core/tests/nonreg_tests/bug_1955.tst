//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1955 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1955
//
// <-- Short Description -->
//    Reshaping global hypermatrices is problematic, as demonstrated by the 
//    attached file 'bug.sci', which contains further comments.

global ghm

ghm = matrix(1:8,2,4);
ghm = matrix(ghm,2,2,2);

if or(ghm<>matrix(1:8,2,2,2)) then pause,end
