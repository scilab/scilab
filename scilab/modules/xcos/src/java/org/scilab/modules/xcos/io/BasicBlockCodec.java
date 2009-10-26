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

package org.scilab.modules.xcos.io;

import java.util.Map;

import org.scilab.modules.xcos.block.BasicBlock;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;

public class BasicBlockCodec extends XcosObjectCodec {

	public BasicBlockCodec(Object template) {
		super(template);
	}


	public BasicBlockCodec(Object template, String[] exclude, String[] idrefs, Map mapping)
	{
		super(template, exclude, idrefs, mapping);

	}
	
	
	public Object beforeEncode(mxCodec enc, Object obj, Node node) {
		((Element) node).setAttribute("simulationFunctionType",
				String.valueOf(((BasicBlock)obj).getSimulationFunctionType()));
		return super.beforeEncode(enc, obj, node);
	}

	public Object afterDecode(mxCodec dec, Node node, Object obj) {
		((BasicBlock) obj).setSimulationFunctionType(
				BasicBlock.SimulationFunctionType.valueOf((((Element)node).getAttribute("simulationFunctionType" ))));
		return super.afterDecode(dec, node, obj);
	}
	

}
