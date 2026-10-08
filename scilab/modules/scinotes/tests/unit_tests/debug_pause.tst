// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systemes S.E. - Cedric DELAMARRE
//
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- INTERACTIVE TEST -->

// pause, resume and abort, in a file run with and without the debugger. The
// code below is what the scenario runs; open this file in SciNotes, next to
// the console.
//
// The keys: Ctrl+F5 saves and runs the file without the debugger. F5 saves and
// runs it with the debugger, and continues while stopped; Scilab then stays
// in debug mode (prompt "debug>", "break>" while stopped) until Shift+F5 or
// Ctrl+F5.
//
// Without the debugger
// 1. Press Ctrl+F5: the console prompt becomes "-1->". Type x: 1 is shown.
//    Type r: 2 is shown.
// 2. Type resume: the prompt is "-1->" again, for the second turn of the
//    loop, and x is now 2. Type resume once more: 6 is displayed and the
//    prompt is back to "-->".
// 3. Press Ctrl+F5, then type abort: the prompt is back to "-->" and nothing
//    is displayed.
// 4. Press Ctrl+F5, then type pause: the prompt becomes "-2->". Type resume:
//    "-1->". Type abort: "-->", nothing is displayed.
// 5. Press Ctrl+F5. At the "-1->" prompt, press F5 in SciNotes: it is
//    refused, the status bar of SciNotes says Scilab is busy, and nothing
//    changes in the console. Press Ctrl+F5: the file runs again, inside the
//    pause, and the prompt becomes "-2->". Type abort: "-->".
// 6. Nothing running, type pause in the console: "-1->". Press F5 in
//    SciNotes: refused the same way, in its status bar. Type debug: refused
//    with a message in the console, and the prompt stays "-1->". Type
//    resume: "-->".
//
// With the debugger
// 7. Press F5: the console prompt becomes "debug>", then "break>" when the
//    run stops on the pause line of inner(): it is highlighted, with the arrow
//    in the margin, although it is the last line of the function. The prompt
//    is never "-1->". Hover x: its value, 1, under what is known of the
//    name. Hover r: 2. The Variable Browser of Scilab, when it is open,
//    shows the variables of inner().
// 8. Press F5: the run stops on the same line again, and x is now 2. Press F5:
//    6 is displayed, the arrow goes away and the prompt is "debug>": Scilab
//    is still in debug mode.
// 9. Press F5. Once stopped, type resume in the console: SciNotes shows the
//    run going on, then stopped again on the pause line. Type resume once
//    more: 6 is displayed and SciNotes shows no stop any more.
// 10. Press F5. Once stopped, type abort in the console: nothing is displayed,
//     SciNotes shows no stop any more, and the prompt is "debug>".
// 11. Press F5. Once stopped, press Shift+F5: nothing is displayed, the prompt
//     is "-->" and the status bar has its usual colour.
// 12. Press F5. Once stopped, type pause in the console: a message says the
//     execution is already paused, and the prompt stays "break>". Press F5:
//     the run goes to its next stop, as if nothing had been typed. Press F5
//     until the run ends.
// 13. Press F5. Once stopped, press F10: the stop leaves inner() for the loop
//     of outer(). Press F5 until the run ends.
//
// In debug mode, nothing running
// 14. At the "debug>" prompt, type pause: a message says there is no debugged
//     execution to pause.
// 15. Type outer(1): the line is debugged, and stops on the pause line of
//     inner(), prompt "break>". Press F5: 2 is displayed.
//
// pause and Ctrl+C while a debugged run is going on
// 16. Remove the comment from the last line of the file and press F5, then F5
//     twice to go past the two stops of inner(). While the last line runs,
//     type pause in the console: the run stops on that line, as the Pause key
//     (F6) does, and the prompt is "break>". Press F5: it runs on. Type
//     "pause; disp(1+1)" on one line: the run stops the same way and nothing
//     is displayed yet. Press F5: 2 is displayed and the run goes on.
// 17. While it runs, press Ctrl+C in the console: the run stops the same way.
// 18. Still stopped, type the last line of the file in the console, and press
//     Ctrl+C while it runs: the line is abandoned and the stopped run with
//     it; SciNotes shows no stop any more and the prompt is "debug>".
// 19. Press Shift+F5: the prompt is "-->".
//
// The status bars, and closing Scilab during a stop
// 20. Press F5. Once stopped, the status bar of SciNotes says the execution
//     is paused and that Shift+F5 leaves debug mode; the one of the console
//     says "Debug mode: execution stopped". Click in the code of inner(): the
//     status bar of SciNotes gives the position, and still says the execution
//     is paused after it.
// 21. Press F5 twice to go past the two stops: while the last line runs, both
//     status bars say the execution is running. Once it is over, the one of
//     SciNotes says the execution is completed, the one of the console "Debug
//     mode".
// 22. Put the comment back on the last line and press F5.
//     Once stopped, close the Scilab window with its cross: SciNotes and
//     Scilab close, and the command that started Scilab gives the hand back.

function r = inner(x)
    r = x * 2;
    pause
endfunction

function total = outer(n)
    total = 0;
    for i = 1:n
        total = total + inner(i);
    end
endfunction
disp(outer(2));

for k = 1:40, sleep(500); end
