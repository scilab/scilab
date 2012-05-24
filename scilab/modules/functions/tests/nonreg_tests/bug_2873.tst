// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO TRY CATCH -->

// <-- Non-regression test for bug 2873 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2873
//
// <-- Short Description -->
//    Use of getd() on the command line for a directory which contains a function named the same 
//    as a permanent variable causes Scilab to stop responding to any input (on the command line or UI in general).
//    The Scilab executable must be forced to close from tas

my_function = ["function out = home()";"out = 1";"endfunction"];
my_dir      = TMPDIR+"/bug2873_dir";
my_file     = my_dir+"/home.sci";

mkdir(my_dir);
mputl(my_function,my_file);

execstr("getd(''"+my_dir+"'')","errcatch");
if( execstr("a = 1;","errcatch") <> 0 ) then pause,end
