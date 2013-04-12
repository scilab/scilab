/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.forge.scirenderer.implementation.jogl.texture;

import com.jogamp.opengl.util.awt.TextureRenderer;
import com.jogamp.opengl.util.texture.TextureCoords;

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLCanvas;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLDrawingTools;
import org.scilab.forge.scirenderer.texture.AbstractTexture;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureManager;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationManager;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

import javax.media.opengl.GLContext;

import javax.media.opengl.GL2;
import java.awt.AlphaComposite;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.nio.FloatBuffer;
import java.util.Collection;
import java.util.HashSet;
import java.util.Set;

/**
 * @author Pierre Lando
 */
public class JoGLTextureManager implements TextureManager {

    private final Set<JoGLTexture> allTextures = new HashSet<JoGLTexture>();
    private final Set<JoGLTexture> toDispose = new HashSet<JoGLTexture>();
    JoGLCanvas canvas;

    public JoGLTextureManager(JoGLCanvas canvas) {
        this.canvas = canvas;
    }

    /**
     * Texture binder.
     * Bind the given texture to the given OpenGl context.
     * @param drawingTools drawing tools.
     * @param texture given texture.
     * @throws org.scilab.forge.scirenderer.SciRendererException if the texture can't be bind.
     */
    public void bind(JoGLDrawingTools drawingTools, Texture texture) throws SciRendererException {
        if ((texture instanceof JoGLTexture) && (allTextures.contains((JoGLTexture) texture))) {
            ((JoGLTexture) texture).bind(drawingTools);
        }
    }

    /**
     * Draw the given texture.
     * @param drawingTools used drawing tools.
     * @param texture the texture too drawn.
     * @throws org.scilab.forge.scirenderer.SciRendererException if the texture is invalid.
     */
    public void draw(JoGLDrawingTools drawingTools, Texture texture) throws SciRendererException {
        if ((texture instanceof JoGLTexture) && (allTextures.contains((JoGLTexture) texture))) {
            final JoGLTexture jt = (JoGLTexture) texture;
            jt.preDraw(drawingTools);
            jt.draw(drawingTools);
            jt.postDraw(drawingTools);
        }
    }

    public void draw(JoGLDrawingTools drawingTools, Texture texture, AnchorPosition anchor, ElementsBuffer positions, double rotationAngle) throws SciRendererException {
        if ((texture instanceof JoGLTexture) && (allTextures.contains((JoGLTexture) texture))) {
            if (positions != null) {
                FloatBuffer data = positions.getData();
                if (data != null) {
                    data.rewind();
                    float[] position = {0, 0, 0, 1};
                    final JoGLTexture jt = (JoGLTexture) texture;
                    jt.preDraw(drawingTools);
                    while (data.remaining() >= 4) {
                        data.get(position);
                        jt.draw(drawingTools, anchor, new Vector3d(position), rotationAngle);
                    }
                    jt.postDraw(drawingTools);
                }
            }
        }
    }

    public void draw(JoGLDrawingTools drawingTools, Texture texture, AnchorPosition anchor, Vector3d position, double rotationAngle) throws SciRendererException {
        if ((texture instanceof JoGLTexture) && (allTextures.contains((JoGLTexture) texture))) {
            final JoGLTexture jt = (JoGLTexture) texture;
            jt.preDraw(drawingTools);
            jt.draw(drawingTools, anchor, position, rotationAngle);
            jt.postDraw(drawingTools);
        }
    }

    /** Called when gl context is gone. */
    public void glReload() {
        for (JoGLTexture texture : allTextures) {
            texture.glReload();
        }
    }

    @Override
    public Texture createTexture() {
        JoGLTexture texture = new JoGLTexture();
        allTextures.add(texture);
        return texture;
    }

    @Override
    public void dispose(Collection<Texture> textures) {
        for (Texture texture : textures) {
            dispose(texture);
        }
    }

    @Override
    public void dispose(Texture texture) {
        if ((texture instanceof JoGLTexture) && (allTextures.contains((JoGLTexture) texture))) {
            allTextures.remove((JoGLTexture) texture);
            synchronized (toDispose) {
                toDispose.add((JoGLTexture) texture);
            }
        }
    }

    /**
     * Inner class for {@link Texture} implementation.
     */
    public class JoGLTexture extends AbstractTexture implements Texture {
        private com.jogamp.opengl.util.texture.Texture[] textures;
        private int wCuts;
        private int hCuts;
        private double sfactor = 1;
        private double tfactor = 1;

        /**
         * Default constructor.
         */
        public JoGLTexture() {
        }

