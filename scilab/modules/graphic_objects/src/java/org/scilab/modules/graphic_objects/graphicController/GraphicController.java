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

package org.scilab.modules.graphic_objects.graphicController;

import java.awt.GraphicsEnvironment;
import java.util.Collections;
import java.util.ConcurrentModificationException;
import java.util.HashSet;
import java.util.Set;
import java.util.Vector;

import org.scilab.modules.graphic_objects.graphicModel.GraphicModel;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.polyline.Polyline;
import org.scilab.modules.graphic_objects.graphicView.FlattenTreeView;
import org.scilab.modules.graphic_objects.graphicView.GedTreeView;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;
import org.scilab.modules.graphic_objects.graphicView.GuiLogView;
import org.scilab.modules.graphic_objects.graphicView.LogView;

/**
 * GraphicController class
 * @author Bruno JOFRET
 */
public class GraphicController {

    private static boolean MVCViewEnable = false;
    private static boolean debugEnable = false;
    private static boolean infoEnable = false;
    private static Integer lastId = 0;

    private static void INFO(String message) {
        if (infoEnable == true) {
            System.err.println("[CONTROLLER - INFO] : " + message);
        }
    }

    private static void DEBUG(String message) {
        if (debugEnable == true) {
            System.err.println("[CONTROLLER - DEBUG] : " + message);
        }
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
        if (!GraphicsEnvironment.isHeadless() && MVCViewEnable) {
            register(GuiLogView.createGuiLogView());
            register(GedTreeView.create());
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
        INFO("Register view : " + view.toString());
        allViews.add(view);
    }

    /**
     * Unregister a view.
     * @param view The view to unregister.
     */
    public void unregister(GraphicView view) {
        INFO("Unregister view : " + view.toString());
        allViews.remove(view);
    }

    /**
     * Creates a UID
     * @return the created UID
     */
    public Integer createUID() {
        return ++lastId;
    }

    /**
     * Returns the object associated to an id
     * @param id the object id
     * @return the object
     */
    public GraphicObject getObjectFromId(Integer id) {
        return GraphicModel.getModel().getObjectFromId(id);
    }

    /**
     * Fast property set method
     * @param id the object id
     * @param prop the property name
     * @param value the property value
     * @return true if the property has been set, false otherwise
     */
    public boolean setProperty(Integer id, int prop, Object value) {
        try {
            switch (GraphicModel.getModel().setProperty(id, prop, value)) {
                case Success : // BroadCast Message + return true
                    objectUpdate(id, prop);
                    return true;
                case NoChange : // Do not broadcast message
                    return true;
                case Fail :
                    return false;
            }
            return false;
        } catch (Exception e) {
            DEBUG("====== Exception caught ======");
            DEBUG("setProperty : " + id + " " + prop);
            e.printStackTrace();
            DEBUG("====== Exception caught ======");
            return false;
        }
    }

    /**
     * Fast property get method
     * @param id the object id
     * @param prop the property name
     * @return the property value
     */
    public Object getProperty(Integer id, int prop) {
        try {
            return GraphicModel.getModel().getProperty(id, prop);
        } catch (Exception e) {
            DEBUG("====== Exception caught ======");
            DEBUG("getProperty : [" + id + "] " + prop);
            e.printStackTrace();
            DEBUG("====== Exception caught ======");
            return null;
        }
    }

    /**
     * Returns a null property
     * @param id the object id
     * @param prop the property name
     * @return the null property
     */
    public Object getNullProperty(Integer id, String prop) {
        return GraphicModel.getModel().getNullProperty(id, prop);
    }

    /**
     * Asks the model to create a new object
     * @param type the object type
     * @return the created object's id
     */
    public Integer askObject(Type type, boolean broadcastMessage) {

        try {
            Integer id = createUID();
            GraphicModel.getModel().createObject(id, type);
            if (broadcastMessage) {
                objectCreated(id);
            }

            return id;
        } catch (Exception e) {
            DEBUG("====== Exception caught ======");
            DEBUG(" askObject type = " + type.name());
            e.printStackTrace();
            DEBUG("====== Exception caught ======");
            return 0;
        }

    }

    public Integer cloneObject(Integer id, boolean broadcastMessage) {
        try {
            Integer newId = createUID();
            GraphicModel.getModel().cloneObject(id, newId);
            if (broadcastMessage) {
                objectCreated(newId);
            }

            return newId;
        } catch (Exception e) {
            DEBUG("====== Exception caught ======");
            DEBUG("cloneObject id = " + id);
            e.printStackTrace();
            DEBUG("====== Exception caught ======");
            return 0;
        }
    }

    /**
     * Ask the model to clone an object
     * @param id : the ID of the object to clone.
     * @return the id of the clone.
     */
    public Integer cloneObject(Integer id) {
        return cloneObject(id, true);
    }

    public Integer askObject(Type type) {
        return askObject(type, true);
    }

    /**
     * Deletes an object
     * @param id the deleted object's id
     */
    public void deleteObject(Integer id) {
        try {
            objectDeleted(id);
            GraphicModel.getModel().deleteObject(id);
        } catch (Exception e) {
            DEBUG("====== Exception caught ======");
            DEBUG("deleteObject id = " + id);
            e.printStackTrace();
            DEBUG("====== Exception caught ======");
        }
    }

    /**
     * Notifies the existing views that an object has been created
     * @param id the created object's id
     */
    public void objectCreated(final Integer id) {
        //INFO("### Create object : "+id);
        //INFO("### type is : " + getProperty(id, GraphicObjectProperties.__GO_TYPE__));
        Vector<Runnable> broadCastVector = new Vector<Runnable>();

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
    public void objectUpdate(final Integer id, final int prop) {
        //INFO("### Update object : "+id);
        //INFO("### type is : " + getProperty(id, GraphicObjectProperties.__GO_TYPE__));
        //INFO("### prop is : " + prop);

        Vector<Runnable> broadCastVector = new Vector<Runnable>();
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
    public void objectDeleted(final Integer id) {
        //INFO("### Delete object : "+id);
        //INFO("### type is : " + getProperty(id, GraphicObjectProperties.__GO_TYPE__));
        Vector<Runnable> broadCastVector = new Vector<Runnable>();

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
    public void setGraphicObjectRelationship(Integer parentId, Integer childId) {
        /*
         * All the parent and children get/set calls must be performed first,
         * and only then the corresponding object updates.
         */
        Object oldParent = getProperty(childId, GraphicObjectProperties.__GO_PARENT__);

        if (oldParent != null && oldParent instanceof Integer) {
            Integer oldParentId = (Integer) oldParent;

            if (oldParentId == parentId) {
                return;
            }

            if (oldParentId != 0) {
                getObjectFromId(oldParentId).removeChild(childId);
            }
        }

        /* Insertion occurs at the head of the children list. */
        if (parentId != null && parentId != 0) {
            GraphicObject go = getObjectFromId(parentId);
            if (go != null) {
                go.addChild(childId);
            }
        }

        setProperty(childId, GraphicObjectProperties.__GO_PARENT__, parentId);

        /* Object updates can now be performed. */
        if (oldParent != null && oldParent instanceof Integer && ((Integer)oldParent) != 0) {
            objectUpdate((Integer)oldParent, GraphicObjectProperties.__GO_CHILDREN__);
        }

        if (parentId != null && parentId != 0) {
            objectUpdate(parentId, GraphicObjectProperties.__GO_CHILDREN__);
        }

        // Useless (already done in setProperty(childId, GraphicObjectProperties.__GO_PARENT__, parentId);)
        //objectUpdate(childId, GraphicObjectProperties.__GO_PARENT__);
    }

    /**
     * Remove relationship between given object and is parent.
     * Then delete it.
     * TODO : Manage children of deleted object.
     * @param id deleted object identifier.
     */
    public void removeRelationShipAndDelete(Integer id) {

        final GraphicObject killMe = getObjectFromId(id);
        // assert that the object has not been deleted yet
        if (killMe == null) {
            return;
        }

        //Datatips are not listed as "children", then they must be deleted separately
        if (killMe.getType() == GraphicObjectProperties.__GO_POLYLINE__) {
            Integer[] datatips = ((Polyline)killMe).getDatatips();
            for (Integer datatip : datatips) {
                deleteObject(datatip);
            }
        }

        Integer parentUID = killMe.getParent();


        /* Remove object from Parent's Children list */
        if (parentUID != null && parentUID != 0) {
            getObjectFromId(parentUID).removeChild(id);
            //setProperty(id, GraphicObjectProperties.__GO_PARENT__, "");

            objectUpdate(parentUID, GraphicObjectProperties.__GO_CHILDREN__);
            objectUpdate(parentUID, GraphicObjectProperties.__GO_DATATIPS__);
            //objectUpdate(id, GraphicObjectProperties.__GO_PARENT__);
        }

        killMe.setValid(false);
        recursiveDeleteChildren(killMe);

        deleteObject(id);
    }

    private void recursiveDeleteChildren(GraphicObject killMe) {
        Integer children[] = killMe.getChildren();

        for (int i = 0 ; i < children.length ; ++i) {
            GraphicObject killMeThisChild = getObjectFromId(children[i]);
            killMeThisChild.setValid(false);
            recursiveDeleteChildren(killMeThisChild);
            deleteObject(children[i]);
        }
    }
}
