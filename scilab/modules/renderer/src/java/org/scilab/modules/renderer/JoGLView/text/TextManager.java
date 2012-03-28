/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 * Copyright (C) 2012 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.text;

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.sprite.Sprite;
import org.scilab.forge.scirenderer.sprite.SpriteAnchorPosition;
import org.scilab.forge.scirenderer.sprite.SpriteManager;
import org.scilab.forge.scirenderer.tranformations.DegenerateMatrixException;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.textObject.Text;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_ANGLE__;

/**
 *
 * This class manage scilab text entity drawing.
 *
 *
 * TODO, Manage: {auto_dimensionning}
 *
 *
 * @author Pierre Lando
 */
public class TextManager {

    /**
     * The {@see Map} off existing {@see TextEntity}.
     */
    private final Map<String, Sprite> spriteMap = new ConcurrentHashMap<String, Sprite>();

    /**
     * The used sprite manager.
     */
    private final SpriteManager spriteManager;

    /**
     * The bounds of the scale factor range for which the sprite does not
     * need to be updated.
     */
    private double[] FACTOR_UPDATE_INTERVAL = {0.99, 1.01};

    /**
     * Default constructor.
     * @param spriteManager the sprite manager.
     */
    public TextManager(SpriteManager spriteManager) {
        this.spriteManager = spriteManager;
    }

    /**
     * Draw the given Scilab {@see Text} with the given {@see DrawingTools}.
     * @param drawingTools the given {@see DrawingTools}.
     * @param colorMap the current {@see ColorMap}
     * @param text the given Scilab {@see Text}
     * @throws DegenerateMatrixException 
     */
    public final void draw(final DrawingTools drawingTools, final ColorMap colorMap, final Text text, final AxesDrawer axesDrawer) throws DegenerateMatrixException {
        Sprite sprite = getSprite(colorMap, text);

        /* The unscaled sprite's dimensions */
        int[] spriteDims = getSpriteDims(colorMap, text);

        Transformation projection = drawingTools.getTransformationManager().getCanvasProjection();

        Vector3d textPosition = new Vector3d(text.getPosition());

        /* Compute the text box vectors and the text box to sprite dimension ratios */
        Vector3d[] textBoxVectors =  computeTextBoxVectors(projection, text, sprite.getWidth(), sprite.getHeight());
        double[] ratios = computeRatios(projection, text, textBoxVectors, sprite.getWidth(), sprite.getHeight(), spriteDims[0], spriteDims[1]);

        /* If text box mode is equal to filled, the sprite must be updated */
        if (text.getTextBoxMode() == 2 && ratios[0] != 1.0) {
            sprite = updateSprite(colorMap, text, ratios[0], ratios[1]);
        }

        /* Compute the text sprite's actual position, which depends on the object's text box mode property */
        Vector3d[] cornerPositions = computeTextPosition(projection, text, textBoxVectors, sprite.getWidth(), sprite.getHeight());

        /* Draw in window coordinates */
        drawingTools.getTransformationManager().useWindowCoordinate();

        /* The Text object's rotation direction convention is opposite to the standard one, its angle is expressed in radians. */
        drawingTools.draw(sprite, SpriteAnchorPosition.LOWER_LEFT, cornerPositions[0], -180.0*text.getFontAngle()/Math.PI);

        drawingTools.getTransformationManager().useSceneCoordinate();

        /* Compute the corners of the text's bounding box in window coordinates */
        Vector3d[] projCorners = null;
        if (text.getTextBoxMode() == 2) {
            projCorners = computeProjTextBoxCorners(projection, cornerPositions[1], text.getFontAngle(), textBoxVectors);
        } else {
            projCorners = computeProjCorners(projection, new Vector3d(text.getPosition()), text.getFontAngle(), sprite.getWidth(), sprite.getHeight());
        }

        Vector3d[] corners = computeCorners(projection, projCorners);
        Double[] coordinates = cornersToCoordinateArray(corners);

        /* Set the computed coordinates */
        text.setCorners(coordinates);
    }

