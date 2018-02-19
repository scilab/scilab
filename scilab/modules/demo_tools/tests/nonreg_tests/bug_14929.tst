// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 14929 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14929
//
// <-- Short Description -->
// When the default language is not en_US, setlanguage failed updating the main
// demos labels in the demo GUI.


// 1) launch scilab in normal mode but -- say -- in Russian (all supported
//    languages except english):
//    # scilab -l ru_RU
//    or
//    # wscilex -l ru_RU
//    or in Scilab on MS Windows:
setdefaultlanguage ru_RU
exit
      // restart Scilab
// 2) Run the demo GUI:
demo_gui
// 3) Check that most of the MAIN labels (in the left column) are in Russian
//   For instance, the "Signal processing"  entry should be labelled
//   "Обработка сигналов" (7th row)
// 4) Close the demo GUI
// 5) Change the current language, for instance to french, of the current session
setlanguage fr_FR
// 6) Reopen the demo GUI:
demo_gui
// 7) The "Signal Processing" item must now be translated into
//    "Traitement du signal" as the translation also exists:
//--> gettext("Signal Processing")
// ans  =
// Traitement du signal
//    Check it visually.
//  Whether it is the case, the bug is FIXED. Otherwise it is still unfixed.
// 8) Finally check that even going to english with setlanguage() is possible:
//    * Close the demo GUI
      setlanguage en_US
      demo_gui
//    If the bug is unfixed, even this will let items in the default Russian
