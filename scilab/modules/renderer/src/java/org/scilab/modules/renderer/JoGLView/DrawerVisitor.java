/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView;

import com.sun.opengl.util.BufferUtil;
import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.Drawer;
import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.sprite.Sprite;
import org.scilab.forge.scirenderer.sprite.SpriteAnchorPosition;
import org.scilab.forge.scirenderer.texture.AbstractDataProvider;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureDataProvider;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.TransformationStack;
import org.scilab.modules.graphic_objects.ObjectRemovedException;
import org.scilab.modules.graphic_objects.arc.Arc;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axis.Axis;
import org.scilab.modules.graphic_objects.compound.Compound;
import org.scilab.modules.graphic_objects.fec.Fec;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;
import org.scilab.modules.graphic_objects.imageplot.Grayplot;
import org.scilab.modules.graphic_objects.imageplot.Matplot;
import org.scilab.modules.graphic_objects.label.Label;
import org.scilab.modules.graphic_objects.legend.Legend;
import org.scilab.modules.graphic_objects.polyline.Polyline;
import org.scilab.modules.graphic_objects.rectangle.Rectangle;
import org.scilab.modules.graphic_objects.surface.Fac3d;
import org.scilab.modules.graphic_objects.surface.Plot3d;
import org.scilab.modules.graphic_objects.textObject.Text;
import org.scilab.modules.graphic_objects.vectfield.Arrow;
import org.scilab.modules.graphic_objects.vectfield.Champ;
import org.scilab.modules.graphic_objects.vectfield.Segs;
import org.scilab.modules.renderer.JoGLView.arrowDrawing.ArrowDrawer;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;
import org.scilab.modules.renderer.JoGLView.contouredObject.ContouredObjectDrawer;
import org.scilab.modules.renderer.JoGLView.label.LabelManager;
import org.scilab.modules.renderer.JoGLView.legend.LegendDrawer;
import org.scilab.modules.renderer.JoGLView.mark.MarkSpriteManager;
import org.scilab.modules.renderer.JoGLView.text.TextManager;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;
import org.scilab.modules.renderer.utils.textRendering.FontManager;

import java.awt.Dimension;
import java.nio.Buffer;
import java.nio.FloatBuffer;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * @author Pierre Lando
 */
public class DrawerVisitor implements Visitor, Drawer, GraphicView {