    /**
     * Computes and returns the two vectors (in object coordinates) respectively corresponding
     * to the text box's base and side (also named the text box width and height vectors). If the
     * Text's text box mode is equal to off, the vectors are then equal to the Text label's base
     * and side vectors.  The Text's rotation is ignored, as it is not required for now.
     * @param projection the projection from object coordinates to window coordinates.
     * @param text the Scilab {@see Text}.
     * @param spriteWidth the current text sprite's width (in pixels).
     * @param spriteHeight the current text sprite's height (in pixels).
     * @return the text box width and height vectors (in object coordinates).
     */
    private Vector3d[] computeTextBoxVectors(Transformation projection, Text text, int spriteWidth, int spriteHeight) {
        Double[] textBox = text.getTextBox();

        Vector3d[] textBoxVectors = new Vector3d[2];

        Vector3d textPosition = new Vector3d(text.getPosition());
        Vector3d projTextPosition = projection.project(textPosition);

        /* Compute the text label vectors in window coordinates */
        Vector3d width = new Vector3d((double) spriteWidth, 0.0, 0.0);
        Vector3d height = new Vector3d(0.0, (double) spriteHeight, 0.0);

        Vector3d projTextWidth = projTextPosition.plus(width);
        Vector3d projTextHeight = projTextPosition.plus(height);

        /*
         * Compute the text box's vectors in object coordinates, from the object coordinate text label vectors.
         * Their norms are unaffected by the text's rotation, which is thus ignored.
         */
        Vector3d textWidth = projection.unproject(projTextWidth).minus(textPosition);
        Vector3d textHeight = projection.unproject(projTextHeight).minus(textPosition);

        if (text.getTextBoxMode() >= 1) {
            textWidth = textWidth.getNormalized().times(textBox[0]);
            textHeight = textHeight.getNormalized().times(textBox[1]);
        }

        textBoxVectors[0] = textWidth;
        textBoxVectors[1] = textHeight;

        return textBoxVectors;
    }

    /**
     * Computes and returns the minimum of the ratios between the text box and the text sprite's dimensions.
     * This minimum ratio is determined for both the current text sprite and the unscaled text sprite.
     * @param projection the projection from object coordinates to window coordinates.
     * @param text the Scilab {@see Text}.
     * @param textBoxVectors the text box width and height vectors (in object coordinates).
     * @param spriteWidth the current text sprite's width (in pixels).
     * @param spriteHeight the current text sprite's height (in pixels).
     * @param baseSpriteWidth the unscaled text sprite's width (in pixels).
     * @param baseSpriteHeight the unscaled text sprite's height (in pixels).
     * @return the minimum ratios (2 elements: text box to current sprite and text box to unscaled sprite ratios).
     */
    private double[] computeRatios(Transformation projection, Text text, Vector3d[] textBoxVectors, int spriteWidth, int spriteHeight,
        int baseSpriteWidth, int baseSpriteHeight) {
        /* 1st element: ratio for the current sprite, 2nd element: ratio for the unscaled sprite */
        double[] ratios = new double[]{1.0, 1.0};

        /* Ratios are relevant only to the filled text box mode */
        if (text.getTextBoxMode() == 2) {
            Vector3d textBoxWidth = projection.projectDirection(textBoxVectors[0]);
            Vector3d textBoxHeight = projection.projectDirection(textBoxVectors[1]);

            /* Compute the ratios. */
            double minRatio = Math.min(Math.abs(textBoxWidth.getX() / spriteWidth), Math.abs(textBoxHeight.getY() / spriteHeight));
            double baseMinRatio = Math.min(Math.abs(textBoxWidth.getX() / baseSpriteWidth), Math.abs(textBoxHeight.getY() / baseSpriteHeight));

            ratios[0] = minRatio;
            ratios[1] = baseMinRatio;
        }

        return ratios;
    }

