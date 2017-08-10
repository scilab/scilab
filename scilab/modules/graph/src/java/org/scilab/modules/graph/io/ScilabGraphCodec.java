/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2009 - DIGITEO - Allan SIMON
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

package org.scilab.modules.graph.io;

import java.awt.Color;
import java.util.Map;

import org.scilab.modules.graph.ScilabGraph;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxObjectCodec;

/**
 * Default codec for the graph component
 */
public class ScilabGraphCodec extends mxObjectCodec {
    private static final String BACKGROUND = "background";

    /**
     * Default constructor
     *
     * @param template
     *            the object template
     */
    public ScilabGraphCodec(Object template) {
        super(template);
    }

    /**
     * The constructor used on for configuration
     *
     * @param template
     *            Prototypical instance of the object to be encoded/decoded.
     * @param exclude
     *            Optional array of fieldnames to be ignored.
     * @param idrefs
     *            Optional array of fieldnames to be converted to/from
     *            references.
     * @param mapping
     *            Optional mapping from field- to attributenames.
     */
    public ScilabGraphCodec(Object template, String[] exclude, String[] idrefs, Map<String, String> mapping) {
        super(template, exclude, idrefs, mapping);
    }

    /**
     * Trace any msg to the xml document.
     *
     * @param enc
     *            the current encoder
     * @param node
     *            the current node
     * @param msg
     *            the message
     * @param format
     *            the format
     */
    protected void trace(mxCodec enc, Node node, String msg, Object... format) {
        node.appendChild(enc.getDocument().createComment(String.format(msg, format)));
    }

    /**
     * Things to do before encoding
     *
     * @param enc
     *            Codec that controls the encoding process.
     * @param obj
     *            Object to be encoded.
     * @param node
     *            XML node to encode the object into.
     * @return Returns the object to be encoded by the default encoding.
     * @see com.mxgraph.io.mxObjectCodec#beforeEncode(com.mxgraph.io.mxCodec,
     *      java.lang.Object, org.w3c.dom.Node)
     */
    @Override
    public Object beforeEncode(mxCodec enc, Object obj, Node node) {
        final ScilabGraph graph = (ScilabGraph) obj;
        if (graph.getAsComponent() != null) {
            ((Element) node).setAttribute(BACKGROUND, String.valueOf(graph.getAsComponent().getBackground().getRGB()));
        }
        return super.beforeEncode(enc, obj, node);
    }

    /**
     * Apply compatibility pattern to the decoded object
     *
     * @param dec
     *            Codec that controls the decoding process.
     * @param node
     *            XML node to decode the object from.
     * @param obj
     *            Object decoded.
     * @return The Object transformed
     * @see org.scilab.modules.xcos.io.codec.XcosObjectCodec#afterDecode(com.mxgraph.io.mxCodec,
     *      org.w3c.dom.Node, java.lang.Object)
     */
    @Override
    public Object afterDecode(mxCodec dec, Node node, Object obj) {
        final ScilabGraph graph = (ScilabGraph) obj;
        final Element elem = (Element) node;
        if (graph.getAsComponent() != null && !elem.getAttribute(BACKGROUND).isEmpty()) {
            graph.getAsComponent().setBackground((new Color(Integer.parseInt((((Element) node).getAttribute(BACKGROUND))))));
        }

        return super.afterDecode(dec, node, obj);
    }

}
