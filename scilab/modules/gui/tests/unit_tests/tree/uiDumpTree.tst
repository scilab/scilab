// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Unitary tests for uiDumpTree()
// ==============================

// DISPLAY
// -------
// Leaves, nodes, branches and tree creation
node1 = uiCreateNode("Node 1", "iconNode1", "callbackNode1");
leaf11 = uiCreateNode("leaf 1.1", "iconLeaf1.1", "callbackLeaf1.1");
leaf12 = uiCreateNode("leaf 1.2", "iconLeaf1.2", "callbackLeaf1.2");
treeNode1 = uiCreateTree(node1, leaf11, leaf12);

node2 = uiCreateNode("Node 2", "iconNode2", "callbackNode2");

node3 = uiCreateNode("Node 3", "iconNode3", "callbackNode3");
leaf31 = uiCreateNode("leaf 3.1", "iconLeaf3.1", "callbackLeaf3.1");
leaf32 = uiCreateNode("leaf 3.2", "iconLeaf3.2", "callbackLeaf3.2");
treeNode3 = uiCreateTree(node3, leaf31, leaf32);

root = uiCreateNode("Root", "iconRoot", "callbackRoot");
treeRoot = uiCreateTree(root, treeNode1, node2, treeNode3);

// Dumping leaves
uiDumpTree(leaf11);
uiDumpTree(leaf11, %F);
uiDumpTree(leaf11, %T);

// Dumping a single naked node
treeOnlyNode1 = uiCreateTree(node1);
uiDumpTree(treeOnlyNode1);
uiDumpTree(treeOnlyNode1,%F);
uiDumpTree(treeOnlyNode1,%T);

// Dumping a single node with leaves
uiDumpTree(treeNode1);
uiDumpTree(treeNode1, %F);
uiDumpTree(treeNode1, %T);

// Dumping the whole tree
uiDumpTree(treeRoot);
uiDumpTree(treeRoot,%F);
uiDumpTree(treeRoot,%T);


// % "\n" "\r" "\t" in contents (http://bugzilla.scilab.org/16129)
// ----------------------------
node1 = uiCreateNode("c:\nine %", "c:\nine\ic%on", "\nine\callback%d");
leaf11 = uiCreateNode("c:\ni#ne\return", "c:\nine\return\icon", "c:\nine\return\callback");
leaf12 = uiCreateNode("c:\nine\tabs", "c:\nine\tabs\icon", "c:\nine\tabs\callback");
treeNode1 = uiCreateTree(node1, leaf11, leaf12);
root = uiCreateNode("\root", "\root\icon", "\root\callback");
treeRoot = uiCreateTree(root, treeNode1, node1, treeNode1);
uiDumpTree(treeRoot, %F);
uiDumpTree(treeRoot, %T);


// Errors messages
// ---------------
// Undefined input
refMsg = msprintf(_("Undefined variable: %s\n"), "myTree");
assert_checkerror("uiDumpTree(myTree)", refMsg);
assert_checkerror("uiDumpTree(myTree, %F)", refMsg);
assert_checkerror("uiDumpTree(myTree, %T)", refMsg);
assert_checkerror("uiDumpTree(myTree, 123)", refMsg);

// Wrong number of input arguments
refMsg = msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"), "uiDumpTree",1,2);
assert_checkerror("uiDumpTree()", refMsg);

refMsg = msprintf(_("Wrong number of input arguments.\n"));
assert_checkerror("uiDumpTree(123, 123, 123)", refMsg);

// Wrong type of input argument
refMsg = msprintf(_("%s: Wrong type for input argument #%d: uitree expected.\n"), "uiDumpTree",1);
assert_checkerror("uiDumpTree(123)", refMsg);
assert_checkerror("uiDumpTree(123, 123)", refMsg);
assert_checkerror("uiDumpTree(123, %F)", refMsg);
assert_checkerror("uiDumpTree(123, %T)", refMsg);

refMsg = msprintf(_("%s: Wrong type for input argument #%d: Boolean expected.\n"), "uiDumpTree",2);
assert_checkerror("uiDumpTree(treeRoot,123)", refMsg);
