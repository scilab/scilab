// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
// Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Unitary tests for uiCreateNode()
// --------------------------------

// node creation
myNode = uiCreateNode("My Node")
myNode = uiCreateNode("My Node", "icon of My Node")
myNode = uiCreateNode("My Node", "icon of My Node", "callback of My Node")


// Error messages for wrong inputs
// -------------------------------
// Wrong number of input arguments:
refMsg =  msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"), "uiCreateNode",1,3);
assert_checkerror("myNode = uiCreateNode()", refMsg);
refMsg =  msprintf(_("Wrong number of input arguments.\n"));
assert_checkerror("myNode = uiCreateNode(''My Node'', ''icon of My Node'', ''callback of My Node'', ''test'')", refMsg);
assert_checkerror("myNode = uiCreateNode(''My Node'', ''icon of My Node'', ''callback of My Node'', 123)", refMsg);


// Wrong type of input arguments:
refMsg = msprintf(_("%s: Wrong type for input argument #%d: String expected.\n"), "uiCreateNode",1);
assert_checkerror("myNode = uiCreateNode(123)", refMsg);

refMsg = msprintf(_("%s: Wrong type for input argument #%d: String expected.\n"), "uiCreateNode",1);
assert_checkerror("myNode = uiCreateNode(123, ''icon of My Node'')", refMsg);

refMsg = msprintf(_("%s: Wrong type for input argument #%d: String expected.\n"), "uiCreateNode",2);
assert_checkerror("myNode = uiCreateNode(''My Node'', 123)", refMsg);

refMsg = msprintf(_("%s: Wrong type for input argument #%d: String expected.\n"), "uiCreateNode",1);
assert_checkerror("myNode = uiCreateNode(123, ''icon of My Node'', ''callback of My Node'')", refMsg);

refMsg = msprintf(_("%s: Wrong type for input argument #%d: String expected.\n"), "uiCreateNode",2);
assert_checkerror("myNode = uiCreateNode(''My Node'', 123, ''callback of My Node'')", refMsg);

refMsg = msprintf(_("%s: Wrong type for input argument #%d: String expected.\n"), "uiCreateNode",3);
assert_checkerror("myNode = uiCreateNode(''My Node'', ''callback of My Node'', 123)", refMsg);
