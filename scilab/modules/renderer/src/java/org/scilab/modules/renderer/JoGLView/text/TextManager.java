/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 * Copyright (C) 2012 - DIGITEO - Manuel Juliachs
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

package org.scilab.modules.renderer.JoGLView.text;

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureManager;
import org.scilab.forge.scirenderer.tranformations.DegenerateMatrixException;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.textObject.Text;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.renderer.JoGLView.util.ScaleUtils;

import java.awt.Dimension;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_ANGLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;

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
    protected final Map<Integer, Texture> spriteMap = new ConcurrentHashMap<Integer, Texture>();

    /**
     * The used texture manager.
     */
    protected final TextureManager textureManager;

    /**
     * The bounds of the scale factor range for which the texture does not
     * need to be updated.
     */
    protected double[] FACTOR_UPDATE_INTERVAL = {0.99, 1.01};

    /**
     * Default constructor.
     * @param textureManager the texture manager.
     */
    public TextManager(TextureManager textureManager) {
        this.textureManager = textureManager;
    }

    /**
     * Draw the given Scilab {@see Text} with the given {@see DrawingTools}.
     * @param drawingTools the given {@see DrawingTools}.
     * @param colorMap the current {@see ColorMap}
     * @param text the given Scilab {@see Text}
     * @throws SciRendererException if the draw fails.
     */
    public final void draw(final DrawingTools drawingTools, final ColorMap colorMap, final Text text) throws SciRendererException {
        Texture texture = getTexture(colorMap, text);

        /* The unscaled texture's dimensions */
        Dimension spriteDims = getSpriteDims(colorMap, text);

        Transformation projection = drawingTools.getTransformationManager().getCanvasProjection();

        Integer parentAxesId = text.getParentAxes();
        Axes parentAxes = (Axes) GraphicController.getController().getObjectFromId(parentAxesId);
        if (parentAxes != null) {
            double[][] factors = parentAxes.getScaleTranslateFactors();
            Double[] pos = text.getPosition();
            pos[0] = pos[0] * factors[0][0] + factors[1][0];
            pos[1] = pos[1] * factors[0][1] + factors[1][1];
            pos[2] = pos[2] * factors[0][2] + factors[1][2];

            Vector3d textPosition = new Vector3d(pos);

            /* Compute the text box vectors and the text box to texture dimension ratios */
            Vector3d[] textBoxVectors =  computeTextBoxVectors(projection, text, texture.getDataProvider().getTextureSize(), parentAxes);
            double[] ratios = computeRatios(projection, text, textBoxVectors, texture.getDataProvider().getTextureSize(), spriteDims);

            /* If text box mode is equal to filled, the texture must be updated */
            if (text.getTextBoxMode() == 2 && ratios[0] != 1.0) {
                texture = updateSprite(colorMap, text, ratios[0], ratios[1]);
            }

            /* Compute the text texture's actual position, which depends on the object's text box mode property */
            Vector3d[] cornerPositions = computeTextPosition(projection, text, textBoxVectors, texture.getDataProvider().getTextureSize());

            /* Draw in window coordinates */
            drawingTools.getTransformationManager().useWindowCoordinate();

            /* The Text object's rotation direction convention is opposite to the standard one, its angle is expressed in radians. */
            drawingTools.draw(texture, AnchorPosition.LOWER_LEFT, cornerPositions[0], -180.0 * text.getFontAngle() / Math.PI);

            drawingTools.getTransformationManager().useSceneCoordinate();

            /* Compute the corners of the text's bounding box in window coordinates */
            Vector3d[] projCorners;
            if (text.getTextBoxMode() == 2) {
                projCorners = computeProjTextBoxCorners(cornerPositions[1], text.getFontAngle(), textBoxVectors);
            } else {
                projCorners = computeProjCorners(cornerPositions[0], text.getFontAngle(), texture.getDataProvider().getTextureSize());
            }

            Vector3d[] corners = computeCorners(projection, projCorners, parentAxes);
            Double[] coordinates = cornersToCoordinateArray(corners);

            /* Set the computed coordinates */
            text.setCorners(coordinates);
        }
    }

    /**
     * Computes and returns the two vectors (in window coordinates) respectively corresponding
     * to the text box's base and side (also named the text box width and height vectors). If the
     * Text's text box mode is equal to off, the vectors are then equal to the Text label's base
     * and side vectors.  The Text's rotation is ignored, as it is not required for now.
     * @param projection the projection from object coordinates to window coordinates.
     * @param text the Scilab {@see Text}.
     * @param dimension the current text texture's dimension (in pixels).
     * @param parentAxes the Axes for which the coordinates are computed.
     * @return the text box width and height vectors (in window coordinates).
     */
    protected Vector3d[] computeTextBoxVectors(Transformation projection, Text text, Dimension dimension, Axes parentAxes) {
        Double[] textBox = text.getTextBox();
        Vector3d[] textBoxVectors = new Vector3d[2];
        double[][] factors = parentAxes.getScaleTranslateFactors();
        Double[] pos = text.getPosition();

        /* The text position vector before logarithmic scaling */
        Vector3d textPosition = new Vector3d(pos);

        boolean[] logFlags = new boolean[] {parentAxes.getXAxisLogFlag(), parentAxes.getYAxisLogFlag(), parentAxes.getZAxisLogFlag()};

        /* Apply logarithmic scaling and then project */
        textPosition = ScaleUtils.applyLogScale(textPosition, logFlags);
        textPosition = new Vector3d(textPosition.getX() * factors[0][0] + factors[1][0], textPosition.getY() * factors[0][1] + factors[1][1], textPosition.getZ() * factors[0][2] + factors[1][2]);
        Vector3d projTextPosition = projection.project(textPosition);

        /* Compute the text label vectors in window coordinates */
        Vector3d width = new Vector3d(dimension.getWidth(), 0.0, 0.0);
        Vector3d height = new Vector3d(0.0, dimension.getHeight(), 0.0);

        Vector3d projTextWidth = projTextPosition.plus(width);
        Vector3d projTextHeight = projTextPosition.plus(height);

        /*
         * Compute the text box's vectors in object coordinates, from the object coordinate text label vectors.
         * Their norms are unaffected by the text's rotation, which is thus ignored.
         */
        Vector3d textWidth = projection.unproject(projTextWidth);
        Vector3d textHeight = projection.unproject(projTextHeight);

        /* Applies inverse logarithmic scaling */
        textWidth = ScaleUtils.applyInverseLogScale(textWidth, logFlags);
        textHeight = ScaleUtils.applyInverseLogScale(textHeight, logFlags);

        textWidth = textWidth.minus(textPosition);
        textHeight = textHeight.minus(textPosition);

        if (text.getTextBoxMode() >= 1) {
            textWidth = textWidth.getNormalized().times(textBox[0] * factors[0][0]);
            textHeight = textHeight.getNormalized().times(textBox[1] * factors[0][1]);
        }

        /*
         * We take into account the reverse axes flags in order to
         * compute the actual text box corners, and hence the correct vectors,
         * which is necessary when logarithmic scaling is applied.
         */
        if (parentAxes.getXAxisReverse()) {
            textWidth = textWidth.setX(Math.abs(textWidth.getX()));
            textHeight = textHeight.setX(Math.abs(textHeight.getX()));
        }
        if (parentAxes.getYAxisReverse()) {
            textWidth = textWidth.setY(Math.abs(textWidth.getY()));
            textHeight = textHeight.setY(Math.abs(textHeight.getY()));
        }
        if (parentAxes.getZAxisReverse()) {
            textWidth = textWidth.setZ(Math.abs(textWidth.getZ()));
            textHeight = textHeight.setZ(Math.abs(textHeight.getZ()));
        }

        /* Computes the lower-right and upper-left corners. */
        textWidth = textWidth.plus(textPosition);
        textHeight = textHeight.plus(textPosition);

        /* Finally re-apply logarithmic scaling, compute the vectors and project */
        textWidth = ScaleUtils.applyLogScale(textWidth, logFlags);
        textHeight = ScaleUtils.applyLogScale(textHeight, logFlags);

        textWidth = textWidth.minus(textPosition);
        textHeight = textHeight.minus(textPosition);

        projTextWidth = projection.projectDirection(textWidth);
        projTextHeight = projection.projectDirection(textHeight);

        /*
         * Ensures that the two window-coordinate base vectors respectively point to the right
         * and to the top, as taking reversed axes into account may have reversed them (see above).
         */
        projTextWidth = projTextWidth.setX(Math.abs(projTextWidth.getX()));
        projTextHeight = projTextHeight.setX(Math.abs(projTextHeight.getX()));
        projTextWidth = projTextWidth.setY(Math.abs(projTextWidth.getY()));
        projTextHeight = projTextHeight.setY(Math.abs(projTextHeight.getY()));

        textBoxVectors[0] = projTextWidth;
        textBoxVectors[1] = projTextHeight;

        return textBoxVectors;
    }

    /**
     * Computes and returns the minimum of the ratios between the text box and the text texture's dimensions.
     * This minimum ratio is determined for both the current text texture and the unscaled text texture.
     * @param projection the projection from object coordinates to window coordinates.
     * @param text the Scilab {@see Text}.
     * @param textBoxVectors the text box width and height vectors (in window coordinates).
     * @param spriteDimension the current text texture's dimension (in pixels).
     * @param baseSpriteDimension the unscaled text texture's dimension (in pixels).
     * @return the minimum ratios (2 elements: text box to current texture and text box to unscaled texture ratios).
     */
    protected double[] computeRatios(Transformation projection, Text text, Vector3d[] textBoxVectors, Dimension spriteDimension,
                                     Dimension baseSpriteDimension) {
        /* 1st element: ratio for the current texture, 2nd element: ratio for the unscaled texture */
        double[] ratios = new double[] {1.0, 1.0};

        /* Ratios are relevant only to the filled text box mode */
        if (text.getTextBoxMode() == 2) {
            Vector3d textBoxWidth = textBoxVectors[0];
            Vector3d textBoxHeight = textBoxVectors[1];

            /* Compute the ratios. */
            double minRatio = Math.min(Math.abs(textBoxWidth.getX() / spriteDimension.width), Math.abs(textBoxHeight.getY() / spriteDimension.height));
            double baseMinRatio = Math.min(Math.abs(textBoxWidth.getX() / baseSpriteDimension.width), Math.abs(textBoxHeight.getY() / baseSpriteDimension.height));

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
     * @param textBoxVectors the text box width and height vectors (in window coordinates).
     * @param spriteDim the text texture's dimension (in pixels).
     * @return the lower-left corners of the Scilab {@see Text}'s text and of its text box in window coordinates (2 elements).
     * @throws DegenerateMatrixException if the projection is not possible.
     */
    protected Vector3d[] computeTextPosition(Transformation projection, Text text, Vector3d[] textBoxVectors, Dimension spriteDim) throws DegenerateMatrixException {
        Vector3d[] cornerPositions = new Vector3d[2];
        Integer parentAxesId = text.getParentAxes();
        Axes parentAxes = (Axes) GraphicController.getController().getObjectFromId(parentAxesId);
        double[][] factors = parentAxes.getScaleTranslateFactors();
        Double[] pos = text.getPosition();
        Vector3d textPosition = new Vector3d(pos);

        /* Apply logarithmic scaling */
        boolean[] logFlags = new boolean[] {parentAxes.getXAxisLogFlag(), parentAxes.getYAxisLogFlag(), parentAxes.getZAxisLogFlag()};
        textPosition = ScaleUtils.applyLogScale(textPosition, logFlags);
        textPosition = new Vector3d(textPosition.getX() * factors[0][0] + factors[1][0], textPosition.getY() * factors[0][1] + factors[1][1], textPosition.getZ() * factors[0][2] + factors[1][2]);

        textPosition = projection.project(textPosition);

        cornerPositions[0] = new Vector3d(textPosition);
        cornerPositions[1] = new Vector3d(textPosition);

        if (text.getTextBoxMode() >= 1) {
            Vector3d textBoxWidth = new Vector3d(textBoxVectors[0]);
            Vector3d textBoxHeight = new Vector3d(textBoxVectors[1]);

            double[] textBoxWidthData = textBoxWidth.getData();
            double[] textBoxHeightData = textBoxHeight.getData();

            /* Reversed axes must be taken into account to correctly compute the text texture's lower-left corner. */
            if (parentAxes.getXAxisReverse()) {
                textBoxWidthData[0] = -textBoxWidthData[0];
                textBoxHeightData[0] = -textBoxHeightData[0];
            }

            if (parentAxes.getYAxisReverse()) {
                textBoxWidthData[1] = -textBoxWidthData[1];
                textBoxHeightData[1] = -textBoxHeightData[1];
            }

            if (parentAxes.getZAxisReverse()) {
                textBoxWidthData[2] = -textBoxWidthData[2];
                textBoxHeightData[2] = -textBoxHeightData[2];
            }

            Vector3d revTextBoxWidth = new Vector3d(textBoxWidthData);
            Vector3d revTextBoxHeight = new Vector3d(textBoxHeightData);

            Vector3d[] projCorners = computeProjCorners(textPosition, text.getFontAngle(), spriteDim);

            Vector3d textWidth = projCorners[1].minus(projCorners[0]);
            Vector3d textHeight = projCorners[2].minus(projCorners[0]);

            /*
             * Compute the final text box's and text's half-length vectors,
             * using the rotated text label vectors.
             */
            revTextBoxWidth = textWidth.getNormalized().times(0.5 * revTextBoxWidth.getX());
            revTextBoxHeight = textHeight.getNormalized().times(0.5 * revTextBoxHeight.getY());

            textBoxWidth = textWidth.getNormalized().times(0.5 * textBoxWidth.getX());
            textBoxHeight = textHeight.getNormalized().times(0.5 * textBoxHeight.getY());

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
     * @param spriteDim the text texture's dimension (in pixels).
     * @return the corners' window coordinates (4-element array).
     * @throws DegenerateMatrixException if the projection is not possible.
     */
    protected Vector3d[] computeProjCorners(Transformation canvasProj, Vector3d position, double fontAngle, Dimension spriteDim) throws DegenerateMatrixException {
        position = canvasProj.project(position);
        return computeProjCorners(position, fontAngle, spriteDim);
    }

    /**
     * Computes the corners of a {@see Text} object's text box, in window coordinates.
     * @param position the position of the text box's lower-left corner in window cordinates.
     * @param fontAngle the text's font angle (radians).
     * @param textBoxVectors the text box width and height vectors (in window coordinates).
     * @return the corners' window coordinates (4-element array).
     * @throws DegenerateMatrixException if the projection is not possible.
     */
    protected Vector3d[] computeProjTextBoxCorners(Vector3d position, double fontAngle, Vector3d[] textBoxVectors) throws DegenerateMatrixException {
        double projTextBoxWidth = textBoxVectors[0].getNorm();
        double projTextBoxHeight = textBoxVectors[1].getNorm();

        return computeProjCorners(position, fontAngle, new Dimension((int) projTextBoxWidth, (int) projTextBoxHeight));
    }

    /**
     * Computes and returns the corners of a {@see Text} object's bounding box, in window coordinates.
     * The returned corners are in the following order: lower-left, lower-right, upper-left and upper-right.
     * @param projPosition the text's position in window coordinates.
     * @param fontAngle the text's font angle (radians).
     * @param spriteDim the text texture's dimension (in pixels).
     * @return the corners' window coordinates (4-element array).
     * @throws DegenerateMatrixException if the projection is not possible.
     */
    protected Vector3d[] computeProjCorners(Vector3d projPosition, double fontAngle, Dimension spriteDim) throws DegenerateMatrixException {
        Vector3d[] projCorners = new Vector3d[4];

        /*
         * Should be -fontAngle, as positive angle values are measured clockwise for texts.
         * Apparently uses the same convention as the texts (clockwise positive directions).
         * To be verified.
         */
        Transformation projRotation = TransformationFactory.getRotationTransformation(180.0 * fontAngle / Math.PI, 0.0, 0.0, 1.0);

        projCorners[0] = projPosition;

        Vector3d width = new Vector3d(spriteDim.width, 0.0, 0.0);
        Vector3d height = new Vector3d(0.0, spriteDim.height, 0.0);

        width = projRotation.projectDirection(width);
        height = projRotation.projectDirection(height);

        projCorners[1] = projCorners[0].plus(width);
        projCorners[2] = projCorners[0].plus(height);
        projCorners[3] = projCorners[2].plus(width);

        return projCorners;
    }

    /**
     * Computes and returns the corners (in user coordinates) of a text's bounding box.
     * @param projection the projection from object coordinates to window coordinates.
     * @param projCorners the corners of the text's bounding box in window coordinates (4-element array).
     * @param parentAxes the Axes for which the coordinates are computed.
     * @return the corners of the text's bounding box in user coordinates (4-element array).
     */
    protected Vector3d[] computeCorners(Transformation projection, Vector3d[] projCorners, Axes parentAxes) {
        Vector3d[] corners = new Vector3d[4];
        boolean[] logFlags = new boolean[] {parentAxes.getXAxisLogFlag(), parentAxes.getYAxisLogFlag(), parentAxes.getZAxisLogFlag()};

        corners[0] = projection.unproject(projCorners[0]);
        corners[1] = projection.unproject(projCorners[1]);
        corners[2] = projection.unproject(projCorners[2]);
        corners[3] = projection.unproject(projCorners[3]);

        /* Apply inverse logarithmic scaling in order to obtain user coordinates */
        corners[0] = ScaleUtils.applyInverseLogScale(corners[0], logFlags);
        corners[1] = ScaleUtils.applyInverseLogScale(corners[1], logFlags);
        corners[2] = ScaleUtils.applyInverseLogScale(corners[2], logFlags);
        corners[3] = ScaleUtils.applyInverseLogScale(corners[3], logFlags);

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
    protected Double[] cornersToCoordinateArray(Vector3d[] corners) {
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
    public void update(Integer id, int property) {
        if (!(__GO_POSITION__ == property) && !(__GO_FONT_ANGLE__ == property)) {
            dispose(id);
        }
    }

    /**
     * Return the SciRenderer {@see Texture} corresponding to the given Scilab {@see Text}.
     * @param colorMap the current color map.
     * @param text the given Scilab {@see Text}.
     * @return the SciRenderer {@see Texture} corresponding to the given Scilab {@see Text}.
     */
    protected Texture getTexture(final ColorMap colorMap, final Text text) {
        Texture texture = spriteMap.get(text.getIdentifier());
        if (texture == null) {
            if (text.getTextBoxMode() == 2) {
                /* Create an unscaled texture (scale factor equal to 1) */
                texture = createSprite(colorMap, text, 1.0);
            } else {
                texture = createSprite(colorMap, text);
            }
            spriteMap.put(text.getIdentifier(), texture);
        }
        return texture;
    }

    /**
     * Updates a texture according to a scale factor, which is the ratio by which the texture is enlarged.
     * @param colorMap the current color map.
     * @param text the Scilab Text.
     * @param scaleFactor the scale factor relative to the current texture's dimensions.
     * @param baseScaleFactor the scale factor relative to the unscaled texture's dimensions.
     * @return the corresponding texture.
     */
    protected Texture updateSprite(final ColorMap colorMap, final Text text, double scaleFactor, double baseScaleFactor) {
        Texture texture = spriteMap.get(text.getIdentifier());

        /* Create a new texture if the scale factor falls outside of the range */
        if (scaleFactor < FACTOR_UPDATE_INTERVAL[0] || scaleFactor > FACTOR_UPDATE_INTERVAL[1]) {
            dispose(text.getIdentifier());
            texture = createSprite(colorMap, text, baseScaleFactor);
            spriteMap.put(text.getIdentifier(), texture);
        }

        return texture;
    }

    /**
     * Returns the dimensions of the SciRenderer {@see Texture} corresponding to the given Scilab {@see Text}.
     * The dimensions are in pixels (width, height).
     * @param colorMap the current color map.
     * @param text the given Scilab {@see Text}.
     * @return the texture's dimension.
     */
    protected Dimension getSpriteDims(final ColorMap colorMap, final Text text) {
        TextSpriteDrawer spriteDrawer;

        if (text.getTextBoxMode() == 2) {
            /* Set the scale factor to 1 in order to return the dimensions of an unscaled texture. */
            spriteDrawer = new TextSpriteDrawer(colorMap, text, 1.0);
        } else {
            spriteDrawer = new TextSpriteDrawer(colorMap, text);
        }

        return spriteDrawer.getTextureSize();
    }

    /**
     * Create a texture for the given text object.
     * @param colorMap the current colormap.
     * @param textObject the given text object.
     * @return a new texture for the given text object.
     */
    protected Texture createSprite(final ColorMap colorMap, final Text textObject) {
        TextSpriteDrawer spriteDrawer = new TextSpriteDrawer(colorMap, textObject);
        Texture texture = textureManager.createTexture();
        texture.setMagnificationFilter(Texture.Filter.LINEAR);
        texture.setMinifyingFilter(Texture.Filter.LINEAR);
        texture.setDrawer(spriteDrawer);
        return texture;
    }

    /**
     * Creates a texture for the given text object, scaled by the given scale factor.
     * @param colorMap the current colormap.
     * @param textObject the given text object.
     * @param scaleFactor the scale factor to apply.
     * @return a new texture for the given text object.
     */
    protected Texture createSprite(final ColorMap colorMap, final Text textObject, double scaleFactor) {
        TextSpriteDrawer spriteDrawer = new TextSpriteDrawer(colorMap, textObject, scaleFactor);
        Texture texture = textureManager.createTexture();
        texture.setMagnificationFilter(Texture.Filter.LINEAR);
        texture.setMinifyingFilter(Texture.Filter.LINEAR);
        texture.setDrawer(spriteDrawer);
        return texture;
    }

    /**
     * Dispose the texture corresponding to the given id.
     * @param id the given id.
     */
    public void dispose(Integer id) {
        Texture texture = spriteMap.get(id);
        if (texture != null) {
            textureManager.dispose(texture);
            spriteMap.remove(id);
        }
    }

    /**
     * Dispose all the text sprites.
     */
    public void disposeAll() {
        textureManager.dispose(spriteMap.values());
        spriteMap.clear();
    }

    /**
     * Computes and updates the corners of the given Scilab {@see Text}.
     * @param text the given Scilab {@see Text}.
     */
    public static void updateTextCorners(Text text) {
        Vector3d[] projCorners = null;

        DrawerVisitor currentVisitor = DrawerVisitor.getVisitor(text.getParentFrameOrFigure());
        Axes parentAxes = (Axes) GraphicController.getController().getObjectFromId(text.getParentAxes());

        /* Compute the corners */
        try {
            Transformation currentProj = currentVisitor.getAxesDrawer().getCurrentProjection(parentAxes);

            Dimension spriteDim = currentVisitor.getTextManager().getSpriteDims(currentVisitor.getColorMap(), text);

            Vector3d[] textBoxVectors = currentVisitor.getTextManager().computeTextBoxVectors(currentProj, text, spriteDim, parentAxes);
            Vector3d[] cornerPositions = currentVisitor.getTextManager().computeTextPosition(currentProj, text, textBoxVectors, spriteDim);

            if (text.getTextBoxMode() == 2) {
                projCorners = currentVisitor.getTextManager().computeProjTextBoxCorners(cornerPositions[1], text.getFontAngle(), textBoxVectors);
            } else {
                projCorners = currentVisitor.getTextManager().computeProjCorners(cornerPositions[0], text.getFontAngle(), spriteDim);
            }

            Vector3d[] corners = currentVisitor.getTextManager().computeCorners(currentProj, projCorners, parentAxes);
            Double[] coordinates = currentVisitor.getTextManager().cornersToCoordinateArray(corners);

            double[][] factors = parentAxes.getScaleTranslateFactors();
            coordinates[0] = (coordinates[0] - factors[1][0]) / factors[0][0];
            coordinates[1] = (coordinates[1] - factors[1][1]) / factors[0][1];
            coordinates[2] = (coordinates[2] - factors[1][2]) / factors[0][2];
            coordinates[3] = (coordinates[3] - factors[1][0]) / factors[0][0];
            coordinates[4] = (coordinates[4] - factors[1][1]) / factors[0][1];
            coordinates[5] = (coordinates[5] - factors[1][2]) / factors[0][2];
            coordinates[6] = (coordinates[6] - factors[1][0]) / factors[0][0];
            coordinates[7] = (coordinates[7] - factors[1][1]) / factors[0][1];
            coordinates[8] = (coordinates[8] - factors[1][2]) / factors[0][2];
            coordinates[9] = (coordinates[9] - factors[1][0]) / factors[0][0];
            coordinates[10] = (coordinates[10] - factors[1][1]) / factors[0][1];
            coordinates[11] = (coordinates[11] - factors[1][2]) / factors[0][2];

            /* Set the computed coordinates */
            text.setCorners(coordinates);

        } catch (DegenerateMatrixException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }
}
