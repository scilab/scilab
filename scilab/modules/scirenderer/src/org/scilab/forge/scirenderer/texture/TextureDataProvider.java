/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.texture;

import org.scilab.forge.scirenderer.data.DataProvider;

import java.awt.Dimension;
import java.awt.image.BufferedImage;
import java.nio.ByteBuffer;

/**
 * @author Pierre Lando
 */
public interface TextureDataProvider extends DataProvider<Texture> {

    public enum ImageType {
        RGB, RGB_RGBA, BGR, GRAY, GRAY_16, RGBA, RGBA_REV, ABGR, RGB_332, RED, GREEN, BLUE, INTENSITY, RGBA_4444, RGBA_5551, RGB_FLOAT, RGBA_FLOAT, GRAY_FLOAT, RED_16, GREEN_16, BLUE_16, RED_FLOAT, GREEN_FLOAT, BLUE_FLOAT, RGBA_BYTE;

        public static ImageType fromInt(int n) {
            switch (n) {
                case 0:
                    return RGB;
                case 1:
                    return RGB_RGBA;
                case 2:
                    return BGR;
                case 3:
                    return GRAY;
                case 4:
                    return GRAY_16;
                case 5:
                    return RGBA;
                case 6:
                    return RGBA_REV;
                case 7:
                    return ABGR;
                case 8:
                    return RGB_332;
                case 9:
                    return RED;
                case 10:
                    return GREEN;
                case 11:
                    return BLUE;
                case 12:
                    return INTENSITY;
                case 13:
                    return RGBA_4444;
                case 14:
                    return RGBA_5551;
                case 15:
                    return RGB_FLOAT;
                case 16:
                    return RGBA_FLOAT;
                case 17:
                    return GRAY_FLOAT;
                case 18:
                    return RED_16;
                case 19:
                    return GREEN_16;
                case 20:
                    return BLUE_16;
                case 21:
                    return RED_FLOAT;
                case 22:
                    return GREEN_FLOAT;
                case 23:
                    return BLUE_FLOAT;
                case 24:
                    return RGBA_BYTE;
                default:
                    return GRAY;
            }
        }
    }

    boolean isRowMajorOrder();

    ImageType getImageType();

    /**
     * Texture size getter.
     * @return the size of the texture in pixel.
     */
    Dimension getTextureSize();

    /**
     * Data getter.
     * @return the data.
     */
    ByteBuffer getData();

    /**
     * Sub-data getter.
     * @param x rectangle origin x-coordinate.
     * @param y rectangle origin y-coordinate.
     * @param width rectangle width.
     * @param height rectangle height.
     * @return the data.
     */
    ByteBuffer getSubData(int x, int y, int width, int height);

    /**
     * @return the data as a BufferedImage
     */
    BufferedImage getImage();

    /**
     * @param x rectangle origin x-coordinate.
     * @param y rectangle origin y-coordinate.
     * @param width rectangle width.
     * @param height rectangle height.
     * @return the sub-data as a BufferedImage.
     */
    BufferedImage getSubImage(int x, int y, int width, int height);
}
