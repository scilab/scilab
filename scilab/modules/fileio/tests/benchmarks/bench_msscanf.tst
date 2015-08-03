// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark msscanf
//==============================================================================

// <-- BENCH NB RUN : 10 -->
NB_VARS = 100;
NB_LINES = 1000;

string_to_parse =[];
string_to_parse = strsubst(emptystr(NB_LINES,1)+'A', 'A', strcat(string(1:NB_VARS)," "));

format_to_parse = "" + strcat(strsubst(strcat(string(ones(1,NB_VARS)), " "), "1", "%s")) + "";

//line_to_exec = "[ N," + strcat("v"+string(1:NB_VARS), ",") + "] = msscanf(-1, string_to_parse , format_to_parse );";
line_to_exec = "res = msscanf(-1, string_to_parse , format_to_parse );";

// <-- BENCH START -->
execstr(line_to_exec)
// <-- BENCH END -->
