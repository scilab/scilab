// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- Non-regression test for bug 4768 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4768
//
// <-- Short Description -->
//strsplit does not work with localized characters

//======================================================================================== 
v = "世界您好";
ref_res1 = ["世";"界您好"];
ref_res3 = ["世界您";"好"];
r = strsplit(v,1)
if r <> ref_res1 then pause,end
r = strsplit(v,3)
if r <> ref_res3 then pause,end
//======================================================================================== 
v = "азеазея";
ref_res1 = ["а";"зеазея"]
ref_res3 = ["азе";"азея"];
r = strsplit(v,1)
if r <> ref_res1 then pause,end
r = strsplit(v,3)
if r <> ref_res3 then pause,end
//======================================================================================== 
