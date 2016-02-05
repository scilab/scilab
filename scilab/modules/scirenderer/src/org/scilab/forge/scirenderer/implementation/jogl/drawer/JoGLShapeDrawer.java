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

package org.scilab.forge.scirenderer.implementation.jogl.drawer;

import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.buffers.IndicesBuffer;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLDrawingTools;
import org.scilab.forge.scirenderer.implementation.jogl.buffers.JoGLBuffersManager;
import org.scilab.forge.scirenderer.implementation.jogl.buffers.JoGLElementsBuffer;
import org.scilab.forge.scirenderer.implementation.jogl.utils.GLShortCuts;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.lightning.LightManager;
import org.scilab.forge.scirenderer.shapes.appearance.Material;

import javax.media.opengl.GL2;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

/**
 * Utility class for drawing shapes.
 * @author Pierre Lando
 */
public final class JoGLShapeDrawer {

    private static JoGLShapeDrawer drawer;

    /**
     * Private constructor : this is an utility class.
     */
    private JoGLShapeDrawer() {
    }

    /**
     * Singleton getter.
     * @return the unique {@link JoGLShapeDrawer}.
     */
    public static JoGLShapeDrawer getDrawer() {
        if (drawer == null) {
            drawer = new JoGLShapeDrawer();
        }
        return drawer;
    }

    /**
     * Draw a given geometry with given appearance.
     * @param drawingTools the drawing tools.
     * @param geometry the geometry.
     * @param appearance the appearance.
     * @throws org.scilab.forge.scirenderer.SciRendererException if the draw is not possible.
     */
    public void draw(JoGLDrawingTools drawingTools, Geometry geometry, Appearance appearance) throws SciRendererException {
        GL2 gl = drawingTools.getGl().getGL2();
        gl.glFrontFace(GL2.GL_CCW);
        switch (geometry.getFaceCullingMode()) {
            case CW:
                gl.glEnable(GL2.GL_CULL_FACE);
                gl.glCullFace(GL2.GL_FRONT);
                break;
            case CCW:
                gl.glEnable(GL2.GL_CULL_FACE);
                gl.glCullFace(GL2.GL_BACK);
                break;
            case BOTH:
                gl.glDisable(GL2.GL_CULL_FACE);
                break;
            default:
                gl.glDisable(GL2.GL_CULL_FACE);
                break;
        }

        if (drawingTools.getCanvas().getJoGLParameters().useVBO()) {
            vboDrawing(drawingTools, geometry, appearance);
        } else {
            directDrawing(drawingTools, geometry, appearance);
        }

        GLShortCuts.useLineAppearance(gl, null);
        gl.glDisable(GL2.GL_CULL_FACE);
    }

