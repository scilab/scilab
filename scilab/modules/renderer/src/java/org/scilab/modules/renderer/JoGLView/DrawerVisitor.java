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
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.sprite.Sprite;
import org.scilab.forge.scirenderer.sprite.SpriteAnchorPosition;
import org.scilab.forge.scirenderer.tranformations.DegenerateMatrixException;
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
import org.scilab.modules.graphic_objects.graphicObject.IVisitor;
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

import java.util.HashMap;
import java.util.Map;

/**
 * @author Pierre Lando
 */
public class DrawerVisitor implements IVisitor, Drawer, GraphicView {
    private final Canvas canvas;
    private final Figure figure;
    private final DataManager dataManager;
    private final MarkSpriteManager markManager;
    private final TextManager textManager;
    private final AxesDrawer axesDrawer;
    private final AxisDrawer axisDrawer;
    private final ContouredObjectDrawer contouredObjectDrawer;
    private final LegendDrawer legendDrawer;
    private final LabelManager labelManager;

    private ColorMap colorMap;
    private DrawingTools drawingTools = null;

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
            } catch (DegenerateMatrixException e) {
                // TODO : warm the user that this axes can't be visible due to
                //        bad data bounds or axes position (margins) values.
            }
        }
    }

    @Override
    public void visit(Arc arc) {
        if (arc.getVisible()) {
            contouredObjectDrawer.draw(arc);
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

    /*
     * To do:
     * -use polygon offset for wireframe rendering.
     */
    @Override
    public void visit(final Fec fec) {
        if (fec.getVisible()) {
            DefaultGeometry triangles = new DefaultGeometry();
            triangles.setDrawingMode(Geometry.DrawingMode.TRIANGLES);
            triangles.setVertices(dataManager.getVertexBuffer(fec.getIdentifier()));
            triangles.setColors(dataManager.getColorBuffer(fec.getIdentifier()));
            triangles.setIndices(dataManager.getIndexBuffer(fec.getIdentifier()));
            triangles.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);


            DefaultGeometry wireFrame = new DefaultGeometry();
            wireFrame.setDrawingMode(Geometry.DrawingMode.SEGMENTS);
            wireFrame.setVertices(dataManager.getVertexBuffer(fec.getIdentifier()));
            wireFrame.setIndices(dataManager.getWireIndexBuffer(fec.getIdentifier()));
            wireFrame.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);

            Appearance trianglesAppearance = new Appearance();
            drawingTools.draw(triangles, trianglesAppearance);

            if (fec.getLineMode()) {
                Appearance wireFrameAppearance = new Appearance();
                wireFrameAppearance.setLineColor(ColorFactory.createColor(colorMap, fec.getLineColor()));
                drawingTools.draw(wireFrame, wireFrameAppearance);
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
            DefaultGeometry triangles = new DefaultGeometry();
            triangles.setDrawingMode(Geometry.DrawingMode.TRIANGLES);
            triangles.setVertices(dataManager.getVertexBuffer(grayplot.getIdentifier()));
            triangles.setColors(dataManager.getColorBuffer(grayplot.getIdentifier()));
            triangles.setIndices(dataManager.getIndexBuffer(grayplot.getIdentifier()));
            triangles.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);
            Appearance trianglesAppearance = new Appearance();
            drawingTools.draw(triangles, trianglesAppearance);
        }
    }

    @Override
    public void visit(final Matplot matplot) {
        if (matplot.getVisible()) {
            DefaultGeometry triangles = new DefaultGeometry();
            triangles.setDrawingMode(Geometry.DrawingMode.TRIANGLES);
            triangles.setVertices(dataManager.getVertexBuffer(matplot.getIdentifier()));
            triangles.setColors(dataManager.getColorBuffer(matplot.getIdentifier()));
            triangles.setIndices(dataManager.getIndexBuffer(matplot.getIdentifier()));
            triangles.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);
            Appearance trianglesAppearance = new Appearance();
            drawingTools.draw(triangles, trianglesAppearance);
        }
    }

    @Override
    public void visit(Label label) {
        // TODO
        System.out.println("How can I draw a label ?");
    }

    @Override
    public void visit(Legend legend) {
        if (legend.getVisible()) {
            legendDrawer.draw(legend);
        }
    }

    @Override
    public void visit(final Polyline polyline) {
        if (polyline.getVisible()) {

            DefaultGeometry triangles = new DefaultGeometry();
            triangles.setDrawingMode(Geometry.DrawingMode.TRIANGLES);
            triangles.setVertices(dataManager.getVertexBuffer(polyline.getIdentifier()));

            /* Interpolated color rendering is used only for basic polylines for now. */
            if (polyline.getInterpColorMode() && polyline.getPolylineStyle() == 1) {
                triangles.setColors(dataManager.getColorBuffer(polyline.getIdentifier()));
            } else {
                triangles.setColors(null);
            }

            triangles.setIndices(dataManager.getIndexBuffer(polyline.getIdentifier()));
            triangles.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);

            /**
             * TODO : try to remove the wire frame and use 'triangle.setEdgeIndices' it's here for that.
             */
            DefaultGeometry wireFrame = new DefaultGeometry();
            wireFrame.setDrawingMode(Geometry.DrawingMode.SEGMENTS);
            wireFrame.setVertices(dataManager.getVertexBuffer(polyline.getIdentifier()));
            wireFrame.setIndices(dataManager.getWireIndexBuffer(polyline.getIdentifier()));
            wireFrame.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);

            Appearance trianglesAppearance = new Appearance();

            if (!polyline.getInterpColorMode() || polyline.getPolylineStyle() != 1) {
                trianglesAppearance.setFillColor(ColorFactory.createColor(colorMap, polyline.getBackground()));
            }

            drawingTools.draw(triangles, trianglesAppearance);

            Appearance appearance = new Appearance();
            appearance.setLineColor(ColorFactory.createColor(colorMap, polyline.getLineColor()));
            appearance.setLineWidth(polyline.getLineThickness().floatValue());
            appearance.setLinePattern(polyline.getLineStyleAsEnum().asPattern());


            drawingTools.draw(wireFrame, appearance);


            if (polyline.getMarkMode()) {
                Sprite sprite = markManager.getMarkSprite(polyline, colorMap);
                ElementsBuffer positions = dataManager.getVertexBuffer(polyline.getIdentifier());  // TODO : getMarkVertexBuffer
                drawingTools.draw(sprite, SpriteAnchorPosition.CENTER, positions);
            }
        }
    }

    @Override
    public void visit(Rectangle rectangle) {
        if (rectangle.getVisible()) {
            contouredObjectDrawer.draw(rectangle);
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

            DefaultGeometry triangles = new DefaultGeometry();
            triangles.setDrawingMode(Geometry.DrawingMode.TRIANGLES);
            triangles.setVertices(dataManager.getVertexBuffer(fac3d.getIdentifier()));

            if (fac3d.getColorFlag() > 0) {
                triangles.setColors(dataManager.getColorBuffer(fac3d.getIdentifier()));
            } else {
                triangles.setColors(null);
            }

            triangles.setIndices(dataManager.getIndexBuffer(fac3d.getIdentifier()));
            triangles.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);
            if (fac3d.getHiddenColor() > 0) {
                triangles.setFaceCullingMode(axesDrawer.getFrontFaceCullingMode());
            } else {
                triangles.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);
            }

            DefaultGeometry backTriangles = new DefaultGeometry();
            backTriangles.setDrawingMode(Geometry.DrawingMode.TRIANGLES);
            backTriangles.setVertices(dataManager.getVertexBuffer(fac3d.getIdentifier()));
            backTriangles.setIndices(dataManager.getIndexBuffer(fac3d.getIdentifier()));
            backTriangles.setFaceCullingMode(axesDrawer.getBackFaceCullingMode());

            if (fac3d.getSurfaceMode()) {
                if (fac3d.getColorMode() != 0) {
                    /* Back-facing triangles */
                    if (fac3d.getHiddenColor() > 0) {
                        Appearance backTrianglesAppearance = new Appearance();
                        backTrianglesAppearance.setFillColor(ColorFactory.createColor(colorMap, fac3d.getHiddenColor()));
                        drawingTools.draw(backTriangles, backTrianglesAppearance);
                    }

                    /* Front-facing triangles */
                    Appearance trianglesAppearance = new Appearance();

                    if (fac3d.getColorFlag() == 0) {
                        trianglesAppearance.setFillColor(ColorFactory.createColor(colorMap, Math.abs(fac3d.getColorMode())));
                    }

                    drawingTools.draw(triangles, trianglesAppearance);
                }

                if ((fac3d.getColorMode() >= 0) && (fac3d.getLineThickness() > 0.0)) {
                    // TODO: use 'EdgeIndices'.
                    DefaultGeometry wireFrame = new DefaultGeometry();
                    wireFrame.setDrawingMode(Geometry.DrawingMode.SEGMENTS);
                    wireFrame.setVertices(dataManager.getVertexBuffer(fac3d.getIdentifier()));
                    wireFrame.setIndices(dataManager.getWireIndexBuffer(fac3d.getIdentifier()));
                    wireFrame.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);

                    Appearance wireFrameAppearance = new Appearance();
                    wireFrameAppearance.setLineColor(ColorFactory.createColor(colorMap, fac3d.getLineColor()));
                    wireFrameAppearance.setLineWidth(fac3d.getLineThickness().floatValue());
                    drawingTools.draw(wireFrame, wireFrameAppearance);
                }
            }

            if (fac3d.getMarkMode()) {
                Sprite sprite = markManager.getMarkSprite(fac3d, colorMap);
                ElementsBuffer positions = dataManager.getVertexBuffer(fac3d.getIdentifier());  // TODO : getMarkVertexBuffer
                drawingTools.draw(sprite, SpriteAnchorPosition.CENTER, positions);
            }

        }

    }

    @Override
    public void visit(final Plot3d plot3d) {
        if (plot3d.getVisible()) {

            DefaultGeometry triangles = new DefaultGeometry();
            triangles.setDrawingMode(Geometry.DrawingMode.TRIANGLES);
            triangles.setVertices(dataManager.getVertexBuffer(plot3d.getIdentifier()));
            triangles.setIndices(dataManager.getIndexBuffer(plot3d.getIdentifier()));

            if (plot3d.getColorFlag() == 1) {
                triangles.setColors(dataManager.getColorBuffer(plot3d.getIdentifier()));
            } else {
                triangles.setColors(null);
            }

            if (plot3d.getHiddenColor() > 0) {
                triangles.setFaceCullingMode(axesDrawer.getFrontFaceCullingMode());
            } else {
                triangles.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);
            }

            DefaultGeometry backTriangles = new DefaultGeometry();
            backTriangles.setDrawingMode(Geometry.DrawingMode.TRIANGLES);
            backTriangles.setVertices(dataManager.getVertexBuffer(plot3d.getIdentifier()));
            backTriangles.setIndices(dataManager.getIndexBuffer(plot3d.getIdentifier()));
            backTriangles.setFaceCullingMode(axesDrawer.getBackFaceCullingMode());

            if (plot3d.getSurfaceMode()) {
                if (plot3d.getColorMode() != 0) {

                    /* Back-facing triangles */
                    if (plot3d.getHiddenColor() > 0) {
                        Appearance backTrianglesAppearance = new Appearance();
                        backTrianglesAppearance.setFillColor(ColorFactory.createColor(colorMap, plot3d.getHiddenColor()));
                        drawingTools.draw(backTriangles, backTrianglesAppearance);
                    }

                    /* Front-facing triangles */
                    Appearance trianglesAppearance = new Appearance();

                    if (plot3d.getColorFlag() == 0) {
                        trianglesAppearance.setFillColor(ColorFactory.createColor(colorMap, Math.abs(plot3d.getColorMode())));
                    }

                    drawingTools.draw(triangles, trianglesAppearance);
                }

                if ((plot3d.getColorMode() >= 0) && (plot3d.getLineThickness() > 0.0)) {
                    // TODO: use edgeIndices.
                    DefaultGeometry wireFrame = new DefaultGeometry();
                    wireFrame.setDrawingMode(Geometry.DrawingMode.SEGMENTS);
                    wireFrame.setVertices(dataManager.getVertexBuffer(plot3d.getIdentifier()));
                    wireFrame.setIndices(dataManager.getWireIndexBuffer(plot3d.getIdentifier()));
                    wireFrame.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);

                    Appearance wireFrameAppearance = new Appearance();
                    wireFrameAppearance.setLineColor(ColorFactory.createColor(colorMap, plot3d.getLineColor()));
                    wireFrameAppearance.setLineWidth(plot3d.getLineThickness().floatValue());
                    drawingTools.draw(wireFrame, wireFrameAppearance);
                }
            }

            if (plot3d.getMarkMode()) {
                Sprite sprite = markManager.getMarkSprite(plot3d, colorMap);
                ElementsBuffer positions = dataManager.getVertexBuffer(plot3d.getIdentifier());  // TODO : getMarkVertexBuffer
                drawingTools.draw(sprite, SpriteAnchorPosition.CENTER, positions);
            }

        }

    }

    @Override
    public void visit(Text text) {
        if (text.getVisible()) {
            try {
                textManager.draw(drawingTools, colorMap, text, axesDrawer);
            } catch (DegenerateMatrixException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
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
            DefaultGeometry segments = new DefaultGeometry();
            segments.setDrawingMode(Geometry.DrawingMode.SEGMENTS);
            segments.setVertices(dataManager.getVertexBuffer(champ.getIdentifier()));
            segments.setIndices(dataManager.getWireIndexBuffer(champ.getIdentifier()));
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
        }
    }

    /*
     * To do: -arrow tip rendering.
     */
    @Override
    public void visit(final Segs segs) {
        if (segs.getVisible()) {
            DefaultGeometry segments = new DefaultGeometry();
            segments.setDrawingMode(Geometry.DrawingMode.SEGMENTS);
            segments.setVertices(dataManager.getVertexBuffer(segs.getIdentifier()));
            segments.setColors(dataManager.getColorBuffer(segs.getIdentifier()));
            segments.setIndices(dataManager.getWireIndexBuffer(segs.getIdentifier()));
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
                ElementsBuffer positions = dataManager.getVertexBuffer(segs.getIdentifier());  // TODO : getMarkVertexBuffer
                drawingTools.draw(sprite, SpriteAnchorPosition.CENTER, positions);
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
}

