/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
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
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;
import org.scilab.modules.graphic_objects.uicontrol.frame.border.FrameBorder;

public abstract class TreeView implements GraphicView, TreeSelectionListener {

    protected Map<Integer, DefaultMutableTreeNode> allObjects = new ConcurrentHashMap<Integer, DefaultMutableTreeNode>();
    protected DefaultTreeModel topModel = null;
    protected DefaultMutableTreeNode top = new DefaultMutableTreeNode("Graphic Objects");
    private JTree tree;
    private JEditorPane htmlDetailPane;
    private JFrame frame;

    public void show() {
        frame.setVisible(true);
    }

    public void hide() {
        frame.setVisible(false);
    }

    protected TreeView() {
        frame = new JFrame("TreeView");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        JPanel panel = new JPanel(new GridLayout(1, 0));

        topModel = new DefaultTreeModel(top);
        tree = new JTree(topModel);
        tree.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);
        tree.addTreeSelectionListener(this);
        tree.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);


        //Listen for when the selection changes.
        tree.addTreeSelectionListener(this);


        Dimension minDims = new Dimension(400, 300);
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

    public abstract void createObject(Integer id);
    public abstract void deleteObject(Integer id) ;
    public abstract void updateObject(Integer id, int property);

    /*
     * Inner class to wrap GraphicObject in a Node.
     */
    protected class GraphicObjectNode {
        private GraphicObject graphicObject;

        public GraphicObjectNode(GraphicObject object) {
            this.graphicObject = object;
        }

        public String toString() {
            return typeToString(graphicObject.getType()) + " : @" + graphicObject.getIdentifier().toString();
        }

        public String toHTML() {
            StringBuffer strBuff = new StringBuffer();
            strBuff.append("<html><body>");
            strBuff.append("<h1>Graphic Object of type: " + typeToString(graphicObject.getType()) + "</h1>");
            strBuff.append("<h2> Id : " + graphicObject.getIdentifier() + "</h2>");
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
                        strBuff.append(strings[i] + ",");
                    }
                    strBuff.append("}");
                } else if (values instanceof Integer[]) {
                    Integer[] ints = (Integer []) values;
                    strBuff.append("{");
                    for (int i = 0; i < ints.length ; ++i) {
                        strBuff.append(ints[i] + ",");
                    }
                    strBuff.append("}");
                } else if (values instanceof Double[]) {
                    Double[] vals = (Double []) values;
                    strBuff.append("{");
                    for (int i = 0; i < vals.length ; ++i) {
                        strBuff.append(vals[i] + ",");
                    }
                    strBuff.append("}");
                } else {
                    if (values != null) {
                        if (values.getClass().getPackage().getName().contains("scilab")) {
                            strBuff.append(values.getClass().getCanonicalName());
                        } else {
                            strBuff.append(values.toString());
                        }
                    } else {
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

            // UiControl
            if (graphicObject instanceof Uicontrol) {
                Uicontrol uiControl = (Uicontrol) graphicObject;
                strBuff.append("<tr><td>STYLE</td><td>" + typeToString(uiControl.getStyle()) + "</td>");
                strBuff.append("<tr><td>LAYOUT</td><td>" + uiControl.getLayoutAsEnum() + "</td>");
                strBuff.append("<tr><td>GROUP NAME</td><td>" + uiControl.getGroupName() + "</td>");
                strBuff.append("<tr><td>FRAME_BORDER</td><td>" + uiControl.getFrameBorder() + "</td>");
            }

            if (graphicObject instanceof FrameBorder) {
                FrameBorder frameBorder = (FrameBorder) graphicObject;
                strBuff.append("<tr><td>STYLE</td><td>" + frameBorder.getStyleAsEnum().toString() + "</td>");
            }

            strBuff.append("</table>");
            strBuff.append("</body></html>");
            return strBuff.toString();
        }

        private String typeToString(Integer key) {
            return LogView.pierreDeRosette.get(key);
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
