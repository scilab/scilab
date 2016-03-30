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
