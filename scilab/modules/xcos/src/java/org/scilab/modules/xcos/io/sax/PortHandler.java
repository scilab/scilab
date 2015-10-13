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

package org.scilab.modules.xcos.io.sax;

import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.VectorOfScicosID;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.xml.sax.Attributes;

class PortHandler implements ScilabHandler {

    private final SAXHandler saxHandler;

    /**
     * Default constructor
     * @param saxHandler the shared sax handler
     */
    PortHandler(SAXHandler saxHandler) {
        this.saxHandler = saxHandler;
    }

    public BasicPort startElement(HandledElement found, Attributes atts) {
        BasicPort port;
        ObjectProperties relatedProperty;
        final long uid = saxHandler.controller.createObject(Kind.PORT);

        /*
         * Allocate the port with the right class to set default properties
         */

        switch (found) {
            case CommandPort:
                port = new CommandPort(uid);
                relatedProperty = ObjectProperties.EVENT_OUTPUTS;
                break;
            case ControlPort:
                port = new ControlPort(uid);
                relatedProperty = ObjectProperties.EVENT_INPUTS;
                break;
            case ExplicitInputPort:
                port = new ExplicitInputPort(uid);
                relatedProperty = ObjectProperties.INPUTS;
                break;
            case ExplicitOutputPort:
                port = new ExplicitOutputPort(uid);
                relatedProperty = ObjectProperties.OUTPUTS;
                break;
            case ImplicitInputPort:
                port = new ImplicitInputPort(uid);
                relatedProperty = ObjectProperties.INPUTS;
                break;
            case ImplicitOutputPort:
                port = new ImplicitOutputPort(uid);
                relatedProperty = ObjectProperties.OUTPUTS;
                break;
            default:
                throw new IllegalArgumentException();
        }

        /*
         * Setup the properties
         */
        String v;

        v = atts.getValue("id");
        if (v != null) {
            saxHandler.allChildren.peek().put(v, uid);
        }

        v = atts.getValue("style");
        if (v != null) {
            saxHandler.controller.setObjectProperty(uid, Kind.PORT, ObjectProperties.STYLE, v);
        }

        VectorOfInt datatype = new VectorOfInt();
        saxHandler.controller.getObjectProperty(uid, Kind.PORT, ObjectProperties.DATATYPE, datatype);

        v = atts.getValue("dataType");
        if (v != null) {
            datatype.set(0, BasicPort.DataType.valueOf(v).ordinal());
        }
        v = atts.getValue("dataColumns");
        if (v != null) {
            datatype.set(1, Integer.valueOf(v));
        }
        v = atts.getValue("dataLines");
        if (v != null) {
            datatype.set(2, Integer.valueOf(v));
        }

        saxHandler.controller.setObjectProperty(uid, Kind.PORT, ObjectProperties.DATATYPE, datatype);

        v = atts.getValue("initialState");
        if (v != null) {
            saxHandler.controller.setObjectProperty(uid, Kind.PORT, ObjectProperties.FIRING, Double.valueOf(v));
        }

        /*
         * Associate to the parent block
         */

        int ordering = 0;
        long parent = 0l;

        v = atts.getValue("ordering");
        if (v != null) {
            ordering = Integer.valueOf(v) - 1;
        }

        v = atts.getValue("parent");
        if (v != null) {
            parent = saxHandler.allChildren.peek().get(v);
        }

        VectorOfScicosID associatedPorts = new VectorOfScicosID();
        saxHandler.controller.getObjectProperty(parent, Kind.BLOCK, relatedProperty, associatedPorts);

        associatedPorts.resize(ordering + 1);
        associatedPorts.set(ordering, uid);
        saxHandler.controller.referenceObject(uid);

        saxHandler.controller.setObjectProperty(uid, Kind.PORT, ObjectProperties.SOURCE_BLOCK, parent);
        saxHandler.controller.setObjectProperty(parent, Kind.BLOCK, relatedProperty, associatedPorts);

        return port;
    }

    public void endElement(HandledElement found) {
    }
}