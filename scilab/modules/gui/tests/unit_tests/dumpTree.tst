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
if( execstr("dumpTree()","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(123)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(123,123)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(123, 123, 123)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(123,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(123,%T)","errcatch") == 0 ) then pause,end

// node creation
leaf11 = createNode('leaf 1.1', 'iconLeaf1.1', 'callbackLeaf1.1')
leaf12 = createNode('leaf 1.2', 'iconLeaf1.2', 'callbackLeaf1.2')
leaf31 = createNode('leaf 3.1', 'iconLeaf3.1', 'callbackLeaf3.1')
leaf32 = createNode('leaf 3.2', 'iconLeaf3.2', 'callbackLeaf3.2')
node1 = createNode('Node 1', 'iconNode1', 'callbackNode1')
node2 = createNode('Node 2', 'iconNode2', 'callbackNode2')
node3 = createNode('Node 3', 'iconNode3', 'callbackNode3')
root = createNode('Root', 'iconRoot', 'callbackRoot')

// right inputs
dumpTree(leaf11)
dumpTree(leaf11,%F)
dumpTree(leaf11,%T)

// wrong inputs
if( execstr("dumpTree(leaf11,123)","errcatch") == 0 ) then pause,end


// node creation with errors
if( execstr("myErrorNode1 = createNode()","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myErrorNode1)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myErrorNode1,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myErrorNode1,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myErrorNode1,123)","errcatch") == 0 ) then pause,end

if( execstr("myErrorNode2 = createNode(123)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myErrorNode2)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myErrorNode2,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myErrorNode2,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myErrorNode2,123)","errcatch") == 0 ) then pause,end

if( execstr("myErrorNode3 = createNode(123, ''icon of My Node'')","errcatch") == 0 ) then pause,end
if( execstr("myErrorNode4 = createNode(''My Node'', 123)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myErrorNode3)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myErrorNode3,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myErrorNode3,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myErrorNode3,123)","errcatch") == 0 ) then pause,end

if( execstr("myErrorNode5 = createNode(123, ''icon of My Node'', ''callback of My Node'')","errcatch") == 0 ) then pause,end
if( execstr("myErrorNode6 = createNode(''My Node'', 123, ''callback of My Node'')","errcatch") == 0 ) then pause,end
if( execstr("myErrorNode7 = createNode(''My Node'', ''icon of My Node'', 123)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myErrorNode5)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myErrorNode5,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myErrorNode5,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myErrorNode5,123)","errcatch") == 0 ) then pause,end

if( execstr("myErrorNode8 = createNode(''My Node'', ''icon of My Node'', ''callback of My Node'', ''test'')","errcatch") == 0 ) then pause,end
if( execstr("myErrorNode9 = createNode(''My Node'', ''icon of My Node'', ''callback of My Node'', 123)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myErrorNode8)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myErrorNode8,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myErrorNode8,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myErrorNode8,123)","errcatch") == 0 ) then pause,end


// tree creation
treeOnlyNode1 = createTree(node1)

// right inputs
dumpTree(treeOnlyNode1)
dumpTree(treeOnlyNode1,%F)
dumpTree(treeOnlyNode1,%T)

// wrong inputs
if( execstr("dumpTree(treeOnlyNode1,123)","errcatch") == 0 ) then pause,end

treeNode1 = createTree(node1, leaf11,leaf12)
treeNode3 = createTree(node3, leaf31,leaf32)
treeRoot = createTree(root, node1,node2,node3)

// right inputs
dumpTree(treeRoot)
dumpTree(treeRoot,%F)
dumpTree(treeRoot,%T)

// wrong inputs
if( execstr("dumpTree(treeRoot,123)","errcatch") == 0 ) then pause,end

// tree creation with wrong inputs
if( execstr("myTree = createTree()","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(123)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(123, node1,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(root, 123,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(root, node1,123,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(root, node1,node2,123)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

// tree creation with invalid nodes
if( execstr("myTree = createTree(myErrorNode1)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(myErrorNode2)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(myErrorNode3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(myErrorNode4)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(myErrorNode5)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(myErrorNode6)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(myErrorNode7)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(myErrorNode8)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(myErrorNode9)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(myErrorNode1, node1,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(root, myErrorNode1,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(root, node1,myErrorNode1,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(root, node1,node2,myErrorNode1)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end


if( execstr("myTree = createTree(myErrorNode2, node1,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(root, myErrorNode2,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(root, node1,myErrorNode2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(root, node1,node2,myErrorNode2)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end


if( execstr("myTree = createTree(myErrorNode3, node1,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(root, myErrorNode3,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(root, node1,myErrorNode3,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(root, node1,node2,myErrorNode3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end


if( execstr("myTree = createTree(myErrorNode5, node1,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(root, myErrorNode5,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(root, node1,myErrorNode5,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(root, node1,node2,myErrorNode5)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end


if( execstr("myTree = createTree(myErrorNode8, node1,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(root, myErrorNode8,node2,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(root, node1,myErrorNode8,node3)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end

if( execstr("myTree = createTree(root, node1,node2,myErrorNode8)","errcatch") == 0 ) then pause,end

// right inputs
if( execstr("dumpTree(myTree)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%F)","errcatch") == 0 ) then pause,end
if( execstr("dumpTree(myTree,%T)","errcatch") == 0 ) then pause,end

// wrong inputs
if( execstr("dumpTree(myTree,123)","errcatch") == 0 ) then pause,end




