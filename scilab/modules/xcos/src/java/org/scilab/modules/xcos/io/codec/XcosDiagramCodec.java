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

import java.lang.reflect.Field;
import java.util.Map;

import org.scilab.modules.graph.io.ScilabGraphCodec;
import org.scilab.modules.xcos.graph.ScicosParameters;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;

/**
 * Codec for an {@link org.scilab.modules.xcos.graph.XcosDiagram} instance.
 */
public class XcosDiagramCodec extends ScilabGraphCodec {
	private static final String SCICOS_PARAMETERS = "scicosParameters";
	private static final String AS_ATTRIBUTE = "as";
	
	
	// The non saved fields are hardcoded and can have the same name.
	// CSOFF: MultipleStringLiterals
	private static final String[] DIAGRAM_IGNORED_FIELDS = {"stylesheet",
			"parentTab", "viewPort", "viewPortMenu", "view", "selectionModel",
			"savedFile", "multiplicities", "opened", "modified", "undoManager" };
	private static final String[] SUPERBLOCKDIAGRAM_IGNORED_FIELDS = {
			"stylesheet", "parentTab", "viewPort", "viewPortMenu", "view",
			"selectionModel", "multiplicities", "opened", "modified",
			"undoManager", "savedFile", "container",
			"integratorAbsoluteTolerance", "integratorRelativeTolerance",
			"maxIntegrationTimeInterval", "toleranceOnTime" };
	// CSON: MultipleStringLiterals

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
	 * Register this codec into the {@link mxCodecRegistry}.
	 */
	public static void register() {
		ScilabGraphCodec diagramCodec = new XcosDiagramCodec(new XcosDiagram(), DIAGRAM_IGNORED_FIELDS, null, null);
		mxCodecRegistry.register(diagramCodec);
		ScilabGraphCodec superBlockDiagramCodec = new XcosDiagramCodec(new SuperBlockDiagram(), SUPERBLOCKDIAGRAM_IGNORED_FIELDS, null, null);
		mxCodecRegistry.register(superBlockDiagramCodec);
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
			 * Remove the "as" attribute
			 */
			NamedNodeMap childAttributes = params.getAttributes();
			childAttributes.removeNamedItem(AS_ATTRIBUTE);

			/*
			 * Move each attribute from child to parent
			 */
			NamedNodeMap parentAttributes = node.getAttributes();
			for (int length = childAttributes.getLength() - 1; length >= 0; length--) {
				Node element = childAttributes.item(length);

				childAttributes.removeNamedItem(element.getNodeName());
				parentAttributes.setNamedItem(element);
			}

			/*
			 * Move each childNode from child to parent
			 */
			NodeList children = params.getChildNodes();
			for (int length = children.getLength() - 1; length >= 0; length--) {
				Node element = children.item(length);

				params.removeChild(element);
				node.appendChild(element);
			}
			
			/*
			 * Remove the ScicosParameter instance
			 */
			node.removeChild(params);
		}
	}
	
	/**
	 * Load the ScicosParameters fields from the current object 
	 * 
	 * @param obj the {@link XcosDiagram} instance
	 * @param fieldname the {@link Current} field name
	 * @param value the current field value
	 * @see com.mxgraph.io.mxObjectCodec#setFieldValue(java.lang.Object, java.lang.String, java.lang.Object)
	 */
	@Override
	protected void setFieldValue(Object obj, String fieldname, Object value) {
		Field field;
		try {
			field = ScicosParameters.class.getDeclaredField(fieldname);
			ScicosParameters params = ((XcosDiagram) obj).getScicosParameters();
			super.setFieldValue(params, fieldname, value);
		} catch (SecurityException e) {
			field = null;
		} catch (NoSuchFieldException e) {
			field = null;
		}
		
		if (field == null) {
			super.setFieldValue(obj, fieldname, value);
		}
	}
	
	/**
	 * Apply compatibility pattern to the decoded object
	 * @param dec Codec that controls the decoding process.
	 * @param node XML node to decode the object from.
	 * @param obj Object decoded.
	 * @return The Object transformed
	 * @see org.scilab.modules.graph.io.ScilabGraphCodec#afterDecode(com.mxgraph.io.mxCodec, org.w3c.dom.Node, java.lang.Object)
	 */
	@Override
	public Object afterDecode(mxCodec dec, Node node, Object obj) {
		XcosDiagram diag = (XcosDiagram) obj;
		
		diag.setChildrenParentDiagram();
		
		// pre-5.3 diagram may be saved in a locked state
		// unlock it
		diag.setReadOnly(false);
		
		return super.afterDecode(dec, node, obj);
	}
}