    /**
     * Perform geometry drawing using VBO.
     * @param drawingTools the drawing tools.
     * @param geometry the geometry to draw.
     * @param appearance the current appearance.
     * @throws org.scilab.forge.scirenderer.SciRendererException if the draw is not possible.
     */
    private void vboDrawing(JoGLDrawingTools drawingTools, Geometry geometry, Appearance appearance) throws SciRendererException {
        final GL2 gl = drawingTools.getGl().getGL2();
        final JoGLBuffersManager buffersManager = drawingTools.getCanvas().getBuffersManager();
        final Texture texture = appearance.getTexture();

        int verticesNumber = buffersManager.bindVertexBuffer(gl, geometry.getVertices());
        if (verticesNumber == 0) {
            gl.glDisableClientState(GL2.GL_VERTEX_ARRAY);
            return;
        }

        buffersManager.bindNormalsBuffer(gl, geometry.getNormals());

        if (texture != null && geometry.getTextureCoordinates() != null) {
            synchronized (texture) {
                if (texture.isValid()) {
                    drawingTools.bind(texture);
                    buffersManager.bindTextureCoordinatesBuffer(gl, geometry.getTextureCoordinates());
                }
            }
        } else {
            buffersManager.bindColorsBuffer(gl, geometry.getColors());
        }

        // We use polygon offset for filled geometry if required.
        if (geometry.getPolygonOffsetMode()) {
            gl.glEnable(GL2.GL_POLYGON_OFFSET_FILL);
            gl.glPolygonOffset(1, 1);
        }

        GLShortCuts.useColor(gl, appearance.getFillColor());

        LightManager lm = drawingTools.getLightManager();
        boolean lighting = lm.isLightningEnable();
        if (lighting) {
            lm.setMaterial(appearance.getMaterial());
            gl.glEnable(GL2.GL_NORMALIZE);
        }

        IndicesBuffer indices = geometry.getIndices();
        if (geometry.getFillDrawingMode() != Geometry.FillDrawingMode.NONE) {
            if (indices != null) {
                int indicesSize = buffersManager.bindIndicesBuffer(gl, indices);
                if (indicesSize > 0) {
                    gl.glDrawElements(getGlMode(geometry.getFillDrawingMode()), indicesSize, GL2.GL_UNSIGNED_INT, 0);
                    gl.glBindBuffer(GL2.GL_ELEMENT_ARRAY_BUFFER, 0);
                }
            } else {
                int count = geometry.getVertices().getSize();
                if (count > 0) {
                    gl.glDrawArrays(getGlMode(geometry.getFillDrawingMode()), 0, count);
                }
            }
        }

        if (geometry.getPolygonOffsetMode()) {
            gl.glDisable(GL2.GL_POLYGON_OFFSET_FILL);
        }

        gl.glDisableClientState(GL2.GL_COLOR_ARRAY);
        gl.glDisableClientState(GL2.GL_NORMAL_ARRAY);
        gl.glDisableClientState(GL2.GL_TEXTURE_COORD_ARRAY);
        gl.glDisable(GL2.GL_TEXTURE_2D);
        //disable lighting to draw lines
        lm.setLightningEnable(false);
        gl.glDisable(GL2.GL_NORMALIZE);

        if (geometry.getLineDrawingMode() != Geometry.LineDrawingMode.NONE) {
            // to avoid color smoothing between edge
            // TODO: add an option in Appearance
            gl.glShadeModel(GL2.GL_FLAT);
            if (appearance.getLineColor() != null || geometry.getColors() != null) {
                GLShortCuts.useLineAppearance(gl, appearance);
                if (appearance.getLineColor() == null) {
                    buffersManager.bindColorsBuffer(gl, geometry.getColors());
                }

                if (geometry.getWireIndices() != null) {
                    int edgesIndicesSize = buffersManager.bindIndicesBuffer(gl, geometry.getWireIndices());
                    if (edgesIndicesSize > 0) {
                        gl.glDrawElements(getGlMode(geometry.getLineDrawingMode()), edgesIndicesSize, GL2.GL_UNSIGNED_INT, 0);
                        gl.glBindBuffer(GL2.GL_ELEMENT_ARRAY_BUFFER, 0);
                    }
                } else {
                    int count = geometry.getVertices().getSize();
                    if (count > 0) {
                        gl.glDrawArrays(getGlMode(geometry.getLineDrawingMode()), 0, count);
                    }
                }

                gl.glDisableClientState(GL2.GL_COLOR_ARRAY);
            }
        }

        gl.glDisableClientState(GL2.GL_VERTEX_ARRAY);
        lm.setLightningEnable(lighting);
    }

