// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- Non-regression test for bug 7278 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7278
//
// <-- Short Description -->
// strsplit did not work with some particular inputs.
//===============================
if strsplit("0") <> "0" then pause,end
//===============================
if strsplit("0","") <> "0" then pause,end
if strsplit("1","") <> "1" then pause,end
if strsplit("2","") <> "2" then pause,end
if strsplit("3","") <> "3" then pause,end
//===============================
[r_1, r_2] = strsplit("",":",1);
if r_1 <> "" then pause,end
if r_2 <> [] then pause,end
//===============================
[r_1, r_2] = strsplit("",":",1);
if r_1 <> "" then pause,end
if r_2 <> [] then pause,end
//===============================
[r_1, r_2] = strsplit("",",");
if r_1 <> "" then pause,end
if r_2 <> [] then pause,end
//===============================
[r_1, r_2] = strsplit("","/:|,/");
if r_1 <> "" then pause,end
if r_2 <> [] then pause,end
//===============================
[r_1, r_2] = strsplit("","/:|,/");
if r_1 <> "" then pause,end
if r_2 <> [] then pause,end
//===============================
[r_1, r_2] = strsplit("",[":";","]);
if r_1 <> "" then pause,end
if r_2 <> [] then pause,end
//===============================
[r_1, r_2] = strsplit("",[":";","]);
if r_1 <> "" then pause,end
if r_2 <> [] then pause,end
//===============================
[r_1, r_2] = strsplit("",[":";","]);
if r_1 <> "" then pause,end
if r_2 <> [] then pause,end
//===============================
[r_1, r_2] = strsplit("",[":";","],2);
if r_1 <> "" then pause,end
if r_2 <> [] then pause,end
//===============================
[r_1, r_2] = strsplit("",[":";","],2);
if r_1 <> "" then pause,end
if r_2 <> [] then pause,end
//===============================
[r_1, r_2] = strsplit("",[":";","],2);
if r_1 <> "" then pause,end
if r_2 <> [] then pause,end
//===============================
[r_1, r_2] = strsplit("r",":",1);
if r_1 <> "r" then pause,end
if r_2 <> [] then pause,end
//===============================
[r_1, r_2] = strsplit(":",":",1);
if r_1 <> ":" then pause,end
if r_2 <> [] then pause,end
//===============================
[r_1, r_2] = strsplit(",",",");
if r_1 <> "," then pause,end
if r_2 <> [] then pause,end
//===============================
[r_1, r_2] = strsplit("r","/:|,/");
if r_1 <> "r" then pause,end
if r_2 <> [] then pause,end
//===============================
[r_1, r_2] = strsplit(":","/:|,/");
if r_1 <> ":" then pause,end
if r_2 <> [] then pause,end
//===============================
[r_1, r_2] = strsplit("a",[":";","]);
if r_1 <> "a" then pause,end
if r_2 <> [] then pause,end
//===============================
[r_1, r_2] = strsplit(":",[":";","]);
if r_1 <> ":" then pause,end
if r_2 <> [] then pause,end
//===============================
[r_1, r_2] = strsplit(",",[":";","]);
if r_1 <> "," then pause,end
if r_2 <> [] then pause,end
//===============================
[r_1, r_2] = strsplit("a",[":";","],2);
if r_1 <> "a" then pause,end
if r_2 <> [] then pause,end
//===============================
[r_1, r_2] = strsplit(":",[":";","],2);
if r_1 <> ":" then pause,end
if r_2 <> [] then pause,end
//===============================
[r_1, r_2] = strsplit(",",[":";","],2);
if r_1 <> "," then pause,end
if r_2 <> [] then pause,end
//===============================
