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
