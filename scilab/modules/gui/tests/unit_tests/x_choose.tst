// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// unit tests for x_choose function
// =============================================================================


// <-- INTERACTIVE TEST -->


// test 1
x_choose(string(1:10)',["Simple choose";"No scroll bar at all"]);

// test 2
x_choose(string(1:10)',["choose with huge vertical label part";
          "Only vertical scroll bar"
          "for the Label part";
          string(1:50)']);

// test 3
x_choose(string(1:10)',["choose with huge horizontal label part";
          "Only horizontal scroll bar"
          "for the Label part";
           strcat(string(1:150),' ')]);

// test 4
x_choose(string(1:10)',["choose with huge  label part";
          "Vertical and horizontal scroll bars"
          "for the Label part";
           strcat(string(1:150)," ");
           string(1:50)']);

// test 5
x_choose(string(1:50)',["choose with huge vertical choose part";
          "Only vertical scroll bar"
          "for the Choose part"]);

// test 6
x_choose(strcat(string(1:150),' '),["choose with huge horizontal choose part";
          "Only horizontal scroll bar"
          "for the Choose part"]);

// test 7
x_choose([strcat(string(1:150)," ");string(1:50)'],["choose with huge  choose part";
          "Vertical and horizontal  scroll bars"
          "for the Choose part"]);

// test 8
x_choose([strcat(string(1:150)," ");string(1:50)'],["choose with huge  label and choose part";
          "Vertical and horizontal scroll bars"
          "for the Label part";
           strcat(string(1:150)," ");
           string(1:50)']);
