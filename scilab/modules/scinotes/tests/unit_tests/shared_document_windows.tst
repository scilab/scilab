// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systemes S.E. - Cedric DELAMARRE
//
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- INTERACTIVE TEST -->

// The same file open in several SciNotes tabs and windows.
//
// Setup: open this file, open it a second time in the same window (File >
// Open), then use "Clone tab in a new window" to get a second window on it.
// Three tabs on one file: two in the first window, one in the second.
//
// Live editing
// 1. Type in one tab: the text appears in the other two as it is typed, and
//    the three tabs show the modified mark.
// 2. Ctrl+Z from any of the three tabs: the same text goes away everywhere,
//    a typed word at a time, and the modified mark disappears from all three
//    once the text is the one that was saved. Ctrl+Y brings it back.
// 3. The undo and redo arrows of the toolbar stay usable in both windows and
//    match what Ctrl+Z and Ctrl+Y do there.
// 4. Comment a line (Ctrl+D), indent a block (Ctrl+I), paste several lines:
//    the three tabs keep showing the same text.
//
// Saving
// 5. Ctrl+S from any tab: the modified mark disappears from the three tabs,
//    and no window asks whether to reload the file.
// 6. Type again, then undo back to the saved text: the three tabs lose their
//    modified mark together.
//
// Changes made outside SciNotes
// 7. With nothing modified, change the file with another editor: the three
//    tabs show the new text without asking anything, the cursor stays where
//    it was, and Ctrl+Z goes back to the text SciNotes was showing.
// 8. Type something, then change the file again with the other editor: the
//    text typed here is kept, and the tab asks whether to reload when it
//    gets the focus. Answering yes shows the file's text in all three tabs.
// 9. Delete the file from the disk: the text stays and the tabs show the
//    modified mark.
//
// Opening and closing
// 10. Type without saving, then open the file in a third window: it opens on
//     the text being typed, marked modified, and not on the text on disk.
// 11. Close one tab while another still shows the file: nothing is asked.
//     Closing the last one asks whether to save.
// 12. Close a window holding the file twice: the question is asked once, and
//     after saving no other window asks to reload.
// 13. "Clone tab in a new window" on a saved file: the new tab carries the
//     file name, not an Untitled number.
