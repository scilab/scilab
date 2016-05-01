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

public class FlattenTreeView extends TreeView {
    private static FlattenTreeView me;

    private FlattenTreeView() {
        super();
    }

    public static void createView() {
        if (me == null) {
            me = new FlattenTreeView();
            GraphicController.getController().register(me);
            me.show();
        }
    }

    public static void deleteView() {
        if (me != null) {
            me.hide();
            GraphicController.getController().unregister(me);
            me = null;
        }
    }

    public void createObject(Integer id) {
        try {
            GraphicObject graphiObject = GraphicController.getController().getObjectFromId(id);
            //if (!(graphiObject instanceof Uimenu)) {
            DefaultMutableTreeNode node = new DefaultMutableTreeNode(new GraphicObjectNode(graphiObject));

            allObjects.put(id, node);
            top.add(node);
            topModel.nodeStructureChanged(top);
            //}
        } catch (Exception e) {
            //e.printStackTrace();
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

    }
}
