// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7952 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7952
//
// <-- Short Description -->
// libraryinfo did not check size of input argument.
//

if execstr("macros = libraryinfo()", "errcatch") <> 999 then pause, end
if lasterror() <> msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "libraryinfo", 1) then pause, end

if execstr("macros = libraryinfo([""functionslib"", ""corelib""])", "errcatch") <> 999 then pause, end
if lasterror() <> msprintf(_("%s: Wrong size for input argument #%d: A String expected.\n"), "libraryinfo", 1) then pause, end

macros = libraryinfo("functionslib");
if size(macros) == [] then pause,end

[macroslist, pathmacros] = libraryinfo("functionslib");
if size(macros) == [] then pause,end
if ~isdir(pathmacros) then pause,end

