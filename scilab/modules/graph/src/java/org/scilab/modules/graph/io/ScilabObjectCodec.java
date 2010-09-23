/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graph.io;

import java.util.Map;

import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabString;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;
import com.mxgraph.io.mxObjectCodec;
import com.mxgraph.model.mxCell;

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
	
	/**
	 * Throw when we cannot load the XML.
	 */
	public class UnrecognizeFormatException extends Exception {
		/**
		 * Default constructor
		 */
		public UnrecognizeFormatException() {
			super();
		}
		
		/**
		 * Constructor from another exception
		 * @param cause the source exception.
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
	public ScilabObjectCodec(Object template, String[] exclude,
			String[] idrefs, Map<String, String> mapping) {
		super(template, exclude, idrefs, mapping);

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
		
		ScilabObjectCodec scilabListCodec = new ScilabListCodec(new ScilabList(), new String[]{"scilabClass"}, null, null);
		mxCodecRegistry.register(scilabListCodec);
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
	 * @see org.scilab.modules.xcos.io.XcosObjectCodec#afterDecode(com.mxgraph.io.mxCodec,
	 *      org.w3c.dom.Node, java.lang.Object)
	 */
	@Override
	public Object afterDecode(mxCodec dec, Node node, Object obj) {
		if (node.getNodeName().equals("mxCell")) {
			NamedNodeMap attrs = node.getAttributes();
			for (int i = 0; i < attrs.getLength(); i++) {
				Node attr = attrs.item(i);
				if (attr.getNodeName().compareToIgnoreCase("id") == 0) {
					((mxCell) obj).setId(attr.getNodeValue());
				}
			}
		}
		return obj;
	}
	
	/**
	 * get an integer value from a attributes.
	 * 
	 * @param dataAttrs the attributes
	 * @param attribute the key to search
	 * @return the value
	 * @throws UnrecognizeFormatException when the key is not valid.
	 */
	private int getIntegerAttribute(final NamedNodeMap dataAttrs,
			final String attribute) throws UnrecognizeFormatException {
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
	 * Get the height of the data attributes.
	 * 
	 * @param attrs
	 *            the data attributes
	 * @return the height
	 * @throws UnrecognizeFormatException
	 *             when the height hasn't been found.
	 */
	protected int getHeight(final NamedNodeMap attrs)
			throws UnrecognizeFormatException {
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
	protected int getWidth(final NamedNodeMap attrs)
			throws UnrecognizeFormatException {
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
	protected int getColumnIndex(final NamedNodeMap dataAttrs)
			throws UnrecognizeFormatException {
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
	protected int getLineIndex(final NamedNodeMap dataAttrs)
			throws UnrecognizeFormatException {
		return getIntegerAttribute(dataAttrs, LINE);
	}
}