    /**
     * Perform geometry drawing by direct OpenGl call.
     * @param drawingTools the drawing tools.
     * @param geometry the geometry to draw.
     * @param appearance the used appearance.
     * @throws org.scilab.forge.scirenderer.SciRendererException if the draw is not possible.
     */
    private void directDrawing(
        JoGLDrawingTools drawingTools, Geometry geometry, Appearance appearance
    ) throws SciRendererException {
        final double sfactor;
        final double tfactor;
        final float[] fbuffer = new float[4];

        GL2 gl = drawingTools.getGl().getGL2();
        if (geometry.getVertices() == null) {
            return;
        }

        FloatBuffer vertexBuffer = geometry.getVertices().getData();
        IndicesBuffer indices = geometry.getIndices();

        FloatBuffer colorBuffer;
        if (geometry.getColors() != null) {
            colorBuffer = geometry.getColors().getData();
        } else {
            colorBuffer = null;
        }

        FloatBuffer normalBuffer;
        if (geometry.getNormals() != null) {
            normalBuffer = geometry.getNormals().getData();
        } else {
            normalBuffer = null;
        }

        Texture texture = appearance.getTexture();
        FloatBuffer textureCoordinatesBuffer;
        if (texture != null && geometry.getTextureCoordinates() != null) {
            synchronized (texture) {
                drawingTools.bind(texture);
                textureCoordinatesBuffer = geometry.getTextureCoordinates().getData();
                sfactor = texture.getSScaleFactor();
                tfactor = texture.getTScaleFactor();
            }
        } else {
            textureCoordinatesBuffer = null;
            sfactor = 1;
            tfactor = 1;
        }

        final int elementsSize = JoGLElementsBuffer.ELEMENT_SIZE;

        if (geometry.getPolygonOffsetMode()) {
            gl.glEnable(GL2.GL_POLYGON_OFFSET_FILL);
            gl.glPolygonOffset(1, 1);
        }

        LightManager lm = drawingTools.getLightManager();
        boolean lighting = lm.isLightningEnable();
        if (lighting) {
            lm.setMaterial(appearance.getMaterial());
        }

        if (geometry.getFillDrawingMode() != Geometry.FillDrawingMode.NONE) {
            GLShortCuts.useColor(gl, appearance.getFillColor());
            gl.glShadeModel(GL2.GL_FLAT);
            gl.glBegin(getGlMode(geometry.getFillDrawingMode()));
            if (indices != null) {
                IntBuffer indicesBuffer = indices.getData();
                indicesBuffer.rewind();
                for (int i = 0; i < indicesBuffer.limit(); i++) {
                    int index = indicesBuffer.get(i);
                    if ((index * elementsSize) < vertexBuffer.limit()) {

                        if (colorBuffer != null) {
                            colorBuffer.position(index * elementsSize);
                            gl.glColor4fv(colorBuffer);
                        }

                        if (normalBuffer != null) {
                            normalBuffer.position(index * elementsSize);
                            gl.glNormal3fv(normalBuffer);
                        }

                        if (textureCoordinatesBuffer != null) {
                            textureCoordinatesBuffer.position(index * elementsSize);
                            textureCoordinatesBuffer.get(fbuffer);

                            gl.glTexCoord4f((float) (sfactor * fbuffer[0]), (float) (tfactor * fbuffer[1]), fbuffer[2], fbuffer[3]);
                        }

                        vertexBuffer.position(index * elementsSize);
                        gl.glVertex4fv(vertexBuffer);

                    }
                }
            } else {
                vertexBuffer.rewind();

                if (colorBuffer != null) {
                    colorBuffer.rewind();
                }

                if (normalBuffer != null) {
                    normalBuffer.rewind();
                }

                for (int i = 0; i < vertexBuffer.limit(); i += elementsSize) {
                    if (colorBuffer != null) {
                        colorBuffer.position(i);
                        gl.glColor4fv(colorBuffer);
                    }

                    if (normalBuffer != null) {
                        normalBuffer.position(i);
                        gl.glNormal3fv(normalBuffer);
                    }

                    if (textureCoordinatesBuffer != null) {
                        textureCoordinatesBuffer.position(i);
                        gl.glTexCoord4fv(textureCoordinatesBuffer);
                    }

                    vertexBuffer.position(i);
                    gl.glVertex4fv(vertexBuffer);
                }
            }
            gl.glEnd();
        }

        if (geometry.getPolygonOffsetMode()) {
            gl.glDisable(GL2.GL_POLYGON_OFFSET_FILL);
        }

        gl.glDisable(GL2.GL_TEXTURE_2D);
        //disable lighting to draw lines
        lm.setLightningEnable(false);

        // Draw edges if any.
        if (geometry.getLineDrawingMode() != Geometry.LineDrawingMode.NONE) {
            GLShortCuts.useLineAppearance(gl, appearance);
            if (appearance.getLineColor() != null) {
                gl.glBegin(getGlMode(geometry.getLineDrawingMode()));
                if (geometry.getWireIndices() != null) {
                    IntBuffer edgesIndicesBuffer = geometry.getWireIndices().getData();
                    edgesIndicesBuffer.rewind();
                    while (edgesIndicesBuffer.remaining() != 0) {
                        int index = edgesIndicesBuffer.get();
                        if ((index * elementsSize) < vertexBuffer.limit()) {
                            vertexBuffer.position(index * elementsSize);
                            gl.glVertex4fv(vertexBuffer);
                        }
                    }
                } else {
                    for (int i = 0; i < vertexBuffer.limit(); i += elementsSize) {
                        vertexBuffer.position(i);
                        gl.glVertex4fv(vertexBuffer);
                    }
                }
                gl.glEnd();
            } else if (colorBuffer != null) {
                gl.glBegin(getGlMode(geometry.getLineDrawingMode()));
                if (geometry.getWireIndices() != null) {
                    IntBuffer edgesIndicesBuffer = geometry.getWireIndices().getData();
                    edgesIndicesBuffer.rewind();
                    while (edgesIndicesBuffer.remaining() != 0) {
                        int index = edgesIndicesBuffer.get();
                        if ((index * elementsSize) < vertexBuffer.limit()) {
                            colorBuffer.position(index * elementsSize);
                            gl.glColor4fv(colorBuffer);

                            vertexBuffer.position(index * elementsSize);
                            gl.glVertex4fv(vertexBuffer);

                        }
                    }
                } else {
                    for (int i = 0; i < vertexBuffer.limit(); i += elementsSize) {
                        colorBuffer.position(i);
                        vertexBuffer.position(i);
                        gl.glColor4fv(colorBuffer);
                        gl.glVertex4fv(vertexBuffer);
                    }
                }
                gl.glEnd();
            }
        }
        lm.setLightningEnable(lighting);
    }


