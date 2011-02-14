// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- INTERACTIVE TEST -->

test = TMPDIR + "/testCompletionOnMlist.sci";
ML = mlist(["type" "a" "ab" "abc" "abcd" "bcd"],1,2,3,4,5);
mputl(["// Put the cursor after ML."; ..
       "ML."; ..
       "// and hit CTRL+<SPACE>, you should have a, ab, abc, abcd, bcd"; ..
       "ML.ab"
       "// now put the cursor after ab and CTRL+<SPACE>, you should have ab, abc, abcd"], test);
scinotes(test);
