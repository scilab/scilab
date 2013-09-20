/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.implementation.jogl.texture;

import com.jogamp.opengl.util.texture.TextureIO;
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
import org.scilab.forge.scirenderer.texture.TextureDataProvider;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationManager;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

import javax.media.opengl.GLContext;

import javax.media.opengl.GL;
import javax.media.opengl.GL2;
import javax.media.opengl.GL2ES1;
import javax.media.opengl.GL2GL3;
import javax.media.opengl.GLProfile;
import java.awt.AlphaComposite;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.Collection;
import java.util.HashSet;
import java.util.Set;

/**
 * @author Pierre Lando
 */
public class JoGLTextureManager implements TextureManager {

    private final Set<JoGLTexture> allTextures = new HashSet<JoGLTexture>();
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
            if (jt.preDraw(drawingTools)) {
                jt.draw(drawingTools);
                jt.postDraw(drawingTools);
            }
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
                    if (jt.preDraw(drawingTools)) {
                        while (data.remaining() >= 4) {
                            data.get(position);
                            jt.draw(drawingTools, anchor, new Vector3d(position), rotationAngle);
                        }
                        jt.postDraw(drawingTools);
                    }
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
            ((JoGLTexture) texture).dispose();
        }
    }

    /**
     * Inner class for {@link Texture} implementation.
     */
    public class JoGLTexture extends AbstractTexture implements Texture {
        private com.jogamp.opengl.util.texture.Texture[] textures;
        private JoGLTextureData[] textureData;
        private int wCuts;
        private int hCuts;
        private double sfactor = 1;
        private double tfactor = 1;
        private ByteBuffer buffer;
        private TextureDataProvider.ImageType previousType;
        private JoGLDrawingTools drawingTools;

        /**
         * Default constructor.
         */
        public JoGLTexture() {
        }

        public final boolean isRowMajorOrder() {
            return getDataProvider().isRowMajorOrder();
        }

        /**
         * Bind the texture in the OpenGl context.
         * @param drawingTools current drawing tools.
         * @throws SciRendererException if the texture is invalid.
         */
        public synchronized void bind(JoGLDrawingTools drawingTools) throws SciRendererException {
            if (this.drawingTools == null) {
                this.drawingTools = this.drawingTools;
            }

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
            if (isValid() && !upToDate) {
                GL2 gl = drawingTools.getGl().getGL2();

                Dimension textureSize = getDataProvider().getTextureSize();
                int maxSize = drawingTools.getGLCapacity().getMaximumTextureSize();
                wCuts = (int) Math.ceil(textureSize.getWidth() / maxSize);
                hCuts = (int) Math.ceil(textureSize.getHeight() / maxSize);
                ByteBuffer newBuffer = getDataProvider().getData();
                TextureDataProvider.ImageType newType = getDataProvider().getImageType();

                if (newBuffer != null && textureSize.width != 0 && textureSize.height != 0) {
                    boolean hasChanged = false;
                    if (newBuffer != buffer || newType != previousType) {
                        releaseTextures(gl);
                        if (newBuffer != null) {
                            textures = new com.jogamp.opengl.util.texture.Texture[wCuts * hCuts];
                            textureData = new JoGLTextureData[wCuts * hCuts];
                        }
                        buffer = newBuffer;
                        previousType = newType;
                        hasChanged = true;
                    }

                    if (newBuffer != null || buffer != null) {
                        if (wCuts == 1 && hCuts == 1) {
                            if (hasChanged) {
                                if (isRowMajorOrder()) {
                                    textureData[0] = new JoGLTextureData(gl.getGLProfile(), newType, buffer, textureSize.width, textureSize.height);
                                } else {
                                    textureData[0] = new JoGLTextureData(gl.getGLProfile(), newType, buffer, textureSize.height, textureSize.width);
                                }
                                try {
                                    textures[0] = TextureIO.newTexture(textureData[0]);
                                } catch (Exception e) {
                                    System.err.println(e);
                                }
                            } else {
                                try {
                                    textures[0].updateSubImage(gl, textureData[0], 0, 0, 0);
                                } catch (Exception e) {
                                    System.err.println(e);
                                }
                            }
                        } else {
                            int k = 0;
                            for (int i = 0; i < wCuts; i++) {
                                for (int j = 0; j < hCuts; j++) {
                                    if (hasChanged) {
                                        final int x = i * maxSize;
                                        final int y = j * maxSize;
                                        final int width = getSubTextureSize((i == (wCuts - 1)), textureSize.width, maxSize);
                                        final int height = getSubTextureSize((j == (hCuts - 1)), textureSize.height, maxSize);
                                        if (isRowMajorOrder()) {
                                            textureData[k] = new JoGLTextureData(gl.getGLProfile(), newType, buffer, width, height, x, y, textureSize.width, textureSize.height);
                                        } else {
                                            textureData[k] = new JoGLTextureData(gl.getGLProfile(), newType, buffer, height, width, y, x, textureSize.height, textureSize.width);
                                        }
                                        textures[k] = TextureIO.newTexture(textureData[k]);
                                    } else {
                                        textures[k].updateSubImage(gl, textureData[k], 0, 0, 0);
                                    }
                                    k++;
                                }
                            }
                        }
                    }
                }
                if (textures != null) {
                    upToDate = true;
                }
            }
        }

        private void releaseTextures(GL2 gl) {
            if (textures != null) {
                for (com.jogamp.opengl.util.texture.Texture texture : textures) {
                    if (texture != null) {
                        texture.destroy(gl);
                    }
                }
                textures = null;
            }

            if (textureData != null) {
                for (JoGLTextureData td : textureData) {
                    if (td != null) {
                        td.destroy();
                    }
                }
                textureData = null;
            }
        }

        public void dispose() {
            if (drawingTools != null) {
                releaseTextures(drawingTools.getGl().getGL2());
            }
        }

        public boolean preDraw(JoGLDrawingTools drawingTools) throws SciRendererException {
            checkData(drawingTools);

            if (textures == null) {
                return false;
            }

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
            }

            return true;
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

            if (textureSize != null && textures != null) {
                if (wCuts == 1 && hCuts == 1) {
                    final TextureCoords coords;
                    if (isRowMajorOrder()) {
                        coords = textures[0].getSubImageTexCoords(0, 0, textureSize.width, textureSize.height);
                    } else {
                        coords = textures[0].getSubImageTexCoords(0, 0, textureSize.height, textureSize.width);
                    }
                    textures[0].bind(gl);

                    gl.glBegin(GL2.GL_QUADS);
                    gl.glTexCoord2f(coords.left(), coords.bottom());
                    if (isRowMajorOrder()) {
                        gl.glVertex2f(0, 0);
                    } else {
                        gl.glVertex2f(textureSize.width, textureSize.height);
                    }
                    gl.glTexCoord2f(coords.right(), coords.bottom());
                    gl.glVertex2f(textureSize.width, 0);
                    gl.glTexCoord2f(coords.right(), coords.top());
                    if (isRowMajorOrder()) {
                        gl.glVertex2f(textureSize.width, textureSize.height);
                    } else {
                        gl.glVertex2f(0, 0);
                    }
                    gl.glTexCoord2f(coords.left(), coords.top());
                    gl.glVertex2f(0, textureSize.height);
                    gl.glEnd();
                } else {
                    int k = 0;
                    for (int i = 0; i < wCuts; i++) {
                        for (int j = 0; j < hCuts; j++) {
                            final int x = i * maxSize;
                            final int y = j * maxSize;
                            final int width = getSubTextureSize((i == (wCuts - 1)), textureSize.width, maxSize);
                            final int height = getSubTextureSize((j == (hCuts - 1)), textureSize.height, maxSize);

                            // if the texture has been transformed in a power-of-two texture we need to have the correct coords.
                            final TextureCoords coords;
                            if (isRowMajorOrder()) {
                                coords = textures[k].getSubImageTexCoords(0, 0, width, height);
                            } else {
                                coords = textures[k].getSubImageTexCoords(0, 0, height, width);
                            }
                            textures[k].bind(gl);

                            gl.glBegin(GL2.GL_QUADS);
                            gl.glTexCoord2f(coords.left(), coords.top());
                            gl.glVertex2f(x, textureSize.height - y);
                            gl.glTexCoord2f(coords.right(), coords.top());
                            if (isRowMajorOrder()) {
                                gl.glVertex2f(x + width, textureSize.height - y);
                            } else {
                                gl.glVertex2f(x, textureSize.height - y - height);
                            }
                            gl.glTexCoord2f(coords.right(), coords.bottom());
                            gl.glVertex2f(x + width, textureSize.height - y - height);
                            gl.glTexCoord2f(coords.left(), coords.bottom());
                            if (isRowMajorOrder()) {
                                gl.glVertex2f(x, textureSize.height - y - height);
                            } else {
                                gl.glVertex2f(x + width, textureSize.height - y);
                            }
                            gl.glEnd();

                            k++;
                        }
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
            buffer = null;
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

    private static class JoGLTextureData extends com.jogamp.opengl.util.texture.TextureData {

        private ByteBuffer data;
        private int shift;

        public JoGLTextureData(GLProfile glp, TextureDataProvider.ImageType type, ByteBuffer data, int width, int height, int x, int y, int totalWidth, int totalHeight) {
            super(glp);
            this.data = data;
            this.mipmap = false;
            this.width = width;
            this.height = height;
            this.mustFlipVertically = true;
            this.shift = ((x == 0 && y == 0) || data.capacity() == 0) ? 0 : (data.capacity() / (totalWidth * totalHeight) * (x + y * totalWidth));
            this.rowLength = totalWidth;

            switch (type) {
                case RGB:
                    this.internalFormat = GL2.GL_RGB;
                    setPixelFormat(GL.GL_RGB);
                    setPixelType(GL2.GL_UNSIGNED_BYTE);
                    this.alignment = 1;
                    break;
                case RGB_RGBA:
                    this.internalFormat = GL2.GL_RGB;
                    setPixelFormat(GL.GL_RGBA);
                    setPixelType(GL2.GL_UNSIGNED_INT_8_8_8_8);
                    this.alignment = 4;
                    break;
                case BGR:
                    this.internalFormat = GL2GL3.GL_RGB;
                    setPixelFormat(GL2GL3.GL_BGR);
                    setPixelType(GL.GL_UNSIGNED_BYTE);
                    this.alignment = 1;
                    break;
                case GRAY:
                    this.internalFormat = GL.GL_LUMINANCE;
                    setPixelFormat(GL.GL_LUMINANCE);
                    setPixelType(GL.GL_UNSIGNED_BYTE);
                    this.alignment = 1;
                    break;
                case GRAY_16:
                    this.internalFormat = GL2.GL_LUMINANCE16;
                    setPixelFormat(GL2.GL_LUMINANCE);
                    setPixelType(GL2.GL_UNSIGNED_SHORT);
                    this.alignment = 2;
                    break;
                case RGBA:
                    this.internalFormat = GL2.GL_RGBA;
                    setPixelFormat(GL2.GL_RGBA);
                    setPixelType(GL2.GL_UNSIGNED_INT_8_8_8_8);
                    this.alignment = 4;
                    break;
                case RGBA_REV:
                    this.internalFormat = GL2.GL_RGBA;
                    setPixelFormat(GL2.GL_RGBA);
                    setPixelType(GL2.GL_UNSIGNED_INT_8_8_8_8_REV);
                    this.alignment = 4;
                    break;
                case ABGR:
                    this.internalFormat = GL2.GL_RGBA;
                    setPixelFormat(GL2.GL_ABGR_EXT);
                    setPixelType(GL.GL_UNSIGNED_BYTE);
                    this.alignment = 1;
                    break;
                case RGB_332:
                    this.internalFormat = GL2.GL_R3_G3_B2;
                    setPixelFormat(GL2.GL_RGB);
                    setPixelType(GL2.GL_UNSIGNED_BYTE_3_3_2);
                    this.alignment = 1;
                    break;
                case RED:
                    this.internalFormat = GL2.GL_RED;
                    setPixelFormat(GL2.GL_RED);
                    setPixelType(GL2.GL_UNSIGNED_BYTE);
                    this.alignment = 1;
                    break;
                case GREEN:
                    this.internalFormat = GL2.GL_RGB8;
                    setPixelFormat(GL2.GL_GREEN);
                    setPixelType(GL2.GL_UNSIGNED_BYTE);
                    this.alignment = 1;
                    break;
                case BLUE:
                    this.internalFormat = GL2.GL_RGB8;
                    setPixelFormat(GL2.GL_BLUE);
                    setPixelType(GL2.GL_UNSIGNED_BYTE);
                    this.alignment = 1;
                    break;
                case INTENSITY:
                    this.internalFormat = GL2.GL_INTENSITY;
                    setPixelFormat(GL2.GL_LUMINANCE);
                    setPixelType(GL2.GL_UNSIGNED_BYTE);
                    this.alignment = 1;
                    break;
                case RGBA_4444:
                    this.internalFormat = GL2.GL_RGBA4;
                    setPixelFormat(GL2.GL_RGBA);
                    setPixelType(GL2.GL_UNSIGNED_SHORT_4_4_4_4);
                    this.alignment = 2;
                    break;
                case RGBA_5551:
                    this.internalFormat = GL2.GL_RGB5_A1;
                    setPixelFormat(GL2.GL_RGBA);
                    setPixelType(GL2.GL_UNSIGNED_SHORT_5_5_5_1);
                    this.alignment = 2;
                    break;
                case RGB_FLOAT:
                    this.internalFormat = GL2ES1.GL_RGB16F;
                    setPixelFormat(GL2.GL_RGB);
                    setPixelType(GL2.GL_FLOAT);
                    this.alignment = 4;
                    break;
                case RGBA_FLOAT:
                    this.internalFormat = GL2.GL_RGBA16F;
                    setPixelFormat(GL2.GL_RGBA);
                    setPixelType(GL2.GL_FLOAT);
                    this.alignment = 4;
                    break;
                case GRAY_FLOAT:
                    this.internalFormat = GL2.GL_LUMINANCE16F;
                    setPixelFormat(GL2.GL_LUMINANCE);
                    setPixelType(GL2.GL_FLOAT);
                    this.alignment = 4;
                    break;
                case RED_16:
                    this.internalFormat = GL2.GL_RGB16;
                    setPixelFormat(GL2.GL_RED);
                    setPixelType(GL2.GL_UNSIGNED_SHORT);
                    this.alignment = 2;
                    break;
                case GREEN_16:
                    this.internalFormat = GL2.GL_RGB16;
                    setPixelFormat(GL2.GL_GREEN);
                    setPixelType(GL2.GL_UNSIGNED_SHORT);
                    this.alignment = 2;
                    break;
                case BLUE_16:
                    this.internalFormat = GL2.GL_RGB16;
                    setPixelFormat(GL2.GL_BLUE);
                    setPixelType(GL2.GL_UNSIGNED_SHORT);
                    this.alignment = 2;
                    break;
                case RED_FLOAT:
                    this.internalFormat = GL2ES1.GL_RGB16F;
                    setPixelFormat(GL2.GL_RED);
                    setPixelType(GL2.GL_FLOAT);
                    this.alignment = 4;
                    break;
                case GREEN_FLOAT:
                    this.internalFormat = GL2ES1.GL_RGB16F;
                    setPixelFormat(GL2.GL_GREEN);
                    setPixelType(GL2.GL_FLOAT);
                    this.alignment = 4;
                    break;
                case BLUE_FLOAT:
                    this.internalFormat = GL2ES1.GL_RGB16F;
                    setPixelFormat(GL2.GL_BLUE);
                    setPixelType(GL2.GL_FLOAT);
                    this.alignment = 4;
                    break;
                case RGBA_BYTE:
                    this.internalFormat = GL2.GL_RGBA;
                    setPixelFormat(GL2.GL_RGBA);
                    setPixelType(GL2.GL_UNSIGNED_BYTE);
                    this.alignment = 1;
                    break;
            }
        }

        public JoGLTextureData(GLProfile glp, TextureDataProvider.ImageType type, ByteBuffer data, int width, int height) {
            this(glp, type, data, width, height, 0, 0, width, height);
        }

        public ByteBuffer getBuffer() {
            if (shift == 0) {
                return data;
            } else {
                data.position(shift);
                ByteBuffer b = data.slice();
                data.rewind();

                return b;
            }
        }

        public void setData(ByteBuffer data) {
            this.data = data;
        }

        public void destroy() {
            super.destroy();
            this.data = null;
        }
    }
}
