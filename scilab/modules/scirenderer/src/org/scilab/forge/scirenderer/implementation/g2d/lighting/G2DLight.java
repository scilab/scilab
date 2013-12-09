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

package org.scilab.forge.scirenderer.implementation.g2d.lighting;


import org.scilab.forge.scirenderer.lightning.Light;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

/**
 * @author Pedro SOUZA
 */
public class G2DLight implements Light {

    private int index;
    private boolean isEnable;
    private Color ambientColor = new Color(0, 0, 0);
    private Color diffuseColor = new Color(0, 0, 0);
    private Color specularColor = new Color(0, 0, 0);
    private Vector3d position = new Vector3d(0, 0, 0);
    private Vector3d spotDirection = new Vector3d(0, 0, -1);
    private Vector3d direction = new Vector3d(0, 0, 0);
    private float spotAngle = 180;
    private boolean isDirectional = false;


    public G2DLight(int index) {
        this.index = index;
    }

    @Override
    public boolean isEnable() {
        return isEnable;
    }

    @Override
    public void setEnable(boolean enable) {
        if (enable != isEnable) {
            isEnable = enable;
        }
    }

    @Override
    public Color getAmbientColor() {
        return ambientColor;
    }

    @Override
    public void setAmbientColor(Color color) {
        if (color != null) {
            ambientColor = color;
        }
    }

    @Override
    public Color getDiffuseColor() {
        return diffuseColor;
    }

    @Override
    public void setDiffuseColor(Color color) {
        if (color != null) {
            diffuseColor = color;
        }
    }

    @Override
    public Color getSpecularColor() {
        return specularColor;
    }

    @Override
    public void setSpecularColor(Color color) {
        if (color != null) {
            specularColor = color;
        }
    }

    @Override
    public Vector3d getPosition() {
        return position;
    }

    @Override
    public void setPosition(Vector3d position) {
        if (position != null) {
            isDirectional = false;
            this.position = position;
        }
    }

    public Vector3d getDirection() {
        return direction;
    }

    public void setDirection(Vector3d direction) {
        if (direction != null) {
            isDirectional = true;
            this.direction = direction.getNormalized();
        }
    }

    @Override
    public Vector3d getSpotDirection() {
        return spotDirection;
    }

    @Override
    public void setSpotDirection(Vector3d spotDirection) {
        if (spotDirection != null) {
            this.spotDirection = spotDirection;
        }
    }

    @Override
    public float getSpotAngle() {
        return spotAngle;
    }

    @Override
    public void setSpotAngle(float angle) {
        if (angle != spotAngle) {
            spotAngle = angle;
        }
    }

    @Override
    public int getIndex() {
        return index;
    }

    public boolean isPoint() {
        return !isDirectional;
    }
}
