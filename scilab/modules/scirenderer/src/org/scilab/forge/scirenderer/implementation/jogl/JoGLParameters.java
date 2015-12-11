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

package org.scilab.forge.scirenderer.implementation.jogl;

import javax.media.opengl.GL;
import javax.media.opengl.glu.GLU;

/**
 * @author Pierre Lando
 */
public class JoGLParameters {

    private static final boolean USE_DISPLAY_LIST = false;
    private static final boolean USE_POINT_SPRITE = true;
    private static Boolean USE_VBO;

    /**
     * Default constructor.
     * It's package : only JoGLCanvas need this kind of object.
     */
    JoGLParameters() {
    }

    /**
     * Use display list.
     * @return true if use display list.
     */
    public boolean useDisplayList() {
        return USE_DISPLAY_LIST;
    }

    /**
     * Use VBO.
     * @return true if use VBO.
     */
    public boolean useVBO() {
        if (USE_VBO == null) {
            GL gl = GLU.getCurrentGL();
            USE_VBO = gl.isExtensionAvailable("GL_ARB_vertex_buffer_object")
                      && gl.isFunctionAvailable("glBindBufferARB")
                      && gl.isFunctionAvailable("glGenBuffersARB")
                      && gl.isFunctionAvailable("glBufferDataARB")
                      && gl.isFunctionAvailable("glDeleteBuffersARB");
        }

        return USE_VBO;
    }

    /**
     * Use point sprite.
     * @return true if use point sprite.
     */
    public boolean usePointSprite() {
        return USE_POINT_SPRITE;
    }
}
