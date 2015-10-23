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
import org.scilab.modules.xcos.graph.model.XcosCell;
import org.xml.sax.Attributes;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.util.mxPoint;

class JGraphXHandler implements ScilabHandler {

    private final SAXHandler saxHandler;

    JGraphXHandler(SAXHandler saxHandler) {
        this.saxHandler = saxHandler;
    }

    @Override
    public Object startElement(HandledElement found, Attributes atts) {
        String v;

        switch (found) {
            case mxCell: {
                v = atts.getValue("parent");
                if (v != null) {
                    long parentUID = saxHandler.allChildren.peek().getOrDefault(v, 0l);
                    if (parentUID != 0) {
                        v = atts.getValue("value");
                        if (v != null) {
                            Kind kind = saxHandler.controller.getKind(parentUID);
                            saxHandler.controller.setObjectProperty(parentUID, kind, ObjectProperties.LABEL, v);
                        }
                    }
                }

                return null;
            }
            case mxGeometry: {
                mxGeometry g = new mxGeometry();

                v = atts.getValue("height");
                if (v != null) {
                    g.setHeight(Double.valueOf(v));
                }
                v = atts.getValue("width");
                if (v != null) {
                    g.setWidth(Double.valueOf(v));
                }
                v = atts.getValue("x");
                if (v != null) {
                    g.setX(Double.valueOf(v));
                }
                v = atts.getValue("y");
                if (v != null) {
                    g.setY(Double.valueOf(v));
                }

                return g;
            }
            case mxPoint: {
                // defensive programming
                if (!(saxHandler.parents.peek() instanceof mxGeometry)) {
                    return null;
                }
                mxGeometry parent = (mxGeometry) saxHandler.parents.peek();

                mxPoint p = new mxPoint();

                v = atts.getValue("x");
                if (v != null) {
                    p.setX(Double.valueOf(v));
                }
                v = atts.getValue("y");
                if (v != null) {
                    p.setY(Double.valueOf(v));
                }
                v = atts.getValue("as");
                if ("sourcePoint".equals(v)) {
                    parent.setSourcePoint(p);
                } else if ("targetPoint".equals(v)) {
                    parent.setTargetPoint(p);
                }
                return p;
            }
            default:
                throw new IllegalArgumentException();
        }
    }

    @Override
    public void endElement(HandledElement found) {
        switch (found) {
            case mxCell:
                break;
            case mxGeometry: {
                // defensive programming
                if (!(saxHandler.parents.peek() instanceof mxGeometry)) {
                    return;
                }
                mxGeometry g = (mxGeometry) saxHandler.parents.peek();
                if (!(saxHandler.parents.peek(1) instanceof XcosCell)) {
                    return;
                }
                XcosCell cell = (XcosCell) saxHandler.parents.peek(1);

                cell.setGeometry(g);
            }
            break;
            case mxPoint:
                break;
            default:
                throw new IllegalArgumentException();
        }
    }
}