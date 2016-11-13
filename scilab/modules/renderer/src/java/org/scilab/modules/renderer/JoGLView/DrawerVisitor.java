/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre Lando
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

package org.scilab.modules.renderer.JoGLView;

import java.awt.Component;
import java.awt.Dimension;
import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.swing.SwingUtilities;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.Drawer;
import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.buffers.BuffersManager;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.texture.AbstractTextureDataProvider;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.TransformationStack;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.forge.scirenderer.utils.shapes.geometry.CubeFactory;
import org.scilab.modules.graphic_objects.ObjectRemovedException;
import org.scilab.modules.graphic_objects.arc.Arc;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axes.AxesContainer;
import org.scilab.modules.graphic_objects.axes.Camera.ViewType;
import org.scilab.modules.graphic_objects.axis.Axis;
import org.scilab.modules.graphic_objects.compound.Compound;
import org.scilab.modules.graphic_objects.datatip.Datatip;
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
import org.scilab.modules.graphic_objects.uicontrol.frame.Frame;
import org.scilab.modules.graphic_objects.vectfield.Arrow;
import org.scilab.modules.graphic_objects.vectfield.Champ;
import org.scilab.modules.graphic_objects.vectfield.Segs;
import org.scilab.modules.renderer.JoGLView.arrowDrawing.ArrowDrawer;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;
import org.scilab.modules.renderer.JoGLView.contouredObject.ContouredObjectDrawer;
import org.scilab.modules.renderer.JoGLView.datatip.DatatipTextDrawer;
import org.scilab.modules.renderer.JoGLView.interaction.InteractionManager;
import org.scilab.modules.renderer.JoGLView.label.LabelManager;
import org.scilab.modules.renderer.JoGLView.legend.LegendDrawer;
import org.scilab.modules.renderer.JoGLView.mark.MarkSpriteManager;
import org.scilab.modules.renderer.JoGLView.postRendering.PostRendered;
import org.scilab.modules.renderer.JoGLView.text.TextManager;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;
import org.scilab.modules.renderer.JoGLView.util.LightingUtils;
import org.scilab.modules.renderer.JoGLView.util.OutOfMemoryException;

/**
 * @author Pierre Lando
 */
public class DrawerVisitor implements Visitor, Drawer, GraphicView {

    /** Set of properties changed during a draw if auto-ticks is on for X axis. */
    private static final Set<Integer> X_AXIS_TICKS_PROPERTIES = new HashSet<Integer>(Arrays.asList(
                GraphicObjectProperties.__GO_X_AXIS_TICKS_LOCATIONS__,
                GraphicObjectProperties.__GO_X_AXIS_TICKS_LABELS__
            ));

    /** Set of properties changed during a draw if auto-ticks is on for Y axis. */
    private static final Set<Integer> Y_AXIS_TICKS_PROPERTIES = new HashSet<Integer>(Arrays.asList(
                GraphicObjectProperties.__GO_Y_AXIS_TICKS_LOCATIONS__,
                GraphicObjectProperties.__GO_Y_AXIS_TICKS_LABELS__
            ));

