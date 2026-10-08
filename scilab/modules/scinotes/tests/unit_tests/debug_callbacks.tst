// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systemes S.E. - Cedric DELAMARRE
//
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- INTERACTIVE TEST -->

// Debugging the callbacks of a window. The code below is what the scenario
// runs; open this file in SciNotes, next to the console.
//
// The keys: Ctrl+F5 saves and runs the file without the debugger. F5 saves and
// runs it with the debugger, and Scilab stays in debug mode once the file has
// run: the status bar of SciNotes has another colour. Shift+F5 leaves debug
// mode.
//
// Without the debugger
// 1. Click in the margin of the "n = ..." line of countUp(): a red dot
//    appears. Press Ctrl+F5: a window opens with a 0 and two buttons. Click
//    Count: the number becomes 1, nothing stops.
// 2. Click Fail: an error is displayed in the console, nothing stops. Close
//    the window.
//
// In debug mode
// 3. Press F5: the window opens again, the status bar of SciNotes changes
//    colour and the console prompt is "debug>", although the file has run to
//    its end.
// 4. Click Count: the callback stops on the dot, the line is highlighted in
//    SciNotes and the prompt is "break>". Hover h: the handle of the text.
// 5. Still stopped, click Count three times: each click runs at once, without
//    stopping on the dot, and the number becomes 3. Click Fail: the error is
//    displayed in the console and the stop goes on. Press F5: the stopped
//    callback goes on and the number becomes 4.
// 6. Click Count, and once stopped press F10 twice: the stop moves down the
//    function. Press F5: the number becomes 5.
// 7. Click Count. Once stopped, open the Variable Browser (Applications
//    menu) and double-click a variable: its editor opens. The tools of
//    Scilab keep working during a stop. Press F5.
// 8. Click Fail: the callback stops on the error, shown in red in SciNotes
//    with its message. Press F5: the error is displayed in the console.
//    The status bar keeps its colour: Scilab is still in debug mode.
// 9. Click Count. Once stopped, close the window with its cross: it closes.
//    Press Shift+F5.
//
// Leaving debug mode
// 10. Press F5, then Shift+F5: the status bar has its usual colour and the
//     prompt is "-->". Click Count: the number changes, nothing stops.
// 11. Close the window. Press F5, close the window it opens, then press
//     Ctrl+F5: the status bar has its usual colour. Click Count: nothing
//     stops. Close the window.

function countUp()
    h = findobj("tag", "debug_callbacks_counter");
    n = evstr(h.string) + 1;
    h.string = string(n);
endfunction

function failing()
    error("failing callback");
endfunction

f = figure("figure_name", "debug_callbacks", "position", [100 100 320 130], ..
           "menubar", "none", "toolbar", "none", "infobar_visible", "off");
uicontrol(f, "style", "text", "tag", "debug_callbacks_counter", "string", "0", ..
          "position", [20 80 280 30]);
uicontrol(f, "style", "pushbutton", "string", "Count", "callback", "countUp()", ..
          "position", [20 20 130 40]);
uicontrol(f, "style", "pushbutton", "string", "Fail", "callback", "failing()", ..
          "position", [170 20 130 40]);
