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

package org.scilab.modules.graphic_objects.surface;

import org.scilab.modules.graphic_objects.contouredObject.ClippableContouredObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;
import org.scilab.modules.graphic_objects.lighting.ColorTriplet;
import org.scilab.modules.graphic_objects.lighting.Material;

/**
 * Surface class
 * @author Manuel JULIACHS
 */
public abstract class Surface extends ClippableContouredObject {
    // TBD Data -> Data Model
    /* TBD: properties relative to the data model */
    /** Surface properties names */
    private enum SurfaceProperty { SURFACEMODE, COLORMODE, COLORFLAG, HIDDENCOLOR };

    /** Specifies whether the surface is drawn or not */
    private boolean surfaceMode;

    /** Specifies the front face color (when colorFlag == 0) */
    private int colorMode;

    /** Specifies how facet colors are computed */
    private int colorFlag;

    /** Back-facing facets color */
    private int hiddenColor;

    /** material properties used for lighting */
    private Material material;

    /** Constructor */
    public Surface() {
        super();
        surfaceMode = false;
        colorMode = 0;
        colorFlag = 0;
        hiddenColor = 0;
        material = new Material();
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(int propertyName) {
        switch (propertyName) {
            case __GO_SURFACE_MODE__ :
                return SurfaceProperty.SURFACEMODE;
            case __GO_COLOR_MODE__ :
                return SurfaceProperty.COLORMODE;
            case __GO_COLOR_FLAG__ :
                return SurfaceProperty.COLORFLAG;
            case __GO_HIDDEN_COLOR__ :
                return SurfaceProperty.HIDDENCOLOR;
            case __GO_AMBIENTCOLOR__ :
                return ColorTriplet.ColorTripletProperty.AMBIENTCOLOR;
            case __GO_DIFFUSECOLOR__ :
                return ColorTriplet.ColorTripletProperty.DIFFUSECOLOR;
            case __GO_SPECULARCOLOR__ :
                return ColorTriplet.ColorTripletProperty.SPECULARCOLOR;
            case __GO_COLOR_MATERIAL__ :
                return Material.MaterialProperty.COLOR_MATERIAL;
            case __GO_MATERIAL_SHININESS__ :
                return Material.MaterialProperty.SHININESS;
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
        if (property instanceof SurfaceProperty) {
            SurfaceProperty sp = (SurfaceProperty)property;
            switch (sp) {
                case SURFACEMODE:
                    return getSurfaceMode();
                case COLORMODE:
                    return getColorMode();
                case COLORFLAG:
                    return getColorFlag();
                case HIDDENCOLOR:
                    return getHiddenColor();
            }
        } else if (property instanceof Material.MaterialProperty) {
            Material.MaterialProperty mp = (Material.MaterialProperty)property;
            switch (mp) {
                case COLOR_MATERIAL:
                    return getColorMaterialMode();
                case SHININESS:
                    return getMaterialShininess();
            }
        } else if (property instanceof ColorTriplet.ColorTripletProperty) {
            ColorTriplet.ColorTripletProperty cp = (ColorTriplet.ColorTripletProperty)property;
            switch (cp) {
                case AMBIENTCOLOR:
                    return getMaterialAmbientColor();
                case DIFFUSECOLOR:
                    return getMaterialDiffuseColor();
                case SPECULARCOLOR:
                    return getMaterialSpecularColor();
            }
        }
        return super.getProperty(property);
    }

    /**
     * Fast property set method
     * @param property the property to set
     * @param value the property value
     * @return true if the property has been set, false otherwise
     */
    public UpdateStatus setProperty(Object property, Object value) {
        if (property instanceof SurfaceProperty) {
            SurfaceProperty sp = (SurfaceProperty)property;
            switch (sp) {
                case SURFACEMODE:
                    setSurfaceMode((Boolean) value);
                    break;
                case COLORMODE:
                    setColorMode((Integer) value);
                    break;
                case COLORFLAG:
                    setColorFlag((Integer) value);
                    break;
                case HIDDENCOLOR:
                    setHiddenColor((Integer) value);
                    break;
            }
        } else if (property instanceof Material.MaterialProperty) {
            Material.MaterialProperty mp = (Material.MaterialProperty)property;
            switch (mp) {
                case COLOR_MATERIAL:
                    return setColorMaterialMode((Boolean)value);
                case SHININESS:
                    return setMaterialShininess((Double)value);
            }
        } else if (property instanceof ColorTriplet.ColorTripletProperty) {
            ColorTriplet.ColorTripletProperty cp = (ColorTriplet.ColorTripletProperty)property;
            switch (cp) {
                case AMBIENTCOLOR:
                    return setMaterialAmbientColor((Double[])value);
                case DIFFUSECOLOR:
                    return setMaterialDiffuseColor((Double[])value);
                case SPECULARCOLOR:
                    return setMaterialSpecularColor((Double[])value);
            }
        } else {
            return super.setProperty(property, value);
        }
        return UpdateStatus.Success;
    }


    /**
     * @return the surfaceMode
     */
    public Boolean getSurfaceMode() {
        return surfaceMode;
    }

    /**
     * @param surfaceMode the surfaceMode to set
     */
    public UpdateStatus setSurfaceMode(Boolean surfaceMode) {
        this.surfaceMode = surfaceMode;
        return UpdateStatus.Success;
    }

    /**
     * @return the colorMode
     */
    public Integer getColorMode() {
        return colorMode;
    }

    /**
     * @param colorMode the colorMode to set
     */
    public UpdateStatus setColorMode(Integer colorMode) {
        this.colorMode = colorMode;
        return UpdateStatus.Success;
    }

    /**
     * @return the colorFlag
     */
    public Integer getColorFlag() {
        return colorFlag;
    }

    /**
     * @param colorFlag the colorFlag to set
     */
    public UpdateStatus setColorFlag(Integer colorFlag) {
        this.colorFlag = colorFlag;
        return UpdateStatus.Success;
    }

    /**
     * @return the hiddenColor
     */
    public Integer getHiddenColor() {
        return hiddenColor;
    }

    /**
     * @param hiddenColor the hiddenColor to set
     */
    public UpdateStatus setHiddenColor(Integer hiddenColor) {
        this.hiddenColor = hiddenColor;
        return UpdateStatus.Success;
    }

    /**
     * Sets the color-material mode, that is,
     * use the surface color as diffuse color
     * @param status if true enables color-material
     */
    public UpdateStatus setColorMaterialMode(Boolean status) {
        return material.setColorMaterialMode(status);
    }

    /**
     * @return if color-material is enabled.
     */
    public Boolean getColorMaterialMode() {
        return material.getColorMaterialMode();
    }

    /**
     * @return the ambient color of the material.
     */
    public Double[] getMaterialAmbientColor() {
        return material.getAmbientColor();
    }

    /**
     * @param the new ambient color of the material.
     */
    public UpdateStatus setMaterialAmbientColor(Double[] color) {
        return material.setAmbientColor(color);
    }

    /**
     * @return the diffuse color of the material.
     */
    public Double[] getMaterialDiffuseColor() {
        return material.getDiffuseColor();
    }

    /**
     * @param the new diffuse color of the material.
     */
    public UpdateStatus setMaterialDiffuseColor(Double[] color) {
        return material.setDiffuseColor(color);
    }

    /**
     * @return the specular color of the material.
     */
    public Double[] getMaterialSpecularColor() {
        return material.getSpecularColor();
    }

    /**
     * @param the new specular color of the material.
     */
    public UpdateStatus setMaterialSpecularColor(Double[] color) {
        return material.setSpecularColor(color);
    }

    /**
     * @return the shininess level of the material.
     */
    public Double getMaterialShininess() {
        return material.getShininess();
    }

    /**
     * @param the new shininess level of the material.
     */
    public UpdateStatus setMaterialShininess(Double s) {
        return material.setShininess(s);
    }

    /**
     * @return the material.
     */
    public Material getMaterial() {
        return material;
    }

    /**
     * @param the new material.
     */
    public UpdateStatus setMaterial(Material material) {
        if (material != null) {
            this.material = material;
        }
        return UpdateStatus.Success;
    }
}
