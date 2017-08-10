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

package org.scilab.forge.scirenderer.texture;

import java.awt.Dimension;
import java.awt.image.BufferedImage;
import java.nio.ByteBuffer;

import org.scilab.forge.scirenderer.data.AbstractDataProvider;

/**
 * @author Calixte DENIZET
 */
public abstract class AbstractTextureDataProvider extends AbstractDataProvider<Texture> implements TextureDataProvider {

    protected ImageType imageType;

    @Override
    public ImageType getImageType() {
        return imageType;
    }

    @Override
    public boolean isRowMajorOrder() {
        return true;
    }

    @Override
    public BufferedImage getImage() {
        final ByteBuffer data = getData();
        if (data == null) {
            return null;
        }

        data.rewind();
        Dimension size = getTextureSize();
        final int width = size.width;
        final int height = size.height;

        final int[] ibuffer = new int[data.capacity() / 4];
        final byte[] RGBA = new byte[4];
        for (int i = 0; i < ibuffer.length; i++) {
            data.get(RGBA);
            ibuffer[i] = ((RGBA[3] & 0xFF) << 24) + ((RGBA[0] & 0xFF) << 16) + ((RGBA[1] & 0xFF) << 8) + (RGBA[2] & 0xFF);
        }
        data.rewind();
        BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
        image.setRGB(0, 0, width, height, ibuffer, 0, width);

        return image;
    }

    @Override
    public BufferedImage getSubImage(int x, int y, int width, int height) {
        BufferedImage image = getImage();
        if (image == null) {
            return null;
        }

        return image.getSubimage(x, y, width, height);
    }
}
