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
	
	public XcosObjectCodec(Object template) {
		super(template);
	}
	
	
	public XcosObjectCodec(Object template, String[] exclude, String[] idrefs,
			Map mapping) {
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
	
}
