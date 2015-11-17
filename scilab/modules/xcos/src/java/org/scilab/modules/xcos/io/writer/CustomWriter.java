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

import java.util.UUID;

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

                shared.controller.getObjectProperty(uid, kind, ObjectProperties.PATH, str);
                shared.stream.writeAttribute("savedFile", str[0]);
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
                writeDiagramAndSuperDiagramContent(uid, kind, children);

                shared.stream.writeEndElement();
                shared.stream.writeEndDocument();

                // post processing for binary data handling
                shared.rawDataWriter.fillSharedDictionary();
                break;
            case BLOCK:
                shared.controller.getObjectProperty(uid, kind, ObjectProperties.CHILDREN, children);
                if (children.size() > 0) {
                    shared.stream.writeStartElement(HandledElement.SuperBlockDiagram.name());
                    writeDiagramAndSuperDiagramContent(uid, kind, children);
                    shared.stream.writeEndElement(); // SuperBlockDiagram
                }
                break;
            case PORT:
                // FIXME encode orientation from the model or not?
                break;
            default:
                break;
        }
    }

    private void writeDiagramAndSuperDiagramContent(long uid, Kind kind, VectorOfScicosID children) throws XMLStreamException {

        VectorOfInt colors = new VectorOfInt();
        shared.controller.getObjectProperty(uid, kind, ObjectProperties.COLOR, colors);
        shared.stream.writeAttribute("background", Integer.toString(colors.get(0)));
        shared.stream.writeAttribute("gridEnabled", Integer.toString(colors.get(1)));

        String[] str = new String[1];
        shared.controller.getObjectProperty(uid, kind, ObjectProperties.TITLE, str);
        shared.stream.writeAttribute("title", str[0]);

        /*
         * Add the legacy version comment
         */
        final Package p = Package.getPackage("org.scilab.modules.xcos");
        StringBuilder comment = new StringBuilder().append(Xcos.TRADENAME).append(SEP).append(Xcos.VERSION).append(SEP)
        .append(p.getSpecificationVersion()).append(SEP).append(p.getImplementationVersion());
        shared.stream.writeComment(comment.toString());

        /*
         * encode some content then the children
         */
        shared.rawDataWriter.write(uid, kind);

        UUID root = UUID.randomUUID();
        UUID layer = UUID.randomUUID();
        shared.layers.push(layer.toString());

        // children header
        shared.stream.writeStartElement("mxGraphModel");
        shared.stream.writeAttribute("as", "model");
        shared.stream.writeStartElement("root");
        shared.stream.writeEmptyElement("mxCell");
        shared.stream.writeAttribute("id", root.toString());
        shared.stream.writeEmptyElement("mxCell");
        shared.stream.writeAttribute("id", shared.layers.peek());
        shared.stream.writeAttribute("parent", root.toString());

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
    }
}
