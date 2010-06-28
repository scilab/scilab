/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.graphicObject;

import java.lang.reflect.Method;
import java.util.ArrayList;

/**
 * GraphicObject class
 * @author Manuel JULIACHS
 */
public abstract class GraphicObject {
	/** Graphic objects types */
	public enum Type { ARC, AXES, AXIS, CHAMP, COMPOUND, FAC3D, FEC, FIGURE, GRAYPLOT,
		LABEL, LEGEND, MATPLOT, PLOT3D, POLYLINE, RECTANGLE, SEGS, TEXT, UNKNOWNOBJECT };
	
	/** GraphicObject properties */
	public enum GraphicObjectPropertyType { PARENT, CHILDREN, VISIBLE, USERDATA, UNKNOWNPROPERTY };

	/** Identifier */
	private String identifier;
	
	/** Parent object */
	private GraphicObject parent;

	/** Child objects list */
	private ArrayList <GraphicObject> children;

	/** Specifies whether the object is visible or not */
	private boolean visible;

	/** User data */
	private byte[] userData;

	/** Constructor */
	public GraphicObject() {
		identifier = null;
		parent = null;
		children = null;
		visible = false;
		userData = null;
	}

	/**
     * Returns the enum associated to a type name
     * @param typeName the property name
     * @return the type enum
     */
    public static Type getTypeFromName(String typeName) {
        if (typeName.equals("arc")) {
            return Type.ARC;
        }
        else if (typeName.equals("axes")) {
            return Type.AXES;
        }
        else if (typeName.equals("axis")) {
            return Type.AXIS;
        }
        else if (typeName.equals("champ")) {
            return Type.CHAMP;
        }
        else if (typeName.equals("compound")) {
            return Type.COMPOUND;
        }
        else if (typeName.equals("fac3d")) {
            return Type.FAC3D;
        }
        else if (typeName.equals("fec")) {
            return Type.FEC;
        }
        else if (typeName.equals("figure")) {
            return Type.FIGURE;
        }
        else if (typeName.equals("grayplot")) {
            return Type.GRAYPLOT;
        }
        else if (typeName.equals("label")) {
            return Type.LABEL;
        }
        else if (typeName.equals("legend")) {
            return Type.LEGEND;
        }
        else if (typeName.equals("matplot")) {
            return Type.MATPLOT;
        }
        else if (typeName.equals("plot3d")) {
            return Type.PLOT3D;
        }
        else if (typeName.equals("polyline")) {
            return Type.POLYLINE;
        }
        else if (typeName.equals("rectangle")) {
            return Type.RECTANGLE;
        }
        else if (typeName.equals("segs")) {
            return Type.SEGS;
        }
        else if (typeName.equals("text")) {
            return Type.TEXT;
        }
        else {
            return Type.UNKNOWNOBJECT;
        }
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		if (propertyName.equals("Parent")) {
			return  GraphicObjectPropertyType.PARENT;
		} else if (propertyName.equals("Children")) {
			return GraphicObjectPropertyType.CHILDREN;
		} else if (propertyName.equals("Visible")) {
			return GraphicObjectPropertyType.VISIBLE;
		} else if (propertyName.equals("UserData")) {
			return GraphicObjectPropertyType.USERDATA;
		} else {
			return GraphicObjectPropertyType.UNKNOWNPROPERTY;
		}
	}

	/**
	 * Fast property get method
	 * @param property the property to get
	 * @return the property value
	 */
	public Object getPropertyFast(Object property) {
		if (property == GraphicObjectPropertyType.PARENT) {
			return getParent();
		} else if (property == GraphicObjectPropertyType.CHILDREN) {
			return getChildren();
		} else if (property == GraphicObjectPropertyType.VISIBLE) {
			return getVisible();
		} else if (property == GraphicObjectPropertyType.USERDATA) {
			return getUserData();
		} else if (property == GraphicObjectPropertyType.UNKNOWNPROPERTY) {
			return null;
		} else {
			return null;
		}
	}

	/**
	 * Fast property set method
	 * @param property the property to set
	 * @param value the property value
	 * @return true if the property has been set, false otherwise
	 */
	public boolean setPropertyFast(Object property, Object value) {
		if (property == GraphicObjectPropertyType.PARENT) {
			setParent((GraphicObject) value);
		} else if (property == GraphicObjectPropertyType.CHILDREN) {
			setChildren((ArrayList<GraphicObject>) value);
		} else if (property == GraphicObjectPropertyType.VISIBLE) {
			setVisible((Boolean) value);
		} else if (property == GraphicObjectPropertyType.USERDATA) {
			setUserData((byte[]) value);
		} else if (property == GraphicObjectPropertyType.UNKNOWNPROPERTY) {
			return false;
		}

		return true;
	}

	/**
	 * Set property method
	 * @param property property name
	 * @param value property value
	 */
    public void setProperty(String property, Object value) {
    	try {
    	    Method setter = this.getClass().getMethod("set" + property, value.getClass());
    	    setter.invoke(this, value);
    	} catch (Exception e) {
    	    System.err.println("Got Exception " + e.getMessage());
    	    e.printStackTrace();
    	}

    }
    
    /**
     * Returns a null property
     * @param property property name
     * @return null property
     */
	public Object getNullProperty(String property) {
		return null;
	}

	/**
	 * Get property method
	 * @param property the property name string
	 * @return the property value
	 */
	public Object getProperty(String property) {
		try {
			Method getter = this.getClass().getMethod("get" + property, (Class[]) null);
			return getter.invoke(this, (Object[]) null);
		} catch (Exception e) {
			System.err.println("Got Exception " + e.getMessage());
		} // TODO Auto-generated method stub
		return null;
	}
	

    /**
     * Void property get method
     * @param property the property name
     */
	public Object getPropertyVoid(String property) {
		// TBD
		return null;
	}

	/* TBD */
	/**
	 * Void property set method
	 * @param property the property name
	 * @param value the property value
	 */
	public void setPropertyVoid(String property, Object value) {
		// TBD
	}
	
	/**
	 * @return the children
	 */
	public ArrayList<GraphicObject> getChildren() {
		return children;
	}

	/**
	 * @param children the children to set
	 */
	public void setChildren(ArrayList<GraphicObject> children) {
		this.children = children;
	}

	/**
	 * @return the identifier
	 */
	public String getIdentifier() {
		return identifier;
	}

	/**
	 * @param identifier the identifier to set
	 */
	public void setIdentifier(String identifier) {
		this.identifier = identifier;
	}

	/**
	 * @return the parent
	 */
	public GraphicObject getParent() {
		return parent;
	}

	/**
	 * @param parent the parent to set
	 */
	public void setParent(GraphicObject parent) {
		this.parent = parent;
	}

	/**
	 * @return the userData
	 */
	public byte[] getUserData() {
		return userData;
	}

	/**
	 * @param userData the userData to set
	 */
	public void setUserData(byte[] userData) {
		this.userData = userData;
	}

	/**
	 * Get visible method
	 * @return the visible
	 */
	public Boolean getVisible() {
		return visible;
	}

	/**
	 * Set visible method
	 * @param visible the visible to set
	 */
	public void setVisible(Boolean visible) {
		this.visible = visible;
	}

}
