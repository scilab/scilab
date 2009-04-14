// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 3394 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3394
//
// <-- Short Description -->
//    A malformed script originating from the editor window hangs Scilab and the editor.

// 1 - Start Scilab

// 2 - Start Scipad

// 3 - In Scipad enter:
// function [in,out]=get_csv();
//   data_file=xgetfile("*.csv",title="Select Data File");
//   Value_labels=["Input columns";"Output columns"];
//   [_Input,_Output]=getvalue("Input and outputs column numbers",Value_labels,...
//   list("vec",-1,"vec",-1)
//   raw_data=fscanfMat(data_file)
//   in=raw_data(:,_Input)
//   out=raw_data(:,_Output)
// endfunction

// 4 - CTRL+L

// 5 - Check that Scilab and Scipad are alive
