// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1321 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1321
//
// <-- Short Description -->
//    Function findobj does not work when property value contains
//    capital letters and is case sensitive for property name.

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

if correct == %F then pause,end
