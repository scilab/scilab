/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
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

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.BasicBlock.SimulationFunctionType;
import org.scilab.modules.xcos.io.XcosObjectCodec;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;
import com.mxgraph.model.mxCell;

public class BasicBlockCodec extends XcosObjectCodec {

	private static final String SIMULATION_FUNCTION_TYPE = "simulationFunctionType";
	
	public BasicBlockCodec(Object template) {
		super(template);
	}


	public BasicBlockCodec(Object template, String[] exclude, String[] idrefs, Map<String, String> mapping)
	{
		super(template, exclude, idrefs, mapping);

	}
	
	
	public Object beforeEncode(mxCodec enc, Object obj, Node node) {
		((Element) node).setAttribute(SIMULATION_FUNCTION_TYPE,
				String.valueOf(((BasicBlock) obj).getSimulationFunctionType()));
		return super.beforeEncode(enc, obj, node);
	}

	public Object afterDecode(mxCodec dec, Node node, Object obj) {
	    ((BasicBlock) obj).setSimulationFunctionType(SimulationFunctionType.DEFAULT);

	    String functionType = (((Element) node).getAttribute(SIMULATION_FUNCTION_TYPE));
	    if(functionType != null && functionType.compareTo("") != 0) {
		SimulationFunctionType type = BasicBlock.SimulationFunctionType.valueOf(functionType);
		if(type != null) {
		    ((BasicBlock) obj).setSimulationFunctionType(type);
		}
	    }

	    if (obj instanceof SuperBlock) {
		((SuperBlock) obj).setChild(null);
	    }

	    NamedNodeMap attrs = node.getAttributes();
	    for (int i = 0; i < attrs.getLength(); i++) {
		Node attr = attrs.item(i);
		if (attr.getNodeName().compareToIgnoreCase("id") == 0) {
		    ((mxCell) obj).setId(attr.getNodeValue());
		}
	    }

	    // update style to replace direction by rotation and add the 
	    // default style if absent
	    ((BasicBlock) obj).setStyle(formatStyle(((Element) node).getAttribute(STYLE), (BasicBlock) obj));
	    ((BasicBlock) obj).updateFieldsFromStyle();
	    
	    return super.afterDecode(dec, node, obj);
	}


	private String formatStyle(String style, BasicBlock obj) {
	    formatStyle(style);
	    
	    if (style.compareTo("") == 0) {
		style = obj.getInterfaceFunctionName();
	    }
	    return style;
	}
}
