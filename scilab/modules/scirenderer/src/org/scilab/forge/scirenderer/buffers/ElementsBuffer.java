/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.buffers;

import java.nio.FloatBuffer;

/**
 * @author Pierre Lando
 */
public interface ElementsBuffer extends DataBuffer {

    /**
     * Set the data.
     * @param data the new data.
     * @param elementSize the size of data elements.
     */
    void setData(float[] data, int elementSize);

    /**
     * Set the data.
     * @param data the new data.
     * @param elementSize the size of data elements.
     */
    void setData(Float[] data, int elementSize);

    /**
     * Set the data.
     * @param data the new data.
     * @param elementsSize the size of data elements.
     */
    void setData(FloatBuffer data, int elementsSize);

    @Override
    FloatBuffer getData();

    /**
     * Return the number of coordinate for one element.
     * @return the number of coordinate for one element.
     */
    int getElementsSize();

    void clear();
}
