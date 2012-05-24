// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for calendar function
// =============================================================================

current_language = getlanguage();
setlanguage("en_US"); // This only chance this test pass is to launch it when scilab is in english

cal = calendar(2005,12);

title_ref = "Dec 2005";
week_ref  = "   M      Tu     W      Th     F     Sat     Sun";
cal_ref   = [0,0,0,1,2,3,4;5,6,7,8,9,10,11;12,13,14,15,16,17,18;19,20,21,22,23,24,25;26,27,28,29,30,31,0;0,0,0,0,0,0,0];

if cal(1)    <> title_ref then pause,end
if cal(2)    <> week_ref  then pause,end
if or(cal(3) <> cal_ref)  then pause,end

setlanguage(current_language);
