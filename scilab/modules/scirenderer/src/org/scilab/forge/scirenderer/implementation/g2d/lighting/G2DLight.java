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