    /**
     * Computes and returns the positions of a Scilab {@see Text} object and its text box in window coordinates, as a function
     * of its text box mode and text box properties. They are the position of the Text's lower-left corner,
     * which may differ from the text's unmodified position, depending on its text box properties, and
     * the position of the text box's lower-left corner. If the text box mode is set to off, the text lower-left corner's position
     * is simply the text's projected position, and its text box's position is equal to the former.
     * @param projection the projection from object coordinates to window coordinates.
     * @param text the Scilab {@see Text}.
     * @param textBoxVectors the text box width and height vectors (in object coordinates).
     * @param spriteWidth the text sprite's width (in pixels).
     * @param spriteHeight the text sprite's height (in pixels).
     * @return the lower-left corners of the Scilab {@see Text}'s text and of its text box in window coordinates (2 elements).
     * @throws DegenerateMatrixException.
     */
    private Vector3d[] computeTextPosition(Transformation projection, Text text, Vector3d[] textBoxVectors, int spriteWidth, int spriteHeight) throws DegenerateMatrixException {
        Vector3d[] cornerPositions = new Vector3d[2];

        Vector3d textPosition = new Vector3d(text.getPosition());

        String parentAxesId = text.getParentAxes();
        Axes parentAxes = (Axes) GraphicController.getController().getObjectFromId(parentAxesId);

        textPosition = projection.project(textPosition);

        cornerPositions[0] = new Vector3d(textPosition);
        cornerPositions[1] = new Vector3d(textPosition);

        if (text.getTextBoxMode() >= 1) {
            Double[] textBox = text.getTextBox();

            Vector3d textBoxWidth = new Vector3d(textBoxVectors[0]);
            Vector3d textBoxHeight = new Vector3d(textBoxVectors[1]);

            double[] textBoxWidthData = textBoxWidth.getData();
            double[] textBoxHeightData = textBoxHeight.getData();

            /* Reversed axes must be taken into account to correctly compute the text sprite's lower-left corner. */
            if (parentAxes.getXAxisReverse()) {
                textBoxWidthData[0] = Math.abs(textBoxWidthData[0]);
                textBoxHeightData[0] = Math.abs(textBoxHeightData[0]);
            }

            if (parentAxes.getYAxisReverse()) {
                textBoxWidthData[1] = Math.abs(textBoxWidthData[1]);
                textBoxHeightData[1] = Math.abs(textBoxHeightData[1]);
            }

            if (parentAxes.getZAxisReverse()) {
                textBoxWidthData[2] = Math.abs(textBoxWidthData[2]);
                textBoxHeightData[2] = Math.abs(textBoxHeightData[2]);
            }

            Vector3d revTextBoxWidth = new Vector3d(textBoxWidthData);
            Vector3d revTextBoxHeight = new Vector3d(textBoxHeightData);

            /* Compute the window coordinate text box vectors */
            revTextBoxWidth = projection.projectDirection(revTextBoxWidth);
            revTextBoxHeight = projection.projectDirection(revTextBoxHeight);

            textBoxWidth = projection.projectDirection(textBoxWidth);
            textBoxHeight = projection.projectDirection(textBoxHeight);

            Vector3d[] projCorners = computeProjCorners(textPosition, text.getFontAngle(), spriteWidth, spriteHeight);

            Vector3d textWidth = projCorners[1].minus(projCorners[0]);
            Vector3d textHeight = projCorners[2].minus(projCorners[0]);

            /*
             * Compute the final text box's and text's half-length vectors,
             * using the rotated text label vectors.
             */
            revTextBoxWidth = textWidth.getNormalized().times(0.5*revTextBoxWidth.getX());
            revTextBoxHeight = textHeight.getNormalized().times(0.5*revTextBoxHeight.getY());

            textBoxWidth = textWidth.getNormalized().times(0.5*textBoxWidth.getX());
            textBoxHeight = textHeight.getNormalized().times(0.5*textBoxHeight.getY());

            textWidth = textWidth.times(0.5);
            textHeight = textHeight.times(0.5);

            /* Compute the actual corner positions from the initial projected position and the half-length vectors */
            textPosition = textPosition.plus(revTextBoxWidth);
            textPosition = textPosition.plus(revTextBoxHeight);

            Vector3d textBoxCorner = textPosition.minus(textBoxWidth);
            textBoxCorner = textBoxCorner.minus(textBoxHeight);

            textPosition = textPosition.minus(textWidth);
            textPosition = textPosition.minus(textHeight);

            cornerPositions[0] = textPosition;
            cornerPositions[1] = textBoxCorner;
        }

        return cornerPositions;
    }

