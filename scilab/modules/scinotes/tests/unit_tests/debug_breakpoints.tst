// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systemes S.E. - Cedric DELAMARRE
//
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- INTERACTIVE TEST -->

// Debugging a file from SciNotes: breakpoints, stops, and the execution
// controls. The code below is what the scenario runs; open this file and
// press F5.
//
// The keys: F5 saves and runs the file with the debugger, and continues while
// stopped. Ctrl+F5 saves and runs it without the debugger. F6 pauses, F10
// steps over, F11 steps in, Shift+F11 steps out, Shift+F5 ends the run and
// leaves debug mode. These are also in the Debug menu and the toolbar. F9
// sets or removes a breakpoint on the line of the caret, Ctrl+F10 runs up to
// that line.
//
// F5 puts Scilab in debug mode, and it stays in it once the run is over: the
// status bar of SciNotes has another colour and the console prompt is
// "debug>" ("break>" while stopped). Shift+F5 and Ctrl+F5 leave debug mode.
//
// Setting a breakpoint
// 1. Click in the margin left of the line numbers, on the "total" line of
//    accumulate(): a red dot appears. Click it again: it goes away. Click
//    in the text of that line and press F9: the dot appears, and goes away
//    with a second F9. Move the pointer up and down that margin: a pale dot
//    follows it, showing where a click would land, and never on a line that
//    already has one.
// 2. Put the dot back and press F5: the run stops on that line, which is
//    highlighted, with a hollow amber arrow in the margin. No chevron in the
//    code: the line starts with what is about to run. The red dot shows
//    through that arrow: click it and the breakpoint behind is removed,
//    click again and it comes back, without the arrow moving.
// 3. F10 steps over, F11 steps into scale(), Shift+F11 steps back out. Both
//    marks follow, and the file of the line stopped on is opened. On a line
//    holding several instructions, an amber chevron appears in the code
//    before the second one, a little apart from it, and moves from one to
//    the next; the rest of that line is highlighted from there.
// 4. F5 continues and stops again on the next turn of the loop; Shift+F5
//    ends the run. The arrow goes away and the red dot stays.
// 5. Run again and press the Pause key while it runs: it stops where it was,
//    on the line being executed.
//
// A stop on an error
// 6. Remove the comment from the last line of the file, and press F5:
//    the run stops on it, the arrow and the line in red, with the error
//    message shown at the end of the line. Stepping is refused there; F5
//    ends the run, and the error is displayed in the console.
// 7. Narrow the window until the stopped line wraps onto two rows: the
//    arrow, the red dot and an anchor on that line appear on its first row
//    only.
//
// The three marks together
// 8. Set an anchor (Ctrl+M) on a line, put a breakpoint on it and run: the
//    anchor keeps the line-number cell, the breakpoint and the arrow of the
//    stop share the margin. Both are visible at once.
// 9. Set the line numbering to off (Preferences), then run again: the
//    arrow is still there. The anchor is not -- it lives in
//    the cell that numbering off removes.
//
// A split tab
// 10. Split the tab (the Window menu). Both halves show the same dots, and a
//     click in the margin of either half sets and removes them. The pale dot
//     follows the pointer in the half it is in, and only there.
//     In the left half, type a new line, then put a dot on an empty line and
//     one on a comment line: they stay pale. Save: each dot jumps onto the
//     next line with code. Do it again from the right half.
// 11. Run with a breakpoint: both halves show the arrow on the same line.
//     Shift+F5 ends the run, from either half.
// 12. Remove the split (the Window menu), edit the file: what is typed shows.
//     Run again: it stops on the breakpoint, F5 continues and Shift+F5 ends
//     the run.
//
// The same file in two windows
// 13. "Clone tab in a new window", then set a breakpoint in one window: the
//     other window shows the same dot, filled the same way.
// 14. Run from either window: both stop on the line, and both show the
//     arrow.
// 15. Close the window where the dot was set, keep the other, and run again:
//     the run still stops there.
// 16. Close every window showing the file, open the file again: the dot is
//     still there.
//
// Editing around a breakpoint
// 17. Add a few lines above a breakpoint without saving: the dot follows its
//     line, paler until the file is saved.
// 18. F5 (it saves first): the stop happens on the line the dot is on.
// 19. Stop on a breakpoint, change a line of the file: a yellow
//     banner at the top of the editor says that the file has changed
//     since it was run. Undo until the file is back as
//     it was run: the banner goes. Change it again and let the run finish
//     (F5 until the end, or Shift+F5): the banner goes too.
// 20. Save As under another name and run: the dots come along with the new
//     file. Open the original again: it still has its own dots.
// 21. Stop on a breakpoint, then Save As: the original file opens again,
//     stopped. F5 continues. Starting another run meanwhile is refused with
//     a message in the bottom bar, nothing in the console.
//
// Without a debug session
// 22. Set a breakpoint in a file, quit Scilab, start it again and open the
//     file: the dot is still there.
//
// Lines where nothing runs
// 23. Put a dot on the empty line above "function total = accumulate(n)":
//     it jumps onto that "function" line, and is still there after closing
//     and opening the file again.
// 24. Put a dot on the very last line of the file, the comment after all the
//     code: it stays pale, and its tooltip reads "Unreachable breakpoint".
//     Running the file does not stop there.
//
// Values under the mouse
// 25. Put a dot on the "out = value * factor" line and press F5. Once
//     stopped, hover value and factor: each shows its value in scale().
//     Select "value * factor" on that line and hover the selection: it
//     shows the product.
// 26. End the run (Shift+F5), then hover value in scale() again: no value
//     is shown, the function is not running.
//
// Debug mode and normal mode
// 27. Keep the dot of step 25 and press Ctrl+F5: the status bar has its
//     usual colour, the file runs to its end without stopping, and the
//     prompt stays "-->". The entries of the Execute menu do the same.
// 28. In the console, type accumulate(2): it does not stop either.
// 29. Press F6, then Shift+F5: each time the status bar says Scilab is not
//     in debug mode, and nothing else happens.
// 30. Press F5: the status bar changes colour, the console prompt becomes
//     "debug>" on the line it was on, then "break>" when the run stops on
//     the dot. Press F10 a few times: the prompt stays "break>", without
//     blinking.
// 31. Still stopped, type scale(1, 2) in the console: the result is shown and
//     no second stop happens.
// 32. F5 until the end of the run: the prompt is "debug>" and the status bar
//     keeps its colour, Scilab is still in debug mode. In the console, type
//     accumulate(2): this time it stops on the dot. F5 until it ends, then
//     Shift+F5.
// 33. While a Ctrl+F5 run is going on, press F5: it is refused, and the
//     status bar of SciNotes says Scilab is busy. Press Ctrl+F5 instead: the
//     file runs a second time, once the first run is over.
//
// Running to the caret
// 34. Remove every dot. Click in the text of the "total = total + ..." line
//     and press Ctrl+F10: the file runs with the debugger and stops on that
//     line, with no dot in the margin. Click in the "out = value * factor"
//     line and press Ctrl+F10: the run stops there. Press F5: it stops on
//     neither line again and runs to its end. Shift+F5.

function out = scale(value, factor)
    out = value * factor;
endfunction

function total = accumulate(n)
    total = 0;
    for i = 1:n
        total = total + scale(i, 2);
        disp(total);sleep(500)
    end
endfunction

disp(accumulate(10));

//disp(accumulate("five"));

// The last line: no code after it (step 24).
