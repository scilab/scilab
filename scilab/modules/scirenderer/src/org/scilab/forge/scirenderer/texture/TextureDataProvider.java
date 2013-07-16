/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
