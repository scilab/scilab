/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011-2012 - DIGITEO - Manuel JULIACHS
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

package org.scilab.modules.renderer.JoGLView.legend;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.buffers.IndicesBuffer;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureManager;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.TransformationStack;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.legend.Legend;
import org.scilab.modules.graphic_objects.legend.Legend.LegendLocation;
import org.scilab.modules.graphic_objects.polyline.Polyline;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.renderer.JoGLView.mark.MarkSpriteManager;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;

import java.awt.Dimension;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;


/**
 * LegendDrawer class.
 * Utility class used by DrawerVisitor to draw the Legend object.
 *
 * To do:
 * - clean up the code
 * - modify bounds depending on the location (for OUT_* values)
 * - take into account the actual tick/label size (instead of an arbitrary value)
 * - implement box clipping mode
 * - use the GraphicController to update links / move the update code somewhere else
 *   in order to perform it when objects are deleted, add the Links property to the set of
 *   properties affecting the sprite's update.
 *
 * @author Manuel JULIACHS
 */
public class LegendDrawer {

    /**
     * Set of properties that affect the legend sprite
     */
    private static final Set<Integer> SPRITE_PROPERTIES = new HashSet<Integer>(Arrays.asList(
                GraphicObjectProperties.__GO_FONT_SIZE__,
                GraphicObjectProperties.__GO_FONT_COLOR__,
                GraphicObjectProperties.__GO_FONT_STYLE__,
                GraphicObjectProperties.__GO_FONT_FRACTIONAL__,
                GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__,
                GraphicObjectProperties.__GO_TEXT_STRINGS__
            ));

    /** The height of a bar relative to the height difference between two lines */
    private static final float BAR_HEIGHT = 0.5f;

    /** The DrawerVisitor used */
    private final DrawerVisitor visitor;

    /** The SpriteManager used */
    private final TextureManager textureManager;

    /** The MarkSpriteManager used */
    private final MarkSpriteManager markManager;

    /** The Legend sprite drawer */
    private LegendSpriteDrawer legendSpriteDrawer;

    /** The relative line width */
    public static final double LINE_WIDTH = 0.1;

    /** The relative y-offset */
    public static final double Y_OFFSET = 0.01;

    /** The relative tick and label size (arbitrarily chosen) */
    private static final double TICK_LABEL_SIZE = 0.055;

    /** The z-value corresponding to the frontmost position */
    private static final float Z_FRONT = 0.99f;

    /** The legend background's vertices */
    private ElementsBuffer rectangleVertices;

    /** The vertices used to draw lines */
    private ElementsBuffer lineVertices;

    /** The vertices used to draw bars */
    private ElementsBuffer barVertices;

    /** The indices used to draw lines */
    private IndicesBuffer lineIndices;

    /** The indices used to draw a rectangle */
    private IndicesBuffer rectangleIndices;

    /** The indices used to draw the outline of a rectangle */
    private IndicesBuffer rectangleOutlineIndices;

    /** The map storing legend text sprites */
    private Map<Integer, Texture> textureMap;

    /**
     * Constructor.
     * @param visitor the DrawerVisitor {@see DrawerVisitor}.
     */
    public LegendDrawer(DrawerVisitor visitor) {
        this.visitor = visitor;
        this.textureManager = visitor.getCanvas().getTextureManager();
        this.markManager = visitor.getMarkManager();

        rectangleVertices = visitor.getCanvas().getBuffersManager().createElementsBuffer();
        lineVertices = visitor.getCanvas().getBuffersManager().createElementsBuffer();
        barVertices = visitor.getCanvas().getBuffersManager().createElementsBuffer();
        lineIndices = visitor.getCanvas().getBuffersManager().createIndicesBuffer();
        rectangleIndices = visitor.getCanvas().getBuffersManager().createIndicesBuffer();
        rectangleOutlineIndices = visitor.getCanvas().getBuffersManager().createIndicesBuffer();

        textureMap = new ConcurrentHashMap<Integer, Texture>();

        int[] lineIndexData = new int[] {0, 1, 1, 2};
        lineIndices.setData(lineIndexData);
    }