    /**
     * Set of properties changed during a draw if auto-ticks is on for X axis.
     */
    private static final Set<String> X_AXIS_TICKS_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_X_AXIS_TICKS_LOCATIONS__,
            GraphicObjectProperties.__GO_X_AXIS_TICKS_LABELS__,
            GraphicObjectProperties.__GO_X_AXIS_SUBTICKS__
    ));

    /**
     * Set of properties changed during a draw if auto-ticks is on for Y axis.
     */
    private static final Set<String> Y_AXIS_TICKS_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_Y_AXIS_TICKS_LOCATIONS__,
            GraphicObjectProperties.__GO_Y_AXIS_TICKS_LABELS__,
            GraphicObjectProperties.__GO_Y_AXIS_SUBTICKS__
    ));

    /**
     * Set of properties changed during a draw if auto-ticks is on for Z axis.
     */
    private static final Set<String> Z_AXIS_TICKS_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_Z_AXIS_TICKS_LOCATIONS__,
            GraphicObjectProperties.__GO_Z_AXIS_TICKS_LABELS__,
            GraphicObjectProperties.__GO_Z_AXIS_SUBTICKS__
    ));



    private final Canvas canvas;
    private final Figure figure;

    private final ColorMapTextureDataProvider colorMapTextureDataProvider;

    private final TextureManager textureManager;
    private final MarkSpriteManager markManager;
    private final LabelManager labelManager;
    private final DataManager dataManager;
    private final TextManager textManager;

    private final ContouredObjectDrawer contouredObjectDrawer;
    private final LegendDrawer legendDrawer;
    private final AxesDrawer axesDrawer;
    private final AxisDrawer axisDrawer;
    private final ArrowDrawer arrowDrawer;
    private final FecDrawer fecDrawer;

    private DrawingTools drawingTools = null;
    private Texture colorMapTexture;
    private ColorMap colorMap;

    private Axes currentAxes;

    /**
     * The map between the existing Figures' identifiers and their corresponding Visitor.
     * Used to get access to the DrawerVisitor corresponding to a given Figure when the
     * renderer module is accessed from another thread than the AWT's.
     */
    private final static Map<String, DrawerVisitor> visitorMap = new HashMap<String, DrawerVisitor>();

    public DrawerVisitor(Canvas canvas, Figure figure) {
        GraphicController.getController().register(this);

        this.canvas = canvas;
        this.figure = figure;

        this.dataManager = new DataManager(canvas);
        this.textureManager = new TextureManager(this);
        this.markManager = new MarkSpriteManager(canvas.getSpriteManager());
        this.textManager = new TextManager(canvas.getSpriteManager());
        this.labelManager = new LabelManager(canvas.getSpriteManager());
        this.axesDrawer = new AxesDrawer(this);
        this.axisDrawer = new AxisDrawer(this);
        this.arrowDrawer = new ArrowDrawer(this);
        this.contouredObjectDrawer = new ContouredObjectDrawer(this, this.dataManager, this.markManager);
        this.legendDrawer = new LegendDrawer(this, canvas.getSpriteManager(), this.markManager);
        this.fecDrawer = new FecDrawer(this);
        this.colorMapTextureDataProvider = new ColorMapTextureDataProvider();

        /*
         * Forces font loading from the main thread. This is done because
         * if getSciFontManager (thus, font loading) is concurrently accessed from
         * 2 different threads (the AWT's and the main one), freezing may occur.
         */
        FontManager.getSciFontManager();

        visitorMap.put(figure.getIdentifier(), this);
    }

    public DrawingTools getDrawingTools() {
        return drawingTools;
    }

    public Canvas getCanvas() {
        return canvas;
    }

    /**
     * @return the DataManager
     */
    public DataManager getDataManager() {
        return dataManager;
    }

    /**
     * @return the TextManager
     */
    public TextManager getTextManager() {
        return textManager;
    }

    /**
     * @return the AxesDrawer
     */
    public AxesDrawer getAxesDrawer() {
        return axesDrawer;
    }

    /**
     * @return the ArrowDrawer
     */
    public ArrowDrawer getArrowDrawer() {
        return arrowDrawer;
    }

    public ColorMap getColorMap() {
        return colorMap;
    }

    /**
     * Returns the visitor corresponding to the Figure identifier.
     * @param figureId the figure identifier.
     * @return the visitor.
     */
    public static DrawerVisitor getVisitor(String figureId) {
        return visitorMap.get(figureId);
    }

    @Override
    public void draw(DrawingTools drawingTools) {
        //long start = System.currentTimeMillis();
        this.drawingTools = drawingTools;
        figure.accept(this);
        //long end = System.currentTimeMillis();
        //long delta = end - start;
        //System.out.println(delta + "ms | " + 1000.0 / delta + " FPS");
    }

    /**
     * Ask the given object to accept visitor.
     * @param childrenId array of object identifier.
     */
    public void askAcceptVisitor(String[] childrenId) {
        if (childrenId != null) {

            for (int i = childrenId.length - 1 ; i >= 0 ; --i) {
                GraphicObject child = GraphicController.getController().getObjectFromId(childrenId[i]);
                if (child != null) {
                    try {
                        child.accept(this);
                    } catch (Exception e) {
                        System.err.println("[DEBUG] Try to draw an already removed object");
                        System.err.println("[DEBUG] " + e);
                        System.err.println("[DEBUG] Skipped...");
                    }
                }
            }
        }
    }

    @Override
    public void visit(Axes axes) {
        if (axes.getVisible()) {
            try {
                currentAxes = axes;
                axesDrawer.draw(axes);
            } catch (Exception e) {
                System.err.println("A '" + axes.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }
    }

    @Override
    public void visit(Arc arc) {
        if (arc.getVisible()) {
            try {
                axesDrawer.enableClipping(currentAxes, arc.getClipProperty());
                contouredObjectDrawer.draw(arc);
                axesDrawer.disableClipping(arc.getClipProperty());
            } catch (Exception e) {
                System.err.println("A '" + arc.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }
    }

    @Override
    public void visit(Axis axis) {
        if (axis.getVisible()) {
            axesDrawer.enableClipping(currentAxes, axis.getClipProperty());
            axisDrawer.draw(axis);
            axesDrawer.disableClipping(axis.getClipProperty());
        }
    }

    @Override
    public void visit(Compound compound) {
        if (compound.getVisible()) {
            askAcceptVisitor(compound.getChildren());
        }
    }

    @Override
    public void visit(Fec fec) throws ObjectRemovedException {
        axesDrawer.enableClipping(currentAxes, fec.getClipProperty());
        fecDrawer.draw(fec);
        axesDrawer.disableClipping(fec.getClipProperty());
    }

    @Override
    public void visit(Figure figure) {
        if (figure.getVisible()) {

            /**
             * Set the current {@see ColorMap}.
             */
            colorMap = figure.getColorMap();

            drawingTools.clear(ColorFactory.createColor(colorMap, figure.getBackground()));
            drawingTools.clearDepthBuffer();
            
            if (figure.getImmediateDrawing()) {
            	askAcceptVisitor(figure.getChildren());
            }
        }
    }

    @Override
    public void visit(final Grayplot grayplot) {
        if (grayplot.getVisible()) {
            try {
                axesDrawer.enableClipping(currentAxes, grayplot.getClipProperty());
                DefaultGeometry triangles = new DefaultGeometry();
                triangles.setFillDrawingMode(Geometry.FillDrawingMode.TRIANGLES);
                triangles.setVertices(dataManager.getVertexBuffer(grayplot.getIdentifier()));
                triangles.setColors(dataManager.getColorBuffer(grayplot.getIdentifier()));
                triangles.setIndices(dataManager.getIndexBuffer(grayplot.getIdentifier()));
                triangles.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);
                Appearance trianglesAppearance = new Appearance();
                drawingTools.draw(triangles, trianglesAppearance);
                axesDrawer.disableClipping(grayplot.getClipProperty());
            } catch (Exception e) {
                System.err.println("A '" + grayplot.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }
    }

    @Override
    public void visit(final Matplot matplot) {
        if (matplot.getVisible()) {
            try {
                axesDrawer.enableClipping(currentAxes, matplot.getClipProperty());
                if ((currentAxes != null) && (currentAxes.getXAxisLogFlag() || currentAxes.getYAxisLogFlag())) {
                    DefaultGeometry geometry = new DefaultGeometry();
                    geometry.setFillDrawingMode(Geometry.FillDrawingMode.TRIANGLES);
                    geometry.setVertices(dataManager.getVertexBuffer(matplot.getIdentifier()));
                    geometry.setColors(dataManager.getColorBuffer(matplot.getIdentifier()));
                    geometry.setIndices(dataManager.getIndexBuffer(matplot.getIdentifier()));
                    geometry.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);
                    Appearance appearance = new Appearance();
                    drawingTools.draw(geometry, appearance);
                } else {
                    TransformationStack modelViewStack = drawingTools.getTransformationManager().getModelViewStack();
                    Transformation t = TransformationFactory.getTranslateTransformation(.5, .5, 0);
                    modelViewStack.pushRightMultiply(t);
                    drawingTools.draw(textureManager.getTexture(matplot.getIdentifier()));
                    modelViewStack.pop();
                }
                axesDrawer.disableClipping(matplot.getClipProperty());
            } catch (Exception e) {
                System.err.println("A '" + matplot.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }
    }

    @Override
    public void visit(Label label) {
        if (label.getVisible()) {
            labelManager.draw(drawingTools, colorMap, label, axesDrawer);
        }
    }

    @Override
    public void visit(Legend legend) {
        if (legend.getVisible()) {
            try {
                legendDrawer.draw(legend);
            } catch (Exception e) {
                System.err.println("A '" + legend.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }
    }

    @Override
    public void visit(final Polyline polyline) {
        if (polyline.getVisible()) {
            try {
                axesDrawer.enableClipping(currentAxes, polyline.getClipProperty());

                DefaultGeometry geometry = new DefaultGeometry();

                geometry.setVertices(dataManager.getVertexBuffer(polyline.getIdentifier()));
                geometry.setIndices(dataManager.getIndexBuffer(polyline.getIdentifier()));
                geometry.setWireIndices(dataManager.getWireIndexBuffer(polyline.getIdentifier()));

                geometry.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
                geometry.setFillDrawingMode(Geometry.FillDrawingMode.TRIANGLES);
                geometry.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);

                /* Interpolated color rendering is used only for basic polylines for now. */
                if (polyline.getInterpColorMode() && polyline.getPolylineStyle() == 1) {
                    geometry.setColors(dataManager.getColorBuffer(polyline.getIdentifier()));
                } else {
                    geometry.setColors(null);
                }

                Appearance appearance = new Appearance();
                appearance.setLineColor(ColorFactory.createColor(colorMap, polyline.getLineColor()));
                appearance.setLineWidth(polyline.getLineThickness().floatValue());
                appearance.setLinePattern(polyline.getLineStyleAsEnum().asPattern());

                if (!polyline.getInterpColorMode() || polyline.getPolylineStyle() != 1) {
                    appearance.setFillColor(ColorFactory.createColor(colorMap, polyline.getBackground()));
                }

                drawingTools.draw(geometry, appearance);

                if (polyline.getPolylineStyle() == 4) {
                    arrowDrawer.drawArrows(polyline.getParentAxes(), polyline.getIdentifier(), polyline.getArrowSizeFactor(),
                        polyline.getLineThickness(), false, false, polyline.getLineColor());
                }

                if (polyline.getMarkMode()) {
                    Sprite sprite = markManager.getMarkSprite(polyline, colorMap);
                    ElementsBuffer positions = dataManager.getVertexBuffer(polyline.getIdentifier());
                    drawingTools.draw(sprite, SpriteAnchorPosition.CENTER, positions);
                }

                axesDrawer.disableClipping(polyline.getClipProperty());
            } catch (Exception e) {
                System.err.println("A '" + polyline.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }
    }

    @Override
    public void visit(Rectangle rectangle) {
        if (rectangle.getVisible()) {
            try {
                axesDrawer.enableClipping(currentAxes, rectangle.getClipProperty());
                contouredObjectDrawer.draw(rectangle);
                axesDrawer.disableClipping(rectangle.getClipProperty());
            } catch (Exception e) {
                System.err.println("A '" + rectangle.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }
    }

    /*
     * To do:
     * -use common code for both the Fac3d and Plot3d visit methods
     *  as they are mostly similar.
     */
    @Override
    public void visit(final Fac3d fac3d) {
        if (fac3d.getVisible()) {
            try {
                axesDrawer.enableClipping(currentAxes, fac3d.getClipProperty());

                if (fac3d.getSurfaceMode()) {
                    DefaultGeometry geometry = new DefaultGeometry();
                    geometry.setVertices(dataManager.getVertexBuffer(fac3d.getIdentifier()));
                    geometry.setIndices(dataManager.getIndexBuffer(fac3d.getIdentifier()));

                    /* Front-facing triangles */
                    Appearance appearance = new Appearance();

                    if (fac3d.getColorMode() != 0) {
                        geometry.setFillDrawingMode(Geometry.FillDrawingMode.TRIANGLES);
                        /* Back-facing triangles */
                        if (fac3d.getHiddenColor() > 0) {
                            geometry.setFaceCullingMode(axesDrawer.getBackFaceCullingMode());
                            Appearance backTrianglesAppearance = new Appearance();
                            backTrianglesAppearance.setFillColor(ColorFactory.createColor(colorMap, fac3d.getHiddenColor()));
                            drawingTools.draw(geometry, backTrianglesAppearance);

                            // Now we will draw front face.
                            geometry.setFaceCullingMode(axesDrawer.getFrontFaceCullingMode());
                        } else {
                            geometry.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);
                        }

                        if (fac3d.getColorFlag() == 0) {
                            appearance.setFillColor(ColorFactory.createColor(colorMap, Math.abs(fac3d.getColorMode())));
                        } else if (fac3d.getColorFlag() > 0) {
                            geometry.setTextureCoordinates(dataManager.getTextureCoordinatesBuffer(fac3d.getIdentifier()));
                            appearance.setTexture(getColorMapTexture());
                        } else {
                            geometry.setColors(null);
                        }
                    } else {
                        geometry.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
                    }

                    if ((fac3d.getColorMode() >= 0) && (fac3d.getLineThickness() > 0.0)) {
                        geometry.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
                        geometry.setWireIndices(dataManager.getWireIndexBuffer(fac3d.getIdentifier()));

                        appearance.setLineColor(ColorFactory.createColor(colorMap, fac3d.getLineColor()));
                        appearance.setLineWidth(fac3d.getLineThickness().floatValue());
                    }

                    drawingTools.draw(geometry, appearance);
                }

                if (fac3d.getMarkMode()) {
                    Sprite sprite = markManager.getMarkSprite(fac3d, colorMap);
                    ElementsBuffer positions = dataManager.getVertexBuffer(fac3d.getIdentifier());
                    drawingTools.draw(sprite, SpriteAnchorPosition.CENTER, positions);
                }
                axesDrawer.disableClipping(fac3d.getClipProperty());
            } catch (Exception e) {
                System.err.println("A '" + fac3d.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }

    }

    @Override
    public void visit(final Plot3d plot3d) {
        if (plot3d.getVisible()) {
            try {
                axesDrawer.enableClipping(currentAxes, plot3d.getClipProperty());

                if (plot3d.getSurfaceMode()) {
                    DefaultGeometry geometry = new DefaultGeometry();
                    if (plot3d.getColorMode() != 0) {
                        geometry.setFillDrawingMode(Geometry.FillDrawingMode.TRIANGLES);
                    } else {
                        geometry.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
                    }
                    geometry.setVertices(dataManager.getVertexBuffer(plot3d.getIdentifier()));
                    geometry.setIndices(dataManager.getIndexBuffer(plot3d.getIdentifier()));
                    /* Back-facing triangles */
                    if (plot3d.getHiddenColor() > 0) {
                        geometry.setFaceCullingMode(axesDrawer.getBackFaceCullingMode());
                        Appearance backTrianglesAppearance = new Appearance();
                        backTrianglesAppearance.setFillColor(ColorFactory.createColor(colorMap, plot3d.getHiddenColor()));
                        drawingTools.draw(geometry, backTrianglesAppearance);
                    }

                    /* Front-facing triangles */
                    Appearance appearance = new Appearance();


                    if (plot3d.getColorFlag() == 1) {
                        geometry.setColors(dataManager.getColorBuffer(plot3d.getIdentifier()));
                    } else {
                        geometry.setColors(null);
                    }

                    if (plot3d.getHiddenColor() > 0) {
                        geometry.setFaceCullingMode(axesDrawer.getFrontFaceCullingMode());
                    } else {
                        geometry.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);
                    }

                    if (plot3d.getColorFlag() == 0) {
                        appearance.setFillColor(ColorFactory.createColor(colorMap, Math.abs(plot3d.getColorMode())));
                    }

                    if ((plot3d.getColorMode() >= 0) && (plot3d.getLineThickness() > 0.0)) {
                        geometry.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
                        geometry.setWireIndices(dataManager.getWireIndexBuffer(plot3d.getIdentifier()));

                        appearance.setLinePattern(plot3d.getLineStyleAsEnum().asPattern());
                        appearance.setLineColor(ColorFactory.createColor(colorMap, plot3d.getLineColor()));
                        appearance.setLineWidth(plot3d.getLineThickness().floatValue());
                    }

                    drawingTools.draw(geometry, appearance);
                }

                if (plot3d.getMarkMode()) {
                    Sprite sprite = markManager.getMarkSprite(plot3d, colorMap);
                    ElementsBuffer positions = dataManager.getVertexBuffer(plot3d.getIdentifier());
                    drawingTools.draw(sprite, SpriteAnchorPosition.CENTER, positions);
                }

                axesDrawer.disableClipping(plot3d.getClipProperty());

            } catch (Exception e) {
                System.err.println("A '" + plot3d.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }

    }

    @Override
    public void visit(Text text) {
        if (text.getVisible()) {
            try {
                axesDrawer.enableClipping(currentAxes, text.getClipProperty());
                textManager.draw(drawingTools, colorMap, text, axesDrawer);
                axesDrawer.disableClipping(text.getClipProperty());
            } catch (Exception e) {
                System.err.println("A '" + text.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }
    }

    @Override
    public void visit(Arrow arrow) {
        // TODO
        System.out.println("How can I draw an arrow ?");
    }

    @Override
    public void visit(final Champ champ) {
        if (champ.getVisible()) {
            try {
                axesDrawer.enableClipping(currentAxes, champ.getClipProperty());
                DefaultGeometry segments = new DefaultGeometry();
                segments.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
                segments.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
                segments.setVertices(dataManager.getVertexBuffer(champ.getIdentifier()));
                segments.setWireIndices(dataManager.getWireIndexBuffer(champ.getIdentifier()));
                segments.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);
                if (champ.getColored()) {
                    segments.setColors(dataManager.getColorBuffer(champ.getIdentifier()));
                } else {
                    segments.setColors(null);
                }

                if (champ.getLineMode()) {
                    Appearance segmentAppearance = new Appearance();

                    /* If not colored, all segments have the same color. */
                    if (champ.getColored()) {
                        segmentAppearance.setLineColor(null);
                    } else {
                        segmentAppearance.setLineColor(ColorFactory.createColor(colorMap, champ.getLineColor()));
                    }

                    segmentAppearance.setLineWidth(champ.getLineThickness().floatValue());
                    segmentAppearance.setLinePattern(champ.getLineStyleAsEnum().asPattern());
                    drawingTools.draw(segments, segmentAppearance);
                }

                /* Draw the arrows */
                if (champ.getArrowSize() != 0.0) {
                    arrowDrawer.drawArrows(champ.getParentAxes(), champ.getIdentifier(), champ.getArrowSize(), champ.getLineThickness(), false,
                        champ.getColored(), champ.getLineColor());
                }

                axesDrawer.disableClipping(champ.getClipProperty());
            } catch (Exception e) {
                System.err.println("A '" + champ.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }
    }

    @Override
    public void visit(final Segs segs) {
        if (segs.getVisible()) {
            try {
                axesDrawer.enableClipping(currentAxes, segs.getClipProperty());
                DefaultGeometry segments = new DefaultGeometry();
                segments.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
                segments.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
                segments.setVertices(dataManager.getVertexBuffer(segs.getIdentifier()));
                segments.setColors(dataManager.getColorBuffer(segs.getIdentifier()));
                segments.setWireIndices(dataManager.getWireIndexBuffer(segs.getIdentifier()));
                segments.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);

                if (segs.getLineMode()) {
                    Appearance segmentAppearance = new Appearance();
                    segmentAppearance.setLineColor(null);
                    segmentAppearance.setLineWidth(segs.getLineThickness().floatValue());
                    segmentAppearance.setLinePattern(segs.getLineStyleAsEnum().asPattern());
                    drawingTools.draw(segments, segmentAppearance);
                }

                /*
                 * Segs does not derive from ContouredObject but Arrow does, hence we have to get the former's first Arrow
                 * in order to obtain the latter's Mark (all arrows are supposed to have the same contour properties for now).
                 */
                if (segs.getMarkMode()) {
                    Sprite sprite = markManager.getMarkSprite(segs.getIdentifier(), segs.getArrows().get(0).getMark(), colorMap);
                    ElementsBuffer positions = dataManager.getVertexBuffer(segs.getIdentifier());
                    drawingTools.draw(sprite, SpriteAnchorPosition.CENTER, positions);
                }

                /* Draw the arrows */
                if (segs.getArrowSize() != 0.0) {
                    arrowDrawer.drawArrows(segs.getParentAxes(), segs.getIdentifier(), segs.getArrowSize(), segs.getLineThickness(), true,
                        true, segs.getLineColor());
                }

                axesDrawer.disableClipping(segs.getClipProperty());
            } catch (Exception e) {
                System.err.println("A '" + segs.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }
    }

    @Override
    public void updateObject(String id, String property) {
        try {
            if (needUpdate(id, property)) {
                if (GraphicObjectProperties.__GO_COLORMAP__.equals(property) && figure.getIdentifier().equals(id)) {
                    labelManager.disposeAll();
                    dataManager.disposeAllColorBuffers();
                    markManager.disposeAll();
                    textManager.disposeAll();
                    axesDrawer.disposeAll();
                    fecDrawer.updateAll();
                    colorMapTextureDataProvider.update();
                } else {
                    labelManager.update(id, property);
                    dataManager.update(id, property);
                    markManager.update(id, property);
                    textManager.update(id, property);
                    axesDrawer.update(id, property);
                    legendDrawer.update(id, property);
                    fecDrawer.update(id, property);
                }
                canvas.redraw();
            }
        } catch (ObjectRemovedException e) {
            // Object has been removed before draw : do nothing.
        }
    }

    /**
     * Check if the given changed property make the figure out of date.
     * @param id the object updated
     * @param property the changed property.
     * @return true id the given changed property make the figure out of date.
     */
    private boolean needUpdate(String id, String property) {
        GraphicObject object = GraphicController.getController().getObjectFromId(id);
        if ((property != null) && (object != null) && isFigureChild(id)) {
            if (object instanceof Axes) {
                Axes axes = (Axes) object;
                if (axes.getXAxisAutoTicks() && X_AXIS_TICKS_PROPERTIES.contains(property)) {
                    return false;
                }

                if (axes.getYAxisAutoTicks() && Y_AXIS_TICKS_PROPERTIES.contains(property)) {
                    return false;
                }

                if (axes.getZAxisAutoTicks() && Z_AXIS_TICKS_PROPERTIES.contains(property)) {
                    return false;
                }
            }
            return true;
        } else {
            return false;
        }
    }

    @Override
    public void createObject(String id) {
    }

    @Override
    public void deleteObject(String id) {
        dataManager.dispose(id);
        markManager.dispose(id);
        textManager.dispose(id);
        labelManager.dispose(id);
        axesDrawer.dispose(id);
        legendDrawer.dispose(id);
        fecDrawer.dispose(id);

        GraphicObject object = GraphicController.getController().getObjectFromId(id);
        if (object instanceof Figure && visitorMap.containsKey(id)) {
            visitorMap.remove(id);
        }

        canvas.redraw();
    }

    /**
     * Check if the given id correspond to a child of the current {@see Figure}.
     * @param id the given id.
     * @return true if the given id correspond to a child of the current {@see Figure}.
     */
    private boolean isFigureChild(String id) {
        String parentFigureID = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_PARENT_FIGURE__);
        return figure.getIdentifier().equals(parentFigureID);
    }

    public LabelManager getLabelManager() {
        return labelManager;
    }

    public Texture getColorMapTexture() {
        if (colorMapTexture == null) {
            colorMapTexture = canvas.getTextureManager().createTexture();
            colorMapTexture.setMagnificationFilter(Texture.Filter.NEAREST);
            colorMapTexture.setMinifyingFilter(Texture.Filter.NEAREST);
            colorMapTexture.setSWrappingMode(Texture.Wrap.CLAMP);
            colorMapTexture.setTWrappingMode(Texture.Wrap.CLAMP);
            colorMapTexture.setDataProvider(colorMapTextureDataProvider);
        }
        return colorMapTexture;
    }

    /**
     * Figure getter.
     * @return the figure this visitor draw.
     */
    Figure getFigure() {
        return figure;
    }

    private class ColorMapTextureDataProvider extends AbstractDataProvider<Texture> implements TextureDataProvider {
        float[] whiteColor = {1.0f, 1.0f, 1.0f, 1.0f};
        float[] blackColor = {0.0f, 0.0f, 0.0f, 1.0f};

        @Override
        public Dimension getTextureSize() {
            return new Dimension(colorMap.getSize() + 2, 1);
        }

        @Override
        public Buffer getData() {
            Double[] data = colorMap.getData();
            FloatBuffer buffer = BufferUtil.newFloatBuffer(4 * ((data.length / 3) + 2));

            /* White and black are written in the first and second positions */
            buffer.put(whiteColor);
            buffer.put(blackColor);

            for (int i = 0 ; i < data.length / 3 ; i++) {
                buffer.put(data[i].floatValue());
                buffer.put(data[i + colorMap.getSize()].floatValue());
                buffer.put(data[i + 2 * colorMap.getSize()].floatValue());
                buffer.put(1);
            }
            buffer.rewind();
            return buffer;
        }

        @Override
        public Buffer getSubData(int x, int y, int width, int height) {
            /*
             * For the moment, we presuppose that x and y are 0 and that
             * width is equal to the colormap's total size (with height == 1).
             * To be correctly implemented.
             */
            return getData();
        }

        @Override
        public boolean isValid() {
            return true;
        }

        public void update() {
            fireUpdate();
        }
    }
}
