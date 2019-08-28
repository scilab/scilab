// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 10455 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/10455
//
// <-- Short Description -->
// In STD or NW mode, when the help browser is opened,
//  A) when Scilab is quit, the browser restored at the next startup
//    - did not display the page left when exiting
//    - did not list help chapters of autoloaded ATOMS modules.
//
//  B) the help browser was not updated with the new chapter when
//    - an ATOMS module is loaded in the session, or
//    - a help chapter is added with add_help_chapter()


function restart_scilab()
    if getos()=="Windows"
        host("Start """" """+sciargs()+"""")
    else
        host(sciargs()+" &")
    end
    exit
endfunction

// Be sure that at least one ATOMS module (with help pages) is installed and autoloaded
// ------------------------------------------------------------------------------------
// If it's not already the case, do the following:
// -----------------------------------------------
offline = atomsGetConfig("offline");
atomsSetConfig("offline","True")
in = atomsInstall(SCI+"/modules/atoms/tests/unit_tests/toolbox_7V6_1.0-1.bin.zip")
atomsSetConfig("offline", offline);
atomsAutoloadAdd(in(1));
restart_scilab()
// Otherwise: continue

// Then:
// ====
// WITH A NATIVE current help page:
// -------------------------------
help linspace
restart_scilab()
// In the new fresh session:
// CHECK: The help browser must be opened, and at the linspace page

// WITH a page from an ATOMS EXTERNAL MODULE:
// -----------------------------------------
// Now, open a help page belonging to an autoloaded module
// help <the_page>
help t1_function1
// Keep the help browser open, and:
restart_scilab()

// In the new fresh session:
// CHECK: The help browser must be opened, and at the <the_page> ATOMS module page

// Cleaning
atomsRemove("toolbox_7V6")


// ====================================
// When Loading an ATOMS module by hand
// ====================================
// In the ongoing session, let's Consider an installed but not loaded ATOMS module. Say plotplot.

global %helps
setlanguage en
helpbrowser(%helps(:,1), "linspace", "ru_RU", %f)
// Check that the page is in Russian
atomsLoad plotplot
// Check that the plotplot help chapter is listed in the browser.
// Check that the displayed page is still linspace in Russian.


// ================
// ADDITIONAL TESTS
// ================
// In the ongoing Scilab session, CHECK :
// * When the loaded external module has some dependencies
//   (ex: MatrixMarket depends on and installs apifun),
//   the updated browser must list chapters of the dependencies as well.
