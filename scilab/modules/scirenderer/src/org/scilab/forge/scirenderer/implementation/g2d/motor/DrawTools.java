/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte Denizet
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

package org.scilab.forge.scirenderer.implementation.g2d.motor;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.GradientPaint;
import java.awt.Graphics2D;
import java.awt.LinearGradientPaint;
import java.awt.Paint;
import java.awt.RenderingHints;
import java.awt.Shape;
import java.awt.Stroke;
import java.awt.geom.AffineTransform;
import java.awt.geom.Area;
import java.awt.geom.NoninvertibleTransformException;
import java.awt.geom.Path2D;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferInt;

import java.awt.geom.Point2D;
import java.awt.MultipleGradientPaint;

/**
 * @author Calixte DENIZET
 */
public final class DrawTools {

    private static final Stroke stroke = new BasicStroke(1.0f, BasicStroke.CAP_BUTT, BasicStroke.JOIN_BEVEL);
    private static final Stroke EMPTYSTROKE = new BasicStroke(0f, BasicStroke.CAP_BUTT, BasicStroke.JOIN_BEVEL);
    private static final Color TRANSLUCENT_BLACK = new Color(0, 0, 0, 0);

    /**
     * Fill a triangle in using a Gouraud shading
     * Only two gradient are used rather than three.
     * @param g2d the Graphics2D where to draw
     * @param t the Triangle to fill
     */
    public static final void fillGouraud(final Graphics2D g2d, final Triangle t) {
        Path2D contour = t.getProjectedContour();
        double[] v0 = new double[] {t.vertices[0].getX(), t.vertices[0].getY()};
        double[] v1 = new double[] {t.vertices[1].getX(), t.vertices[1].getY()};
        double[] v2 = new double[] {t.vertices[2].getX(), t.vertices[2].getY()};
        double[] pv0 = get2DProjection(v0[0], v0[1], v1[0], v1[1], v2[0], v2[1]);
        double[] pv1 = get2DProjection(v1[0], v1[1], v0[0], v0[1], v2[0], v2[1]);
        double[] pv2 = get2DProjection(v2[0], v2[1], v0[0], v0[1], v1[0], v1[1]);

        Paint oldPaint = g2d.getPaint();
        Area area = new Area(contour);
        area.add(new Area(stroke.createStrokedShape(contour)));


        g2d.setColor(t.getColor(0));
        g2d.fill(area);


        float[] col = t.getColor(1).getComponents(null);
        GradientPaint gp = new GradientPaint((float) v1[0], (float) v1[1], t.getColor(1), (float) pv1[0], (float) pv1[1], new Color(col[0], col[1], col[2], 0.0f));
        g2d.setPaint(gp);
        g2d.fill(area);


        col = t.getColor(2).getComponents(null);
        gp = new GradientPaint((float) v2[0], (float) v2[1], t.getColor(2), (float) pv2[0], (float) pv2[1], new Color(col[0], col[1], col[2], 0.0f));
        g2d.setPaint(gp);
        g2d.fill(area);

        g2d.setPaint(oldPaint);
    }

