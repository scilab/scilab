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

jdeff("java.lang.Double", "parseDouble", "str2number");
PI = str2number("3.141592653589");
assert_checkalmostequal(PI,%pi);
clear str2number;

jdeff("java.lang.Double", ["parseDouble", "toString"], ["str2dbl", "dbl2str"]);
assert_checkalmostequal(str2dbl(dbl2str(%pi)), %pi);
clear str2dbl dbl2str;
