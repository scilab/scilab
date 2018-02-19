/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015-2017 - Scilab Enterprises - Clement DAVID
 * Copyright (C) 2017 - ESI Group - Clement DAVID
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
import org.scilab.modules.xcos.ObjectProperties;

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

    /**
     * Write the label annotation of an object
     * @param parentUID the uid to serialize (object with a label).
     * @param parentKind the kind of uid
     */
    public final void writeAnnotationCell(long parentUID, Kind parentKind) throws XMLStreamException {
        String[] v = {""};

        // write the label as a custom mxCell
        long[] label = { 0 };
        shared.controller.getObjectProperty(parentUID, parentKind, ObjectProperties.LABEL, label);
        if (label[0] != 0) {
            shared.stream.writeStartElement("mxCell");

            shared.stream.writeAttribute("connectable", "0");

            shared.controller.getObjectProperty(label[0], Kind.ANNOTATION, ObjectProperties.UID, v);
            shared.stream.writeAttribute("id", v[0]);

            shared.controller.getObjectProperty(label[0],  Kind.ANNOTATION, ObjectProperties.STYLE, v);
            shared.stream.writeAttribute("style", v[0]);

            shared.controller.getObjectProperty(label[0],  Kind.ANNOTATION, ObjectProperties.DESCRIPTION, v);

            // remove any '\n' character that will not be preserved by the XML Handlers on re-load
            String escaped = v[0].replace('\n', ' ');
            shared.stream.writeAttribute("value", escaped);

            shared.stream.writeAttribute("vertex", "1");

            new JGraphXWriter(shared).write(label[0], Kind.ANNOTATION);
            shared.stream.writeEndElement();
        }
    }
}
