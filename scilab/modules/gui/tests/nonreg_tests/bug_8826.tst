// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 8826 -->
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8826
//
// <-- Short Description -->
// userdata property for a menu crashed Scilab
//

if execstr("f = uimenu(""Parent"", gcf(), ""Label"", ""test"", ""userdata"", ""crash"")", "errcatch") <> 0 then pause, end
if execstr("f = uimenu(""Parent"", gcf(), ""Label"", ""test"", ""userdata"", 10)", "errcatch") <> 0 then pause, end

data_matrix = ones(2,2);
f = uimenu("Parent", gcf(), "Label", "test", "userdata", data_matrix);

data_strings = ["tests", "scilab"; "uimenu", "user_data"];
f = uimenu("Parent", gcf(), "Label", "test", "userdata", data_strings);

delete(gcf());
