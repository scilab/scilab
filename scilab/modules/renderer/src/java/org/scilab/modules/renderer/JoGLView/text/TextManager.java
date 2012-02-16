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
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.textObject.Text;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;

import java.util.HashMap;
import java.util.Map;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_ANGLE__;

/**
 *
 * This class manage scilab text entity drawing.
 *
 *
 * TODO, Manage: {text_box, text_box_mode (filled), auto_dimensionning, clip_state, clip_box}
 *
 *
 * @author Pierre Lando
 */
public class TextManager {

    /**
     * The {@see Map} off existing {@see TextEntity}.
     */
    private final Map<String, Sprite> spriteMap = new HashMap<String, Sprite>();

    /**
     * The used sprite manager.
     */
    private final SpriteManager spriteManager;


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

        Transformation projection = drawingTools.getTransformationManager().getCanvasProjection();

        /* Compute the text's position and draw in window coordinates */
        Vector3d textPosition = new Vector3d(text.getPosition());
        textPosition = computeTextPosition(projection, text, sprite.getWidth(), sprite.getHeight());

        drawingTools.getTransformationManager().useWindowCoordinate();

        /* The Text object's rotation direction convention is opposite to the standard one, its angle is expressed in radians. */
        drawingTools.draw(sprite, SpriteAnchorPosition.LOWER_LEFT, textPosition, -180.0*text.getFontAngle()/Math.PI);

        drawingTools.getTransformationManager().useSceneCoordinate();

        /* Compute the corners of the text's bounding box in window coordinates */
        Vector3d[] projCorners = computeProjCorners(projection, new Vector3d(text.getPosition()), text.getFontAngle(), sprite.getWidth(), sprite.getHeight());

        Vector3d[] corners = computeCorners(projection, projCorners);
        Double[] coordinates = cornersToCoordinateArray(corners);

        /* Set the computed coordinates */
        text.setCorners(coordinates);
    }

    /**
     * Computes and returns the position of a Scilab {@see Text} object in window coordinates as a function
     * of its text box mode and text box properties. It is the position of its lower-left corner,
     * which may differ from the text's unmodified lower-left corner position, depending on its text box properties.
     * If the text box mode is set to off, the returned position is the text's unmodified projected position.
     * @param projection the projection from object coordinates to window coordinates.
     * @param text the Scilab {@see Text}.
     * @param spriteWidth the text sprite's width (in pixels).
     * @param spriteHeight the text sprite's height (in pixels).
     * @return the position of the Scilab {@see Text}'s text box.
     * @throws DegenerateMatrixException.
     */
    private Vector3d computeTextPosition(Transformation projection, Text text, int spriteWidth, int spriteHeight) throws DegenerateMatrixException {
        Vector3d textPosition = new Vector3d(text.getPosition());

        textPosition = projection.project(textPosition);

        /* Only manages the centered mode at the moment */
        if (text.getTextBoxMode() == 1) {
            Double[] textBox = text.getTextBox();

            Vector3d textBoxWidth = new Vector3d(textBox[0], 0.0, 0.0);
            Vector3d textBoxHeight = new Vector3d(0.0, textBox[1], 0.0);

            textBoxWidth = projection.projectDirection(textBoxWidth);
            textBoxHeight = projection.projectDirection(textBoxHeight);

            Vector3d[] projCorners = computeProjCorners(textPosition, text.getFontAngle(), spriteWidth, spriteHeight);

            Vector3d textWidth = projCorners[1].minus(projCorners[0]);
            Vector3d textHeight = projCorners[2].minus(projCorners[0]);

            /* Compute the text box's and text's half-length vectors */
            textBoxWidth = textWidth.getNormalized().times(0.5*textBoxWidth.getX());
            textBoxHeight = textHeight.getNormalized().times(0.5*textBoxHeight.getY());

            textWidth = textWidth.times(0.5);
            textHeight = textHeight.times(0.5);

            /* Compute the actual position from the initial projected position and the half-length vectors */
            textPosition = textPosition.plus(textBoxWidth);
            textPosition = textPosition.plus(textBoxHeight);

            textPosition = textPosition.minus(textWidth);
            textPosition = textPosition.minus(textHeight);
        }

        return textPosition;
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
            sprite = createSprite(colorMap, text);
            spriteMap.put(text.getIdentifier(), sprite);
        }
        return sprite;
    }

    /**
     * Returns the dimensions of the SciRenderer {@see Sprite} corresponding to the given Scilab {@see Text}.
     * The dimensions are in pixels (width, height).
     * @param colorMap the current color map.
     * @param text the given Scilab {@see Text}.
     * @return the sprite's dimensions (2-element array)
     */
    private int[] getSpriteDims(final ColorMap colorMap, final Text text) {
        int[] spriteDims = new int[2];
        Sprite sprite = spriteMap.get(text.getIdentifier());
        if (sprite == null) {
            TextSpriteDrawer spriteDrawer = new TextSpriteDrawer(spriteManager, colorMap, text);

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
        Vector3d[] projCorners = null;

        DrawerVisitor currentVisitor = DrawerVisitor.getVisitor(text.getParentFigure());
        Transformation currentProj = currentVisitor.getAxesDrawer().getProjection(text.getParentAxes());

        int[] spriteDims = currentVisitor.getTextManager().getSpriteDims(currentVisitor.getColorMap(), text);
        Vector3d textPosition = new Vector3d(text.getPosition());

        /* Compute the corners */
        try {
            textPosition = currentVisitor.getTextManager().computeTextPosition(currentProj, text, spriteDims[0], spriteDims[1]);
            projCorners = currentVisitor.getTextManager().computeProjCorners(textPosition, text.getFontAngle(), spriteDims[0], spriteDims[1]);
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
