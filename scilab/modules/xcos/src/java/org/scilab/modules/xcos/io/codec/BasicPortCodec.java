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
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.scilab.modules.xcos.io.XcosObjectCodec;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.BasicPort.Orientation;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;

public class BasicPortCodec extends XcosObjectCodec {

    private static final String DATA_TYPE = "dataType";
    
    private static final Pattern ROTATION_STYLE = Pattern.compile("(.*)(;rotation=(0|90|180|270))(.*)");
    private static final int ROTATION_STYLE_BEFORE_GROUP = 1;
    private static final int ROTATION_STYLE_VALUE_GROUP = 3;
    private static final int ROTATION_STYLE_AFTER_GROUP = 4;
    
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
	
	// Replace rotation by defaultOrientation and the new rotation value.
	result = updateDefaultOrientation(result, obj);
	
	if (result.compareTo("") == 0) {
		result = obj.getTypeName();
	}
	return result;
    }

    /**
     * Replace the style rotation field by setting the obj defaultOrientation property.
     * @param style The style applied to the element.
     * @param obj The port we are working on.
     * @return The new style value
     * 
     * @since 5.2.0+ 
     */
	private String updateDefaultOrientation(String style, BasicPort obj) {
		String result = style;
		
		Matcher m = ROTATION_STYLE.matcher(style);
		if (m.matches()) {
			
			// Parse the rotation field value
			String value = m.group(ROTATION_STYLE_VALUE_GROUP);
			int rotation = Integer.parseInt(value);
			
			// Update angle value
			rotation = obj.getDefaultOrientation().getAngle(rotation, false, false);
			obj.setAngle(rotation);
			
			// Update the field
			result = m.replaceAll(m.group(ROTATION_STYLE_BEFORE_GROUP)
					+ ";rotation=" + rotation
					+ m.group(ROTATION_STYLE_AFTER_GROUP));
		}
		return result;
	}
}
