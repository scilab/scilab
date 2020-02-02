// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - ESI Group - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 15996 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15996
//
// <-- Short Description -->
// Priority of closerequestfcn and callbacks with while loop

function quit_waiter()
    a = resume(1);
endfunction

figure(...
    "tag", "quit_fig", ...
    "closerequestfcn", "quit_waiter");

printf("Close the figure to continue");

a = 0;
tic();
while a == 0
    printf("\r");printf("%0.2f", toc());
    sleep(50);
end

delete(get("quit_fig"));
