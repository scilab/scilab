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
import org.scilab.forge.scirenderer.examples.utils.ExampleFrame;
import org.scilab.forge.scirenderer.examples.utils.MouseRotationAdapter;
import org.scilab.forge.scirenderer.lightning.Light;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureDrawer;
import org.scilab.forge.scirenderer.texture.TextureDrawingTools;
import org.scilab.forge.scirenderer.tranformations.Rotation;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.forge.scirenderer.utils.shapes.geometry.CubeFactory;

import java.awt.Dimension;

/**
 * @author Pierre Lando
 */
@SuppressWarnings(value = { "serial" })
public final class LightenedCube extends ExampleFrame {

    /**
     * Frame title.
     */
    private static final String TITLE = "Lightening Example";

    /**
     * Private constructor.
     * This is the main class.
     */
    private LightenedCube() {
        setTitle(TITLE);

        /**
         * A MouseRotationAdapter to add some interactivity.
         */
        final MouseRotationAdapter mra = new MouseRotationAdapter(
            new Rotation(),
            getCanvas()
        );
        getPanel().addMouseListener(mra);

        getCanvas().setMainDrawer(new LightenedCubeDrawer(mra, getCanvas()));
    }

    /**
     * Main function.
     * @param arguments launch arguments. Ignored.
     */
    public static void main(String[] arguments) {
        new LightenedCube();
    }

    /**
     * The lightened cube drawer.
     */
    private final class LightenedCubeDrawer implements Drawer {

        /**
         * A sprite to represent light position.
         */
        private final Texture sprite;

        /**
         * The cube geometry.
         */
        private final Geometry cube;

        /**
         * The mouse rotation adapter used for interactivity.
         */
        private final MouseRotationAdapter mra;

        /**
         * Default constructor.
         * @param mouseRotationAdapter the mouse rotation adapter used for interactivity.
         * @param canvas the canvas where the draw will be performed.
         */
        public LightenedCubeDrawer(MouseRotationAdapter mouseRotationAdapter, Canvas canvas) {
            sprite = createSprite(canvas);
            cube = CubeFactory.createCube(canvas, 10);
            mra = mouseRotationAdapter;
        }

        /**
         * Sprite constructor.
         * @param canvas the canvas where the sprite is created.
         * @return the sprite representing the light.
         */
        private Texture createSprite(Canvas canvas) {
            Texture newSprite = canvas.getTextureManager().createTexture();
            newSprite.setDrawer(new TextureDrawer() {

                @Override
                public void draw(TextureDrawingTools drawingTools) {
                    drawingTools.fillDisc(0, 0, 10, new Color(1, 0, 0));
                }

                @Override
                public Dimension getTextureSize() {
                    return new Dimension(16, 16);
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
            dt.clear(new Color());

            try {
                Transformation projection = TransformationFactory.getPreferredAspectRatioTransformation(dt.getCanvas().getDimension(), 1f);
                dt.getTransformationManager().getProjectionStack().push(projection);
                dt.getTransformationManager().getModelViewStack().push(TransformationFactory.getScaleTransformation(.2, .2, .2));

                dt.getLightManager().setLightningEnable(true);
                Light light = dt.getLightManager().getLight(0);
                light.setEnable(true);
                light.setAmbientColor(new Color(.5f, .5f, .5f));
                light.setDiffuseColor(new Color(.5f, 0, 0));
                light.setSpecularColor(new Color(.4f, .4f, 0));

                Vector3d lightPosition = new Vector3d(0, 2, -2);
                light.setPosition(lightPosition);
                dt.draw(sprite, AnchorPosition.CENTER, lightPosition);


                dt.getTransformationManager().getModelViewStack().pushRightMultiply(TransformationFactory.getRotationTransformation(mra.getRotation()));
                dt.draw(cube);
            } catch (SciRendererException ignored) {
                // Should not occur.
            }
        }

        @Override
        public boolean is2DView() {
            return false;
        }
    }
}
