/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.implementation.g2d.G2DCanvas;
import org.scilab.forge.scirenderer.implementation.g2d.G2DDrawingTools;
import org.scilab.forge.scirenderer.texture.AbstractTexture;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureDrawer;
import org.scilab.forge.scirenderer.texture.TextureManager;
import org.scilab.forge.scirenderer.texture.TextureDataProvider.ImageType;

import java.awt.Dimension;
import java.awt.image.BufferedImage;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
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

    private static final int toColorComp(final float f) {
        return (f < 0) ? 0 : ((f > 1) ? 0xFF : ((int) (f * 255.0f)));
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
                ImageType type = getDataProvider().getImageType();
                int imageType = BufferedImage.TYPE_INT_RGB;
                int[] ibuffer = null;
                final ByteOrder order = ByteOrder.nativeOrder();

                switch (type) {
                    case RGB: {
                        imageType = BufferedImage.TYPE_INT_RGB;
                        ibuffer = new int[buffer.capacity() / 3];
                        int k = 0;
                        for (int i = 0; i < buffer.capacity(); i += 3) {
                            ibuffer[k++] = ((buffer.get(i) & 0xFF) << 16) | ((buffer.get(i + 1) & 0xFF) << 8) | (buffer.get(i + 2) & 0xFF);
                        }
                        break;
                    }
                    case RGB_RGBA: {
                        imageType = BufferedImage.TYPE_INT_RGB;
                        ibuffer = new int[buffer.capacity() / 4];
                        int k = 0;
                        if (order == ByteOrder.LITTLE_ENDIAN) {
                            for (int i = 0; i < buffer.capacity(); i += 4) {
                                ibuffer[k++] = ((buffer.get(i + 3) & 0xFF) << 16) | ((buffer.get(i + 2) & 0xFF) << 8) | (buffer.get(i + 1) & 0xFF);
                            }
                        } else {
                            for (int i = 0; i < buffer.capacity(); i += 4) {
                                ibuffer[k++] = ((buffer.get(i) & 0xFF) << 16) | ((buffer.get(i + 1) & 0xFF) << 8) | (buffer.get(i + 2) & 0xFF);
                            }
                        }
                        break;
                    }
                    case BGR: {
                        imageType = BufferedImage.TYPE_INT_BGR;
                        ibuffer = new int[buffer.capacity() / 3];
                        int k = 0;
                        for (int i = 0; i < buffer.capacity(); i += 3) {
                            ibuffer[k++] = ((buffer.get(i) & 0xFF) << 16) | ((buffer.get(i + 1) & 0xFF) << 8) | (buffer.get(i + 2) & 0xFF);
                        }
                        break;
                    }
                    case GRAY: {
                        imageType = BufferedImage.TYPE_INT_RGB;
                        ibuffer = new int[buffer.capacity()];
                        for (int i = 0; i < buffer.capacity(); i++) {
                            final int c = buffer.get(i) & 0xFF;
                            ibuffer[i] = (c << 16) | (c << 8) | c;
                        }
                        break;
                    }
                    case GRAY_16: {
                        imageType = BufferedImage.TYPE_INT_RGB;
                        ibuffer = new int[buffer.capacity() / 2];
                        int k = 0;
                        if (order == ByteOrder.LITTLE_ENDIAN) {
                            for (int i = 0; i < buffer.capacity(); i += 2) {
                                final int c = (((buffer.get(i + 1) & 0xFF) << 8) | (buffer.get(i) & 0xFF)) / 257;
                                ibuffer[k++] = (c << 16) | (c << 8) | c;
                            }
                        } else {
                            for (int i = 0; i < buffer.capacity(); i += 2) {
                                final int c = (((buffer.get(i) & 0xFF) << 8) | (buffer.get(i + 1) & 0xFF)) / 257;
                                ibuffer[k++] = (c << 16) | (c << 8) | c;
                            }
                        }
                        break;
                    }
                    case RGBA: {
                        imageType = BufferedImage.TYPE_INT_ARGB;
                        ibuffer = new int[buffer.capacity() / 4];
                        if (order == ByteOrder.LITTLE_ENDIAN) {
                            int k = 0;
                            for (int i = 0; i < buffer.capacity(); i += 4) {
                                ibuffer[k++] = ((buffer.get(i) & 0xFF) << 24) | ((buffer.get(i + 3) & 0xFF) << 16) | ((buffer.get(i + 2) & 0xFF) << 8) | (buffer.get(i + 1) & 0xFF);
                            }
                        } else {
                            int k = 0;
                            for (int i = 0; i < buffer.capacity(); i += 4) {
                                ibuffer[k++] = ((buffer.get(i + 3) & 0xFF) << 24) | ((buffer.get(i) & 0xFF) << 16) | ((buffer.get(i + 1) & 0xFF) << 8) | (buffer.get(i + 2) & 0xFF);
                            }
                        }
                        break;
                    }
                    case RGBA_REV: {
                        imageType = BufferedImage.TYPE_INT_ARGB;
                        ibuffer = new int[buffer.capacity() / 4];
                        if (order == ByteOrder.BIG_ENDIAN) {
                            int k = 0;
                            for (int i = 0; i < buffer.capacity(); i += 4) {
                                ibuffer[k++] = ((buffer.get(i) & 0xFF) << 24) | ((buffer.get(i + 3) & 0xFF) << 16) | ((buffer.get(i + 2) & 0xFF) << 8) | (buffer.get(i + 1) & 0xFF);
                            }
                        } else {
                            int k = 0;
                            for (int i = 0; i < buffer.capacity(); i += 4) {
                                ibuffer[k++] = ((buffer.get(i + 3) & 0xFF) << 24) | ((buffer.get(i) & 0xFF) << 16) | ((buffer.get(i + 1) & 0xFF) << 8) | (buffer.get(i + 2) & 0xFF);
                            }
                        }
                        break;
                    }
                    case ABGR: {
                        imageType = BufferedImage.TYPE_INT_ARGB;
                        ibuffer = new int[buffer.capacity() / 4];
                        int k = 0;
                        for (int i = 0; i < buffer.capacity(); i += 4) {
                            ibuffer[k++] = ((buffer.get(i) & 0xFF) << 24) | ((buffer.get(i + 3) & 0xFF) << 16) | ((buffer.get(i + 2) & 0xFF) << 8) | (buffer.get(i + 1) & 0xFF);
                        }
                        break;
                    }
                    case RGB_332: {
                        imageType = BufferedImage.TYPE_INT_RGB;
                        ibuffer = new int[buffer.capacity()];
                        for (int i = 0; i < buffer.capacity(); i++) {
                            final int c = buffer.get(i) & 0xFF;
                            ibuffer[i] = ((((c >> 5) & 0x7) * 255 / 7) << 16) | ((((c >> 2) & 0x7) * 255 / 7) << 8) | ((c & 0x3) * 255 / 3);
                        }
                        break;
                    }
                    case RED: {
                        imageType = BufferedImage.TYPE_INT_RGB;
                        ibuffer = new int[buffer.capacity()];
                        for (int i = 0; i < buffer.capacity(); i++) {
                            ibuffer[i] = (buffer.get(i) & 0xFF) << 16;
                        }
                        break;
                    }
                    case GREEN: {
                        imageType = BufferedImage.TYPE_INT_RGB;
                        ibuffer = new int[buffer.capacity()];
                        for (int i = 0; i < buffer.capacity(); i++) {
                            ibuffer[i] = (buffer.get(i) & 0xFF) << 8;
                        }
                        break;
                    }
                    case BLUE: {
                        imageType = BufferedImage.TYPE_INT_RGB;
                        ibuffer = new int[buffer.capacity()];
                        for (int i = 0; i < buffer.capacity(); i++) {
                            ibuffer[i] = (buffer.get(i) & 0xFF);
                        }
                        break;
                    }
                    case INTENSITY: {
                        imageType = BufferedImage.TYPE_INT_ARGB;
                        ibuffer = new int[buffer.capacity()];
                        for (int i = 0; i < buffer.capacity(); i++) {
                            final int c = buffer.get(i) & 0xFF;
                            ibuffer[i] = (c << 24) | (c << 16) | (c << 8) | c;
                        }
                        break;
                    }
                    case RGBA_4444: {
                        imageType = BufferedImage.TYPE_INT_ARGB;
                        ibuffer = new int[buffer.capacity() / 2];
                        int k = 0;
                        if (order == ByteOrder.LITTLE_ENDIAN) {
                            for (int i = 0; i < buffer.capacity(); i += 2) {
                                int c = buffer.get(i) & 0xFF;
                                final int B = ((c >> 4) & 0xF) * 0x11;
                                final int A = (c & 0xF) * 0x11;
                                c = buffer.get(i + 1) & 0xFF;
                                final int R = ((c >> 4) & 0xF) * 0x11;
                                final int G = (c & 0xF) * 0x11;
                                ibuffer[k++] = (A << 24) | (R << 16) | (G << 8) | B;
                            }
                        } else {
                            for (int i = 0; i < buffer.capacity(); i += 2) {
                                int c = buffer.get(i + 1) & 0xFF;
                                final int B = ((c >> 4) & 0xF) * 0x2F;
                                final int A = (c & 0xF) * 0x2F;
                                c = buffer.get(i) & 0xFF;
                                final int R = ((c >> 4) & 0xF) * 0x2F;
                                final int G = (c & 0xF) * 0x2F;
                                ibuffer[k++] = (A << 24) | (R << 16) | (G << 8) | B;
                            }
                        }
                        break;
                    }
                    case RGBA_5551: {
                        imageType = BufferedImage.TYPE_INT_ARGB;
                        ibuffer = new int[buffer.capacity() / 2];
                        int k = 0;
                        if (order == ByteOrder.LITTLE_ENDIAN) {
                            for (int i = 0; i < buffer.capacity(); i += 2) {
                                int c = buffer.get(i) & 0xFF;
                                final int B = (((c >> 1) & 0x1F) * 0xFF) / 0x1F;
                                final int A = (c & 0x1) * 0xFF;
                                int G = (c >> 6) & 0x3;
                                c = buffer.get(i + 1) & 0xFF;
                                final int R = (((c >> 3) & 0x1F) * 0xFF) / 0x1F;
                                G = (((c & 0x7) << 2 | G) * 0xFF) / 0x1F;
                                ibuffer[k++] = (A << 24) | (R << 16) | (G << 8) | B;
                            }
                        } else {
                            for (int i = 0; i < buffer.capacity(); i += 2) {
                                int c = buffer.get(i + 1) & 0xFF;
                                final int B = (((c >> 1) & 0x1F) * 0xFF) / 0x1F;
                                final int A = (c & 0x1) * 0xFF;
                                int G = (c >> 6) & 0x3;
                                c = buffer.get(i) & 0xFF;
                                final int R = (((c >> 3) & 0x1F) * 0xFF) / 0x1F;
                                G = (((c & 0x7) << 2 | G) * 0xFF) / 0x1F;
                                ibuffer[k++] = (A << 24) | (R << 16) | (G << 8) | B;
                            }
                        }
                        break;
                    }
                    case RGB_FLOAT: {
                        imageType = BufferedImage.TYPE_INT_RGB;
                        FloatBuffer fbuffer = buffer.asFloatBuffer();
                        ibuffer = new int[fbuffer.capacity() / 3];
                        int k = 0;
                        for (int i = 0; i < fbuffer.capacity(); i += 3) {
                            ibuffer[k++] = (toColorComp(fbuffer.get(i)) << 16) | (toColorComp(buffer.get(i + 1)) << 8) | toColorComp(buffer.get(i + 2));
                        }
                        break;
                    }
                    case RGBA_FLOAT: {
                        imageType = BufferedImage.TYPE_INT_ARGB;
                        buffer.order(order);
                        FloatBuffer fbuffer = buffer.asFloatBuffer();
                        ibuffer = new int[fbuffer.capacity() / 4];
                        int k = 0;
                        for (int i = 0; i < fbuffer.capacity(); i += 4) {
                            ibuffer[k++] = (toColorComp(fbuffer.get(i + 3)) << 24) | (toColorComp(fbuffer.get(i)) << 16) | (toColorComp(fbuffer.get(i + 1)) << 8) | toColorComp(fbuffer.get(i + 2));
                        }
                        break;
                    }
                    case GRAY_FLOAT: {
                        imageType = BufferedImage.TYPE_INT_RGB;
                        buffer.order(order);
                        FloatBuffer fbuffer = buffer.asFloatBuffer();
                        ibuffer = new int[fbuffer.capacity()];
                        for (int i = 0; i < fbuffer.capacity(); i++) {
                            final int c = toColorComp(fbuffer.get(i));
                            ibuffer[i] = (c << 16) | (c << 8) | c;
                        }
                        break;
                    }
                    case RED_16: {
                        imageType = BufferedImage.TYPE_INT_RGB;
                        ibuffer = new int[buffer.capacity() / 2];
                        int k = 0;
                        if (order == ByteOrder.LITTLE_ENDIAN) {
                            for (int i = 0; i < buffer.capacity(); i += 2) {
                                final int c = (((buffer.get(i + 1) & 0xFF) << 8) | (buffer.get(i) & 0xFF)) / 257;
                                ibuffer[k++] = c << 16;
                            }
                        } else {
                            for (int i = 0; i < buffer.capacity(); i += 2) {
                                final int c = (((buffer.get(i) & 0xFF) << 8) | (buffer.get(i + 1) & 0xFF)) / 257;
                                ibuffer[k++] = c << 16;
                            }
                        }
                        break;
                    }
                    case GREEN_16: {
                        imageType = BufferedImage.TYPE_INT_RGB;
                        ibuffer = new int[buffer.capacity() / 2];
                        int k = 0;
                        if (order == ByteOrder.LITTLE_ENDIAN) {
                            for (int i = 0; i < buffer.capacity(); i += 2) {
                                final int c = (((buffer.get(i + 1) & 0xFF) << 8) | (buffer.get(i) & 0xFF)) / 257;
                                ibuffer[k++] = c << 8;
                            }
                        } else {
                            for (int i = 0; i < buffer.capacity(); i += 2) {
                                final int c = (((buffer.get(i) & 0xFF) << 8) | (buffer.get(i + 1) & 0xFF)) / 257;
                                ibuffer[k++] = c << 8;
                            }
                        }
                        break;
                    }
                    case BLUE_16: {
                        imageType = BufferedImage.TYPE_INT_RGB;
                        ibuffer = new int[buffer.capacity() / 2];
                        int k = 0;
                        if (order == ByteOrder.LITTLE_ENDIAN) {
                            for (int i = 0; i < buffer.capacity(); i += 2) {
                                final int c = (((buffer.get(i + 1) & 0xFF) << 8) | (buffer.get(i) & 0xFF)) / 257;
                                ibuffer[k++] = c;
                            }
                        } else {
                            for (int i = 0; i < buffer.capacity(); i += 2) {
                                final int c = (((buffer.get(i) & 0xFF) << 8) | (buffer.get(i + 1) & 0xFF)) / 257;
                                ibuffer[k++] = c;
                            }
                        }
                        break;
                    }
                    case RED_FLOAT: {
                        imageType = BufferedImage.TYPE_INT_RGB;
                        buffer.order(order);
                        FloatBuffer fbuffer = buffer.asFloatBuffer();
                        ibuffer = new int[fbuffer.capacity()];
                        for (int i = 0; i < fbuffer.capacity(); i++) {
                            ibuffer[i] = toColorComp(fbuffer.get(i)) << 16;
                        }
                        break;
                    }
                    case GREEN_FLOAT: {
                        imageType = BufferedImage.TYPE_INT_RGB;
                        buffer.order(order);
                        FloatBuffer fbuffer = buffer.asFloatBuffer();
                        ibuffer = new int[fbuffer.capacity()];
                        for (int i = 0; i < fbuffer.capacity(); i++) {
                            ibuffer[i] = toColorComp(fbuffer.get(i)) << 8;
                        }
                        break;
                    }
                    case BLUE_FLOAT: {
                        imageType = BufferedImage.TYPE_INT_RGB;
                        buffer.order(order);
                        FloatBuffer fbuffer = buffer.asFloatBuffer();
                        ibuffer = new int[fbuffer.capacity()];
                        for (int i = 0; i < fbuffer.capacity(); i++) {
                            ibuffer[i] = toColorComp(fbuffer.get(i));
                        }
                        break;
                    }
                    case RGBA_BYTE: {
                        imageType = BufferedImage.TYPE_INT_ARGB;
                        ibuffer = new int[buffer.capacity() / 4];
                        int k = 0;
                        for (int i = 0; i < buffer.capacity(); i += 4) {
                            ibuffer[k++] = ((buffer.get(i + 3) & 0xFF) << 24) | ((buffer.get(i) & 0xFF) << 16) | ((buffer.get(i + 1) & 0xFF) << 8) | (buffer.get(i + 2) & 0xFF);
                        }
                        break;
                    }
                }

                ImageBuffer ib1;
                if (getDataProvider().isRowMajorOrder()) {
                    ib1 = new ImageBuffer(imageType, ibuffer, (int) textureSize.getWidth(), (int) textureSize.getHeight());
                } else {
                    int[] tibuffer = new int[ibuffer.length];
                    final int w = (int) textureSize.getWidth();
                    final int h = (int) textureSize.getHeight();
                    int k = 0;
                    for (int i = 0; i < h; i++) {
                        for (int j = 0; j < w; j++) {
                            tibuffer[k++] = ibuffer[i + j * h];
                        }
                    }
                    ib1 = new ImageBuffer(imageType, tibuffer, w, h);
                }

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

        private int[] buffer;
        private int hash;
        private int width;
        private int height;
        private BufferedImage image;
        private int type;

        ImageBuffer(int type, int[] buffer, int width, int height) {
            this.type = type;
            this.buffer = buffer;
            this.hash = Arrays.hashCode(buffer) * 31 + type;
            this.width = width;
            this.height = height;
        }

        BufferedImage getImage() {
            if (image == null) {
                image = new BufferedImage(width, height, type);
                image.setRGB(0, 0, width, height, buffer, 0, width);
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
                return type == mfb.type && Arrays.equals(buffer, mfb.buffer);
            }
            return false;
        }
    }
}
