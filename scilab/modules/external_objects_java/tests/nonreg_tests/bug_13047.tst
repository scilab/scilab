// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================
//
// <-- Non-regression test for bug 13047 -->
//
// <-- JVM MANDATORY -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13047
//
// <-- Short Description -->
// jcompile did not allow class reloading
//

function r = myconvert(Text, method)
    // Compiling the meta-methode
    arrayConv = jcompile("arrayConv", ..
    ["public class arrayConv {"
    "public static String[] convInArray(String arr[]) {"
    "    int nbelem = arr.length; "
    "    String[] out = new String[nbelem];"
    "    for (int i = 0; i < nbelem; i++)  "
    "        out[i] = arr[i]."+method+"(); "
    "    return out; "
    "    } "
    "} "
    ])
    r = matrix(arrayConv.convInArray(Text(:)'), size(Text))
    // jremove arrayConv convInArray
endfunction

strs = ["Scilab" "GDL" "Yorick" "Octave" "Scipy"];

assert_checkequal(convstr(strs, "u"), myconvert(strs,"toUpperCase"));
assert_checkequal(convstr(strs, "l"), myconvert(strs,"toLowerCase"));