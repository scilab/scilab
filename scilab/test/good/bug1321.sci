// Non-regression test file for bug 1321
// Copyright INRIA
// Scilab Project - V. Couvert

mode(-1);
clear all;

h=figure(1);

t = uicontrol( h, "style","text", ...
  "string","This is a figure", ...
  "position",[50 70 100 100], ...
  "fontsize",15, ...
  "tag","Alabel");

correct = findobj("tag","Alabel")==t & ...
    isempty(findobj("tag","alabel")) & ...
    findobj("Tag","Alabel")==t & ...
    isempty(findobj("Tag","alabel"));

close(1);

affich_result(correct,1321);

clear all

