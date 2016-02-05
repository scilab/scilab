// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6387 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6387
//
// <-- Short Description -->
// 'dos' failed to execute very long command line and returns an msg error : line too long

// try as very long command as (example)

dos("""D:\GIT-SC~2\scilab\scilab\bin\translator.exe"" -lib " + ...
"""C:\Users\toto\AppData\Local\Temp\SCI_TMP_6196_\MYMOPACKAGE.mo"" -lib " + ...
"""C:\Users\toto\AppData\Local\Temp\SCI_TMP_6196_\RLC_circuit_im.mo"" -o " + ...
"""C:\Users\toto\AppData\Local\Temp\SCI_TMP_6196_\RLC_circuit_imf.mo"" -with-init-command ""RLC_circuit_im " + ...
"RLC_circuit_imf;"" > ""C:\Users\toto\AppData\Local\Temp\SCI_TMP_6196_\S_translator.err""")