    /** Set of properties changed during a draw if auto-ticks is on for Z axis. */
    private static final Set<Integer> Z_AXIS_TICKS_PROPERTIES = new HashSet<Integer>(Arrays.asList(
                GraphicObjectProperties.__GO_Z_AXIS_TICKS_LOCATIONS__,
                GraphicObjectProperties.__GO_Z_AXIS_TICKS_LABELS__
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
    private final AxesContainer figure;
    private final InteractionManager interactionManager;

    private final ColorMapTextureDataProvider colorMapTextureDataProvider;

    private final ScilabTextureManager textureManager;
    private final MarkSpriteManager markManager;
    private final LabelManager labelManager;
    private final DataManager dataManager;
    private final TextManager textManager;

    private final ContouredObjectDrawer contouredObjectDrawer;
    private final LegendDrawer legendDrawer;
    protected final AxesDrawer axesDrawer;
    private final AxisDrawer axisDrawer;
    private final ArrowDrawer arrowDrawer;
    private final FecDrawer fecDrawer;
    private final DatatipTextDrawer datatipTextDrawer;

    private DrawingTools drawingTools;
    private Texture colorMapTexture;
    private ColorMap colorMap;

    private Axes currentAxes;
    private Canvas canvas;

    /**
     * The map between the existing Figures' identifiers and their corresponding Visitor.
     * Used to get access to the DrawerVisitor corresponding to a given Figure when the
     * renderer module is accessed from another thread than the AWT's.
     */
    private final static Map<Integer, DrawerVisitor> visitorMap = new HashMap<Integer, DrawerVisitor>();
    private final List<PostRendered> postRenderedList = new LinkedList<PostRendered>();
    private final static Map<Integer, List<Integer>> openGLChildren = new HashMap<Integer, List<Integer>>();

    public static int[] getSize() {
        return new int[] {visitorMap.size(), openGLChildren.size()};
    }

    public DrawerVisitor(Component component, Canvas canvas, AxesContainer figure) {
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
        this.datatipTextDrawer = new DatatipTextDrawer(canvas);

        visitorMap.put(figure.getIdentifier(), this);
    }

    public static void changeVisitor(AxesContainer figure, DrawerVisitor visitor) {
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

    public void setCanvas(Canvas canvas) {
        this.canvas = canvas;
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
     * @return the LegendDrawer
     */
    public LegendDrawer getLegendDrawer() {
        return legendDrawer;
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

    public DatatipTextDrawer getDatatipTextDrawer() {
        return datatipTextDrawer;
    }

    /**
     * Returns the visitor corresponding to the Figure identifier.
     * @param figureId the figure identifier.
     * @return the visitor.
     */
    public static DrawerVisitor getVisitor(Integer figureId) {
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
        if (figure instanceof Figure) {
            visit((Figure) figure);
        } else {
            visit((Frame) figure);
        }

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
    public void askAcceptVisitor(Integer[] childrenId) {
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
                    axesDrawer.computeRulers(axes);
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
            axisDrawer.draw(currentAxes, axis);
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
            try {
                Dimension dims = getCanvas().getDimension();
                colorMap = figure.getColorMap();
                drawingTools.clear(ColorFactory.createColor(colorMap, figure.getBackground()));
                drawingTools.clearDepthBuffer();
                if (figure.isValid() && figure.getVisible() && figure.getImmediateDrawing() && dims.width > 1 && dims.height > 1) {
                    askAcceptVisitor(figure.getChildren());
                }
            } catch (Exception e) {
                System.err.println(e);
            }
        }
    }

    public void visit(Frame frame) {
        synchronized (frame) {
            /** Set the current {@see ColorMap}. */
            try {
                colorMap = frame.getColorMap();
                drawingTools.clear(ColorFactory.createColor(colorMap, frame.getBackground()));
                drawingTools.clearDepthBuffer();
                if (frame.isValid() && frame.getVisible()) {
                    DrawerVisitor visitor = visitorMap.get(frame.getIdentifier());
                    if (visitor != null) {
                        Dimension dims = visitor.getCanvas().getDimension();
                        visitor.setDrawingTools(drawingTools);
                        if (dims.width > 1 && dims.height > 1) {
                            visitor.askAcceptVisitor(frame.getChildren());
                        }
                    }
                }
            } catch (Exception e) {
                System.err.println(e);
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
                /*} catch (ObjectRemovedException e) {
                  invalidate(grayplot, e);
                  } catch (SciRendererException e) {
                  invalidate(grayplot, e);
                  } catch (OutOfMemoryException e) {
                  invalidate(grayplot, e);
                  }*/
            } catch (Exception e) {
                System.err.println(e);
                e.printStackTrace();
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
                    double[][] factors = currentAxes.getScaleTranslateFactors();
                    Double[] scale = matplot.getScale();
                    Double[] translate = matplot.getTranslate();

                    scale[0] *= factors[0][0];
                    scale[1] *= factors[0][1];

                    translate[0] = translate[0] * factors[0][0] + factors[1][0];
                    translate[1] = translate[1] * factors[0][1] + factors[1][1];

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
        if (label.isValid() && label.getVisible() && !label.isEmpty()) {
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
                        if (polyline.getColorSet()) {
                            ElementsBuffer colors = dataManager.getColorBuffer(polyline.getIdentifier());
                            geometry.setColors(colors);
                            appearance.setLineColor(null);
                        } else {
                            geometry.setColors(null);
                            appearance.setLineColor(ColorFactory.createColor(colorMap, polyline.getLineColor()));
                        }
                    }

                    Integer lineColor = polyline.getSelected() ? polyline.getSelectedColor() : polyline.getLineColor();
                    appearance.setLineColor(ColorFactory.createColor(colorMap, lineColor));
                    appearance.setLineWidth(polyline.getLineThickness().floatValue());
                    appearance.setLinePattern(polyline.getLineStyleAsEnum().asPattern());

                    if (!polyline.getInterpColorMode() || style != 1) {
                        int fillColor;

                        /*
                         * The line color is used as fill color for the filled patch polyline style
                         * whereas the background color is used for all the other styles.
                         */
                        if (style == 5) {
                            fillColor = lineColor;
                        } else {
                            fillColor = polyline.getBackground();
                        }

                        appearance.setFillColor(ColorFactory.createColor(colorMap, fillColor));
                    }

                    drawingTools.draw(geometry, appearance);

                    if (style == 4) {
                        arrowDrawer.drawArrows(polyline.getParentAxes(), polyline.getIdentifier(), polyline.getArrowSizeFactor(),
                                               polyline.getLineThickness(), false, false, lineColor, true);
                    }

                    if (polyline.getMarkMode()) {
                        ElementsBuffer positions = dataManager.getVertexBuffer(polyline.getIdentifier());
                        int offset = polyline.getMarkOffset();
                        int stride = polyline.getMarkStride();
                        if (polyline.getColorSet() && (polyline.getNumMarkForegrounds() > 0) || (polyline.getNumMarkBackgrounds() > 0)) {
                            ElementsBuffer colors = dataManager.getColorBuffer(polyline.getIdentifier());
                            Color auxColor;
                            if (polyline.getNumMarkBackgrounds() > 0) {
                                auxColor = ColorFactory.createColor(colorMap, polyline.getMark().getForeground());
                            } else {
                                auxColor = ColorFactory.createColor(colorMap, polyline.getMark().getBackground());
                            }
                            FloatBuffer data = positions.getData();
                            FloatBuffer colorData = colors.getData();
                            Integer[] sizes = polyline.getMarkSizes();
                            if ( (sizes.length > 0) && (data != null) && (colorData != null) && (positions.getSize() == sizes.length) && (colors.getSize() == sizes.length) ) {

                                Integer markSizeTmp = polyline.getMarkSize();

                                // markers with different sizes
                                data.rewind();
                                colorData.rewind();

                                stride = stride < 1 ? 1 : stride;
                                offset = offset < 0 ? 0 : offset;

                                int elementSize = positions.getElementsSize();
                                int mark = offset * elementSize;
                                int k = 0;

                                while (data.remaining() >= stride * elementSize) {

                                    // Be careful, do not use polyline.setMarkSize since this will destroy the sizes
                                    polyline.getMark().setSize(sizes[k++]);

                                    BuffersManager bufferManager = drawingTools.getCanvas().getBuffersManager();
                                    ElementsBuffer singlePosition = bufferManager.createElementsBuffer();
                                    ElementsBuffer singleColor = bufferManager.createElementsBuffer();

                                    float[] position = {0, 0, 0, 1};
                                    data.position(mark);
                                    data.get(position);

                                    float[] color = {0, 0, 0, 0};
                                    colorData.position(mark);
                                    colorData.get(color);

                                    mark += stride * elementSize;

                                    singlePosition.setData(position, elementSize);
                                    singleColor.setData(color, elementSize);

                                    Texture sprite = markManager.getMarkSprite(polyline, null, appearance);
                                    drawingTools.draw(sprite, AnchorPosition.CENTER, singlePosition, 0, 0, 0, auxColor, singleColor);

                                    bufferManager.dispose(singleColor);
                                    bufferManager.dispose(singlePosition);
                                }
                                // restore the size of the mark
                                // Be careful, do not use polyline.setMarkSize since this will destroy the sizes
                                polyline.getMark().setSize(markSizeTmp);
                            } else {
                                Texture sprite = markManager.getMarkSprite(polyline, null, appearance);
                                drawingTools.draw(sprite, AnchorPosition.CENTER, positions, offset, stride, 0, auxColor, colors);
                            }
                        } else {
                            FloatBuffer data = positions.getData();
                            Integer[] sizes = polyline.getMarkSizes();
                            if ( (sizes.length > 0) && (data != null) && (positions.getSize() == sizes.length) ) {

                                Integer markSizeTmp = polyline.getMarkSize();

                                // markers with different sizes
                                data.rewind();

                                stride = stride < 1 ? 1 : stride;
                                offset = offset < 0 ? 0 : offset;

                                int elementSize = positions.getElementsSize();
                                int mark = offset * elementSize;
                                int k = 0;

                                while (data.remaining() >= stride * elementSize) {

                                    // setting the size of the mark temporary
                                    polyline.getMark().setSize(sizes[k++]);

                                    BuffersManager bufferManager = drawingTools.getCanvas().getBuffersManager();
                                    ElementsBuffer singlePosition = bufferManager.createElementsBuffer();

                                    float[] position = {0, 0, 0, 1};
                                    data.position(mark);
                                    data.get(position);
                                    mark += stride * elementSize;
                                    singlePosition.setData(position, elementSize);

                                    Texture sprite = markManager.getMarkSprite(polyline, colorMap, appearance);
                                    drawingTools.draw(sprite, AnchorPosition.CENTER, singlePosition, 0, 0, 0, null, null);

                                    bufferManager.dispose(singlePosition);
                                }
                                // restore the size of the mark
                                polyline.getMark().setSize(markSizeTmp);
                            } else {
                                Texture sprite = markManager.getMarkSprite(polyline, colorMap, appearance);
                                drawingTools.draw(sprite, AnchorPosition.CENTER, positions, offset, stride, 0, null, null);
                            }
                        }
                    }
                } catch (ObjectRemovedException e) {
                    invalidate(polyline, e);
                } catch (OutOfMemoryException e) {
                    invalidate(polyline, e);
                } catch (SciRendererException e) {
                    invalidate(polyline, e);
                }
                axesDrawer.disableClipping(polyline.getClipProperty());
                askAcceptVisitor(polyline.getDatatips());
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
                    geometry.setNormals(dataManager.getNormalBuffer(fac3d.getIdentifier()));
                    geometry.setIndices(dataManager.getIndexBuffer(fac3d.getIdentifier()));

                    geometry.setPolygonOffsetMode(true);

                    /* Front-facing triangles */
                    Appearance appearance = new Appearance();
                    appearance.setMaterial(LightingUtils.getMaterial(fac3d.getMaterial()));
                    LightingUtils.setupLights(drawingTools.getLightManager(), currentAxes);

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
                        Integer lineColor = fac3d.getSelected() ? fac3d.getSelectedColor() : fac3d.getLineColor();
                        appearance.setLineColor(ColorFactory.createColor(colorMap, lineColor));
                        appearance.setLineWidth(fac3d.getLineThickness().floatValue());
                    }

                    drawingTools.draw(geometry, appearance);
                    LightingUtils.setLightingEnable(drawingTools.getLightManager(), false);
                }

                if (fac3d.getMarkMode()) {
                    Appearance appearance = null;
                    if (fac3d.getLineThickness() > 0.0) {
                        appearance = new Appearance();
                        appearance.setLineWidth(fac3d.getLineThickness().floatValue());
                    }

                    Texture texture = markManager.getMarkSprite(fac3d, colorMap, appearance);
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
                    geometry.setNormals(dataManager.getNormalBuffer(plot3d.getIdentifier()));
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
                    appearance.setMaterial(LightingUtils.getMaterial(plot3d.getMaterial()));
                    LightingUtils.setupLights(drawingTools.getLightManager(), currentAxes);

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
                        Integer lineColor = plot3d.getSelected() ? plot3d.getSelectedColor() : plot3d.getLineColor();
                        appearance.setLineColor(ColorFactory.createColor(colorMap, lineColor));
                        appearance.setLineWidth(plot3d.getLineThickness().floatValue());
                    }
                    drawingTools.draw(geometry, appearance);
                    LightingUtils.setLightingEnable(drawingTools.getLightManager(), false);
                }

                if (plot3d.getMarkMode()) {
                    Appearance appearance = null;
                    if (plot3d.getLineThickness() > 0.0) {
                        appearance = new Appearance();
                        appearance.setLineWidth(plot3d.getLineThickness().floatValue());
                    }

                    ElementsBuffer positions = dataManager.getVertexBuffer(plot3d.getIdentifier());
                    if ((plot3d.getMark().getBackground() == -3 || plot3d.getMark().getForeground() == -3) && plot3d.getColorFlag() == 1) {
                        Texture sprite = markManager.getMarkSprite(plot3d, null, appearance);
                        ElementsBuffer colors = dataManager.getColorBuffer(plot3d.getIdentifier());
                        Color auxColor;
                        if (plot3d.getMark().getBackground() == -3) {
                            auxColor = ColorFactory.createColor(colorMap, plot3d.getMark().getForeground());
                        } else {
                            auxColor = ColorFactory.createColor(colorMap, plot3d.getMark().getBackground());
                        }
                        drawingTools.draw(sprite, AnchorPosition.CENTER, positions, auxColor, colors);
                    } else {
                        Texture sprite = markManager.getMarkSprite(plot3d, colorMap, appearance);
                        drawingTools.draw(sprite, AnchorPosition.CENTER, positions, null, null);
                    }
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
    public void visit(Datatip datatip) {
        if (datatip.isValid() && datatip.getVisible()) {
            axesDrawer.enableClipping(currentAxes, datatip.getClipProperty());
            try {
                Double[] box = currentAxes.getCorrectedBounds();
                Vector3d markPos = DatatipTextDrawer.calculateAnchorPoint(datatip);
                Double x = markPos.getX();
                Double y = markPos.getY();
                Double z = markPos.getZ();
                if (x >= box[0] && x <= box[1] &&
                        y >= box[2] && y <= box[3] &&
                        z >= box[4] && z <= box[5]) {
                    if (datatip.getMarkMode()) {
                        /* TODO: appearance can be not-null */
                        Texture texture = markManager.getMarkSprite(datatip, colorMap, null);
                        drawingTools.draw(texture, AnchorPosition.CENTER, markPos);
                    }
                    if (datatip.getTipLabelMode()) {
                        datatipTextDrawer.draw(drawingTools, colorMap, datatip);
                    }
                }
            } catch (SciRendererException e) {
                invalidate((Text)datatip, e);
            }
            axesDrawer.disableClipping(datatip.getClipProperty());
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
                    ElementsBuffer positions = dataManager.getVertexBuffer(segs.getIdentifier());
                    // Take only into account start-end of segs and not the arrow head.
                    positions.getData().limit(segs.getNumberArrows() * 2 * 4);

                    if (segs.getArrows().get(0).getMark().getBackground() == -3 || segs.getArrows().get(0).getMark().getForeground() == -3) {
                        Texture sprite = markManager.getMarkSprite(segs.getIdentifier(), segs.getArrows().get(0).getMark(), null, null);
                        ElementsBuffer colors = dataManager.getColorBuffer(segs.getIdentifier());
                        Color auxColor;
                        if (segs.getArrows().get(0).getMark().getBackground() == -3) {
                            auxColor = ColorFactory.createColor(colorMap, segs.getArrows().get(0).getMark().getForeground());
                        } else {
                            auxColor = ColorFactory.createColor(colorMap, segs.getArrows().get(0).getMark().getBackground());
                        }
                        drawingTools.draw(sprite, AnchorPosition.CENTER, positions, auxColor, colors);
                    } else {
                        Texture sprite = markManager.getMarkSprite(segs.getIdentifier(), segs.getArrows().get(0).getMark(), colorMap, null);
                        drawingTools.draw(sprite, AnchorPosition.CENTER, positions, null, null);
                    }

                    positions.getData().limit(positions.getData().capacity());
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
    public void updateObject(Integer id, int property) {
        /*
         * Check if property is CHILDREN and if there is a new child I should care about
         */
        Integer type = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);
        int objectStyle = (type == GraphicObjectProperties.__GO_UICONTROL__ ? (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_STYLE__) : -1);
        if (id.intValue() != figure.getIdentifier().intValue()
                && ((type == GraphicObjectProperties.__GO_UICONTROL__ && objectStyle != GraphicObjectProperties.__GO_UI_FRAME__)
                    || type == GraphicObjectProperties.__GO_UIMENU__)) {
            return;
        }

        if (property == GraphicObjectProperties.__GO_CHILDREN__) {
            if (id.intValue() != figure.getIdentifier().intValue()) {
                /* Ignore children that are not mine */
                return;
            }
            Integer[] children = GraphicController.getController().getObjectFromId(id).getChildren();
            List<Integer> currentOpenGLChildren = openGLChildren.get(id);
            if (currentOpenGLChildren == null) {
                /* No openGLChildren in cache, create empty one */
                openGLChildren.put(id, new ArrayList<Integer>());
                currentOpenGLChildren = openGLChildren.get(id);
            }
            List<Integer> updatedOpenGLChildren = new ArrayList<Integer>();
            for (int i = 0 ; i < children.length ; ++i) {
                Integer currentType = (Integer) GraphicController.getController().getProperty(children[i], GraphicObjectProperties.__GO_TYPE__);
                if (currentType != GraphicObjectProperties.__GO_UICONTROL__ && currentType != GraphicObjectProperties.__GO_UIMENU__) {
                    updatedOpenGLChildren.add(children[i]);
                }
            }
            if (currentOpenGLChildren.size() == updatedOpenGLChildren.size()) {
                /* No change made on openGL children => nothing to do */
                return;
            } else {
                openGLChildren.put(id, updatedOpenGLChildren);
            }
        }

        try {
            if (needUpdate(id, property)) {
                if (GraphicObjectProperties.__GO_COLORMAP__ == property) {
                    labelManager.disposeAll();
                    dataManager.disposeAllColorBuffers();
                    dataManager.disposeAllTextureCoordinatesBuffers();
                    markManager.disposeAll();
                    textManager.disposeAll();
                    axesDrawer.disposeAll();
                    fecDrawer.updateAll();
                    colorMapTextureDataProvider.update();
                    datatipTextDrawer.disposeAll();
                    textureManager.disposeAll();
                } else {
                    labelManager.update(id, property);
                    dataManager.update(id, property);
                    markManager.update(id, property);
                    textManager.update(id, property);
                    axesDrawer.update(id, property);
                    legendDrawer.update(id, property);
                    fecDrawer.update(id, property);
                    datatipTextDrawer.update(id, property);
                }

                if (GraphicObjectProperties.__GO_ANTIALIASING__ == property) {
                    canvas.setAntiAliasingLevel(figure.getAntialiasing());
                }

                Figure parentFigure = (Figure) GraphicController.getController().getObjectFromId(figure.getParentFigure());
                if (figure.getVisible() && parentFigure != null && parentFigure.getVisible()) {
                    if (isImmediateDrawing(id)) {
                        if (GraphicObjectProperties.__GO_IMMEDIATE_DRAWING__ == property) {
                            canvas.redrawAndWait();
                        } else {
                            canvas.redraw();
                        }
                    }
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
    protected boolean needUpdate(Integer id, int property) {
        GraphicObject object = GraphicController.getController().getObjectFromId(id);
        int objectType = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);
        int objectStyle = (objectType == GraphicObjectProperties.__GO_UICONTROL__ ? (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_STYLE__) : -1);
        if ((object != null) && (isFigureChild(id) || isFigureParent(id)) || (objectType == GraphicObjectProperties.__GO_UICONTROL__ && objectStyle == GraphicObjectProperties.__GO_UI_FRAME__)
                && objectType != GraphicObjectProperties.__GO_UIMENU__ && objectType != GraphicObjectProperties.__GO_UI_FRAME_BORDER__) {

            if (GraphicObjectProperties.__GO_VALID__ == property) {
                return false;
            }

            if (object instanceof Axes) {
                Axes axes = (Axes) object;

                if ((axes.getXAxisAutoTicks() && X_AXIS_TICKS_PROPERTIES.contains(property)) ||
                        (axes.getYAxisAutoTicks() && Y_AXIS_TICKS_PROPERTIES.contains(property)) ||
                        (axes.getZAxisAutoTicks() && Z_AXIS_TICKS_PROPERTIES.contains(property))) {
                    return false;
                }

                if ((!axes.getXAxisAutoTicks() && X_AXIS_TICKS_PROPERTIES.contains(property)) ||
                        (!axes.getYAxisAutoTicks() && Y_AXIS_TICKS_PROPERTIES.contains(property)) ||
                        (!axes.getZAxisAutoTicks() && Z_AXIS_TICKS_PROPERTIES.contains(property))) {
                    axesDrawer.computeMargins(axes);
                    return true;
                }

                if (property != GraphicObjectProperties.__GO_CHILDREN__) {
                    axesDrawer.computeRulers(axes);
                }
            }

            if (object instanceof Label || object instanceof Legend) {
                GraphicObject parent = GraphicController.getController().getObjectFromId(object.getParent());
                if (parent instanceof Axes) {
                    Axes axes = (Axes) parent;
                    if (axes.getXAxisLabel().equals(id) ||
                            axes.getYAxisLabel().equals(id) ||
                            axes.getZAxisLabel().equals(id) ||
                            axes.getTitle().equals(id)) {
                        labelManager.update(id, property);
                        axesDrawer.computeMargins(axes);
                    } else if (object instanceof Legend && (property == GraphicObjectProperties.__GO_LEGEND_LOCATION__ || property == GraphicObjectProperties.__GO_LINE_WIDTH__)) {
                        legendDrawer.update(id, property);
                        axesDrawer.computeMargins(axes);
                    }
                }
            } else if (object instanceof Figure) {
                if (property == GraphicObjectProperties.__GO_SIZE__
                        || property == GraphicObjectProperties.__GO_AXES_SIZE__
                        || property == GraphicObjectProperties.__GO_CHILDREN__
                        || property == GraphicObjectProperties.__GO_POSITION__
                        || property == GraphicObjectProperties.__GO_VISIBLE__) {
                    for (Integer gid : figure.getChildren()) {
                        GraphicObject go = GraphicController.getController().getObjectFromId(gid);
                        if (go instanceof Axes) {
                            axesDrawer.computeMargins((Axes) go);
                            axesDrawer.computeRulers((Axes) go);
                        }
                    }
                }

                if (SILENT_FIGURE_PROPERTIES.contains(property)) {
                    return false;
                }
            } else if (object instanceof Frame
                       && id.intValue() == figure.getIdentifier().intValue()
                       && property == GraphicObjectProperties.__GO_POSITION__) {
                Frame fig = (Frame) object;
                for (Integer gid : fig.getChildren()) {
                    GraphicObject go = GraphicController.getController().getObjectFromId(gid);
                    if (go instanceof Axes) {
                        axesDrawer.computeRulers((Axes) go);
                    }
                }

            } else if (object instanceof Axes && property == GraphicObjectProperties.__GO_X_AXIS_LOCATION__ ||
                       property == GraphicObjectProperties.__GO_Y_AXIS_LOCATION__ || property == GraphicObjectProperties.__GO_AUTO_MARGINS__) {
                axesDrawer.computeMargins((Axes) object);
            }

            if (!object.isValid()) {
                GraphicController.getController().setProperty(id, GraphicObjectProperties.__GO_VALID__, true);
            }

            return true;
        }
        // Special case if top level figure colormap/immediate_drawing has been updated, force redraw
        if ((property == GraphicObjectProperties.__GO_COLORMAP__ ||  property == GraphicObjectProperties.__GO_IMMEDIATE_DRAWING__)
                && id.intValue() == figure.getParentFigure().intValue()) {
            return true;
        }
        return false;
    }

    private boolean isImmediateDrawing(Integer id) {
        Integer parentId = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_PARENT_FIGURE__);
        if (figure instanceof Frame) {
            parentId = figure.getParentFigure();
        }
        if (figure instanceof Figure && (parentId == null || !parentId.equals(figure.getIdentifier()))) {
            return false;
        } else {
            Boolean b =  (Boolean) GraphicController.getController().getProperty(parentId, GraphicObjectProperties.__GO_IMMEDIATE_DRAWING__);
            return (b == null) ? false : b;
        }
    }

    @Override
    public void createObject(Integer id) {
    }

    @Override
    public void deleteObject(Integer id) {
        Integer type = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);
        if (!figure.getIdentifier().equals(id) && type == GraphicObjectProperties.__GO_UICONTROL__ || type == GraphicObjectProperties.__GO_UIMENU__) {
            return; // Not of my managed openGL children
        }

        openGLChildren.remove(id);

        if (isImmediateDrawing(id)) {
            canvas.redraw();
        }

        dataManager.dispose(id);
        markManager.dispose(id);
        textManager.dispose(id);
        labelManager.dispose(id);
        axesDrawer.dispose(id);
        legendDrawer.dispose(id);
        fecDrawer.dispose(id);
        textureManager.dispose(id);
        /*
         * Check we are deleting Figure managed by DrawerVisitor(this)
         * Otherwise do nothing on deletion.
         */
        if (!figure.getIdentifier().equals(id)) {
            return;
        }

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
    }

    /**
     * Check if the given id correspond to a parent of the current {@see Figure}.
     * @param id the given id.
     * @return true if the given id correspond to a parent of the current {@see Figure}.
     */
    private boolean isFigureParent(Integer id) {
        GraphicObject object = GraphicController.getController().getObjectFromId(id);
        if (object != null) {
            Object parentObject = GraphicController.getController().getProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_PARENT__);
            Integer parentUID = parentObject == null ? 0 : (Integer) parentObject;
            while (parentUID.intValue() != 0) {
                if (parentUID.intValue() == id.intValue()) {
                    return true;
                }
                parentObject = GraphicController.getController().getProperty(parentUID, GraphicObjectProperties.__GO_PARENT__);
                parentUID = parentObject == null ? 0 : (Integer) parentObject;
            }
        }

        return false;
    }

    /**
     * Check if the given id correspond to a child of the current {@see Figure}.
     * @param id the given id.
     * @return true if the given id correspond to a child of the current {@see Figure}.
     */
    private boolean isFigureChild(Integer id) {
        if (id.intValue() == figure.getIdentifier().intValue()) {
            return true;
        }

        Object parentObject = GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_PARENT__);
        Integer parentUID = parentObject == null ? 0 : (Integer) parentObject;
        while (parentUID != 0) {

            if (figure.getIdentifier().intValue() == parentUID.intValue()) {
                return true;
            }

            Integer parentStyle = (Integer) GraphicController.getController().getProperty(parentUID, GraphicObjectProperties.__GO_STYLE__);
            if (parentStyle != null && parentStyle.intValue() == GraphicObjectProperties.__GO_UI_FRAME__) {
                // Drop drawing if parent is a Frame and I'm not the dedicated visitor.
                return false;
            }

            parentObject = GraphicController.getController().getProperty(parentUID, GraphicObjectProperties.__GO_PARENT__);
            parentUID = parentObject == null ? 0 : (Integer) parentObject;

        }
        return false;
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
    public AxesContainer getFigure() {
        return figure;
    }

    public Axes getAxes() {
        return currentAxes;
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

        public ColorMapTextureDataProvider() {
            super();
            this.imageType = ImageType.RGBA_BYTE;
        }

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
