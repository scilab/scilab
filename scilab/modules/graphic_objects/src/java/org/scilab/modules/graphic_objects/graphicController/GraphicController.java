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

package org.scilab.modules.graphic_objects.graphicController;

import java.rmi.server.UID;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.scilab.modules.graphic_objects.graphicModel.GraphicModel;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;
import org.scilab.modules.graphic_objects.graphicView.LogView;


/**
 * GraphicController class
 * @author Bruno JOFRET
 */
public class GraphicController {


    private Set<GraphicView> allViews = new HashSet<GraphicView>();

    private static GraphicController me = null;

    /**
     * Default constructor
     */
    private GraphicController() {
        /**
         * Debug Only !
         */
        this.register(LogView.createLogView());
    }

    /**
     * Returns the controller
     * @return the controller
     */
    public static GraphicController getController() {
	if (me == null) {
	    me = new GraphicController();
	}

	return me;
    }

    /**
     * Register a view that will receive notification
     * of any model changes.
     * @param view The view to register.
     */
    public void register(GraphicView view) {
        allViews.add(view);
    }

    /**
     * Creates a UID
     * @return the created UID
     */
    public UID createUID() {
    	return new UID();
    }

    /**
     * Returns the object associated to an id
     * @param id the object id
     * @return the object
     */
    public Object getObjectFromId(String id) {
    	Object object;
    	object = GraphicModel.getModel().getObjectFromId(id);

    	return object;
    }

    /**
     * Fast property set method
     * @param id the object id
     * @param prop the property name
     * @param value the property value
     * @return true if the property has been set, false otherwise
     */
    public boolean setProperty(String id, String prop, Object value) {
    	if (GraphicModel.getModel().setProperty(id, prop, value)) {
    	    objectUpdate(id, prop);
    	    return true;
    	}
    	return false;
    }

    /**
     * Fast property get method
     * @param id the object id
     * @param prop the property name
     * @return the property value
     */
    public Object getProperty(String id, String prop) {
    	return GraphicModel.getModel().getProperty(id, prop);
    }

    /**
     * Returns a null property
     * @param id the object id
     * @param prop the property name
     * @return the null property
     */
    public Object getNullProperty(String id, String prop) {
    	return GraphicModel.getModel().getNullProperty(id, prop);
    }

	/**
	 * Asks the model to create a new object
	 * @param type the object type
	 * @return the created object's id
	 */
    public String askObject(Type type) {
    	UID id = createUID();
    	GraphicModel.getModel().createObject(id.toString(), type);
    	objectCreated(id.toString());

    	return id.toString();
    }

    /**
     * Ask the model to clone an object
     * @param id : the ID of the object to clone.
     * @return the id of the clone.
     */
    public String cloneObject(String id) {
        UID newId = createUID();
        GraphicModel.getModel().cloneObject(id, newId.toString());
        objectCreated(newId.toString());

        return newId.toString();
    }

    /**
     * Deletes an object
     * @param id the deleted object's id
     */
    public void deleteObject(String id) {
        GraphicModel.getModel().deleteObject(id);
        objectDeleted(id);
    }

    /**
     * Notifies the existing views that an object has been created
     * @param id the created object's id
     */
    public void objectCreated(String id) {
        for (GraphicView allView : allViews) {
            allView.createObject(id);
        }
    }

    /**
     * Notified the existing views that an object has been updated
     * @param id the updated object's id
     * @param prop the property that has been updated
     */
    public void objectUpdate(String id, String prop) {
        for (GraphicView allView : allViews) {
            allView.updateObject(id, prop);
        }
    }

    /**
     * Notified the existing views that an object has been deleted
     * @param id the deleted object's id
     */
    public void objectDeleted(String id) {
        for (GraphicView allView : allViews) {
            allView.deleteObject(id);
        }
    }

    /**
     * Set relationship between two object and remove old relationship.
     * @param parentId id of the parent object.
     * @param childId id of the child object.
     */
    public void setGraphicObjectRelationship(String parentId, String childId) {
        Object oldParent = getProperty(childId, GraphicObjectProperties.__GO_PARENT__);

        if (oldParent != null && oldParent instanceof String) {
            String oldParentId = (String) oldParent;

            if (oldParentId.equals(parentId)) {
                return;
            }

            if (!oldParentId.equals("")) {
                String[] children = (String[]) GraphicController.getController().getProperty(oldParentId, GraphicObjectProperties.__GO_CHILDREN__);
                List<String> list = new ArrayList(Arrays.asList(children));
                list.remove(childId);
                setProperty(oldParentId, GraphicObjectProperties.__GO_CHILDREN__, list);
            }
        }

        if (parentId != null && !parentId.equals("")) {
            String[] children = (String[]) GraphicController.getController().getProperty(parentId, GraphicObjectProperties.__GO_CHILDREN__);
            String[] newChildren = new String[children.length + 1]; 
            System.arraycopy(children, 0, newChildren, 0, children.length);
            newChildren[children.length] = childId;
            setProperty(parentId, GraphicObjectProperties.__GO_CHILDREN__, Arrays.asList(newChildren));
        }

        setProperty(childId, GraphicObjectProperties.__GO_PARENT__, parentId);
    }
}
