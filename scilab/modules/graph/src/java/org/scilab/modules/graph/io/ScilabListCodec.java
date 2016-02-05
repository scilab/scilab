/*
 * Scilab (http://www.scilab.org/ ) - This file is part of Scilab
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

import java.util.Map;
import java.util.logging.Logger;

import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabType;

import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;

/**
 * Define serialization for a {@link ScilabList} instance.
 */
public class ScilabListCodec extends ScilabObjectCodec {

    private static final String SCILAB_CLASS = "scilabClass";

    /**
     * Default constructor
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
    public ScilabListCodec(Object template, String[] exclude, String[] idrefs, Map<String, String> mapping) {
        super(template, exclude, idrefs, mapping);

    }

    /**
     * Add the class name as an attribute.
     *
     * @param enc
     *            Codec that controls the encoding process.
     * @param obj
     *            Object to be encoded.
     * @param node
     *            XML node that represents the default encoding.
     * @return Returns the resulting node of the encoding.
     * @see com.mxgraph.io.mxObjectCodec#beforeEncode(com.mxgraph.io.mxCodec,
     *      java.lang.Object, org.w3c.dom.Node)
     */
    public Object beforeEncode(mxCodec enc, Object obj, Node node) {
        if (!binary) {
            String type = "";
            for (Class <? extends Object > klass = obj.getClass(); klass != Object.class; klass = klass.getSuperclass()) {
                if (klass == ScilabMList.class || klass == ScilabTList.class || klass == ScilabList.class || klass.isArray()) {
                    type = klass.getSimpleName();
                    break;
                }
            }

            assert !type.equals("");
            mxCodec.setAttribute(node, SCILAB_CLASS, type);
        }

        return obj;
    }

    /**
     * Instantiate defined class for the attribute
     *
     * @param node
     *            the node we are working on
     * @return the instance of the node.
     * @see com.mxgraph.io.mxObjectCodec#cloneTemplate(org.w3c.dom.Node)
     */
    public Object cloneTemplate(Node node) {
        Object obj = null;
        if (node.getAttributes().getNamedItem(SCILAB_CLASS) != null) {
            String scilabClass = node.getAttributes().getNamedItem(SCILAB_CLASS).getNodeValue();
            if (scilabClass.equalsIgnoreCase("ScilabMList")) {
                obj = new ScilabMList();
            } else if (scilabClass.equalsIgnoreCase("ScilabTList")) {
                obj = new ScilabTList();
            } else if (scilabClass.equalsIgnoreCase("ScilabList")) {
                obj = new ScilabList();
            } else {

                obj = super.cloneTemplate(node);
            }

        } else {
            obj = super.cloneTemplate(node);
        }

        return obj;
    }

    /**
     * Encodes the specified object and returns a node representing then given
     * object. Calls beforeEncode after creating the node and afterEncode with
     * the resulting node after processing.
     *
     * @param enc
     *            Codec that controls the encoding process.
     * @param obj
     *            Object to be encoded.
     * @return Returns the resulting XML node that represents the given object.
     */
    @Override
    public Node encode(mxCodec enc, Object obj) {
        String name = mxCodecRegistry.getName(obj);
        Node node = enc.getDocument().createElement(name);

        if (binary && (obj instanceof ScilabType)) {
            int pos = binaryObjects.size();
            binaryObjects.add((ScilabType) obj);
            mxCodec.setAttribute(node, BINARY, "true");
            mxCodec.setAttribute(node, POSITION, pos);

            return node;
        }

        return super.encode(enc, obj);
    }

    /**
     * Workaround for a jgraphx bug on deserialization with a possible abstract
     * array and default value.
     *
     * @param dec
     *            the current decoder instance
     * @param node
     *            the current node
     * @param into
     *            the object decode into (may be a wrongly typed instance)
     * @return a valid (right typed) instance
     * @see com.mxgraph.io.mxObjectCodec#decode(com.mxgraph.io.mxCodec,
     *      org.w3c.dom.Node, java.lang.Object)
     * @see http://www.jgraph.org/bugzilla/show_bug.cgi?id=55
     * @see http://bugzilla.scilab.org/show_bug.cgi?id=8141
     */
    @Override
    public Object decode(mxCodec dec, Node node, Object into) {
        try {
            final NamedNodeMap attrs = node.getAttributes();
            if (attrs != null && getBooleanAttribute(attrs, BINARY)) {
                return binaryObjects.get(getIntegerAttribute(attrs, POSITION));
            }
        } catch (UnrecognizeFormatException e) {
            Logger.getLogger(ScilabDoubleCodec.class.getName()).severe(e.toString());
        }

        // Workaround case selection :
        // - node is an "Array"
        // - into (the default template) is not.
        if (node.getNodeName().equals("Array") && into != null && !into.getClass().isArray()) {
            return super.decode(dec, node, null);
        } else {
            return super.decode(dec, node, into);
        }
    }
}
