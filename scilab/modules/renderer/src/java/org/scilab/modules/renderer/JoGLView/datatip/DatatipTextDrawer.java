/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.renderer.JoGLView.datatip;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;
import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureManager;
import org.scilab.forge.scirenderer.tranformations.DegenerateMatrixException;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.PolylineData;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.datatip.Datatip;
import org.scilab.modules.renderer.CallRenderer;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.renderer.JoGLView.util.ScaleUtils;
import java.awt.Dimension;
import org.scilab.modules.renderer.JoGLView.text.TextManager;

import java.nio.FloatBuffer;

/**
 * Datatip text drawer
 *
 * Draw the datatip text according to its orientation and mark properties
 */

public class DatatipTextDrawer extends TextManager {

    ElementsBuffer lineBuffer;
    Canvas canvas;
    public DatatipTextDrawer(Canvas canvas) {
        super(canvas.getTextureManager());
        this.canvas = canvas;
        lineBuffer = canvas.getBuffersManager().createElementsBuffer();
    }

    @Override
    public void disposeAll() {
        super.disposeAll();
        canvas.getBuffersManager().dispose(lineBuffer);
    }

    /**
     * Draw the given Scilab {@see Datatip} with the given {@see DrawingTools}.
     * @param drawingTools the given {@see DrawingTools}.
     * @param colorMap the current {@see ColorMap}
     * @param text the given Scilab {@see Datatip}
     * @throws SciRendererException if the draw fails.
     */
    public final void draw(final DrawingTools drawingTools, final ColorMap colorMap, final Datatip datatip) throws SciRendererException {
        Texture texture = getTexture(colorMap, datatip);

        /* The unscaled texture's dimensions */
        Dimension spriteDims = getSpriteDims(colorMap, datatip);

        Transformation projection = drawingTools.getTransformationManager().getCanvasProjection();

        Integer parentAxesId = datatip.getParentAxes();
        Axes parentAxes = (Axes) GraphicController.getController().getObjectFromId(parentAxesId);

        /* Compute the text box vectors and the text box to texture dimension ratios */
        Vector3d[] textBoxVectors = computeTextBoxVectors(projection, datatip, texture.getDataProvider().getTextureSize(), parentAxes);

        double[] ratios = computeRatios(projection, datatip, textBoxVectors, texture.getDataProvider().getTextureSize(), spriteDims);

        /* If text box mode is equal to filled, the texture must be updated */
        if (datatip.getTextBoxMode() == 2 && ratios[0] != 1.0) {
            texture = updateSprite(colorMap, datatip, ratios[0], ratios[1]);
        }

        /* Compute the text texture's actual position, which depends on the object's text box mode property */
        Vector3d[] cornerPositions = computeTextPosition(projection, datatip, textBoxVectors, texture.getDataProvider().getTextureSize());

        /* Draw in window coordinates */
        drawingTools.getTransformationManager().useWindowCoordinate();

        Integer size = datatip.getMarkSize();
        Integer unit = datatip.getMarkSizeUnit();

        /* calculate the size of the mark to dont draw the text over the mark*/
        double finalSize = (unit == 1) ? (8.0 + 2.0 * size) : size;
        finalSize /= 2.0;
        double r = datatip.getMarkStyle() == 11 ? 1.0 : 2.0;
        finalSize -= (finalSize >= 2.0) ? r : 0.0;

        if (!datatip.getDetachedMode()) {
            Vector3d delta = new Vector3d(finalSize, finalSize, 0);
            /* set up the text position according to the datatip orientation*/
            if (datatip.isAutoOrientationEnabled()) {
                int autopos = getAutoOrientation(datatip);
                if (autopos != -1) {
                    Vector3d cp = cornerPositions[0], d = delta, p;
                    if (autopos == 2 || autopos == 3) {
                        cp = cp.minus(textBoxVectors[1]);
                        d = d.setY(-finalSize);
                    }
                    if (autopos == 0 || autopos == 2) {
                        cp = cp.minus(textBoxVectors[0]);
                        d = d.setX(-finalSize);
                    }

                    p = projection.unproject(cp.plus(textBoxVectors[0]).plus(textBoxVectors[1]));
                    Vector3d ucp = projection.unproject(cp);
                    if (p.getX() < -1 || p.getX() > 1 || p.getY() < -1 || p.getY() > 1 || ucp.getX() < -1 || ucp.getX() > 1 || ucp.getY() < -1 || ucp.getY() > 1) {
                        autopos = -1;
                    } else {
                        cornerPositions[0] = cp;
                        delta = d;
                    }
                }

                if (autopos == -1) {
                    Vector3d position = projection.unproject(cornerPositions[0].minus(textBoxVectors[0]).plus(textBoxVectors[1]));
                    if (position.getX() >= -1 && position.getX() <= 1 && position.getY() >= -1 && position.getY() <= 1) {
                        cornerPositions[0] = cornerPositions[0].minus(textBoxVectors[0]);
                        delta = delta.setX(-finalSize);
                    } else {
                        position = projection.unproject(cornerPositions[0].plus(textBoxVectors[0]).minus(textBoxVectors[1]));
                        if (position.getX() >= -1 && position.getX() <= 1 && position.getY() >= -1 && position.getY() <= 1) {
                            cornerPositions[0] = cornerPositions[0].minus(textBoxVectors[1]);
                            delta = delta.setY(-finalSize);
                        } else {
                            position = projection.unproject(cornerPositions[0].minus(textBoxVectors[0]).minus(textBoxVectors[1]));
                            if (position.getX() >= -1 && position.getX() <= 1 && position.getY() >= -1 && position.getY() <= 1) {
                                cornerPositions[0] = cornerPositions[0].minus(textBoxVectors[1]);
                                cornerPositions[0] = cornerPositions[0].minus(textBoxVectors[0]);
                                delta = delta.setX(-finalSize);
                                delta = delta.setY(-finalSize);
                            }
                        }
                    }
                }
            } else {
                if (datatip.getOrientation() == 2 || datatip.getOrientation() == 3) {
                    cornerPositions[0] = cornerPositions[0].minus(textBoxVectors[1]);
                    delta = delta.setY(-finalSize);
                }
                if (datatip.getOrientation() == 0 || datatip.getOrientation() == 2) {
                    cornerPositions[0] = cornerPositions[0].minus(textBoxVectors[0]);
                    delta = delta.setX(-finalSize);
                }
                if (datatip.getOrientation() == 4) {
                    cornerPositions[0] = cornerPositions[0].minus(textBoxVectors[0]);
                    cornerPositions[0] = cornerPositions[0].minus(textBoxVectors[1].times(0.5));
                    delta = delta.setY(0);
                    delta = delta.setX(Math.sqrt(2) * (-finalSize));
                }
                if (datatip.getOrientation() == 5) {
                    cornerPositions[0] = cornerPositions[0].minus(textBoxVectors[1].times(0.5));
                    delta = delta.setY(0);
                    delta = delta.setX(Math.sqrt(2) * finalSize);
                }
                if (datatip.getOrientation() == 6) {
                    cornerPositions[0] = cornerPositions[0].minus(textBoxVectors[0].times(0.5));
                    delta = delta.setX(0);
                    delta = delta.setY(Math.sqrt(2) * finalSize);
                }
                if (datatip.getOrientation() == 7) {
                    cornerPositions[0] = cornerPositions[0].minus(textBoxVectors[0].times(0.5));
                    cornerPositions[0] = cornerPositions[0].minus(textBoxVectors[1]);
                    delta = delta.setX(0);
                    delta = delta.setY(Math.sqrt(2) * (-finalSize));
                }
            }

            cornerPositions[0] = cornerPositions[0].plus(delta);
            cornerPositions[1] = cornerPositions[1].plus(delta);
        }
        Vector3d p = cornerPositions[0];

        if (datatip.getDetachedMode()) {
            p = projection.project(calculateDetachedPoint(datatip));
            drawDetachedLine(drawingTools, colorMap, datatip, calculateDetachedLine(p, cornerPositions, textBoxVectors, finalSize));
            Vector3d diff = cornerPositions[1].minus(cornerPositions[0]);
            cornerPositions[0] = p;
            cornerPositions[1] = p.plus(diff);
        }
        /* The Text object's rotation direction convention is opposite to the standard one, its angle is expressed in radians. */
        drawingTools.draw(texture, AnchorPosition.LOWER_LEFT, p, -180.0 * datatip.getFontAngle() / Math.PI);
        drawingTools.getTransformationManager().useSceneCoordinate();



        /* Compute the corners of the text's bounding box in window coordinates */
        Vector3d[] projCorners;
        if (datatip.getTextBoxMode() == 2) {
            projCorners = computeProjTextBoxCorners(cornerPositions[1], datatip.getFontAngle(), textBoxVectors);
        } else {
            projCorners = computeProjCorners(cornerPositions[0], datatip.getFontAngle(), texture.getDataProvider().getTextureSize());
        }

        Vector3d[] corners = computeCorners(projection, projCorners, parentAxes);
        Double[] coordinates = cornersToCoordinateArray(corners);

        /* Set the computed coordinates */
        datatip.setCorners(coordinates);
    }

