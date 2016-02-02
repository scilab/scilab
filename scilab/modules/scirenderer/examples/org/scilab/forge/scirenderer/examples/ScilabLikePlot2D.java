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
import org.scilab.forge.scirenderer.examples.utils.ExampleFrame;
import org.scilab.forge.scirenderer.examples.utils.MouseRotationAdapter;
import org.scilab.forge.scirenderer.ruler.DefaultRulerModel;
import org.scilab.forge.scirenderer.ruler.RulerDrawer;
import org.scilab.forge.scirenderer.ruler.RulerModel;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureDrawer;
import org.scilab.forge.scirenderer.texture.TextureDrawingTools;
import org.scilab.forge.scirenderer.tranformations.Rotation;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

import java.awt.Dimension;

/**
 * @author Pierre Lando
 */
@SuppressWarnings(value = { "serial" })
public final class ScilabLikePlot2D extends ExampleFrame {

    /**
     * Frame title.
     */
    private static final String TITLE = "Scilab like 'plot2D' Example";

    /**
     * Private constructor.
     * This is the main class.
     */
    private ScilabLikePlot2D() {
        setTitle(TITLE);

        final MouseRotationAdapter mra = new MouseRotationAdapter(
            new Rotation(Math.toRadians(0), new Vector3d(1, 0, 0)),
            getCanvas()
        );
        getPanel().addMouseListener(mra);

        getCanvas().setMainDrawer(new Plot2DDrawer(getCanvas(), mra));
    }

    /**
     * Main function.
     * @param arguments main arguments.
     */
    public static void main(String[] arguments) {
        new ScilabLikePlot2D();
    }

    /**
     * A plot2d like drawer
     *
     * @author Pierre Lando
     */
    public static class Plot2DDrawer implements Drawer {

        // Some geometry
        private final DefaultGeometry plot1;
        private final DefaultGeometry plot2;

        // Some sprite and position buffer for it.
        private final ElementsBuffer plot3Data;
        private final Texture sprite;

        // Some rulers.
        private final RulerDrawer rulerDrawer;
        private DefaultRulerModel xRulerModel;
        private DefaultRulerModel yRulerModel;

        private final MouseRotationAdapter mra;


        /**
         * Default constructor.
         * @param canvas parent {@link org.scilab.forge.scirenderer.Canvas}
         * @param mra a {@link org.scilab.forge.scirenderer.examples.utils.MouseRotationAdapter} to add some interactivity.
         */
        public Plot2DDrawer(Canvas canvas, MouseRotationAdapter mra) {

            this.mra = mra;
            /**
             * Create the ruler drawer.
             */
            rulerDrawer = new RulerDrawer(canvas.getTextureManager());

            // Create a geometry
            ElementsBuffer sinData1 = canvas.getBuffersManager().createElementsBuffer();
            sinData1.setData(getSinData(3, 100, 10), 4);
            plot1 = new DefaultGeometry();
            plot1.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
            plot1.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS_STRIP);
            plot1.setVertices(sinData1);

            // Create a second geometry
            ElementsBuffer sinData2 = canvas.getBuffersManager().createElementsBuffer();
            sinData2.setData(getSinData(2, 100, 10), 4);
            plot2 = new DefaultGeometry();
            plot2.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
            plot2.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS_STRIP);
            plot2.setVertices(sinData2);

            // Create a position buffer to draw some sprite.
            plot3Data = canvas.getBuffersManager().createElementsBuffer();
            plot3Data.setData(getSinData(1, 60, 10), 4);

