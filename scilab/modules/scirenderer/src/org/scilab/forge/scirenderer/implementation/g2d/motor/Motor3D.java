/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012-2013 - Scilab Enterprises - Calixte Denizet
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

package org.scilab.forge.scirenderer.implementation.g2d.motor;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.Stroke;
import java.awt.image.BufferedImage;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.Arrays;
import java.util.List;

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.clipping.ClippingPlane;
import org.scilab.forge.scirenderer.implementation.g2d.G2DCanvas;
import org.scilab.forge.scirenderer.implementation.g2d.buffers.G2DElementsBuffer;
import org.scilab.forge.scirenderer.implementation.g2d.texture.G2DTextureDrawingTools;
import org.scilab.forge.scirenderer.implementation.g2d.texture.G2DTextureManager;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry.FaceCullingMode;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.forge.scirenderer.tranformations.Vector3f;

import org.scilab.forge.scirenderer.lightning.Light;
import org.scilab.forge.scirenderer.lightning.LightManager;
import org.scilab.forge.scirenderer.implementation.g2d.lighting.G2DLight;
import org.scilab.forge.scirenderer.implementation.g2d.lighting.G2DLightManager;
import org.scilab.forge.scirenderer.shapes.appearance.Material;

/**
 * @author Calixte DENIZET
 */
public class Motor3D {

    private Transformation transf;
    private Transformation singleTransf;
    private FaceCullingMode mode = FaceCullingMode.BOTH;
    private Graphics2D g2d;
    private Dimension dim;
    private G2DTextureDrawingTools textureDrawingTools;
    private G2DCanvas canvas;

    /**
     * Default constructor
     * @param g2d a Graphics2D object where to draw
     * @param dim the graphic dimensions
     */
    public Motor3D(G2DCanvas canvas, Graphics2D g2d, Dimension dim) {
        this.canvas = canvas;
        this.g2d = g2d;
        this.dim = dim;
        this.textureDrawingTools = new G2DTextureDrawingTools(g2d);
        AbstractDrawable3DObject.resetDefaultPrecedence();
    }

    public void setGraphics(Graphics2D g2d) {
        this.g2d = g2d;
        this.textureDrawingTools.setGraphics(g2d);
    }

