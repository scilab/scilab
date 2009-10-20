/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.io;

import java.util.Map;

import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;

public class ScilabStringCodec extends XcosObjectCodec {

    
    private static final String VALUE = "value";
    private static final String COLUMN = "column";
    private static final String LINE = "line";
    private static final String DATA = "data";
    private static final String HEIGHT = "height";
    private final static String WIDTH = "width";
    
    public ScilabStringCodec(Object template) {
	super(template);
    }


    public ScilabStringCodec(Object template, String[] exclude, String[] idrefs, Map mapping)
    {
	super(template, exclude, idrefs, mapping);

    }

    public Node encode(mxCodec enc, Object obj)
    {
	String name = mxCodecRegistry.getName(obj.getClass());
	Node node = enc.getDocument().createElement(name);

	ScilabString scilabString = (ScilabString) obj;
	mxCodec.setAttribute(node, WIDTH, scilabString.getWidth());
	mxCodec.setAttribute(node, HEIGHT, scilabString.getHeight());

	for(int i = 0 ; i < scilabString.getHeight() ; ++i) {
	    for(int j = 0 ; j < scilabString.getWidth() ; ++j) {
		Node data = enc.getDocument().createElement(DATA);
		mxCodec.setAttribute(data, LINE, i);
		mxCodec.setAttribute(data, COLUMN, j);
		mxCodec.setAttribute(data, VALUE, scilabString.getData()[i][j]);
		node.appendChild(data);
	    }
	}
	return node;
    }

    public Object decode(mxCodec dec, Node node, Object into)
    {
	Object obj = null;
	try {
	    if (!(node instanceof Element)) { return null; }
	    obj = cloneTemplate(node);


	    // attrs = {"as", "height", "width"}
	    NamedNodeMap attrs = node.getAttributes();
	    int heightXMLPosition = -1;
	    int widthXMLPosition = -1;
	    for (int i = 0; i < attrs.getLength(); i++)
	    {
		Node attr = attrs.item(i);
		if (attr.getNodeName().compareToIgnoreCase(WIDTH) == 0) { widthXMLPosition = i; }
		if (attr.getNodeName().compareToIgnoreCase(HEIGHT) == 0) { heightXMLPosition = i; }
	    }
	    if (heightXMLPosition == -1 || widthXMLPosition == -1) { throw new UnrecognizeFormatException(); }

	    int height = Integer.parseInt(attrs.item(heightXMLPosition).getNodeValue());
	    int width = Integer.parseInt(attrs.item(widthXMLPosition).getNodeValue());

	    String[][] data = new String[height][width];
	    NodeList allValues = node.getChildNodes();
	    for (int i = 0 ; i < allValues.getLength() ; ++i) {
		int lineXMLPosition = -1;
		int columnXMLPosition = -1;
		int valueXMLPosition = -1;
		NamedNodeMap dataAttributes = allValues.item(i).getAttributes();
		for (int j = 0; j < dataAttributes.getLength(); j++)
		{
		    Node attr = dataAttributes.item(j);
		    if (attr.getNodeName().compareToIgnoreCase(LINE) == 0) { lineXMLPosition = j; }
		    if (attr.getNodeName().compareToIgnoreCase(COLUMN) == 0) { columnXMLPosition = j; }
		    if (attr.getNodeName().compareToIgnoreCase(VALUE) == 0) { valueXMLPosition = j; }
		}

		if (lineXMLPosition == -1 || columnXMLPosition == -1 || valueXMLPosition == -1) { throw new UnrecognizeFormatException(); }
		int line = Integer.parseInt(dataAttributes.item(lineXMLPosition).getNodeValue());
		int column = Integer.parseInt(dataAttributes.item(columnXMLPosition).getNodeValue());
		data[line][column] = dataAttributes.item(valueXMLPosition).getNodeValue();
	    }

	    ((ScilabString) obj).setData(data);
	}
	catch (UnrecognizeFormatException e) {
	    e.printStackTrace();
	}
	finally {
	    return obj;
	}

    }

    private class UnrecognizeFormatException extends Exception {}

}
