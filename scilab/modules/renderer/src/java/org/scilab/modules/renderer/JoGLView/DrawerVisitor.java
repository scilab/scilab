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

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.Drawer;
import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLCanvas;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.AbstractTextureDataProvider;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.TransformationStack;
import org.scilab.forge.scirenderer.utils.shapes.geometry.CubeFactory;
import org.scilab.modules.graphic_objects.ObjectRemovedException;
import org.scilab.modules.graphic_objects.arc.Arc;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axes.Camera.ViewType;
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
import org.scilab.modules.renderer.JoGLView.interaction.InteractionManager;
import org.scilab.modules.renderer.JoGLView.label.LabelManager;
import org.scilab.modules.renderer.JoGLView.legend.LegendDrawer;
import org.scilab.modules.renderer.JoGLView.mark.MarkSpriteManager;
import org.scilab.modules.renderer.JoGLView.postRendering.PostRendered;
import org.scilab.modules.renderer.JoGLView.text.TextManager;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;
import org.scilab.modules.renderer.JoGLView.util.OutOfMemoryException;
import org.scilab.modules.renderer.utils.textRendering.FontManager;

import java.awt.Component;
import java.awt.Dimension;
import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.swing.SwingUtilities;

/**
 * @author Pierre Lando
 */
public class DrawerVisitor implements Visitor, Drawer, GraphicView {

    /** Set of properties changed during a draw if auto-ticks is on for X axis. */
    private static final Set<Integer> X_AXIS_TICKS_PROPERTIES = new HashSet<Integer>(Arrays.asList(
                                                                                       GraphicObjectProperties.__GO_X_AXIS_TICKS_LOCATIONS__,
                                                                                       GraphicObjectProperties.__GO_X_AXIS_TICKS_LABELS__,
                                                                                       GraphicObjectProperties.__GO_X_AXIS_SUBTICKS__
                                                                                       ));

    /** Set of properties changed during a draw if auto-ticks is on for Y axis. */
    private static final Set<Integer> Y_AXIS_TICKS_PROPERTIES = new HashSet<Integer>(Arrays.asList(
                                                                                       GraphicObjectProperties.__GO_Y_AXIS_TICKS_LOCATIONS__,
                                                                                       GraphicObjectProperties.__GO_Y_AXIS_TICKS_LABELS__,
                                                                                       GraphicObjectProperties.__GO_Y_AXIS_SUBTICKS__
                                                                                       ));

    /** Set of properties changed during a draw if auto-ticks is on for Z axis. */
    private static final Set<Integer> Z_AXIS_TICKS_PROPERTIES = new HashSet<Integer>(Arrays.asList(
                                                                                       GraphicObjectProperties.__GO_Z_AXIS_TICKS_LOCATIONS__,
                                                                                       GraphicObjectProperties.__GO_Z_AXIS_TICKS_LABELS__,
                                                                                       GraphicObjectProperties.__GO_Z_AXIS_SUBTICKS__
                                                                                       ));

    /** Set of figure properties for witch a change doesn't lead to a redraw */
    private static final Set<Integer> SILENT_FIGURE_PROPERTIES = new HashSet<Integer>(Arrays.asList(
                GraphicObjectProperties.__GO_ROTATION_TYPE__,
                GraphicObjectProperties.__GO_INFO_MESSAGE__,
                GraphicObjectProperties.__GO_FIGURE_NAME__,
                GraphicObjectProperties.__GO_AUTORESIZE__,
                GraphicObjectProperties.__GO_POSITION__,
                GraphicObjectProperties.__GO_SIZE__,
                GraphicObjectProperties.__GO_ID__
            ));

    private static final boolean DEBUG_MODE = false;

    private final Component component;
    private final Canvas canvas;
    private final Figure figure;
    private final InteractionManager interactionManager;

    private final ColorMapTextureDataProvider colorMapTextureDataProvider;

    private final ScilabTextureManager textureManager;
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

    private DrawingTools drawingTools;
    private Texture colorMapTexture;
    private ColorMap colorMap;

    private Axes currentAxes;

