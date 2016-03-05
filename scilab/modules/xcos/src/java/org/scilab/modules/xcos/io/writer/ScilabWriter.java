/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Clement DAVID
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
