/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
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
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.TextEntity;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureDrawer;
import org.scilab.forge.scirenderer.texture.TextureDrawingTools;
import org.scilab.forge.scirenderer.texture.TextureManager;
import org.scilab.forge.scirenderer.tranformations.DegenerateMatrixException;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

import java.awt.Dimension;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

/**
 * A basic example featuring rotatable 2D sprites.
 *
 * @author Pierre Lando
 * @author Manuel Juliachs
 */
@SuppressWarnings(value = { "serial" })
public final class RotatableSprite extends ExampleFrame {

    /**
     * Frame title.
     */
    private static final String TITLE = "Rotatable Sprites Example";

    /**
     * Private constructor.
     * This is the main class.
     */
    private RotatableSprite() {
        setTitle(TITLE);

        final RotatableSpriteDrawer drawer = new RotatableSpriteDrawer(getCanvas());
        getCanvas().setMainDrawer(drawer);

        /* Key listener */
        getPanel().addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_T) {
                    drawer.toggleAnchorDisplay();
                } else if (e.getKeyCode() == KeyEvent.VK_D) {
                    drawer.switchRotationDirection();
                } else if (e.getKeyCode() == KeyEvent.VK_M) {
                    drawer.toggleMessageDisplay();
                }
            }
        });

        animate(true);
    }

    /**
     * Main function.
     * @param arguments launch arguments. Ignored.
     */
    public static void main(String[] arguments) {
        new RotatableSprite();
    }

    /**
     * A rotatable sprite drawer.
     * Draws one rotating sprite for each existing sprite anchor position value.
     *
     * @author Manuel Juliachs
     */
    public static class RotatableSpriteDrawer implements Drawer {

        /** The sprites' text strings. */
        private static final String[] TEXT_STRINGS = {
            "Center anchor", "Left anchor", "Lower-left anchor",
            "Down anchor", "Lower-right anchor", "Right anchor",
            "Upper-right anchor", "Up anchor", "Upper-left anchor"
        };

        /** The anchor positions. */
        private static final AnchorPosition[] ANCHOR_POSITIONS = {
            AnchorPosition.CENTER, AnchorPosition.LEFT, AnchorPosition.LOWER_LEFT,
            AnchorPosition.DOWN, AnchorPosition.LOWER_RIGHT, AnchorPosition.RIGHT,
            AnchorPosition.UPPER_RIGHT, AnchorPosition.UP, AnchorPosition.UPPER_LEFT
        };

        /** The step added to the rotation angle at each draw call. */
        private static final float ROTATION_ANGLE_STEP = 2.5f;

        /** Arbitrary displacement used to position the sprites. */
        private static final float DELTA_POSITION = 0.8f;

        /** The number of sprites to draw, equal to the number of available sprite anchor position values. */
        private static final int NUM_SPRITES = 9;

        /** The margin used for the text sprites. */
        private static final int MARGIN = 4;

        /** The sprites' coordinates. */
        private final float[] positions;

        /** The buffer used to position anchor sprites. */
        private final ElementsBuffer position;

        /** The text sprites. */
        private final Texture[] textSprites;

        /** The sprite used to draw anchors. */
        private final Texture anchorSprite;

        /** The message sprite. */
        private final Texture messageSprite;

        /** The sprites' text entities. */
        private final TextEntity[] textEntities;

        /** The rotation angle applied to all the sprites. */
        private float rotationAngle;

        /** The line width of the sprites' box. */
        private final int lineWidth = 2;

        /** The line's half width. */
        private final int halfLineWidth = lineWidth / 2;

        /** Specifies whether anchor sprites are displayed or not. */
        private boolean anchorsDisplayed;

        /** Specifies the sprites' rotation direction. */
        private boolean clockwiseRotation;

        /** Specifies whether the message is displayed or not. */
        private boolean messageDisplayed;

        /**
         * Constructor.
         * @param canvas the canvas to use.
         */
        public RotatableSpriteDrawer(Canvas canvas) {
            /* The texture manager. */
            TextureManager textureManager = canvas.getTextureManager();

            anchorsDisplayed = true;
            clockwiseRotation = false;
            messageDisplayed = true;
            rotationAngle = 0;

            textEntities = new TextEntity[NUM_SPRITES];

            for (int i = 0; i < NUM_SPRITES; i++) {
                TextEntity textEntity = new TextEntity(TEXT_STRINGS[i]);
                textEntity.setTextColor(new Color(0.0f, 0.0f, 0.0f));
                textEntity.setTextAntiAliased(false);
                textEntities[i] = textEntity;
            }

            textSprites = new Texture[NUM_SPRITES];

            for (int i = 0; i < NUM_SPRITES; i++) {
                textSprites[i] = textureManager.createTexture();
                textSprites[i].setDrawer(getTextDrawer(i));
            }

            /* The sprite used to visualize anchor points */
            anchorSprite = textureManager.createTexture();
            anchorSprite.setDrawer(getSpriteDrawer());

            position = canvas.getBuffersManager().createElementsBuffer();

            positions = new float[] {
                0, 0, 0,
                -DELTA_POSITION, 0, 0,
                -DELTA_POSITION, -DELTA_POSITION, 0,
                0, -DELTA_POSITION, 0,
                DELTA_POSITION, -DELTA_POSITION, 0,
                DELTA_POSITION, 0, 0,
                DELTA_POSITION, DELTA_POSITION, 0,
                0, DELTA_POSITION, 0,
                -DELTA_POSITION, DELTA_POSITION, 0
            };

            position.setData(positions, 3);

            /* The message sprite */
            messageSprite = createMessage(canvas);
        }

        @Override
        public void draw(DrawingTools drawingTools) {
            try {
                drawingTools.clear(new Color(1.0f, 1.0f, 1.0f));

                Transformation projection = null;
                try {
                    projection = TransformationFactory.getPreferredAspectRatioTransformation(drawingTools.getCanvas().getDimension(), 1f);
                } catch (DegenerateMatrixException ignored) {
                    // Should not occur.
                }

                drawingTools.getTransformationManager().getProjectionStack().push(projection);

                for (int i = 0; i < NUM_SPRITES; i++) {
                    drawingTools.draw(
                        textSprites[i],
                        ANCHOR_POSITIONS[i],
                        new Vector3d(positions[3 * i], positions[3 * i + 1], positions[3 * i + 2]),
                        rotationAngle
                    );
                }

                if (anchorsDisplayed) {
                    drawingTools.draw(anchorSprite, AnchorPosition.CENTER, position);
                }

                if (messageDisplayed) {
                    drawingTools.draw(messageSprite, AnchorPosition.UPPER_LEFT, new Vector3d(-1.0, 1.0, 0.0));
                }


                if (clockwiseRotation) {
                    rotationAngle -= ROTATION_ANGLE_STEP;
                } else {
                    rotationAngle += ROTATION_ANGLE_STEP;
                }

                rotationAngle %= 360.0f;
            } catch (SciRendererException ignored) {
            }
        }

        @Override
        public boolean is2DView() {
            return false;
        }

        /**
         * Returns the anchor sprite drawer.
         * @return the anchor sprite drawer.
         */
        public TextureDrawer getSpriteDrawer() {

            return new TextureDrawer() {

                @Override
                public void draw(TextureDrawingTools drawingTools) {
                    drawingTools.fillDisc(0, 0, 10, new Color(1, 0, 0));


                    Appearance appearance = new Appearance();
                    appearance.setLineColor(new Color(0, 0, 0));
                    drawingTools.drawCircle(0, 0, 10, appearance);
                }

                @Override
                public Dimension getTextureSize() {
                    return new Dimension(10, 10);
                }

                @Override
                public OriginPosition getOriginPosition() {
                    return OriginPosition.CENTER;
                }
            };
        }

        /**
         * Returns the sprite drawer associated to a particular text sprite.
         * @param textSpriteIndex the sprite index.
         * @return the text sprite drawer.
         */
        public TextureDrawer getTextDrawer(final int textSpriteIndex) {

            if ((textSpriteIndex > (NUM_SPRITES - 1)) || (textSpriteIndex < 0)) {
                return null;
            }

            return new TextureDrawer() {

                @Override
                public void draw(TextureDrawingTools drawingTools) {
                    TextEntity textEntity = textEntities[textSpriteIndex];

                    drawingTools.draw(textEntity, 0 + MARGIN + halfLineWidth, 0 + MARGIN + halfLineWidth);

                    Dimension dimension = getTextureSize();
                    int boxWidth = dimension.width;
                    int boxHeight = dimension.height;

                    Appearance appearance = new Appearance();
                    appearance.setLineColor(new Color(0, 0, 0));
                    appearance.setLineWidth((float) lineWidth);

                    drawingTools.drawPolyline(new int[] {halfLineWidth, halfLineWidth, boxWidth - halfLineWidth, halfLineWidth,
                                                         boxWidth - halfLineWidth, boxHeight - halfLineWidth, halfLineWidth, boxHeight - halfLineWidth,
                                                         halfLineWidth, halfLineWidth
                                                        },
                                              appearance);
                }

                @Override
                public Dimension getTextureSize() {
                    Dimension dimension = textEntities[textSpriteIndex].getSize();

                    int boxWidth = (int) dimension.getWidth() + 2 * MARGIN + lineWidth;
                    int boxHeight = (int) dimension.getHeight() + 2 * MARGIN + lineWidth;

                    return new Dimension(boxWidth, boxHeight);
                }

                @Override
                public OriginPosition getOriginPosition() {
                    return OriginPosition.UPPER_LEFT;
                }

            };
        }

        /**
         * Toggles display of the anchors.
         */
        public void toggleAnchorDisplay() {
            anchorsDisplayed = !anchorsDisplayed;
        }

        /**
         * Switches the rotation direction.
         */
        public void switchRotationDirection() {
            clockwiseRotation = !clockwiseRotation;
        }

        /**
         * Toggles display of the help message.
         */
        public void toggleMessageDisplay() {
            messageDisplayed = !messageDisplayed;
        }

        /**
         * Creates a help message.
         * @param canvas the canvas where the message sprite is created.
         * @return a sprite that draws the message.
         */
        private Texture createMessage(Canvas canvas) {
            final TextEntity text = new TextEntity("Press 'T' to toggle anchor drawing, 'D' to switch the rotation direction, 'M' to toggle this message.");
            Texture message = canvas.getTextureManager().createTexture();
            message.setDrawer(new TextureDrawer() {

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

            return message;
        }

    }
}
