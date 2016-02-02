/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
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

package org.scilab.forge.scirenderer.implementation.g2d.texture;

import org.scilab.forge.scirenderer.implementation.g2d.motor.G2DStroke;
import org.scilab.forge.scirenderer.implementation.g2d.texture.G2DTextureManager;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureDrawer;
import org.scilab.forge.scirenderer.texture.TextureDrawingTools;
import org.scilab.forge.scirenderer.texture.TextEntity;

import javax.swing.Icon;
import javax.swing.JLabel;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.font.TextLayout;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Rectangle2D;

/**
 *
 * Implementation of {@link TextureDrawingTools}.
 * This implementation create a {@link TextureBufferedImage} an fill it with sprite drawing.
 *
 * @author Pierre Lando
 */
public class G2DTextureDrawingTools implements TextureDrawingTools {

    private Graphics2D g2d;
    private int width;
    private int height;
    private java.awt.Color fillColor;
    private java.awt.Color borderColor;

    /**
     * Default constructor.
     * @param width the image width.
     * @param height the image height.
     * @param useSquareTexture true if square texture is needed.
     */
    public G2DTextureDrawingTools(Graphics2D g2d) {
        this.g2d = g2d;
    }

    public void setGraphics(Graphics2D g2d) {
        this.g2d = g2d;
    }

    public void accept(Texture texture) {
        accept(texture, null, null);
    }

    public void accept(Texture texture, java.awt.Color borderColor, java.awt.Color fillColor) {
        G2DTextureManager.G2DTexture t = (G2DTextureManager.G2DTexture) texture;
        TextureDrawer drawer = t.getDrawer();
        Dimension d = drawer.getTextureSize();
        this.width = (int) d.getWidth();
        this.height = (int) d.getHeight();
        accept(drawer, this.width, this.height, borderColor, fillColor);
    }

    /**
     * Ask this image to accept a sprite drawer.
     * This image will contain the drawing of the given drawer.
     * @param spriteDrawer the given sprite drawer.
     */
    public void accept(TextureDrawer textureDrawer, int width, int height, java.awt.Color borderColor, java.awt.Color fillColor) {
        // Change center coordinate to (0, 0).
        if (textureDrawer.getOriginPosition() == TextureDrawer.OriginPosition.CENTER) {
            g2d.translate(width / 2.0, height / 2.0);
        }

        boolean aa = g2d.getRenderingHint(RenderingHints.KEY_ANTIALIASING) == RenderingHints.VALUE_ANTIALIAS_ON;
        if (!aa) {
            g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        }
        this.fillColor = fillColor;
        this.borderColor = borderColor;
        textureDrawer.draw(this);
        this.borderColor = null;
        this.fillColor = null;

        if (!aa) {
            g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_OFF);
        }
    }

    @Override
    public void drawPlus(int size, Appearance appearance) {
        if (size > 1) {
            int r = size / 2;
            int[] coords1 = new int[] { -r, 0, r, 0};
            int[] coords2 = new int[] {0, -r, 0, r};

            drawPolyline(coords1, appearance);
            drawPolyline(coords2, appearance);
        } else {
            fillDisc(0, 0, 1, appearance.getLineColor());
        }
    }

    @Override
    public void drawPolyline(int[] coordinates, Appearance appearance) {
        if (coordinates.length == 2) {

        }

        int nbPoint = coordinates.length / 2;

        int[] xCoordinates = new int[nbPoint];
        int[] yCoordinates = new int[nbPoint];

        int k = 0;
        for (int i = 0; i < coordinates.length; i += 2) {
            xCoordinates[k] = coordinates[i];
            yCoordinates[k] = coordinates[i + 1];
            k++;
        }

        g2d.setColor(getColor(appearance.getLineColor()));
        g2d.setStroke(G2DStroke.getStroke(appearance, 0));

        g2d.drawPolyline(xCoordinates, yCoordinates, nbPoint);
    }

    @Override
    public void fillPolygon(int[] coordinates, Appearance appearance) {
        int nbPoint = coordinates.length / 2;

        int[] xCoordinates = new int[nbPoint];
        int[] yCoordinates = new int[nbPoint];

        int k = 0;
        for (int i = 0; i < coordinates.length; i += 2) {
            xCoordinates[k] = coordinates[i];
            yCoordinates[k] = coordinates[i + 1];
            k++;
        }

        if (appearance.getFillColor().getAlphaAsFloat() != 0) {
            g2d.setColor(getColor(appearance.getFillColor()));
            g2d.fillPolygon(xCoordinates, yCoordinates, nbPoint);
        }

        if (!appearance.getLineColor().equals(appearance.getFillColor())) {
            int usedLength = coordinates.length - (coordinates.length % 2);
            int[] borderCoordinate = new int[usedLength + 2];
            System.arraycopy(coordinates, 0, borderCoordinate, 0, usedLength);
            borderCoordinate[usedLength]     = coordinates[0];
            borderCoordinate[usedLength + 1] = coordinates[1];

            drawPolyline(borderCoordinate, appearance);
        }
    }

    @Override
    public void drawCircle(int x, int y, int diameter, Appearance appearance) {
        g2d.setColor(getColor(appearance.getLineColor()));
        g2d.setStroke(G2DStroke.getStroke(appearance, 0));
        double r = ((double) diameter) / 2;
        g2d.draw(new Ellipse2D.Double(x - r, y - r, diameter, diameter));
    }

    @Override
    public void fillDisc(int x, int y, int diameter, Color color) {
        if (color.getAlphaAsFloat() != 0) {
            g2d.setColor(getColor(color));
            double r = ((double) diameter) / 2;
            g2d.fill(new Ellipse2D.Double(x - r, y - r, diameter, diameter));
        }
    }

    @Override
    public void draw(TextEntity textEntity, int x, int y) {
        if (textEntity != null && textEntity.isValid()) {
            if (textEntity.isTextAntiAliased()) {
                g2d.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
            } else {
                g2d.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_OFF);
            }

            if (textEntity.isTextUseFractionalMetrics()) {
                g2d.setRenderingHint(RenderingHints.KEY_FRACTIONALMETRICS, RenderingHints.VALUE_FRACTIONALMETRICS_ON);
            } else {
                g2d.setRenderingHint(RenderingHints.KEY_FRACTIONALMETRICS, RenderingHints.VALUE_FRACTIONALMETRICS_OFF);
            }
            g2d.setColor(getColor(textEntity.getTextColor()));
            TextLayout textLayout = new TextLayout(textEntity.getText(), textEntity.getFont(), g2d.getFontRenderContext());
            Rectangle2D bounds = textLayout.getBounds();
            g2d.setFont(textEntity.getFont());
            g2d.drawString(textEntity.getText(), (float) (x - bounds.getX()), y + textLayout.getAscent());
        }
    }

    @Override
    public void draw(Icon icon, int x, int y) {
        icon.paintIcon(new JLabel(), g2d, x, y);
    }

    @Override
    public void clear(Color color) {
        g2d.setColor(getColor(color));
        g2d.fillRect(0, 0, width, height);
    }

    public java.awt.Color getColor(Color color) {
        if (color.equals(Color.BLACK)) {
            if (fillColor == null) {
                return color;
            }
            return fillColor;
        } else if (color.equals(Color.WHITE)) {
            if (borderColor == null) {
                return color;
            }
            return borderColor;
        }
        return color;
    }
}
