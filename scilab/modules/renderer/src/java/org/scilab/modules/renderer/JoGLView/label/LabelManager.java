/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 * Copyright (C) 2011-2012 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.modules.renderer.JoGLView.label;

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureManager;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axes.Camera;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.label.Label;
import org.scilab.modules.graphic_objects.utils.Utils;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;
import org.scilab.modules.renderer.JoGLView.util.ScaleUtils;

import java.awt.Dimension;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AUTO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AUTO_ROTATION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_ANGLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;

/**
 *
 * This class performs the drawing of scilab's Label entity.
 *
 * TODO, manage: {font_fractional}
 *
 * @author Manuel Juliachs
 */
public class LabelManager {

    /**
     * The {@see Map} of existing {@see Sprite}.
     */
    private final Map<Integer, Texture> textureMap = new ConcurrentHashMap<Integer, Texture>();

    /**
     * The used sprite manager.
     */
    private final TextureManager textureManager;

    /**
     * Default constructor.
     * @param textureManager the texture manager.
     */
    public LabelManager(TextureManager textureManager) {
        this.textureManager = textureManager;
    }

    public Dimension[] getLabelsSize(ColorMap colorMap, Axes axes, AxesDrawer drawer) {
        final Label xl = (Label) GraphicController.getController().getObjectFromId(axes.getXAxisLabel());
        final Label yl = (Label) GraphicController.getController().getObjectFromId(axes.getYAxisLabel());
        final Label tl = (Label) GraphicController.getController().getObjectFromId(axes.getTitle());
        final Dimension[] dims = new Dimension[3];
        final Label[] labels = new Label[] {xl, yl, tl};
        final LabelPositioner[] positioners = new LabelPositioner[] {drawer.getXAxisLabelPositioner(axes), drawer.getYAxisLabelPositioner(axes), drawer.getTitlePositioner(axes) };
        for (int i = 0; i < 3; i++) {
            if (labels[i] != null && !labels[i].isEmpty() && labels[i].getAutoPosition() && labels[i].getAutoRotation()) {
                final Texture texture = getTexture(colorMap, labels[i]);
                dims[i] = texture.getDataProvider().getTextureSize();
                final double a = positioners[i].getAutoRotationAngle();
                if (a == 90 || a == 270) {
                    final int t = dims[i].width;
                    dims[i].width = dims[i].height;
                    dims[i].height = t;
                }
            } else {
                dims[i] = new Dimension();
            }
        }

        return dims;
    }

    public Dimension getXLabelSize(ColorMap colorMap, Axes axes, AxesDrawer drawer) {
        final Label xl = (Label) GraphicController.getController().getObjectFromId(axes.getXAxisLabel());
        if (xl != null && !xl.isEmpty()) {
            final Texture texture = getTexture(colorMap, xl);
            return texture.getDataProvider().getTextureSize();
        }

        return new Dimension();
    }

