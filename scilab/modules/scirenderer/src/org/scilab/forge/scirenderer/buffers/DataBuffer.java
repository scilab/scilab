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

import java.nio.Buffer;

/**
 * Interface for a data buffer.
 * @author Pierre Lando
 */
public interface DataBuffer {

    /**
     * Return the data.
     * @return the data.
     */
    Buffer getData();

    /**
     * Return the number of elements.
     * @return the number of elements.
     */
    int getSize();

    void clear();
}
