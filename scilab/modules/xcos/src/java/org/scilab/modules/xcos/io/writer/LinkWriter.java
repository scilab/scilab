/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015-2015 - Scilab Enterprises - Clement DAVID
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

import java.rmi.server.UID;
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

            while (id[0].isEmpty() || shared.uniqueUIDs.contains(id[0])) {
                id[0] = new UID().toString();
            }
            shared.controller.setObjectProperty(uid, kind, ObjectProperties.UID, id[0]);
            shared.stream.writeAttribute("id", id[0]);

            shared.stream.writeAttribute("parent", shared.layers.peek());

            shared.stream.writeAttribute("source", src[0]);
            shared.stream.writeAttribute("target", dst[0]);
            shared.stream.writeAttribute("style", style[0]);
            // remove any '\n' character that will not be preserved by the XML Handlers on re-load
            String escaped = value[0].replace('\n', ' ');
            shared.stream.writeAttribute("value", escaped);

            new JGraphXWriter(shared).write(uid, kind);

            shared.stream.writeEndElement(); // localName
        }
    }
}
