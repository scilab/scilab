/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
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

package org.scilab.forge.scirenderer.implementation.jogl.lightning;

import javax.media.opengl.GL2;

import org.scilab.forge.scirenderer.implementation.jogl.utils.GLShortCuts;
import org.scilab.forge.scirenderer.lightning.Light;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

/**
 * @author Pierre Lando
 */
public class JoGLLight implements Light {
    private final int index;
    private GL2 gl;
    private boolean isEnable;
    private Color ambientColor = new Color(0, 0, 0);
    private Color diffuseColor = new Color(0, 0, 0);
    private Color specularColor = new Color(0, 0, 0);
    private Vector3d position = new Vector3d(0, 0, 0);
    private Vector3d spotDirection = new Vector3d(0, 0, 1);
    private Vector3d direction = new Vector3d(0, 0, 0);
    private float spotAngle = 180;
    private boolean isDirectional = false;

    /**
     * Default constructor.
     * @param gl the gl context.
     * @param index the light index.
     */
    public JoGLLight(GL2 gl, int index) {
        this.gl = gl;
        this.index = index;
    }

    /**
     * Reload this light.
     * @param gl the gl context.
     */
    public void reload(GL2 gl) {
        this.gl = gl;

        GLShortCuts.setEnable(gl, GL2.GL_LIGHT0 + index, isEnable);
        gl.glLightfv(GL2.GL_LIGHT0 + index, GL2.GL_AMBIENT, ambientColor.getRGBComponents(null), 0);
        gl.glLightfv(GL2.GL_LIGHT0 + index, GL2.GL_DIFFUSE, diffuseColor.getRGBComponents(null), 0);
        gl.glLightfv(GL2.GL_LIGHT0 + index, GL2.GL_SPECULAR, specularColor.getRGBComponents(null), 0);
        if (isDirectional) {
            float[] pos = position.getDataAsFloatArray(4);
            pos[3] = 0.0f;
            pos[0] = -pos[0];
            pos[1] = -pos[1];
            pos[2] = -pos[2];
            gl.glLightfv(GL2.GL_LIGHT0 + index, GL2.GL_POSITION, pos, 0);
        } else {
            float[] pos = position.getDataAsFloatArray(4);
            pos[3] = 1.0f;
            gl.glLightfv(GL2.GL_LIGHT0 + index, GL2.GL_POSITION, pos, 0);
        }
        gl.glLightfv(GL2.GL_LIGHT0 + index, GL2.GL_SPOT_DIRECTION, spotDirection.getDataAsFloatArray(4), 0);
    }

    @Override
    public boolean isEnable() {
        return isEnable;
    }

    @Override
    public void setEnable(boolean enable) {
        if (enable != isEnable) {
            isEnable = enable;
            GLShortCuts.setEnable(gl, GL2.GL_LIGHT0 + index, isEnable);
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
            gl.glLightfv(GL2.GL_LIGHT0 + index, GL2.GL_AMBIENT, ambientColor.getRGBComponents(null), 0);
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
            gl.glLightfv(GL2.GL_LIGHT0 + index, GL2.GL_DIFFUSE, diffuseColor.getRGBComponents(null), 0);
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
            gl.glLightfv(GL2.GL_LIGHT0 + index, GL2.GL_SPECULAR, specularColor.getRGBComponents(null), 0);
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
            float[] pos = position.getDataAsFloatArray(4);
            pos[3] = 1.0f;
            gl.glLightfv(GL2.GL_LIGHT0 + index, GL2.GL_POSITION, pos, 0);
        }
    }

    public Vector3d getDirection() {
        return direction;
    }

    public void setDirection(Vector3d direction) {
        if (direction != null) {
            isDirectional = true;
            this.direction = direction;
            float[] dir = direction.getDataAsFloatArray(4);
            dir[3] = 0.0f;
            dir[0] = -dir[0];
            dir[1] = -dir[1];
            dir[2] = -dir[2];
            gl.glLightfv(GL2.GL_LIGHT0 + index, GL2.GL_POSITION, dir, 0);
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
            gl.glLightfv(GL2.GL_LIGHT0 + index, GL2.GL_SPOT_DIRECTION, spotDirection.getNormalized().getDataAsFloatArray(4), 0);
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
            gl.glLightf(GL2.GL_LIGHT0 + index, GL2.GL_SPOT_CUTOFF, spotAngle);
        }
    }

    @Override
    public int getIndex() {
        return index;
    }
}
