/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan Simon
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

import org.scilab.modules.xcos.io.XcosObjectCodec;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.BasicPort.Orientation;
import org.scilab.modules.xcos.utils.StyleMap;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;
import com.mxgraph.util.mxUtils;

public class BasicPortCodec extends XcosObjectCodec {

    private static final String DATA_TYPE = "dataType";
    
    public BasicPortCodec(Object template) {
	super(template);
    }

    public BasicPortCodec(Object template, String[] exclude, String[] idrefs, Map<String, String> mapping)
    {
	super(template, exclude, idrefs, mapping);

    }

    public Object beforeEncode(mxCodec enc, Object obj, Node node) {
	((Element) node).setAttribute(DATA_TYPE,
		String.valueOf(((BasicPort) obj).getDataType()));
	return super.beforeEncode(enc, obj, node);
    }

    public Object afterDecode(mxCodec dec, Node node, Object obj) {
	String attr = ((Element) node).getAttribute(DATA_TYPE);

	if(attr == null || attr.equals("")) {
	    ((BasicPort) obj).setDataType(BasicPort.DataType.REAL_MATRIX);

	} else {
	    ((BasicPort) obj).setDataType(BasicPort.DataType.valueOf(attr));
	}

	//update style from version to version.
	((BasicPort)obj).setStyle(formatStyle(((Element) node).getAttribute(STYLE), (BasicPort) obj));
	
	return super.afterDecode(dec, node, obj);
    }

    private String formatStyle(String style, BasicPort obj) {
    String result;
    	
    // Replace direction by rotation
	result = formatStyle(style);
	
	// Update the rotation value according to the Orientation
	result = updateRotationFromOrientation(style, obj);
	
	if (result.compareTo("") == 0) {
		result = obj.getTypeName();
	}
	return result;
    }

	/**
	 * Update the rotation value when the block has been rotated on 5.2.0
	 * format. Update it according to the Orientation field added 2010/01/08
	 * between 5.2.0 and 5.2.1.
	 * 
	 * @param style The previous style value
	 * @param obj The port we are working on
	 * @return The new style value
	 */
	private String updateRotationFromOrientation(String style, BasicPort obj) {
		final Orientation orientation = obj.getOrientation();
		int rotation = 0;
		boolean flipped = false;
		boolean mirrored = false;

		StyleMap map = new StyleMap(style);
		
		rotation = Integer.parseInt(map.get("rotation"));
		flipped = Boolean.parseBoolean(map.get("flip"));
		mirrored = Boolean.parseBoolean(map.get("mirror"));

		// First calculate the block angle then calculate the current rotation
		// from it.
		rotation = orientation.getAngle(orientation.getBlockRotationValue(
				rotation, flipped, mirrored), flipped, mirrored);

		return mxUtils.setStyle(style, XcosConstants.STYLE_ROTATION, Integer
				.toString(rotation));
	}
}
