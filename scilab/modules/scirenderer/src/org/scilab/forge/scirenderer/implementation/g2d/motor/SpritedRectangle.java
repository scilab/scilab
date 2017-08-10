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

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.Stroke;
import java.awt.geom.AffineTransform;
import java.awt.geom.Path2D;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.List;

import org.scilab.forge.scirenderer.implementation.g2d.texture.G2DTextureDrawingTools;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.forge.scirenderer.tranformations.Vector4d;

/**
 * @author Calixte DENIZET
 */
public class SpritedRectangle extends ConvexObject {

    private static final Color[] COLORS = new Color[] {Color.BLACK, Color.BLACK, Color.BLACK};

    private Texture sprite;
    private BufferedImage image;
    private G2DTextureDrawingTools drawingTools;
    private double rotationAngle;
    private Texture.Filter filter;
    private Vector3d position;
    private Color fillColor;
    private Color borderColor;

    public SpritedRectangle(Vector3d vertex, Texture sprite, AnchorPosition anchor, G2DTextureDrawingTools drawingTools, double rotationAngle, Color borderColor, Color fillColor) throws InvalidPolygonException {
        super(getSpriteVertices(vertex, sprite, anchor, rotationAngle), null);
        this.sprite = sprite;
        this.drawingTools = drawingTools;
        this.rotationAngle = rotationAngle;
        this.position = vertex;
        this.fillColor = fillColor;
        this.borderColor = borderColor;
    }

    public SpritedRectangle(Vector3d vertex, Transformation transf, BufferedImage image, Texture.Filter filter) throws InvalidPolygonException {
        super(getSpriteVertices(vertex, transf, image), null);
        this.image = image;
        this.filter = filter;
        this.position = vertex;
    }

    @Override
    public List<ConvexObject> breakObject(ConvexObject o) {
        if (o instanceof Triangle) {
            return breakObject((Triangle) o);
        } else if (o instanceof Segment) {
            return breakObject((Segment) o);
        } else if (o instanceof SpritedRectangle) {
            return breakObject((SpritedRectangle) o);
        }
        return null;
    }

    public List<ConvexObject> breakObject(Triangle o) {
        try {
            Vector3d[] v1 = new Vector3d[] {vertices[0], vertices[1], vertices[2]};
            Vector3d[] v2 = new Vector3d[] {vertices[0], vertices[2], vertices[3]};
            Triangle t1 = new Triangle(v1, COLORS);
            Triangle t2 = new Triangle(v2, COLORS);
            t1.setSprite(this);
            t2.setSprite(this);
            List<ConvexObject> list = new ArrayList<ConvexObject>();
            if (o.isBehind(t1) == 2) {
                List<ConvexObject> list2 = o.breakObject(t1);
                if (list2 != null) {
                    list.addAll(list2);
                }
            } else {
                list.add(t1);
            }
            if (o.isBehind(t2) == 2) {
                List<ConvexObject> list2 = o.breakObject(t2);
                if (list2 != null) {
                    list.addAll(list2);
                }
            } else {
                list.add(t2);
            }

            return list;
        } catch (InvalidPolygonException e) { }

        return null;
    }

    public List<ConvexObject> breakObject(Segment o) {
        try {
            Vector3d[] v = new Vector3d[] {vertices[0], vertices[1], vertices[2]};
            Triangle t1 = new Triangle(v, COLORS);
            t1.setSprite(this);
            v = new Vector3d[] {vertices[0], vertices[2], vertices[3]};
            Triangle t2 = new Triangle(v, COLORS);
            t2.setSprite(this);
            List<ConvexObject> list = t1.breakObject(o);
            if (list != null) {
                list.add(t2);
                return list;
            }

            list = t2.breakObject(o);
            if (list != null) {
                list.add(t1);
                return list;
            }
        } catch (InvalidPolygonException e) { }

        return null;
    }

    public List<ConvexObject> breakObject(SpritedRectangle o) {
        try {
            Vector3d[] v1 = new Vector3d[] {vertices[0], vertices[1], vertices[2]};
            Vector3d[] v2 = new Vector3d[] {vertices[0], vertices[2], vertices[3]};
            Triangle t1 = new Triangle(v1, COLORS);
            Triangle t2 = new Triangle(v2, COLORS);
            t1.setSprite(this);
            t2.setSprite(this);
            List<ConvexObject> list = o.breakObject(t1);
            if (list == null) {
                list = o.breakObject(t2);
            } else {
                List<ConvexObject> list2 = o.breakObject(t2);
                if (list2 != null) {
                    list.addAll(list2);
                }
            }

            return list;
        } catch (InvalidPolygonException e) { }

        return null;
    }

    public List<ConvexObject> breakObject(Vector4d v) {
        try {
            double[] vv = v.getData();
            Vector3d np = new Vector3d(vv);
            int ret = isBehind(np, vv[3]);
            if (ret == 1) {
                List<ConvexObject> list = new ArrayList<ConvexObject>();
                list.add(this);
                return list;
            } else if (ret == -1) {
                return null;
            }

            Vector3d[] v1 = new Vector3d[] {vertices[0], vertices[1], vertices[2]};
            Vector3d[] v2 = new Vector3d[] {vertices[0], vertices[2], vertices[3]};
            Triangle t1 = new Triangle(v1, COLORS);
            Triangle t2 = new Triangle(v2, COLORS);
            t1.setSprite(this);
            t2.setSprite(this);
            List<ConvexObject> list = t1.breakObject(v);
            if (list != null) {
                List<ConvexObject> list2 = t2.breakObject(v);
                if (list2 != null) {
                    list.addAll(list2);
                }
                return list;
            } else {
                return t2.breakObject(v);
            }
        } catch (InvalidPolygonException e) { }

        return null;
    }

