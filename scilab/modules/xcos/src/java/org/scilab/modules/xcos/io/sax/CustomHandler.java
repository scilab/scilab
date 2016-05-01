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

package org.scilab.modules.xcos.io.sax;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map.Entry;
import java.util.Optional;
import java.util.logging.Level;
import java.util.stream.Stream;

import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.graph.ScicosParameters;
import org.scilab.modules.xcos.graph.model.XcosCell;
import org.scilab.modules.xcos.graph.model.XcosCellFactory;
import org.scilab.modules.xcos.io.HandledElement;
import org.scilab.modules.xcos.io.sax.XcosSAXHandler.UnresolvedReference;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;
import org.xml.sax.Attributes;

class CustomHandler implements ScilabHandler {

    private final XcosSAXHandler saxHandler;

    /**
     * Default constructor
     *
     * @param saxHandler
     *            the shared sax handler
     */
    CustomHandler(XcosSAXHandler saxHandler) {
        this.saxHandler = saxHandler;
    }

    @Override
    @SuppressWarnings({ "fallthrough" })
    public Object startElement(HandledElement found, final Attributes atts) {
        long uid = 0l;
        String v;

        switch (found) {
            case Orientation: {
                // defensive programming
                if (!(saxHandler.parents.peek() instanceof BasicPort)) {
                    return null;
                }
                BasicPort parent = (BasicPort) saxHandler.parents.peek();

                v = atts.getValue("value");
                if (v != null) {
                    final String orientationString = v;
                    Optional<Orientation> orientation = Stream.of(Orientation.values()).filter(o -> o.name().equals(orientationString)).findFirst();
                    if (orientation.isPresent()) {
                        parent.setOrientation(orientation.get());
                    }
                }

                return null;
            }
            case XcosDiagram:
                if (XcosSAXHandler.LOG.isLoggable(Level.FINER)) {
                    XcosSAXHandler.LOG.entering(CustomHandler.class.getName(), "startElement(\"XcosDiagram\", ...)");
                }

                // do not allocate this is already allocated as #root
                uid = saxHandler.root.getUID();

                /*
                 * Decode some graph properties
                 */
                // the legacy savedFile attribute is removed to avoid any diff between two saved files with the same content
                v = atts.getValue("debugLevel");
                if (v != null) {
                    saxHandler.controller.setObjectProperty(uid, Kind.DIAGRAM, ObjectProperties.DEBUG_LEVEL, Integer.valueOf(v));
                }

                /*
                 * Decode simulation properties
                 */
                VectorOfDouble properties = new VectorOfDouble();
                saxHandler.controller.getObjectProperty(uid, Kind.DIAGRAM, ObjectProperties.PROPERTIES, properties);

                v = atts.getValue("finalIntegrationTime");
                if (v != null) {
                    properties.set(ScicosParameters.FINAL_INTEGRATION_TIME, Double.valueOf(v));
                }
                v = atts.getValue("integratorAbsoluteTolerance");
                if (v != null) {
                    properties.set(ScicosParameters.INTEGRATOR_ABSOLUTE_TOLERANCE, Double.valueOf(v));
                }
                v = atts.getValue("integratorRelativeTolerance");
                if (v != null) {
                    properties.set(ScicosParameters.INTEGRATOR_RELATIVE_TOLERANCE, Double.valueOf(v));
                }
                v = atts.getValue("toleranceOnTime");
                if (v != null) {
                    properties.set(ScicosParameters.TOLERANCE_ON_TIME, Double.valueOf(v));
                }
                v = atts.getValue("maxIntegrationTimeinterval");
                if (v != null) {
                    properties.set(ScicosParameters.MAX_INTEGRATION_TIME_INTERVAL, Double.valueOf(v));
                }
                v = atts.getValue("maximumStepSize");
                if (v != null) {
                    properties.set(ScicosParameters.MAXIMUM_STEP_SIZE, Double.valueOf(v));
                }
                v = atts.getValue("realTimeScaling");
                if (v != null) {
                    properties.set(ScicosParameters.REAL_TIME_SCALING, Double.valueOf(v));
                }
                v = atts.getValue("solver");
                if (v != null) {
                    properties.set(ScicosParameters.SOLVER, Double.valueOf(v));
                }

                saxHandler.controller.setObjectProperty(uid, Kind.DIAGRAM, ObjectProperties.PROPERTIES, properties);

                // no break on purpose, we decode non-root specific properties later
            case SuperBlockDiagram:
                final Kind kind;
                XcosCell parent;
                if (uid == 0l) {
                    parent = saxHandler.lookupForParentXcosCellElement();
                    uid = parent.getUID();
                    kind = parent.getKind();
                } else {
                    kind = Kind.DIAGRAM;
                    parent = new XcosCell(saxHandler.controller, uid, kind, null, null, "", "");
                }

                /*
                 * Decode the properties shared between a DIAGRAM and a BLOCK
                 */
                VectorOfInt colors = new VectorOfInt();
                saxHandler.controller.getObjectProperty(uid, kind, ObjectProperties.COLOR, colors);
                v = atts.getValue("background");
                if (v != null) {
                    colors.set(0, Integer.valueOf(v));
                }
                v = atts.getValue("gridEnabled");
                if (v != null) {
                    colors.set(1, Integer.valueOf(v));
                }
                saxHandler.controller.setObjectProperty(uid, kind, ObjectProperties.COLOR, colors);

                // TODO: implement a GUI to setup the title property (currently file name is used)
                v = atts.getValue("title");
                if (v != null) {
                    saxHandler.controller.setObjectProperty(uid, kind, ObjectProperties.TITLE, v);
                }

                /*
                 * Update some states
                 */
                saxHandler.allChildren.push(new HashMap<>());
                return parent;
            default:
                throw new IllegalArgumentException();
        }
    }

    @Override
    public void endElement(HandledElement found) {
        switch (found) {
            case Orientation:
                break;
            case XcosDiagram:
                resolve();
                saxHandler.allChildren.pop();
                XcosCellFactory.insertChildren(saxHandler.controller, saxHandler.root);

                if (XcosSAXHandler.LOG.isLoggable(Level.FINER)) {
                    XcosSAXHandler.LOG.exiting(CustomHandler.class.getName(), "endElement(\"XcosDiagram\")");
                }
                break;
            case SuperBlockDiagram:
                resolve();
                saxHandler.allChildren.pop();
                break;
            default:
                throw new IllegalArgumentException();
        }
    }

    private void resolve() {
        HashMap<String, Long> allLocalChildren = saxHandler.allChildren.peek();

        for (Entry<String, ArrayList<UnresolvedReference>> entry : saxHandler.unresolvedReferences.entrySet()) {
            Long uidObject = allLocalChildren.get(entry.getKey());
            if (uidObject != null) {
                long uid = uidObject.longValue();

                for (UnresolvedReference unresolvedReference : entry.getValue()) {
                    unresolvedReference.resolve(saxHandler.controller, uid, saxHandler.controller.getKind(uid));
                }
            }
        }
    }
}