    /**
     * Computes and returns the corners of a {@see Text} object's bounding box, in window coordinates.
     * The returned corners are in the following order: lower-left, lower-right, upper-left and upper-right.
     * @param canvasProj the projection from object coordinates to window coordinates.
     * @param position the text's position in object coordinates.
     * @param fontAngle the text's font angle (radians).
     * @param spriteWidth the text sprite's width (in pixels).
     * @param spriteHeight the text sprite's height (in pixels).
     * @return the corners' window coordinates (4-element array).
     * @throws DegenerateMatrixException.
     */
    private Vector3d[] computeProjCorners(Transformation canvasProj, Vector3d position, double fontAngle, int spriteWidth, int spriteHeight) throws DegenerateMatrixException {
        Vector3d[] projCorners = new Vector3d[4];

        position = canvasProj.project(position);

        projCorners = computeProjCorners(position, fontAngle, spriteWidth, spriteHeight);

        return projCorners;
    }

    /**
     * Computes the corners of a {@see Text} object's text box, in window coordinates.
     * @param canvasProj the projection from object coordinates to window coordinates.
     * @param position the position of the text box's lower-left corner in window cordinates.
     * @param fontAngle the text's font angle (radians).
     * @return the corners' window coordinates (4-element array).
     * @throws DegenerateMatrixException.
     */
    private Vector3d[] computeProjTextBoxCorners(Transformation canvasProj, Vector3d position, double fontAngle, Vector3d[] textBoxVectors) throws DegenerateMatrixException {
        Vector3d[] projCorners = new Vector3d[4];

        double projTextBoxWidth = canvasProj.projectDirection(textBoxVectors[0]).getNorm();
        double projTextBoxHeight = canvasProj.projectDirection(textBoxVectors[1]).getNorm();

        projCorners = computeProjCorners(position, fontAngle, (int) projTextBoxWidth, (int) projTextBoxHeight);

        return projCorners;
    }

    /**
     * Computes and returns the corners of a {@see Text} object's bounding box, in window coordinates.
     * The returned corners are in the following order: lower-left, lower-right, upper-left and upper-right.
     * @param projPosition the text's position in window coordinates.
     * @param fontAngle the text's font angle (radians).
     * @param spriteWidth the text sprite's width (in pixels).
     * @param spriteHeight the text sprite's height (in pixels).
     * @return the corners' window coordinates (4-element array).
     * @throws DegenerateMatrixException.
     */
    private Vector3d[] computeProjCorners(Vector3d projPosition, double fontAngle, int spriteWidth, int spriteHeight) throws DegenerateMatrixException {
        Vector3d[] projCorners = new Vector3d[4];

        /*
         * Should be -fontAngle, as positive angle values are measured clockwise for texts.
         * Apparently uses the same convention as the texts (clockwise positive directions).
         * To be verified.
         */
        Transformation projRotation = TransformationFactory.getRotationTransformation(180.0*fontAngle/Math.PI, 0.0, 0.0, 1.0);

        projCorners[0] = projPosition;

        Vector3d width = new Vector3d((double) spriteWidth, 0.0, 0.0);
        Vector3d height = new Vector3d(0.0, (double) spriteHeight, 0.0);

        width = projRotation.projectDirection(width);
        height = projRotation.projectDirection(height);

        projCorners[1] = projCorners[0].plus(width);
        projCorners[2] = projCorners[0].plus(height);
        projCorners[3] = projCorners[2].plus(width);

        return projCorners;
    }

    /**
     * Computes and returns the corners (in object coordinates) of a text's bounding box.
     * @param projection the projection from object coordinates to window coordinates.
     * @param projCorners the corners of the text's bounding box in window coordinates (4-element array).
     * @return the corners of the text's bounding box in object coordinates (4-element array).
     */
    private Vector3d[] computeCorners(Transformation projection, Vector3d[] projCorners) {
        Vector3d[] corners = new Vector3d[4];

        corners[0] = projection.unproject(projCorners[0]);
        corners[1] = projection.unproject(projCorners[1]);
        corners[2] = projection.unproject(projCorners[2]);
        corners[3] = projection.unproject(projCorners[3]);

        return corners;
    }

