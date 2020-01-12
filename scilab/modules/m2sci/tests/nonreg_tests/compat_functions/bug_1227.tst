//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1227 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1227
//
// <-- Short Description -->
//    -->a=1;savematfile('file','a')
//    WARNING:Option -v6 added
//     !--error   246
//    function not defined for given argument type(s)
//    Check arguments or define function %s_strindex
//    for overloading
//    at line     287 of function savematfile called by :
//    savematfile('totoo','a')

// <-- ENGLISH IMPOSED -->

a = 123456789;
savematfile(TMPDIR+'/file_1232.mat','a');
clear a
loadmatfile(TMPDIR+'/file_1232.mat','a');

if a <> 123456789 then pause,end
