/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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

import java.util.logging.Logger;

import org.scilab.modules.graph.io.ScilabObjectCodec;
import org.scilab.modules.types.ScilabList;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;
import com.mxgraph.io.mxObjectCodec;

/**
 * Root codec for Xcos diagram instance.
 *
 * This class register all packages used by Xcos for
 * serialization/deserialization.
 */
public class XcosCodec extends mxCodec {
    /**
     * Register packages for encoding/decoding diagrams
     */
    static {
        try {

            // Add all xcos packages
            mxCodecRegistry.addPackage("org.scilab.modules.graph");
            mxCodecRegistry.addPackage("org.scilab.modules.xcos");
            mxCodecRegistry.addPackage("org.scilab.modules.xcos.graph");
            mxCodecRegistry.addPackage("org.scilab.modules.xcos.block");
            mxCodecRegistry
            .addPackage("org.scilab.modules.xcos.block.custom");
            mxCodecRegistry.addPackage("org.scilab.modules.xcos.link");
            mxCodecRegistry
            .addPackage("org.scilab.modules.xcos.link.commandcontrol");
            mxCodecRegistry.addPackage("org.scilab.modules.xcos.link.explicit");
            mxCodecRegistry.addPackage("org.scilab.modules.xcos.link.implicit");
            mxCodecRegistry.addPackage("org.scilab.modules.xcos.port");
            mxCodecRegistry.addPackage("org.scilab.modules.xcos.port.command");
            mxCodecRegistry.addPackage("org.scilab.modules.xcos.port.control");
            mxCodecRegistry.addPackage("org.scilab.modules.xcos.port.input");
            mxCodecRegistry.addPackage("org.scilab.modules.xcos.port.output");
            // Add 'types' package to have all scilab types known
            mxCodecRegistry.addPackage("org.scilab.modules.types");

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * Install codecs for serializable instance
     */
    static {
        try {

            // Types
            ScilabObjectCodec.register();

            // Diagram
            XcosDiagramCodec.register();

            // Blocks
            BasicBlockCodec.register();

            // Link
            BasicLinkCodec.register();

            // Ports
            BasicPortCodec.register();

            // Orientation
            OrientationCodec.register();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static final Logger LOG = Logger.getLogger(XcosCodec.class.getSimpleName());

    /**
     * Default constructor
     */
    public XcosCodec() {
        super();
    }

    public XcosCodec(Document document) {
        super(document);
    }


    /**
     * Set the setIdAttribute for all node in the {@link Document}.
     */
    public void setElementIdAttributes () {
        setElementIdAttributes(getDocument());
    }

    /**
     * Set the setIdAttribute for all node in the {@link Node}.
     *
     * After this call the user can easily getElementById() on the DOM tree.
     *
     * @param n the root node to start with
     */
    public static void setElementIdAttributes (final Node root) {
        for (Node next = root.getNextSibling(); next != null; next = next.getNextSibling()) {
            if (next.getNodeType() == Node.ELEMENT_NODE) {
                final Element elem = (Element) next;

                if (elem.hasAttribute("id")) {
                    elem.setIdAttribute("id", true);
                }
            }
        }

        NodeList children = root.getChildNodes();
        for (int i = 0; i < children.getLength(); i++) {
            final Node child = children.item(i);
            if (child.getNodeType() == Node.ELEMENT_NODE) {
                setElementIdAttributes(child);
            }
        }
    }

    /**
     * As each node has an xml:id, use getElementById() instead of an xpath expression (through JGraphX).
     *
     * {@inheritDoc}
     */
    @Override
    public Object lookup(String id) {
        final Element element = getDocument().getElementById(id);
        if (element == null) {
            LOG.warning("Your file might be corrupted, '" + id + "' is not indexed in the document ID list");
            return null;
        } else {
            final mxObjectCodec decoder = mxCodecRegistry.getCodec(element.getNodeName());
            return decoder.decode(this, element);
        }
    }

    public static Object enableBinarySerialization(ScilabList dictionary) {
        return ScilabObjectCodec.enableBinarySerialization(dictionary);
    }

    public static ScilabList disableBinarySerialization() {
        return ScilabObjectCodec.disableBinarySerialization();
    }
}
