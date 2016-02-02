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

package org.scilab.forge.scirenderer.examples;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.Drawer;
import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.buffers.IndicesBuffer;
import org.scilab.forge.scirenderer.examples.utils.ExampleFrame;
import org.scilab.forge.scirenderer.examples.utils.MouseRotationAdapter;
import org.scilab.forge.scirenderer.ruler.DefaultRulerModel;
import org.scilab.forge.scirenderer.ruler.RulerDrawer;
import org.scilab.forge.scirenderer.ruler.RulerModel;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.tranformations.Rotation;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;

/**
 * Simple example of SciRenderer usage.
 *
 * @author Pierre Lando
 */
@SuppressWarnings(value = { "serial" })
public final class MilkDrop extends ExampleFrame {

    /**
     * Frame title.
     */
    private static final String TITLE = "MilkDrop example";

    /**
     * Private constructor.
     * This is the main class.
     */
    private MilkDrop() {
        setTitle(TITLE);

        /**
         * Add a mouse rotation adapter for interactivity.
         */
        final MouseRotationAdapter mra = new MouseRotationAdapter(
            new Rotation(Math.toRadians(90), new Vector3d(1, 0, 0)),
            getCanvas()
        );
        getPanel().addMouseListener(mra);

        getCanvas().setMainDrawer(new MilkDropDrawer(getCanvas(), mra));
    }

    /**
     * Main function.
     * @param arguments main arguments.
     */
    public static void main(String[] arguments) {
        new MilkDrop();
    }

    /**
     * @author Pierre Lando
     */
    private final class MilkDropDrawer implements Drawer {
        private final RulerDrawer rulerDrawer;
        private final Geometry milkDrop;
        private final MouseRotationAdapter mouseRotationAdapter;
        private final Appearance appearance;

        private DefaultRulerModel xRulerModel;
        private DefaultRulerModel yRulerModel;
        private DefaultRulerModel zRulerModel;

        /**
         * Default constructor.
         * @param canvas the canvas where the draw will be done.
         * @param mra the {@link org.scilab.forge.scirenderer.examples.utils.MouseRotationAdapter} used for interactivity.
         */
        public MilkDropDrawer(Canvas canvas, MouseRotationAdapter mra) {
            this.mouseRotationAdapter = mra;

            appearance = new Appearance();
            appearance.setLineWidth(1);
            appearance.setLineColor(new Color(0, 0, 0));
            rulerDrawer = new RulerDrawer(canvas.getTextureManager());
            milkDrop = new MilkDropGeometry(canvas);
        }

        @Override
        public void draw(DrawingTools dt) {
            dt.clear(new Color(1f, 1f, 1f));

            try {
                Transformation projection = TransformationFactory.getPreferredAspectRatioTransformation(dt.getCanvas().getDimension(), 1f);
                dt.getTransformationManager().getProjectionStack().push(projection);
                dt.getTransformationManager().getModelViewStack().pushRightMultiply(TransformationFactory.getScaleTransformation(.03, .03, .03));
                dt.getTransformationManager().getModelViewStack().pushRightMultiply(TransformationFactory.getRotationTransformation(mouseRotationAdapter.getRotation()));
                dt.draw(milkDrop, appearance);
            } catch (SciRendererException ignored) {
                // Should not occur.
            }

            rulerDrawer.draw(dt, getXRulerModel());
            rulerDrawer.draw(dt, getYRulerModel());
            rulerDrawer.draw(dt, getZRulerModel());
        }

        @Override
        public boolean is2DView() {
            return false;
        }

        /**
         * X ruler model getter.
         * @return the X ruler model.
         */
        public RulerModel getXRulerModel() {
            if (xRulerModel == null) {
                xRulerModel = new DefaultRulerModel();
                xRulerModel.setFirstPoint(new Vector3d(-20, 20, 0));
                xRulerModel.setSecondPoint(new Vector3d(20, 20, 0));
                xRulerModel.setValues(-20, 20);
                xRulerModel.setLineVisible(true);
                xRulerModel.setTicksDirection(new Vector3d(0, 1, 0));
            }
            return xRulerModel;
        }

        /**
         * Y ruler model getter.
         * @return the Y ruler model.
         */
        public RulerModel getYRulerModel() {
            if (yRulerModel == null) {
                yRulerModel = new DefaultRulerModel();
                yRulerModel.setFirstPoint(new Vector3d(20, -20, 0));
                yRulerModel.setSecondPoint(new Vector3d(20, 20, 0));
                yRulerModel.setValues(-20, 20);
                yRulerModel.setLineVisible(true);
                yRulerModel.setTicksDirection(new Vector3d(1, 0, 0));
            }
            return yRulerModel;
        }

        /**
         * Z ruler model getter.
         * @return the Z ruler model.
         */
        public RulerModel getZRulerModel() {
            if (zRulerModel == null) {
                zRulerModel = new DefaultRulerModel();
                zRulerModel.setFirstPoint(new Vector3d(20, 20, 20));
                zRulerModel.setSecondPoint(new Vector3d(20, 20, -20));
                zRulerModel.setValues(-20, 20);
                zRulerModel.setLineVisible(true);
                zRulerModel.setTicksDirection(new Vector3d(1, 1, 0));
            }
            return zRulerModel;
        }
    }

