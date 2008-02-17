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
x_dialog(["Simple dialog";"No scroll bar at all"],["0";"1"]);

// test 2
x_dialog(["dialog with huge vertical label part";
          "Only vertical scroll bar"
          "for the Label part";
          string(1:50)'],["0";"1"]);

 // test 3
x_dialog(["dialog with huge horizontal label part";
          "Only horizontal scroll bar"
          "for the Label part";
           strcat(string(1:150)," ")],["0";"1"]);

// test 4
x_dialog(["dialog with huge  label part";
          "Vertical and horizontal scroll bars"
          "for the Label part";
           strcat(string(1:150)," ");
           string(1:50)'],["0";"1"]);

// test 5
x_dialog(["dialog with huge vertical dialog part";
          "Only vertical scroll bar"
          "for the Dialog part"],string(1:50)');

// test 6
x_dialog(["dialog with huge horizontal dialog part";
          "Only horizontal scroll bar"
          "for the Dialog part"],strcat(string(1:150)," "));

// test 7
x_dialog(["dialog with huge  dialog part";
          "Vertical and horizontal  scroll bars"
          "for the Dialog part"],[strcat(string(1:150)," ");string(1:50)']);

// test 8
x_dialog(["dialog with huge  label and dialog part";
          "Vertical and horizontal scroll bars"
          "for the Label part";
           strcat(string(1:150)," ");
           string(1:50)'],[strcat(string(1:150)," ");string(1:50)']);
