// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH GRAPHIC -->

// Create a default figure
function closeRequestFcn()
    btn = messagebox("Do you want to close the figure?", "Close?", "question", ["Yes", "No"], "modal")
    if btn==1 then
        delete(gcf());
    else
        disp("Figure is still opened.");
    end
endfunction

plot3d();
// Try to close the figure using the top-right cross
// The figure must be closed

plot3d();
// Try to close the figure using delete(gcf());
// The figure must be closed

plot3d();
set(gcf(), "closerequestfcn", "closeRequestFcn()");
// Try to close the figure using the top-right cross
// A message box must be displayed
// Answer "No"
// The figure must still be visible
// The message "Figure is still opened." must appear in the console
// Try to close the figure using the top-right cross
// A message box must be displayed
// Answer "Yes"
// The figure must be closed
// No message must appear in the console

plot3d();
set(gcf(), "closerequestfcn", "closeRequestFcn()");
// Try to close the figure using delete(gcf());
// The figure must be closed

plot3d();
set(gcf(), "closerequestfcn", "closeRequestFcn()");
exit
// A message box must be displayed
// Answer "No"
// The figure must still be visible
// The message "Figure is still opened." must appear in the console
// Scilab must not quit.
exit
// A message box must be displayed
// Answer "Yes"
// The figure must be closed
// No message must appear in the console
// Scilab must quit.

