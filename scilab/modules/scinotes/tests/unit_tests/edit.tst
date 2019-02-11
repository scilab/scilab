// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// =============================================================================
//                          Unitary tests for edit()
// =============================================================================

// <-- INTERACTIVE TEST -->

// 1) edit a new untitled macro
//    -------------------------
edit  // Must open TMPDIR/untitled.sci with the following (uncommented) content:
// function [] = untitled()
// endfunction


// 2) edit a new macro with given name following functions naming rules
//    -----------------------------------------------------------------
clear abcdef
edit abcdef  // Must open TMPDIR/abcdef.sci with the following (uncommented) content:
// function [] = abcdef()
// endfunction


// 3) edit an existing file, while macroname does not follow macros naming rules (due to "/" and ".")
//    --------------------------------------------------------------------------
edit SCI/Version.incl  // Must open SCI/Version.incl


// 4) edit a new file, while macroname does not follow macros naming rules (due to ".")
//    ---------------------------------------------------------------------
edit trials.sce   // Must ask for confirmation and then open trials.sce in the working directory


// 5) edit a native macro directly
//    ----------------------------
clear factors
edit factors  // Must open SCI/modules/polynomials/macros/factors.sci


// 6) edit a native macro through an alias
//    ------------------------------------
sinus = sind
edit sinus  // Must display a warning and open SCI/modules/elementary_functions/macros/sind.sci
// --> edit sinus
// WARNING: edit: "sinus" is an alias of "sind" => editing sind()
// WARNING:       Please reset sinus=sind if sind() is recompiled.


// 7) edit a native macro overwritten by a user-defined macro
//    -------------------------------------------------------
function r = factors(a)
    disp("Actually my factors()")
endfunction
edit factors  // Must still open SCI/modules/polynomials/macros/factors.sci


// 8) edit an existing user-defined function with no alias in libraries
//    -----------------------------------------------------------------
function r = my_test(w)
  // heading comments
  r = w+2
endfunction
edit my_test    // Must open TMPDIR/my_test.sci


// 9) try to edit a primitive
//    -----------------------
clear sin
edit sin     // Must display the following error:
// --> edit sin
// at line    78 of function edit ( SCI\modules\core\macros\edit.sci line 92 )
// edit: sin() is an uneditable hard coded function.


// 10) edit a native macro through an alias named after a primitive
//     ------------------------------------------------------------
sin = sind
edit sin  // Must display a warning and open SCI/modules/elementary_functions/macros/sind.sci
// --> edit sin
// WARNING: edit: "sin" is an alias of "sind" => editing sind()
// WARNING:       Please reset sin=sind if sind() is recompiled.


// 11) edit a macro in a custom library, named as an existing primitive
//     ----------------------------------------------------------------
clear sin
path = pathconvert("TMPDIR/editest");
mkdir(path);
mputl("function a = sin(a),a = a+%pi, endfunction", path+"sin.sci");
genlib("sinlib", path);
sinlib = lib(path);
sinlib.sin(2);
edit sin     // Must display the following error:
// --> edit sin
// at line    78 of function edit ( SCI\modules\core\macros\edit.sci line 92 )
// edit: sin() is an uneditable hard coded function.
//
// NOTE: This could be improved by searching in loaded libraries
rmdir(path, "s");
