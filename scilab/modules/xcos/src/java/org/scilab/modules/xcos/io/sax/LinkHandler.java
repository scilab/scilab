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

import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.graph.model.ScicosObjectOwner;
import org.scilab.modules.xcos.io.HandledElement;
import org.scilab.modules.xcos.io.sax.XcosSAXHandler.UnresolvedReference;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.link.CommandControlLink;
import org.scilab.modules.xcos.link.ExplicitLink;
import org.scilab.modules.xcos.link.ImplicitLink;
import org.xml.sax.Attributes;

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

        String strUID = atts.getValue("id");
        if (strUID != null) {
            saxHandler.allChildren.peek().put(strUID, uid);
        }
        String style = atts.getValue("style");
        String value = atts.getValue("value");

        switch (found) {
            case CommandControlLink:
                link = new CommandControlLink(saxHandler.controller, uid, Kind.LINK, value, null, style, strUID);
                linkKind = -1;
                break;
            case ExplicitLink:
                link = new ExplicitLink(saxHandler.controller, uid, Kind.LINK, value, null, style, strUID);
                linkKind = 1;
                break;
            case ImplicitLink:
                link = new ImplicitLink(saxHandler.controller, uid, Kind.LINK, value, null, style, strUID);
                linkKind = 2;
                break;
            default:
                throw new IllegalArgumentException();
        }

        /*
         * Set the attributes
         */

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

        saxHandler.insertChild(link);
        return link;
    }

    @Override
    public void endElement(HandledElement found) {
    }
}