/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
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

import javax.swing.tree.DefaultMutableTreeNode;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

public class GedTreeView extends TreeView {

    private static GedTreeView me;

    private GedTreeView() {
        super();
    }

    public static GedTreeView create() {
        if (me == null) {
            me = new GedTreeView();
        }
        return me;
    }

    public void createObject(Integer id) {
        try {
            GraphicObject graphiObject = GraphicController.getController().getObjectFromId(id);
            DefaultMutableTreeNode node = new DefaultMutableTreeNode(new GraphicObjectNode(graphiObject));

            allObjects.put(id, node);
            top.add(node);
            topModel.nodeStructureChanged(top);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void deleteObject(Integer id) {
        try {
            DefaultMutableTreeNode objectNode = allObjects.get(id);
            if (objectNode != null) {
                DefaultMutableTreeNode parentNode = (DefaultMutableTreeNode) objectNode.getParent();
                objectNode.removeFromParent();
                allObjects.remove(id);
                topModel.nodeStructureChanged(parentNode);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void updateObject(Integer id, int property) {

        DefaultMutableTreeNode objectNode = allObjects.get(id);
        /*
         * Update parent property
         */
        if (objectNode != null && property == GraphicObjectProperties.__GO_PARENT__) {
            DefaultMutableTreeNode parentNode = (DefaultMutableTreeNode) objectNode.getParent();

            if (parentNode != null) {
                // Remove from old parent
                objectNode.removeFromParent();
                topModel.nodeStructureChanged(parentNode);

                Integer parentUID = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_PARENT__);
                // Retrieve new Parent
                DefaultMutableTreeNode newParentNode = allObjects.get(parentUID);

                // If there is no more parent, leave object at top level
                if (newParentNode == null) {
                    newParentNode = top;
                }

                newParentNode.add(objectNode);
                topModel.nodeStructureChanged(newParentNode);
            }
        }
    }

}
