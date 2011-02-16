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
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.shapes.geometry.GeometryImpl;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.TransformationStack;
import org.scilab.forge.scirenderer.utils.shapes.geometry.CubeFactory;
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
public class DrawerVisitor implements IVisitor, Drawer {
    private static final double DEFAULT_THETA = 270.0;
    private static final int ELEMENT_SIZE = 4;


    private final Geometry cubeGeometry;
    private final Geometry boxBorderGeometry;
    private final Geometry frontBoxBorderGeometry;
    private final Geometry hiddenBoxBorderGeometry;

    private final ElementsBuffer boxBorderVertices;
    private static final float[] boxBorderVerticesData = new float[] {
        -1,-1,+1,1, -1,+1,+1,1,
        -1,+1,-1,1, +1,+1,-1,1,
        +1,-1,-1,1, +1,-1,+1,1
    };

    private static final Line.LineType HIDDEN_BORDER_PATTERN = Line.LineType.STYLE1;
    private final ElementsBuffer hiddenBoxBorderVertices;
    private static final float[] hiddenBoxBorderVerticesData = new float[] {
        -1,-1,-1,1, +1,-1,-1,1,
        -1,-1,-1,1, -1,+1,-1,1,
        -1,-1,-1,1, -1,-1,+1,1
    };

    private final ElementsBuffer frontBoxBorderVertices;
    private static final float[] frontBoxBorderVerticesData = new float[] {
        +1,+1,+1,1, -1,+1,+1,1,
        +1,+1,+1,1, +1,-1,+1,1,
        +1,+1,+1,1, +1,+1,-1,1
    };

    private final Canvas canvas;
    private final Figure figure;
    private final DataManager dataManager;

    private ColorMap colorMap;
    private DrawingTools drawingTools = null;

    public DrawerVisitor(Canvas canvas, Figure figure) {
        this.canvas = canvas;
        this.figure = figure;
        this.dataManager = new DataManager(canvas);

        boxBorderVertices = canvas.getBuffersManager().createElementsBuffer();
        boxBorderVertices.setData(boxBorderVerticesData, ELEMENT_SIZE);

        hiddenBoxBorderVertices = canvas.getBuffersManager().createElementsBuffer();
        hiddenBoxBorderVertices.setData(hiddenBoxBorderVerticesData, ELEMENT_SIZE);

        frontBoxBorderVertices = canvas.getBuffersManager().createElementsBuffer();
        frontBoxBorderVertices.setData(frontBoxBorderVerticesData, ELEMENT_SIZE);

        cubeGeometry = CubeFactory.createCube(canvas);
        hiddenBoxBorderGeometry = new GeometryImpl(Geometry.DrawingMode.SEGMENTS, hiddenBoxBorderVertices);
        frontBoxBorderGeometry  = new GeometryImpl(Geometry.DrawingMode.SEGMENTS, frontBoxBorderVertices);
        boxBorderGeometry       = new GeometryImpl(Geometry.DrawingMode.SEGMENTS_LOOP, boxBorderVertices);
    }

    @Override
    public void draw(DrawingTools drawingTools) {
        this.drawingTools = drawingTools;
        figure.accept(this);
    }

