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
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.port.BasicPort;

public class PortWriter extends ScilabWriter {

    private final ObjectProperties portKind;
    private int ordering;

    public PortWriter(XcosWriter writer, ObjectProperties portKind) {
        super(writer);

        this.portKind = portKind;
        this.ordering = 0;
    }

    @Override
    public void write(long uid, Kind kind) throws XMLStreamException {
        assert kind == Kind.PORT;

        String localName;
        boolean[] isImplicit = new boolean[1];

        // retrieve some properties before emiting the element
        shared.controller.getObjectProperty(uid, Kind.PORT, ObjectProperties.IMPLICIT, isImplicit);

        switch (portKind) {
            case INPUTS:
                if (isImplicit[0]) {
                    localName = "ImplicitInputPort";
                } else {
                    localName = "ExplicitInputPort";
                }
                break;
            case OUTPUTS:
                if (isImplicit[0]) {
                    localName = "ImplicitOutputPort";
                } else {
                    localName = "ExplicitOutputPort";
                }
                break;
            case EVENT_INPUTS:
                localName = "ControlPort";
                break;
            case EVENT_OUTPUTS:
                localName = "CommandPort";
                break;

            default:
                return;
        }

        /*
         * Emit the XML
         */
        String[] str = new String[1];

        // the Geometry does not need to be saved, serialize only attributes there
        shared.stream.writeEmptyElement(localName);

        shared.controller.getObjectProperty(uid, kind, ObjectProperties.UID, str);
        while (str[0].isEmpty() || shared.uniqueUIDs.contains(str[0])) {
            str[0] = new UID().toString();
        }
        shared.controller.setObjectProperty(uid, kind, ObjectProperties.UID, str[0]);
        shared.stream.writeAttribute("id", str[0]);

        shared.stream.writeAttribute("parent", shared.layers.peek());
        shared.stream.writeAttribute("ordering", Integer.toString(++ordering));

        VectorOfInt datatype = new VectorOfInt();
        shared.controller.getObjectProperty(uid, Kind.PORT, ObjectProperties.DATATYPE, datatype);

        int type = datatype.get(2);
        if (0 <= type && type < BasicPort.DataType.values().length) {
            shared.stream.writeAttribute("dataType", BasicPort.DataType.values()[type].name());
            shared.stream.writeAttribute("dataColumns", Integer.toString(datatype.get(1)));
            shared.stream.writeAttribute("dataLines", Integer.toString(datatype.get(0)));
        }

        double[] firing = new double[1];
        shared.controller.getObjectProperty(uid, Kind.PORT, ObjectProperties.FIRING, firing);
        shared.stream.writeAttribute("initialState", Double.toString(firing[0]));

        String[] style = new String[1];
        shared.controller.getObjectProperty(uid, Kind.PORT, ObjectProperties.STYLE, style);
        shared.stream.writeAttribute("style", style[0]);
    }
}
