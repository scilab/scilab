/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
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

import java.util.logging.Logger;

import org.scilab.modules.xcos.port.Orientation;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;
import com.mxgraph.io.mxObjectCodec;

/**
 * Encode and decode an Orientation.
 *
 * This class is need as jgraphx doesn't provide serialization for enums.
 */
public class OrientationCodec extends mxObjectCodec {

    /**
     * The default attribute name
     */
    private static final String ATTRIBUTE_NAME = "value";

    /**
     * Constructor with template
     *
     * @param template
     *            the default instance
     */
    public OrientationCodec(Object template) {
        super(template);
    }

    /**
     * Register this codec on the {@link mxCodecRegistry}.
     */
    public static void register() {
        mxCodecRegistry.register(new OrientationCodec(Orientation.NORTH));
    }

    /**
     * Encode the orientation
     *
     * @param enc
     *            the encoder instance
     * @param obj
     *            the object instance
     * @return the Node
     * @see com.mxgraph.io.mxObjectCodec#encode(com.mxgraph.io.mxCodec,
     *      java.lang.Object)
     */
    @Override
    public Node encode(mxCodec enc, Object obj) {
        String name = mxCodecRegistry.getName(obj);
        Node node = enc.getDocument().createElement(name);

        Orientation instance = (Orientation) obj;

        mxCodec.setAttribute(node, ATTRIBUTE_NAME, instance.name());

        return node;
    }

    /**
     * Decode the orientation
     *
     * @param dec
     *            the decoder
     * @param node
     *            the node to decode
     * @param into
     *            not used
     * @return an object instance
     * @see com.mxgraph.io.mxObjectCodec#decode(com.mxgraph.io.mxCodec,
     *      org.w3c.dom.Node, Object)
     */
    @Override
    public Object decode(mxCodec dec, Node node, Object into) {
        final NamedNodeMap attrs = node.getAttributes();

        final Node value = attrs.getNamedItem(ATTRIBUTE_NAME);

        /*
         * Return the first value when unable to get it.
         */
        if (value == null) {
            Logger.getLogger(OrientationCodec.class.getName()).severe("Corrupted diagram, port may be wrongly oriented");
            return Orientation.values()[0];
        }

        final String enumName = value.getNodeValue();
        return Enum.valueOf(Orientation.class, enumName);
    }

}
