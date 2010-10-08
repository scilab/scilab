/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView;

import org.scilab.forge.scirenderer.canvas.interfaces.appearance.IAppearance;
import org.scilab.forge.scirenderer.canvas.interfaces.appearance.material.color.FinalColor;
import org.scilab.forge.scirenderer.canvas.interfaces.appearance.material.color.IColor;
import org.scilab.forge.scirenderer.canvas.interfaces.buffers.IVertexBuffer;
import org.scilab.forge.scirenderer.canvas.interfaces.canvas.ICanvas;
import org.scilab.forge.scirenderer.canvas.interfaces.core.math.FinalRectangle;
import org.scilab.forge.scirenderer.canvas.interfaces.geometry.box.FinalBox;
import org.scilab.forge.scirenderer.canvas.interfaces.geometry.box.IBox;
import org.scilab.forge.scirenderer.canvas.interfaces.geometry.line.ILines;
import org.scilab.forge.scirenderer.canvas.interfaces.geometry.line.Lines;
import org.scilab.forge.scirenderer.canvas.interfaces.tranformations.ITransformation;
import org.scilab.forge.scirenderer.canvas.interfaces.tranformations.Rotation;
import org.scilab.forge.scirenderer.canvas.interfaces.tranformations.Scale;
import org.scilab.forge.scirenderer.canvas.interfaces.tranformations.Translate;
import org.scilab.modules.graphic_objects.arc.Arc;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axes.Box;
import org.scilab.modules.graphic_objects.axis.Axis;
import org.scilab.modules.graphic_objects.compound.Compound;
import org.scilab.modules.graphic_objects.contouredObject.Line;
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
import org.scilab.modules.graphic_objects.textObject.TextObject;
import org.scilab.modules.graphic_objects.vectfield.Arrow;
import org.scilab.modules.graphic_objects.vectfield.Champ;
import org.scilab.modules.graphic_objects.vectfield.Segs;

/**
 * @author Pierre Lando
 */
public class DrawerVisitor implements IVisitor {
    private static final double DEFAULT_ALPHA = 0.0;
    private static final double DEFAULT_THETA = 270.0;
    
    private final IVertexBuffer boxBorderVertices;
    private static final float[] boxBorderVerticesData = new float[] {
        -1,-1,+1,1, -1,+1,+1,1,
        -1,+1,-1,1, +1,+1,-1,1,
        +1,-1,-1,1, +1,-1,+1,1
    };

    private static final Line.LineType HIDDEN_BORDER_PATTERN = Line.LineType.STYLE1;
    private final IVertexBuffer hiddenBoxBorderVertices;
    private static final float[] hiddenBoxBorderVerticesData = new float[] {
        -1,-1,-1,1, +1,-1,-1,1,
        -1,-1,-1,1, -1,+1,-1,1,
        -1,-1,-1,1, -1,-1,+1,1
    };

    private final IVertexBuffer frontBoxBorderVertices;
    private static final float[] frontBoxBorderVerticesData = new float[] {
        +1,+1,+1,1, -1,+1,+1,1,
        +1,+1,+1,1, +1,-1,+1,1,
        +1,+1,+1,1, +1,+1,-1,1
    };

    private final ICanvas canvas;
    private final DataManager dataManager;

    private ColorMap colorMap;

    public DrawerVisitor(ICanvas canvas) {
        this.canvas = canvas;
        this.dataManager = new DataManager(canvas);
        boxBorderVertices = canvas.getBuffersManager().newVertexBuffer();
        boxBorderVertices.setData(boxBorderVerticesData, IVertexBuffer.ECoordinates.XYZW);

        hiddenBoxBorderVertices = canvas.getBuffersManager().newVertexBuffer();
        hiddenBoxBorderVertices.setData(hiddenBoxBorderVerticesData, IVertexBuffer.ECoordinates.XYZW);

        frontBoxBorderVertices = canvas.getBuffersManager().newVertexBuffer();
        frontBoxBorderVertices.setData(frontBoxBorderVerticesData, IVertexBuffer.ECoordinates.XYZW);
    }

    @Override
    public void finalize() throws Throwable {
        super.finalize();
        canvas.getBuffersManager().freeBuffer(boxBorderVertices);
        canvas.getBuffersManager().freeBuffer(hiddenBoxBorderVertices);
        canvas.getBuffersManager().freeBuffer(frontBoxBorderVertices);
    }


