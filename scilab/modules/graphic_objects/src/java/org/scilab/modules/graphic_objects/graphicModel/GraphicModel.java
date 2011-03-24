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

package org.scilab.modules.graphic_objects.graphicModel;

import java.util.HashMap;
import java.util.Map;

import org.scilab.modules.graphic_objects.arc.Arc;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axis.Axis;
import org.scilab.modules.graphic_objects.compound.Compound;
import org.scilab.modules.graphic_objects.fec.Fec;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.imageplot.Grayplot;
import org.scilab.modules.graphic_objects.imageplot.Matplot;
import org.scilab.modules.graphic_objects.label.Label;
import org.scilab.modules.graphic_objects.legend.Legend;
import org.scilab.modules.graphic_objects.polyline.Polyline;
import org.scilab.modules.graphic_objects.rectangle.Rectangle;
import org.scilab.modules.graphic_objects.surface.Fac3d;
import org.scilab.modules.graphic_objects.surface.Plot3d;
import org.scilab.modules.graphic_objects.textObject.Text;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;
import org.scilab.modules.graphic_objects.vectfield.Champ;
import org.scilab.modules.graphic_objects.vectfield.Segs;

//import demo.mvc.model.INetworkObject.Type;


/**
 * GraphicModel class
 * @author Bruno JOFRET
 */
public class GraphicModel {

    private Map<String, GraphicObject> allObjects = new HashMap<String, GraphicObject>();

    private static GraphicModel me = null;

    /**
     * Default constructor
     */
    private GraphicModel() { }

    /**
     * Returns the model
     * @return the model
     */
    public static GraphicModel getModel() {
    	if (me == null) {
    		me = new GraphicModel();
    	}

    	return me;
    }

    /**
     * @param id the id of the object to get
     * @return the object
     */
    public GraphicObject getObjectFromId(String id) {
    	return allObjects.get(id);
    }

    /**
     * Returns a null property
     * @param id the id of the object
     * @param property the property name
     * @return the property
     */
    public Object getNullProperty(String id, String property){
    	GraphicObject object = allObjects.get(id);
    	return object.getNullProperty(property);
    }

    /**
     * Fast property get
     * @param id object id
     * @param property property name
     * @return property value
     */
    public Object getProperty(String id, String property) {
    	GraphicObject object = allObjects.get(id);
    	Object propertyType = object.getPropertyFromName(property);
    	return object.getProperty(propertyType);
    }

    /**
     * Fast property set
     * @param id object id
     * @param property name
     * @param value property value
     * @return true if the property has been set, false otherwise
     */
    public boolean setProperty(String id, String property, Object value) {
    	GraphicObject object = allObjects.get(id);
    	Object propertyType = object.getPropertyFromName(property);
    	return object.setProperty(propertyType, value);
    }

    /**
     * Creates an object
     * @param id object id
     * @param type object type
     * @return the created object's id
     */
    public String createObject(String id, GraphicObject.Type type) {
    	GraphicObject object = createTypedObject(type);

    	if (object != null) {
    		allObjects.put(id, object);
    		object.setIdentifier(id);
    		
    		return id;
    	} else {
    		return null;
    	}
    }

    /**
     * Clone object
     * @param id : id of the object to clone
     * @param newId : id of the clone
     * @return newId
     */
    public String cloneObject(String id, String newId) {
        GraphicObject object = allObjects.get(id);
        GraphicObject objectClone = object.clone();
        objectClone.setIdentifier(newId);
        
        allObjects.put(newId, objectClone);
        
        return newId;
    }
    
    /**
     * Creates a typed object
     * @param type the object type
     * @return the created typed object
     */
    private GraphicObject createTypedObject(Type type) {
        try {
		switch (type) {

		case ARC:
			return new Arc();
		case AXES:
			return new Axes();
		case AXIS:
			return new Axis();
		case CHAMP:
			return new Champ();
		case COMPOUND:
			return new Compound();
		case FAC3D:
			return new Fac3d();
		case FEC:
			return new Fec();
		case FIGURE:
			return new Figure();
		case GRAYPLOT:
			return new Grayplot();
		case LABEL:
			return new Label();
		case LEGEND:
			return new Legend();
		case MATPLOT:
			return new Matplot();
		case PLOT3D:
			return new Plot3d();
		case POLYLINE:
			return new Polyline();
		case RECTANGLE:
			return new Rectangle();
		case SEGS:
			return new Segs();
		case TEXT:
			return new Text();
		case UICONTROL:
		    return new Uicontrol();
		case UNKNOWNOBJECT:
			return null;
		default:
		    return null;
		}
        } catch (Throwable t) {
            t.printStackTrace();
            return null;
        }
    }

    /**
     * Deletes an object
     * @param id object id
     */
    public void deleteObject(String id) {
    	allObjects.remove(id);
    }

}
