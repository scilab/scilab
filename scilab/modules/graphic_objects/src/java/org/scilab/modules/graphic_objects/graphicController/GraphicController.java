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
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

import org.scilab.modules.graphic_objects.graphicModel.GraphicModel;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
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
     * @param view
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
    	if (GraphicModel.getModel().setProperty(id, prop, value) == true) {
    	    objectUpdate(id);
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
     * Notifies the existing views that an object has been created
     * @param id the created object's id
     */
    public void objectCreated(String id) {
        Iterator<GraphicView> itr = allViews.iterator();
        while (itr.hasNext()) {
            itr.next().createObject(id);
        }
    }

    /**
     * Notified the existing views that an object has been updated 
     * @param id the updated object's id
     */
    public void objectUpdate(String id) {
        Iterator<GraphicView> itr = allViews.iterator();
        while (itr.hasNext()) {
            itr.next().updateObject(id);
        }
    }


    /**
     * Deletes an object
     * @param id the deleted object's id
     */
    public void deleteObject(String id) {
        GraphicModel.getModel().deleteObject(id);
        Iterator<GraphicView> itr = allViews.iterator();
        while (itr.hasNext()) {
            itr.next().deleteObject(id);
        }
    }
}
