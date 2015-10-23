/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.io.writer;

import javax.xml.stream.XMLStreamException;

import org.scilab.modules.xcos.Kind;

abstract class ScilabWriter {
    protected final XcosWriter shared;

    public ScilabWriter(XcosWriter writer) {
        this.shared = writer;
    }

    /**
     * Write the uid object to the stream
     *
     * @param uid the uid to serialize
     * @param kind the kind of uid
     * @throws XMLStreamException on write error
     */
    public abstract void write(long uid, Kind kind) throws XMLStreamException;
}