        /**
         * Bind the texture in the OpenGl context.
         * @param drawingTools current drawing tools.
         * @throws SciRendererException if the texture is invalid.
         */
        public synchronized void bind(JoGLDrawingTools drawingTools) throws SciRendererException {
            GL2 gl = drawingTools.getGl().getGL2();
            if (isValid()) {
                checkData(drawingTools);
                if (textures.length == 1) {
                    gl.glEnable(GL2.GL_TEXTURE_2D);
                    gl.glEnable(GL2.GL_BLEND);
                    gl.glBlendFunc(GL2.GL_ONE, GL2.GL_ONE_MINUS_SRC_ALPHA);
                    gl.glTexEnvi(GL2.GL_TEXTURE_ENV, GL2.GL_TEXTURE_ENV_MODE, GL2.GL_REPLACE);

                    textures[0].setTexParameteri(gl, GL2.GL_TEXTURE_MAG_FILTER, getAsGLFilter(getMagnificationFilter(), false));
                    textures[0].setTexParameteri(gl, GL2.GL_TEXTURE_MIN_FILTER, getAsGLFilter(getMinifyingFilter(), false));
                    if (gl.isNPOTTextureAvailable()) {
                        textures[0].setTexParameteri(gl, GL2.GL_TEXTURE_WRAP_S, getAsGLWrappingMode(getSWrappingMode()));
                        textures[0].setTexParameteri(gl, GL2.GL_TEXTURE_WRAP_T, getAsGLWrappingMode(getTWrappingMode()));
                    } else {
                        textures[0].setTexParameteri(gl, GL2.GL_TEXTURE_WRAP_S, GL2.GL_CLAMP_TO_EDGE);
                        textures[0].setTexParameteri(gl, GL2.GL_TEXTURE_WRAP_T, GL2.GL_CLAMP_TO_EDGE);
                    }
                    gl.glTexEnvi(GL2.GL_TEXTURE_ENV, GL2.GL_TEXTURE_ENV_MODE, GL2.GL_REPLACE);
                    textures[0].bind(gl);

                    /* sfactor and tfactor are useful to have the correct texture coordinates when the texture
                       was transformed into a power-of-two texture */
                    Dimension textureSize = getDataProvider().getTextureSize();
                    sfactor = (double) textureSize.width / (double) textures[0].getWidth();
                    tfactor = (double) textureSize.height / (double) textures[0].getHeight();
                } else {
                    throw new SciRendererException("Texture is too large");
                }
            } else {
                throw new SciRendererException("Texture have no data.");
            }
        }

        /**
         * Check if the texture data are up to date.
         * @param drawingTools the drawing tools.
         * @throws SciRendererException if the texture is too big.
         */
        private synchronized void checkData(JoGLDrawingTools drawingTools) throws SciRendererException {
            synchronized (toDispose) {
                if (!toDispose.isEmpty()) {
                    final GL2 gl = drawingTools.getGl().getGL2();
                    for (JoGLTexture jt : toDispose) {
                        jt.releaseTextures(gl);
                    }
                    toDispose.clear();
                }
            }

            if (isValid() && !upToDate) {
                GL2 gl = drawingTools.getGl().getGL2();
                releaseTextures(gl);

                Dimension textureSize = getDataProvider().getTextureSize();
                int maxSize = drawingTools.getGLCapacity().getMaximumTextureSize();
                wCuts = (int) Math.ceil(textureSize.getWidth() / maxSize);
                hCuts = (int) Math.ceil(textureSize.getHeight() / maxSize);

                textures = new com.jogamp.opengl.util.texture.Texture[wCuts * hCuts];
                int k = 0;
                for (int i = 0; i < wCuts; i++) {
                    for (int j = 0; j < hCuts; j++) {
                        int x = i * maxSize;
                        int y = j * maxSize;
                        int width = getSubTextureSize((i == (wCuts - 1)), textureSize.width, maxSize);
                        int height = getSubTextureSize((j == (hCuts - 1)), textureSize.height, maxSize);

                        TextureRenderer renderer = new TextureRenderer(width, height, true, false);
                        Graphics2D g2d = renderer.createGraphics();
                        g2d.setComposite(AlphaComposite.Src);
                        g2d.drawImage(getDataProvider().getSubImage(x, y, width, height), null, 0, 0);
                        g2d.dispose();
                        renderer.markDirty(0, 0, width, height);
                        textures[k] = renderer.getTexture();

                        k++;
                    }
                }
                upToDate = true;
            }
        }

        private void releaseTextures(GL2 gl) {
            if (textures != null) {
                for (com.jogamp.opengl.util.texture.Texture texture : textures) {
                    texture.destroy(gl);
                }
                textures = null;
            }
        }

