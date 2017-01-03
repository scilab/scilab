/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.utils;

import java.awt.image.BufferedImage;

import javax.swing.ImageIcon;

import org.apache.batik.transcoder.TranscoderException;
import org.apache.batik.transcoder.TranscoderOutput;
import org.apache.batik.transcoder.image.ImageTranscoder;

/**
 * A simple transcoder that can be used to generate ImageIcon and BufferedImage.
 * @author Marcos Cardinot <mcardinot@gmail.com>
 */
public class ImageIconTranscoder extends ImageTranscoder {
    private BufferedImage bufferedImage;
    private ImageIcon imgIcon;

    /**
     * Default constructor
     */
    public ImageIconTranscoder() {
    }

    /**
     * Creates a new ARGB image with the specified dimension.
     * @param width the image width
     * @param height the image height
     * @return BufferedImage
     */
    @Override
    public BufferedImage createImage(int width, int height) {
        return new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
    }

    /**
     * Writes a image.
     * @param img the image to write
     * @param output the output where to store the image
     * @throws TranscoderException if an error occurred while storing the image
     */
    @Override
    public void writeImage(BufferedImage img, TranscoderOutput output) throws TranscoderException {
        bufferedImage = img;
        imgIcon = new ImageIcon(img);
    }

    /**
     * Get the image in BufferedImage format
     * @return BufferedImage
     */
    public BufferedImage getBufferedImage() {
        return bufferedImage;
    }

    /**
     * Get the image in ImageIcon format
     * @return ImageIcon
     */
    public ImageIcon getImageIcon() {
        return imgIcon;
    }

    /**
     * Set the dimension of the image.
     * @param width the image width
     * @param height the image height
     */
    public void setDimensions(int width, int height) {
        hints.put(KEY_WIDTH, new Float(width));
        hints.put(KEY_HEIGHT, new Float(height));
    }
}
