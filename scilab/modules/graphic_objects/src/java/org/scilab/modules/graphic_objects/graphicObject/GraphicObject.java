/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Manuel JULIACHS
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.graphicObject;

import org.scilab.modules.graphic_objects.ObjectRemovedException;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.legend.Legend;

import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * GraphicObject class
 * @author Manuel JULIACHS
 * @author Vincent COUVERT
 */
public abstract class GraphicObject implements Cloneable {

    public enum UpdateStatus {
        Success,        // Property updated with new values
        NoChange,       // Property leave unchanged
        Fail            // Update Fail
    };

    /** User data array default size */
    public static final int USER_DATA_DEFAULT_SIZE = 0;

    /** Graphic objects types */
    public enum Type { ARC, AXES, AXESMODEL, AXIS, CHAMP, COMPOUND, FAC3D, FEC, FIGURE, FIGUREMODEL, GRAYPLOT,
                       LABEL, LEGEND, MATPLOT, PLOT3D, POLYLINE, RECTANGLE, SEGS, TEXT, CHECKBOX, EDIT, FRAME,
                       IMAGE, LISTBOX, POPUPMENU, PUSHBUTTON, RADIOBUTTON, CONSOLE, JAVACONSOLE, SLIDER, TABLE, UITEXT, UIMENU, UIMENUMODEL,
                       PROGRESSIONBAR, WAITBAR, UICONTEXTMENU, UNKNOWNOBJECT
                     };

    /** GraphicObject properties */
    public enum GraphicObjectPropertyType { PARENT, CHILDREN, CHILDREN_COUNT, HIDDEN, VISIBLE, USERDATA, USERDATASIZE, TYPE, REFERENCED, VALID, DATA,
                                            PARENT_FIGURE, PARENT_AXES, HASLEGENDCHILD, LEGENDCHILD, SELECTEDCHILD, TAG, CALLBACK, CALLBACKTYPE, UNKNOWNPROPERTY
                                          };

    /** Identifier */
    private String identifier;

    /** Parent object is known by its UID */
    private String parent;

    /** Child objects list. Known by their UID */
    private List <String> children;

    /** Specifies whether the object is visible or not */
    private boolean visible;

    /** Specifies if the "handle" is referenced in scilab */
    private boolean referenced;

    /** Specifies if the "handle" is valid, i.e included in a rendered object */
    private boolean valid;

    /** Specifies if the "handle" is hidden, i.e not listed as children in Scilab view */
    private boolean hidden;

    /** User data */
    private Integer[] userData;

    /** Tag */
    private String tag;

    /** Callback */
    private CallBack callback;

    /**
     * Identifier of the selected child
     * This was previously implemented as a list, but is used in practice
     * to store only the identifier of the currently selected child.
     * To do: use a list if required
     */
    private String selectedChild;

    /** Constructor */
    public GraphicObject() {
        identifier = null;
        parent = "";
        children = new LinkedList<String>();
        visible = true;
        userData = null;
        valid = true;
        referenced = false;
        selectedChild = "";
        tag = "";
        callback = new CallBack("");
    }