    /**
     * Ask the given object to accept visitor.
     * @param ids array of object identifier.
     */
    private void askAcceptVisitor(String[] ids) {
        for (String id : ids) {
            GraphicObject object = GraphicController.getController().getObjectFromId(id);
            object.accept(this);
        }
    }

    @Override
    public void visit(Axes axes) {
        if (axes.getVisible()) {
            // Set current zone
            /**********************************************************************************************************/
            Double[] axesBounds = axes.getAxesBounds();
            Double[] margins = axes.getMargins();

            double x = (axesBounds[0] + axesBounds[2] * margins[0]);
            double y = (1.0 - axesBounds[1] - axesBounds[3] * (1.0 - margins[3]));
            double w = (1 - margins[0] - margins[1]) * axesBounds[2];
            double h = (1 - margins[2] - margins[3]) * axesBounds[3];

            canvas.getZoneControl().setCurrentZone(new FinalRectangle(x+w/2, y+h/2, w, h));
            /***********************************************************************************************************
             * Compute transformation matrix.
             *
             * Data -> centered data (translation)
             * centered data -> scaled data (scale, depend on cube scaling)
             * scaled data -> rotated data
             * rotated data -> final 'in cube' data (scale, depend on isoview).
             * set square good ratio.
             *
             **********************************************************************************************************/

            // TODO : a vérifer. en fait FRect c'est qui ?
            Double[] bounds = axes.getDataBounds();

            double tmpX;
            double tmpY;
            double tmpZ;
            
            ITransformation transformation;

            // Set zone aspect ratio.
            double ratio = canvas.getZoneControl().getFinalAspectRatio();
            if (ratio < 1.0) {
                transformation = new Scale(1, ratio, 1);
            } else {
                transformation = new Scale(1/ratio, 1, 1);
            }

            // Rotate.
            transformation.times(new Rotation(DEFAULT_ALPHA - axes.getRotationAngles()[0], 1.0, 0.0, 0.0));
            transformation.times(new Rotation(DEFAULT_THETA - axes.getRotationAngles()[1], 0.0, 0.0, 1.0));

            // If there is no cube scaling, we must take into account the distribution of data.
            if (!axes.getCubeScaling()) {
                tmpX = (bounds[1] - bounds[0]);
                tmpY = (bounds[3] - bounds[2]);
                tmpZ = (bounds[5] - bounds[4]);

                /**
                 * Here, we should divide the values by their maximum.
                 * But the next operation will automatically.  
                 */
                transformation.times(new Scale(tmpX, tmpY, tmpZ));
            }

            // Compute bounds of projected data.
            double matrix[] = transformation.getMatrix16dv();
            tmpX = 1 / (Math.abs(matrix[0]) + Math.abs(matrix[4]) + Math.abs(matrix[8]));
            tmpY = 1 / (Math.abs(matrix[1]) + Math.abs(matrix[5]) + Math.abs(matrix[9]));
            tmpZ = 1 / (Math.abs(matrix[2]) + Math.abs(matrix[6]) + Math.abs(matrix[10]));

            // Scale projected data to fit in the square.
            if (axes.getIsoview()) {
                double minScale = Math.min(tmpX, tmpY);
                transformation = transformation.leftTimes(new Scale(minScale, minScale, tmpZ));
            } else {
                transformation = transformation.leftTimes(new Scale(tmpX, tmpY, tmpZ));
            }


            canvas.getTransformationsStack().push(transformation);

            // Draw box.
            /**********************************************************************************************************/
            canvas.getZoneControl().clearZBuffer();
            IColor backgroundColor = new FinalColor(colorMap.getScilabColor(axes.getBackground()));
            IBox box = new FinalBox(2, 2, 2);
            box.getAppearance().getFillProperties().getMaterial().setDiffuseColor(backgroundColor);
            box.getAppearance().setRenderingMode(IAppearance.ERenderingMode.FILL);
            canvas.draw(box);
            canvas.getZoneControl().clearZBuffer();

            // Draw box border.
            /**********************************************************************************************************/
            Box.BoxType boxed = axes.getBox().getBox();
            if (boxed != Box.BoxType.OFF) {
                /**
                 * Mirror the cube such that the corner with the minimum Z value was (-1, -1, -1).
                 */
                canvas.getTransformationsStack().push(new Scale(
                    matrix[2] < 0  ? -1 : 1,
                    matrix[6] < 0  ? -1 : 1,
                    matrix[10] < 0 ? -1 : 1
                ));

                ILines lines = new Lines();
                lines.getLineProperties().setLineWidth(axes.getLineThickness().floatValue());

                IColor lineColor;
                lineColor = new FinalColor(colorMap.getScilabColor(axes.getHiddenAxisColor()));
                lines.getLineProperties().getMaterial().setDiffuseColor(lineColor);
                lines.getLineProperties().setLinePattern(HIDDEN_BORDER_PATTERN.asPattern());

                lines.setMode(ILines.EMode.SEGMENTS);
                lines.setVertexBuffer(hiddenBoxBorderVertices);
                canvas.draw(lines);

                if (boxed != Box.BoxType.HIDDEN_AXES) {
                    lineColor = new FinalColor(colorMap.getScilabColor(axes.getLineColor()));
                    lines.getLineProperties().getMaterial().setDiffuseColor(lineColor);

                    lines.getLineProperties().setLinePattern(axes.getLine().getLineStyle().asPattern());
                    lines.setMode(ILines.EMode.LOOP);
                    lines.setVertexBuffer(boxBorderVertices);
                    canvas.draw(lines);

                    if (boxed != Box.BoxType.BACK_HALF) {
                        lines.setMode(ILines.EMode.SEGMENTS);
                        lines.setVertexBuffer(frontBoxBorderVertices);
                        canvas.draw(lines);
                    }
                }

                canvas.getTransformationsStack().pop();
            }
            /**********************************************************************************************************/

            // Draw data.
            /**********************************************************************************************************/

            canvas.getTransformationsStack().pop();

            // Reverse data if needed.
            transformation.times(
                new Scale(
                    axes.getAxes()[0].getReverse() ? -1 : 1,
                    axes.getAxes()[1].getReverse() ? -1 : 1,
                    axes.getAxes()[2].getReverse() ? -1 : 1
                )
            );


            // Scale data.
            tmpX = 2/(bounds[1] - bounds[0]);
            tmpY = 2/(bounds[3] - bounds[2]);
            tmpZ = 2/(bounds[5] - bounds[4]);

            transformation.times(new Scale(tmpX, tmpY, tmpZ));


            // Translate data.
            transformation.times(new Translate(
               - (bounds[0] + bounds[1]) / 2.0,
               - (bounds[2] + bounds[3]) / 2.0,
               - (bounds[4] + bounds[5]) / 2.0
            ));

            canvas.getTransformationsStack().push(transformation);
            askAcceptVisitor(axes.getChildren());
            canvas.getTransformationsStack().pop();
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
            colorMap = figure.getColorMap();
            int background = figure.getBackground();
            IColor clearColor = new FinalColor(colorMap.getScilabColor(background));

            canvas.setCoordinate(new FinalRectangle(.5,.5,1,1));
            canvas.getZoneControl().setCurrentZone(new FinalRectangle(.5,.5,1,1));
            canvas.getZoneControl().clearZone(clearColor);
            canvas.getZoneControl().clearZBuffer();

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
        /*
        ISciBuffersManager buffersManager = canvas.getBuffersManager();
        IVertexBuffer vertexBuffer = buffersManager.getVertexBuffer(polyline);
        */

        IVertexBuffer vertexBuffer = dataManager.getVertexBuffer(polyline.getIdentifier());
        ILines lines = new Lines();
        lines.setVertexBuffer(vertexBuffer);
        
        IColor frontColor = new FinalColor(colorMap.getScilabColor(polyline.getLineColor()));
        float lineWidth = polyline.getLineThickness().floatValue();
        short pattern = polyline.getLineStyleAsEnum().asPattern();

        lines.setMode(ILines.EMode.STRIP);
        lines.getLineProperties().setLineWidth(lineWidth);
        lines.getLineProperties().setLinePattern(pattern);
        lines.getLineProperties().getMaterial().setDiffuseColor(frontColor);

        canvas.draw(lines);
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
        // TODO
        System.out.println("How can I draw a text ?");
    }

    @Override
    public void visit(TextObject textObject) {
        // TODO
        System.out.println("How can I draw a textObject ?");
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
