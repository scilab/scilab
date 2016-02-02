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
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
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
public final class Sprites extends ExampleFrame {

    /**
     * Frame title.
     */
    private static final String TITLE = "Sprites Example";

    /**
     * Private constructor.
     * This is the main class.
     */
    private Sprites() {
        setTitle(TITLE);

        /**
         * Add a mouse rotation adapter.
         */
        final MouseRotationAdapter mra = new MouseRotationAdapter(
            new Rotation(Math.toRadians(90), new Vector3d(1, 0, 0)),
            getCanvas()
        );
        getPanel().addMouseListener(mra);

        getCanvas().setMainDrawer(new SpritesDrawer(getCanvas(), mra));
    }

    /**
     * Main function.
     * @param arguments launch arguments. Ignored.
     */
    public static void main(String[] arguments) {
        new Sprites();
    }

    /**
     * Sprites drawer.
     */
    public static class SpritesDrawer implements Drawer {
        private static final int SPRITE_RADIUS = 5;
        private final int cubeSize = 8;
        private final MouseRotationAdapter mouseRotationAdapter;
        private final ElementsBuffer position;
        private final Texture sprite;

        /**
         * Constructor.
         * @param canvas the canvas where the sprites are drawn.
         * @param mra the mouse listener used for the interaction.
         */
        public SpritesDrawer(Canvas canvas, MouseRotationAdapter mra) {
            mouseRotationAdapter = mra;
            sprite = createSprite(canvas);

            position = canvas.getBuffersManager().createElementsBuffer();
            position.setData(createData(), 4);
        }

        @Override
        public void draw(DrawingTools drawingTools) {
            drawingTools.clear(new Color(1, 1, 1));

            try {
                Transformation projection = TransformationFactory.getPreferredAspectRatioTransformation(drawingTools.getCanvas().getDimension(), 1f);
                drawingTools.getTransformationManager().getProjectionStack().push(projection);
                drawingTools.getTransformationManager().getModelViewStack().push(TransformationFactory.getScaleTransformation(.6, .6, .6));
                drawingTools.getTransformationManager().getModelViewStack().pushRightMultiply(TransformationFactory.getRotationTransformation(mouseRotationAdapter.getRotation()));
                drawingTools.draw(sprite, AnchorPosition.CENTER, position);
            } catch (SciRendererException ignored) {
                // Should not occur.
            }
        }

        @Override
        public boolean is2DView() {
            return false;
        }

        /**
         * Create sprite position data.
         * @return an array filled with the position where the sprite will be drawn.
         */
        private float[] createData() {
            float[] data = new float[4 * cubeSize * cubeSize * cubeSize];
            int index = 0;
            for (float i = 0; i < cubeSize; i++) {
                for (float j = 0; j < cubeSize; j++) {
                    for (float k = 0; k < cubeSize; k++) {
                        data[index++] = (i / (cubeSize - 1)) * 2 - 1;
                        data[index++] = (j / (cubeSize - 1)) * 2 - 1;
                        data[index++] = (k / (cubeSize - 1)) * 2 - 1;
                        data[index++] = 1;
                    }
                }
            }
            return data;
        }

        /**
         * Sprite constructor.
         * @param canvas the canvas where the sprite will be drawn.
         * @return the example sprite.
         */
        private Texture createSprite(Canvas canvas) {
            Texture newSprite = canvas.getTextureManager().createTexture();
            newSprite.setDrawer(new TextureDrawer() {

                @Override
                public void draw(TextureDrawingTools drawingTools) {
                    Appearance appearance = new Appearance();
                    appearance.setLineColor(new Color(0, 0, 0));
                    drawingTools.drawPolyline(new int[] {
                                                  -SPRITE_RADIUS, 0,
                                                  +SPRITE_RADIUS, 0,
                                              }, appearance);

                    drawingTools.drawPolyline(new int[] {
                                                  0, +SPRITE_RADIUS,
                                                  0, -SPRITE_RADIUS
                                              }, appearance);
                }

                @Override
                public Dimension getTextureSize() {
                    return new Dimension(SPRITE_RADIUS * 2 + 1, SPRITE_RADIUS * 2 + 1);
                }

                @Override
                public OriginPosition getOriginPosition() {
                    return OriginPosition.CENTER;
                }
            });

            return newSprite;
        }
    }
}
