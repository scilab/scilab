/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Clement DAVID
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

import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabIntegerTypeEnum;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;

/**
 * Define serialization for a {@link ScilabInteger} instance.
 */
public class ScilabIntegerCodec extends ScilabObjectCodec {

    private static final String BUNSIGNED = "bUnsigned"; /* With old IntegerType */
    private static final String VALUE = "value";
    private static final String PREC = "precision"; /* With old IntegerType */
    private static final String PRECISION = "intPrecision"; /*
                                                             * with
                                                             * ScilabIntegerTypeEnum
                                                             */

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
    public ScilabIntegerCodec(Object template, String[] exclude, String[] idrefs, Map<String, String> mapping) {
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

        ScilabInteger scilabInteger = (ScilabInteger) obj;

        if (binary) {
            int pos = binaryObjects.size();
            binaryObjects.add(scilabInteger);
            mxCodec.setAttribute(node, BINARY, "true");
            mxCodec.setAttribute(node, POSITION, pos);

            return node;
        }

        mxCodec.setAttribute(node, WIDTH, scilabInteger.getWidth());
        mxCodec.setAttribute(node, HEIGHT, scilabInteger.getHeight());
        mxCodec.setAttribute(node, PRECISION, scilabInteger.getPrec().name());

        for (int i = 0; i < scilabInteger.getHeight(); ++i) {
            for (int j = 0; j < scilabInteger.getWidth(); ++j) {
                Node data = enc.getDocument().createElement(DATA);
                mxCodec.setAttribute(data, LINE, i);
                mxCodec.setAttribute(data, COLUMN, j);
                mxCodec.setAttribute(data, VALUE, scilabInteger.getData()[i][j]);
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
        ScilabInteger obj = null;

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

            obj = (ScilabInteger) cloneTemplate(node);

            final int height = getHeight(attrs);
            final int width = getWidth(attrs);

            if (height * width == 0) {
                return obj;
            }

            /* Default values */
            final Node precNode = attrs.getNamedItem(PRECISION);
            ScilabIntegerTypeEnum precision;
            if (precNode != null) {
                precision = ScilabIntegerTypeEnum.valueOf(precNode.getNodeValue());
            } else {
                precision = ScilabIntegerTypeEnum.sci_uint8;
            }

            /* Compatibility for pre-5.2.2 version */
            final Node prec = attrs.getNamedItem(PREC);
            if (prec != null) {
                /* Old version, we have to convert to the new one */
                final Node u = attrs.getNamedItem(BUNSIGNED);
                final boolean unsigned;
                /*
                 * the default boolean value is false, this value is not
                 * serialized by jgraphx this if we doesn't have attribute the
                 * value is "false".
                 */
                unsigned = u != null;

                precision = ScilabInteger.convertOldType(prec.getNodeValue(), unsigned);

            }

            switch (precision) {
                case sci_int8:
                case sci_uint8:
                    final byte[][] data8 = new byte[height][width];
                    fillData(node, data8);
                    obj.setData(data8, precision == ScilabIntegerTypeEnum.sci_uint8);
                    break;
                case sci_int16:
                case sci_uint16:
                    final short[][] data16 = new short[height][width];
                    fillData(node, data16);
                    obj.setData(data16, precision == ScilabIntegerTypeEnum.sci_uint16);
                    break;
                case sci_int32:
                case sci_uint32:
                    final int[][] data32 = new int[height][width];
                    fillData(node, data32);
                    obj.setData(data32, precision == ScilabIntegerTypeEnum.sci_uint32);
                    break;
                default:
                    final long[][] data64 = new long[height][width];
                    fillData(node, data64);
                    obj.setData(data64, precision == ScilabIntegerTypeEnum.sci_uint64);
                    break;
            }

        } catch (UnrecognizeFormatException e) {
            Logger.getLogger(ScilabIntegerCodec.class.getName()).severe(e.toString());
        } catch (NumberFormatException e) {
            Logger.getLogger(ScilabIntegerCodec.class.getName()).severe(e.toString());
        }
        return obj;
    }

    /**
     * Fill the data from the node.
     *
     * @param node
     *            the ScilabInteger node
     * @param data
     *            the allocated data
     * @throws UnrecognizeFormatException
     *             when we are unable to decode the node.
     */
    private void fillData(Node node, byte[][] data) throws UnrecognizeFormatException {
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

            data[line][column] = Byte.parseByte(v.getNodeValue());
        }
    }

    /**
     * Fill the data from the node.
     *
     * @param node
     *            the ScilabInteger node
     * @param data
     *            the allocated data
     * @throws UnrecognizeFormatException
     *             when we are unable to decode the node.
     */
    private void fillData(Node node, short[][] data) throws UnrecognizeFormatException {
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

            data[line][column] = Short.parseShort(v.getNodeValue());
        }
    }

    /**
     * Fill the data from the node.
     *
     * @param node
     *            the ScilabInteger node
     * @param data
     *            the allocated data
     * @throws UnrecognizeFormatException
     *             when we are unable to decode the node.
     */
    private void fillData(Node node, int[][] data) throws UnrecognizeFormatException {
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

            data[line][column] = Integer.parseInt(v.getNodeValue());
        }
    }

    /**
     * Fill the data from the node.
     *
     * @param node
     *            the ScilabInteger node
     * @param data
     *            the allocated data
     * @throws UnrecognizeFormatException
     *             when we are unable to decode the node.
     */
    private void fillData(Node node, long[][] data) throws UnrecognizeFormatException {
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

            data[line][column] = Long.parseLong(v.getNodeValue());
        }
    }
}