    /**
     * The map between the existing Figures' identifiers and their corresponding Visitor.
     * Used to get access to the DrawerVisitor corresponding to a given Figure when the
     * renderer module is accessed from another thread than the AWT's.
     */
    private final static Map<String, DrawerVisitor> visitorMap = new HashMap<String, DrawerVisitor>();
    private final List<PostRendered> postRenderedList = new LinkedList<PostRendered>();

    public DrawerVisitor(Component component, Canvas canvas, Figure figure) {
        GraphicController.getController().register(this);

        this.component = component;
        this.canvas = canvas;
        this.figure = figure;

        this.interactionManager = new InteractionManager(this);
        this.dataManager = new DataManager(canvas);
        this.textureManager = new ScilabTextureManager(this);
        this.markManager = new MarkSpriteManager(canvas.getTextureManager());
        this.textManager = new TextManager(canvas.getTextureManager());
        this.labelManager = new LabelManager(canvas.getTextureManager());
        this.axesDrawer = new AxesDrawer(this);
        this.axisDrawer = new AxisDrawer(this);
        this.arrowDrawer = new ArrowDrawer(this);
        this.contouredObjectDrawer = new ContouredObjectDrawer(this, this.dataManager, this.markManager);
        this.legendDrawer = new LegendDrawer(this);
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

    public static void changeVisitor(Figure figure, DrawerVisitor visitor) {
        if (visitor == null) {
            visitorMap.remove(figure.getIdentifier());
        } else {
            visitorMap.put(figure.getIdentifier(), visitor);
        }
    }

    public void setDrawingTools(DrawingTools drawingTools) {
        this.drawingTools = drawingTools;
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
     * Mark manager getter.
     * @return the mark manager.
     */
    public MarkSpriteManager getMarkManager() {
        return markManager;
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

    public void addPostRendering(PostRendered postRendered) {
        if (postRendered != null) {
            postRenderedList.add(postRendered);
        }
    }

    public void removePostRendering(PostRendered postRendered) {
        postRenderedList.remove(postRendered);
    }

    @Override
    public void draw(DrawingTools drawingTools) {
        this.drawingTools = drawingTools;
        figure.accept(this);

        for (PostRendered postRendered : postRenderedList) {
            try {
                postRendered.draw(drawingTools);
            } catch (SciRendererException e) {
                System.err.println("A 'PostRendered' is not drawable because: '" + e.getMessage() + "'");
            }
        }
        drawingTools.getTransformationManager().useSceneCoordinate();
    }

    /**
     * Ask the given object to accept visitor.
     * @param childrenId array of object identifier.
     */
    public void askAcceptVisitor(String[] childrenId) {
        if (childrenId != null) {

            for (int i = childrenId.length - 1; i >= 0; --i) {
                GraphicObject child = GraphicController.getController().getObjectFromId(childrenId[i]);
                if (child != null) {
                    try {
                        child.accept(this);
                    } catch (ObjectRemovedException e) {
                        if (DEBUG_MODE) {
                            System.err.println("[DEBUG] Try to draw an already removed object");
                            System.err.println("[DEBUG] " + e);
                            System.err.println("[DEBUG] Skipped...");
                        }
                    }
                }
            }
        }
    }

    /**
     * @return true if it is a 2D view
     */
    public boolean is2DView() {
        return currentAxes.getViewAsEnum() == ViewType.VIEW_2D;
    }

    @Override
    public void visit(Axes axes) {
        synchronized (axes) {
            if (axes.isValid() && axes.getVisible()) {
                try {
                    currentAxes = axes;
                    axesDrawer.draw(axes);
                } catch (SciRendererException e) {
                    invalidate(axes, e);
                }
            }
        }
    }

    @Override
    public void visit(Arc arc) {
        if (arc.isValid() && arc.getVisible()) {
            axesDrawer.enableClipping(currentAxes, arc.getClipProperty());
            try {
                contouredObjectDrawer.draw(arc, currentAxes.getViewAsEnum() == ViewType.VIEW_2D);
            } catch (OutOfMemoryException e) {
                invalidate(arc, e);
            } catch (SciRendererException e) {
                invalidate(arc, e);
            } catch (ObjectRemovedException e) {
                invalidate(arc, e);
            }
            axesDrawer.disableClipping(arc.getClipProperty());
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
        if (fec.isValid() && fec.getVisible()) {
            axesDrawer.enableClipping(currentAxes, fec.getClipProperty());
            try {
                fecDrawer.draw(fec);
            } catch (OutOfMemoryException e) {
                invalidate(fec, e);
            }
            axesDrawer.disableClipping(fec.getClipProperty());
        }
    }

    @Override
    public void visit(Figure figure) {
        synchronized (figure) {
            /** Set the current {@see ColorMap}. */
            colorMap = figure.getColorMap();

            drawingTools.clear(ColorFactory.createColor(colorMap, figure.getBackground()));
            drawingTools.clearDepthBuffer();
            if (figure.getVisible() && figure.getImmediateDrawing()) {
                askAcceptVisitor(figure.getChildren());
            }
        }
    }

    @Override
    public void visit(final Grayplot grayplot) {
        if (grayplot.isValid() && grayplot.getVisible()) {
            axesDrawer.enableClipping(currentAxes, grayplot.getClipProperty());
            try {
                DefaultGeometry triangles = new DefaultGeometry();
                triangles.setFillDrawingMode(Geometry.FillDrawingMode.TRIANGLES);
                triangles.setVertices(dataManager.getVertexBuffer(grayplot.getIdentifier()));
                triangles.setColors(dataManager.getColorBuffer(grayplot.getIdentifier()));
                triangles.setIndices(dataManager.getIndexBuffer(grayplot.getIdentifier()));
                triangles.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);
                Appearance trianglesAppearance = new Appearance();
                drawingTools.draw(triangles, trianglesAppearance);
            } catch (ObjectRemovedException e) {
                invalidate(grayplot, e);
            } catch (SciRendererException e) {
                invalidate(grayplot, e);
            } catch (OutOfMemoryException e) {
                invalidate(grayplot, e);
            }
            axesDrawer.disableClipping(grayplot.getClipProperty());
        }
    }

    @Override
    public void visit(final Matplot matplot) {
        if (matplot.isValid() && matplot.getVisible()) {
            axesDrawer.enableClipping(currentAxes, matplot.getClipProperty());
            try {
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
                    Double[] scale = matplot.getScale();
                    Double[] translate = matplot.getTranslate();
                    Transformation t = TransformationFactory.getTranslateTransformation(translate[0], translate[1], 0);
                    Transformation t2 = TransformationFactory.getScaleTransformation(scale[0], scale[1], 1);
                    modelViewStack.pushRightMultiply(t);
                    modelViewStack.pushRightMultiply(t2);
                    drawingTools.draw(textureManager.getTexture(matplot.getIdentifier()));
                    modelViewStack.pop();
                    modelViewStack.pop();
                }
            } catch (ObjectRemovedException e) {
                invalidate(matplot, e);
            } catch (SciRendererException e) {
                invalidate(matplot, e);
            } catch (OutOfMemoryException e) {
                invalidate(matplot, e);
            }
            axesDrawer.disableClipping(matplot.getClipProperty());
        }
    }

    @Override
    public void visit(Label label) {
        if (label.isValid() && label.getVisible()) {
            try {
                labelManager.draw(drawingTools, colorMap, label, axesDrawer);
            } catch (SciRendererException e) {
                invalidate(label, e);
            }
        }
    }

    @Override
    public void visit(Legend legend) {
        if (legend.isValid() && legend.getVisible()) {
            try {
                legendDrawer.draw(legend);
            } catch (SciRendererException e) {
                invalidate(legend, e);
            }
        }
    }

    @Override
    public void visit(final Polyline polyline) {
        synchronized (polyline) {
            if (polyline.isValid() && polyline.getVisible()) {
                axesDrawer.enableClipping(currentAxes, polyline.getClipProperty());
                try {
                    DefaultGeometry geometry = new DefaultGeometry();

                    geometry.setVertices(dataManager.getVertexBuffer(polyline.getIdentifier()));
                    geometry.setIndices(dataManager.getIndexBuffer(polyline.getIdentifier()));
                    geometry.setWireIndices(dataManager.getWireIndexBuffer(polyline.getIdentifier()));

                    final int style = polyline.getPolylineStyle();
                    if (style == 1 || style == 2 || style == 4 || style == 5) {
                        geometry.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS_STRIP);
                    } else {
                        geometry.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
                    }

                    geometry.setFillDrawingMode(Geometry.FillDrawingMode.TRIANGLES);
                    geometry.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);

                    geometry.setPolygonOffsetMode(currentAxes.getCamera().getView() == ViewType.VIEW_3D);

                    /* Interpolated color rendering is used only for basic polylines for now. */
                    Appearance appearance = new Appearance();

                    if (polyline.getInterpColorMode() && style == 1) {
                        geometry.setTextureCoordinates(dataManager.getTextureCoordinatesBuffer(polyline.getIdentifier()));
                        appearance.setTexture(getColorMapTexture());
                    } else {
                        geometry.setColors(null);
                    }

                    appearance.setLineColor(ColorFactory.createColor(colorMap, polyline.getLineColor()));
                    appearance.setLineWidth(polyline.getLineThickness().floatValue());
                    appearance.setLinePattern(polyline.getLineStyleAsEnum().asPattern());

                    if (!polyline.getInterpColorMode() || style != 1) {
                        int fillColor;

                        /*
                         * The line color is used as fill color for the filled patch polyline style
                         * whereas the background color is used for all the other styles.
                         */
                        if (style == 5) {
                            fillColor = polyline.getLineColor();
                        } else {
                            fillColor = polyline.getBackground();
                        }

                        appearance.setFillColor(ColorFactory.createColor(colorMap, fillColor));
                    }

                    drawingTools.draw(geometry, appearance);

                    if (style == 4) {
                        arrowDrawer.drawArrows(polyline.getParentAxes(), polyline.getIdentifier(), polyline.getArrowSizeFactor(),
                                               polyline.getLineThickness(), false, false, polyline.getLineColor(), true);
                    }

                    if (polyline.getMarkMode()) {
                        Texture sprite = markManager.getMarkSprite(polyline, colorMap);
                        ElementsBuffer positions = dataManager.getVertexBuffer(polyline.getIdentifier());
                        drawingTools.draw(sprite, AnchorPosition.CENTER, positions);
                    }
                } catch (ObjectRemovedException e) {
                    invalidate(polyline, e);
                } catch (OutOfMemoryException e) {
                    invalidate(polyline, e);
                } catch (SciRendererException e) {
                    invalidate(polyline, e);
                }
                axesDrawer.disableClipping(polyline.getClipProperty());
            }
        }
    }