    /**
     * Returns the positions of a bounding box's corners as an array of (x,y,z) coordinate triplets.
     * The output corners are reordered to match their order in the {@see Text} object's
     * equivalent array, respectively: lower-left, lower-right, upper-left, upper-right in the input array,
     * starting from the lower-left and going in clockwise order in the returned array.
     * @param corners of the bounding box (4-element array).
     * @return the corners' coordinates (12-element array).
     */
    private Double[] cornersToCoordinateArray(Vector3d[] corners) {
        Double[] coordinates = new Double[12];
        coordinates[0] = corners[0].getX();
        coordinates[1] = corners[0].getY();
        coordinates[2] = corners[0].getZ();

        coordinates[3] = corners[2].getX();
        coordinates[4] = corners[2].getY();
        coordinates[5] = corners[2].getZ();

        coordinates[6] = corners[3].getX();
        coordinates[7] = corners[3].getY();
        coordinates[8] = corners[3].getZ();

        coordinates[9] = corners[1].getX();
        coordinates[10] = corners[1].getY();
        coordinates[11] = corners[1].getZ();

        return coordinates;
    }


    /**
     * Update the data if needed.
     * @param id the modified object.
     * @param property the changed property.
     */
    public void update(String id, String property) {
        if (!__GO_POSITION__.equals(property) && !__GO_FONT_ANGLE__.equals(property)) {
            dispose(id);
        }
    }

    /**
     * Return the SciRenderer {@see Sprite} corresponding to the given Scilab {@see Text}.
     * @param colorMap the current color map.
     * @param text the given Scilab {@see Text}.
     * @return the SciRenderer {@see Sprite} corresponding to the given Scilab {@see Text}.
     */
    private Sprite getSprite(final ColorMap colorMap, final Text text) {
        Sprite sprite = spriteMap.get(text.getIdentifier());
        if (sprite == null) {
            if (text.getTextBoxMode() == 2) {
                /* Create an unscaled sprite (scale factor equal to 1) */
                sprite = createSprite(colorMap, text, 1.0);
            } else {
                sprite = createSprite(colorMap, text);
            }
            spriteMap.put(text.getIdentifier(), sprite);
        }
        return sprite;
    }

    /**
     * Updates a sprite according to a scale factor, which is the ratio by which the sprite is enlarged.
     * @param colorMap the current color map.
     * @param text the Scilab Text.
     * @param scaleFactor the scale factor relative to the current sprite's dimensions.
     * @param baseScaleFactor the scale factor relative to the unscaled sprite's dimensions.
     */
    private Sprite updateSprite(final ColorMap colorMap, final Text text, double scaleFactor, double baseScaleFactor) {
        Sprite sprite = spriteMap.get(text.getIdentifier());

        /* Create a new sprite if the scale factor falls outside of the range */
        if (scaleFactor < FACTOR_UPDATE_INTERVAL[0] || scaleFactor > FACTOR_UPDATE_INTERVAL[1]) {
            dispose(text.getIdentifier());
            sprite = createSprite(colorMap, text, baseScaleFactor);
            spriteMap.put(text.getIdentifier(), sprite);
        }

        return sprite;
    }

