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

import java.awt.GraphicsEnvironment;
import java.rmi.server.UID;
import java.util.Collections;
import java.util.ConcurrentModificationException;
import java.util.HashSet;
import java.util.Set;
import java.util.Vector;

import org.scilab.modules.graphic_objects.graphicModel.GraphicModel;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;
import org.scilab.modules.graphic_objects.graphicView.GuiLogView;
import org.scilab.modules.graphic_objects.graphicView.TreeView;

/**
 * GraphicController class
 * @author Bruno JOFRET
 */
public class GraphicController {

    private static void DEBUG(String message)
    {
        //System.err.println("[CONTROLLER] : "+message);
    }

    /**
     * Set of all views attached to this controller.
     */
    private volatile static Set<GraphicView> allViews =  Collections.synchronizedSet(new HashSet<GraphicView>());

    /**
     * Graphic controller singleton.
     */
    private static GraphicController me = null;

    /**
     * Default constructor
     */
    private GraphicController() {
        if (!GraphicsEnvironment.isHeadless()) {
            register(GuiLogView.createGuiLogView());
            register(TreeView.createTreeView());
        }
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
        DEBUG("Register view : " + view.toString());
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
    public GraphicObject getObjectFromId(String id) {
        return GraphicModel.getModel().getObjectFromId(id);
    }

    /**
     * Fast property set method
     * @param id the object id
     * @param prop the property name
     * @param value the property value
     * @return true if the property has been set, false otherwise
     */
    public boolean setProperty(String id, String prop, Object value) {
        try {
            if (GraphicModel.getModel().setProperty(id, prop, value)) {
                objectUpdate(id, prop);
                return true;
            }
            return false;
        }
        catch (Exception e) {
            System.err.println("====== Exception caught ======");
            DEBUG("setProperty : " + id + " " + prop);
            e.printStackTrace();
            System.err.println("====== Exception caught ======");
            return false;
        }
    }

    /**
     * Fast property get method
     * @param id the object id
     * @param prop the property name
     * @return the property value
     */
    public Object getProperty(String id, String prop) {
        try {
            return GraphicModel.getModel().getProperty(id, prop);
        }
        catch (Exception e) {
            System.err.println("====== Exception caught ======");
            DEBUG("getProperty : [" + id + "] " + prop);
            e.printStackTrace();
            System.err.println("====== Exception caught ======");
            return null;
        }
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
        try {
            UID id = createUID();
            GraphicModel.getModel().createObject(id.toString(), type);
            objectCreated(id.toString());

            return id.toString();
        }
        catch (Exception e) {
            System.err.println("====== Exception caught ======");
            e.printStackTrace();
            System.err.println("====== Exception caught ======");
            return "";
        }

    }

    /**
     * Ask the model to clone an object
     * @param id : the ID of the object to clone.
     * @return the id of the clone.
     */
    public String cloneObject(String id) {
        try {
            UID newId = createUID();
            GraphicModel.getModel().cloneObject(id, newId.toString());
            objectCreated(newId.toString());

            return newId.toString();
        }
        catch (Exception e) {
            System.err.println("====== Exception caught ======");
            e.printStackTrace();
            System.err.println("====== Exception caught ======");
            return "";
        }
    }

    /**
     * Deletes an object
     * @param id the deleted object's id
     */
    public void deleteObject(String id) {
        try {
            objectDeleted(id);
            GraphicModel.getModel().deleteObject(id);
        }
        catch (Exception e) {
            System.err.println("====== Exception caught ======");
            e.printStackTrace();
            System.err.println("====== Exception caught ======");
        }
    }

    /**
     * Notifies the existing views that an object has been created
     * @param id the created object's id
     */
    public void objectCreated(final String id) {
        DEBUG("### Create object : "+id);
        DEBUG("### type is : " + getProperty(id, "Type"));
        Vector<Runnable> broadCastVector= new Vector<Runnable>();

        try {
            for (final GraphicView view : allViews) {
                broadCastVector.add(new Runnable() {
                    public void run() {
                        view.createObject(id);
                    }
                });
            }
            for (final Runnable runMe : broadCastVector) {
                runMe.run();
            }
        } catch (ConcurrentModificationException e) {
            e.printStackTrace();
        }
    }

    /**
     * Notified the existing views that an object has been updated
     * @param id the updated object's id
     * @param prop the property that has been updated
     */
    public void objectUpdate(final String id, final String prop) {
        DEBUG("### Update object : "+id);
        DEBUG("### type is : " + getProperty(id, "Type"));
        DEBUG("### prop is : " + prop);

        Vector<Runnable> broadCastVector= new Vector<Runnable>();
        try {
            for (final GraphicView view : allViews) {
                broadCastVector.add(new Runnable() {
                    public void run() {
                        view.updateObject(id, prop);
                    }
                });
            }
            for (final Runnable runMe : broadCastVector) {
                runMe.run();
            }
        } catch (ConcurrentModificationException e) {
            e.printStackTrace();
        } 
    }

    /**
     * Notified the existing views that an object has been deleted
     * @param id the deleted object's id
     */
    public void objectDeleted(final String id) {
        DEBUG("### Delete object : "+id);
        DEBUG("### type is : " + getProperty(id, "Type"));
        Vector<Runnable> broadCastVector= new Vector<Runnable>();

        try {
            for (final GraphicView view : allViews) {
                broadCastVector.add(new Runnable() {
                    public void run() {
                        view.deleteObject(id);
                    }
                });
            }
            for (final Runnable runMe : broadCastVector) {
                runMe.run();
            }
        } catch (ConcurrentModificationException e) {
            e.printStackTrace();
        }
    }

    /**
     * Set relationship between two object and remove old relationship.
     * @param parentId id of the parent object.
     * @param childId id of the child object.
     */
    public void setGraphicObjectRelationship(String parentId, String childId) {
        /*
         * All the parent and children get/set calls must be performed first,
         * and only then the corresponding object updates.
         */
        Object oldParent = getProperty(childId, GraphicObjectProperties.__GO_PARENT__);

        if (oldParent != null && oldParent instanceof String) {
            String oldParentId = (String) oldParent;

            if (oldParentId.equals(parentId)) {
                return;
            }

            if (!oldParentId.equals("")) {
                getObjectFromId(oldParentId).removeChild(childId);
            }
        }

        /* Insertion occurs at the head of the children list. */
        if (parentId != null && !parentId.equals("")) {
            getObjectFromId(parentId).addChild(childId);
        }

        setProperty(childId, GraphicObjectProperties.__GO_PARENT__, parentId);

        /* Object updates can now be performed. */
        if (oldParent != null && oldParent instanceof String && !((String)oldParent).equals("")) {
            objectUpdate((String)oldParent, GraphicObjectProperties.__GO_CHILDREN__);
        }

        if (parentId != null && !parentId.equals("")) {
            objectUpdate(parentId, GraphicObjectProperties.__GO_CHILDREN__);
        }

        objectUpdate(childId, GraphicObjectProperties.__GO_PARENT__);
    }

    /**
     * Remove relationship between given object and is parent.
     * Then delete it.
     * TODO : Manage children of deleted object.
     * @param id deleted object identifier.
     */
    public void removeRelationShipAndDelete(String id) {
        Object parent = getProperty(id, GraphicObjectProperties.__GO_PARENT__);
        if ((parent != null) && (parent instanceof String)) {
            String parentId = (String) parent;
            if (!parentId.equals("")) {
                getObjectFromId(parentId).removeChild(id);
                setProperty(id, GraphicObjectProperties.__GO_PARENT__, "");

                objectUpdate(parentId, GraphicObjectProperties.__GO_CHILDREN__);
                objectUpdate(id, GraphicObjectProperties.__GO_PARENT__);
            }
        }

        deleteObject(id);
    }
}
