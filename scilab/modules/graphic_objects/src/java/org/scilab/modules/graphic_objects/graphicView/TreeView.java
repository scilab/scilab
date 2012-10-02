/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.graphic_objects.graphicView;

import java.awt.Dimension;
import java.awt.GridLayout;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import javax.swing.JEditorPane;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTree;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeSelectionModel;

import org.scilab.modules.graphic_objects.contouredObject.ContouredObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;

public abstract class TreeView implements GraphicView, TreeSelectionListener{

    protected Map<String, DefaultMutableTreeNode> allObjects = new ConcurrentHashMap<String, DefaultMutableTreeNode>();
    protected DefaultTreeModel topModel = null;
    protected DefaultMutableTreeNode top = new DefaultMutableTreeNode("Graphic Objects");
    private JTree tree;
    private JEditorPane htmlDetailPane;
    private JFrame frame;

    public void show() {
        frame.setVisible(true);
    }

    protected TreeView() {
        frame = new JFrame("TreeView");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        JPanel panel = new JPanel(new GridLayout(1,0));

        topModel = new DefaultTreeModel(top);
        tree = new JTree(topModel);
        tree.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);
        tree.addTreeSelectionListener(this);
        tree.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);


        //Listen for when the selection changes.
        tree.addTreeSelectionListener(this);


        Dimension minDims = new Dimension(400,300);
        //Create the scroll pane and add the tree to it. 
        JScrollPane treeView = new JScrollPane(tree);
        treeView.setMinimumSize(minDims);

        //Create the HTML detail viewing pane.
        htmlDetailPane = new JEditorPane();
        htmlDetailPane.setEditable(false);
        htmlDetailPane.setContentType("text/html");
        JScrollPane htmlView = new JScrollPane(htmlDetailPane);

        //Add the scroll panes to a split pane.
        JSplitPane treeDetailPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT);
        //treeDetailPane.setLeftComponent(fTreeView);
        treeDetailPane.setLeftComponent(treeView);
        treeDetailPane.setRightComponent(htmlView);

        treeDetailPane.setDividerLocation(100); 
        treeDetailPane.setPreferredSize(new Dimension(1200, 600));

        //Add the split pane to this panel.
        panel.add(treeDetailPane);
        frame.add(panel);
        frame.setVisible(false);
        frame.setSize(1200, 600);
    }

    public abstract void createObject(String id);
    public abstract void deleteObject(String id) ;
    public abstract void updateObject(String id, int property);
    
    /*
     * Inner class to wrap GraphicObject in a Node.
     */
    protected class GraphicObjectNode {
        private GraphicObject graphicObject;

        public GraphicObjectNode(GraphicObject object) {
            this.graphicObject = object;
        }

        public String toString() {
            return graphicObject.getType() + " : @" + graphicObject.getIdentifier();
        }

        public String toHTML() {
            StringBuffer strBuff = new StringBuffer();
            strBuff.append("<html><body>");
            strBuff.append("<h1>Graphic Object of type: "+graphicObject.getType()+"</h1>");
            strBuff.append("<h2> Id : "+graphicObject.getIdentifier()+"</h2>");
            strBuff.append("<table border=\"1\">");
            strBuff.append("<tr><th>Property Name</th><th>Value</th></tr>");
            // Graphic Object
            for (GraphicObject.GraphicObjectPropertyType goProperty : GraphicObject.GraphicObjectPropertyType.values()) {
                strBuff.append("<tr><td>");
                strBuff.append(goProperty);
                strBuff.append("</td><td>");
                Object values = graphicObject.getProperty(goProperty);
                if (values instanceof String[]) {
                    String[] strings = (String []) values;
                    strBuff.append("{");
                    for (int i = 0; i < strings.length ; ++i) {
                        strBuff.append(strings[i]+",");
                    }
                    strBuff.append("}");
                }
                else {
                    if (values != null) {
                        strBuff.append(values.toString());
                    }
                    else
                    {
                        strBuff.append("??? NULL ???");
                    }
                }
                strBuff.append("</td></tr>");
            }

            // ContouredObject
            if (graphicObject instanceof ContouredObject) {
                for (ContouredObject.ContouredObjectPropertyType goProperty : ContouredObject.ContouredObjectPropertyType.values()) {
                    strBuff.append("<tr><td>");
                    strBuff.append(goProperty);
                    strBuff.append("</td><td>");
                    strBuff.append(graphicObject.getProperty(goProperty));
                    strBuff.append("</td></tr>");
                }
            }
            strBuff.append("</table>");
            strBuff.append("</body></html>");
            return strBuff.toString();
        }
    }
    /** Required by TreeSelectionListener interface. */
    public void valueChanged(TreeSelectionEvent e) {
        Object node = tree.getLastSelectedPathComponent();

        if (node == null || !(node instanceof DefaultMutableTreeNode)) {
            htmlDetailPane.setText("");
            return;
        }

        Object nodeInfo = ((DefaultMutableTreeNode) node).getUserObject();
        if (nodeInfo == null || !(nodeInfo instanceof GraphicObjectNode)) {
            htmlDetailPane.setText("");
            return;
        }
        GraphicObjectNode graphicObject = (GraphicObjectNode) nodeInfo;
        htmlDetailPane.setText(graphicObject.toHTML());
    }

}
