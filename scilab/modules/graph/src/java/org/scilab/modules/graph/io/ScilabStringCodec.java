/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID
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

import org.scilab.modules.types.ScilabString;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;

/**
 * Define serialization for a {@link ScilabString} instance.
 */
public class ScilabStringCodec extends ScilabObjectCodec {

    private static final String VALUE = "value";

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
    public ScilabStringCodec(Object template, String[] exclude, String[] idrefs, Map<String, String> mapping) {
        super(template, exclude, idrefs, mapping);
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

        ScilabString scilabString = (ScilabString) obj;

        if (binary) {
            int pos = binaryObjects.size();
            binaryObjects.add(scilabString);
            mxCodec.setAttribute(node, BINARY, "true");
            mxCodec.setAttribute(node, POSITION, pos);

            return node;
        }

        mxCodec.setAttribute(node, WIDTH, scilabString.getWidth());
        mxCodec.setAttribute(node, HEIGHT, scilabString.getHeight());

        for (int i = 0; i < scilabString.getHeight(); ++i) {
            for (int j = 0; j < scilabString.getWidth(); ++j) {
                Node data = enc.getDocument().createElement(DATA);
                mxCodec.setAttribute(data, LINE, i);
                mxCodec.setAttribute(data, COLUMN, j);
                mxCodec.setAttribute(data, VALUE, scilabString.getData()[i][j]);
                node.appendChild(data);
            }
        }
        return node;
    }

    /**
     * Parses the given node into the object or returns a new object
     * representing the given node.
     *
     * @param dec
     *            Codec that controls the encoding process.
     * @param node
     *            XML node to be decoded.
     * @param into
     *            Optional object to encode the node into.
     * @return Returns the resulting object that represents the given XML node
     *         or the object given to the method as the into parameter.
     */
    @Override
    public Object decode(mxCodec dec, Node node, Object into) {
        ScilabString obj = null;

        try {
            if (node.getNodeType() != Node.ELEMENT_NODE) {
                throw new UnrecognizeFormatException();
            }

            // attrs = {"as", "height", "width", "binary", "position"}
            final NamedNodeMap attrs = node.getAttributes();
            if (attrs == null) {
                throw new UnrecognizeFormatException();
            }

            if (getBooleanAttribute(attrs, BINARY)) {
                return binaryObjects.get(getIntegerAttribute(attrs, POSITION));
            }

            obj = (ScilabString) cloneTemplate(node);

            final int height = getHeight(attrs);
            final int width = getWidth(attrs);

            if (height * width == 0) {
                return obj;
            }

            final String[][] data = new String[height][width];
            fillData(node, data);

            obj.setData(data);

        } catch (UnrecognizeFormatException e) {
            Logger.getLogger(ScilabStringCodec.class.getName()).severe(e.toString());
        } catch (NumberFormatException e) {
            Logger.getLogger(ScilabStringCodec.class.getName()).severe(e.toString());
        }

        return obj;
    }

    /**
     * Fill the data from the node.
     *
     * @param node
     *            the ScilabString node
     * @param data
     *            the allocated data
     * @throws UnrecognizeFormatException
     *             when we are unable to decode the node.
     */
    private void fillData(Node node, final String[][] data) throws UnrecognizeFormatException {
        for (Node n = node.getFirstChild(); n != null; n = n.getNextSibling()) {
            if (n.getNodeType() != Node.ELEMENT_NODE) {
                continue;
            }

            final NamedNodeMap dataAttrs = n.getAttributes();
            if (dataAttrs == null) {
                throw new UnrecognizeFormatException();
            }

            final int column = getColumnIndex(dataAttrs);
            final int line = getLineIndex(dataAttrs);

            final Node v = dataAttrs.getNamedItem(VALUE);
            if (v == null) {
                throw new UnrecognizeFormatException();
            }
            data[line][column] = v.getNodeValue();
        }
    }
}