    /**
     * Returns the dimensions of the SciRenderer {@see Sprite} corresponding to the given Scilab {@see Text}.
     * The dimensions are in pixels (width, height).
     * @param colorMap the current color map.
     * @param text the given Scilab {@see Text}.
     * @return the sprite's dimensions (2-element array).
     */
    private int[] getSpriteDims(final ColorMap colorMap, final Text text) {
        int[] spriteDims = new int[2];
        Sprite sprite = spriteMap.get(text.getIdentifier());
        if (sprite == null || true) {
            TextSpriteDrawer spriteDrawer = null;

            if (text.getTextBoxMode() == 2) {
                /* Set the scale factor to 1 in order to return the dimensions of an unscaled sprite. */
                spriteDrawer = new TextSpriteDrawer(spriteManager, colorMap, text, 1.0);
            } else {
                spriteDrawer = new TextSpriteDrawer(spriteManager, colorMap, text);
            }

            spriteDims[0] = spriteDrawer.getWidth();
            spriteDims[1] = spriteDrawer.getHeight();
        } else {
            spriteDims[0] = sprite.getWidth();
            spriteDims[1] = sprite.getHeight();
        }

        return spriteDims;
    }

    /**
     * Create a sprite for the given text object.
     * @param colorMap the current colormap.
     * @param textObject the given text object.
     * @return a new sprite for the given text object.
     */
    private Sprite createSprite(final ColorMap colorMap, final Text textObject) {
        TextSpriteDrawer spriteDrawer = new TextSpriteDrawer(spriteManager, colorMap, textObject);
        Sprite sprite = spriteManager.createRotatableSprite(spriteDrawer.getWidth(), spriteDrawer.getHeight());
        sprite.setDrawer(spriteDrawer);
        return sprite;
    }

    /**
     * Creates a sprite for the given text object, scaled by the given scale factor.
     * @param colorMap the current colormap.
     * @param textObject the given text object.
     * @param scaleFactor the scale factor to apply.
     * @return a new sprite for the given text object.
     */
    private Sprite createSprite(final ColorMap colorMap, final Text textObject, double scaleFactor) {
        TextSpriteDrawer spriteDrawer = new TextSpriteDrawer(spriteManager, colorMap, textObject, scaleFactor);
        Sprite sprite = spriteManager.createRotatableSprite(spriteDrawer.getWidth(), spriteDrawer.getHeight());
        sprite.setDrawer(spriteDrawer);
        return sprite;
    }

    /**
     * Dispose the sprite corresponding to the given id.
     * @param id the given id.
     */
    public void dispose(String id) {
        Sprite sprite = spriteMap.get(id);
        if (sprite != null) {
            spriteManager.dispose(sprite);
            spriteMap.remove(id);
        }
    }

    /**
     * Dispose all the text sprites.
     */
    public void disposeAll() {
        spriteManager.dispose(spriteMap.values());
        spriteMap.clear();
    }

    /**
     * Computes and updates the corners of the given Scilab {@see Text}.
     * @param text the given Scilab {@see Text}.
     */
    public static void updateTextCorners(Text text) {
        double[] retRatio = new double[1];
        Vector3d[] projCorners = null;

        DrawerVisitor currentVisitor = DrawerVisitor.getVisitor(text.getParentFigure());
        Transformation currentProj = currentVisitor.getAxesDrawer().getProjection(text.getParentAxes());

        int[] spriteDims = currentVisitor.getTextManager().getSpriteDims(currentVisitor.getColorMap(), text);
        Vector3d textPosition = new Vector3d(text.getPosition());

        /* Compute the corners */
        try {
            Vector3d[] textBoxVectors = currentVisitor.getTextManager().computeTextBoxVectors(currentProj, text, spriteDims[0], spriteDims[1]);
            Vector3d[] cornerPositions = currentVisitor.getTextManager().computeTextPosition(currentProj, text, textBoxVectors, spriteDims[0], spriteDims[1]);

            if (text.getTextBoxMode() == 2) {
                projCorners = currentVisitor.getTextManager().computeProjTextBoxCorners(currentProj, cornerPositions[1], text.getFontAngle(), textBoxVectors);
            } else {
                projCorners = currentVisitor.getTextManager().computeProjCorners(cornerPositions[0], text.getFontAngle(), spriteDims[0], spriteDims[1]);
            }
        } catch (DegenerateMatrixException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        Vector3d[] corners = currentVisitor.getTextManager().computeCorners(currentProj, projCorners);
        Double[] coordinates = currentVisitor.getTextManager().cornersToCoordinateArray(corners);

        /* Set the computed coordinates */
        text.setCorners(coordinates);
    }
}
