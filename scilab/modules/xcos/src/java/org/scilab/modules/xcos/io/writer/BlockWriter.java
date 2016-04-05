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
import java.util.EnumSet;

import javax.xml.stream.XMLStreamException;

import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.VectorOfScicosID;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BasicBlock.SimulationFunctionType;
import org.scilab.modules.xcos.graph.model.BlockInterFunction;
import org.scilab.modules.xcos.graph.model.XcosCellFactory;

public class BlockWriter extends ScilabWriter {

    public BlockWriter(XcosWriter writer) {
        super(writer);
    }

    @Override
    public void write(long uid, Kind kind) throws XMLStreamException {
        // Lookup for the element name
        String[] interfaceFunction = new String[1];
        BlockInterFunction interFunction;
        if (kind == Kind.BLOCK) {
            shared.controller.getObjectProperty(uid, kind, ObjectProperties.INTERFACE_FUNCTION, interfaceFunction);
            interFunction = XcosCellFactory.lookForInterfunction(interfaceFunction[0]);
        } else {
            interfaceFunction[0] = BlockInterFunction.TEXT_f.name();
            interFunction = BlockInterFunction.TEXT_f;
        }

        shared.stream.writeStartElement(interFunction.getKlass().getSimpleName());

        String[] v = new String[1];

        shared.controller.getObjectProperty(uid, kind, ObjectProperties.UID, v);
        while (isInvalidUID(v[0])) {
            v[0] = new UID().toString();
        }
        shared.controller.setObjectProperty(uid, kind, ObjectProperties.UID, v[0]);
        shared.stream.writeAttribute("id", v[0]);
        shared.stream.writeAttribute("parent", shared.layers.peek());
        shared.layers.push(v[0]);

        shared.stream.writeAttribute("interfaceFunctionName", interfaceFunction[0]);

        if (kind == Kind.BLOCK) {
            shared.controller.getObjectProperty(uid, kind, ObjectProperties.SIM_BLOCKTYPE, v);
            shared.stream.writeAttribute("blockType", v[0]);

            VectorOfInt vecOfInt = new VectorOfInt(2);
            shared.controller.getObjectProperty(uid, kind, ObjectProperties.SIM_DEP_UT, vecOfInt);
            shared.stream.writeAttribute("dependsOnU", Integer.toString(vecOfInt.get(0)));
            shared.stream.writeAttribute("dependsOnT", Integer.toString(vecOfInt.get(1)));

            shared.controller.getObjectProperty(uid, kind, ObjectProperties.SIM_FUNCTION_NAME, v);
            shared.stream.writeAttribute("simulationFunctionName", v[0]);

            int[] functionAPI = new int[1];
            shared.controller.getObjectProperty(uid, kind, ObjectProperties.SIM_FUNCTION_API, functionAPI);
            SimulationFunctionType type = BasicBlock.SimulationFunctionType.convertScilabValue(functionAPI[0]);
            shared.stream.writeAttribute("simulationFunctionType", type.name());
        }

        shared.controller.getObjectProperty(uid, kind, ObjectProperties.STYLE, v);
        shared.stream.writeAttribute("style", v[0]);

        shared.controller.getObjectProperty(uid, kind, ObjectProperties.DESCRIPTION, v);
        if (!v[0].isEmpty()) {
            // remove any '\n' character that will not be preserved by the XML Handlers on re-load
            String escaped = v[0].replace('\n', ' ');
            shared.stream.writeAttribute("value", escaped);
        }

        // delegate the sub-elements to their writers
        shared.rawDataWriter.write(uid, kind);
        new JGraphXWriter(shared).write(uid, kind);
        new CustomWriter(shared).write(uid, kind);

        shared.stream.writeEndElement(); // Block name

        /*
         * We serialize the ports on the inner layer but close it before to
         * have a flat object view per diagram, this is Scilab 5 compatible.
         */

        // serialize the ports after the block
        VectorOfScicosID ports = new VectorOfScicosID();
        final EnumSet<ObjectProperties> portKind = EnumSet.of(ObjectProperties.INPUTS, ObjectProperties.OUTPUTS,
                ObjectProperties.EVENT_INPUTS, ObjectProperties.EVENT_OUTPUTS);
        for (ObjectProperties p : portKind) {
            shared.controller.getObjectProperty(uid, kind, p, ports);
            PortWriter portWriter = new PortWriter(shared, p);

            final int length = ports.size();
            for (int i = 0; i < length; i++) {
                portWriter.write(ports.get(i), Kind.PORT);
            }
        }

        shared.layers.pop();
    }

    private boolean isInvalidUID(String uid) {
        if (uid.isEmpty()) {
            return true;
        }
        if (shared.uniqueUIDs.contains(uid)) {
            return true;
        }

        // more advanced check to remove non-generated UID without loosing performance
        // a valid one is in the form : "-5151d0b0:12dcdfdd360:-7fff"
        if (3 < uid.length() && uid.length() < 28) {
            // more costly operations there but there is no allocation
            int firstSep = uid.indexOf(':', 0);
            if (firstSep <= 0) {
                return true;
            }

            int secondSep = uid.indexOf(':', firstSep + 1);
            if (secondSep <= firstSep + 1) {
                return true;
            }

            return false;
        } else {
            return true;
        }
    }




}