    /**
     * Draw a texture (ie a BufferedImage) in a triangle
     * @param g2d the Graphics2D where to draw
     * @param image the texture to apply
     * @param ximg the x-coordinates of the triangle to use in the texture
     * @param yimg the y-coordinates of the triangle to use in the texture
     * @param xdest the x-coordinates of the destination triangle
     * @param ydest the y-coordinates of the destination triangle
     * @param key the rendering hint to use for interpolation
     */
    public static final void drawTriangleTexture(final Graphics2D g2d, final BufferedImage image, final double[] ximg, final double[] yimg, final double[] xdest, final double[] ydest, Object key) {
        try {
            double w = image.getWidth();
            double h = image.getHeight();

            Path2D.Double path = new Path2D.Double();
            path.moveTo(xdest[0], ydest[0]);
            path.lineTo(xdest[1], ydest[1]);
            path.lineTo(xdest[2], ydest[2]);
            path.closePath();
            Area area = new Area(path);
            area.add(new Area(stroke.createStrokedShape(path)));

            boolean is1d = is1d(ximg, yimg);

            // if we have a 1D texture we must slighlty modified the coordinates to use the algorithm below.
            if (checkSourceCoordinates(ximg, yimg)) {
                // three coordinates are the same in 1D texture
                int[] pixels = ((DataBufferInt) image.getRaster().getDataBuffer()).getData();
                int index = (int) Math.floor(w * ximg[0]);
                Color color;
                if (index >= pixels.length) {
                    color = new Color(pixels[pixels.length - 1]);
                } else if (index < 0) {
                    color = new Color(pixels[0]);
                } else {
                    color = new Color(pixels[index]);
                }
                //color = Color.PINK;

                g2d.setColor(color);
                g2d.fill(area);
                return;
            }

            AffineTransform translationDest = AffineTransform.getTranslateInstance(xdest[0], ydest[0]);
            AffineTransform translationImg = AffineTransform.getTranslateInstance(-w * ximg[0], -h * yimg[0]);

            AffineTransform toDest = new AffineTransform(xdest[1] - xdest[0], ydest[1] - ydest[0], xdest[2] - xdest[0], ydest[2] - ydest[0], 0, 0);
            AffineTransform fromImg = new AffineTransform(w * (ximg[1] - ximg[0]), h * (yimg[1] - yimg[0]), w * (ximg[2] - ximg[0]), h * (yimg[2] - yimg[0]), 0, 0).createInverse();

            AffineTransform transformation = new AffineTransform();
            transformation.concatenate(translationDest);
            transformation.concatenate(toDest);
            transformation.concatenate(fromImg);
            transformation.concatenate(translationImg);

            AffineTransform oldTransform = g2d.getTransform();

            // For now we don't enter in this
            // SVGGraphics2D doesn't handle MultipleGradient :(
            if (false && is1d && key == RenderingHints.VALUE_INTERPOLATION_BILINEAR) {
                int[] pixels = ((DataBufferInt) image.getRaster().getDataBuffer()).getData();
                float[] fractions = new float[pixels.length];
                for (int i = 0; i < fractions.length; i++) {
                    fractions[i] = (((float) i) / (fractions.length - 1));
                }

                Color[] colors = new Color[pixels.length];
                for (int i = 0; i < colors.length; i++) {
                    colors[i] = new Color(pixels[i]);
                }
                LinearGradientPaint gradient = new LinearGradientPaint(new Point2D.Double(0, 0), new Point2D.Double(pixels.length, 0), fractions, colors, MultipleGradientPaint.CycleMethod.NO_CYCLE,  MultipleGradientPaint.ColorSpaceType.SRGB, transformation);
                Shape oldClip = g2d.getClip();
                g2d.clip(area);
                g2d.setPaint(gradient);
                g2d.fill(area);
                g2d.setClip(oldClip);
            } else {
                clamp(g2d, ximg, yimg, xdest, ydest, transformation, image);
                Object oldKey = g2d.getRenderingHint(RenderingHints.KEY_INTERPOLATION);
                g2d.setRenderingHint(RenderingHints.KEY_INTERPOLATION, key);
                g2d.setStroke(EMPTYSTROKE);
                Shape oldClip = g2d.getClip();
                g2d.clip(area);
                g2d.drawImage(image, transformation, null);
                g2d.setClip(oldClip);
                if (oldKey != null) {
                    g2d.setRenderingHint(RenderingHints.KEY_INTERPOLATION, oldKey);
                }
            }

            g2d.setTransform(oldTransform);
        } catch (NoninvertibleTransformException e) {
            System.err.println(e);
        }
    }

    /**
     * Draw a texture (ie a BufferedImage) in a parallelogram
     * @param g2d the Graphics2D where to draw
     * @param image the texture to apply
     * @param ximg the x-coordinates of the parallelogram to use in the texture
     * @param yimg the y-coordinates of the parallelogram to use in the texture
     * @param xdest the x-coordinates of the destination parallelogram
     * @param ydest the y-coordinates of the destination parallelogram
     * @param key the rendering hint to use for interpolation
     */
    public static final void drawParallelogramTexture(final Graphics2D g2d, final BufferedImage image, final double[] ximg, final double[] yimg, final double[] xdest, final double[] ydest, Object key) {
        try {
            Object oldKey = g2d.getRenderingHint(RenderingHints.KEY_INTERPOLATION);

            double w = image.getWidth();
            double h = image.getHeight();

            AffineTransform translationDest = AffineTransform.getTranslateInstance(xdest[0], ydest[0]);
            AffineTransform translationImg = AffineTransform.getTranslateInstance(-w * ximg[0], -h * yimg[0]);

            AffineTransform toDest = new AffineTransform(xdest[1] - xdest[0], ydest[1] - ydest[0], xdest[2] - xdest[0], ydest[2] - ydest[0], 0, 0);
            AffineTransform fromImg = new AffineTransform(w * (ximg[1] - ximg[0]), h * (yimg[1] - yimg[0]), w * (ximg[2] - ximg[0]), h * (yimg[2] - yimg[0]), 0, 0).createInverse();

            AffineTransform transformation = new AffineTransform();
            transformation.concatenate(translationDest);
            transformation.concatenate(toDest);
            transformation.concatenate(fromImg);
            transformation.concatenate(translationImg);

            AffineTransform oldTransform = g2d.getTransform();
            g2d.setRenderingHint(RenderingHints.KEY_INTERPOLATION, key);
            g2d.drawImage(image, transformation, null);
            g2d.setTransform(oldTransform);

            if (oldKey != null) {
                g2d.setRenderingHint(RenderingHints.KEY_INTERPOLATION, oldKey);
            }
        } catch (NoninvertibleTransformException e) { }
    }

