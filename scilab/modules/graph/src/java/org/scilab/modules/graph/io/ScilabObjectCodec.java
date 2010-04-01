/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
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

import org.scilab.modules.hdf5.scilabTypes.ScilabBoolean;
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabInteger;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
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
}
