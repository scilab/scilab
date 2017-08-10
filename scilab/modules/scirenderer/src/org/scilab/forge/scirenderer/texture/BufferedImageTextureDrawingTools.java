/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
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

package org.scilab.forge.scirenderer.texture;

import org.scilab.forge.scirenderer.implementation.jogl.utils.G2DShortCuts;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;

import javax.swing.Icon;
import javax.swing.JLabel;
import java.awt.AlphaComposite;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.font.TextLayout;
import java.awt.geom.AffineTransform;
import java.awt.geom.Rectangle2D;

/**
 *
 * Implementation of {@link org.scilab.forge.scirenderer.texture.TextureDrawingTools}.
 * This implementation create a {@link TextureBufferedImage} an fill it with texture drawing.
 *
 * @author Pierre Lando
 */
public class BufferedImageTextureDrawingTools implements TextureDrawingTools {

    /**
     * The {@link Graphics2D} used to fill the {@link TextureBufferedImage}
     */
    private Graphics2D g2d;
    private final TextureBufferedImage image;
    private final Dimension textureSize;
    private final AffineTransform baseTransform;

    /**
     * Default constructor.
     * @param textureSize the texture size.
     */
    public BufferedImageTextureDrawingTools(Dimension textureSize) {
        image = new TextureBufferedImage(textureSize.width, textureSize.height);
        this.textureSize = textureSize;

        double deltaW = (image.getWidth() - textureSize.width) / 2.0;
        double deltaH = (image.getHeight() - textureSize.height) / 2.0;
        baseTransform = AffineTransform.getTranslateInstance(deltaW, deltaH);
    }

    /**
     * Ask this image to accept a texture drawer.
     * This image will contain the drawing of the given drawer.
     * @param textureDrawer the given texture drawer.
     */
    public void accept(TextureDrawer textureDrawer) {
        g2d = image.createGraphics();
        g2d.setComposite(AlphaComposite.Src);
        g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                             RenderingHints.VALUE_ANTIALIAS_ON);
        g2d.setRenderingHint(RenderingHints.KEY_RENDERING,
                             RenderingHints.VALUE_RENDER_QUALITY);
        g2d.transform(baseTransform);

        // Change center coordinate to (0, 0).
        if (textureDrawer.getOriginPosition() == TextureDrawer.OriginPosition.CENTER) {
            g2d.translate(textureSize.width / 2, textureSize.height / 2);
        }

        textureDrawer.draw(this);

        g2d.dispose();
    }

    @Override
    public void drawPlus(int size, Appearance appearance) {
        int r = size / 2;
        int[] coords1 = new int[] { -r, 0, r, 0};
        drawPolyline(coords1, appearance);
        if (r != 0) {
            int[] coords2 = new int[] {0, -r, 0, r};
            drawPolyline(coords2, appearance);
        }
    }

    @Override
    public void drawPolyline(int[] coordinates, Appearance appearance) {
        int nbPoint = coordinates.length / 2;

        int[] xCoordinates = new int[nbPoint];
        int[] yCoordinates = new int[nbPoint];

        int k = 0;
        for (int i = 0; i < coordinates.length; i += 2) {
            xCoordinates[k] = coordinates[i];
            yCoordinates[k] = coordinates[i + 1];
            k++;
        }

        G2DShortCuts.useLineAppearance(g2d, appearance);
        G2DShortCuts.useColor(g2d, appearance.getLineColor());
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
            G2DShortCuts.useColor(g2d, appearance.getFillColor());
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
        G2DShortCuts.useLineAppearance(g2d, appearance);
        G2DShortCuts.useColor(g2d, appearance.getLineColor());
        int r = diameter / 2;
        g2d.drawOval(x - r, y - r, diameter, diameter);
    }

    @Override
    public void fillDisc(int x, int y, int diameter, Color color) {
        if (color.getAlphaAsFloat() != 0) {
            G2DShortCuts.useColor(g2d, color);
            int r = diameter / 2;
            g2d.fillOval(x - r, y - r, diameter, diameter);
        }
    }

    @Override
    public void draw(TextEntity textEntity, int x, int y) {
        if ((textEntity != null) && (textEntity.isValid())) {
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
            g2d.setColor(textEntity.getTextColor());
            TextLayout textLayout = new TextLayout(textEntity.getText(), textEntity.getFont(), g2d.getFontRenderContext());
            Rectangle2D bounds = textLayout.getBounds();
            textLayout.draw(g2d, (float) (x + 1 - bounds.getX()), y + textLayout.getAscent());
        }
    }

    @Override
    public void draw(Icon icon, int x, int y) {
        icon.paintIcon(new JLabel(), g2d, x, y);
    }

    @Override
    public void clear(Color color) {
        AffineTransform oldTransform = g2d.getTransform();
        g2d.setTransform(baseTransform);
        G2DShortCuts.useColor(g2d, color);
        g2d.fillRect(0, 0, textureSize.width, textureSize.height);
        g2d.setTransform(oldTransform);
    }

    /**
     * Return the image.
     * @return the image.
     */
    public TextureBufferedImage getImage() {
        return image;
    }
}
