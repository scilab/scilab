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
import org.scilab.forge.scirenderer.picking.PickingTask;
import org.scilab.forge.scirenderer.picking.PickingTools;
import org.scilab.forge.scirenderer.renderer.Renderer;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureDrawer;
import org.scilab.forge.scirenderer.texture.TextureDrawingTools;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.forge.scirenderer.utils.shapes.geometry.CubeFactory;

import java.awt.Dimension;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.nio.FloatBuffer;

/**
 * @author Pierre Lando
 */
@SuppressWarnings(value = { "serial" })
public final class ChickenPoxCube extends ExampleFrame {

    /**
     * Frame title.
     */
    private static final String TITLE = "Rotating Cube Example";

    /**
     * The last used transformation.
     */
    private Transformation lastTransformation;

    /**
     * The clicked point.
     */
    private final ElementsBuffer spritePosition;

    /**
     * Private constructor.
     * This is the main class.
     */
    private ChickenPoxCube() {
        setTitle(TITLE);

        /**
         * Add a {@link MouseUnprojectAdapter}
         */
        MouseListener mua = new MouseUnprojectAdapter(getCanvas());
        getPanel().addMouseListener(mua);

        getCanvas().setMainDrawer(new SimpleCubeDrawer(getCanvas()));
        spritePosition = getCanvas().getBuffersManager().createElementsBuffer();

        animate(true);
    }

    /**
     * Main function.
     * @param arguments launch arguments. Ignored.
     */
    public static void main(String[] arguments) {
        new ChickenPoxCube();
    }

    /**
     * A simple cube drawer.
     */
    private final class SimpleCubeDrawer implements Drawer {

        /**
         * The sprite size.
         */
        private static final int SPRITE_RADIUS = 15;

        /**
         * The cube geometry.
         */
        private final Geometry cube;

        /**
         * A rendering proxy.
         */
        private final Renderer renderer;
        private final Texture sprite;

        /**
         * Default constructor.
         * @param canvas the canvas where the cube is drawn.
         */
        public SimpleCubeDrawer(Canvas canvas) {
            cube = CubeFactory.createCube(canvas, 3, true);
            renderer = canvas.getRendererManager().createRenderer();

            final Appearance appearance = new Appearance();
            appearance.setLineColor(new Color(0, 0, 0));
            appearance.setLineWidth(4);

            sprite = createSprite(canvas);

            renderer.setDrawer(new Drawer() {
                @Override
                public void draw(DrawingTools dt) {
                    try {
                        dt.draw(cube, appearance);
                    } catch (SciRendererException ignored) {
                    }
                }

                @Override
                public boolean is2DView() {
                    return false;
                }
            });
        }

        /**
         * Sprite creator.
         * @param canvas the canvas where the sprite will be drawn.
         * @return a sprite to represent clicked point.
         */
        private Texture createSprite(Canvas canvas) {
            Texture newSprite = canvas.getTextureManager().createTexture();
            newSprite.setDrawer(new TextureDrawer() {

                @Override
                public void draw(TextureDrawingTools drawingTools) {
                    drawingTools.fillDisc(0, 0, SPRITE_RADIUS, new Color(1, 0, 0));
                }

                @Override
                public Dimension getTextureSize() {
                    return new Dimension(SPRITE_RADIUS * 2, SPRITE_RADIUS * 2);
                }

                @Override
                public OriginPosition getOriginPosition() {
                    return OriginPosition.CENTER;
                }
            });

            return newSprite;
        }

        @Override
        public void draw(DrawingTools dt) {
            try {
                double t = System.currentTimeMillis() / 50.;
                dt.clear(new Color());
                Transformation projection = TransformationFactory.getPreferredAspectRatioTransformation(dt.getCanvas().getDimension(), 1f);
                Transformation perspective = TransformationFactory.getPerspectiveTransformation(1, 10, 45);
                Transformation displacement = TransformationFactory.getTranslateTransformation(0, 0, -5);
                dt.getTransformationManager().getProjectionStack().push(projection.rightTimes(perspective.rightTimes(displacement)));
                dt.getTransformationManager().getModelViewStack().pushRightMultiply(TransformationFactory.getRotationTransformation(t, 1, 2, 3));
                lastTransformation = dt.getTransformationManager().getTransformation();
                dt.draw(renderer);
                dt.draw(sprite, AnchorPosition.CENTER, spritePosition);
            } catch (SciRendererException ignored) {
                // Should not occur.
            }
        }

        @Override
        public boolean is2DView() {
            return false;
        }
    }

    /**
     * This class is a simple example of picking usage.
     * @author Pierre Lando
     */
    private final class MouseUnprojectAdapter extends MouseAdapter {

        private final Canvas canvas;

        /**
         * Default constructor.
         * @param canvas the listened canvas.
         */
        public MouseUnprojectAdapter(Canvas canvas) {
            this.canvas = canvas;
        }

        @Override
        public void mousePressed(final MouseEvent e) {
            canvas.getPickingManager().addPickingTask(new PickingTask() {
                @Override
                public void perform(PickingTools pickingTools) {
                    Vector3d l = pickingTools.getUnderlyingPoint(e.getPoint());
                    if ((lastTransformation != null) && (l.getZ() != 1)) {
                        Vector3d lastClickedPoint = lastTransformation.unproject(l);
                        FloatBuffer data = spritePosition.getData();
                        FloatBuffer newData;
                        if (data == null) {
                            newData = FloatBuffer.allocate(4);
                            newData.rewind();
                        } else {
                            data.rewind();
                            newData = FloatBuffer.allocate(data.limit() + 4);
                            float[] dataArray = new float[data.limit()];
                            data.get(dataArray);
                            newData.put(dataArray);
                        }
                        newData.put(lastClickedPoint.getDataAsFloatArray(4));
                        spritePosition.setData(newData, 4);
                    }
                }
            });
        }
    }
}
