/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Marcos CARDINOT
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

package org.scilab.modules.gui.ged.graphic_objects.contouredObject;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.geom.GeneralPath;
import java.awt.geom.Point2D;

/**
* Mark Style.
* Types of mark to use when mark_mode property is "on"
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public enum MarkStyle {
    /**
     * mark_style: index 0.
     */
    CIRCLE_SOLID {
        @Override
        public Graphics2D getMarkStyle() {
            g2d.setPaint(foreground);
            g2d.drawOval(5, 2, 14, 14);
            g2d.fillOval(5, 2, 14, 14);

            return g2d;
        }
    },
    /**
     * mark_style: index 1.
     */
    PLUS {
        @Override
        public Graphics2D getMarkStyle() {
            g2d.setPaint(foreground);
            g2d.setStroke(new BasicStroke(2));
            g2d.drawLine(12, 4, 12, 16);
            g2d.drawLine(6, 10, 18, 10);

            return g2d;
        }
    },
    /**
     * mark_style: index 2.
     */
    CROSS {
        @Override
        public Graphics2D getMarkStyle() {
            g2d.setPaint(foreground);
            g2d.setStroke(new BasicStroke(2));
            g2d.drawLine(6, 6, 16, 16);
            g2d.drawLine(16, 6, 6, 16);

            return g2d;
        }
    },
    /**
     * mark_style: index 3.
     */
    CIRCLE_PLUS {
        @Override
        public Graphics2D getMarkStyle() {
            g2d.setPaint(background);
            g2d.fillOval(5, 2, 14, 14);
            g2d.setPaint(foreground);
            g2d.drawOval(5, 2, 14, 14);
            g2d.drawLine(12, 3, 12, 15);
            g2d.drawLine(6, 9, 18, 9);

            return g2d;
        }
    },
    /**
     * mark_style: index 4.
     */
    DIAMOND_SOLID {
        @Override
        public Graphics2D getMarkStyle() {
            g2d.setPaint(background);
            g2d.rotate(Math.toRadians(45), 6, 6);
            g2d.fillRect(7, -1, 11, 11);
            g2d.setPaint(foreground);

            return g2d;
        }
    },
    /**
     * mark_style: index 5.
     */
    DIAMOND {
        @Override
        public Graphics2D getMarkStyle() {
            g2d.setPaint(background);
            g2d.rotate(Math.toRadians(45), 6, 6);
            g2d.fillRect(7, -1, 11, 11);
            g2d.setPaint(foreground);
            g2d.drawRect(7, -1, 11, 11);

            return g2d;
        }
    },
    /**
     * mark_style: index 6.
     */
    UP {
        @Override
        public Graphics2D getMarkStyle() {
            int[] xPoints = {5, 12, 19};
            int[] yPoints = {14, 3, 14};
            int numPoints = xPoints.length;

            g2d.setPaint(background);
            g2d.fillPolygon(xPoints, yPoints, numPoints);
            g2d.setPaint(foreground);
            g2d.drawPolygon(xPoints, yPoints, numPoints);

            return g2d;
        }
    },
    /**
     * mark_style: index 7.
     */
    DOWN {
        @Override
        public Graphics2D getMarkStyle() {
            int[] xPoints = {5, 12, 19};
            int[] yPoints = {3, 14, 3};
            int numPoints = xPoints.length;

            g2d.setPaint(background);
            g2d.fillPolygon(xPoints, yPoints, numPoints);
            g2d.setPaint(foreground);
            g2d.drawPolygon(xPoints, yPoints, numPoints);

            return g2d;
        }
    },
    /**
     * mark_style: index 8.
     */
    DIAMOND_PLUS {
        @Override
        public Graphics2D getMarkStyle() {
            g2d.setPaint(background);
            g2d.rotate(Math.toRadians(45), 6, 6);
            g2d.fillRect(7, -1, 11, 11);
            g2d.setPaint(foreground);
            g2d.drawRect(7, -1, 11, 11);
            g2d.rotate(Math.toRadians(-45), 6, 6);
            g2d.drawLine(12, 3, 12, 16);
            g2d.drawLine(5, 10, 18, 10);

            return g2d;
        }
    },
    /**
     * mark_style: index 9.
     */
    CIRCLE {
        @Override
        public Graphics2D getMarkStyle() {
            g2d.setPaint(background);
            g2d.fillOval(5, 2, 14, 14);
            g2d.setPaint(foreground);
            g2d.drawOval(5, 2, 14, 14);

            return g2d;
        }
    },
    /**
     * mark_style: index 10.
     */
    ASTERISK {
        @Override
        public Graphics2D getMarkStyle() {
            g2d.setPaint(foreground);
            g2d.drawLine(7, 5, 17, 15);
            g2d.drawLine(17, 5, 7, 15);
            g2d.drawLine(12, 4, 12, 16);
            g2d.drawLine(6, 10, 18, 10);

            return g2d;
        }
    },
    /**
     * mark_style: index 11.
     */
    SQUARE {
        @Override
        public Graphics2D getMarkStyle() {
            g2d.setPaint(background);
            g2d.fillRect(6, 2, 12, 12);
            g2d.setPaint(foreground);
            g2d.drawRect(6, 2, 12, 12);

            return g2d;
        }
    },
    /**
     * mark_style: index 12.
     */
    FORWARD {
        @Override
        public Graphics2D getMarkStyle() {
            final int[] xPoints = {6, 6, 19};
            final int[] yPoints = {2, 16, 9};
            final int numPoints = xPoints.length;

            g2d.setPaint(background);
            g2d.fillPolygon(xPoints, yPoints, numPoints);
            g2d.setPaint(foreground);
            g2d.drawPolygon(xPoints, yPoints, numPoints);

            return g2d;
        }
    },
    /**
     * mark_style: index 13.
     */
    BACKWARD {
        @Override
        public Graphics2D getMarkStyle() {
            int[] xPoints = {6, 19, 19};
            int[] yPoints = {9, 2, 16};
            int numPoints = xPoints.length;

            g2d.setPaint(background);
            g2d.fillPolygon(xPoints, yPoints, numPoints);
            g2d.setPaint(foreground);
            g2d.drawPolygon(xPoints, yPoints, numPoints);

            return g2d;
        }
    },
    /**
     * mark_style: index 14.
     */
    STAR {
        @Override
        public Graphics2D getMarkStyle() {
            final int arms = 5;
            final double rInner = 3;
            final double rOuter = 8;
            final Point center = new Point(15, 9);
            double angle = Math.PI / arms;
            GeneralPath path = new GeneralPath();
            for (int i = 0; i < 2 * arms; i++) {
                double r = (i & 1) == 0 ? rOuter : rInner;
                Point2D.Double p = new Point2D.Double(center.x + Math.cos(i * angle) * r, center.y + Math.sin(i * angle) * r);
                if (i == 0) {
                    path.moveTo(p.getX(), p.getY());
                } else {
                    path.lineTo(p.getX(), p.getY());
                }
            }
            path.closePath();

            g2d.rotate(Math.toRadians(54), center.getX(), center.getY());
            g2d.setPaint(background);
            g2d.fill(path);
            g2d.setPaint(foreground);
            g2d.draw(path);

            return g2d;
        }
    };

    private static Graphics2D g2d;
    private static Color background = Color.WHITE;
    private static Color foreground = Color.BLACK;

    /**
     * Get Mark Style Selected.
     * @return Graphics2D
     */
    public abstract Graphics2D getMarkStyle();

    /**
     * Set Graphics2D of Component
     * @param g Graphics2D
     */
    public void setGraphics2D(Graphics2D g) {
        g2d = g;
    }

    /**
     * Set the Background of the Drawing.
     * @param c Color
     */
    public void setBackground(Color c) {
        background = c;
    }

    /**
     * Set the Foreground of the Drawing.
     * @param c Color
     */
    public void setForeground(Color c) {
        foreground = c;
    }
}