    @Override
    public void visit(Rectangle rectangle) {
        if (rectangle.isValid() && rectangle.getVisible()) {
            axesDrawer.enableClipping(currentAxes, rectangle.getClipProperty());
            try {
                contouredObjectDrawer.draw(rectangle, currentAxes.getCamera().getView() == ViewType.VIEW_2D);
            } catch (ObjectRemovedException e) {
                invalidate(rectangle, e);
            } catch (OutOfMemoryException e) {
                invalidate(rectangle, e);
            } catch (SciRendererException e) {
                invalidate(rectangle, e);
            }
            axesDrawer.disableClipping(rectangle.getClipProperty());
        }
    }

    /*
     * To do:
     * -use common code for both the Fac3d and Plot3d visit methods
     *  as they are mostly similar.
     */
    @Override
    public void visit(final Fac3d fac3d) {
        if (fac3d.isValid() && fac3d.getVisible()) {
            axesDrawer.enableClipping(currentAxes, fac3d.getClipProperty());
            try {
                if (fac3d.getSurfaceMode()) {
                    DefaultGeometry geometry = new DefaultGeometry();
                    geometry.setVertices(dataManager.getVertexBuffer(fac3d.getIdentifier()));
                    geometry.setIndices(dataManager.getIndexBuffer(fac3d.getIdentifier()));

                    geometry.setPolygonOffsetMode(true);

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
                    Texture texture = markManager.getMarkSprite(fac3d, colorMap);
                    ElementsBuffer positions = dataManager.getVertexBuffer(fac3d.getIdentifier());
                    drawingTools.draw(texture, AnchorPosition.CENTER, positions);
                }
            } catch (ObjectRemovedException e) {
                invalidate(fac3d, e);
            } catch (OutOfMemoryException e) {
                invalidate(fac3d, e);
            } catch (SciRendererException e) {
                invalidate(fac3d, e);
            }
            axesDrawer.disableClipping(fac3d.getClipProperty());
        }

    }

