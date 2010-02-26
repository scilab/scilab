/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clément DAVID
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
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;

/**
 * Define serialization for a {@link ScilabBoolean} instance.
 */
public class ScilabBooleanCodec extends ScilabObjectCodec {

    
    private static final String VALUE = "value";
    private static final String COLUMN = "column";
    private static final String LINE = "line";
    private static final String DATA = "data";
    private static final String HEIGHT = "height";
    private static final String WIDTH = "width";

    /**
     * Default constructor
	 * @param template Prototypical instance of the object to be encoded/decoded.
	 * @param exclude Optional array of fieldnames to be ignored.
	 * @param idrefs Optional array of fieldnames to be converted to/from references.
	 * @param mapping Optional mapping from field- to attributenames.
     */
    public ScilabBooleanCodec(Object template, String[] exclude, String[] idrefs, Map<String, String> mapping)
    {
	super(template, exclude, idrefs, mapping);

    }

	/**
	 * Encodes the specified object and returns a node representing then given
	 * object. Calls beforeEncode after creating the node and afterEncode
	 * with the resulting node after processing.
	 * 
	 * @param enc Codec that controls the encoding process.
	 * @param obj Object to be encoded.
	 * @return Returns the resulting XML node that represents the given object. 
	 */
    @Override
    public Node encode(mxCodec enc, Object obj) {
    	String name = mxCodecRegistry.getName(obj);
	Node node = enc.getDocument().createElement(name);

	ScilabBoolean scilabBoolean = (ScilabBoolean) obj;
	mxCodec.setAttribute(node, WIDTH, scilabBoolean.getWidth());
	mxCodec.setAttribute(node, HEIGHT, scilabBoolean.getHeight());

	for (int i = 0; i < scilabBoolean.getHeight(); ++i) {
	    for (int j = 0; j < scilabBoolean.getWidth(); ++j) {
		Node data = enc.getDocument().createElement(DATA);
		mxCodec.setAttribute(data, LINE, i);
		mxCodec.setAttribute(data, COLUMN, j);
		mxCodec.setAttribute(data, VALUE, scilabBoolean.getData()[i][j]);
		node.appendChild(data);
	    }
	}
	return node;
    }
    
    /**
     * Parses the given node into the object or returns a new object
	 * representing the given node.
	 * 
	 * @param dec Codec that controls the encoding process.
	 * @param node XML node to be decoded.
	 * @param into Optional object to encode the node into.
	 * @return Returns the resulting object that represents the given XML node
	 * or the object given to the method as the into parameter.
	 */
    @Override
    public Object decode(mxCodec dec, Node node, Object into) {
	Object obj = null;
	try {

	    if (!(node instanceof Element)) { return null; }
	    obj = cloneTemplate(node);


	    // attrs = {"as", "height", "width"}
	    NamedNodeMap attrs = node.getAttributes();
	    int heightXMLPosition = -1;
	    int widthXMLPosition = -1;
	    for (int i = 0; i < attrs.getLength(); i++) {
		Node attr = attrs.item(i);
		if (attr.getNodeName().compareToIgnoreCase(WIDTH) == 0) { widthXMLPosition = i; }
		if (attr.getNodeName().compareToIgnoreCase(HEIGHT) == 0) { heightXMLPosition = i; }
	    }
	    if (heightXMLPosition == -1 || widthXMLPosition == -1) { throw new UnrecognizeFormatException(); }

	    int height = Integer.parseInt(attrs.item(heightXMLPosition).getNodeValue());
	    int width = Integer.parseInt(attrs.item(widthXMLPosition).getNodeValue());

	    boolean[][] data = parseData(node, height, width);

	    ((ScilabBoolean) obj).setData(data);
	} catch (UnrecognizeFormatException e) {
	    e.printStackTrace();
	}
	return obj;
    }

	/**
	 * Parse the node data and return them.
	 * @param node The node we are working on
	 * @param height the height of the data
	 * @param width the width of the data
	 * @return the data
	 * @throws UnrecognizeFormatException when the data cannot be parsed.
	 */
	private boolean[][] parseData(Node node, int height, int width)
			throws UnrecognizeFormatException {
		boolean[][] data = new boolean[height][width];
	    NodeList allValues = node.getChildNodes();
	    for (int i = 0; i < allValues.getLength(); ++i) {
	    	int lineXMLPosition = -1;
	    	int columnXMLPosition = -1;
	    	int valueXMLPosition = -1;
	    	NamedNodeMap dataAttributes = allValues.item(i).getAttributes();
	    	for (int j = 0; j < dataAttributes.getLength(); j++) {
	    		Node attr = dataAttributes.item(j);
	    		if (attr.getNodeName().compareToIgnoreCase(LINE) == 0) { lineXMLPosition = j; }
	    		if (attr.getNodeName().compareToIgnoreCase(COLUMN) == 0) { columnXMLPosition = j; }
	    		if (attr.getNodeName().compareToIgnoreCase(VALUE) == 0) { valueXMLPosition = j; }
	    	}

	    	if (lineXMLPosition == -1 || columnXMLPosition == -1 || valueXMLPosition == -1) { throw new UnrecognizeFormatException(); }
	    	int line = Integer.parseInt(dataAttributes.item(lineXMLPosition).getNodeValue());
	    	int column = Integer.parseInt(dataAttributes.item(columnXMLPosition).getNodeValue());
	    	
	    	if (dataAttributes.item(valueXMLPosition).getNodeValue().compareToIgnoreCase("false") == 0) {
	    		data[line][column] = false;
	    	} else {
	    		data[line][column] = true;
	    	}

	    }
		return data;
	}
}