    /**
     * Check and modify the coordinates if we have a 1D texture (i.e. all the y-coords are zero)
     * @param x the x-coordinates
     * @param y the y-coordinates
     */
    private static final boolean checkSourceCoordinates(final double[] x, final double[] y) {
        if (is1d(x, y)) {
            if (!AbstractDrawable3DObject.isEqual(x[0], x[1]) && !AbstractDrawable3DObject.isEqual(x[1], x[2]) && !AbstractDrawable3DObject.isEqual(x[2], x[0])) {
                y[0] = 1;
            } else if (AbstractDrawable3DObject.isEqual(x[0], x[1]) && !AbstractDrawable3DObject.isEqual(x[1], x[2])) {
                y[0] = 1;
            } else if (AbstractDrawable3DObject.isEqual(x[0], x[2]) && !AbstractDrawable3DObject.isEqual(x[2], x[1])) {
                y[2] = 1;
            } else if (AbstractDrawable3DObject.isEqual(x[1], x[2]) && !AbstractDrawable3DObject.isEqual(x[2], x[0])) {
                y[2] = 1;
            } else {
                return true;
            }

            y[0] = !AbstractDrawable3DObject.isNull(y[0]) ? 1 : 0;
            y[1] = !AbstractDrawable3DObject.isNull(y[1]) ? 1 : 0;
            y[2] = !AbstractDrawable3DObject.isNull(y[2]) ? 1 : 0;
        }

        return false;
    }

    /**
     * Check if the triangle in texture is degenerate
     * @param x x-coordinates
     * @param y y-coordinates
     * @return true if 1d
     */
    private static final boolean is1d(final double[] x, final double[] y) {
        return AbstractDrawable3DObject.isNull(y[0]) && AbstractDrawable3DObject.isNull(y[1]) && AbstractDrawable3DObject.isNull(y[2]);
    }

    private static final void clamp(Graphics2D g2d, double[] ximg, double[] yimg, double[] xdest, double[] ydest, AffineTransform transformation, BufferedImage image) {
        if (ximg[0] < 0 || ximg[1] < 0 || ximg[2] < 0 || ximg[0] > 1 || ximg[1] > 1 || ximg[2] > 1) {
            double w = image.getWidth();
            double h = image.getHeight();
            int[] pixels = ((DataBufferInt) image.getRaster().getDataBuffer()).getData();
            Path2D.Double path = new Path2D.Double();
            path.moveTo(w * ximg[0], h * yimg[0]);
            path.lineTo(w * ximg[1], h * yimg[1]);
            path.lineTo(w * ximg[2], h * yimg[2]);
            path.closePath();
            Area tri = new Area(path);
            Rectangle2D bounds = tri.getBounds2D();

            if (bounds.getX() < 0) {
                Area rect = new Area(new Rectangle2D.Double(bounds.getX(), bounds.getY(), -bounds.getX(), bounds.getHeight()));
                tri.intersect(rect);
                if (!tri.isEmpty()) {
                    tri.transform(transformation);
                    g2d.setColor(new Color(pixels[0]));
                    g2d.fill(tri);
                }
            }

            if (bounds.getX() + bounds.getWidth() > w) {
                tri = new Area(path);
                Area rect = new Area(new Rectangle2D.Double(w, bounds.getY(), bounds.getX() + bounds.getWidth() - w, bounds.getHeight()));
                tri.intersect(rect);
                if (!tri.isEmpty()) {
                    tri.transform(transformation);
                    g2d.setColor(new Color(pixels[pixels.length - 1]));
                    g2d.fill(tri);
                }
            }
        }
    }

    /**
     * Get the projection in 2D of point A on line (BC)
     * @param A the point to project
     * @param B a point of the line
     * @param C an other point of the line (different of B)
     * @return the projected point
     */
    private static final double[] get2DProjection(final double xA, final double yA, final double xB, final double yB, final double xC, final double yC) {
        final double xBC = xC - xB;
        final double yBC = yC - yB;
        final double n = xBC * xBC + yBC * yBC;
        final double s = (xBC * (xA - xB) + yBC * (yA - yB)) / n;

        return new double[] {xB + s * xBC, yB + s * yBC};
    }
}
