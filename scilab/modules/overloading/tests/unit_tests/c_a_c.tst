// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
a = "a";
b = ["a";"b"];
c = a + b;
ref = ["aa";"ab"];
if or(c <> ref) then pause,end

a = "a";
b = ["a","b"];
c = a + b;
ref = ["aa","ab"];
if or(c <> ref) then pause,end

a = ["a";"b"];
b = "a";
c = a + b;
ref = ["aa";"ba"];
if or(c <> ref) then pause,end

a = ["a","b"];
b = "a";
c = a + b;
ref = ["aa","ba"];
if or(c <> ref) then pause,end

a = "a";
b = "b";
c = a + b;
ref = "ab";
if or(c <> ref) then pause,end

a = ["a","b"];
b = ["c","d"];
c = a + b;
ref = ["ac","bd"];
if or(c <> ref) then pause,end

a = ["a";"b"];
b = ["c";"d"];
c = a + b;
ref = ["ac";"bd"];
if or(c <> ref) then pause,end

a = ["a";"b";"e"];
b = ["c";"d"];
ierr = execstr("c = a + b;","errcatch");
if ierr == 0 then pause,end

a = ["a";"b"];
b = ["c";"d";"e"];
ierr = execstr("c = a + b;","errcatch");
if ierr ==0 then pause,end

