// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

wMode = warning("query");
warning("off");

refMsg = msprintf(_("%s: Wrong type for input argument #%d: A string vector expected.\n"), "uipopup", 1);
assert_checkerror("res = uipopup(1);", refMsg);

refMsg = msprintf(_("%s: Wrong size for input argument #%d: A string vector expected.\n"), "uipopup", 1);
assert_checkerror("res = uipopup([""a"",""b"";""c"",""d""]);", refMsg);

res = uipopup("Item1")
// Clic on Item1
assert_checkequal(res, "Item1");


res = uipopup(["Item1";"Item2";"Item3"])
// Clic on Item2
assert_checkequal(res, "Item2");

warning(wMode);