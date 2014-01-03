// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// unit tests for x_dialog function
// =============================================================================


// <-- INTERACTIVE TEST -->

// test 1
x_mdialog( ["Simple multiple dialog";"No scroll bar at all"], ..
["A";"B"], ..
["0";"1"] );

// test 2
x_mdialog(["dialog with huge vertical label part";
"Only vertical scroll bar"
"for the Label part";
string(1:50)'], ..
["A";"B"], ..
["0";"1"] );

// test 3
x_mdialog(["multiple dialog with huge horizontal label part";
"Only horizontal scroll bar"
"for the Label part";
strcat(string(1:150)," ")],..
["A";"B"], ..
["0";"1"]);

// test 4
x_mdialog(["multiple dialog with huge  label part";
"Vertical and horizontal scroll bars"
"for the Label part";
strcat(string(1:150)," ");
string(1:50)'],..
["A";"B"],..
["0";"1"]);

// test 5
x_mdialog(["multiple dialog with huge vertical dialog part";
"Only vertical scroll bar"
"for the Dialog part"],..
string(1:50)',..
string(1:50)');

// test 6
x_mdialog(["multiple dialog with huge horizontal dialog part";
"Only horizontal scroll bar"
"for the Dialog part"],"label",strcat(string(1:150)," "));


// test 7
x_mdialog(["multiple dialog with huge  dialog part";
"Vertical and horizontal  scroll bars"
"for the Dialog part"],..
["label";string(1:50)'],..
[strcat(string(1:150)," ");..
string(1:50)']);

// test 8
x_mdialog(["multiple dialog with huge  label and dialog part";
"Vertical and horizontal scroll bars"
"for the Label part";
strcat(string(1:150)," ");
string(1:50)'],..
["label";string(1:50)'],..
[strcat(string(1:150)," ");string(1:50)']);

// test 9
n      = 5;
m      = 4;
mat    = rand(n,m);
row    = "row";
labelv = row(ones(1,n))+string(1:n);
col    = "col";
labelh = col(ones(1,m))+string(1:m);
new    = evstr(x_mdialog("Small Matrix to edit",labelv,labelh,string(mat)));

n      = 20;
m      = 20;
mat    = rand(n,m);
row    = "row";
labelv = row(ones(1,n))+string(1:n);
col    = "col";
labelh = col(ones(1,m))+string(1:m);
new    = evstr(x_mdialog("Big Matrix to edit",labelv,labelh,string(mat)));
