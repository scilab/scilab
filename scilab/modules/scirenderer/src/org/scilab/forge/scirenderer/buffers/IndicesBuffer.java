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

import java.nio.IntBuffer;
import java.util.Collection;

/**
 * @author Pierre Lando
 */
public interface IndicesBuffer extends DataBuffer {

    /**
     * Set the data
     * @param indices the new data.
     */
    void setData(int[] indices);


    /**
     * Set the data
     * @param indices the new data.
     */
    void setData(Collection<Integer> indices);

    /**
     * Set the data.
     * @param indexBuffer the new data.
     */
    void setData(IntBuffer indexBuffer);

    @Override
    IntBuffer getData();


    void clear();
}
