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

import java.util.HashMap;
import java.util.Map;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_ANGLE__;

/**
 *
 * This class manage scilab text entity drawing.
 *
 *
 * TODO, Manage: {text_box, text_box_mode, auto_dimensionning, clip_state, clip_box}
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

        /* The Text object's rotation direction convention is opposite to the standard one, its angle is expressed in radians. */
        drawingTools.draw(sprite, SpriteAnchorPosition.LOWER_LEFT, new Vector3d(text.getPosition()), -180.0*text.getFontAngle()/Math.PI);

        /* Compute the corners of the text's bounding box in window coordinates */
        Vector3d[] projCorners = computeProjCorners(drawingTools, new Vector3d(text.getPosition()), text.getFontAngle(), sprite.getWidth(), sprite.getHeight());

        Transformation projection = axesDrawer.getProjection();

        Vector3d[] corners = computeCorners(projection, projCorners);
        Double[] coordinates = cornersToCoordinateArray(corners);

        /* Set the computed coordinates */
        text.setCorners(coordinates);
    }

    /**
     * Computes and returns the corners of a {@see Text} object's bounding box, in window coordinates.
     * The returned corners are in the following order: lower-left, lower-right, upper-left and upper-right.
     * @param drawingTools the given {@see DrawingTools}.
     * @param position the text's position.
     * @param fontAngle the text's font angle (radians).
     * @param spriteWidth the text sprite's width (in pixels).
     * @param spriteHeight the text sprite's height (in pixels).
     * @return the corners' window coordinates (4-element array).
     * @throws DegenerateMatrixException 
     */
    private Vector3d[] computeProjCorners(DrawingTools drawingTools, Vector3d position, double fontAngle, int spriteWidth, int spriteHeight) throws DegenerateMatrixException {
        Transformation canvasProj = drawingTools.getTransformationManager().getCanvasProjection();

        Vector3d[] projCorners = new Vector3d[4];

        /*
         * Should be -fontAngle, as positive angle values are measured clockwise for texts.
         * Apparently uses the same convention as the texts (clockwise positive directions).
         * To be verified.
         */
        Transformation projRotation = TransformationFactory.getRotationTransformation(180.0*fontAngle/Math.PI, 0.0, 0.0, 1.0);

        projCorners[0] = canvasProj.project(position);

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
}
