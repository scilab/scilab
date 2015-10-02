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

import java.util.HashMap;
import java.util.Optional;
import java.util.stream.Stream;

import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.graph.ScicosParameters;
import org.scilab.modules.xcos.graph.model.ScicosObjectOwner;
import org.scilab.modules.xcos.graph.model.XcosCell;
import org.scilab.modules.xcos.graph.model.XcosCellFactory;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;
import org.xml.sax.Attributes;

class CustomHandler implements ScilabHandler {

    private final SAXHandler saxHandler;

    /**
     * Default constructor
     * @param saxHandler the shared sax handler
     */
    CustomHandler(SAXHandler saxHandler) {
        this.saxHandler = saxHandler;
    }

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
                // do not allocate this is already allocated as #root
                uid = saxHandler.root.getUID();

                /*
                 * Decode some graph properties
                 */
                v = atts.getValue("savedFile");
                if (v != null) {
                    saxHandler.controller.setObjectProperty(uid, Kind.DIAGRAM, ObjectProperties.PATH, v);
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
                if (uid == 0l) {
                    XcosCell parent = saxHandler.lookupForParentXcosCellElement();
                    uid = parent.getUID();
                    kind = parent.getKind();
                } else {
                    kind = Kind.DIAGRAM;
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

                v = atts.getValue("title");
                if (v != null) {
                    saxHandler.controller.setObjectProperty(uid, kind, ObjectProperties.TITLE, v);
                }

                /*
                 * Update some states
                 */
                saxHandler.allChildren.push(new HashMap<>());
                return new ScicosObjectOwner(uid, kind);
            default:
                throw new IllegalArgumentException();
        }
    }

    public void endElement(HandledElement found) {
        switch (found) {
            case Orientation:
                break;
            case XcosDiagram:
                XcosCellFactory.insertChildren(saxHandler.controller, saxHandler.root);
                break;
            case SuperBlockDiagram:
                saxHandler.allChildren.pop();
                break;
            default:
                throw new IllegalArgumentException();
        }

        // FIXME manage unresolved link source
        // FIXME manage unresolved link target

    }
}