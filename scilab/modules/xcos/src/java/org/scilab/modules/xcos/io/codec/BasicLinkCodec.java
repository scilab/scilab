/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.io.codec;

import static org.scilab.modules.xcos.io.codec.XcosCodec.LOG;

import java.util.Map;

import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.link.CommandControlLink;
import org.scilab.modules.xcos.link.ExplicitLink;
import org.scilab.modules.xcos.link.ImplicitLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;

public class BasicLinkCodec extends XcosObjectCodec {

    public static void register() {
        JavaController controller = new JavaController();

        BasicLinkCodec explicitlinkCodec = new BasicLinkCodec(new ExplicitLink(controller, controller.createObject(Kind.LINK), Kind.LINK, null, null, null, null), null, REFS, null);
        mxCodecRegistry.register(explicitlinkCodec);
        BasicLinkCodec implicitlinkCodec = new BasicLinkCodec(new ImplicitLink(controller, controller.createObject(Kind.LINK), Kind.LINK, null, null, null, null), null, REFS, null);
        mxCodecRegistry.register(implicitlinkCodec);
        BasicLinkCodec commandControllinkCodec = new BasicLinkCodec(new CommandControlLink(controller, controller.createObject(Kind.LINK), Kind.LINK, null, null, null, null), null, REFS, null);
        mxCodecRegistry.register(commandControllinkCodec);
    }

    public BasicLinkCodec(Object template, String[] exclude, String[] idrefs,
                          Map<String, String> mapping) {
        super(template, exclude, idrefs, mapping);
    }

    @Override
    public Object beforeEncode(mxCodec enc, Object obj, Node node) {

        /*
         * Log some information
         */
        final BasicLink l = (BasicLink) obj;

        if (!(l.getSource() instanceof BasicPort)) {
            trace(enc, node, "Invalid source");
            LOG.warning("The saved file might be incomplete, '" + l + "' is not connected");
        } else {
            final BasicPort p = (BasicPort) l.getSource();

            if (!(p.getParent() instanceof BasicBlock)) {
                trace(enc, node, "Invalid source parent");
                LOG.warning("The saved file might be incomplete, '" + l + "' is wrongly connected");
            }
        }
        if (!(l.getTarget() instanceof BasicPort)) {
            trace(enc, node, "Invalid target");
            LOG.warning("The saved file might be incomplete, '" + l + "' is not connected");
        } else {
            final BasicPort p = (BasicPort) l.getTarget();

            if (!(p.getParent() instanceof BasicBlock)) {
                trace(enc, node, "Invalid target parent");
                LOG.warning("The saved file might be incomplete, '" + l + "' is wrongly connected");
            }
        }

        return super.beforeEncode(enc, obj, node);
    }
}
