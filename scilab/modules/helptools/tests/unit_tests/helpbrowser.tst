// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// help browser

// ----------------------------------------
// Switching the language: Menu "Languages"
// ----------------------------------------
help ones
//  - Switch the language.
//  - CHECK that the same page is displayed in the help browser in the
//    required language, for the main 5 supported languages.

// -------------------------------------
// Seeing the page online: Menu "Online"
// -------------------------------------
//  - Be sure to have an active internet connexion

// Page of a native function
// .........................
help ones
//  - Select "See this page online"
//  - CHECK:
//    - Your web browser must open
//    - The online version of the page must be displayed
//      - for the current helpbrowser language
//      - for the current Scilab version

// Page of a section summary
// .........................
// - In the help browser, display a section summary
// - Select the "Online" menu
// - CHECK: a modal messagebox must open, saying
//   "Help browser: Tables of contents can't be displayed online"
//   The box is modal: you must click "OK" to close it and unlock Scilab

// Plain page from an external module
// ..................................
// Be sure that an external module with help pages is loaded
// - In the help browser, display a plain page of the external module
// - Select the "Online" menu
// - CHECK: a modal messagebox must open, saying
//   "Help browser: Pages of external modules can't be viewed online"
//   The box is modal: you must click "OK" to close it and unlock Scilab

// -----------------------------------------------------
// List related bugs declared on bugzilla: Menu "Issues"
// -----------------------------------------------------
//  - Be sure to have an active internet connexion
//
// Issues about a native function
// ..............................
help plot2d
// a) Select the menu "Issues => List ALL issues declared for this item (online)"
//   CHECK:
//   * Your web browser must open
//   * The listed items must all have "plot2d" in their summary
//   * Unresolved issues must be listed first, sorted by decreasing bug_id
//   * Resolved issues must then be listed. Their list is sorted
//     a) by resolution status b) by decreasing bug_id
// b) Select the menu "Issues => List only UNRESOLVED issues declared for this item (online)"
//   CHECK:
//   * Your web browser must open
//   * The listed items must all have "plot2d" in their summary
//   * Only UNRESOLVED issues must be listed, sorted by decreasing bug_id
//     This includes NEW, ASSIGNED, and REOPENED issues

// Issues about a section summary
// ..............................
// * Display a section summary in the help browser
// * Select any item in the "Issues" menu
// * CHECK: a modal messagebox must open, saying
//   "Help browser: Tables of contents can't be displayed online"

// Issues about an external feature
// ................................
// Be sure that an external module with help pages is loaded
// * Display a section summary in the help browser
// * In the help browser, display a plain page of the external module
// * Select any item in the "Issues" menu
// * CHECK: a modal messagebox must open, saying
//   "Help browser: Bugs of external modules can't be listed"
