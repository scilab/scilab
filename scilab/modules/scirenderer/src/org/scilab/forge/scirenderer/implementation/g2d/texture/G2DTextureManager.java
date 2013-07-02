/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.forge.scirenderer.implementation.g2d.texture;

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.implementation.g2d.G2DCanvas;
import org.scilab.forge.scirenderer.implementation.g2d.G2DDrawingTools;
import org.scilab.forge.scirenderer.texture.AbstractTexture;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureDrawer;
import org.scilab.forge.scirenderer.texture.TextureManager;

import java.awt.Dimension;
import java.awt.image.BufferedImage;
import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

/**
 * @author Calixte DENIZET
 */
public class G2DTextureManager implements TextureManager {

    private static Map<ImageBuffer, ImageBuffer> images = new HashMap<ImageBuffer, ImageBuffer>();

    public G2DTextureManager(G2DCanvas canvas) { }

    public static void clear() {
        images.clear();
    }

    /**
     * Draw the given texture.
     * @param drawingTools used drawing tools.
     * @param texture the texture too drawn.
     * @throws org.scilab.forge.scirenderer.SciRendererException if the texture is invalid.
     */
    public void draw(G2DDrawingTools drawingTools, Texture texture) throws SciRendererException {
        if (texture instanceof G2DTexture) {
            ((G2DTexture) texture).draw(drawingTools);
        }
    }

    @Override
    public Texture createTexture() {
        return new G2DTexture();
    }

    @Override
    public void dispose(Collection<Texture> textures) {
        for (Texture texture : textures) {
            dispose(texture);
        }
    }

    @Override
    public void dispose(Texture texture) {
        if (texture instanceof G2DTexture) {
            ((G2DTexture) texture).dispose();
        }
    }

    /**
     * Inner class for {@link Texture} implementation.
     */
    public class G2DTexture extends AbstractTexture implements Texture {

        private ImageBuffer image;
        private TextureDrawer drawer;

        /**
         * Default constructor.
         */
        public G2DTexture() { }

        public void dispose() {
            images.remove(image);
        }

        @Override
        public void setDrawer(TextureDrawer drawer) {
            this.drawer = drawer;
            super.setDrawer(drawer);
        }

        public TextureDrawer getDrawer() {
            return drawer;
        }

        public void draw(G2DDrawingTools drawingTools) {
            drawingTools.getMotor3D().drawTexture(drawingTools, getImage(), this);
        }

        public BufferedImage getImage() {
            if (image == null) {
                Dimension textureSize = getDataProvider().getTextureSize();
                ByteBuffer buffer = getDataProvider().getData();
                byte[] bbuffer;
                if (buffer.hasArray()) {
                    bbuffer = buffer.array();
                } else {
                    bbuffer = new byte[buffer.capacity()];
                    buffer.get(bbuffer);
                }

                ImageBuffer ib1 = new ImageBuffer(bbuffer, (int) textureSize.getWidth(), (int) textureSize.getHeight());
                ImageBuffer ib2 = images.get(ib1);
                if (ib2 == null) {
                    images.put(ib1, ib1);
                    ib2 = ib1;
                }

                image = ib2;
            }

            return image.getImage();
        }
    }

    private static class ImageBuffer {

        private byte[] buffer;
        private int hash;
        private int width;
        private int height;
        private BufferedImage image;

        ImageBuffer(byte[] buffer, int width, int height) {
            this.buffer = buffer;
            this.hash = Arrays.hashCode(buffer);
            this.width = width;
            this.height = height;
        }

        BufferedImage getImage() {
            if (image == null) {
                int[] ibuffer = new int[buffer.length / 4];
                for (int i = 0; i < ibuffer.length; i++) {
                    final int A = buffer[4 * i + 3] & 0xFF;
                    final int R = buffer[4 * i] & 0xFF;
                    final int G = buffer[4 * i + 1] & 0xFF;
                    final int B = buffer[4 * i + 2] & 0xFF;
                    ibuffer[i] = (A << 24) | (R << 16) | (G << 8) | B;
                }
                image = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
                image.setRGB(0, 0, width, height, ibuffer, 0, width);
            }

            return image;
        }

        @Override
        public int hashCode() {
            return hash;
        }

        @Override
        public boolean equals(Object o) {
            if (o instanceof ImageBuffer) {
                ImageBuffer mfb = (ImageBuffer) o;
                return Arrays.equals(buffer, mfb.buffer);
            }
            return false;
        }
    }
}
