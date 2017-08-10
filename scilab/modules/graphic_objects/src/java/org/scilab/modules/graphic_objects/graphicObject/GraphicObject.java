/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Manuel JULIACHS
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
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

package org.scilab.modules.graphic_objects.graphicObject;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_ARC__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AXESMODEL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AXES__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AXIS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACKTYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACK__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CHAMP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CHILDREN_COUNT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CHILDREN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_COMPOUND__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATA_MODEL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FAC3D__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FEC__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FIGUREMODEL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FIGURE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_GRAYPLOT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_HAS_LEGEND_CHILD__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_HIDDEN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LABEL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LEGEND_CHILD__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LEGEND__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LIGHT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MATPLOT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PARENT_AXES__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PARENT_FIGURE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PARENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PLOT3D__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POLYLINE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PROGRESSIONBAR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_RECTANGLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_REFERENCED__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SEGS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SELECTED_CHILD__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TAG__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TEXT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICONTEXTMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UIMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_CHECKBOX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_EDIT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_SCROLLABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_IMAGE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LAYER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LISTBOX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_POPUPMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_PUSHBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RADIOBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SLIDER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SPINNER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TAB__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TEXT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VALID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_WAITBAR__;

import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

import org.scilab.modules.graphic_objects.ObjectRemovedException;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.legend.Legend;
import org.scilab.modules.graphic_objects.uicontrol.frame.Frame;

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
                       LABEL, LEGEND, MATPLOT, PLOT3D, POLYLINE, RECTANGLE, SEGS, TEXT, CHECKBOX, EDIT, SPINNER, FRAME,
                       IMAGE, LISTBOX, POPUPMENU, PUSHBUTTON, RADIOBUTTON, CONSOLE, JAVACONSOLE, SLIDER, TABLE, UITEXT, UIMENU, UIMENUMODEL,
                       PROGRESSIONBAR, WAITBAR, UICONTEXTMENU, DATATIP, LIGHT, TABGROUP, TAB, LAYER, BORDER, FRAME_SCROLLABLE, UNKNOWNOBJECT
                     };

    /** GraphicObject properties */
    public enum GraphicObjectPropertyType { PARENT, CHILDREN, CHILDREN_COUNT, HIDDEN, VISIBLE, TYPE, REFERENCED, VALID, DATA,
                                            PARENT_FIGURE, PARENT_AXES, HASLEGENDCHILD, LEGENDCHILD, SELECTEDCHILD, TAG, CALLBACK, CALLBACKTYPE, UNKNOWNPROPERTY
                                          };

    /** Identifier */
    private Integer identifier;

    /** Parent object is known by its UID */
    private Integer parent;

    /** Child objects list. Known by their UID */
    private List <Integer> children;

    /** Specifies whether the object is visible or not */
    private boolean visible;

    /** Specifies if the "handle" is referenced in scilab */
    private boolean referenced;

    /** Specifies if the "handle" is valid, i.e included in a rendered object */
    private boolean valid;

    /** Specifies if the "handle" is hidden, i.e not listed as children in Scilab view */
    private boolean hidden;

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
    private Integer selectedChild;

    /** Constructor */
    public GraphicObject() {
        identifier = 0;
        parent = 0;
        children = new LinkedList<Integer>();
        visible = true;
        valid = true;
        referenced = false;
        selectedChild = 0;
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
        copy.setChildren(new LinkedList<Integer>());

        /*
         * Avoids keeping the Figure model as a parent
         * when the Axes model is cloned.
         */
        copy.setParent(0);

        /*
         * Sets no object as the selected child.
         */
        copy.setSelectedChild(0);

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
            case __GO_UI_SPINNER__ :
                return Type.SPINNER;
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
            case __GO_DATATIP__:
                return Type.DATATIP;
            case __GO_LIGHT__ :
                return Type.LIGHT;
            case __GO_UI_TAB__ :
                return Type.TAB;
            case __GO_UI_LAYER__ :
                return Type.LAYER;
            case __GO_UI_FRAME_BORDER__ :
                return Type.BORDER;
            case __GO_UI_FRAME_SCROLLABLE__ :
                return Type.FRAME_SCROLLABLE;
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
                setParent((Integer) value);
                break;
            case CHILDREN:
                setChildren((Integer[]) value);
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
            case SELECTEDCHILD:
                setSelectedChild((Integer) value);
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
    public Integer[] getChildren() {
        return children.toArray(new Integer[children.size()]);
    }

    /**
     * Adds a child.
     * @param child the identifier of the added child.
     */
    public void addChild(Integer child) {
        children.add(0, child);
    }

    /**
     * Removes a child.
     * @param child the identifier of the removed child.
     */
    public void removeChild(Integer child) {
        children.remove(child);
    }

    /**
     * @param children the children to set
     */
    private UpdateStatus setChildren(List<Integer> children) {
        this.children = children;
        return UpdateStatus.Success;
    }

    /**
     * @param children the children to set
     */
    public UpdateStatus setChildren(Integer[] children) {
        this.children = new LinkedList<Integer>(Arrays.asList(children));
        return UpdateStatus.Success;
    }

    /**
     * @return the identifier
     */
    public Integer getIdentifier() {
        return identifier;
    }

    /**
     * @param identifier the identifier to set
     */
    public UpdateStatus setIdentifier(Integer identifier) {
        this.identifier = identifier;
        return UpdateStatus.Success;
    }

    /**
     * @return the parent
     */
    public Integer getParent() {
        return parent;
    }

    /**
     * @param parent the parent to set
     * @return TODO
     */
    public UpdateStatus setParent(Integer parent) {
        this.parent = parent;
        return UpdateStatus.Success;
    }

    /**
     * @return the tag
     */
    public String getTag() {
        return tag;
    }

    /**
     * @param tag the tag to set
     * @return TODO
     */
    public UpdateStatus setTag(String tag) {
        if (this.tag.equals(tag)) {
            return UpdateStatus.NoChange;
        }

        this.tag = tag;
        return UpdateStatus.Success;
    }

    /**
     * @return the callback
     */
    public String getCallbackString() {
        return callback.getCommand();
    }

    /**
     * @param callback the callback to set
     * @return TODO
     */
    public UpdateStatus setCallbackString(String callback) {
        return this.callback.setCommand(callback);
    }

    /**
     * @return the callbackType
     */
    public Integer getCallbackType() {
        return callback.getCommandType();
    }

    /**
     * @param callbackType the callbackType to set
     * @return TODO
     */
    public UpdateStatus setCallbackType(Integer callbackType) {
        this.callback.setCommandType(callbackType);
        return UpdateStatus.Success;
    }

    /**
     * Get parent Figure method
     * Returns the identifier of the object's parent Figure
     * If the object is a Figure, then returns its own identifier.
     * To be done: use a member variable storing the up-to-date current parent Figure,
     * returned instead of recursively ascending the hierarchy at each call.
     * @return the parent Figure identifier
     */
    public Integer getParentFigure() {
        if (this instanceof Figure) {
            return getIdentifier();
        }

        if (getParent() != 0 && GraphicController.getController().getObjectFromId(getParent()) != null) {
            return GraphicController.getController().getObjectFromId(getParent()).getParentFigure();
        }

        /* No parent Figure found */
        return 0;
    }

    /**
     * Get parent Figure method
     * Returns the identifier of the object's parent Figure
     * If the object is a Figure, then returns its own identifier.
     * To be done: use a member variable storing the up-to-date current parent Figure,
     * returned instead of recursively ascending the hierarchy at each call.
     * @return the parent Figure identifier
     */
    public Integer getParentFrameOrFigure() {
        if (this instanceof Figure || this instanceof Frame) {
            return getIdentifier();
        }

        if (getParent() != 0 && GraphicController.getController().getObjectFromId(getParent()) != null) {
            return GraphicController.getController().getObjectFromId(getParent()).getParentFrameOrFigure();
        }

        /* No parent Figure nor Frame found */
        return 0;
    }

    /**
     * Get parent Axes method
     * Returns the identifier of the object's parent Axes
     * If the object is an Axes, then returns its own identifier.
     * To be done: use a member variable storing the up-to-date current parent Axes,
     * returned instead of recursively ascending the hierarchy at each call.
     * @return the parent Axes identifier
     */
    public Integer getParentAxes() {
        if (this instanceof Axes) {
            return getIdentifier();
        } else {
            if (getParent() != 0 && GraphicController.getController().getObjectFromId(getParent()) != null) {
                return GraphicController.getController().getObjectFromId(getParent()).getParentAxes();
            } else {
                /* No parent Axes found */
                return 0;
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
    public Integer getLegendChild() {
        for (int i = 0; i < children.size(); i++) {
            GraphicObject currentObject = GraphicController.getController().getObjectFromId(children.get(i));

            if (currentObject instanceof Legend) {
                return currentObject.getIdentifier();
            }
        }

        /* No child legend found */
        return 0;
    }

    /**
     * Get selected child method
     * @return the selected child
     */
    public Integer getSelectedChild() {
        return selectedChild;
    }

    /**
     * Set selected child method
     * @param selectedChild the selected child to set
     */
    public UpdateStatus setSelectedChild(Integer selectedChild) {
        this.selectedChild = selectedChild;
        return UpdateStatus.Success;
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
     * @return TODO
     */
    public UpdateStatus setVisible(Boolean visible) {
        if (this.visible == visible) {
            return UpdateStatus.NoChange;
        }

        this.visible = visible;
        return UpdateStatus.Success;
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
     * @return TODO
     */
    public UpdateStatus setHidden(Boolean hidden) {
        this.hidden = hidden;
        return UpdateStatus.Success;
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
     * @return TODO
     */
    public UpdateStatus setValid(Boolean valid) {
        this.valid = valid;
        return UpdateStatus.Success;
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
     * @return TODO
     */
    public UpdateStatus setReferenced(Boolean referenced) {
        this.referenced = referenced;
        return UpdateStatus.Success;
    }
}