    @Override
    public void visit(final Plot3d plot3d) {
        if (plot3d.isValid() && plot3d.getVisible()) {
            axesDrawer.enableClipping(currentAxes, plot3d.getClipProperty());
            try {
                if (plot3d.getSurfaceMode()) {
                    DefaultGeometry geometry = new DefaultGeometry();
                    if (plot3d.getColorMode() != 0) {
                        geometry.setFillDrawingMode(Geometry.FillDrawingMode.TRIANGLES);
                    } else {
                        geometry.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
                    }

                    geometry.setPolygonOffsetMode(true);

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
                    Texture texture = markManager.getMarkSprite(plot3d, colorMap);
                    ElementsBuffer positions = dataManager.getVertexBuffer(plot3d.getIdentifier());
                    drawingTools.draw(texture, AnchorPosition.CENTER, positions);
                }
            } catch (ObjectRemovedException e) {
                invalidate(plot3d, e);
            } catch (OutOfMemoryException e) {
                invalidate(plot3d, e);
            } catch (SciRendererException e) {
                invalidate(plot3d, e);
            }
            axesDrawer.disableClipping(plot3d.getClipProperty());
        }

    }

    @Override
    public void visit(Text text) {
        if (text.isValid() && text.getVisible()) {
            axesDrawer.enableClipping(currentAxes, text.getClipProperty());
            try {
                textManager.draw(drawingTools, colorMap, text);
            } catch (SciRendererException e) {
                invalidate(text, e);
            }
            axesDrawer.disableClipping(text.getClipProperty());
        }
    }

