// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

messagebox(["messagebox: Simple example of message ";
"First row of comments";
"Second row";
"etc...";],"modal");

x_dialog("x_dialog: trial","                ")
x_dialog("Trial           ","3.1592653")
x_dialog(["trial           ";"trial1";"trial2";"trial3";"trial4";..
"trial6";"trial7"],"[0 1 2 3]")

messagebox("trial","modal");
messagebox(["trial";"trial1";"trial2";"trial3";"trial4";"trial6";"trial7";..
"trial3";"trial4";"trial6";"trial7";"trial3";"trial4";"trial6";"trial7"],"modal");

x_choose(["trial";"trial1";"trial2";"trial3";"trial4";"trial7";..
"trial3";"trial4";"trial6";"trial7";"trial3";"trial4";"trial6"],..
"selection (click on one item --> returns the item number)")

x_mdialog(["here: your message";...
"(edit the boxes below --> output: edited boxes)"],...
["first row";"second row";"etc..."],...
["value1";"value2";"value3"])

n=5;m=4;mat=rand(n,m);
row="row";labelv=row(ones(1,n))+string(1:n);
col="col";labelh=col(ones(1,m))+string(1:m);
new=evstr(x_mdialog("Matrix to edit",labelv,labelh,string(mat)))
