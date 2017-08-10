// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// <-- INTERACTIVE TEST -->

test = TMPDIR + "/testCompletionOnMlist.sci";
ML = mlist(["type" "a" "ab" "abc" "abcd" "bcd"],1,2,3,4,5);
mputl(["// Put the cursor after ML."; ..
       "ML."; ..
       "// and hit CTRL+<SPACE>, you should have a, ab, abc, abcd, bcd"; ..
       "ML.ab"
       "// now put the cursor after ab and CTRL+<SPACE>, you should have ab, abc, abcd"], test);
scinotes(test);