    /**
     * Return the gl drawing mode corresponding to the given {@link Geometry.FillDrawingMode}.
     * @param drawingMode the given drawing mode..
     * @return the gl drawing mode corresponding to the given {@link Geometry.FillDrawingMode}.
     */
    private int getGlMode(Geometry.FillDrawingMode drawingMode) {
        switch (drawingMode) {
            case TRIANGLE_FAN:
                return GL2.GL_TRIANGLE_FAN;
            case TRIANGLE_STRIP:
                return GL2.GL_TRIANGLE_STRIP;
            case TRIANGLES:
                return GL2.GL_TRIANGLES;
            default:
                return GL2.GL_TRIANGLES;
        }
    }

    /**
     * Return the gl drawing mode corresponding to the given {@link org.scilab.forge.scirenderer.shapes.geometry.Geometry.LineDrawingMode}
     * @param drawingMode the given drawing mode.
     * @return the gl drawing mode corresponding to the given {@link org.scilab.forge.scirenderer.shapes.geometry.Geometry.LineDrawingMode}
     */
    private int getGlMode(Geometry.LineDrawingMode drawingMode) {
        switch (drawingMode) {
            case SEGMENTS:
                return GL2.GL_LINES;
            case SEGMENTS_LOOP:
                return GL2.GL_LINE_LOOP;
            case SEGMENTS_STRIP:
                return GL2.GL_LINE_STRIP;
            default:
                return GL2.GL_LINES;
        }
    }
}
