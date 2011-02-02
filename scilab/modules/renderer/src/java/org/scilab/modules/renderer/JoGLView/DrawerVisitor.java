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
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.shapes.geometry.GeometryImpl;
import org.scilab.forge.scirenderer.sprite.Sprite;
import org.scilab.forge.scirenderer.sprite.SpriteAnchorPosition;
import org.scilab.modules.graphic_objects.arc.Arc;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axis.Axis;
import org.scilab.modules.graphic_objects.compound.Compound;
import org.scilab.modules.graphic_objects.fec.Fec;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.IVisitor;
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
import org.scilab.modules.renderer.JoGLView.mark.MarkSpriteManager;
import org.scilab.modules.renderer.JoGLView.text.TextManager;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;

/**
 * @author Pierre Lando
 */
public class DrawerVisitor implements IVisitor, Drawer {
    private final Canvas canvas;
    private final Figure figure;
    private final DataManager dataManager;
    private final MarkSpriteManager MarkManager;
    private final TextManager textManager;
    private final AxesDrawer axesDrawer;

    private ColorMap colorMap;
    private DrawingTools drawingTools = null;

    public DrawerVisitor(Canvas canvas, Figure figure) {
        this.canvas = canvas;
        this.figure = figure;

        this.dataManager = new DataManager(canvas);
        this.MarkManager = new MarkSpriteManager(canvas.getSpriteManager());
        this.textManager = new TextManager(canvas.getSpriteManager());
        this.axesDrawer = new AxesDrawer(this);
    }

    public DrawingTools getDrawingTools() {
        return drawingTools;
    }

    public Canvas getCanvas() {
        return canvas;
    }

    public ColorMap getColorMap() {
        return colorMap;
    }

    @Override
    public void draw(DrawingTools drawingTools) {
        long start = System.currentTimeMillis();
        this.drawingTools = drawingTools;
        figure.accept(this);
        long end = System.currentTimeMillis();
        long delta = end - start;
        System.out.println(delta + "ms | " + 1000.0 / delta + " FPS");
    }

    /**
     * Ask the given object to accept visitor.
     * @param ids array of object identifier.
     */
    public void askAcceptVisitor(String[] ids) {
        for (String id : ids) {
            GraphicObject object = GraphicController.getController().getObjectFromId(id);
            object.accept(this);
        }
    }

    @Override
    public void visit(Axes axes) {
        if (axes.getVisible()) {
            axesDrawer.draw(axes);
        }
    }

    @Override
    public void visit(Arc arc) {
        // TODO
        System.out.println("How can I draw an arc ?");
    }

    @Override
    public void visit(Axis axis) {
        // TODO
        System.out.println("How can I draw an axis ?");
    }

    @Override
    public void visit(Compound compound) {
        if (compound.getVisible()) {
            askAcceptVisitor(compound.getChildren());
        }
    }

    @Override
    public void visit(Fec fec) {
        // TODO
        System.out.println("How can I draw a fec ?");
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

            askAcceptVisitor(figure.getChildren());
        }
    }

    @Override
    public void visit(Grayplot grayplot) {
        // TODO
        System.out.println("How can I draw a grayplot ?");
    }

    @Override
    public void visit(Matplot matplot) {
        // TODO
        System.out.println("How can I draw a matplot ?");
    }

    @Override
    public void visit(Label label) {
        // TODO
        System.out.println("How can I draw a label ?");
    }

    @Override
    public void visit(Legend legend) {
        // TODO
        System.out.println("How can I draw a legend ?");
    }

    @Override
    public void visit(Polyline polyline) {
        if (polyline.getVisible()) {
            if (polyline.getLineMode()) {
                Geometry geometry = new GeometryImpl(
                        Geometry.DrawingMode.SEGMENTS_STRIP,
                        dataManager.getVertexBuffer(polyline.getIdentifier())
                );

                Appearance appearance = new Appearance();

                appearance.setLineColor(ColorFactory.createColor(colorMap, polyline.getLineColor()));
                appearance.setLineWidth(polyline.getLineThickness().floatValue());
                appearance.setLinePattern(polyline.getLineStyleAsEnum().asPattern());

                drawingTools.draw(geometry, appearance);
            }

            if (polyline.getMarkMode()) {
                Sprite sprite = MarkManager.getMarkSprite(polyline, colorMap);
                ElementsBuffer positions = dataManager.getVertexBuffer(polyline.getIdentifier());
                drawingTools.draw(sprite, SpriteAnchorPosition.CENTER, positions);
            }
        }
    }

    @Override
    public void visit(Rectangle rectangle) {
        // TODO
        System.out.println("How can I draw a rectangle ?");
    }

    @Override
    public void visit(Fac3d fac3d) {
        // TODO
        System.out.println("How can I draw a fac3d ?");
    }

    @Override
    public void visit(Plot3d plot3d) {
        // TODO
        System.out.println("How can I draw a plot3d ?");
    }

    @Override
    public void visit(Text text) {
        if (text.getVisible()) {
            textManager.draw(drawingTools, colorMap, text);
        }
    }

    @Override
    public void visit(Arrow arrow) {
        // TODO
        System.out.println("How can I draw an arrow ?");
    }

    @Override
    public void visit(Champ champ) {
        // TODO
        System.out.println("How can I draw a champ ?");
    }

    @Override
    public void visit(Segs segs) {
        // TODO
        System.out.println("How can I draw a segs ?");
    }
}