        public void preDraw(JoGLDrawingTools drawingTools) throws SciRendererException {
            checkData(drawingTools);
            final GL2 gl = drawingTools.getGl().getGL2();

            gl.glMatrixMode(GL2.GL_TEXTURE);
            gl.glPushMatrix();
            gl.glLoadIdentity();

            gl.glMatrixMode(GL2.GL_PROJECTION);
            gl.glPushMatrix();
            gl.glLoadIdentity();

            gl.glEnable(GL2.GL_TEXTURE_2D);

            gl.glEnable(GL2.GL_BLEND);
            gl.glBlendFunc(GL2.GL_ONE, GL2.GL_ONE_MINUS_SRC_ALPHA);

            gl.glEnable(GL2.GL_ALPHA_TEST);
            gl.glAlphaFunc(GL2.GL_GREATER, 0.0f);

            gl.glPushAttrib(GL2.GL_ALL_ATTRIB_BITS);

            gl.glTexEnvi(GL2.GL_TEXTURE_ENV, GL2.GL_TEXTURE_ENV_MODE, GL2.GL_REPLACE);

            for (int k = 0; k < wCuts * hCuts; k++) {
                textures[k].enable(gl);
                textures[k].setTexParameteri(gl, GL2.GL_TEXTURE_MAG_FILTER, getAsGLFilter(getMagnificationFilter(), false));
                textures[k].setTexParameteri(gl, GL2.GL_TEXTURE_MIN_FILTER, getAsGLFilter(getMinifyingFilter(), false));
                if (gl.isNPOTTextureAvailable()) {
                    textures[k].setTexParameteri(gl, GL2.GL_TEXTURE_WRAP_S, getAsGLWrappingMode(getSWrappingMode()));
                    textures[k].setTexParameteri(gl, GL2.GL_TEXTURE_WRAP_T, getAsGLWrappingMode(getTWrappingMode()));
                } else {
                    textures[k].setTexParameteri(gl, GL2.GL_TEXTURE_WRAP_S, GL2.GL_CLAMP_TO_EDGE);
                    textures[k].setTexParameteri(gl, GL2.GL_TEXTURE_WRAP_T, GL2.GL_CLAMP_TO_EDGE);
                }
                textures[k].bind(gl);
            }
        }

        public void draw(JoGLDrawingTools drawingTools, AnchorPosition anchor, Vector3d position, double rotationAngle) throws SciRendererException {
            TransformationManager transformationManager = drawingTools.getTransformationManager();
            Transformation canvasProjection = transformationManager.getCanvasProjection();
            boolean sceneCoordinate = drawingTools.getTransformationManager().isUsingSceneCoordinate();
            Dimension dimension = drawingTools.getCanvas().getDimension();
            Vector3d projected;
            final GL2 gl = drawingTools.getGl().getGL2();

            if (sceneCoordinate) {
                projected = canvasProjection.project(position);
            } else {
                projected = position;
            }

            gl.glMatrixMode(GL2.GL_MODELVIEW);
            gl.glPushMatrix();
            gl.glLoadIdentity();
            gl.glOrtho(0, dimension.width, 0,  dimension.height, 1, -1);

            if (rotationAngle == 0) {
                gl.glTranslated(Math.round(projected.getX() + getAnchorDeltaX(anchor)), Math.round(projected.getY() + getAnchorDeltaY(anchor)), projected.getZ());
            } else {
                gl.glTranslated(Math.round(projected.getX()), Math.round(projected.getY()), projected.getZ());
                gl.glRotated(rotationAngle, 0, 0, 1);
                gl.glTranslated(Math.round(getAnchorDeltaX(anchor)), Math.round(getAnchorDeltaY(anchor)), 0);
            }

            draw(drawingTools);

            gl.glMatrixMode(GL2.GL_MODELVIEW);
            gl.glPopMatrix();
        }

        public void postDraw(JoGLDrawingTools drawingTools) {
            final GL2 gl = drawingTools.getGl().getGL2();

            for (int k = 0; k < wCuts * hCuts; k++) {
                textures[k].disable(gl);
            }

            gl.glPopAttrib();
            gl.glDisable(GL2.GL_ALPHA_TEST);
            gl.glDisable(GL2.GL_TEXTURE_2D);
            gl.glDisable(GL2.GL_BLEND);

            gl.glMatrixMode(GL2.GL_PROJECTION);
            gl.glPopMatrix();
            gl.glMatrixMode(GL2.GL_TEXTURE);
            gl.glPopMatrix();
        }

