/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.gui.tree;

import java.util.StringTokenizer;
import java.util.Hashtable;

/**
 * Display a graphical tree
 *
 */
public class ScilabDisplayTree {

    private static String[][] myShapedTree;


    /**
     * PRIVATE constructor
     */
    private ScilabDisplayTree() { };

    /**
     * scilabDisplayTree called with 1 arg (treeData)
     * @param treeData structure of the trees
     */
    public static void scilabDisplayTree(String[] treeData) {

        // Get depth and parent node for each node
        myShapedTree = treeShaping(treeData);

        // Display the GUI tree
        createGraphicTree();
    }

    public static Tree uicontrolScilabDisplayTree(String[] treeData) {
        // Get depth and parent node for each node
        myShapedTree = treeShaping(treeData);
        // Create the ScilabTree
        ScilabTree t = createTree(myShapedTree);

        return t;
    }

    /**
     * Get depth and parent node for each node
     * @param treeData data of a tree
     * @return treeShape
     */
    public static String[][] treeShaping(String[] treeData) {

        String[][] treeShape = new String[treeData.length][3];
        StringTokenizer st;
        int depth = 0;
        int point_index = 0;

        for (int i = 0; i < treeData.length; i++) {

            // Get original structure
            treeShape[i][0] = treeData[i];

            // Get depth & parent node
            if (i % 4 == 0) {
                // Depth
                st = new StringTokenizer(treeData[i], ".");
                depth = st.countTokens() - 1;
                treeShape[i][1] = "" + depth;

                // Parent Node
                if (depth == 0) {
                    treeShape[i][2] = "root";
                } else {
                    point_index = treeData[i].lastIndexOf(".");
                    treeShape[i][2] = treeData[i].substring(0, point_index);
                }
            }
        }
        return treeShape;
    }

    /**
     * Display the GUI tree
     */
    public static void createGraphicTree() {

        // Create the ScilabTree
        ScilabTree t = createTree(myShapedTree);

        // Display the tree
        t.showTree();
    }

    /**
     * Get tree's max depth
     * @param myShapedTree shaped tree
     * @return an integer
     */
    public static int getTreeDepth(String[][] myShapedTree) {
        int depth = 0;
        for (int i = 0; i < myShapedTree.length; i++) {

            if (i % 4 == 0 && i > 0) {
                if (Integer.parseInt(myShapedTree[i][1]) > depth) {
                    depth = Integer.parseInt(myShapedTree[i][1]);
                }
            }
        }
        return depth;
    }

    /**
     * Create the tree structure
     * @param myShapedTree shaped tree
     * @return ScilabTree
     */
    public static ScilabTree createTree(String[][] myShapedTree) {

        ScilabTree tree = null;
        Hashtable<String, ScilabTree> treeVsPosition = new Hashtable<String, ScilabTree>();

        tree = new ScilabTree(myShapedTree[1][0], myShapedTree[2][0], myShapedTree[3][0]);
        treeVsPosition.put(myShapedTree[0][0], tree);
        for (int i = 4; i < myShapedTree.length; i = i + 4) {
            tree = new ScilabTree(myShapedTree[i + 1][0], myShapedTree[i + 2][0], myShapedTree[i + 3][0]);
            treeVsPosition.put(myShapedTree[i][0], tree);
            treeVsPosition.get(myShapedTree[i][2]).addChild(tree);
        }
        return treeVsPosition.get(myShapedTree[0][0]);
    }

}
