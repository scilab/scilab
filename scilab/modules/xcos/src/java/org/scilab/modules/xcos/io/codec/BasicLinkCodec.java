/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.io.codec;

import java.util.Map;

import org.scilab.modules.xcos.link.commandcontrol.CommandControlLink;
import org.scilab.modules.xcos.link.explicit.ExplicitLink;
import org.scilab.modules.xcos.link.implicit.ImplicitLink;

import com.mxgraph.io.mxCellCodec;
import com.mxgraph.io.mxCodecRegistry;

public class BasicLinkCodec extends mxCellCodec {

    public static void register() {
        BasicLinkCodec explicitlinkCodec = new BasicLinkCodec(
                new ExplicitLink(), null, null, null);
        mxCodecRegistry.register(explicitlinkCodec);
        BasicLinkCodec implicitlinkCodec = new BasicLinkCodec(
                new ImplicitLink(), null, null, null);
        mxCodecRegistry.register(implicitlinkCodec);
        BasicLinkCodec commandControllinkCodec = new BasicLinkCodec(
                new CommandControlLink(), null, null, null);
        mxCodecRegistry.register(commandControllinkCodec);
    }

    /*
     * Inherit all constructors
     */

    public BasicLinkCodec() {
    }

    public BasicLinkCodec(Object template) {
        super(template);
    }

    public BasicLinkCodec(Object template, String[] exclude, String[] idrefs,
            Map<String, String> mapping) {
        super(template, exclude, idrefs, mapping);
    }
}
