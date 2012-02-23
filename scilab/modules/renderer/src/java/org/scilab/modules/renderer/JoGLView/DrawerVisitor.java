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
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.sprite.Sprite;
import org.scilab.forge.scirenderer.sprite.SpriteAnchorPosition;
import org.scilab.forge.scirenderer.texture.AbstractDataProvider;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureDataProvider;
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
import java.util.HashMap;
import java.util.Map;

/**
 * @author Pierre Lando
 */
public class DrawerVisitor implements Visitor, Drawer, GraphicView {
    private final Canvas canvas;
    private final Figure figure;

    private final ColorMapTextureDataProvider colorMapTextureDataProvider;

    private final MarkSpriteManager markManager;
    private final LabelManager labelManager;
    private final DataManager dataManager;
    private final TextManager textManager;

    private final ContouredObjectDrawer contouredObjectDrawer;
    private final LegendDrawer legendDrawer;
    private final AxesDrawer axesDrawer;
    private final AxisDrawer axisDrawer;

    private DrawingTools drawingTools = null;
    private Texture colorMapTexture;
    private ColorMap colorMap;

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
        this.markManager = new MarkSpriteManager(canvas.getSpriteManager());
        this.textManager = new TextManager(canvas.getSpriteManager());
        this.labelManager = new LabelManager(canvas.getSpriteManager());
        this.axesDrawer = new AxesDrawer(this);
        this.axisDrawer = new AxisDrawer(this);
        this.contouredObjectDrawer = new ContouredObjectDrawer(this, this.dataManager, this.markManager);
        this.legendDrawer = new LegendDrawer(this, canvas.getSpriteManager(), this.markManager);
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
     * @return the TextManager
     */
    public TextManager getTextManager() {
        return textManager;
    }

