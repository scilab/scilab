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
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.TextEntity;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureDrawer;
import org.scilab.forge.scirenderer.texture.TextureDrawingTools;
import org.scilab.forge.scirenderer.tranformations.DegenerateMatrixException;
import org.scilab.forge.scirenderer.tranformations.Rotation;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.forge.scirenderer.utils.shapes.geometry.TetrahedronFactory;

import java.awt.Dimension;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

/**
 * @author Pierre Lando
 */
@SuppressWarnings(value = { "serial" })
public final class LineRendering extends ExampleFrame {

    /**
     * Frame title.
     */
    private static final String TITLE = "Line Rendering Example";

    private final LineDrawer drawer;

    /**
     * Private constructor.
     * This is the main class.
     */
    private LineRendering() {
        setTitle(TITLE);

        /**
         * Add a mouse rotation adapter.
         */
        final MouseRotationAdapter mra = new MouseRotationAdapter(
            new Rotation(Math.toRadians(90), new Vector3d(1, 0, 0)),
            getCanvas()
        );
        getPanel().addMouseListener(mra);

        drawer = new LineDrawer(getCanvas(), mra);
        getCanvas().setMainDrawer(drawer);

        /**
         * Add a key listener.
         */
        addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_F) {
                    drawer.switchFace();
                    getCanvas().redraw();
                }
            }
        });
    }

    /**
     * Main function.
     * @param arguments launch arguments. Ignored.
     */
    public static void main(String[] arguments) {
        new LineRendering();
    }

    /**
     * @author Pierre Lando
     */
    private final class LineDrawer implements Drawer {
        private static final String MESSAGE_TEXT = "Press 'F' to switch culling mode";
        private final DefaultGeometry tetrahedron;
        private final MouseRotationAdapter mra;
        private final Texture message;

        /**
         * Default constructor.
         * @param canvas the canvas where the drawing will be performed.
         * @param mouseRotationAdapter the {@link org.scilab.forge.scirenderer.examples.utils.MouseRotationAdapter} used for interactivity.
         */
        public LineDrawer(Canvas canvas, MouseRotationAdapter mouseRotationAdapter) {
            tetrahedron = TetrahedronFactory.createTetrahedron(canvas);
            tetrahedron.setFaceCullingMode(Geometry.FaceCullingMode.CCW);
            message = createMessage(canvas);
            mra = mouseRotationAdapter;
        }

        /**
         * Switch culled face.
         */
        public void switchFace() {
            switch (tetrahedron.getFaceCullingMode()) {
                case CCW:
                    tetrahedron.setFaceCullingMode(Geometry.FaceCullingMode.CW);
                    break;
                default:
                    tetrahedron.setFaceCullingMode(Geometry.FaceCullingMode.CCW);
            }
        }

        @Override
        public void draw(DrawingTools dt) {
            dt.clear(new Color(1f, 1f, 1f));

            try {
                Transformation projection = TransformationFactory.getPreferredAspectRatioTransformation(dt.getCanvas().getDimension(), 1f);
                dt.getTransformationManager().getProjectionStack().push(projection);
            } catch (DegenerateMatrixException ignored) {
                // Should not occur.
            }

            try {
                dt.draw(message, AnchorPosition.UPPER_LEFT, new Vector3d(-.95, .95, 0));
            } catch (SciRendererException ignored) {
            }

            try {
                dt.getTransformationManager().getModelViewStack().pushRightMultiply(TransformationFactory.getScaleTransformation(.5, .5, .5));

                dt.getTransformationManager().getModelViewStack().pushRightMultiply(TransformationFactory.getRotationTransformation(mra.getRotation()));

                Appearance appearance = new Appearance();
                appearance.setLineColor(new Color(.2f, .2f, .2f));
                appearance.setLineWidth(3);
                dt.draw(tetrahedron, appearance);
            } catch (SciRendererException ignored) {
                // Should not occur.
            }
        }

        @Override
        public boolean is2DView() {
            return false;
        }

        /**
         * Create a help message.
         * @param canvas the canvas where the message sprite is created.
         * @return a sprite that draws the message.
         */
        private Texture createMessage(final Canvas canvas) {
            final TextEntity text = new TextEntity(MESSAGE_TEXT);
            Texture texture = canvas.getTextureManager().createTexture();
            texture.setDrawer(new TextureDrawer() {

                @Override
                public void draw(TextureDrawingTools drawingTools) {
                    drawingTools.draw(text, 0, 0);
                }

                @Override
                public Dimension getTextureSize() {
                    return text.getSize();
                }

                @Override
                public OriginPosition getOriginPosition() {
                    return OriginPosition.UPPER_LEFT;
                }
            });

            return texture;
        }
    }
}