    @Override
    public void visit(Arrow arrow) {
        // TODO
        System.out.println("How can I draw an arrow ?");
    }

    @Override
    public void visit(final Champ champ) {
        if (champ.isValid() && champ.getVisible()) {
            axesDrawer.enableClipping(currentAxes, champ.getClipProperty());
            try {
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
                                           champ.getColored(), champ.getLineColor(), false);
                }
            } catch (OutOfMemoryException e) {
                invalidate(champ, e);
            } catch (ObjectRemovedException e) {
                invalidate(champ, e);
            } catch (SciRendererException e) {
                invalidate(champ, e);
            }
            axesDrawer.disableClipping(champ.getClipProperty());
        }
    }

    @Override
    public void visit(final Segs segs) {
        if (segs.isValid() && segs.getVisible() && segs.getArrows().size() != 0) {
            axesDrawer.enableClipping(currentAxes, segs.getClipProperty());
            try {
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
                    Texture texture = markManager.getMarkSprite(segs.getIdentifier(), segs.getArrows().get(0).getMark(), colorMap);
                    ElementsBuffer positions = dataManager.getVertexBuffer(segs.getIdentifier());
                    drawingTools.draw(texture, AnchorPosition.CENTER, positions);
                }

                /* Draw the arrows */
                if (segs.getArrowSize() != 0.0) {
                    arrowDrawer.drawArrows(segs.getParentAxes(), segs.getIdentifier(), segs.getArrowSize(), segs.getLineThickness(), true,
                                           true, segs.getLineColor(), false);
                }
            } catch (OutOfMemoryException e) {
                invalidate(segs, e);
            } catch (ObjectRemovedException e) {
                invalidate(segs, e);
            } catch (SciRendererException e) {
                invalidate(segs, e);
            }
            axesDrawer.disableClipping(segs.getClipProperty());
        }
    }

    @Override
    public void updateObject(String id, int property) {
        try {
            if (needUpdate(id, property)) {
                GraphicController.getController().setProperty(id, GraphicObjectProperties.__GO_VALID__, true);
                if (GraphicObjectProperties.__GO_COLORMAP__ == property && figure.getIdentifier().equals(id)) {
                    labelManager.disposeAll();
                    dataManager.disposeAllColorBuffers();
                    dataManager.disposeAllTextureCoordinatesBuffers();
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

                if (GraphicObjectProperties.__GO_ANTIALIASING__ == property) {
                    canvas.setAntiAliasingLevel(figure.getAntialiasing());
                }

                if (isImmediateDrawing(id)) {
                    canvas.redraw();
                }
            }

            if (GraphicObjectProperties.__GO_IMMEDIATE_DRAWING__ == property && !isImmediateDrawing(id)) {
                canvas.waitImage();
            }

        } catch (OutOfMemoryException e) {
            invalidate(GraphicController.getController().getObjectFromId(id), e);
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
    private boolean needUpdate(String id, int property) {
        GraphicObject object = GraphicController.getController().getObjectFromId(id);
        int objectType = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);
        if ((object != null) && isFigureChild(id)
            && objectType != GraphicObjectProperties.__GO_UICONTROL__
            && objectType !=GraphicObjectProperties.__GO_UIMENU__) {

            if (GraphicObjectProperties.__GO_VALID__ == property) {
                return false;
            }

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

            if (object instanceof Figure) {
                if (SILENT_FIGURE_PROPERTIES.contains(property)) {
                    return false;
                }
            }

            return true;
        } else {
            return false;
        }
    }

    private boolean isImmediateDrawing(String id) {
        String parentId = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_PARENT_FIGURE__);
        if (parentId == null) {
            return false;
        } else {
            Boolean b =  (Boolean) GraphicController.getController().getProperty(parentId, GraphicObjectProperties.__GO_IMMEDIATE_DRAWING__);
            return (b == null) ? false : b;
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
            GraphicController.getController().unregister(this);
            if (SwingUtilities.isEventDispatchThread()) {
                canvas.destroy();
            } else {
                try {
                    SwingUtilities.invokeAndWait(new Runnable() {
                            public void run() {
                                canvas.destroy();
                            }
                        });
                } catch (Exception e) { }
            }
        } else {
            if (isImmediateDrawing(id)) {
                canvas.redraw();
            }
        }
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

    /**
     * Invalidate the given graphic object and inform the user.
     * @param graphicObject the graphic object to invalidate
     * @param exception the cause of invalidation.
     */
    public void invalidate(GraphicObject graphicObject, Exception exception) {
        if (DEBUG_MODE) {
            System.err.println("The " + graphicObject.getType() + " \"" + graphicObject.getIdentifier()
                               + "\" has been invalidated: " + exception.getMessage());
            exception.printStackTrace();
        }
        GraphicController.getController().setProperty(graphicObject.getIdentifier(), GraphicObjectProperties.__GO_VALID__, false);
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
    public Figure getFigure() {
        return figure;
    }

    private Geometry cube;
    public Geometry getCube() {
        if (cube == null) {
            cube = CubeFactory.createCube(canvas);
        }
        return cube;
    }

    /**
     * Component getter.
     * @return return the attached component.
     */
    public Component getComponent() {
        return component;
    }

    /**
     * Interaction manager getter
     * @return the interaction manager.
     */
    public InteractionManager getInteractionManager() {
        return interactionManager;
    }

    private class ColorMapTextureDataProvider extends AbstractTextureDataProvider {
        byte[] whiteColor = {(byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF};
        byte[] blackColor = {0x00, 0x00, 0x00, (byte) 0xFF};

        @Override
        public Dimension getTextureSize() {
            return new Dimension(colorMap.getSize() + 2, 1);
        }

        @Override
        public ByteBuffer getData() {
            Double[] data = colorMap.getData();
            ByteBuffer buffer = ByteBuffer.allocate(4 * ((data.length / 3) + 2));

            /* White and black are written in the first and second positions */
            buffer.put(whiteColor);
            buffer.put(blackColor);

            for (int i = 0 ; i < data.length / 3 ; i++) {
                buffer.put(toByte(data[i]));
                buffer.put(toByte(data[i + colorMap.getSize()].floatValue()));
                buffer.put(toByte(data[i + 2 * colorMap.getSize()].floatValue()));
                buffer.put(toByte(1));
            }
            buffer.rewind();
            return buffer;
        }

        @Override
        public ByteBuffer getSubData(int x, int y, int width, int height) {
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
