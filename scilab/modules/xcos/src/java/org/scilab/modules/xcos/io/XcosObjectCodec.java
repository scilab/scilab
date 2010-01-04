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

import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxObjectCodec;
import com.mxgraph.model.mxCell;

public class XcosObjectCodec extends mxObjectCodec {
    protected static final String STYLE = "style";
    protected static final String ROTATION = "rotation";
    protected static final String DIRECTION = "direction";
    protected static final String EAST = "east";
    protected static final String NORTH = "north";
    protected static final String WEST = "west";
    protected static final String SOUTH = "south";
    protected static final String FLIP = "flip=";
    protected static final String MIRROR = "mirror=";
    protected static final String COMMAND = "ControlPort";
    protected static final String CONTROL = "CommandPort";

    public XcosObjectCodec(Object template) {
	super(template);
    }


    public XcosObjectCodec(Object template, String[] exclude, String[] idrefs,
	    Map<String, String> mapping) {
	super(template, exclude, idrefs, mapping);

    }

    public Object afterDecode(mxCodec dec, Node node, Object obj){
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

    public String formatStyle(String style) {
	if(style.compareTo("") == 0) {
	    return style;
	}
	
	String result = style;
	result = result.replaceAll(DIRECTION, ROTATION);
	if(result.indexOf(CONTROL) < 0 && result.indexOf(COMMAND) < 0) {
	    result = result.replaceAll(EAST, "0");
	    result = result.replaceAll(NORTH, "270");
	    result = result.replaceAll(WEST, "180");
	    result = result.replaceAll(SOUTH, "90");
	} else {
	    result = result.replaceAll(EAST, "270");
	    result = result.replaceAll(NORTH, "180");
	    result = result.replaceAll(WEST, "90");
	    result = result.replaceAll(SOUTH, "0");
	}

	if(result.indexOf(FLIP) < 0) {
	    result += ";flip=false";
	}

	if(result.indexOf(MIRROR) < 0) {
	    result += ";mirror=false";
	}

	return result;
    }
}
