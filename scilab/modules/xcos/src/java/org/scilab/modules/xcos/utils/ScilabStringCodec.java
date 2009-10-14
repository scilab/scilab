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

package org.scilab.modules.xcos.utils;

import java.util.Map;

import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;

public class ScilabStringCodec extends XcosObjectCodec {

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
	mxCodec.setAttribute(node, "width", scilabString.getWidth());
	mxCodec.setAttribute(node, "height", scilabString.getHeight());

	for(int i = 0 ; i < scilabString.getHeight() ; ++i) {
	    for(int j = 0 ; j < scilabString.getHeight() ; ++j) {
		Node data = enc.getDocument().createElement("data");
		mxCodec.setAttribute(data, "line", i);
		mxCodec.setAttribute(data, "column", j);
		mxCodec.setAttribute(data, "value", scilabString.getData()[i][j]);
		node.appendChild(data);
	    }
	}
	return node;
    }

    public Object decode(mxCodec dec, Node node)
    {
	System.err.println("*** SPECIAL DECODE ***");
	if (!(node instanceof Element)) { return null; }
	Object obj = cloneTemplate(node);

	NamedNodeMap attrs = node.getAttributes();
	if (attrs.item(0).getNodeName().compareTo("width") != 0) { return null; }
	if (attrs.item(1).getNodeName().compareTo("height") != 0) { return null; }

	int width = Integer.parseInt(attrs.item(0).getNodeValue());
	int height = Integer.parseInt(attrs.item(0).getNodeValue());

	String[][] data = new String[height][width];
	NodeList allValues = node.getChildNodes();
	for (int i = 0 ; i < allValues.getLength() ; ++i) {
	    NamedNodeMap dataAttributes = allValues.item(i).getAttributes();
	    if (dataAttributes.item(0).getNodeName().compareTo("line") != 0) { return null; }
	    if (dataAttributes.item(1).getNodeName().compareTo("column") != 0) { return null; }
	    if (dataAttributes.item(2).getNodeName().compareTo("value") != 0) { return null; }
	    int line = Integer.parseInt(dataAttributes.item(0).getNodeValue());
	    int column = Integer.parseInt(dataAttributes.item(1).getNodeValue());
	    data[line][column] = dataAttributes.item(2).getNodeValue();
	    System.err.println("&&&&&&&&&&&&&& I saw : "+data[line][column]);
	}

	((ScilabString) obj).setData(data);
	
	return obj;
    }

}