    /**
     * Clone
     * @return clone
     * @see java.lang.Object#clone()
     */
    public GraphicObject clone() {
        GraphicObject copy = null;

        try {
            copy = (GraphicObject) super.clone();
        } catch (CloneNotSupportedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        /*
         * Creating an empty list is done to avoid
         * still referencing the original object's own list,
         * which occurs when the Figure model is cloned.
         */
        copy.setChildren(new LinkedList<String>());

        /*
         * Avoids keeping the Figure model as a parent
         * when the Axes model is cloned.
         */
        copy.setParent("");

        /*
         * Sets no object as the selected child.
         */
        copy.setSelectedChild("");

        return copy;
    }

    abstract public void accept(Visitor visitor) throws ObjectRemovedException;

    /**
     * Returns the enum associated to a type name
     * @param typeName the property name
     * @return the type enum
     */
    public static Type getTypeFromName(int typeName) {
        switch (typeName) {
        case __GO_ARC__ :
            return Type.ARC;
        case __GO_AXES__ :
            return Type.AXES;
        case __GO_AXESMODEL__ :
            return Type.AXESMODEL;
        case __GO_AXIS__ :
            return Type.AXIS;
        case __GO_CHAMP__ :
            return Type.CHAMP;
        case __GO_COMPOUND__ :
            return Type.COMPOUND;
        case __GO_FAC3D__ :
            return Type.FAC3D;
        case __GO_FEC__ :
            return Type.FEC;
        case __GO_FIGURE__ :
            return Type.FIGURE;
        case __GO_FIGUREMODEL__ :
            return Type.FIGUREMODEL;
        case __GO_GRAYPLOT__ :
            return Type.GRAYPLOT;
        case __GO_LABEL__ :
            return Type.LABEL;
        case __GO_LEGEND__ :
            return Type.LEGEND;
        case __GO_MATPLOT__ :
            return Type.MATPLOT;
        case __GO_PLOT3D__ :
            return Type.PLOT3D;
        case __GO_POLYLINE__ :
            return Type.POLYLINE;
        case __GO_RECTANGLE__ :
            return Type.RECTANGLE;
        case __GO_SEGS__ :
            return Type.SEGS;
        case __GO_TEXT__ :
            return Type.TEXT;
        case __GO_UI_CHECKBOX__ :
            return Type.CHECKBOX;
        case __GO_UI_EDIT__ :
            return Type.EDIT;
        case __GO_UI_FRAME__ :
            return Type.FRAME;
        case __GO_UI_IMAGE__ :
            return Type.IMAGE;
        case __GO_UI_LISTBOX__ :
            return Type.LISTBOX;
        case __GO_UI_POPUPMENU__ :
            return Type.POPUPMENU;
        case __GO_UI_PUSHBUTTON__ :
            return Type.PUSHBUTTON;
        case __GO_UI_RADIOBUTTON__ :
            return Type.RADIOBUTTON;
        case __GO_UI_SLIDER__ :
            return Type.SLIDER;
        case __GO_UI_TABLE__ :
            return Type.TABLE;
        case __GO_UI_TEXT__ :
            return Type.UITEXT;
        case __GO_UIMENU__ :
            return Type.UIMENU;
        case __GO_UICONTEXTMENU__ :
            return Type.UICONTEXTMENU;
        case __GO_PROGRESSIONBAR__ :
            return Type.PROGRESSIONBAR;
        case __GO_WAITBAR__ :
            return Type.WAITBAR;
        default :
            return Type.UNKNOWNOBJECT;
        }
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(int propertyName) {
        switch (propertyName) {
        case __GO_PARENT__ :
            return  GraphicObjectPropertyType.PARENT;
        case __GO_CHILDREN__ :
            return GraphicObjectPropertyType.CHILDREN;
        case __GO_CHILDREN_COUNT__ :
            return GraphicObjectPropertyType.CHILDREN_COUNT;
        case __GO_HIDDEN__ :
            return GraphicObjectPropertyType.HIDDEN;
        case __GO_VISIBLE__ :
            return GraphicObjectPropertyType.VISIBLE;
        case __GO_USER_DATA__ :
            return GraphicObjectPropertyType.USERDATA;
        case __GO_USER_DATA_SIZE__ :
            return GraphicObjectPropertyType.USERDATASIZE;
        case __GO_REFERENCED__ :
            return GraphicObjectPropertyType.REFERENCED;
        case __GO_VALID__ :
            return GraphicObjectPropertyType.VALID;
        case __GO_PARENT_FIGURE__ :
            return GraphicObjectPropertyType.PARENT_FIGURE;
        case __GO_PARENT_AXES__ :
            return GraphicObjectPropertyType.PARENT_AXES;
        case __GO_HAS_LEGEND_CHILD__ :
            return GraphicObjectPropertyType.HASLEGENDCHILD;
        case __GO_LEGEND_CHILD__ :
            return GraphicObjectPropertyType.LEGENDCHILD;
        case __GO_SELECTED_CHILD__ :
            return GraphicObjectPropertyType.SELECTEDCHILD;
        case __GO_TYPE__ :
            return GraphicObjectPropertyType.TYPE;
        case __GO_DATA_MODEL__ :
            return GraphicObjectPropertyType.DATA;
        case __GO_TAG__ :
            return GraphicObjectPropertyType.TAG;
        case __GO_CALLBACK__ :
            return GraphicObjectPropertyType.CALLBACK;
        case __GO_CALLBACKTYPE__ :
            return GraphicObjectPropertyType.CALLBACKTYPE;
        default:
            //System.err.println("[ERROR] Unknown Property : "+propertyName+" !!!!!!!!!!");
            return GraphicObjectPropertyType.UNKNOWNPROPERTY;
        }
    }

    /**
     * Fast property get method
     * @param property the property to get
     * @return the property value
     */
    public Object getProperty(Object property) {
        if (!(property instanceof GraphicObjectPropertyType)) {
            return null;
        }

        GraphicObjectPropertyType p = (GraphicObjectPropertyType) property;
        switch (p) {
        case PARENT:
            return getParent();
        case CHILDREN:
            return getChildren();
        case CHILDREN_COUNT:
            return children.size();
        case VALID:
            return isValid();
        case HIDDEN:
            return isHidden();
        case VISIBLE:
            return getVisible();
        case USERDATA:
            return getUserData();
        case USERDATASIZE:
            return getUserDataSize();
        case PARENT_FIGURE:
            return getParentFigure();
        case PARENT_AXES:
            return getParentAxes();
        case HASLEGENDCHILD:
            return getHasLegendChild();
        case LEGENDCHILD:
            return getLegendChild();
        case SELECTEDCHILD:
            return getSelectedChild();
        case TYPE:
            return getType();
        case DATA:
            return getIdentifier();
        case TAG:
            return getTag();
        case CALLBACK:
            return getCallbackString();
        case CALLBACKTYPE:
            return getCallbackType();
        case UNKNOWNPROPERTY:
            return null;
        default:
            return null;
        }
    }

    /**
     * Fast property set method
     * @param property the property to set
     * @param value the property value
     * @return true if the property has been set, false otherwise
     */
    public UpdateStatus setProperty(Object property, Object value) {
        if (!(property instanceof GraphicObjectPropertyType)) {
            return UpdateStatus.Success;
        }

        GraphicObjectPropertyType p = (GraphicObjectPropertyType) property;
        switch (p) {
        case PARENT:
            setParent((String) value);
            break;
        case CHILDREN:
            setChildren((String[]) value);
            break;
        case VALID:
            setValid((Boolean) value);
            break;
        case HIDDEN:
            setHidden((Boolean) value);
            break;
        case VISIBLE:
            setVisible((Boolean) value);
            break;
        case USERDATA:
            setUserData((Integer[]) value);
            break;
        case USERDATASIZE:
            return UpdateStatus.Fail;
        case SELECTEDCHILD:
            setSelectedChild((String) value);
            break;
        case DATA:
            return UpdateStatus.Success;
        case TAG:
            setTag((String) value);
            break;
        case CALLBACK:
            setCallbackString((String) value);
            break;
        case CALLBACKTYPE:
            setCallbackType((Integer) value);
            break;
        case UNKNOWNPROPERTY:
            return UpdateStatus.Fail;
        default:
            return UpdateStatus.Success;
        }

        return UpdateStatus.Success;
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
     * Void property get method
     * @param property the property name
     */
    public Object getPropertyVoid(String property) {
        // TODO
        return null;
    }

    /* TODO */
    /**
     * Void property set method
     * @param property the property name
     * @param value the property value
     */
    public void setPropertyVoid(String property, Object value) {
        // TODO
    }

    /**
     * @return the children
     */
    public String[] getChildren() {
        return children.toArray(new String[children.size()]);
    }

    /**
     * Adds a child.
     * @param child the identifier of the added child.
     */
    public void addChild(String child) {
	children.add(0, child);
    }

    /**
     * Removes a child.
     * @param child the identifier of the removed child.
     */
    public void removeChild(String child) {
        children.remove(child);
    }

    /**
     * @param children the children to set
     */
    private void setChildren(List<String> children) {
        this.children = children;
    }

    /**
     * @param children the children to set
     */
    public void setChildren(String[] children) {
        this.children = new LinkedList<String>(Arrays.asList(children));
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
    public String getParent() {
        return parent;
    }

    /**
     * @param parent the parent to set
     */
    public void setParent(String parent) {
        this.parent = parent;
    }

    /**
     * @return the userData
     */
    public Object getUserData() {
        return userData;
    }

    /**
     * @param userData the userData to set
     */
    public void setUserData(Integer[] userData) {
        this.userData = userData;
    }

    /**
     * @return the userDataSize
     */
    public Integer getUserDataSize() {
        if (userData != null) {
            return userData.length;
        }
        return 0;
    }

    /**
     * @return the tag
     */
    public String getTag() {
        return tag;
    }

    /**
     * @param tag the tag to set
     */
    public void setTag(String tag) {
        this.tag = tag;
    }

    /**
     * @return the callback
     */
    public String getCallbackString() {
        return callback.getCommand();
    }

    /**
     * @param callback the callback to set
     */
    public void setCallbackString(String callback) {
        this.callback.setCommand(callback);
    }

    /**
     * @return the callbackType
     */
    public Integer getCallbackType() {
        return callback.getCommandType();
    }

    /**
     * @param callbackType the callbackType to set
     */
    public void setCallbackType(Integer callbackType) {
        this.callback.setCommandType(callbackType);
    }

    /**
     * Get parent Figure method
     * Returns the identifier of the object's parent Figure
     * If the object is a Figure, then returns its own identifier.
     * To be done: use a member variable storing the up-to-date current parent Figure,
     * returned instead of recursively ascending the hierarchy at each call.
     * @return the parent Figure identifier
     */
    public String getParentFigure() {
        if (this instanceof Figure) {
            return getIdentifier();
        } else {
            if (getParent() != null && GraphicController.getController().getObjectFromId(getParent()) != null) {
                return GraphicController.getController().getObjectFromId(getParent()).getParentFigure();
            } else {
                /* No parent Figure found */
                return "";
            }
        }
    }

    /**
     * Get parent Axes method
     * Returns the identifier of the object's parent Axes
     * If the object is an Axes, then returns its own identifier.
     * To be done: use a member variable storing the up-to-date current parent Axes,
     * returned instead of recursively ascending the hierarchy at each call.
     * @return the parent Axes identifier
     */
    public String getParentAxes() {
        if (this instanceof Axes) {
            return getIdentifier();
        } else {
            if (getParent() != null && GraphicController.getController().getObjectFromId(getParent()) != null) {
                return GraphicController.getController().getObjectFromId(getParent()).getParentAxes();
            } else {
                /* No parent Axes found */
                return "";
            }
        }
    }

    /**
     * Get has legend child method
     * Returns a boolean indicating whether one of the object's direct children
     * is a Legend object. Only one Legend is supposed to be present in the list.
     * To be done: storing the property and updating it only when a Legend object
     * is inserted or deleted instead of searching the children list when the
     * property is queried.
     * @return a Boolean indicating whether the object has a child Legend object or not
     */
    public Boolean getHasLegendChild() {
        for (int i = 0; i < children.size(); i++) {
            GraphicObject currentObject = GraphicController.getController().getObjectFromId(children.get(i));

            if (currentObject instanceof Legend) {
                return true;
            }
        }

        return false;
    }

    /**
     * Returns the identifier of the object's direct child that is a Legend object.
     * It returns an empty string if the object has no Legend in its children list
     * (one Legend is supposed to be present at most).
     * @return the object's child Legend object identifier, or an empty string if no child Legend found.
     */
    public String getLegendChild() {
        for (int i = 0; i < children.size(); i++) {
            GraphicObject currentObject = GraphicController.getController().getObjectFromId(children.get(i));

            if (currentObject instanceof Legend) {
                return currentObject.getIdentifier();
            }
        }

        /* No child legend found */
        return "";
    }

    /**
     * Get selected child method
     * @return the selected child
     */
    public String getSelectedChild() {
        return selectedChild;
    }

    /**
     * Set selected child method
     * @param selectedChild the selected child to set
     */
    public void setSelectedChild(String selectedChild) {
        this.selectedChild = selectedChild;
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

    /**
     * Each type should name itself
     * @return Type as String
     */
    public abstract Integer getType();

    /**
     * isValid method
     * @return valid
     */
    public Boolean isValid() {
        return valid;
    }

    /**
     * Set hidden method
     * @param hidden the value to set
     */
    public void setHidden(Boolean hidden) {
        this.hidden = hidden;
    }

    /**
     * isHidden method
     * @return hidden
     */
    public Boolean isHidden() {
        return hidden;
    }

    /**
     * Set valid method
     * @param valid the validity to set
     */
    public void setValid(Boolean valid) {
        this.valid = valid;
    }

    /**
     * isReferenced method
     * @return referenced
     */
    public Boolean isReferenced() {
        return referenced;
    }

    /**
     * Set referenced method
     * @param referenced the reference status to set
     */
    public void setReferenced(Boolean referenced) {
        this.referenced = referenced;
    }
}
