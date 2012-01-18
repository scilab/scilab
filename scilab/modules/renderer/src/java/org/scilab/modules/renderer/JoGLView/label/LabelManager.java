/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 * Copyright (C) 2011-2012 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.label;

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.sprite.Sprite;
import org.scilab.forge.scirenderer.sprite.SpriteAnchorPosition;
import org.scilab.forge.scirenderer.sprite.SpriteManager;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.label.Label;
import org.scilab.modules.graphic_objects.utils.Utils;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;
import org.scilab.modules.renderer.JoGLView.label.LabelPositioner;
import org.scilab.modules.renderer.JoGLView.label.TitlePositioner;

import java.util.HashMap;
import java.util.Map;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AUTO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_ANGLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AUTO_ROTATION__;

/**
 *
 * This class performs the drawing of scilab's Label entity.
 *
 * TODO, manage: {font_angle, auto_rotation, font_fractional}
 *
 * @author Manuel Juliachs
 */
public class LabelManager {

    /**
     * The {@see Map} of existing {@see Sprite}.
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
    public LabelManager(SpriteManager spriteManager) {
        this.spriteManager = spriteManager;
    }

    /**
     * Positions and draws the given Scilab {@see Label} with the given {@see DrawingTools}.
     * First, it initializes the label positioner's remaining parameters which have not been previously
     * obtained from the axis ruler drawer. It then asks the positioner the relevant values and
     * finally draws the label's sprite using these resulting values.
     * @param drawingTools the given {@see DrawingTools}.
     * @param colorMap the current {@see ColorMap}.
     * @param label the given Scilab {@see Label}.
     * @param labelPositioner the positioner used to compute the label's position.
     * @param axesDrawer the Axes drawer used to draw the label's parent Axes.
     * @param drawnFlag a flag indicating whether the label must be drawn or not.
     */
    public final void positionAndDraw(final DrawingTools drawingTools, final ColorMap colorMap, final Label label, LabelPositioner labelPositioner, AxesDrawer axesDrawer, boolean drawnFlag) {
        String parentAxesID = label.getParentAxes();
        Axes parentAxes = (Axes) GraphicController.getController().getObjectFromId(parentAxesID);

        Sprite labelSprite = getSprite(colorMap, label);

        labelPositioner.setLabelSprite(labelSprite);
        labelPositioner.setDrawingTools(drawingTools);
        labelPositioner.setParentAxes(parentAxes);
        labelPositioner.setAutoPosition(label.getAutoPosition());
        labelPositioner.setAutoRotation(label.getAutoRotation());
        labelPositioner.setUserRotationAngle(180.0*label.getFontAngle()/Math.PI);

        Vector3d labelUserPosition = new Vector3d(label.getPosition());
        labelUserPosition = axesDrawer.getBoxCoordinates(labelUserPosition);
        labelPositioner.setLabelUserPosition(labelUserPosition);

        /* Computes the label's position values */
        labelPositioner.positionLabel();

        Vector3d labelPos = labelPositioner.getAnchorPoint();
        SpriteAnchorPosition labelAnchor = labelPositioner.getAnchorPosition();

        /* Set the lower-left corner position if auto-positioned */
        if (label.getAutoPosition()) {
            Vector3d cornerPos = labelPositioner.getLowerLeftCornerPosition();
            Vector3d objectCornerPos = axesDrawer.getObjectCoordinates(cornerPos);
            label.setPosition(new Double[]{objectCornerPos.getX(), objectCornerPos.getY(), objectCornerPos.getZ()});

            /*
             * Must be reset to true as setPosition modifies the label's auto position field.
             * To be modified.
             */
            label.setAutoPosition(true);
        }

        /* Compute and set the label's corners */
        Transformation projection = axesDrawer.getProjection();

        Vector3d[] projCorners = labelPositioner.getProjCorners();

        Vector3d[] corners = computeCorners(projection, projCorners);
        Double[] coordinates = cornersToCoordinateArray(corners);

        /* Set the computed coordinates */
        label.setCorners(coordinates);


        double rotationAngle = 0.0;

        if (label.getAutoRotation()) {
            rotationAngle = labelPositioner.getRotationAngle();

            label.setFontAngle(Math.PI*rotationAngle/180.0);

            /*
             * Must be reset to true as setFontAngle modifies the label's auto rotation field.
             * To be modified.
             */
            label.setAutoRotation(true);
        } else {
            rotationAngle = labelPositioner.getUserRotationAngle();
        }

        /* The label's rotation direction convention is opposite to the standard one. */
        rotationAngle = -rotationAngle;

        if (label.getVisible() && drawnFlag) {
            if (Utils.isValid(labelPos.getX(), labelPos.getY(), labelPos.getZ()) && Utils.isValid(rotationAngle)) {
                if (labelPositioner.getUseWindowCoordinates()) {
                    /* Draw in window coordinates */
                    Transformation canvasProjection = drawingTools.getTransformationManager().getCanvasProjection();
                    Vector3d projLabelPos = canvasProjection.project(labelPos);

                    drawingTools.getTransformationManager().useWindowCoordinate();
                    drawingTools.draw(labelSprite, labelAnchor, projLabelPos, rotationAngle);
                    drawingTools.getTransformationManager().useSceneCoordinate();
                } else {
                    /* Draw using box coordinates */
                    drawingTools.draw(labelSprite, labelAnchor, labelPos, rotationAngle);
                }
            }
        }

    }

    /**
     * Computes and returns the corners (in object coordinates) of a label's bounding box.
     * @param projection the projection from object coordinates to window coordinates.
     * @param projCorners the corners of the label's bounding box in window coordinates (4-element array).
     * @return the corners of the label's bounding box in object coordinates (4-element array).
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
     * The output corners are reordered to match their order in the {@see Label} object's
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
     * Updates the sprite data if needed.
     * @param id the modified object.
     * @param property the changed property.
     */
    public void update(String id, String property) {
        if (!__GO_POSITION__.equals(property) && !__GO_AUTO_POSITION__.equals(property)
         && !__GO_FONT_ANGLE__.equals(property) && !__GO_AUTO_ROTATION__.equals(property)) {
            dispose(id);
        }
    }

    /**
     * Returns the SciRenderer {@see Sprite} corresponding to the given Scilab {@see Label}.
     * @param colorMap the current color map.
     * @param label the given Scilab {@see Label}.
     * @return the SciRenderer {@see Sprite} corresponding to the given Scilab {@see Label}.
     */
    public Sprite getSprite(final ColorMap colorMap, final Label label) {
        Sprite sprite = spriteMap.get(label.getIdentifier());
        if (sprite == null) {
            sprite = createSprite(colorMap, label);
            spriteMap.put(label.getIdentifier(), sprite);
        }
        return sprite;
    }

    /**
     * Creates a sprite for the given label.
     * @param colorMap the current colormap.
     * @param label the given label.
     * @return a new sprite for the given label.
     */
    private Sprite createSprite(final ColorMap colorMap, final Label label) {
        LabelSpriteDrawer spriteDrawer = new LabelSpriteDrawer(spriteManager, colorMap, label);
        Sprite sprite = spriteManager.createRotatableSprite(spriteDrawer.getWidth(), spriteDrawer.getHeight());
        sprite.setDrawer(spriteDrawer);
        return sprite;
    }

    /**
     * Disposes the sprite corresponding to the given id.
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
     * Disposes all the label text sprites.
     */
    public void disposeAll() {
        spriteManager.dispose(spriteMap.values());
        spriteMap.clear();
    }
}