            // Create a sprite.
            sprite = canvas.getTextureManager().createTexture();
            sprite.setDrawer(getSpriteDrawer());
        }

        @Override
        public void draw(DrawingTools drawingTools) {
            drawingTools.clear(new Color(1f, 1f, 1f));

            try {
                Transformation projection = TransformationFactory.getPreferredAspectRatioTransformation(drawingTools.getCanvas().getDimension(), 1f);
                drawingTools.getTransformationManager().getProjectionStack().push(projection);
                drawingTools.getTransformationManager().getModelViewStack().pushRightMultiply(TransformationFactory.getScaleTransformation(.1, .1, .1));
                drawingTools.getTransformationManager().getModelViewStack().pushRightMultiply(TransformationFactory.getRotationTransformation(mra.getRotation()));
                Appearance appearance = new Appearance();

                // Draw first geometry.
                appearance.setLineColor(new Color(0, 1, 0));
                appearance.setLineWidth(4);
                drawingTools.draw(plot1, appearance);

                // Draw second geometry.
                appearance.setLineColor(new Color(0, 0, 0));
                appearance.setLineWidth(1);
                drawingTools.draw(plot2, appearance);

                // Draw sprites.
                drawingTools.draw(sprite, AnchorPosition.CENTER, plot3Data);

                // Draw rulers.
                rulerDrawer.draw(drawingTools, getXRulerModel());
                rulerDrawer.draw(drawingTools, getYRulerModel());
            } catch (SciRendererException ignored) {
                // Should not occur.
            }
        }


        @Override
        public boolean is2DView() {
            return true;
        }

        /**
         * Return a float array filled with some "y = sin(theta * x)" data.
         * @param theta the data period.
         * @param density the data density (number of point).
         * @param bound the data bounds (result have x in [-bound, bound[).
         * @return a float array filled with some "y = sin(theta * x)" data.
         */
        public float[] getSinData(float theta, int density, float bound) {
            float[] sinData = new float[density * 4];
            for (int i = 0; i < density; i++) {
                float x = bound * 2 * (i - density / 2f) / density; // x : [-bound, bound[
                float y = (float) Math.sin(x * theta);

                sinData[i * 4] = x;
                sinData[i * 4 + 1] = y;
                sinData[i * 4 + 2] = 0;
                sinData[i * 4 + 3] = 1;
            }
            return sinData;
        }

        /**
         * Return a sprite drawer.
         * @return a sprite drawer.
         */
        public TextureDrawer getSpriteDrawer() {
            return new TextureDrawer() {

                @Override
                public void draw(TextureDrawingTools drawingTools) {
                    Appearance appearance = new Appearance();
                    appearance.setLineColor(new Color(1, 0, 0));
                    drawingTools.drawPolyline(new int[] {
                                                  -5, 0,
                                                  +5, 0
                                              }, appearance);
                    drawingTools.drawPolyline(new int[] {
                                                  0, -5,
                                                  0, +5
                                              }, appearance);
                }

                @Override
                public Dimension getTextureSize() {
                    return new Dimension(16, 16);
                }

                @Override
                public OriginPosition getOriginPosition() {
                    return OriginPosition.CENTER;
                }
            };
        }

        /**
         * Return the X ruler model.
         * @return the X ruler model.
         */
        public RulerModel getXRulerModel() {
            if (xRulerModel == null) {
                xRulerModel = new DefaultRulerModel();
                xRulerModel.setFirstPoint(new Vector3d(-10, -1, 0));
                xRulerModel.setSecondPoint(new Vector3d(10, -1, 0));
                xRulerModel.setValues(10, -10);
                xRulerModel.setLineVisible(true);
                xRulerModel.setTicksDirection(new Vector3d(0, -1, 0));
            }
            return xRulerModel;
        }

        /**
         * Return the Y ruler model.
         * @return the Y ruler model.
         */
        public RulerModel getYRulerModel() {
            if (yRulerModel == null) {
                yRulerModel = new DefaultRulerModel();
                yRulerModel.setFirstPoint(new Vector3d(-10, -1, 0));
                yRulerModel.setSecondPoint(new Vector3d(-10, 1, 0));
                yRulerModel.setValues(1, -1);
                yRulerModel.setLineVisible(true);
                yRulerModel.setTicksDirection(new Vector3d(-1, 0, 0));
            }
            return yRulerModel;
        }
    }
}
