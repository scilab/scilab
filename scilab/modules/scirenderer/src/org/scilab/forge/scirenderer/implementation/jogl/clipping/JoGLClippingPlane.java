/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.implementation.jogl.clipping;

import org.scilab.forge.scirenderer.clipping.ClippingPlane;
import org.scilab.forge.scirenderer.implementation.jogl.utils.GLShortCuts;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.Vector4d;

import javax.media.opengl.GL2;

/**
 * @author Pierre Lando
 */
public class JoGLClippingPlane implements ClippingPlane {

    /**
     * GL index of this clipping plane.
     */
    private final int index;

    /**
     * Current GL context.
     */
    private GL2 gl;

    /**
     * Clipping plane enabled status.
     */
    private boolean isEnable;

    /**
     * Clipping plane equation look like: {@code x*a + y*b + z*c + d = 0}.
     * Where {@code equation} is {@code [a, b, c, d]}.
     */
    private Vector4d equation = new Vector4d(0, 0, 0, 0);
    private Transformation transformation = TransformationFactory.getIdentity();

    /**
     * Default constructor.
     * @param gl the OpenGl context.
     * @param index the id of this clipping plane.
     */
    public JoGLClippingPlane(GL2 gl, int index) {
        this.isEnable = false;
        this.index = index;
        reload(gl);
    }

    @Override
    public boolean isEnable() {
        return isEnable;
    }

    @Override
    public void setEnable(boolean isEnable) {
        this.isEnable = isEnable;
        GLShortCuts.setEnable(gl, GL2.GL_CLIP_PLANE0 + getIndex(), isEnable);
    }

    @Override
    public void setEquation(Vector4d v) {
        equation = v;
        setGLEquation();
    }

    @Override
    public Vector4d getEquation() {
        return equation;
    }

    @Override
    public void setTransformation(Transformation transformation) {
        this.transformation = transformation;
        setGLEquation();
    }

    @Override
    public Transformation getTransformation() {
        return transformation;
    }

    @Override
    public int getIndex() {
        return index;
    }

    /**
     * Set GL context and synchronise it.
     * @param gl the current gl context.
     */
    void reload(GL2 gl) {
        this.gl = gl;
        GLShortCuts.setEnable(gl, GL2.GL_CLIP_PLANE0 + getIndex(), isEnable());
        setGLEquation();
    }

    /**
     * Set the GL equation.
     */
    private void setGLEquation() {
        // We use global coordinate. So we need to load identity.
        gl.glMatrixMode(GL2.GL_MODELVIEW);
        gl.glPushMatrix();
        if (transformation == null) {
            gl.glLoadIdentity();
        } else {
            gl.glLoadMatrixd(transformation.getMatrix(), 0);
        }
        gl.glClipPlane(GL2.GL_CLIP_PLANE0 + getIndex(), equation.getData(), 0);
        gl.glPopMatrix();
    }
}