    public void drawDetachedLine(final DrawingTools drawingTools, final ColorMap colorMap, final Datatip datatip, Vector3d[] pos) throws SciRendererException {
        DefaultGeometry geom = new DefaultGeometry();

        geom.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
        geom.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
        geom.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);
        Appearance appearance = new Appearance();
        appearance.setLineColor(ColorFactory.createColor(colorMap, datatip.getLine().getColor()));

        appearance.setLinePattern(datatip.getLineStyleAsEnum().asPattern());

        FloatBuffer vertices = FloatBuffer.allocate(6);
        vertices.put(0, (float)pos[0].getX());
        vertices.put(1, (float)pos[0].getY());
        vertices.put(2, (float)pos[0].getZ());

        vertices.put(3, (float)pos[1].getX());
        vertices.put(4, (float)pos[1].getY());
        vertices.put(5, (float)pos[1].getZ());
        lineBuffer.setData(vertices, 3);
        geom.setVertices(lineBuffer);

        /* Draws in window coordinates */
        //visitor.getDrawingTools().getTransformationManager().useWindowCoordinate();
        drawingTools.draw(geom, appearance);
        //visitor.getDrawingTools().getTransformationManager().useSceneCoordinate();
    }

    private Vector3d[] calculateDetachedLine(Vector3d pos, Vector3d[] corners, Vector3d[] textBox, double halfMarkSize) {
        Vector3d p1 = pos;
        Vector3d p0 = corners[0];
        double dx = halfMarkSize;
        double dy = halfMarkSize;

        if (p1.getX() < corners[0].getX()) {
            p1 = p1.plus(textBox[0]);
            dx = -halfMarkSize;
        }
        if (p1.getY() < corners[0].getY()) {
            p1 = p1.plus(textBox[1]);
            dy = -halfMarkSize;
        }
        p0 = p0.plus(new Vector3d(new double[] {dx, dy, 0.0}));
        return new Vector3d[] {p0, p1};
    }

    /**
     * Update the given datatip text corners
     * @param datatip the given datatip
     */
    public static void updateTextCorners(Datatip datatip) {
        Vector3d[] projCorners = null;
        DrawerVisitor currentVisitor = DrawerVisitor.getVisitor(datatip.getParentFrameOrFigure());
        Transformation currentProj = currentVisitor.getAxesDrawer().getProjection(datatip.getParentAxes());
        Axes parentAxes = (Axes) GraphicController.getController().getObjectFromId(datatip.getParentAxes());
        Dimension spriteDim = currentVisitor.getDatatipTextDrawer().getSpriteDims(currentVisitor.getColorMap(), datatip);

        /* Compute the corners */
        try {
            Vector3d[] textBoxVectors = currentVisitor.getDatatipTextDrawer().computeTextBoxVectors(currentProj, datatip, spriteDim, parentAxes);
            Vector3d[] cornerPositions = currentVisitor.getDatatipTextDrawer().computeTextPosition(currentProj, datatip, textBoxVectors, spriteDim);

            Integer size = datatip.getMarkSize();
            Integer unit = datatip.getMarkSizeUnit();

            /* calculate the size of the mark to dont position the text over the mark*/
            double finalSize = (unit == 1) ? (8.0 + 2.0 * size) : size;
            finalSize /= 2.0;
            double r = datatip.getMarkStyle() == 11 ? 1.0 : 2.0;
            finalSize -= (finalSize >= 2.0) ? r : 0.0;

            Vector3d delta = new Vector3d(finalSize, finalSize, 0);
            /* set up the text position according to the datatip orientation*/
            if (datatip.getOrientation() == 2 || datatip.getOrientation() == 3) {
                cornerPositions[0] = cornerPositions[0].minus(textBoxVectors[1]);
                delta = delta.setY(-finalSize);
            }
            if (datatip.getOrientation() == 0 || datatip.getOrientation() == 2) {
                cornerPositions[0] = cornerPositions[0].minus(textBoxVectors[0]);
                delta = delta.setX(-finalSize);
            }
            if (datatip.getOrientation() == 4) {
                cornerPositions[0] = cornerPositions[0].minus(textBoxVectors[0]);
                cornerPositions[0] = cornerPositions[0].minus(textBoxVectors[1].times(0.5));
                delta = delta.setY(0);
                delta = delta.setX(Math.sqrt(2) * (-finalSize));
            }
            if (datatip.getOrientation() == 5) {
                cornerPositions[0] = cornerPositions[0].minus(textBoxVectors[1].times(0.5));
                delta = delta.setY(0);
                delta = delta.setX(Math.sqrt(2) * finalSize);
            }
            if (datatip.getOrientation() == 6) {
                cornerPositions[0] = cornerPositions[0].minus(textBoxVectors[0].times(0.5));
                delta = delta.setX(0);
                delta = delta.setY(Math.sqrt(2) * finalSize);
            }
            if (datatip.getOrientation() == 7) {
                cornerPositions[0] = cornerPositions[0].minus(textBoxVectors[0].times(0.5));
                cornerPositions[0] = cornerPositions[0].minus(textBoxVectors[1]);
                delta = delta.setX(0);
                delta = delta.setY(Math.sqrt(2) * (-finalSize));
            }

            cornerPositions[0] = cornerPositions[0].plus(delta);
            cornerPositions[1] = cornerPositions[1].plus(delta);

            if (datatip.getTextBoxMode() == 2) {
                projCorners = currentVisitor.getDatatipTextDrawer().computeProjTextBoxCorners(cornerPositions[1], datatip.getFontAngle(), textBoxVectors);
            } else {
                projCorners = currentVisitor.getDatatipTextDrawer().computeProjCorners(cornerPositions[0], datatip.getFontAngle(), spriteDim);
            }
        } catch (DegenerateMatrixException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        Vector3d[] corners = currentVisitor.getDatatipTextDrawer().computeCorners(currentProj, projCorners, parentAxes);
        Double[] coordinates = currentVisitor.getDatatipTextDrawer().cornersToCoordinateArray(corners);

        /* Set the computed coordinates */
        datatip.setCorners(coordinates);
    }

    /**
     * Calculates the anchor point from datatip (Used to draw the datatip mark)
     * @param datatip the given datatip
     * @return Vector3d the anchor point position
     */
    public static Vector3d calculateAnchorPoint(Datatip datatip) {
        Axes axes = (Axes) GraphicController.getController().getObjectFromId(datatip.getParentAxes());
        double[][] factors = axes.getScaleTranslateFactors();
        boolean[] logFlags = new boolean[] {axes.getXAxisLogFlag(), axes.getYAxisLogFlag(), axes.getZAxisLogFlag()};
        Vector3d v = ScaleUtils.applyLogScale(new Vector3d(datatip.getTipData()), logFlags);

        return new Vector3d(v.getX() * factors[0][0] + factors[1][0], v.getY() * factors[0][1] + factors[1][1], v.getZ() * factors[0][2] + factors[1][2]);
    }

    public static Vector3d calculateDetachedPoint(Datatip datatip) {
        Axes axes = (Axes) GraphicController.getController().getObjectFromId(datatip.getParentAxes());
        double[][] factors = axes.getScaleTranslateFactors();
        boolean[] logFlags = new boolean[] {axes.getXAxisLogFlag(), axes.getYAxisLogFlag(), axes.getZAxisLogFlag()};
        Double[] dp = datatip.getDetachedPosition();
        Vector3d v = ScaleUtils.applyLogScale(new Vector3d(dp[0], dp[1], dp[2]), logFlags);

        return new Vector3d(v.getX() * factors[0][0] + factors[1][0], v.getY() * factors[0][1] + factors[1][1], v.getZ() * factors[0][2] + factors[1][2]);
    }

    private static int getAutoOrientation(Datatip datatip) {
        final double[] dataX = (double[]) PolylineData.getDataX(datatip.getParent());
        int index = datatip.getIndexes();
        if (index == 0 || index >= dataX.length - 1) {
            return -1;
        }

        final double[] dataY = (double[]) PolylineData.getDataY(datatip.getParent());
        final double[] first, second, third;
        Integer axes = datatip.getParentAxes();

        if (PolylineData.isZCoordSet(datatip.getParent()) != 0) {
            final double[] dataZ = (double[]) PolylineData.getDataZ(datatip.getParent());
            first = CallRenderer.getPixelFrom3dCoordinates(axes, new double[] {dataX[index - 1], dataY[index - 1], dataZ[index - 1]});
            second = CallRenderer.getPixelFrom3dCoordinates(axes, new double[] {dataX[index], dataY[index], dataZ[index]});
            third = CallRenderer.getPixelFrom3dCoordinates(axes, new double[] {dataX[index + 1], dataY[index + 1], dataZ[index + 1]});
        } else {
            first = CallRenderer.getPixelFrom3dCoordinates(axes, new double[] {dataX[index - 1], dataY[index - 1], 0});
            second = CallRenderer.getPixelFrom3dCoordinates(axes, new double[] {dataX[index], dataY[index], 0});
            third = CallRenderer.getPixelFrom3dCoordinates(axes, new double[] {dataX[index + 1], dataY[index + 1], 0});
        }

        final double a = Math.atan2(second[1] - first[1], first[0] - second[0]);
        final double b = Math.atan2(second[1] - third[1], third[0] - second[0]);

        final int quadA = getQuad(a);
        final int quadB = getQuad(b);
        final int quadDatatip;

        // UL quadrant is 1, UR quadrant is 0
        // LL quadrant is 2, LR quadrant is 3

        if (quadA == quadB) {
            quadDatatip = (quadA + 2) % 4;
        } else {
            final int sum = quadA + quadB;
            if (sum == 3) {
                quadDatatip = (quadA * quadB + 1) % 3;
            } else if (sum == 1 || sum == 5) {
                quadDatatip = (sum + 3) % 8;
            } else {
                final double M = Math.max(a, b);
                final double m = Math.min(a, b);
                if (sum == 4) {
                    if (M - m <= Math.PI) {
                        quadDatatip = 2;
                    } else {
                        quadDatatip = 0;
                    }
                } else {
                    if (M - m <= Math.PI) {
                        quadDatatip = 1;
                    } else {
                        quadDatatip = 3;
                    }
                }
            }
        }

        if (quadDatatip <= 1) {
            return 1 - quadDatatip;
        }

        return quadDatatip;
    }

    private static int getQuad(final double a) {
        if (a >= 0) {
            if (a <= Math.PI / 2) {
                return 0;
            } else {
                return 1;
            }
        } else {
            if (a <= -Math.PI / 2) {
                return 2;
            } else {
                return 3;
            }
        }
    }
}