    private static Vector3d[] getSpriteVertices(Vector3d vertex, Transformation transf, BufferedImage image) {
        int w = image.getWidth();
        int h = image.getHeight();
        double x = vertex.getX();
        double y = vertex.getY();
        double z = vertex.getZ();

        return new Vector3d[] {transf.project(new Vector3d(x, y, z)), transf.project(new Vector3d(x + w, y, z)), transf.project(new Vector3d(x + w, y + h, z)), transf.project(new Vector3d(x, y + h, z))};
    }

    private static Vector3d[] getSpriteVertices(Vector3d vertex, Texture sprite, AnchorPosition anchor) {
        Dimension d = sprite.getDataProvider().getTextureSize();
        double spriteWidth = d.getWidth();
        double spriteHeight = d.getHeight();
        double deltaX = 0;
        double deltaY = 0;

        switch (anchor) {
            case LEFT:
                deltaX = spriteWidth / 2;
                break;
            case LOWER_LEFT:
                deltaX = spriteWidth / 2;
                deltaY = spriteHeight / 2;
                break;
            case UPPER_LEFT:
                deltaX = spriteWidth / 2;
                deltaY = -spriteHeight / 2;
                break;
            case UP:
                deltaY = -spriteHeight / 2;
                break;
            case CENTER:
                break;
            case DOWN:
                deltaY = spriteHeight / 2;
                break;
            case RIGHT:
                deltaX = -spriteWidth / 2;
                break;
            case LOWER_RIGHT:
                deltaX = -spriteWidth / 2;
                deltaY = spriteHeight / 2;
                break;
            case UPPER_RIGHT:
                deltaX = -spriteWidth / 2;
                deltaY = -spriteHeight / 2;
                break;
            default:
        }

        double x = deltaX;
        double y = -deltaY;
        double z = vertex.getZ();

        return new Vector3d[] {new Vector3d(x - spriteWidth / 2, y - spriteHeight / 2, z),
                   new Vector3d(x - spriteWidth / 2, y + spriteHeight / 2, z),
                   new Vector3d(x + spriteWidth / 2, y + spriteHeight / 2, z),
                   new Vector3d(x + spriteWidth / 2, y - spriteHeight / 2, z)
        };
    }

    private static Vector3d[] getSpriteVertices(Vector3d vertex, Texture sprite, AnchorPosition anchor, double rotationAngle) {
        Vector3d[] points = getSpriteVertices(vertex, sprite, anchor);

        if (rotationAngle != 0) {
            final double a = -rotationAngle * Math.PI / 180;
            final double ca = Math.cos(a);
            final double sa = Math.sin(a);

            for (int i = 0; i < 4; i++) {
                points[i] = new Vector3d(vertex.getX() + ca * points[i].getX() - sa * points[i].getY(), vertex.getY() + sa * points[i].getX() + ca * points[i].getY(), vertex.getZ());
            }
        } else {
            for (int i = 0; i < 4; i++) {
                points[i] = new Vector3d(vertex.getX() + points[i].getX(), vertex.getY() + points[i].getY(), vertex.getZ());
            }
        }

        return points;
    }

    private static Vector3d unrotate(Vector3d v, Vector3d t, double rotationAngle) {
        v = v.minus(t);
        final double a = rotationAngle * Math.PI / 180;
        final double ca = Math.cos(a);
        final double sa = Math.sin(a);

        return new Vector3d(ca * v.getX() - sa * v.getY(), sa * v.getX() + ca * v.getY(), v.getZ());
    }

    public Texture getSprite() {
        return sprite;
    }

    @Override
    public void draw(Graphics2D g2d) {
        if (sprite != null) {
            Path2D contour = getProjectedContour();
            AffineTransform oldTransf = g2d.getTransform();
            Stroke oldStroke = g2d.getStroke();

            if (rotationAngle != 0) {
                g2d.translate(position.getX(), position.getY());
                g2d.rotate(-rotationAngle * Math.PI / 180);
                Vector3d v = unrotate(vertices[0], position, rotationAngle);
                g2d.translate(v.getX(), v.getY());
            } else {
                g2d.translate(vertices[0].getX(), vertices[0].getY());
            }

            drawingTools.accept(sprite, borderColor, fillColor);
            g2d.setTransform(oldTransf);
            g2d.setStroke(oldStroke);
        } else {
            Object key = filter == Texture.Filter.LINEAR ? RenderingHints.VALUE_INTERPOLATION_BILINEAR : RenderingHints.VALUE_INTERPOLATION_NEAREST_NEIGHBOR;
            DrawTools.drawParallelogramTexture(g2d, image, new double[] {0, 1, 1, 0}, new double[] {0, 0, 1, 1},
                                               new double[] {vertices[3].getX(), vertices[2].getX(), vertices[1].getX(), vertices[0].getX()},
                                               new double[] {vertices[3].getY(), vertices[2].getY(), vertices[1].getY(), vertices[0].getY()}, key);
        }
    }

    public String toString() {
        return "SpritedRectangle: " + vertices[0].toString() + " " + vertices[1].toString() + " " + vertices[2].toString() + " " + vertices[3].toString() + "\nPrecedence: " + precedence;
    }
}
