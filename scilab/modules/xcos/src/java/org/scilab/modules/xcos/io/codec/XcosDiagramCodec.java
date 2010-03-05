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

package org.scilab.modules.xcos.io.codec;

import java.util.Map;

import org.scilab.modules.graph.io.ScilabGraphCodec;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;

/**
 * Codec for an {@link org.scilab.modules.xcos.graph.XcosDiagram} instance.
 */
public class XcosDiagramCodec extends ScilabGraphCodec {
	private static final String SCICOS_PARAMETERS = "scicosParameters";

	/**
	 * Default constructor
	 * 
	 * @param template
	 *            the instance template
	 */
	public XcosDiagramCodec(Object template) {
		super(template);
	}

	/**
	 * The constructor used for configuration
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
	public XcosDiagramCodec(Object template, String[] exclude, String[] idrefs,
			Map<String, String> mapping) {
		super(template, exclude, idrefs, mapping);
	}

	/**
	 * Encode the fieldname value.
	 * 
	 * This method encode the 'scicosParameters' variable to the parent node.
	 * 
	 * @param enc
	 *            Codec that controls the encoding process.
	 * @param obj
	 *            Object whose field is going to be encoded.
	 * @param fieldname
	 *            Name if the field to be encoded.
	 * @param value
	 *            Value of the property to be encoded.
	 * @param node
	 *            XML node that contains the encoded object.
	 * @see com.mxgraph.io.mxObjectCodec#encodeValue(com.mxgraph.io.mxCodec,
	 *      java.lang.Object, java.lang.String, java.lang.Object,
	 *      org.w3c.dom.Node)
	 */
	@Override
	protected void encodeValue(mxCodec enc, Object obj, String fieldname,
			Object value, Node node) {
		super.encodeValue(enc, obj, fieldname, value, node);

		/*
		 * Put child parameters on the parent when child is 'scicosParameters'.
		 */
		if (fieldname.equals(SCICOS_PARAMETERS)) {
			Node params = node.getLastChild();

			/*
			 * Move each attribute from child to parent.
			 */
			NamedNodeMap childAttributes = params.getAttributes();
			NamedNodeMap parentAttributes = node.getAttributes();
			for (int length = childAttributes.getLength() - 1; length > 0; length--) {
				Node element = childAttributes.item(length);

				childAttributes.removeNamedItem(element.getNodeName());
				parentAttributes.setNamedItem(element);
			}

			/*
			 * Remove the ScicosParameter instance if empty
			 */
			if (params.getChildNodes().getLength() == 0) {
				node.removeChild(params);
			}
		}
	}
}
