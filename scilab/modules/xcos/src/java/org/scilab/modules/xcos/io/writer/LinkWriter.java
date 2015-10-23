/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015-2015 - Scilab Enterprises - Clement DAVID
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
import org.scilab.modules.xcos.ObjectProperties;

public class LinkWriter extends ScilabWriter {

    public LinkWriter(XcosWriter writer) {
        super(writer);
    }

    @Override
    public void write(long uid, Kind kind) throws XMLStreamException {
        assert kind == Kind.LINK;

        int[] linkKind = new int[1];
        shared.controller.getObjectProperty(uid, kind, ObjectProperties.KIND, linkKind);

        String localName;
        switch (linkKind[0]) {
            case -1:
                localName = "CommandControlLink";
                break;
            case 1:
                localName = "ExplicitLink";
                break;
            case 2:
                localName = "ImplicitLink";
                break;
            default:
                return;
        }

        String[] id = new String[1];
        long[] scicosId = new long[1];

        String[] src = new String[1];
        String[] dst = new String[1];
        String[] style = new String[1];
        String[] value = new String[1];

        /*
         * Retrieve all the serialized info
         */
        shared.controller.getObjectProperty(uid, kind, ObjectProperties.UID, id);

        shared.controller.getObjectProperty(uid, kind, ObjectProperties.SOURCE_PORT, scicosId);
        shared.controller.getObjectProperty(scicosId[0], Kind.PORT, ObjectProperties.UID, src);

        shared.controller.getObjectProperty(uid, kind, ObjectProperties.DESTINATION_PORT, scicosId);
        shared.controller.getObjectProperty(scicosId[0], Kind.PORT, ObjectProperties.UID, dst);

        shared.controller.getObjectProperty(uid, kind, ObjectProperties.STYLE, style);
        shared.controller.getObjectProperty(uid, kind, ObjectProperties.LABEL, value);

        /*
         * Only serialized a fully connected link
         */
        if (!src[0].isEmpty() && !dst[0].isEmpty()) {
            shared.stream.writeStartElement(localName);

            shared.stream.writeAttribute("id", id[0]);
            shared.stream.writeAttribute("parent", shared.layers.peek());

            shared.stream.writeAttribute("source", src[0]);
            shared.stream.writeAttribute("target", dst[0]);
            shared.stream.writeAttribute("style", style[0]);
            shared.stream.writeAttribute("value", value[0]);

            shared.stream.writeEndElement(); // localName
        }
    }
}
