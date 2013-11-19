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

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.clipping.ClippingManager;
import org.scilab.forge.scirenderer.implementation.jogl.clipping.JoGLClippingManager;
import org.scilab.forge.scirenderer.implementation.jogl.drawer.JoGLShapeDrawer;
import org.scilab.forge.scirenderer.implementation.jogl.lightning.JoGLLightManager;
import org.scilab.forge.scirenderer.lightning.LightManager;
import org.scilab.forge.scirenderer.renderer.Renderer;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.tranformations.TransformationManager;
import org.scilab.forge.scirenderer.tranformations.TransformationManagerImpl;
import org.scilab.forge.scirenderer.tranformations.TransformationManagerListener;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

import javax.media.opengl.GL2;

/**
 *
 * JoGl implementation of the DrawingTools.
 *
 * @author Pierre Lando
 */
public class JoGLDrawingTools implements DrawingTools {

    private final JoGLCapacity capacity = new JoGLCapacity();
    private final TransformationManager transformationManager;
    private final JoGLLightManager lightManager;
    private final JoGLClippingManager clippingManager;
    private final JoGLCanvas canvas;
    private GL2 gl;

    /**
     * Default constructor.
     * @param canvas the canvas where this drawing tools live.
     */
    JoGLDrawingTools(JoGLCanvas canvas) {
        this.transformationManager = new TransformationManagerImpl(canvas);
        this.lightManager = new JoGLLightManager(this);
        this.clippingManager = new JoGLClippingManager(this);

        this.canvas = canvas;

        transformationManager.addListener(new TransformationManagerListener() {
            @Override
            public void transformationChanged(TransformationManager transformationManager) {
                gl.glMatrixMode(GL2.GL_MODELVIEW);
                if (transformationManager.isUsingSceneCoordinate()) {
                    gl.glLoadMatrixd(transformationManager.getTransformation().getMatrix(), 0);
                } else {
                    gl.glLoadMatrixd(transformationManager.getWindowTransformation().getMatrix(), 0);
                }
            }
        });
    }

    /**
     * Synchronise to the given OpenGl context.
     * @param gl the OpenGL context.
     */
    void glSynchronize(GL2 gl) {
        this.gl = gl;
        transformationManager.reset();
        capacity.glReload(gl);
        lightManager.reload();
        clippingManager.reload();
    }

    @Override
    public JoGLCanvas getCanvas() {
        return canvas;
    }

    /**
     * Return the OpenGl context.
     * @return the OpenGl context.
     */
    public GL2 getGl() {
        return gl;
    }

    /**
     * Return the OpenGl capacity of this canvas.
     * @return the OpenGl capacity of this canvas.
     */
    public JoGLCapacity getGLCapacity() {
        return capacity;
    }

    @Override
    public TransformationManager getTransformationManager() {
        return transformationManager;
    }

    @Override
    public LightManager getLightManager() {
        return lightManager;
    }

    @Override
    public ClippingManager getClippingManager() {
        return clippingManager;
    }

    @Override
    public void clear(Color color) {
        gl.glClearColor(color.getRedAsFloat(), color.getGreenAsFloat(), color.getBlueAsFloat(), 1.f);
        gl.glClear(GL2.GL_COLOR_BUFFER_BIT);
    }

    @Override
    public void clear(java.awt.Color color) {
        float[] colorData = color.getRGBColorComponents(null);
        gl.glClearColor(colorData[0], colorData[1], colorData[2], 1f);
        gl.glClear(GL2.GL_COLOR_BUFFER_BIT);
    }

    @Override
    public void clearDepthBuffer() {
        gl.glClear(GL2.GL_DEPTH_BUFFER_BIT);
    }

    @Override
    public void draw(Renderer renderer) {
        canvas.getRendererManager().draw(this, renderer);
    }

    @Override
    public void draw(Geometry geometry) throws SciRendererException {
        JoGLShapeDrawer.getDrawer().draw(this, geometry, Appearance.getDefault());
    }

    @Override
    public void draw(Geometry geometry, Appearance appearance) throws SciRendererException {
        JoGLShapeDrawer.getDrawer().draw(this, geometry, appearance);
    }

    @Override
    public void draw(Texture texture) throws SciRendererException {
        canvas.getTextureManager().draw(this, texture);
    }

    @Override
    public void draw(Texture texture, AnchorPosition anchor, ElementsBuffer positions) throws SciRendererException {
        canvas.getTextureManager().draw(this, texture, anchor, positions, 0, 1, 0);
    }

    @Override
    public void draw(Texture texture, AnchorPosition anchor, ElementsBuffer positions, int offset, int stride, double rotationAngle) throws SciRendererException {
        canvas.getTextureManager().draw(this, texture, anchor, positions, offset, stride, rotationAngle);
    }

    @Override
    public void draw(Texture texture, AnchorPosition anchor, Vector3d position) throws SciRendererException {
        canvas.getTextureManager().draw(this, texture, anchor, position, 0);
    }

    @Override
    public void draw(Texture texture, AnchorPosition anchor, Vector3d position, double rotationAngle) throws SciRendererException {
        canvas.getTextureManager().draw(this, texture, anchor, position, rotationAngle);
    }

    /**
     * Bind the given texture to the OpenGl context.
     * @param texture the given texture.
     * @throws SciRendererException is thrown if the texture is invalid.
     */
    public void bind(Texture texture) throws SciRendererException {
        canvas.getTextureManager().bind(this, texture);
    }
}