    /**
     * A geometry implementation representing a MilkDrop.
     * @author Pierre Lando
     */
    private final class MilkDropGeometry extends DefaultGeometry {
        private static final int HALF_SIDE = 20;
        private static final int SIDE = 2 * HALF_SIDE + 1;
        private static final int ELEMENTS_SIZE = 4;


        /**
         * @param canvas the canvas where the buffers are created.
         */
        public MilkDropGeometry(Canvas canvas) {
            FloatBuffer vertices = FloatBuffer.allocate(SIDE * SIDE * ELEMENTS_SIZE);
            FloatBuffer colors = FloatBuffer.allocate(SIDE * SIDE * ELEMENTS_SIZE);
            IntBuffer wireIndices = IntBuffer.allocate((SIDE - 1) * (SIDE - 1) * 8);
            IntBuffer indices = IntBuffer.allocate((SIDE - 1) * (SIDE - 1) * 6);

            fillVerticesAndColors(vertices, colors);
            fillIndices(indices, wireIndices);

            ElementsBuffer vertexBuffer = canvas.getBuffersManager().createElementsBuffer();
            ElementsBuffer colorBuffer = canvas.getBuffersManager().createElementsBuffer();
            IndicesBuffer indicesBuffer = canvas.getBuffersManager().createIndicesBuffer();
            IndicesBuffer wireIndicesBuffer = canvas.getBuffersManager().createIndicesBuffer();

            vertexBuffer.setData(vertices, ELEMENTS_SIZE);
            colorBuffer.setData(colors, ELEMENTS_SIZE);
            indicesBuffer.setData(indices);
            wireIndicesBuffer.setData(wireIndices);

            setFillDrawingMode(FillDrawingMode.TRIANGLES);
            setLineDrawingMode(LineDrawingMode.SEGMENTS);
            setPolygonOffsetMode(true);
            setVertices(vertexBuffer);
            setColors(colorBuffer);
            setIndices(indicesBuffer);
            setWireIndices(wireIndicesBuffer);
        }

        /**
         * Fill indices buffers.
         * @param indices the triangles indices to fill.
         * @param wireIndices the segments indices to fill.s
         */
        private void fillIndices(IntBuffer indices, IntBuffer wireIndices) {
            for (int x = 0; x < SIDE - 1; x++) {
                for (int y = 0; y < SIDE - 1; y++) {
                    /**
                     * (x, y)  -- (x+1, y)
                     *    |          |
                     *    |          |
                     * (x, y+1)--(x+1, y+1)
                     *
                     *  (x,y) => (x*SIDE + y);
                     *
                     */

                    if (((x >= 0) && (y < 0)) || ((x < 0) && (y >= 0))) {
                        indices.put((x * SIDE) + y);
                        indices.put(((x + 1) * SIDE) + y);
                        indices.put(((x + 1) * SIDE) + (y + 1));

                        indices.put((x * SIDE) + y);
                        indices.put(((x + 1) * SIDE) + (y + 1));
                        indices.put((x * SIDE) + (y + 1));
                    } else {
                        indices.put((x * SIDE) + y);
                        indices.put(((x + 1) * SIDE) + y);
                        indices.put((x * SIDE) + (y + 1));

                        indices.put(((x + 1) * SIDE) + y);
                        indices.put(((x + 1) * SIDE) + (y + 1));
                        indices.put((x * SIDE) + (y + 1));
                    }

                    wireIndices.put((x * SIDE) + y);
                    wireIndices.put((x * SIDE) + (y + 1));
                    wireIndices.put((x * SIDE) + (y + 1));
                    wireIndices.put(((x + 1) * SIDE) + (y + 1));
                    wireIndices.put(((x + 1) * SIDE) + (y + 1));
                    wireIndices.put(((x + 1) * SIDE) + y);
                    wireIndices.put(((x + 1) * SIDE) + y);
                    wireIndices.put((x * SIDE) + y);
                }
            }
        }

        /**
         * Fill the given buffer with vertices data and color data.
         * @param vertices the vertices buffer.
         * @param colors the colors buffer.
         */
        private void fillVerticesAndColors(FloatBuffer vertices, FloatBuffer colors) {
            vertices.rewind();
            colors.rewind();
            for (int x = -HALF_SIDE; x <= HALF_SIDE; x++) {
                for (int y = -HALF_SIDE; y <= HALF_SIDE; y++) {
                    double d = Math.sqrt((x * x) + (y * y));
                    double z = HALF_SIDE * Math.cos(d) * Math.exp(-d / 6) / 2;

                    vertices.put(x);
                    vertices.put(y);
                    vertices.put((float) z);
                    vertices.put(1);

                    colors.put((float) z);
                    colors.put((float) (1 - z));
                    colors.put(0);
                    colors.put(1);
                }
            }
            vertices.rewind();
            colors.rewind();
        }
    }
}
