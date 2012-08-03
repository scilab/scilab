// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 2333 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2333
//
// <-- Short Description -->
//    I am trying to convert a large number of Matlab files to Scilab. Either 
//    by using translatepath or a loop with mfile2sci I get the same error 
//    message when converting specific .m files. It appears in recursive mode 
//    only when calling another matlab function.

load("SCI/modules/m2sci/macros/kernel/lib");
if ~(exists("mfile_path")) then pause,end