    public Dimension computeDimensions(Axes parentAxes, Legend legend) {
        LegendLocation loc = legend.getLegendLocationAsEnum();
        if (loc != LegendLocation.IN_UPPER_RIGHT && loc != LegendLocation.IN_UPPER_LEFT &&
                loc != LegendLocation.IN_LOWER_RIGHT && loc != LegendLocation.IN_LOWER_LEFT) {
            Figure figure = (Figure) GraphicController.getController().getObjectFromId(parentAxes.getParentFigure());
            final ColorMap colorMap = figure.getColorMap();
            final Integer[] links = legend.getLinks();
            final int nbValidLinks = getNumberValidLinks(legend);
            if (nbValidLinks < links.length) {
                dispose(legend.getIdentifier());
                updateLinks(legend, nbValidLinks);
            }

            if (nbValidLinks > 0) {
                Texture legendSprite = getTexture(colorMap, legend);
                return legendSprite.getDataProvider().getTextureSize();
            }
        }

        return null;
    }

    /**
     * Draws the given Legend.
     * @param legend the Legend to draw.
     * @throws SciRendererException if the draw fail.
     */
    public void draw(Legend legend) throws SciRendererException {
        /* The coordinates of the legend box's lower-left corner */
        double[] legendCorner = new double[] {0.25, 0.75, Z_FRONT};

        DrawingTools drawingTools = visitor.getDrawingTools();
        ColorMap colorMap = visitor.getColorMap();
        Canvas canvas = visitor.getCanvas();

        Integer[] links = legend.getLinks();

        /*
         * Determine whether any links have become invalid,
         * force the sprite's update and update the Legend's
         * links property as needed.
         */
        int nbValidLinks = getNumberValidLinks(legend);

        if (nbValidLinks < links.length) {
            dispose(legend.getIdentifier());
            updateLinks(legend, nbValidLinks);
        }

        links = legend.getLinks();

        /*
         * Set the projection and modelview transformations so that coordinates
         * are specified in the {0,+1,0,+1,0,+1} space.
         */
        TransformationStack modelViewStack = drawingTools.getTransformationManager().getModelViewStack();
        TransformationStack projectionStack = drawingTools.getTransformationManager().getProjectionStack();

        Transformation identity = TransformationFactory.getIdentity();
        modelViewStack.push(identity);

        Transformation orthoProj = TransformationFactory.getOrthographic(0.0, 1.0, 0.0, 1.0, -1.0, 0.0);
        projectionStack.push(orthoProj);


        /* First, compute the legend box's position and dimensions from the Axes' parameters and the text sprite's dimensions */

        Integer parentAxesID = legend.getParentAxes();
        Axes parentAxes = (Axes) GraphicController.getController().getObjectFromId(parentAxesID);

        Double[] axesBounds = parentAxes.getAxesBounds();
        Double[] margins = parentAxes.getMargins();

        int xAxisLocation = parentAxes.getXAxisLocation();
        int yAxisLocation = parentAxes.getYAxisLocation();


        int canvasWidth = canvas.getWidth();
        int canvasHeight = canvas.getHeight();

        if (canvasWidth == 0) {
            canvasWidth = 1;
        }

        if (canvasHeight == 0) {
            canvasHeight = 1;
        }

        Texture legendSprite = null;
        double normSpriteWidth = 0;
        double normSpriteHeight = 0;

        if (nbValidLinks > 0) {
            legendSprite = getTexture(colorMap, legend);
            Dimension textureSize = legendSprite.getDataProvider().getTextureSize();
            normSpriteWidth = textureSize.getWidth() / (double) canvasWidth;
            normSpriteHeight = textureSize.getHeight() / (double) canvasHeight;
        }

        double lineWidth;

        /* The legend box's width and height */
        double[] legendDims = new double[2];

        double[] axesPos = new double[2];
        double[] axesDims = new double[2];

        lineWidth = legend.getLineWidth() * (axesBounds[2]) * (1.0 - margins[0] - margins[1]);

        double xOffset = LINE_WIDTH * (axesBounds[2]) * (1.0 - margins[0] - margins[1]) / 8.0;
        double yOffset = (Y_OFFSET * (axesBounds[3]) * (1.0 - margins[2] - margins[3]));

        /*
         * If one of these relations is modified, then AxesDrawer::computeMargins should be modified too
         */
        legendDims[0] = normSpriteWidth + lineWidth + 3.0 * xOffset;
        legendDims[1] = normSpriteHeight + 2.0 * yOffset;

        /* Save the legend box size */
        Double[] DimsToSet = { legendDims[0], legendDims[1]};
        legend.setSize(DimsToSet);

        axesPos[0] = axesBounds[0];
        axesPos[1] = 1.0 - (axesBounds[1] + axesBounds[3]);

        axesDims[0] = axesBounds[2];
        axesDims[1] = axesBounds[3];

        /* The {x, y} coordinates of the axes box's lower-left and upper-right corners (as defined by bounds and margins) */
        double[] llBoxCorner = new double[2];
        double[] urBoxCorner = new double[2];

        LegendLocation legendLocation = legend.getLegendLocationAsEnum();

        llBoxCorner[0] = axesPos[0] + margins[0] * axesDims[0];
        llBoxCorner[1] = axesPos[1] + margins[3] * axesDims[1];

        urBoxCorner[0] = axesPos[0] + (1.0 - margins[1]) * axesDims[0];
        urBoxCorner[1] = axesPos[1] + (1.0 - margins[2]) * axesDims[1];

        switch (legendLocation) {
            case IN_UPPER_RIGHT:
                legendCorner[0] = (float) (urBoxCorner[0] - xOffset - legendDims[0]);
                legendCorner[1] = (float) (urBoxCorner[1] - yOffset - legendDims[1]);
                break;
            case IN_UPPER_LEFT:
                legendCorner[0] = (float) (llBoxCorner[0] + xOffset);
                legendCorner[1] = (float) (urBoxCorner[1] - yOffset - legendDims[1]);
                break;
            case IN_LOWER_RIGHT:
                legendCorner[0] = (float) (urBoxCorner[0] - xOffset - legendDims[0]);
                legendCorner[1] = (float) (llBoxCorner[1] + yOffset);
                break;
            case IN_LOWER_LEFT:
                legendCorner[0] = (float) (llBoxCorner[0] + xOffset);
                legendCorner[1] = (float) (llBoxCorner[1] + yOffset);
                break;
            case OUT_UPPER_RIGHT:
                legendCorner[0] = (float) (urBoxCorner[0] + xOffset);
                legendCorner[1] = (float) (urBoxCorner[1] - legendDims[1]);
                break;
            case OUT_UPPER_LEFT:
                legendCorner[0] = (float) (llBoxCorner[0] - xOffset - legendDims[0]);
                legendCorner[1] = (float) (urBoxCorner[1] - legendDims[1]);
                break;
            case OUT_LOWER_RIGHT:
                legendCorner[0] = (float) (urBoxCorner[0] + xOffset);
                legendCorner[1] = (float) (llBoxCorner[1]);
                break;
            case OUT_LOWER_LEFT:
                legendCorner[0] = (float) (llBoxCorner[0] - xOffset - legendDims[0]);
                legendCorner[1] = (float) (llBoxCorner[1]);
                break;
            case UPPER_CAPTION:
                legendCorner[0] = (float) (llBoxCorner[0]);
                legendCorner[1] = (float) (urBoxCorner[1] + yOffset);

                /* x-axis at the top */
                if (xAxisLocation == 1) {
                    /* To do: use the actual label+tick bounding box height */
                    legendCorner[1] += TICK_LABEL_SIZE;
                }
                break;
            case LOWER_CAPTION:
                legendCorner[0] = (float) (llBoxCorner[0]);
                legendCorner[1] = (float) (llBoxCorner[1] - yOffset - legendDims[1]);

                /* x-axis at the bottom */
                if (xAxisLocation == 0) {
                    /* To do: use the actual label+tick bounding box height */
                    legendCorner[1] -= TICK_LABEL_SIZE;
                }
                break;
            case BY_COORDINATES:
                Double[] legPos = legend.getPosition();

                legendCorner[0] = (float) (axesPos[0] + legPos[0] * axesBounds[2]);
                legendCorner[1] = (float) (axesPos[1] + (1.0 - legPos[1]) * axesBounds[3] - legendDims[1]);
                break;
        }

        /* y-axis positioned to the left */
        if ((legendLocation == LegendLocation.OUT_UPPER_LEFT || legendLocation == LegendLocation.OUT_LOWER_LEFT) && yAxisLocation == 4) {
            /* To do: use the actual label+tick bounding box width */
            legendCorner[0] -= TICK_LABEL_SIZE;
            /* y-axis positioned to the right */
        } else if ((legendLocation == LegendLocation.OUT_UPPER_RIGHT || legendLocation == LegendLocation.OUT_LOWER_RIGHT) && yAxisLocation == 5) {
            /* To do: use the actual label+tick bounding box width */
            legendCorner[0] += TICK_LABEL_SIZE;
        }


        /* Afterwards, draw the elements making up the Legend using the previously computed values */

        /* Legend background vertex data: lower-left, lower-right, upper-left and upper-right corners */
        float[] rectangleVertexData = new float[] {
            (float)legendCorner[0], (float)legendCorner[1], Z_FRONT, 1.0f,
            (float)(legendCorner[0] + legendDims[0]), (float)legendCorner[1], Z_FRONT, 1.0f,
            (float)legendCorner[0], (float)(legendCorner[1] + legendDims[1]), Z_FRONT, 1.0f,
            (float)(legendCorner[0] + legendDims[0]), (float)(legendCorner[1] + legendDims[1]), Z_FRONT, 1.0f
        };

        /* The indices of a rectangle's triangles and a rectangle outline's segment loop */
        int[] rectangleIndexData = new int[] {0, 1, 3, 0, 3, 2};
        int[] rectangleOutlineIndexData = new int[] {0, 1, 1, 3, 3, 2, 2, 0};

        rectangleIndices.setData(rectangleIndexData);
        rectangleOutlineIndices.setData(rectangleOutlineIndexData);

        rectangleVertices.setData(rectangleVertexData, 4);

        /* Legend rectangle background and outline */
        DefaultGeometry legendRectangle = new DefaultGeometry();
        legendRectangle.setVertices(rectangleVertices);
        legendRectangle.setIndices(rectangleIndices);

        Appearance appearance = new Appearance();

        if (legend.getFillMode()) {
            legendRectangle.setFillDrawingMode(Geometry.FillDrawingMode.TRIANGLES);
            appearance.setFillColor(ColorFactory.createColor(colorMap, legend.getBackground()));
        } else {
            legendRectangle.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
        }

        /* Legend outline */
        if (legend.getLineMode()) {
            legendRectangle.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
            legendRectangle.setWireIndices(rectangleOutlineIndices);

            appearance.setLineColor(ColorFactory.createColor(colorMap, legend.getLineColor()));
            appearance.setLineWidth(legend.getLineThickness().floatValue());
            appearance.setLinePattern(legend.getLineStyleAsEnum().asPattern());
        } else {
            legendRectangle.setLineDrawingMode(Geometry.LineDrawingMode.NONE);
        }

        drawingTools.draw(legendRectangle, appearance);

        /* Lines: 3 vertices each, left, middle, and right */
        float[] lineVertexData = new float[] {0.25f, 0.75f, Z_FRONT, 1.0f,
                                              0.5f, 0.75f, Z_FRONT, 1.0f,
                                              0.75f, 0.75f, Z_FRONT, 1.0f
                                             };

        double normSpriteMargin = 0.0;

        if (nbValidLinks > 0) {
            normSpriteMargin = (double) legendSpriteDrawer.getVMargin() / (double) canvasHeight;
        }

        lineVertexData[0] = (float) (legendCorner[0] + xOffset);
        lineVertexData[1] = (float) (legendCorner[1] + normSpriteMargin + yOffset);

        lineVertexData[4] = lineVertexData[0] + 0.5f * (float) lineWidth;
        lineVertexData[5] = lineVertexData[1];

        lineVertexData[8] = lineVertexData[0] + (float) lineWidth;
        lineVertexData[9] = lineVertexData[1];

        float deltaHeight = 0.0f;

        if (links.length > 0) {
            deltaHeight = (float) (normSpriteHeight - 2.0 * normSpriteMargin) / ((float)(links.length));
        }

        lineVertexData[1] = lineVertexData[1] + 0.5f * deltaHeight;
        lineVertexData[5] = lineVertexData[5] + 0.5f * deltaHeight;
        lineVertexData[9] = lineVertexData[9] + 0.5f * deltaHeight;

        /* Bar vertex data: lower-left, lower-right, upper-left and upper-right corners */
        float[] barVertexData = new float[] {0.25f, 0.75f, Z_FRONT, 1.0f,
                                             0.75f, 0.75f, Z_FRONT, 1.0f,
                                             0.25f, 1.00f, Z_FRONT, 1.0f,
                                             0.75f, 1.00f, Z_FRONT, 1.0f
                                            };

        float barHeight = BAR_HEIGHT * deltaHeight;

        barVertexData[0] = (float) (legendCorner[0] + xOffset);
        barVertexData[1] = (float) (legendCorner[1] + normSpriteMargin + yOffset) + 0.5f * (deltaHeight - barHeight);

        barVertexData[4] = barVertexData[0] + (float) lineWidth;
        barVertexData[5] = barVertexData[1];

        barVertexData[8] = barVertexData[0];
        barVertexData[9] = barVertexData[1] + barHeight;

        barVertexData[12] = barVertexData[4];
        barVertexData[13] = barVertexData[9];

        for (Integer link : links) {
            Polyline currentLine = (Polyline) GraphicController.getController().getObjectFromId(link);

            drawLegendItem(legend, drawingTools, colorMap, currentLine, barVertexData, lineVertexData);

            /* Update the vertex data's vertical position */
            lineVertexData[1] += deltaHeight;
            lineVertexData[5] += deltaHeight;
            lineVertexData[9] += deltaHeight;

            barVertexData[1] += deltaHeight;
            barVertexData[5] += deltaHeight;
            barVertexData[9] += deltaHeight;
            barVertexData[13] += deltaHeight;
        }

        /* Legend text */
        float[] spritePosition = new float[] {lineVertexData[8] + (float) xOffset, (float) (legendCorner[1] + yOffset), Z_FRONT};

        /* Draw the sprite only if there are valid links */
        if (nbValidLinks > 0) {
            drawingTools.draw(legendSprite, AnchorPosition.LOWER_LEFT, new Vector3d(spritePosition));
        }

        /* Restore the transformation stacks */
        modelViewStack.pop();
        projectionStack.pop();

        /* Output the position if required */
        Double[] legendPosition = new Double[2];

        if (axesDims[0] == 0.0) {
            axesDims[0] = 1.0;
        }

        if (axesDims[1] == 0.0) {
            axesDims[1] = 1.0;
        }

        legendPosition[0] = (legendCorner[0] - axesPos[0]) / axesDims[0];
        legendPosition[1] = 1.0 - (legendCorner[1] + legendDims[1] - axesPos[1]) / axesDims[1];

        if (legendLocation != LegendLocation.BY_COORDINATES) {
            legend.setPosition(legendPosition);
        }
    }

