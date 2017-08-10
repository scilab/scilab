/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
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

package org.scilab.modules.graphic_objects.textObject;

import org.scilab.modules.graphic_objects.graphicObject.ClippableProperty;
import org.scilab.modules.graphic_objects.graphicObject.ClippableProperty.ClippablePropertyType;
import org.scilab.modules.graphic_objects.graphicObject.ClippableProperty.ClipStateType;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * ClippableTextObject
 * @author Manuel JULIACHS
 */
public abstract class ClippableTextObject extends TextObject {
    /** ClippableTextObject properties names */
    private enum ClippableTextObjectProperty { CLIPPROPERTY };

    /** Clipping state */
    private ClippableProperty clipProperty;

    /**  Constructor */
    public ClippableTextObject() {
        clipProperty = new ClippableProperty();
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(int propertyName) {
        switch (propertyName) {
            case __GO_CLIP_PROPERTY__ :
                return ClippableTextObjectProperty.CLIPPROPERTY;
            case __GO_CLIP_STATE__ :
                return ClippablePropertyType.CLIPSTATE;
            case __GO_CLIP_BOX__ :
                return ClippablePropertyType.CLIPBOX;
            case __GO_CLIP_BOX_SET__ :
                return ClippablePropertyType.CLIPBOXSET;
            default :
                return super.getPropertyFromName(propertyName);
        }
    }

    /**
     * Fast property get method
     * @param property the property to get
     * @return the property value
     */
    public Object getProperty(Object property) {
        if (property == ClippableTextObjectProperty.CLIPPROPERTY) {
            return getClipProperty();
        } else if (property == ClippablePropertyType.CLIPSTATE) {
            return getClipState();
        } else if (property == ClippablePropertyType.CLIPBOX) {
            return getClipBox();
        } else if (property == ClippablePropertyType.CLIPBOXSET) {
            return getClipBoxSet();
        } else {
            return super.getProperty(property);
        }
    }

    /**
     * Fast property set method
     * @param property the property to set
     * @param value the property value
     * @return true if the property has been set, false otherwise
     */
    public UpdateStatus setProperty(Object property, Object value) {
        if (property == ClippableTextObjectProperty.CLIPPROPERTY) {
            setClipProperty((ClippableProperty) value);
        } else if (property == ClippablePropertyType.CLIPSTATE) {
            setClipState((Integer) value);
        } else if (property == ClippablePropertyType.CLIPBOX) {
            setClipBox((Double[]) value);
        } else if (property == ClippablePropertyType.CLIPBOXSET) {
            setClipBoxSet((Boolean) value);
        } else {
            return super.setProperty(property, value);
        }

        return UpdateStatus.Success;
    }

    /**
     * @return the clipProperty
     */
    public ClippableProperty getClipProperty() {
        return clipProperty;
    }

    /**
     * @param clipProperty the clipProperty to set
     */
    public UpdateStatus setClipProperty(ClippableProperty clipProperty) {
        this.clipProperty = clipProperty;
        return UpdateStatus.Success;
    }

    /**
     * @return the clipBox
     */
    public Double[] getClipBox() {
        return clipProperty.getClipBox();
    }

    /**
     * @param clipBox the clipBox to set
     */
    public UpdateStatus setClipBox(Double[] clipBox) {
        return clipProperty.setClipBox(clipBox);
    }

    /**
     * @return the clipState
     */
    public Integer getClipState() {
        return getClipStateAsEnum().ordinal();
    }

    /**
     * @return the clipState
     */
    public ClipStateType getClipStateAsEnum() {
        return clipProperty.getClipState();
    }

    /**
     * @param clipState the clipState to set
     */
    public UpdateStatus setClipState(Integer clipState) {
        return setClipStateAsEnum(ClipStateType.intToEnum(clipState));
    }

    /**
     * @param clipState the clipState to set
     */
    public UpdateStatus setClipStateAsEnum(ClipStateType clipState) {
        return clipProperty.setClipState(clipState);
    }

    /**
     * @return the clipBoxSet
     */
    public Boolean getClipBoxSet() {
        return clipProperty.getClipBoxSet();
    }

    /**
     * @param clipBoxSet the clipBoxSet to set
     */
    public UpdateStatus setClipBoxSet(Boolean clipBoxSet) {
        return clipProperty.setClipBoxSet(clipBoxSet);
    }

}
