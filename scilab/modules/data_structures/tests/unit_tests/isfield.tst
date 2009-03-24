// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- JVM NOT MANDATORY -->

// unit tests for isfield function
// =============================================================================

my_struct = struct("field_1",123,"field_2",456);

// syntax

ierr = execstr("isfield()","errcatch");
if ierr == 0 then pause,end
if lasterror() <> "isfield: Wrong number of input argument(s): 2 expected." then pause, end

ierr = execstr("isfield(my_struct)","errcatch");
if ierr == 0 then pause,end
if lasterror() <> "isfield: Wrong number of input argument(s): 2 expected." then pause, end

ierr = execstr("isfield(my_struct,''field_1'',''field_2'')","errcatch");
if ierr == 0 then pause,end

ierr = execstr("isfield(my_struct,2)","errcatch");
if ierr == 0 then pause,end
if lasterror() <> "isfield: Wrong type for input argument #2: A string expected." then pause, end

my_struct = ["field_1","field_2"];

ierr = execstr("isfield(my_struct,''field_1'')","errcatch");
if ierr == 0 then pause,end
if lasterror() <> "isfield: Wrong type for input argument #1: struct array expected." then pause, end

// Fonctionnality

my_struct = struct("field_1",123,"field_2",456);

if ~ isfield(my_struct,"field_1") then pause, end
if ~ isfield(my_struct,"field_2") then pause, end
if   isfield(my_struct,"field_3") then pause, end
