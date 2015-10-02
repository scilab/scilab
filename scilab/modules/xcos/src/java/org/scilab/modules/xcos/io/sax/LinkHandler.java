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
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.link.commandcontrol.CommandControlLink;
import org.scilab.modules.xcos.link.explicit.ExplicitLink;
import org.scilab.modules.xcos.link.implicit.ImplicitLink;
import org.xml.sax.Attributes;

class LinkHandler implements ScilabHandler {

    private final SAXHandler saxHandler;

    /**
     * Default constructor
     * @param saxHandler the shared sax handler
     */
    LinkHandler(SAXHandler saxHandler) {
        this.saxHandler = saxHandler;
    }

    public BasicLink startElement(HandledElement found, Attributes atts) {
        String v;
        BasicLink link;
        final long uid = saxHandler.controller.createObject(Kind.LINK);

        switch (found) {
            case CommandControlLink:
                link = new CommandControlLink(uid);
                saxHandler.controller.setObjectProperty(uid, Kind.LINK, ObjectProperties.KIND, -1);
                break;
            case ExplicitLink:
                link = new ExplicitLink(uid);
                saxHandler.controller.setObjectProperty(uid, Kind.LINK, ObjectProperties.KIND, 1);
                break;
            case ImplicitLink:
                link = new ImplicitLink(uid);
                saxHandler.controller.setObjectProperty(uid, Kind.LINK, ObjectProperties.KIND, 2);
                break;
            default:
                throw new IllegalArgumentException();
        }

        /*
         * Set the attributes
         */
        v = atts.getValue("id");
        if (v != null) {
            saxHandler.allChildren.peek().put(v, uid);
            saxHandler.controller.setObjectProperty(uid, Kind.LINK, ObjectProperties.UID, v);
        }

        v = atts.getValue("source");
        if (v != null) {
            // if the attribute is present then the connected port is already
            // decoded and present in the map
            long src = saxHandler.allChildren.peek().get(v);
            saxHandler.controller.setObjectProperty(uid, Kind.LINK, ObjectProperties.SOURCE_PORT, src);
        }

        v = atts.getValue("target");
        if (v != null) {
            // if the attribute is present then the connected port is already
            // decoded and present in the map
            long dst = saxHandler.allChildren.peek().get(v);
            saxHandler.controller.setObjectProperty(uid, Kind.LINK, ObjectProperties.DESTINATION_PORT, dst);
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

    public void endElement(HandledElement found) {
    }
}