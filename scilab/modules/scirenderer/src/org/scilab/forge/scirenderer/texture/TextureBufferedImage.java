/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.forge.scirenderer.texture;

import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;
import java.awt.BorderLayout;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferInt;
import java.nio.ByteBuffer;

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
    public byte[] getRGBAData() {
        int[] pixels = ((DataBufferInt) getRaster().getDataBuffer()).getData();

        byte[] bytes = new byte[pixels.length * NB_COMPONENTS];
        int j = 0;
        for (int p : pixels) {
            bytes[j] = (byte) ((p >> R_SHIFT) & COMPONENT_MASK);
            bytes[j + 1] = (byte) ((p >> G_SHIFT) & COMPONENT_MASK);
            bytes[j + 2] = (byte) ((p >> B_SHIFT) & COMPONENT_MASK);
            bytes[j + 3] = (byte) ((p >> A_SHIFT) & COMPONENT_MASK);
            j += NB_COMPONENTS;
        }
        //updateFrame(this);
        return bytes;
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
        return ByteBuffer.wrap(getRGBAData());
    }
}