    /**
     * Draws the given {@see Label} with the given {@see DrawingTools}.
     * @param drawingTools the given {@see DrawingTools}.
     * @param colorMap the current {@see ColorMap}.
     * @param label the given Scilab {@see Label}.
     * @param axesDrawer the Axes drawer used to draw the label's parent Axes.
     * @throws SciRendererException if the label is not drawable.
     */
    public void draw(final DrawingTools drawingTools, final ColorMap colorMap, final Label label, AxesDrawer axesDrawer) throws SciRendererException {
        /* Only the z-axis Label may not be drawn depending on the view mode */
        boolean drawnFlag = true;
        Integer parentId;
        Integer labelId = label.getIdentifier();
        LabelPositioner labelPositioner;

        parentId = label.getParentAxes();

        if (parentId == null) {
            return;
        }

        Axes parentAxes = (Axes) GraphicController.getController().getObjectFromId(parentId);

        /* Get the positioner associated to the label */
        if (parentAxes.getXAxisLabel().equals(label.getIdentifier())) {
            labelPositioner = axesDrawer.getXAxisLabelPositioner(parentAxes);
        } else if (parentAxes.getYAxisLabel().equals(label.getIdentifier())) {
            labelPositioner = axesDrawer.getYAxisLabelPositioner(parentAxes);
        } else if (parentAxes.getZAxisLabel().equals(label.getIdentifier())) {
            labelPositioner = axesDrawer.getZAxisLabelPositioner(parentAxes);
            drawnFlag = (parentAxes.getViewAsEnum() == Camera.ViewType.VIEW_3D);
        } else if (parentAxes.getTitle().equals(label.getIdentifier())) {
            labelPositioner = axesDrawer.getTitlePositioner(parentAxes);
        } else {
            /* Do not do anything */
            return;
        }

        /*
         * The topmost transformation, which is the data transformation, must be popped before drawing
         * and restored afterwards because Labels, like Axes rulers, are drawn in box coordinates.
         */
        drawingTools.getTransformationManager().getModelViewStack().pop();

        positionAndDraw(drawingTools, colorMap, label, labelPositioner, parentAxes, axesDrawer, drawnFlag);

        drawingTools.getTransformationManager().getModelViewStack().pushRightMultiply(axesDrawer.getDataTransformation());
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
     * @param parentAxes the label's parent Axes.
     * @param axesDrawer the Axes drawer used to draw the label's parent Axes.
     * @param drawnFlag a flag indicating whether the label must be drawn or not.
     * @throws SciRendererException if the label is not drawable.
     */
    public final void positionAndDraw(final DrawingTools drawingTools, final ColorMap colorMap, final Label label, LabelPositioner labelPositioner, Axes parentAxes, AxesDrawer axesDrawer, boolean drawnFlag) throws SciRendererException {
        boolean[] logFlags = new boolean[] {parentAxes.getXAxisLogFlag(), parentAxes.getYAxisLogFlag(), parentAxes.getZAxisLogFlag()};
        Texture labelSprite = getTexture(colorMap, label);

        labelPositioner.setLabelTexture(labelSprite);
        labelPositioner.setDrawingTools(drawingTools);
        labelPositioner.setParentAxes(parentAxes);
        labelPositioner.setAutoPosition(label.getAutoPosition());
        labelPositioner.setAutoRotation(label.getAutoRotation());
        labelPositioner.setUserRotationAngle(180.0 * label.getFontAngle() / Math.PI);

        double[][] factors = parentAxes.getScaleTranslateFactors();
        /* Un scale/translate the label position to be drawn */
        Vector3d labelUserPosition = computeUnscaledAxesCoords(new Vector3d(label.getPosition()), factors);

        /* Logarithmic scaling must be applied to the label's user position to obtain object coordinates */
        labelUserPosition = ScaleUtils.applyLogScale(labelUserPosition, logFlags);

        labelUserPosition = axesDrawer.getBoxCoordinates(labelUserPosition);
        labelPositioner.setLabelUserPosition(labelUserPosition);

        /* Computes the label's position values */
        labelPositioner.positionLabel();

        Vector3d labelPos = labelPositioner.getAnchorPoint();
        AnchorPosition labelAnchor = labelPositioner.getAnchorPosition();

        /* Set the lower-left corner position if auto-positioned */
        if (label.getAutoPosition()) {
            Vector3d cornerPos = labelPositioner.getLowerLeftCornerPosition();
            Vector3d objectCornerPos = axesDrawer.getObjectCoordinates(cornerPos);
            /* Apply inverse scaling to obtain user coordinates */
            objectCornerPos = ScaleUtils.applyInverseLogScale(objectCornerPos, logFlags);

            objectCornerPos = computeScaledAxesCoords(objectCornerPos, factors);
            label.setPosition(new Double[] {objectCornerPos.getX(), objectCornerPos.getY(), objectCornerPos.getZ()});

            /*
             * Must be reset to true as setPosition modifies the label's auto position field.
             * To be modified.
             */
            label.setAutoPosition(true);
        }

        /* Compute and set the label's corners */
        Transformation projection = axesDrawer.getProjection();
        Vector3d[] projCorners = labelPositioner.getProjCorners();
        Vector3d[] corners = computeCorners(projection, projCorners, parentAxes);
        Double[] coordinates = cornersToCoordinateArray(corners);

        /* Set the computed coordinates */
        label.setCorners(coordinates);

        double rotationAngle = 0.0;

        if (label.getAutoRotation()) {
            rotationAngle = labelPositioner.getRotationAngle();
            label.setFontAngle(Math.PI * rotationAngle / 180.0);

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
                    projLabelPos = new Vector3d(Math.round(projLabelPos.getX()), Math.round(projLabelPos.getY()), Math.round(projLabelPos.getZ()));
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
     * Computes the given scaled/translated coordinate for the given vector and factors
     * Used to keep the Label position and Corners in Axes coordinates
     * @param unscaled The un scaled/translated vector
     * @param factors The Axes factors
     * @return The new Coordinates
     */
    private Vector3d computeScaledAxesCoords(Vector3d unscaled, double[][] factors) {

        double[] coord = unscaled.getData();
        coord[0] = (coord[0] - factors[1][0]) / factors[0][0];
        coord[1] = (coord[1] - factors[1][1]) / factors[0][1];
        coord[2] = (coord[2] - factors[1][2]) / factors[0][2];

        return new Vector3d(coord);
    }

    /**
     * Computes the given un scaled/translated coordinate for the given vector and factors
     * Used to draw the Label in the correct position
     * @param scaled The scaled/translated vector
     * @param factors The Axes factors
     * @return The new Coordinates
     */
    private Vector3d computeUnscaledAxesCoords(Vector3d scaled, double[][] factors) {

        double[] coord = scaled.getData();
        coord[0] = coord[0] * factors[0][0] + factors[1][0];
        coord[1] = coord[1] * factors[0][1] + factors[1][1];
        coord[2] = coord[2] * factors[0][2] + factors[1][2];

        return new Vector3d(coord);
    }
    /**
     * Computes and returns the corners (in user coordinates) of a label's bounding box.
     * @param projection the projection from object coordinates to window coordinates.
     * @param projCorners the corners of the label's bounding box in window coordinates (4-element array).
     * @param parentAxes the Axes for which the coordinates are computed.
     * @return the corners of the label's bounding box in user coordinates (4-element array).
     */
    private Vector3d[] computeCorners(Transformation projection, Vector3d[] projCorners, Axes parentAxes) {
        Vector3d[] corners = new Vector3d[4];
        boolean[] logFlags = new boolean[] {parentAxes.getXAxisLogFlag(), parentAxes.getYAxisLogFlag(), parentAxes.getZAxisLogFlag()};

        double[][] factors = parentAxes.getScaleTranslateFactors();
        corners[0] = projection.unproject(projCorners[0]);
        corners[1] = projection.unproject(projCorners[1]);
        corners[2] = projection.unproject(projCorners[2]);
        corners[3] = projection.unproject(projCorners[3]);

        /* Apply inverse logarithmic scaling in order to obtain user coordinates */
        corners[0] = ScaleUtils.applyInverseLogScale(corners[0], logFlags);
        corners[1] = ScaleUtils.applyInverseLogScale(corners[1], logFlags);
        corners[2] = ScaleUtils.applyInverseLogScale(corners[2], logFlags);
        corners[3] = ScaleUtils.applyInverseLogScale(corners[3], logFlags);

        /* Used to keep corners in axes scaled/translated coordinates */
        corners[0] = computeScaledAxesCoords(corners[0], factors);
        corners[1] = computeScaledAxesCoords(corners[1], factors);
        corners[2] = computeScaledAxesCoords(corners[2], factors);
        corners[3] = computeScaledAxesCoords(corners[3], factors);

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
    public void update(Integer id, int property) {
        if (!(__GO_POSITION__ == property) && !(__GO_AUTO_POSITION__ == property)
                && !(__GO_FONT_ANGLE__ == property) && !(__GO_AUTO_ROTATION__ == property)) {
            dispose(id);
        }
    }

    /**
     * Returns the SciRenderer {@see Sprite} corresponding to the given Scilab {@see Label}.
     * @param colorMap the current color map.
     * @param label the given Scilab {@see Label}.
     * @return the SciRenderer {@see Sprite} corresponding to the given Scilab {@see Label}.
     */
    public Texture getTexture(final ColorMap colorMap, final Label label) {
        Texture texture = textureMap.get(label.getIdentifier());
        if (texture == null) {
            texture = createSprite(colorMap, label);
            textureMap.put(label.getIdentifier(), texture);
        }
        return texture;
    }

    /**
     * Creates a sprite for the given label.
     * @param colorMap the current colormap.
     * @param label the given label.
     * @return a new sprite for the given label.
     */
    private Texture createSprite(final ColorMap colorMap, final Label label) {
        LabelSpriteDrawer spriteDrawer = new LabelSpriteDrawer(colorMap, label);
        Texture sprite = textureManager.createTexture();
        sprite.setMagnificationFilter(Texture.Filter.LINEAR);
        sprite.setMinifyingFilter(Texture.Filter.LINEAR);
        sprite.setDrawer(spriteDrawer);
        return sprite;
    }

    /**
     * Disposes the texture corresponding to the given id.
     * @param id the given id.
     */
    public void dispose(Integer id) {
        Texture texture = textureMap.get(id);
        if (texture != null) {
            textureManager.dispose(texture);
            textureMap.remove(id);
        }
    }

    /**
     * Disposes all the label text sprites.
     */
    public void disposeAll() {
        textureManager.dispose(textureMap.values());
        textureMap.clear();
    }
}
