// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// unit tests for x_message function
// =============================================================================


// <-- INTERACTIVE TEST -->

x_message(["Simple message";"No scroll bar at all"]);

x_message(["message with huge vertical part";
           "Only vertical scroll bar"
          string(1:50)']);

x_message(["message with huge horizontal  part";
          "Only horizontal scroll bar"
           strcat(string(1:150),' ')]);

x_message(["huge message";
           "Vertical and horizontal scroll bars"
           strcat(string(1:150)," ");
           string(1:50)']);

