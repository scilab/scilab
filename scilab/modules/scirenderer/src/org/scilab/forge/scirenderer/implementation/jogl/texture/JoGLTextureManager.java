/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
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

package org.scilab.forge.scirenderer.implementation.jogl.texture;

import com.jogamp.opengl.util.texture.TextureIO;
import com.jogamp.opengl.util.texture.TextureCoords;

import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLCanvas;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLDrawingTools;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.texture.AbstractTexture;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureManager;
import org.scilab.forge.scirenderer.texture.TextureDataProvider;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationManager;
import org.scilab.forge.scirenderer.tranformations.Vector3d;


import javax.media.opengl.GL;
import javax.media.opengl.GL2;
import javax.media.opengl.GL2ES1;
import javax.media.opengl.GL2GL3;
import javax.media.opengl.GLProfile;
import java.awt.Dimension;
import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.util.Collection;
import java.util.Collections;
import java.util.HashSet;
import java.util.Set;

/**
 * @author Pierre Lando
 */
public class JoGLTextureManager implements TextureManager {

    private final Set<JoGLTexture> allTextures = Collections.synchronizedSet(new HashSet<JoGLTexture>());
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
     * @param texture the texture to draw.
     * @throws org.scilab.forge.scirenderer.SciRendererException if the texture is invalid.
     */
    public void draw(JoGLDrawingTools drawingTools, Texture texture) throws SciRendererException {
        if ((texture instanceof JoGLTexture) && (allTextures.contains((JoGLTexture) texture))) {
            final JoGLTexture jt = (JoGLTexture) texture;
            if (jt.preDraw(drawingTools, null)) {
                jt.draw(drawingTools);
                jt.postDraw(drawingTools);
            }
        }
    }