    @Override
    public void finalize() throws Throwable {
        super.finalize();
        canvas.getBuffersManager().dispose(boxBorderVertices);
        canvas.getBuffersManager().dispose(hiddenBoxBorderVertices);
        canvas.getBuffersManager().dispose(frontBoxBorderVertices);
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
        TransformationStack projectionStack = drawingTools.getTransformationManager().getModelViewStack();
        if (axes.getVisible()) {
            // Set current zone
            /**********************************************************************************************************/
            Double[] axesBounds = axes.getAxesBounds();
            Double[] margins = axes.getMargins();

            double x = (axesBounds[0] + axesBounds[2] * margins[0])*2 - 1;
            double y = (1.0 - axesBounds[1] - axesBounds[3] * (1.0 - margins[3]))*2 - 1;
            double w = (1 - margins[0] - margins[1]) * axesBounds[2];
            double h = (1 - margins[2] - margins[3]) * axesBounds[3];

            Transformation zoneTranslation = TransformationFactory.getTranslateTransformation(x+w, y+h, 0);
            Transformation zoneScale = TransformationFactory.getScaleTransformation(w, h, 1);
            drawingTools.getTransformationManager().getProjectionStack().push(zoneTranslation.rightTimes(zoneScale));
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
            
            Transformation transformation;

            // Set zone aspect ratio.
            // TODO double ratio = canvas.getFinalAspectRatio();
            double canvasRatio = (double) canvas.getWidth() / (double) canvas.getHeight();
            double axesRatio = w / h;
            double ratio = canvasRatio * axesRatio;
            if (ratio < 1.0) {
                transformation = TransformationFactory.getScaleTransformation(1, ratio, 1);
            } else {
                transformation = TransformationFactory.getScaleTransformation(1/ratio, 1, 1);
            }

            // Rotate.
            Transformation alphaRotation = TransformationFactory.getRotationTransformation(-axes.getRotationAngles()[0], 1.0, 0.0, 0.0);
            transformation = transformation.rightTimes(alphaRotation);

            Transformation thetaRotation = TransformationFactory.getRotationTransformation(DEFAULT_THETA - axes.getRotationAngles()[1], 0.0, 0.0, 1.0);
            transformation = transformation.rightTimes(thetaRotation);

            // If there is no cube scaling, we must take into account the distribution of data.
            if (!axes.getCubeScaling()) {
                tmpX = (bounds[1] - bounds[0]);
                tmpY = (bounds[3] - bounds[2]);
                tmpZ = (bounds[5] - bounds[4]);

                /**
                 * Here, we should divide the values by their maximum.
                 * But the next operation will automatically.  
                 */
                Transformation cubeScale = TransformationFactory.getScaleTransformation(tmpX, tmpY, tmpZ);
                transformation = transformation.rightTimes(cubeScale);
            }

            // Compute bounds of projected data.
            double matrix[] = transformation.getMatrix();
            tmpX = 1 / (Math.abs(matrix[0]) + Math.abs(matrix[4]) + Math.abs(matrix[8]));
            tmpY = 1 / (Math.abs(matrix[1]) + Math.abs(matrix[5]) + Math.abs(matrix[9]));
            tmpZ = 1 / (Math.abs(matrix[2]) + Math.abs(matrix[6]) + Math.abs(matrix[10]));

            // Scale projected data to fit in the cube.
            Transformation isoScale;
            if (axes.getIsoview()) {
                double minScale = Math.min(tmpX, tmpY);
                isoScale = TransformationFactory.getScaleTransformation(minScale, minScale, tmpZ);
            } else {
                isoScale = TransformationFactory.getScaleTransformation(tmpX, tmpY, tmpZ);
            }
            transformation = transformation.leftTimes(isoScale);

            projectionStack.pushRightMultiply(transformation);

            // Draw box.
            /**********************************************************************************************************/
            drawingTools.clearDepthBuffer();
            Appearance appearance = new Appearance();
            appearance.setFillColor(new Color(colorMap.getScilabColor(axes.getBackground())));
            drawingTools.draw(cubeGeometry, appearance);
            drawingTools.clearDepthBuffer();

            // Draw box border.
            /**********************************************************************************************************/
            Box.BoxType boxed = axes.getBox().getBox();
            if (boxed != Box.BoxType.OFF) {
                /**
                 * Mirror the cube such that the corner with the maximum Z value was (-1, -1, -1).
                 */
                Transformation cubeOrientationScale = TransformationFactory.getScaleTransformation(
                        matrix[2] < 0 ? 1 : -1,
                        matrix[6] < 0 ? 1 : -1,
                        matrix[10] < 0 ? 1 : -1
                );
                projectionStack.pushRightMultiply(cubeOrientationScale);

                /**
                 * Draw hidden box border.
                 */
                appearance.setLineColor(new Color(colorMap.getScilabColor(axes.getHiddenAxisColor())));
                appearance.setLineWidth(axes.getLineThickness().floatValue());
                appearance.setLinePattern(HIDDEN_BORDER_PATTERN.asPattern());
                drawingTools.draw(hiddenBoxBorderGeometry, appearance);


                if (boxed != Box.BoxType.HIDDEN_AXES) {

                    /**
                     * Draw box border.
                     */
                    appearance.setLineColor(new Color(colorMap.getScilabColor(axes.getLineColor())));
                    appearance.setLineWidth(axes.getLineThickness().floatValue());
                    appearance.setLinePattern(axes.getLine().getLineStyle().asPattern());
                    drawingTools.draw(boxBorderGeometry, appearance);


                    if (boxed != Box.BoxType.BACK_HALF) {
                        /**
                         * Draw front box border.
                         * Draw front box border.
                         */
                        drawingTools.draw(frontBoxBorderGeometry, appearance);
                    }
                }

                projectionStack.pop();
            }
            /**********************************************************************************************************/

            // Draw data.
            /**********************************************************************************************************/

            projectionStack.pop();

            // Reverse data if needed.
            Transformation reverseAxesScale = TransformationFactory.getScaleTransformation(
                    axes.getAxes()[0].getReverse() ? -1 : 1,
                    axes.getAxes()[1].getReverse() ? -1 : 1,
                    axes.getAxes()[2].getReverse() ? -1 : 1
            );
            transformation = transformation.rightTimes(reverseAxesScale);


            // Scale data.
            Transformation scaleTransformation = TransformationFactory.getScaleTransformation(
                    2/(bounds[1] - bounds[0]),
                    2/(bounds[3] - bounds[2]),
                    2/(bounds[5] - bounds[4])
            );
            transformation = transformation.rightTimes(scaleTransformation);


            // Translate data.
            Transformation translateTransformation = TransformationFactory.getTranslateTransformation(
                    -(bounds[0] + bounds[1]) / 2.0,
                    -(bounds[2] + bounds[3]) / 2.0,
                    -(bounds[4] + bounds[5]) / 2.0
            );
            transformation = transformation.rightTimes(translateTransformation);

            projectionStack.push(transformation);
            askAcceptVisitor(axes.getChildren());
            projectionStack.pop();

            drawingTools.getTransformationManager().getProjectionStack().pop();
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
             * Set the used color map.
             */
            colorMap = figure.getColorMap();

            drawingTools.clear(new Color(colorMap.getScilabColor(figure.getBackground())));
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
        Geometry geometry = new GeometryImpl(
                Geometry.DrawingMode.SEGMENTS_STRIP,
                dataManager.getVertexBuffer(polyline.getIdentifier())
        );

        Appearance appearance = new Appearance();

        appearance.setLineColor(new Color(colorMap.getScilabColor(polyline.getLineColor())));
        appearance.setLineWidth(polyline.getLineThickness().floatValue());
        appearance.setLinePattern(polyline.getLineStyleAsEnum().asPattern());

        drawingTools.draw(geometry, appearance);
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
