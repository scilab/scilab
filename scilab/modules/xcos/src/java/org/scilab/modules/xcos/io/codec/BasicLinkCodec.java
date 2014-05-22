/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.io.codec;

import java.util.Map;

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.link.commandcontrol.CommandControlLink;
import org.scilab.modules.xcos.link.explicit.ExplicitLink;
import org.scilab.modules.xcos.link.implicit.ImplicitLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;

import static org.scilab.modules.xcos.io.codec.XcosCodec.LOG;

public class BasicLinkCodec extends XcosObjectCodec {

    public static void register() {
        BasicLinkCodec explicitlinkCodec = new BasicLinkCodec(
            new ExplicitLink(), null, REFS, null);
        mxCodecRegistry.register(explicitlinkCodec);
        BasicLinkCodec implicitlinkCodec = new BasicLinkCodec(
            new ImplicitLink(), null, REFS, null);
        mxCodecRegistry.register(implicitlinkCodec);
        BasicLinkCodec commandControllinkCodec = new BasicLinkCodec(
            new CommandControlLink(), null, REFS, null);
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