    public void draw(JoGLDrawingTools drawingTools, Texture texture, AnchorPosition anchor, ElementsBuffer positions, int offset, int stride, double rotationAngle, Color auxColor, ElementsBuffer colors) throws SciRendererException {
        if ((texture instanceof JoGLTexture) && (allTextures.contains((JoGLTexture) texture))) {
            if (positions != null) {
                FloatBuffer data = positions.getData();
                FloatBuffer dataColors = null;

                if (data != null) {
                    data.rewind();
                    // Initializing dataColors Buffer
                    if (colors != null) {
                        dataColors = colors.getData();
                        if (dataColors != null) {
                            dataColors.rewind();
                            // There should be as many colors as there are positions
                            if (dataColors.limit() != data.limit()) {
                                throw new SciRendererException("Vertices do not have the same number of positions and colors");
                            }
                        }
                    }
                    float[] position = {0, 0, 0, 1};
                    float[] color = {0, 0, 0, 0};
                    float[] auxcolor = null;
                    if (auxColor != null) {
                        auxcolor = new float[4];
                        auxColor.getComponents(auxcolor);
                    }

                    int mark = 0;

                    final JoGLTexture jt = (JoGLTexture) texture;
                    if (jt.preDraw(drawingTools, auxcolor)) {
                        stride = stride < 1 ? 1 : stride;
                        offset = offset < 0 ? 0 : offset;
                        if (stride == 1) {
                            // skip offset positions in the data Buffer
                            // a position is 4 elements
                            mark = 4 * offset;
                            data.position(mark);
                            if (dataColors != null) {
                                dataColors.position(mark);
                            }
                            while (data.remaining() >= 4) {
                                data.get(position);
                                if (dataColors == null) {
                                    jt.draw(drawingTools, anchor, new Vector3d(position), rotationAngle, null);
                                } else {
                                    dataColors.get(color);
                                    jt.draw(drawingTools, anchor, new Vector3d(position), rotationAngle, color);
                                }
                            }
                        } else {
                            mark = 4 * offset;
                            if (mark < data.capacity()) {
                                data.position(mark);
                                while (data.remaining() >= 4) {
                                    data.get(position);
                                    if (dataColors == null) {
                                        jt.draw(drawingTools, anchor, new Vector3d(position), rotationAngle, null);
                                    } else {
                                        dataColors.position(mark);
                                        dataColors.get(color);
                                        jt.draw(drawingTools, anchor, new Vector3d(position), rotationAngle, color);
                                    }
                                    // reposition the mark on data and dataColors
                                    mark += stride * 4;
                                    if (mark < data.capacity()) {
                                        data.position(mark);
                                        if (dataColors != null) {
                                            dataColors.position(mark);
                                        }
                                    } else {
                                        break;
                                    }
                                }
                            }
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
            jt.preDraw(drawingTools, null);
            jt.draw(drawingTools, anchor, position, rotationAngle, null);
            jt.postDraw(drawingTools);
        }
    }

    /** Called when gl context is gone. */
    public void glReload() {
        synchronized (allTextures) {
            for (JoGLTexture texture : allTextures) {
                texture.glReload();
            }
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
                    gl.glBlendFunc(GL2.GL_SRC_ALPHA, GL2.GL_ONE_MINUS_SRC_ALPHA);
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

        public boolean preDraw(JoGLDrawingTools drawingTools, float[] auxcolor) throws SciRendererException {
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

            // ONE => SRC_ALPHA (fix for bug 13673)
            gl.glBlendFunc(GL2.GL_SRC_ALPHA, GL2.GL_ONE_MINUS_SRC_ALPHA);

            gl.glEnable(GL2.GL_ALPHA_TEST);
            gl.glAlphaFunc(GL2.GL_GREATER, 0.0f);

            gl.glPushAttrib(GL2.GL_ALL_ATTRIB_BITS);

            gl.glColor4f(1f, 1f, 1f, 1f);
            if (auxcolor != null) {
                gl.glTexEnvfv(GL2.GL_TEXTURE_ENV, GL2.GL_TEXTURE_ENV_COLOR, auxcolor, 0);
                gl.glTexEnvi(GL2.GL_TEXTURE_ENV, GL2.GL_TEXTURE_ENV_MODE, GL2.GL_BLEND);
            } else {
                gl.glTexEnvi(GL2.GL_TEXTURE_ENV, GL2.GL_TEXTURE_ENV_MODE, GL2.GL_MODULATE);
            }

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

        public void draw(JoGLDrawingTools drawingTools, AnchorPosition anchor, Vector3d position, double rotationAngle, float[] color) throws SciRendererException {
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

            draw(drawingTools, color);

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

        public void draw(JoGLDrawingTools drawingTools) throws SciRendererException {
            draw(drawingTools, null);
        }

        /**
         * Draw the texture in XY plane.
         * @param drawingTools the drawing tools.
         * @throws SciRendererException if the texture is invalid.
         */
        public void draw(JoGLDrawingTools drawingTools, float[] color) throws SciRendererException {
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

                    if (color != null) {
                        gl.glColor4fv(color, 0);
                    }

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

                            if (color != null) {
                                gl.glColor4fv(color, 0);
                            }

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

        public JoGLTextureData(GLProfile glp, int[] infos, ByteBuffer data, int width, int height, int x, int y, int totalWidth, int totalHeight) {
            super(glp, infos[0], width, height, 0, infos[1], infos[2], false, false, true, null, null);
            this.data = data;
            this.shift = ((x == 0 && y == 0) || data.capacity() == 0) ? 0 : (data.capacity() / (totalWidth * totalHeight) * (x + y * totalWidth));
            this.rowLength = totalWidth;
            this.alignment = infos[3];
        }

        public JoGLTextureData(GLProfile glp, TextureDataProvider.ImageType type, ByteBuffer data, int width, int height) {
            this(glp, getInfos(type), data, width, height, 0, 0, width, height);
        }

        public JoGLTextureData(GLProfile glp, TextureDataProvider.ImageType type, ByteBuffer data, int width, int height, int x, int y, int totalWidth, int totalHeight) {
            this(glp, getInfos(type), data, width, height, x, y, totalWidth, totalHeight);
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

        public static int[] getInfos(TextureDataProvider.ImageType type) {
            switch (type) {
                case RGB:
                    //internalFormat, pixelFormat, pixelType, alignment
                    return new int[] {GL2.GL_RGB, GL.GL_RGB, GL2.GL_UNSIGNED_BYTE, 1};
                case RGB_RGBA:
                    return new int[] {GL2.GL_RGB, GL.GL_RGBA, GL2.GL_UNSIGNED_INT_8_8_8_8, 4};
                case BGR:
                    return new int[] {GL2GL3.GL_RGB, GL2GL3.GL_BGR, GL.GL_UNSIGNED_BYTE, 1};
                case GRAY:
                    return new int[] {GL.GL_LUMINANCE, GL.GL_LUMINANCE, GL.GL_UNSIGNED_BYTE, 1};
                case GRAY_16:
                    return new int[] {GL2.GL_LUMINANCE16, GL2.GL_LUMINANCE, GL2.GL_UNSIGNED_SHORT, 2};
                case RGBA:
                    return new int[] {GL2.GL_RGBA, GL.GL_RGBA, GL2.GL_UNSIGNED_INT_8_8_8_8, 4};
                case RGBA_REV:
                    return new int[] {GL2.GL_RGBA, GL2.GL_RGBA, GL2.GL_UNSIGNED_INT_8_8_8_8_REV, 4};
                case ABGR:
                    return new int[] {GL2.GL_RGBA, GL2.GL_ABGR_EXT, GL.GL_UNSIGNED_BYTE, 1};
                case RGB_332:
                    return new int[] {GL2.GL_R3_G3_B2, GL2.GL_RGB, GL2.GL_UNSIGNED_BYTE_3_3_2, 1};
                case RED:
                    return new int[] {GL2.GL_RED, GL2.GL_RED, GL2.GL_UNSIGNED_BYTE, 1};
                case GREEN:
                    return new int[] {GL2.GL_RGB8, GL2.GL_GREEN, GL2.GL_UNSIGNED_BYTE, 1};
                case BLUE:
                    return new int[] {GL2.GL_RGB8, GL2.GL_BLUE, GL2.GL_UNSIGNED_BYTE, 1};
                case INTENSITY:
                    return new int[] {GL2.GL_INTENSITY, GL2.GL_LUMINANCE, GL2.GL_UNSIGNED_BYTE, 1};
                case RGBA_4444:
                    return new int[] {GL2.GL_RGBA4, GL2.GL_RGBA, GL2.GL_UNSIGNED_SHORT_4_4_4_4, 2};
                case RGBA_5551:
                    return new int[] {GL2.GL_RGB5_A1, GL2.GL_RGBA, GL2.GL_UNSIGNED_SHORT_5_5_5_1, 2};
                case RGB_FLOAT:
                    return new int[] {GL2ES1.GL_RGB16F, GL2.GL_RGB, GL2.GL_FLOAT, 4};
                case RGBA_FLOAT:
                    return new int[] {GL2.GL_RGBA16F, GL2.GL_RGBA, GL2.GL_FLOAT, 4};
                case GRAY_FLOAT:
                    return new int[] {GL2.GL_LUMINANCE16F, GL2.GL_LUMINANCE, GL2.GL_FLOAT, 4};
                case RED_16:
                    return new int[] {GL2.GL_RGB16, GL2.GL_RED, GL2.GL_UNSIGNED_SHORT, 2};
                case GREEN_16:
                    return new int[] {GL2.GL_RGB16, GL2.GL_GREEN, GL2.GL_UNSIGNED_SHORT, 2};
                case BLUE_16:
                    return new int[] {GL2.GL_RGB16, GL2.GL_BLUE, GL2.GL_UNSIGNED_SHORT, 2};
                case RED_FLOAT:
                    return new int[] {GL2ES1.GL_RGB16F, GL2.GL_RED, GL2.GL_FLOAT, 4};
                case GREEN_FLOAT:
                    return new int[] {GL2ES1.GL_RGB16F, GL2.GL_GREEN, GL2.GL_FLOAT, 4};
                case BLUE_FLOAT:
                    return new int[] {GL2ES1.GL_RGB16F, GL2.GL_BLUE, GL2.GL_FLOAT, 4};
                case RGBA_BYTE:
                    return new int[] {GL2.GL_RGBA, GL2.GL_RGBA, GL2.GL_UNSIGNED_BYTE, 1};
            }

            return new int[] {GL2.GL_RGB, GL.GL_RGB, GL2.GL_UNSIGNED_BYTE, 1};
        }
    }
}