    public void setAntialiased(boolean aa) {
        if (aa) {
            g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        } else {
            g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_OFF);
        }
    }

    public boolean is2DView() {
        return canvas.getMainDrawer().is2DView();
    }

    public void setClippingPlanes(List<ClippingPlane> clippingPlanes) {
        Scene.setClippingPlanes(clippingPlanes);
    }

    /**
     * Set the face culling mode
     * @param mode the mode to set
     */
    public void setFaceCullingMode(FaceCullingMode mode) {
        this.mode = mode;
    }

    /**
     * Set the current transformation
     * @param transf the transformation to set
     */
    public void setTransformation(Transformation transf, Transformation single) {
        this.transf = transf;
        this.singleTransf = single;
    }

    public Transformation getCurrentTransformation() {
        return transf;
    }

    public Transformation getCurrentSingleTransformation() {
        return singleTransf;
    }

    /**
     * Reset this motor
     * @param color the filling color
     */
    public void reset(Color color) {
        transf = null;
        mode = FaceCullingMode.BOTH;
        g2d.setColor(color);
        g2d.fillRect(0, 0, (int) dim.getWidth(), (int) dim.getHeight());
        Scene.clear();
    }

    /**
     * Clear the depth buffer
     */
    public void clearDepth() {
        Scene.clearDepth();
    }

    /**
     * Draw the scene in the Graphics2D
     */
    public void draw() {
        Scene.drawRoot(g2d);
        clean();
    }

    public void clean() {
        Scene.clearAll();
        G2DTextureManager.clear();
    }

    public void drawTexture(DrawingTools drawingTools, BufferedImage image, Texture texture) {
        try {
            SpritedRectangle o = new SpritedRectangle(new Vector3d(0, 0, 0), transf, image, texture.getMagnificationFilter());
            add(o);
        } catch (InvalidPolygonException e) { }
    }

    /**
     * Add the geometry to the scene
     * @param drawingTools the DrawingTools
     * @param geometry the geometry to draw
     * @param appearance the appearance to use
     */
    public void draw(DrawingTools drawingTools, Geometry geometry, Appearance appearance) {
        setFaceCullingMode(geometry.getFaceCullingMode());
        FloatBuffer vertexBuffer = geometry.getVertices().getData();

        IntBuffer indicesBuffer = null;;
        if (geometry.getIndices() != null) {
            indicesBuffer = geometry.getIndices().getData();
        }

        IntBuffer wireIndicesBuffer = null;
        if (geometry.getWireIndices() != null) {
            wireIndicesBuffer = geometry.getWireIndices().getData();
        }

        FloatBuffer colorBuffer = null;
        if (geometry.getColors() != null) {
            colorBuffer = geometry.getColors().getData();
        }

        FloatBuffer normalBuffer = null;
        if (geometry.getNormals() != null) {
            normalBuffer = geometry.getNormals().getData();
        }

        Texture texture = appearance.getTexture();
        FloatBuffer textureCoordinatesBuffer = null;
        BufferedImage image = null;
        if (texture != null && geometry.getTextureCoordinates() != null) {
            textureCoordinatesBuffer = geometry.getTextureCoordinates().getData();
            image = ((G2DTextureManager.G2DTexture) texture).getImage();
        }

        G2DLightManager lm = (G2DLightManager)drawingTools.getLightManager();
        lm.setMaterial(appearance.getMaterial());

        if (geometry.getFillDrawingMode() != Geometry.FillDrawingMode.NONE) {
            addTriangles(vertexBuffer, normalBuffer, colorBuffer, appearance.getFillColor(), indicesBuffer, textureCoordinatesBuffer, image, texture, geometry.getFillDrawingMode(), lm);
        }

        if (geometry.getLineDrawingMode() != Geometry.LineDrawingMode.NONE) {
            if (appearance.getLineColor() == null) {
                addSegments(vertexBuffer, colorBuffer, null, wireIndicesBuffer, geometry.getLineDrawingMode(), appearance);
            } else {
                addSegments(vertexBuffer, null, appearance.getLineColor(), wireIndicesBuffer, geometry.getLineDrawingMode(), appearance);
            }
        }
    }

    public void draw(DrawingTools drawingTools, Texture texture, AnchorPosition anchor, ElementsBuffer positions, int offset, int stride, double rotationAngle, org.scilab.forge.scirenderer.shapes.appearance.Color auxColor, ElementsBuffer colors) {
        FloatBuffer positionsBuffer = positions.getData();
        float[] buffer;
        offset = offset < 0 ? 0 : offset;
        stride = stride < 1 ? 1 : stride;
        Color[] colorsArray = null;

        positionsBuffer.rewind();
        if (positionsBuffer.hasArray()) {
            buffer = positionsBuffer.array();
        } else {
            buffer = new float[positionsBuffer.limit()];
            positionsBuffer.get(buffer);
        }
        Vector3d[] verticesArray = getMultiVectors(buffer, transf, false);

        if (colors != null) {
            FloatBuffer colorsBuffer = colors.getData();
            colorsBuffer.rewind();
            if (colorsBuffer.hasArray()) {
                buffer = colorsBuffer.array();
            } else {
                buffer = new float[colorsBuffer.limit()];
                colorsBuffer.get(buffer);
            }
            colorsArray = getMultiColors(buffer);
        }

        for (int i = offset; i < verticesArray.length; i += stride) {
            try {
                Vector3d v = verticesArray[i];
                SpritedRectangle o;
                if (colorsArray == null) {
                    o = new SpritedRectangle(v, texture, anchor, textureDrawingTools, rotationAngle, null, null);
                } else {
                    o = new SpritedRectangle(v, texture, anchor, textureDrawingTools, rotationAngle, (Color) auxColor, colorsArray[i]);
                }
                add(o);
            } catch (InvalidPolygonException e) { }
        }
    }

    public void draw(DrawingTools drawingTools, Texture texture, AnchorPosition anchor, Vector3d position, double rotationAngle) {
        try {
            add(new SpritedRectangle(transf.project(position), texture, anchor, textureDrawingTools, rotationAngle, null, null));
        } catch (InvalidPolygonException e) { }
    }

    /**
     * Add a Triangle to the scene
     * @param tri the triangle to add
     */
    private void add(Triangle tri) {
        final boolean is2d = is2DView();
        if (is2d) {
            Scene.addToRoot(is2d, tri);
        } else {
            Vector3d normal = tri.getNormal();
            if (normal != null) {
                //normal = transf.projectDirection(normal);
                if ((mode == FaceCullingMode.CW && normal.getZ() > 0) || (mode == FaceCullingMode.CCW && normal.getZ() < 0) || mode == FaceCullingMode.BOTH) {
                    Scene.addToRoot(is2d, tri);
                }
            } else {
                Scene.addToRoot(is2d, tri);
            }
        }
    }

    /**
     * Add a segment to the scene
     * @param s the segment to add
     */
    private void add(Segment s) {
        Scene.addToRoot(is2DView(), s);
    }

    private void add(SpritedRectangle sprite) {
        Scene.addToRoot(is2DView(), sprite);
    }

    private void add(PolyLine p) {
        Scene.addToRoot(is2DView(), p);
    }

    /**
     * Get arrays from Buffer
     * @param vertices a buffer containing vertices
     * @param colors a buffer containing the colors
     * @param defaultColor the color to use when colors is null
     * @param indices a buffer containing the index of the vertices to retrieve
     * @return an array of length 2 containing the vertices array and the colors array
     */
    private Object[] getArrays(FloatBuffer vertices, FloatBuffer colors, Color defaultColor, FloatBuffer textureCoords, IntBuffer indices) {
        float[] buffer;
        Vector3d[] verticesArray;
        Vector3d[] textureCoordsArray = null;
        Color[] colorsArray;

        vertices.rewind();
        if (vertices.hasArray()) {
            buffer = vertices.array();
        } else {
            buffer = new float[vertices.limit()];
            vertices.get(buffer);
        }
        verticesArray = getMultiVectors(buffer, transf, false);

        if (colors != null) {
            colors.rewind();
            if (colors.hasArray()) {
                buffer = colors.array();
            } else {
                buffer = new float[colors.limit()];
                colors.get(buffer);
            }
            colorsArray = getMultiColors(buffer);
        } else {
            colorsArray = new Color[vertices.limit() / G2DElementsBuffer.ELEMENT_SIZE];
            Arrays.fill(colorsArray, defaultColor);
        }

        if (textureCoords != null) {
            textureCoords.rewind();
            if (textureCoords.hasArray()) {
                buffer = textureCoords.array();
            } else {
                buffer = new float[textureCoords.limit()];
                textureCoords.get(buffer);
            }
            textureCoordsArray = getMultiVectors(buffer);
        }

        if (indices != null) {
            indices.rewind();
            int[] ind;
            if (indices.hasArray()) {
                ind = indices.array();
            } else {
                ind = new int[indices.limit()];
                indices.get(ind);
            }
            Vector3d[] va = new Vector3d[ind.length];
            Color[] ca = new Color[ind.length];
            Vector3d[] ta = null;
            if (textureCoords != null) {
                ta = new Vector3d[ind.length];
            }

            for (int i = 0; i < ind.length; i++) {
                va[i] = verticesArray[ind[i]];
                ca[i] = colorsArray[ind[i]];
                if (ta != null) {
                    ta[i] = textureCoordsArray[ind[i]];
                }
            }
            verticesArray = va;
            colorsArray = ca;
            textureCoordsArray = ta;
        }

        return new Object[] {verticesArray, colorsArray, textureCoordsArray};
    }

    /**
     * Convert the buffer vertices into vertices array and put the segments in the scene
     * @param vertices a buffer containing vertices
     * @param colors a buffer containing the colors
     * @param defaultColor the color to use when colors is null
     * @param indices a buffer containing the index of the vertices to retrieve
     * @param drawingMode the drawing mode
     * @param stroke the Stroke to use to draw a segment
     */
    private void addSegments(FloatBuffer vertices, FloatBuffer colors, Color defaultColor, IntBuffer indices, Geometry.LineDrawingMode drawingMode, Appearance appearance) {
        Object[] arrays = getArrays(vertices, colors, defaultColor, null, indices);
        Vector3d[] verticesArray = (Vector3d[]) arrays[0];
        Color[] colorsArray = (Color[]) arrays[1];
        Vector3d[] v;
        Color[] c;
        double cumLength = 0;

        if (verticesArray.length <= 1) {
            return;
        }

        switch (drawingMode) {
            case SEGMENTS_STRIP :
                if (is2DView()) {
                    List<PolyLine> list = PolyLine.getPolyLines(verticesArray, colorsArray, G2DStroke.getStroke(appearance, 0), false);
                    for (PolyLine p : list) {
                        add(p);
                    }
                } else {
                    for (int i = 0; i < verticesArray.length - 1; i++) {
                        v = new Vector3d[] {verticesArray[i], verticesArray[i + 1]};
                        c = new Color[] {colorsArray[i], colorsArray[i + 1]};
                        try {
                            add(new Segment(v, c, G2DStroke.getStroke(appearance, cumLength), false));
                            cumLength += Segment.getLength(v);
                        } catch (InvalidPolygonException e) {
                            cumLength = 0;
                        }
                    }
                }
                break;
            case SEGMENTS_LOOP :
                if (is2DView()) {
                    List<PolyLine> list = PolyLine.getPolyLines(verticesArray, colorsArray, G2DStroke.getStroke(appearance, 0), true);
                    for (PolyLine p : list) {
                        add(p);
                    }
                } else {
                    for (int i = 0; i < verticesArray.length - 1; i++) {
                        v = new Vector3d[] {verticesArray[i], verticesArray[i + 1]};
                        c = new Color[] {colorsArray[i], colorsArray[i + 1]};
                        try {
                            add(new Segment(v, c, G2DStroke.getStroke(appearance, cumLength), false));
                            cumLength += Segment.getLength(v);
                        } catch (InvalidPolygonException e) {
                            cumLength = 0;
                        }
                    }
                    int n = verticesArray.length - 1;
                    v = new Vector3d[] {verticesArray[n], verticesArray[0]};
                    c = new Color[] {colorsArray[n], colorsArray[0]};
                    try {
                        add(new Segment(v, c, G2DStroke.getStroke(appearance, cumLength), false));
                    } catch (InvalidPolygonException e) { }
                }
                break;
            case SEGMENTS :
            default :
                for (int i = 0; i < verticesArray.length - 1; i += 2) {
                    v = new Vector3d[] {verticesArray[i], verticesArray[i + 1]};
                    c = new Color[] {colorsArray[i], colorsArray[i + 1]};
                    try {
                        add(new Segment(v, c, G2DStroke.getStroke(appearance, 0), is2DView()));
                    } catch (InvalidPolygonException e) { }
                }
                break;
        }
    }

    /**
     * Convert the buffer vertices into vertices array and put the triangles in the scene
     * @param vertices a buffer containing vertices
     * @param normals a buffer containing the normals (not used)
     * @param colors a buffer containing the colors
     * @param defaultColor the color to use when colors is null
     * @param indices a buffer containing the index of the vertices to retrieve
     * @param drawingMode the drawing mode
     */
    private void addTriangles(FloatBuffer vertices, FloatBuffer normals, FloatBuffer colors, Color defaultColor, IntBuffer indices, FloatBuffer textureCoords, final BufferedImage image, Texture texture, Geometry.FillDrawingMode drawingMode, G2DLightManager lightManager) {
        Object[] arrays = getArrays(vertices, colors, defaultColor, textureCoords, indices);
        Vector3d[] verticesArray = (Vector3d[]) arrays[0];
        Color[] colorsArray = (Color[]) arrays[1];
        Vector3d[] textureCoordsArray = (Vector3d[]) arrays[2];
        Vector3d[] v;
        Color[] c;
        Texture.Filter filter = Texture.Filter.NEAREST;

        if (texture != null) {
            filter = texture.getMagnificationFilter();
        }

        colorsArray = applyLighting(vertices, normals, indices, colorsArray, lightManager);

        switch (drawingMode) {
            case TRIANGLE_FAN :
                for (int i = 1; i < verticesArray.length - 1; i++) {
                    v = new Vector3d[] {verticesArray[0], verticesArray[i], verticesArray[i + 1]};
                    try {
                        if (image == null) {
                            c = new Color[] {colorsArray[0], colorsArray[i], colorsArray[i + 1]};
                            add(new Triangle(v, c, null));
                        } else {
                            add(new Triangle(v, new Vector3d[] {textureCoordsArray[0], textureCoordsArray[i], textureCoordsArray[i + 1]}, image, filter));
                        }
                    } catch (InvalidPolygonException e) { }
                }
                int n = verticesArray.length - 1;
                v = new Vector3d[] {verticesArray[0], verticesArray[n], verticesArray[1]};
                try {
                    if (image == null) {
                        c = new Color[] {colorsArray[0], colorsArray[n], colorsArray[1]};
                        add(new Triangle(v, c, null));
                    } else {
                        add(new Triangle(v, new Vector3d[] {textureCoordsArray[0], textureCoordsArray[n], textureCoordsArray[1]}, image, filter));
                    }
                } catch (InvalidPolygonException e) { }
                break;
            case TRIANGLE_STRIP :
                for (int i = 0; i < verticesArray.length - 2; i++) {
                    v = new Vector3d[] {verticesArray[i], verticesArray[i + 1], verticesArray[i + 2]};
                    try {
                        if (image == null) {
                            c = new Color[] {colorsArray[i], colorsArray[i + 1], colorsArray[i + 2]};
                            add(new Triangle(v, c, null));
                        } else {
                            add(new Triangle(v, new Vector3d[] {textureCoordsArray[i], textureCoordsArray[i + 1], textureCoordsArray[i + 2]}, image, filter));
                        }
                    } catch (InvalidPolygonException e) { }
                }
                break;
            case TRIANGLES :
            default :
                for (int i = 0; i < verticesArray.length - 2; i += 3) {
                    v = new Vector3d[] {verticesArray[i], verticesArray[i + 1], verticesArray[i + 2]};
                    try {
                        if (image == null) {
                            c = new Color[] {colorsArray[i], colorsArray[i + 1], colorsArray[i + 2]};
                            add(new Triangle(v, c, null));
                        } else {
                            add(new Triangle(v, new Vector3d[] {textureCoordsArray[i], textureCoordsArray[i + 1], textureCoordsArray[i + 2]}, image, filter));
                        }
                    } catch (InvalidPolygonException e) { }
                }
                break;
        }
    }

    /**
     * Convert an array of float into an array of Vector3d objects
     * @param vertices an array of float containing (vertices.length / G2DElementsBuffer.ELEMENT_SIZE) vectors coordinates
     * @param t the transformation to use for the projection
     * @param dir if true t.projectDirection() is used rather than t.project()
     * @return an array of Vector3d containing the vertices
     */
    private static final Vector3d[] getMultiVectors(final float[] vertices, final Transformation t, final boolean dir) {
        Vector3d[] v = new Vector3d[vertices.length / G2DElementsBuffer.ELEMENT_SIZE];
        if (dir) {
            int j = 0;
            for (int i = 0; i < v.length; i++) {
                v[i] = t.projectDirection(new Vector3d(vertices[j], vertices[j + 1], vertices[j + 2]));
                j += G2DElementsBuffer.ELEMENT_SIZE;
            }
        } else {
            int j = 0;
            for (int i = 0; i < v.length; i++) {
                v[i] = t.project(new Vector3d(vertices[j], vertices[j + 1], vertices[j + 2]));
                j += G2DElementsBuffer.ELEMENT_SIZE;
            }
        }

        return v;
    }

    /**
     * Convert an array of float into an array of Vector3d objects
     * @param vertices an array of float containing (vertices.length / G2DElementsBuffer.ELEMENT_SIZE) vectors coordinates
     * @return an array of Vector3d containing the vertices
     */
    private static final Vector3d[] getMultiVectors(final float[] vertices) {
        Vector3d[] v = new Vector3d[vertices.length / G2DElementsBuffer.ELEMENT_SIZE];
        int j = 0;
        for (int i = 0; i < v.length; i++) {
            v[i] = new Vector3d(vertices[j], vertices[j + 1], vertices[j + 2]);
            j += G2DElementsBuffer.ELEMENT_SIZE;
        }

        return v;
    }

    /**
     * Convert a float array into a Color array
     * @param colors a float array
     * @return an array of Color
     */
    private static final Color[] getMultiColors(final float[] colors) {
        Color[] c = new Color[colors.length / G2DElementsBuffer.ELEMENT_SIZE];
        int j = 0;
        Color prev = Color.BLACK;
        for (int i = 0; i < c.length; i++) {
            c[i] = new Color(colors[j], colors[j + 1], colors[j + 2], colors[j + 3]);
            if (prev.equals(c[i])) {
                c[i] = prev;
            }
            prev = c[i];
            j += G2DElementsBuffer.ELEMENT_SIZE;
        }

        return c;
    }

    /**
     * Perform per-vertex lighting
     */
    private Color[] applyLighting(FloatBuffer vertices, FloatBuffer normals, IntBuffer index, Color[] colors, G2DLightManager lightManager) {

        if (!lightManager.isLightningEnable() || vertices == null || normals == null
                || index == null || colors == null) {
            return colors;
        }

        Material mat = lightManager.getMaterial();
        if (mat == null) {
            return colors;
        }

        float[] vertexTransf = lightManager.getVertexTransform();
        float[] normalTransf = lightManager.getNormalTransform();
        //for transformed vertices camera is at origin.
        Vector3f camera = new Vector3f(0.f, 0.f , 0.f);
        Vector3f[] vertexArray = LightHelper.getIndexedVector3f(vertices, index, G2DElementsBuffer.ELEMENT_SIZE, vertexTransf);
        Vector3f[] normalArray = LightHelper.getIndexedVector3f(normals, index, G2DElementsBuffer.ELEMENT_SIZE, normalTransf);

        for (int i = 0; i < normalArray.length; ++i) {
            normalArray[i] = normalArray[i].getNormalized();
        }


        Color[] outColors = new Color[colors.length];
        boolean first = true;
        for (int i = 0; i < lightManager.getLightNumber(); ++i) {
            G2DLight l = (G2DLight)lightManager.getLight(i);

            if (l == null || !l.isEnable()) {
                continue;
            }

            outColors = LightHelper.applyLight(l, mat, camera, vertexArray, normalArray, colors, outColors, vertexTransf, !first);
            first = false;
        }
        return outColors;
    }
}