    /**
     * Draw the legend item corresponding to the given polyline.
     * It draws either a horizontal line or bar depending on the polyline's properties (style, fill and line modes).
     * @param legend the Legend.
     * @param drawingTools the DrawingTools {@see DrawingTools} used to draw the Legend.
     * @param colorMap the colorMap used.
     * @param polyline the given polyline.
     * @param barVertexData a bar's vertex data (4 consecutive (x,y,z,w) quadruplets: lower-left, lower-right, upper-left and upper-right corners.
     * @param lineVertexData a line's vertex data (3 consecutive (x,y,z,w) quadruplets: left, middle and right vertices).
     * @throws org.scilab.forge.scirenderer.SciRendererException if the draw fail.
     */
    private void drawLegendItem(Legend legend, DrawingTools drawingTools, ColorMap colorMap, Polyline polyline, float[] barVertexData, float[] lineVertexData) throws SciRendererException {
        int polylineStyle = polyline.getPolylineStyle();

        int lineColor = polyline.getLineColor();
        double lineThickness = polyline.getLineThickness();
        short linePattern = polyline.getLineStyleAsEnum().asPattern();

        boolean isBar = (polylineStyle == 6) || (polylineStyle == 7);
        boolean barDrawn = isBar || polyline.getFillMode();

        /* Draw a bar if the curve is a bar or if it is filled */
        if (barDrawn) {
            barVertices.setData(barVertexData, 4);

            DefaultGeometry bar = new DefaultGeometry();
            bar.setFillDrawingMode(Geometry.FillDrawingMode.TRIANGLES);

            Appearance barAppearance = new Appearance();
            barAppearance.setFillColor(ColorFactory.createColor(colorMap, polyline.getBackground()));
            bar.setVertices(barVertices);
            bar.setIndices(rectangleIndices);

            /* Bar outline */
            if (isBar || polyline.getLineMode()) {
                bar.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
                bar.setWireIndices(rectangleOutlineIndices);

                barAppearance.setLineColor(ColorFactory.createColor(colorMap, polyline.getLineColor()));
                barAppearance.setLineWidth((float) lineThickness);
                barAppearance.setLinePattern(linePattern);
            } else {
                bar.setLineDrawingMode(Geometry.LineDrawingMode.NONE);
            }

            drawingTools.draw(bar, barAppearance);
        }

        /* Draw a line otherwise */
        if (!barDrawn) {
            lineVertices.setData(lineVertexData, 4);

            /* A line must also be drawn for the vertical polyline style (3), whatever line mode's value */
            if (polyline.getLineMode() || polylineStyle == 3) {
                DefaultGeometry line = new DefaultGeometry();
                line.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
                line.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS_STRIP);
                line.setVertices(lineVertices);
                Appearance lineAppearance = new Appearance();
                lineAppearance.setLineColor(ColorFactory.createColor(colorMap, lineColor));
                lineAppearance.setLineWidth((float) lineThickness);
                lineAppearance.setLinePattern(linePattern);

                drawingTools.draw(line, lineAppearance);
            }
        }

