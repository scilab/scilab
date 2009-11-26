// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH TREE -->

lines(0)

// test to check the dumped tree

// tree dumping error test
if( execstr("uiDumpTree()","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(123)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(123,123)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(123, 123, 123)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(123,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(123,%T)","errcatch") == 0 ) then pause,end

// node creation
leaf11 = uiCreateNode('leaf 1.1', 'iconLeaf1.1', 'callbackLeaf1.1')
leaf12 = uiCreateNode('leaf 1.2', 'iconLeaf1.2', 'callbackLeaf1.2')
leaf31 = uiCreateNode('leaf 3.1', 'iconLeaf3.1', 'callbackLeaf3.1')
leaf32 = uiCreateNode('leaf 3.2', 'iconLeaf3.2', 'callbackLeaf3.2')
node1 = uiCreateNode('Node 1', 'iconNode1', 'callbackNode1')
node2 = uiCreateNode('Node 2', 'iconNode2', 'callbackNode2')
node3 = uiCreateNode('Node 3', 'iconNode3', 'callbackNode3')
root = uiCreateNode('Root', 'iconRoot', 'callbackRoot')

// right inputs
uiDumpTree(leaf11)
uiDumpTree(leaf11,%F)
uiDumpTree(leaf11,%T)

// wrong inputs
if( execstr("uiDumpTree(leaf11,123)","errcatch") == 0 ) then pause,end


// node creation with errors
if( execstr("myErrorNode1 = uiCreateNode()","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myErrorNode1)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myErrorNode1,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myErrorNode1,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myErrorNode1,123)","errcatch") == 0 ) then pause,end

if( execstr("myErrorNode2 = uiCreateNode(123)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myErrorNode2)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myErrorNode2,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myErrorNode2,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myErrorNode2,123)","errcatch") == 0 ) then pause,end

if( execstr("myErrorNode3 = uiCreateNode(123, ''icon of My Node'')","errcatch") == 0 ) then pause,end
if( execstr("myErrorNode4 = uiCreateNode(''My Node'', 123)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myErrorNode3)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myErrorNode3,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myErrorNode3,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myErrorNode3,123)","errcatch") == 0 ) then pause,end

if( execstr("myErrorNode5 = uiCreateNode(123, ''icon of My Node'', ''callback of My Node'')","errcatch") == 0 ) then pause,end
if( execstr("myErrorNode6 = uiCreateNode(''My Node'', 123, ''callback of My Node'')","errcatch") == 0 ) then pause,end
if( execstr("myErrorNode7 = uiCreateNode(''My Node'', ''icon of My Node'', 123)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myErrorNode5)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myErrorNode5,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myErrorNode5,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myErrorNode5,123)","errcatch") == 0 ) then pause,end

if( execstr("myErrorNode8 = uiCreateNode(''My Node'', ''icon of My Node'', ''callback of My Node'', ''test'')","errcatch") == 0 ) then pause,end
if( execstr("myErrorNode9 = uiCreateNode(''My Node'', ''icon of My Node'', ''callback of My Node'', 123)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myErrorNode8)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myErrorNode8,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myErrorNode8,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myErrorNode8,123)","errcatch") == 0 ) then pause,end


// tree creation
treeOnlyNode1 = uiCreateTree(node1)

// right inputs
uiDumpTree(treeOnlyNode1)
uiDumpTree(treeOnlyNode1,%F)
uiDumpTree(treeOnlyNode1,%T)

// wrong inputs
if( execstr("uiDumpTree(treeOnlyNode1,123)","errcatch") == 0 ) then pause,end

treeNode1 = uiCreateTree(node1, leaf11,leaf12)
treeNode3 = uiCreateTree(node3, leaf31,leaf32)
treeRoot = uiCreateTree(root, node1,node2,node3)

// right inputs
uiDumpTree(treeRoot)
uiDumpTree(treeRoot,%F)
uiDumpTree(treeRoot,%T)

// wrong inputs
if( execstr("uiDumpTree(treeRoot,123)","errcatch") == 0 ) then pause,end

// tree creation with wrong inputs
if( execstr("myTree = uiCreateTree()","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(123)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(123, node1,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(root, 123,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(root, node1,123,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(root, node1,node2,123)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

// tree creation with invalid nodes
if( execstr("myTree = uiCreateTree(myErrorNode1)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(myErrorNode2)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(myErrorNode3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(myErrorNode4)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(myErrorNode5)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(myErrorNode6)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(myErrorNode7)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(myErrorNode8)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(myErrorNode9)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(myErrorNode1, node1,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(root, myErrorNode1,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(root, node1,myErrorNode1,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(root, node1,node2,myErrorNode1)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end


if( execstr("myTree = uiCreateTree(myErrorNode2, node1,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(root, myErrorNode2,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(root, node1,myErrorNode2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(root, node1,node2,myErrorNode2)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end


if( execstr("myTree = uiCreateTree(myErrorNode3, node1,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(root, myErrorNode3,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(root, node1,myErrorNode3,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(root, node1,node2,myErrorNode3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end


if( execstr("myTree = uiCreateTree(myErrorNode5, node1,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(root, myErrorNode5,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(root, node1,myErrorNode5,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(root, node1,node2,myErrorNode5)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end


if( execstr("myTree = uiCreateTree(myErrorNode8, node1,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(root, myErrorNode8,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(root, node1,myErrorNode8,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = uiCreateTree(root, node1,node2,myErrorNode8)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("uiDumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("uiDumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("uiDumpTree(myTree,123)","errcatch") == 0 ) then pause,end




