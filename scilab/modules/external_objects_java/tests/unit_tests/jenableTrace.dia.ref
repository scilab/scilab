//
//  Copyright (C) 2013 - S/E - Sylvestre Ledru
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
//
// <-- JVM MANDATORY -->
//
debugFile = fullpath(TMPDIR + "/debug_jims.log");
jenableTrace(debugFile);
jwrap(2);
jdisableTrace();
assert_checktrue(isfile(debugFile));
mdelete(debugFile);
jwrap(2);
assert_checkfalse(isfile(debugFile));
