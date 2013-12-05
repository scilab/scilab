/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro SOUZA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
