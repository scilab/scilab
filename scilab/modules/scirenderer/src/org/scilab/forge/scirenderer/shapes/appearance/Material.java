/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro SOUZA
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

package org.scilab.forge.scirenderer.shapes.appearance;

/**
 * Material class used for lighting
 * @author Pedro SOUZA
 */
public class Material {

    /** enable use color as material */
    private boolean colorMaterial = true;
    /** ambient color */
    private Color ambient;
    /** diffuse color*/
    private Color diffuse;
    /** specular color */
    private Color specular;
    /** shininess level */
    private float shininess;

    /**
     * @param the new ambient color;.
     */
    public void setAmbientColor(Color color) {
        ambient = color;
    }

    /**
     * @param the new diffuse color.
     */
    public void setDiffuseColor(Color color) {
        diffuse = color;
    }

    /**
     * @param the new specular color.
     */
    public void setSpecularColor(Color color) {
        specular = color;
    }

    /**
     * @param the new shinines.
     */
    public void setShininess(float s) {
        shininess = s;
    }

    /**
     * @return the ambient color.
     */
    public Color getAmbientColor() {
        return ambient;
    }

    /**
     * @return the diffuse color.
     */
    public Color getDiffuseColor() {
        return diffuse;
    }

    /**
     * @return the specular color.
     */
    public Color getSpecularColor() {
        return specular;
    }

    /**
     * @return the shininess.
     */
    public float getShininess() {
        return shininess;
    }

    /**
     * @return the color material status.
     */
    public boolean isColorMaterialEnable() {
        return colorMaterial;
    }

    /**
     * @param the new color material status.
     */
    public void setColorMaterialEnable(boolean status) {
        colorMaterial = status;
    }
}