        /**
         * Draw the texture in XY plane.
         * @param drawingTools the drawing tools.
         * @throws SciRendererException if the texture is invalid.
         */
        public void draw(JoGLDrawingTools drawingTools) throws SciRendererException {
            final int maxSize = drawingTools.getGLCapacity().getMaximumTextureSize();
            final Dimension textureSize = getDataProvider().getTextureSize();
            final GL2 gl = drawingTools.getGl().getGL2();

            if (textureSize != null) {
                int k = 0;
                for (int i = 0; i < wCuts; i++) {
                    for (int j = 0; j < hCuts; j++) {
                        final int x = i * maxSize;
                        final int y = j * maxSize;
                        final int width = getSubTextureSize((i == (wCuts - 1)), textureSize.width, maxSize);
                        final int height = getSubTextureSize((j == (hCuts - 1)), textureSize.height, maxSize);

                        // if the texture has been transformed in a power-of-two texture we need to have the correct coords.
                        final TextureCoords coords = textures[k].getSubImageTexCoords(0, 0, width, height);

                        gl.glBegin(GL2.GL_QUADS);
                        gl.glTexCoord2f(coords.left(), coords.bottom());
                        gl.glVertex2f(x, y);
                        gl.glTexCoord2f(coords.right(), coords.bottom());
                        gl.glVertex2f(x + width, y);
                        gl.glTexCoord2f(coords.right(), coords.top());
                        gl.glVertex2f(x + width, y + height);
                        gl.glTexCoord2f(coords.left(), coords.top());
                        gl.glVertex2f(x, y + height);
                        gl.glEnd();

                        k++;
                    }
                }
            }
        }

        @Override
        public double getSScaleFactor() {
            return sfactor;
        }

        @Override
        public double getTScaleFactor() {
            return tfactor;
        }

        /**
         * Compute the sub texture size.
         * @param lastPart true if this is the last part.
         * @param textureSize the global texture size.
         * @param maxSize the maximum sub-texture size.
         * @return the sub texture size.
         */
        private int getSubTextureSize(boolean lastPart, int textureSize, int maxSize) {
            if (lastPart) {
                int lastSize = textureSize % maxSize;
                if (lastSize == 0) {
                    return maxSize;
                } else {
                    return lastSize;
                }
            } else {
                return maxSize;
            }
        }

        private int getAsGLWrappingMode(Texture.Wrap wrappingMode) {
            switch (wrappingMode) {
                case CLAMP:
                    return GL2.GL_CLAMP_TO_EDGE;
                case REPEAT:
                    return GL2.GL_REPEAT;
                default:
                    return GL2.GL_REPEAT;
            }
        }

        private int getAsGLFilter(Texture.Filter filter, boolean mipmap) {
            int returnedValue;
            if (mipmap) {
                switch (filter) {
                    case LINEAR:
                        returnedValue = GL2.GL_LINEAR_MIPMAP_LINEAR;
                        break;
                    case NEAREST:
                        returnedValue = GL2.GL_NEAREST_MIPMAP_NEAREST;
                        break;
                    default:
                        returnedValue = GL2.GL_NEAREST;
                        break;
                }
            } else {
                switch (filter) {
                    case LINEAR:
                        returnedValue = GL2.GL_LINEAR;
                        break;
                    case NEAREST:
                        returnedValue = GL2.GL_NEAREST;
                        break;
                    default:
                        returnedValue = GL2.GL_NEAREST;
                        break;
                }
            }
            return returnedValue;
        }

        /** Called when gl context is gone. */
        public void glReload() {
            textures = null;
            upToDate = false;
        }

        /**
         * Return the deltaX to apply to the sprite in regards to the given anchor.
         * @param anchor the given anchor.
         * @return the deltaX to apply to the sprite in regards to the given anchor.
         */
        protected double getAnchorDeltaX(AnchorPosition anchor) {
            int spriteWidth = getDataProvider().getTextureSize().width;
            switch (anchor) {
                case LEFT:
                case LOWER_LEFT:
                case UPPER_LEFT:
                    return 0;
                case UP:
                case CENTER:
                case DOWN:
                    return -spriteWidth / 2f;
                case RIGHT:
                case LOWER_RIGHT:
                case UPPER_RIGHT:
                    return -spriteWidth;
                default:
                    return 0;
            }
        }

        /**
         * Return the deltaY to apply to the sprite in regards to the given anchor.
         * @param anchor the given anchor.
         * @return the deltaY to apply to the sprite in regards to the given anchor.
         */
        protected double getAnchorDeltaY(AnchorPosition anchor) {
            int spriteHeight = getDataProvider().getTextureSize().height;
            switch (anchor) {
                case UPPER_LEFT:
                case UP:
                case UPPER_RIGHT:
                    return -spriteHeight;
                case LEFT:
                case CENTER:
                case RIGHT:
                    return -spriteHeight / 2f;
                case LOWER_LEFT:
                case DOWN:
                case LOWER_RIGHT:
                    return 0;
                default:
                    return 0;
            }
        }
    }
}
