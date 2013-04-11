// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- INTERACTIVE TEST -->
//open a diary
setlanguage("en_US");
clc;
diary(SCIHOME + "/exec_mode.new");

disp(1);

//one line instruction
a = 1;

//many instructions on one line
a = 1;b = 2;c = 3;

//one instruction on many lines
a = [1, ..
2, ..
3];

//exec many instructions on many lines
a = 1;b = [1, ..
2, ..
3];

a = [1, ..
2, ..
3];b = 1;

a = 1;b = [1, ..
2, ..
3];c = 1;

//instruction block

//one line
if a then a = 1;else a = 1;end

//many lines
if a then
    a = 1;
else
    a = 1;
end

//functions

//one line
function foo1(), a = 1;endfunction

//many lines
function foo2()
    a = 1;
endfunction

//comments

a = 1;//comments

a = [1, .. //comments
2];
a = [1, ..
2, .. //comments
3];

//printf and instruction
printf("printf")
a = 1;

//printf, blank line and instruction
printf("printf")

a = 1;

//warning
warning("warning");
diary(SCIHOME + "/exec_mode.new", "close");
