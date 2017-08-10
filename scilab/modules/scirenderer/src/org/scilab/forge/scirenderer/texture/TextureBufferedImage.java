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

package org.scilab.forge.scirenderer.texture;

import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;
import java.awt.BorderLayout;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferInt;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * @author Pierre Lando
 */
public class TextureBufferedImage extends BufferedImage {
    private static final int A_SHIFT = 24;
    private static final int R_SHIFT = 16;
    private static final int G_SHIFT = 8;
    private static final int B_SHIFT = 0;
    private static final int COMPONENT_MASK = 0xFF;
    private static final int NB_COMPONENTS = 4;

    /**
     * Default constructor.
     * @param width texture width.
     * @param height texture height.
     */
    public TextureBufferedImage(int width, int height) {
        super(width, height, TYPE_INT_ARGB);
    }

    /**
     * Return the buffer data of the image.
     * Returned data are stored in 4 bytes (RGBA) per pixel.
     * @return the buffer data of the image.
     */
    public int[] getRGBAData() {
        int[] pixels = ((DataBufferInt) getRaster().getDataBuffer()).getData();

        for (int i = 0; i < pixels.length; i++) {
            pixels[i] = (pixels[i] & 0xFF00FF00) | ((pixels[i] << 16) & 0x00FF0000) | ((pixels[i] >> 16) & 0xFF);
        }

        //updateFrame(this);
        return pixels;
    }

    static JLabel label;
    private static void updateFrame(TextureBufferedImage textureBufferedImage) {
        if (label == null) {
            JFrame frame = new JFrame("Test");
            frame.setLayout(new BorderLayout());
            label = new JLabel();
            frame.add(label, BorderLayout.CENTER);
            frame.setSize(500, 500);
            frame.setVisible(true);
        }
        label.setIcon(new ImageIcon(textureBufferedImage));
    }


    /**
     * Buffered data getter.
     * @return a byte buffer filled with RGBA data.
     */
    public ByteBuffer getRGBABuffer() {
        int[] pixels = ((DataBufferInt) getRaster().getDataBuffer()).getData();
        ByteBuffer buffer = ByteBuffer.allocate(pixels.length * 4);
        buffer.order(ByteOrder.nativeOrder());
        for (int i = 0; i < pixels.length; i++) {
            buffer.putInt((pixels[i] & 0xFF00FF00) | ((pixels[i] << 16) & 0x00FF0000) | ((pixels[i] >> 16) & 0xFF));
        }
        buffer.rewind();

        return buffer;
    }
}
