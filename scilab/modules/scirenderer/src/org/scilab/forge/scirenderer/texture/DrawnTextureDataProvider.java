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

import org.scilab.forge.scirenderer.data.AbstractDataProvider;

import java.awt.Dimension;
import java.awt.image.BufferedImage;
import java.nio.ByteBuffer;

/**
 * @author Pierre Lando
 */
public class DrawnTextureDataProvider extends AbstractDataProvider<Texture> implements TextureDataProvider {

    /** Texture drawer */
    private TextureDrawer textureDrawer;
    protected ImageType imageType;

    /** Current image */
    BufferedImageTextureDrawingTools image;

    public DrawnTextureDataProvider(TextureDrawer textureDrawer) {
        this.textureDrawer = textureDrawer;
        imageType = ImageType.RGBA_BYTE;
        //reDraw();
    }

    @Override
    public ImageType getImageType() {
        return imageType;
    }

    @Override
    public boolean isRowMajorOrder() {
        return true;
    }

    /** Reload the texture and recall the texture drawing tools. */
    public void reDraw() {
        if (isValid()) {
            image = new BufferedImageTextureDrawingTools(textureDrawer.getTextureSize());
            image.accept(textureDrawer);
        }
    }

    /**
     * Texture drawer setter.
     * @param textureDrawer the new texture drawer.
     */
    public void setTextureDrawingTools(TextureDrawer textureDrawer) {
        this.textureDrawer = textureDrawer;
        reDraw();
    }

    @Override
    public Dimension getTextureSize() {
        if (isValid()) {
            return textureDrawer.getTextureSize();
        } else {
            return new Dimension(-1, -1);
        }
    }

    @Override
    public ByteBuffer getData() {
        if (isValid()) {
            if (image == null) {
                reDraw();
            }
            return image.getImage().getRGBABuffer();
        } else {
            return null;
        }
    }

    @Override
    public ByteBuffer getSubData(int x, int y, int width, int height) {
        if (isValid()) {
            ByteBuffer buffer = getData();
            return buffer;
            /*
              ByteBuffer tempBuffer = ByteBuffer.allocate(4 * width * height);
              byte[] data = new byte[4 * height];
              for (int j = y; j < y + height; j++) {
              buffer.position(4 * (x + j * getTextureSize().width));
              buffer.get(data);
              tempBuffer.put(data);
              }
              tempBuffer.rewind();
              buffer.rewind();
              return tempBuffer;
            */
        } else {
            return null;
        }
    }

    @Override
    public BufferedImage getImage() {
        if (isValid()) {
            if (image == null) {
                reDraw();
            }
            return image.getImage();
        } else {
            return null;
        }
    }

    @Override
    public BufferedImage getSubImage(int x, int y, int width, int height) {
        if (isValid()) {
            BufferedImage image = getImage();
            return image.getSubimage(x, y, width, height);
        } else {
            return null;
        }
    }

    @Override
    public boolean isValid() {
        return textureDrawer != null;
    }
}
