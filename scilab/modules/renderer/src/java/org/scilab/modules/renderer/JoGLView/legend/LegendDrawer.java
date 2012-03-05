/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011-2012 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.legend;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.buffers.IndicesBuffer;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.sprite.Sprite;
import org.scilab.forge.scirenderer.sprite.SpriteAnchorPosition;
import org.scilab.forge.scirenderer.sprite.SpriteManager;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.TransformationStack;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.polyline.Polyline;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.legend.Legend;
import org.scilab.modules.graphic_objects.legend.Legend.LegendLocation;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.renderer.JoGLView.mark.MarkSpriteManager;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;

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
 * - implement arrow rendering (polyline_style == 4)
 * - modify bounds depending on the location (for OUT_* values)
 * - take into account the actual tick/label size (instead of an arbitrary value)
 * - correct clipping (works incorrectly when subplots are used)
 * - implement box clipping mode
 *
 * @author Manuel JULIACHS
 */
public class LegendDrawer {

        /**
         * Set of properties that affect the legend sprite
         */
        private static final Set<String> SPRITE_PROPERTIES = new HashSet<String>(Arrays.asList(
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
        private final SpriteManager spriteManager;

        /** The MarkSpriteManager used */
        private final MarkSpriteManager markManager;

        /** The Legend sprite drawer */
        private LegendSpriteDrawer legendSpriteDrawer;

        /** The relative line width */
        private static final double LINE_WIDTH = 0.1;

        /** The relative y-offset */
        private static final double Y_OFFSET = 0.01;

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

        /** The indices used to draw a rectangle */
        private IndicesBuffer rectangleIndices;

        /** The indices used to draw the outline of a rectangle */
        private IndicesBuffer rectangleOutlineIndices;

        /** The map storing legend text sprites */
        private Map<String, Sprite> textSpriteMap;

        /**
         * Constructor.
         * @param visitor the DrawerVisitor {@see DrawerVisitor}.
         * @param spriteManager the SpriteManager {@see SpriteManager}.
         * @param markManager the MarkSpriteManager {@see MarkSpriteManager}.
         */
        public LegendDrawer(DrawerVisitor visitor, SpriteManager spriteManager, MarkSpriteManager markManager) {
                this.visitor = visitor;
                this.spriteManager = spriteManager;
                this.markManager = markManager;

                rectangleVertices = visitor.getCanvas().getBuffersManager().createElementsBuffer();
                lineVertices = visitor.getCanvas().getBuffersManager().createElementsBuffer();
                barVertices = visitor.getCanvas().getBuffersManager().createElementsBuffer();
                rectangleIndices = visitor.getCanvas().getBuffersManager().createIndicesBuffer();
                rectangleOutlineIndices = visitor.getCanvas().getBuffersManager().createIndicesBuffer();

                textSpriteMap = new ConcurrentHashMap<String, Sprite>();
        }

        /**
         * Draws the given Legend.
         * @param legend the Legend to draw.
         */
        public void draw(Legend legend) throws SciRendererException {
                /* The coordinates of the legend box's lower-left corner */
                double [] legendCorner = new double[]{0.25, 0.75, Z_FRONT};

                DrawingTools drawingTools = visitor.getDrawingTools();
                ColorMap colorMap = visitor.getColorMap();
                Canvas canvas = visitor.getCanvas();

                String [] links = legend.getLinks();

                /*
                 * Set the projection and modelview transformations so that coordinates
                 * are specified in the {0,+1,0,+1,0,+1} space then disable clipping
                 */
                TransformationStack modelViewStack = drawingTools.getTransformationManager().getModelViewStack();
                TransformationStack projectionStack = drawingTools.getTransformationManager().getProjectionStack();

                Transformation identity = TransformationFactory.getIdentity();
                modelViewStack.push(identity);

                Transformation orthoProj = TransformationFactory.getOrthographic(0.0, 1.0, 0.0, 1.0, -1.0, 0.0);
                projectionStack.push(orthoProj);

                drawingTools.getClippingManager().disableClipping();


                /* First, compute the legend box's position and dimensions from the Axes' parameters and the text sprite's dimensions */

                String parentAxesID = legend.getParentAxes();
                Axes parentAxes = (Axes) GraphicController.getController().getObjectFromId(parentAxesID);

                Double [] axesBounds = parentAxes.getAxesBounds();
                Double [] margins = parentAxes.getMargins();

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

                Sprite legendSprite = getSprite(colorMap, legend);

                double normSpriteWidth = (double) legendSprite.getWidth() / (double) canvasWidth;
                double normSpriteHeight = (double) legendSprite.getHeight() / (double) canvasHeight;

                double lineWidth;

                /* The legend box's width and height */
                double [] legendDims = new double[2];

                double [] axesPos = new double[2];
                double [] axesDims = new double[2];

                lineWidth = LINE_WIDTH * (axesBounds[2])*(1.0-margins[0]-margins[1]);

                double xOffset = lineWidth/8.0;
                double yOffset = (Y_OFFSET * (axesBounds[3])*(1.0-margins[2]-margins[3]));

                legendDims[0] = normSpriteWidth + lineWidth + 3.0*xOffset;
                legendDims[1] = normSpriteHeight + 2.0*yOffset;

                axesPos[0] = axesBounds[0];
                axesPos[1] = 1.0 - (axesBounds[1]+axesBounds[3]);

                axesDims[0] = axesBounds[2];
                axesDims[1] = axesBounds[3];

                /* The {x, y} coordinates of the axes box's lower-left and upper-right corners (as defined by bounds and margins) */
                double [] llBoxCorner = new double[2];
                double [] urBoxCorner = new double[2];

                LegendLocation legendLocation = legend.getLegendLocationAsEnum();

                llBoxCorner[0] = axesPos[0] + margins[0]*axesDims[0];
                llBoxCorner[1] = axesPos[1] + margins[3]*axesDims[1];

                urBoxCorner[0] = axesPos[0] + (1.0 - margins[1])*axesDims[0];
                urBoxCorner[1] = axesPos[1] + (1.0 - margins[2])*axesDims[1];

                if (legendLocation == LegendLocation.IN_UPPER_RIGHT) {
                        legendCorner[0] = (float) (urBoxCorner[0] - xOffset - legendDims[0]);
                        legendCorner[1] = (float) (urBoxCorner[1] - yOffset - legendDims[1]);
                } else if (legendLocation == LegendLocation.IN_UPPER_LEFT) {
                        legendCorner[0] = (float) (llBoxCorner[0] + xOffset);
                        legendCorner[1] = (float) (urBoxCorner[1] - yOffset - legendDims[1]);
                } else if (legendLocation == LegendLocation.IN_LOWER_RIGHT) {
                        legendCorner[0] = (float) (urBoxCorner[0] - xOffset - legendDims[0]);
                        legendCorner[1] = (float) (llBoxCorner[1] + yOffset);
                } else if (legendLocation == LegendLocation.IN_LOWER_LEFT) {
                        legendCorner[0] = (float) (llBoxCorner[0] + xOffset);
                        legendCorner[1] = (float) (llBoxCorner[1] + yOffset);
                } else if (legendLocation == LegendLocation.OUT_UPPER_RIGHT) {
                        legendCorner[0] = (float) (urBoxCorner[0] + xOffset);
                        legendCorner[1] = (float) (urBoxCorner[1] - legendDims[1]);
                } else if (legendLocation == LegendLocation.OUT_UPPER_LEFT) {
                        legendCorner[0] = (float) (llBoxCorner[0] - xOffset - legendDims[0]);
                        legendCorner[1] = (float) (urBoxCorner[1] - legendDims[1]);
                } else if (legendLocation == LegendLocation.OUT_LOWER_RIGHT) {
                        legendCorner[0] = (float) (urBoxCorner[0] + xOffset);
                        legendCorner[1] = (float) (llBoxCorner[1]);
                } else if (legendLocation == LegendLocation.OUT_LOWER_LEFT) {
                        legendCorner[0] = (float) (llBoxCorner[0] - xOffset - legendDims[0]);
                        legendCorner[1] = (float) (llBoxCorner[1]);
                } else if (legendLocation == LegendLocation.UPPER_CAPTION) {
                        legendCorner[0] = (float) (llBoxCorner[0]);
                        legendCorner[1] = (float) (urBoxCorner[1] + yOffset);

                        /* x-axis at the top */
                        if (xAxisLocation == 1) {
                                /* To do: use the actual label+tick bounding box height */
                                legendCorner[1] += TICK_LABEL_SIZE;
                        }
                } else if (legendLocation == LegendLocation.LOWER_CAPTION) {
                        legendCorner[0] = (float) (llBoxCorner[0]);
                        legendCorner[1] = (float) (llBoxCorner[1] - yOffset - legendDims[1]);

                        /* x-axis at the bottom */
                        if (xAxisLocation == 0) {
                                /* To do: use the actual label+tick bounding box height */
                                legendCorner[1] -= TICK_LABEL_SIZE;
                        }
                } else if (legendLocation == LegendLocation.BY_COORDINATES) {
                        Double [] legPos = legend.getPosition();

                        legendCorner[0] = (float) (axesPos[0] + legPos[0]*axesBounds[2]);
                        legendCorner[1] = (float) (axesPos[1] + (1.0-legPos[1])*axesBounds[3] - legendDims[1]);
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
                float [] rectangleVertexData = new float[] {
                    (float)legendCorner[0], (float)legendCorner[1], Z_FRONT, 1.0f,
                    (float)(legendCorner[0]+legendDims[0]), (float)legendCorner[1], Z_FRONT, 1.0f,
                    (float)legendCorner[0], (float)(legendCorner[1]+legendDims[1]), Z_FRONT, 1.0f,
                    (float)(legendCorner[0]+legendDims[0]), (float)(legendCorner[1]+legendDims[1]), Z_FRONT, 1.0f
                };

                /* The indices of a rectangle's triangles and a rectangle outline's segment loop */
                int[] rectangleIndexData = new int[] {0, 1, 3, 0, 3, 2};
                int[] rectangleOutlineIndexData = new int[] {0, 1, 3, 2};

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
                    legendRectangle.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS_LOOP);
                    legendRectangle.setWireIndices(rectangleOutlineIndices);

                    appearance.setLineColor(ColorFactory.createColor(colorMap, legend.getLineColor()));
                    appearance.setLineWidth(legend.getLineThickness().floatValue());
                    appearance.setLinePattern(legend.getLineStyleAsEnum().asPattern());
                } else {
                    legendRectangle.setLineDrawingMode(Geometry.LineDrawingMode.NONE);
                }

                drawingTools.draw(legendRectangle, appearance);

                /* Lines: 3 vertices each, left, middle, and right */
                float [] lineVertexData = new float[] {0.25f, 0.75f, Z_FRONT, 1.0f,
                                                 0.5f, 0.75f, Z_FRONT, 1.0f,
                                                 0.75f, 0.75f, Z_FRONT, 1.0f};

                double normSpriteMargin = (double) legendSpriteDrawer.getMargin() / (double) canvasHeight;

                lineVertexData[0] = (float) (legendCorner[0] + xOffset);
                lineVertexData[1] = (float) (legendCorner[1] + normSpriteMargin + yOffset);

                lineVertexData[8] = lineVertexData[0] + (float) lineWidth;
                lineVertexData[9] = lineVertexData[1];

                lineVertexData[4] = 0.5f*(lineVertexData[0] + lineVertexData[8]);
                lineVertexData[5] = lineVertexData[1];

                float deltaHeight = (float) (normSpriteHeight - 2.0 * normSpriteMargin) / ((float)(links.length));

                lineVertexData[1] = lineVertexData[1] + 0.5f*deltaHeight;
                lineVertexData[5] = lineVertexData[5] + 0.5f*deltaHeight;
                lineVertexData[9] = lineVertexData[9] + 0.5f*deltaHeight;

                /* Bar vertex data: lower-left, lower-right, upper-left and upper-right corners */
                float [] barVertexData = new float[] {0.25f, 0.75f, Z_FRONT, 1.0f,
                                                0.75f, 0.75f, Z_FRONT, 1.0f,
                                                0.25f, 1.00f, Z_FRONT, 1.0f,
                                                0.75f, 1.00f, Z_FRONT, 1.0f};

                float barHeight = BAR_HEIGHT*deltaHeight;

                barVertexData[0] = (float) (legendCorner[0] + xOffset);
                barVertexData[1] = (float) (legendCorner[1] + normSpriteMargin + yOffset) + 0.5f*(deltaHeight - barHeight);

                barVertexData[4] = barVertexData[0] + (float) lineWidth;
                barVertexData[5] = barVertexData[1];

                barVertexData[8] = barVertexData[0];
                barVertexData[9] = barVertexData[1] + barHeight;

                barVertexData[12] = barVertexData[4];
                barVertexData[13] = barVertexData[9];

                for (String link : links) {
                        Polyline currentLine = (Polyline) GraphicController.getController().getObjectFromId(link);

                        drawLegendItem(drawingTools, colorMap, currentLine, barVertexData, lineVertexData);

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
                float [] spritePosition = new float[]{lineVertexData[8] + (float) xOffset, (float) (legendCorner[1] + yOffset), Z_FRONT};

                drawingTools.draw(legendSprite, SpriteAnchorPosition.LOWER_LEFT, new Vector3d(spritePosition));

                /* Re-enable clipping and restore the transformation stacks */
                for (int i = 0; i < 6; i++) {
                    drawingTools.getClippingManager().getClippingPlane(i).setEnable(true);
                }

                modelViewStack.pop();
                projectionStack.pop();

                /* Output the position if required */
                Double [] legendPosition = new Double[2];

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
         * @param drawingTools the DrawingTools {@see DrawingTools} used to draw the Legend.
         * @param colorMap the colorMap used.
         * @param polyline the given polyline.
         * @param barVertexData a bar's vertex data (4 consecutive (x,y,z,w) quadruplets: lower-left, lower-right, upper-left and upper-right corners.
         * @param lineVertexData a line's vertex data (3 consecutive (x,y,z,w) quadruplets: left, middle and right vertices).
         * @throws org.scilab.forge.scirenderer.SciRendererException if the draw fail.
         */
        private void drawLegendItem(DrawingTools drawingTools, ColorMap colorMap, Polyline polyline, float[] barVertexData, float[] lineVertexData) throws SciRendererException {
            int polylineStyle = polyline.getPolylineStyle();

            int lineColor = polyline.getLineColor();
            double lineThickness = polyline.getLineThickness();
            short linePattern = polyline.getLineStyleAsEnum().asPattern();

            boolean isBar = (polylineStyle == 6) || (polylineStyle == 7);
            boolean barDrawn = isBar || polyline.getFillMode();

            /* Draw a bar if the curve is a bar or if is is filled */
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
                    bar.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS_LOOP);
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

                if (polyline.getLineMode()) {
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

            if (polyline.getMarkMode()) {
                Sprite markSprite = markManager.getMarkSprite(polyline, colorMap);

                if (barDrawn) {
                    drawingTools.draw(markSprite, SpriteAnchorPosition.CENTER, barVertices);
                } else {
                    drawingTools.draw(markSprite, SpriteAnchorPosition.CENTER, lineVertices);
                }
            }

        }

        /**
         * Updates the legend by disposing its sprite.
         * @param id the legend id.
         * @param property the property to update.
         */
        public void update(String id, String property) {
                if (textSpriteMap.containsKey(id)) {
                        if (SPRITE_PROPERTIES.contains(property)) {
                                dispose(id);
                        }
                }
        }

        /**
         * Disposes the Legend sprite corresponding to the given id.
         * @param id the legend id.
         */
        public void dispose(String id) {
                Sprite textSprite = textSpriteMap.get(id);

                if (textSprite != null) {
                        spriteManager.dispose(textSprite);
                        textSpriteMap.remove(id);
                }
        }

        /**
         * Disposes all the Legend resources.
         */
        public void disposeAll() {
                visitor.getCanvas().getBuffersManager().dispose(rectangleVertices);
                visitor.getCanvas().getBuffersManager().dispose(lineVertices);
                visitor.getCanvas().getBuffersManager().dispose(barVertices);
                visitor.getCanvas().getBuffersManager().dispose(rectangleIndices);
                visitor.getCanvas().getBuffersManager().dispose(rectangleOutlineIndices);

                spriteManager.dispose(textSpriteMap.values());
                textSpriteMap.clear();
        }

        /**
         * Returns the legend text sprite.
         * @param colorMap the color map.
         * @param legend the Legend.
         * @return the text sprite.
         */
        private Sprite getSprite(ColorMap colorMap, Legend legend) {
                Sprite textSprite = textSpriteMap.get(legend.getIdentifier());

                if (textSprite == null) {
                        this.legendSpriteDrawer = new LegendSpriteDrawer(this.spriteManager, colorMap, legend);
                        textSprite = spriteManager.createSprite(legendSpriteDrawer.getWidth(), legendSpriteDrawer.getHeight());
                        textSprite.setDrawer(legendSpriteDrawer);
                        textSpriteMap.put(legend.getIdentifier(), textSprite);
                }

                return textSprite;
        }
}