        /* Draw arrows */
        if (polylineStyle == 4) {
            if (barDrawn) {
                /*
                 * Overlap can occur between the arrow heads of the bar's two smallest segments and the adjacent items.
                 * To do: adjust arrow size to correct this.
                 */
                visitor.getArrowDrawer().drawArrows(polyline.getParentAxes(), barVertices, rectangleOutlineIndices,
                                                    polyline.getArrowSizeFactor(), lineThickness, lineColor, false);
            } else {
                visitor.getArrowDrawer().drawArrows(polyline.getParentAxes(), lineVertices, lineIndices,
                                                    polyline.getArrowSizeFactor(), lineThickness, lineColor, false);
            }
        }

        if (polyline.getMarkMode()) {
            Appearance lineAppearance = new Appearance();
            lineAppearance.setLineWidth((float) lineThickness);
            Texture markTexture = markManager.getMarkSprite(polyline, colorMap, lineAppearance);
            if (barDrawn) {
                drawingTools.draw(markTexture, AnchorPosition.CENTER, barVertices);
            } else {
                int marksCount = legend.getMarksCount();
                float[] data;
                if (marksCount != 0) {
                    switch (marksCount) {
                        case 1:
                            data = new float[] {lineVertexData[4], lineVertexData[5], lineVertexData[6], lineVertexData[7]};
                            break;
                        case 2:
                            data = new float[] {lineVertexData[0], lineVertexData[1], lineVertexData[2], lineVertexData[3],
                                                lineVertexData[8], lineVertexData[9], lineVertexData[10], lineVertexData[11]
                                           };
                            break;
                        default:
                            data = new float[] {lineVertexData[0], lineVertexData[1], lineVertexData[2], lineVertexData[3],
                                                lineVertexData[4], lineVertexData[5], lineVertexData[6], lineVertexData[7],
                                                lineVertexData[8], lineVertexData[9], lineVertexData[10], lineVertexData[11]
                                           };
                            break;
                    }
                    lineVertices.setData(data, 4);
                    drawingTools.draw(markTexture, AnchorPosition.CENTER, lineVertices);
                }
            }
        }
    }

    /**
     * Updates the legend by disposing its sprite.
     * @param id the legend id.
     * @param property the property to update.
     */
    public void update(Integer id, int property) {
        if (textureMap.containsKey(id)) {
            if (SPRITE_PROPERTIES.contains(property)) {
                dispose(id);
            }
        }
    }

    /**
     * Disposes the Legend sprite corresponding to the given id.
     * @param id the legend id.
     */
    public void dispose(Integer id) {
        Texture texture = textureMap.get(id);
        if (texture != null) {
            textureManager.dispose(texture);
            textureMap.remove(id);
        }
    }

    /**
     * Disposes all the Legend resources.
     */
    public void disposeAll() {
        visitor.getCanvas().getBuffersManager().dispose(rectangleVertices);
        visitor.getCanvas().getBuffersManager().dispose(lineVertices);
        visitor.getCanvas().getBuffersManager().dispose(barVertices);
        visitor.getCanvas().getBuffersManager().dispose(lineIndices);
        visitor.getCanvas().getBuffersManager().dispose(rectangleIndices);
        visitor.getCanvas().getBuffersManager().dispose(rectangleOutlineIndices);

        textureManager.dispose(textureMap.values());
        textureMap.clear();
    }

    /**
     * Returns the legend text texture.
     * @param colorMap the color map.
     * @param legend the Legend.
     * @return the text sprite.
     */
    private Texture getTexture(ColorMap colorMap, Legend legend) {
        Texture texture = textureMap.get(legend.getIdentifier());
        if (texture == null) {
            this.legendSpriteDrawer = new LegendSpriteDrawer(colorMap, legend);
            texture = textureManager.createTexture();
            texture.setDrawer(legendSpriteDrawer);
            textureMap.put(legend.getIdentifier(), texture);
        }
        return texture;
    }

    /**
     * Determines and returns the number of valid links for the given Legend object.
     * @param legend the given Legend.
     * @return the number of valid links.
     */
    private int getNumberValidLinks(Legend legend) {
        int nbValidLinks = 0;
        Integer[] links = legend.getLinks();

        for (Integer link : links) {
            Polyline currentLine = (Polyline) GraphicController.getController().getObjectFromId(link);

            if (currentLine != null) {
                nbValidLinks++;
            }
        }

        return nbValidLinks;
    }

    /**
     * Updates the links and text properties of the Legend depending
     * on the number of valid links provided (the number of links
     * to non-null objects).
     * To do: use the graphic controller to perform the update;
     * move the link update from LegendDrawer to somewhere more appropriate.
     * @param legend the Legend to update.
     * @param nbValidLinks the number of valid links.
     */
    private void updateLinks(Legend legend, int nbValidLinks) {
        int i1 = 0;
        ArrayList <Integer> newLinks = new ArrayList<Integer>(legend.getLinks().length);
        String[] newStrings;
        Integer[] newDims = new Integer[2];

        /*
         * In case there are no valid links, we create a single empty String
         * in order to retain the Legend's font properties.
         */
        if (nbValidLinks == 0) {
            newDims[0] = 1;
        } else {
            newDims[0] = nbValidLinks;
        }

        newDims[1] = 1;

        newStrings = new String[newDims[0]];

        if (nbValidLinks == 0) {
            newStrings[0] = new String("");
        }

        Integer[] links = legend.getLinks();
        String[] strings = legend.getTextStrings();

        for (int i = 0; i < links.length; i++) {
            Polyline currentLine = (Polyline) GraphicController.getController().getObjectFromId(links[i]);

            /* Text strings are stored in reverse order relative to links. */
            if (currentLine != null) {
                newLinks.add(links[i]);

                newStrings[nbValidLinks - i1 - 1] = new String(strings[strings.length - i - 1]);
                i1++;
            }
        }

        /* Update the legend's links and text */
        legend.setLinks(newLinks);

        legend.setTextArrayDimensions(newDims);
        legend.setTextStrings(newStrings);
    }
}
