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
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.VectorOfScicosID;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.graph.ScicosParameters;
import org.scilab.modules.xcos.io.HandledElement;

public class CustomWriter extends ScilabWriter {
    private static final String SEP = " - ";

    public CustomWriter(XcosWriter writer) {
        super(writer);
    }

    @Override
    public void write(long uid, Kind kind) throws XMLStreamException {
        String[] str = new String[1];
        int[] integer = new int[1];
        VectorOfDouble vDouble = new VectorOfDouble();
        VectorOfScicosID children = new VectorOfScicosID();

        switch (kind) {
            case DIAGRAM:
                shared.stream.writeStartDocument();
                shared.stream.writeStartElement(HandledElement.XcosDiagram.name());

                /*
                 * Write diagram content
                 */
                shared.controller.getObjectProperty(uid, kind, ObjectProperties.DEBUG_LEVEL, integer);
                shared.stream.writeAttribute("debugLevel", Integer.toString(integer[0]));

                // write simulation properties
                shared.controller.getObjectProperty(uid, kind, ObjectProperties.PROPERTIES, vDouble);
                String prop;

                prop = Double.toString(vDouble.get(ScicosParameters.FINAL_INTEGRATION_TIME));
                shared.stream.writeAttribute("finalIntegrationTime", prop);
                prop = Double.toString(vDouble.get(ScicosParameters.INTEGRATOR_ABSOLUTE_TOLERANCE));
                shared.stream.writeAttribute("integratorAbsoluteTolerance", prop);
                prop = Double.toString(vDouble.get(ScicosParameters.INTEGRATOR_RELATIVE_TOLERANCE));
                shared.stream.writeAttribute("integratorRelativeTolerance", prop);
                prop = Double.toString(vDouble.get(ScicosParameters.TOLERANCE_ON_TIME));
                shared.stream.writeAttribute("toleranceOnTime", prop);
                prop = Double.toString(vDouble.get(ScicosParameters.MAX_INTEGRATION_TIME_INTERVAL));
                shared.stream.writeAttribute("maxIntegrationTimeInterval", prop);
                prop = Double.toString(vDouble.get(ScicosParameters.MAXIMUM_STEP_SIZE));
                shared.stream.writeAttribute("maximumStepSize", prop);
                prop = Double.toString(vDouble.get(ScicosParameters.REAL_TIME_SCALING));
                shared.stream.writeAttribute("realTimeScaling", prop);
                prop = Double.toString(vDouble.get(ScicosParameters.SOLVER));
                shared.stream.writeAttribute("solver", prop);

                // write the graphical part and children
                shared.controller.getObjectProperty(uid, kind, ObjectProperties.CHILDREN, children);
                String zeroUID = new UID((short) 0).toString();
                shared.uniqueUIDs.add(zeroUID);
                writeDiagramAndSuperDiagramContent(uid, kind, children, zeroUID);

                shared.stream.writeEndElement();
                shared.stream.writeEndDocument();

                // post processing for binary data handling
                shared.rawDataWriter.fillSharedDictionary();
                break;
            case BLOCK:
                shared.controller.getObjectProperty(uid, kind, ObjectProperties.CHILDREN, children);
                if (children.size() > 0) {
                    shared.stream.writeStartElement(HandledElement.SuperBlockDiagram.name());
                    shared.stream.writeAttribute("as", "child");
                    writeDiagramAndSuperDiagramContent(uid, kind, children, shared.layers.peek());
                    shared.stream.writeEndElement(); // SuperBlockDiagram
                }
                break;
            case PORT:
                // the orientation is not encoded on the model but propagated inside the style
                break;
            default:
                break;
        }
    }

    private void writeDiagramAndSuperDiagramContent(long uid, Kind kind, VectorOfScicosID children, String parentUID) throws XMLStreamException {
        VectorOfInt colors = new VectorOfInt();
        shared.controller.getObjectProperty(uid, kind, ObjectProperties.COLOR, colors);
        shared.stream.writeAttribute("background", Integer.toString(colors.get(0)));
        shared.stream.writeAttribute("gridEnabled", Integer.toString(colors.get(1)));

        String[] str = new String[1];
        shared.controller.getObjectProperty(uid, kind, ObjectProperties.TITLE, str);
        shared.stream.writeAttribute("title", str[0]);

        /*
         * Add a version comment at the top layer
         */
        if (kind == Kind.DIAGRAM) {
            final Package p = Package.getPackage("org.scilab.modules.xcos");
            String comment = new StringBuilder().append(Xcos.TRADENAME).append(SEP).append(Xcos.VERSION).append(SEP)
            .append(p.getSpecificationVersion()).append(SEP).append(p.getImplementationVersion()).toString();
            shared.stream.writeComment(comment);
        }

        /*
         * encode some content then the children
         */
        shared.rawDataWriter.write(uid, kind);

        /*
         * Generate uniques but predictables UIDs
         */
        String[] parent = parentUID.split(":");
        long uidCounter = 1;
        String rootUID = parent[0] + ":" + Long.toString(Long.parseLong(parent[1], 16) + uidCounter, 16) + ":" + parent[2];
        for (; shared.uniqueUIDs.contains(rootUID); uidCounter++) {
            rootUID = parent[0] + ":" + Long.toString(Long.parseLong(parent[1], 16) + uidCounter, 16) + ":" + parent[2];
        }
        shared.uniqueUIDs.add(rootUID);
        String layerUID = parent[0] + ":" + Long.toString(Long.parseLong(parent[1], 16) + uidCounter, 16) + ":" + parent[2];
        for (; shared.uniqueUIDs.contains(layerUID); uidCounter++) {
            layerUID = parent[0] + ":" + Long.toString(Long.parseLong(parent[1], 16) + uidCounter, 16) + ":" + parent[2];
        }
        shared.uniqueUIDs.add(layerUID);
        shared.layers.push(layerUID);

        // children header
        shared.stream.writeStartElement("mxGraphModel");
        shared.stream.writeAttribute("as", "model");
        shared.stream.writeStartElement("root");
        shared.stream.writeEmptyElement("mxCell");
        shared.stream.writeAttribute("id", rootUID);
        shared.stream.writeEmptyElement("mxCell");
        shared.stream.writeAttribute("id", layerUID);
        shared.stream.writeAttribute("parent", rootUID);

        // loop on all children, encode the blocks and ports then the links
        VectorOfInt kinds = new VectorOfInt();
        shared.controller.sortAndFillKind(children, kinds);

        final int len = children.size();
        for (int i = 0; i < len; i++) {
            long child = children.get(i);
            Kind childKind = Kind.values()[kinds.get(i)];
            shared.write(child, childKind);
        }

        shared.layers.pop();
        shared.stream.writeEndElement(); // root
        shared.stream.writeEndElement(); // mxGraphModel
        shared.stream.writeEmptyElement("mxCell");
        shared.stream.writeAttribute("as", "defaultParent");
        shared.stream.writeAttribute("id", layerUID);
        shared.stream.writeAttribute("parent", rootUID);
    }
}
