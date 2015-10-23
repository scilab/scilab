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

import java.util.ArrayList;

import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.graph.model.ScicosObjectOwner;
import org.scilab.modules.xcos.io.HandledElement;
import org.scilab.modules.xcos.io.sax.XcosSAXHandler.UnresolvedReference;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.link.commandcontrol.CommandControlLink;
import org.scilab.modules.xcos.link.explicit.ExplicitLink;
import org.scilab.modules.xcos.link.implicit.ImplicitLink;
import org.xml.sax.Attributes;
import org.xml.sax.SAXParseException;

class LinkHandler implements ScilabHandler {

    private final XcosSAXHandler saxHandler;

    /**
     * Default constructor
     *
     * @param saxHandler
     *            the shared sax handler
     */
    LinkHandler(XcosSAXHandler saxHandler) {
        this.saxHandler = saxHandler;
    }

    @Override
    public BasicLink startElement(HandledElement found, Attributes atts) {
        String v;
        BasicLink link;
        int linkKind;
        final long uid = saxHandler.controller.createObject(Kind.LINK);

        switch (found) {
            case CommandControlLink:
                link = new CommandControlLink(uid);
                linkKind = -1;
                break;
            case ExplicitLink:
                link = new ExplicitLink(uid);
                linkKind = 1;
                break;
            case ImplicitLink:
                link = new ImplicitLink(uid);
                linkKind = 2;
                break;
            default:
                throw new IllegalArgumentException();
        }

        /*
         * Set the attributes
         */
        v = atts.getValue("id");
        if (v != null) {
            link.setId(v);
            saxHandler.allChildren.peek().put(v, uid);
        }

        saxHandler.controller.setObjectProperty(uid, Kind.LINK, ObjectProperties.KIND, linkKind);

        v = atts.getValue("source");
        if (v != null) {
            Long src = saxHandler.allChildren.peek().get(v);
            if (src != null) {
                // if the attribute is present then the connected port is already
                // decoded and present in the map
                saxHandler.controller.setObjectProperty(uid, Kind.LINK, ObjectProperties.SOURCE_PORT, src.longValue());
                saxHandler.controller.setObjectProperty(src.longValue(), Kind.PORT, ObjectProperties.CONNECTED_SIGNALS, uid);
            } else {
                // if not present then it will be resolved later
                ArrayList<UnresolvedReference> refList = saxHandler.unresolvedReferences.get(v);
                if (refList == null) {
                    refList = new ArrayList<>();
                    saxHandler.unresolvedReferences.put(v, refList);
                }
                refList.add(new UnresolvedReference(new ScicosObjectOwner(uid, Kind.LINK), ObjectProperties.SOURCE_PORT, ObjectProperties.CONNECTED_SIGNALS, 0));
            }
        }

        v = atts.getValue("target");
        if (v != null) {
            Long dst = saxHandler.allChildren.peek().get(v);
            if (dst != null) {
                // if the attribute is present then the connected port is already
                // decoded and present in the map
                saxHandler.controller.setObjectProperty(uid, Kind.LINK, ObjectProperties.DESTINATION_PORT, dst.longValue());
                saxHandler.controller.setObjectProperty(dst.longValue(), Kind.PORT, ObjectProperties.CONNECTED_SIGNALS, uid);
            } else {
                // if not present then it will be resolved later
                ArrayList<UnresolvedReference> refList = saxHandler.unresolvedReferences.get(v);
                if (refList == null) {
                    refList = new ArrayList<>();
                    saxHandler.unresolvedReferences.put(v, refList);
                }
                refList.add(new UnresolvedReference(new ScicosObjectOwner(uid, Kind.LINK), ObjectProperties.DESTINATION_PORT, ObjectProperties.CONNECTED_SIGNALS, 0));
            }
        }

        v = atts.getValue("style");
        if (v != null) {
            saxHandler.controller.setObjectProperty(uid, Kind.LINK, ObjectProperties.STYLE, v);
        }

        v = atts.getValue("value");
        if (v != null) {
            saxHandler.controller.setObjectProperty(uid, Kind.LINK, ObjectProperties.LABEL, v);
        }

        saxHandler.insertChild(link);
        return link;
    }

    @Override
    public void endElement(HandledElement found) {
    }
}