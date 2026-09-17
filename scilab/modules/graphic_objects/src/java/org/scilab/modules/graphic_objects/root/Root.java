/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2021 - Stéphane Mottelet
 *
 * This file is hereby licensed under the terms of the GNU GPL v3.0,
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.graphic_objects.root;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_ROOT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SHOWHIDDENHANDLES__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SHOWHIDDENPROPERTIES__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_USEDEPRECATEDLF__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POINTER_LOCATION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_UNITS__;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

import java.awt.MouseInfo;
import java.awt.PointerInfo;
import java.awt.Point;

/**
 * @author Stéphane MOTTELET
 */
public final class Root extends GraphicObject {

    /**
     * Scilab mode enum
     * Java Root created in both modes
     */
    public enum ScilabMode { STD, NW };

    private static Root me;

    private boolean showHiddenHandles;

    private boolean showHiddenProperties;

    private ScilabMode scilabMode;

    private boolean useDeprecatedLF = false;

    private String units = "pixels";

    /** Root properties names */
    private enum RootProperty {
        SHOWHIDDENHANDLES,
        SHOWHIDDENPROPERTIES,
        USEDEPRECATEDLF,
        POINTER_LOCATION,
        UNITS
    };

    /**
     * Constructor
     */
    private Root() {
        super();
    }

    /**
     * Returns the root object
     * @return the root object
     */
    public static Root getRoot() {
        if (me == null) {
            me = new Root();
        }

        return me;

    }

    /**
     * Get this object type
     * @return "Root"
     * @see org.scilab.modules.graphic_objects.graphicObject.GraphicObject#getType()
     */
    public Integer getType() {
        return __GO_ROOT__;
    }

    /**
     * Set the scilabMode property
     * @param scilabMode the new value to set
     */
    public UpdateStatus setScilabMode(ScilabMode scilabMode) {
        this.scilabMode = scilabMode;
        return UpdateStatus.Success;
    }

    /**
     * Get the javaEnabled proeprty
     * @return javaEnabled
     */
    public ScilabMode getScilabMode() {
        return this.scilabMode;
    }

    /**
     * Set the showHiddenHandles property
     * @param showHiddenHandles the new value to set
     */
    public UpdateStatus setShowHiddenHandles(boolean showHiddenHandles) {
        this.showHiddenHandles = showHiddenHandles;
        return UpdateStatus.Success;
    }

    /**
     * Get the showHiddenHandles property
     * @return showHiddenHandles
     */
    public boolean getShowHiddenHandles() {
        return this.showHiddenHandles;
    }

    /**
     * Set the showHiddenProperties property
     * @param showHiddenProperties the new value to set
     */
    public UpdateStatus setShowHiddenProperties(boolean showHiddenProperties) {
        this.showHiddenProperties = showHiddenProperties;
        return UpdateStatus.Success;
    }

    /**
     * Get the showHiddenProperties property
     * @return showHiddenProperties
     */
    public boolean getShowHiddenProperties() {
        return this.showHiddenProperties;
    }

    /**
     * Get the pointer_location property
     * @return pointer_location
     */
    public Integer[] getPointerLocation() {
        Integer[] retPoint = new Integer[2];
        Point location = MouseInfo.getPointerInfo().getLocation();
        retPoint[0] = location.x;
        retPoint[1] = location.y;

        return retPoint;
    }

    /**
     * Accept method
     * @param visitor the visitor
     * @see org.scilab.modules.graphic_objects.graphicObject.GraphicObject#accept(org.scilab.modules.graphic_objects.graphicObject.Visitor)
     */
    public void accept(Visitor visitor) {

    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(int propertyName) {
        if (propertyName == __GO_SHOWHIDDENHANDLES__) {
            return RootProperty.SHOWHIDDENHANDLES;
        } else if (propertyName == __GO_SHOWHIDDENPROPERTIES__) {
            return RootProperty.SHOWHIDDENPROPERTIES;
        } else if (propertyName == __GO_USEDEPRECATEDLF__) {
            return RootProperty.USEDEPRECATEDLF;
        } else if (propertyName == __GO_POINTER_LOCATION__) {
            return RootProperty.POINTER_LOCATION;
        } else if (propertyName == __GO_UI_UNITS__) {
            return RootProperty.UNITS;
        } else {
            return super.getPropertyFromName(propertyName);
        }
    }
    /**
     * Fast property get method
     * @param property the property to get
     * @return the property value
     */
    public Object getProperty(Object property) {
        if (property == RootProperty.SHOWHIDDENHANDLES) {
            return getShowHiddenHandles();
        } else if (property == RootProperty.SHOWHIDDENPROPERTIES) {
            return getShowHiddenProperties();
        } else if (property == RootProperty.USEDEPRECATEDLF) {
            return getUseDeprecatedLF();
        } else if (property == RootProperty.POINTER_LOCATION) {
            return getPointerLocation();
        } else if (property == RootProperty.UNITS) {
            return getUnits();
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
        if (property == RootProperty.SHOWHIDDENHANDLES) {
            setShowHiddenHandles((Boolean) value);
        } else if (property == RootProperty.SHOWHIDDENPROPERTIES) {
            setShowHiddenProperties((Boolean) value);
        } else if (property == RootProperty.USEDEPRECATEDLF) {
            setUseDeprecatedLF((Boolean) value);
        } else if (property == RootProperty.UNITS) {
            return setUnits((String) value);
        } else {
            return super.setProperty(property, value);
        }
        return UpdateStatus.Success;
    }

    public boolean getUseDeprecatedLF() {
        return useDeprecatedLF;
    }

    public UpdateStatus setUseDeprecatedLF(boolean useDeprecatedLF) {
        if (this.useDeprecatedLF == useDeprecatedLF) {
            return UpdateStatus.NoChange;
        }
        this.useDeprecatedLF = useDeprecatedLF;
        return UpdateStatus.Success;
    }

    /**
     * Get the units property
     * @return units
     */
    public String getUnits() {
        return this.units;
    }

    /**
     * Set the units property
     * @param units the new value to set
     */
    public UpdateStatus setUnits(String units) {
        if (this.units.equals(units)) {
            return UpdateStatus.NoChange;
        }
        this.units = units;
        return UpdateStatus.Success;
    }

}