    /**
     * @return the AxeDrawer
     */
    public AxesDrawer getAxesDrawer() {
        return axesDrawer;
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
                    child.accept(this);
                }
            }
        }
    }

    @Override
    public void visit(Axes axes) {
        if (axes.getVisible()) {
            try {
                axesDrawer.draw(axes);
            } catch (SciRendererException e) {
                System.err.println("A '" + axes.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }
    }

    @Override
    public void visit(Arc arc) {
        if (arc.getVisible()) {
            try {
                contouredObjectDrawer.draw(arc);
            } catch (SciRendererException e) {
                System.err.println("A '" + arc.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }
    }

    @Override
    public void visit(Axis axis) {
        if (axis.getVisible()) {
            axisDrawer.draw(axis);
        }
    }

    @Override
    public void visit(Compound compound) {
        if (compound.getVisible()) {
            askAcceptVisitor(compound.getChildren());
        }
    }

    @Override
    public void visit(final Fec fec) {
        if (fec.getVisible()) {
            try {
                DefaultGeometry geometry = new DefaultGeometry();
                Appearance appearance = new Appearance();

                geometry.setVertices(dataManager.getVertexBuffer(fec.getIdentifier()));
                geometry.setTextureCoordinates(dataManager.getTextureCoordinatesBuffer(fec.getIdentifier()));
                geometry.setIndices(dataManager.getIndexBuffer(fec.getIdentifier()));

                geometry.setFillDrawingMode(Geometry.FillDrawingMode.TRIANGLES);
                geometry.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);

                if (fec.getLineMode()) {
                    geometry.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
                    geometry.setWireIndices(dataManager.getWireIndexBuffer(fec.getIdentifier()));
                    appearance.setLineColor(ColorFactory.createColor(colorMap, fec.getLineColor()));
                }
                appearance.setTexture(getColorMapTexture());

                drawingTools.draw(geometry, appearance);
            } catch (SciRendererException e) {
                System.err.println("A '" + fec.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }
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
                DefaultGeometry triangles = new DefaultGeometry();
                triangles.setFillDrawingMode(Geometry.FillDrawingMode.TRIANGLES);
                triangles.setVertices(dataManager.getVertexBuffer(grayplot.getIdentifier()));
                triangles.setColors(dataManager.getColorBuffer(grayplot.getIdentifier()));
                triangles.setIndices(dataManager.getIndexBuffer(grayplot.getIdentifier()));
                triangles.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);
                Appearance trianglesAppearance = new Appearance();
                drawingTools.draw(triangles, trianglesAppearance);
            } catch (SciRendererException e) {
                System.err.println("A '" + grayplot.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }
    }

    @Override
    public void visit(final Matplot matplot) {
        if (matplot.getVisible()) {
            try {
                DefaultGeometry triangles = new DefaultGeometry();
                triangles.setFillDrawingMode(Geometry.FillDrawingMode.TRIANGLES);
                triangles.setVertices(dataManager.getVertexBuffer(matplot.getIdentifier()));
                triangles.setColors(dataManager.getColorBuffer(matplot.getIdentifier()));
                triangles.setIndices(dataManager.getIndexBuffer(matplot.getIdentifier()));
                triangles.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);
                Appearance trianglesAppearance = new Appearance();
                drawingTools.draw(triangles, trianglesAppearance);
            } catch (SciRendererException e) {
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
            } catch (SciRendererException e) {
                System.err.println("A '" + legend.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }
    }

    @Override
    public void visit(final Polyline polyline) {
        if (polyline.getVisible()) {
            try {
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

                if (polyline.getMarkMode()) {
                    Sprite sprite = markManager.getMarkSprite(polyline, colorMap);
                    ElementsBuffer positions = dataManager.getVertexBuffer(polyline.getIdentifier());
                    drawingTools.draw(sprite, SpriteAnchorPosition.CENTER, positions);
                }
            } catch (SciRendererException e) {
                System.err.println("A '" + polyline.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }
    }

    @Override
    public void visit(Rectangle rectangle) {
        if (rectangle.getVisible()) {
            try {
                contouredObjectDrawer.draw(rectangle);
            } catch (SciRendererException e) {
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
                            geometry.setColors(dataManager.getColorBuffer(fac3d.getIdentifier()));
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
            } catch (SciRendererException e) {
                System.err.println("A '" + fac3d.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }

    }

    @Override
    public void visit(final Plot3d plot3d) {
        if (plot3d.getVisible()) {
            try {
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
            } catch (SciRendererException e) {
                System.err.println("A '" + plot3d.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }

    }

    @Override
    public void visit(Text text) {
        if (text.getVisible()) {
            try {
                textManager.draw(drawingTools, colorMap, text, axesDrawer);
            } catch (SciRendererException e) {
                System.err.println("A '" + text.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }
    }

    @Override
    public void visit(Arrow arrow) {
        // TODO
        System.out.println("How can I draw an arrow ?");
    }

    /*
     * To do: -arrow tip rendering.
     */
    @Override
    public void visit(final Champ champ) {
        if (champ.getVisible()) {
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
                    if (!champ.getColored()) {
                        segmentAppearance.setLineColor(ColorFactory.createColor(colorMap, champ.getLineColor()));
                    }

                    segmentAppearance.setLineWidth(champ.getLineThickness().floatValue());
                    segmentAppearance.setLinePattern(champ.getLineStyleAsEnum().asPattern());
                    drawingTools.draw(segments, segmentAppearance);
                }
            } catch (SciRendererException e) {
                System.err.println("A '" + champ.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }
    }

    /*
     * To do: -arrow tip rendering.
     */
    @Override
    public void visit(final Segs segs) {
        if (segs.getVisible()) {
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
                    segmentAppearance.setLineColor(ColorFactory.createColor(colorMap, segs.getLineColor()));
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
            } catch (SciRendererException e) {
                System.err.println("A '" + segs.getType() + "' is not drawable because: '" + e.getMessage() + "'");
            }
        }
    }

    @Override
    public void updateObject(String id, String property) {
        if (GraphicObjectProperties.__GO_COLORMAP__.equals(property) && figure.getIdentifier().equals(id)) {
            dataManager.disposeAllColorBuffers();
            markManager.disposeAll();
            textManager.disposeAll();
            labelManager.disposeAll();
            axesDrawer.disposeAll();
            canvas.redraw();
            colorMapTextureDataProvider.update();
        } else if (isFigureChild(id)) {
            dataManager.update(id, property);
            markManager.update(id, property);
            textManager.update(id, property);
            labelManager.update(id, property);
            axesDrawer.update(id, property);
            legendDrawer.update(id, property);
            canvas.redraw();
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

    private class ColorMapTextureDataProvider extends AbstractDataProvider<Texture> implements TextureDataProvider {

        @Override
        public Dimension getTextureSize() {
            return new Dimension(colorMap.getSize(), 1);
        }

        @Override
        public Buffer getData() {
            Double[] data = colorMap.getData();
            FloatBuffer buffer = BufferUtil.newFloatBuffer(4 * data.length / 3);
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
            Double[] data = colorMap.getData();
            FloatBuffer buffer = BufferUtil.newFloatBuffer(4 * width);
            for (int i = x ; i < x + width ; i++) {
                buffer.put(data[i].floatValue());
                buffer.put(data[i + colorMap.getSize()].floatValue());
                buffer.put(data[i + 2 * colorMap.getSize()].floatValue());
                buffer.put(1);
            }
            buffer.rewind();
            return buffer;
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

