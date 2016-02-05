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

import java.util.HashSet;
import java.util.Map;

import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabString;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodecRegistry;
import com.mxgraph.io.mxObjectCodec;

/**
 * Codec for any Scilab object
 */
public abstract class ScilabObjectCodec extends mxObjectCodec {
    /** Height of the data */
    protected static final String HEIGHT = "height";
    /** Width of the data */
    protected static final String WIDTH = "width";
    /** columns index of the data */
    protected static final String COLUMN = "column";
    /** line index of the data */
    protected static final String LINE = "line";
    /** Data field */
    protected static final String DATA = "data";

    protected static final String POSITION = "position";
    protected static final String BINARY = "binary";

    protected static boolean binary;
    protected static ScilabList binaryObjects;
    protected static final Object LOCK = new Object();;

    /**
     * Throw when we cannot load the XML.
     */
    @SuppressWarnings(value = { "serial" })
    public class UnrecognizeFormatException extends Exception {
        /**
         * Default constructor
         */
        public UnrecognizeFormatException() {
            super();
        }

        /**
         * Constructor from another exception
         *
         * @param cause
         *            the source exception.
         */
        public UnrecognizeFormatException(Exception cause) {
            super(cause);
        }
    }

    /**
     * The constructor used on the configuration
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
    public ScilabObjectCodec(Object template, String[] exclude, String[] idrefs, Map<String, String> mapping) {
        super(template, exclude, idrefs, mapping);

        final HashSet<String> newExclude = new HashSet<String>(this.exclude);
        newExclude.add("type");
        this.exclude = newExclude;

    }

    public static Object enableBinarySerialization(ScilabList list) {
        synchronized (LOCK) {
            binary = true;
            if (list == null) {
                binaryObjects = new ScilabList();
            } else {
                binaryObjects = list;
            }

            return LOCK;
        }
    }

    public static ScilabList disableBinarySerialization() {
        synchronized (LOCK) {
            binary = false;
            ScilabList ret = binaryObjects;
            binaryObjects = null;

            return ret;
        }
    }

    public static ScilabList getBinaryObjects() {
        return binaryObjects;
    }

    /**
     * Register all known codecs on the {@link mxCodecRegistry}.
     */
    public static void register() {
        ScilabObjectCodec scilabStringCodec = new ScilabStringCodec(new ScilabString(), null, null, null);
        mxCodecRegistry.register(scilabStringCodec);
        ScilabObjectCodec scilabBooleanCodec = new ScilabBooleanCodec(new ScilabBoolean(), null, null, null);
        mxCodecRegistry.register(scilabBooleanCodec);
        ScilabObjectCodec scilabDoubleCodec = new ScilabDoubleCodec(new ScilabDouble(), null, null, null);
        mxCodecRegistry.register(scilabDoubleCodec);
        ScilabObjectCodec scilabIntegerCodec = new ScilabIntegerCodec(new ScilabInteger(), null, null, null);
        mxCodecRegistry.register(scilabIntegerCodec);

        ScilabObjectCodec scilabListCodec = new ScilabListCodec(new ScilabList(), new String[] { "scilabClass" }, null, null);
        mxCodecRegistry.register(scilabListCodec);
    }

    /**
     * get an integer value from a attributes.
     *
     * @param dataAttrs
     *            the attributes
     * @param attribute
     *            the key to search
     * @return the value
     * @throws UnrecognizeFormatException
     *             when the key is not valid.
     */
    protected int getIntegerAttribute(final NamedNodeMap dataAttrs, final String attribute) throws UnrecognizeFormatException {
        final Node node = dataAttrs.getNamedItem(attribute);
        if (node == null) {
            throw new UnrecognizeFormatException();
        }

        final int value;
        try {
            value = Integer.parseInt(node.getNodeValue());
        } catch (NumberFormatException e) {
            throw new UnrecognizeFormatException(e);
        }
        return value;
    }

    /**
     * get an integer value from a attributes.
     *
     * @param dataAttrs
     *            the attributes
     * @param attribute
     *            the key to search
     * @return the value
     * @throws UnrecognizeFormatException
     *             when the key is not valid.
     */
    protected boolean getBooleanAttribute(final NamedNodeMap dataAttrs, final String attribute) {
        final Node node = dataAttrs.getNamedItem(attribute);
        if (node == null) {
            return false;
        }

        return Boolean.parseBoolean(node.getNodeValue());
    }

    /**
     * Get the height of the data attributes.
     *
     * @param attrs
     *            the data attributes
     * @return the height
     * @throws UnrecognizeFormatException
     *             when the height hasn't been found.
     */
    protected int getHeight(final NamedNodeMap attrs) throws UnrecognizeFormatException {
        return getIntegerAttribute(attrs, HEIGHT);
    }

    /**
     * Get the width of the data attributes.
     *
     * @param attrs
     *            the data attributes
     * @return the width
     * @throws UnrecognizeFormatException
     *             when the width hasn't been found.
     */
    protected int getWidth(final NamedNodeMap attrs) throws UnrecognizeFormatException {
        return getIntegerAttribute(attrs, WIDTH);
    }

    /**
     * Get the column index of the data attributes.
     *
     * @param dataAttrs
     *            the data attributes
     * @return the column index
     * @throws UnrecognizeFormatException
     *             when the column index hasn't been found.
     */
    protected int getColumnIndex(final NamedNodeMap dataAttrs) throws UnrecognizeFormatException {
        return getIntegerAttribute(dataAttrs, COLUMN);
    }

    /**
     * Get the line index of the data attributes.
     *
     * @param dataAttrs
     *            the data attributes
     * @return the column index
     * @throws UnrecognizeFormatException
     *             when the column index hasn't been found.
     */
    protected int getLineIndex(final NamedNodeMap dataAttrs) throws UnrecognizeFormatException {
        return getIntegerAttribute(dataAttrs, LINE);
    }
}
