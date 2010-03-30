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

import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.xcos.io.XcosObjectCodec;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;

/**
 * Codec for any Port
 */
public class BasicPortCodec extends XcosObjectCodec {

    private static final String DATA_TYPE = "dataType";

	/**
	 * The constructor used on for configuration
	 * @param template Prototypical instance of the object to be encoded/decoded.
	 * @param exclude Optional array of fieldnames to be ignored.
	 * @param idrefs Optional array of fieldnames to be converted to/from references.
	 * @param mapping Optional mapping from field- to attributenames.
	 */
    public BasicPortCodec(Object template, String[] exclude, String[] idrefs, Map<String, String> mapping)
    {
	super(template, exclude, idrefs, mapping);

    }

	/**
	 * Things to do before encoding
	 * @param enc Codec that controls the encoding process.
	 * @param obj Object to be encoded.
	 * @param node XML node to encode the object into.
	 * @return Returns the object to be encoded by the default encoding.
	 * @see com.mxgraph.io.mxObjectCodec#beforeEncode(com.mxgraph.io.mxCodec, java.lang.Object, org.w3c.dom.Node)
	 */
    public Object beforeEncode(mxCodec enc, Object obj, Node node) {
	((Element) node).setAttribute(DATA_TYPE,
		String.valueOf(((BasicPort) obj).getDataType()));
	return super.beforeEncode(enc, obj, node);
    }

	/**
	 * Apply compatibility pattern to the decoded object
	 * @param dec Codec that controls the decoding process.
	 * @param node XML node to decode the object from.
	 * @param obj Object decoded.
	 * @return The Object transformed 
	 * @see org.scilab.modules.xcos.io.XcosObjectCodec#afterDecode(com.mxgraph.io.mxCodec, org.w3c.dom.Node, java.lang.Object)
	 */
    public Object afterDecode(mxCodec dec, Node node, Object obj) {
	String attr = ((Element) node).getAttribute(DATA_TYPE);

	if (attr == null || attr.equals("")) {
	    ((BasicPort) obj).setDataType(BasicPort.DataType.REAL_MATRIX);

	} else {
	    ((BasicPort) obj).setDataType(BasicPort.DataType.valueOf(attr));
	}

	//update style from version to version.
	StyleMap map = new StyleMap(((Element) node).getAttribute(STYLE));
	formatStyle(map, (BasicPort) obj);
	((BasicPort) obj).setStyle(map.toString());
	
	return super.afterDecode(dec, node, obj);
    }

    /**
     * Format the style value
     * @param map The style as a map
     * @param obj the associated obj
     */
	private void formatStyle(StyleMap map, BasicPort obj) {

		// Append the bloc style if not present.
		String name = obj.getClass().getSimpleName();
		if (!map.containsKey(name)) {
			map.put(name, null);
		}

		// Replace direction by rotation
		formatStyle(map);

		// Update the rotation value according to the Orientation
		updateRotationFromOrientation(map, obj);
	}

	/**
	 * Update the rotation value when the block has been rotated on 5.2.0
	 * format. Update it according to the Orientation field added 2010/01/08
	 * between 5.2.0 and 5.2.1.
	 * 
	 * @param map The previous style value
	 * @param obj The port we are working on
	 */
	private void updateRotationFromOrientation(StyleMap map, BasicPort obj) {
		final Orientation orientation = obj.getOrientation();
		int rotation = 0;
		boolean flipped = false;
		boolean mirrored = false;

		
		if (map.get(XcosConstants.STYLE_ROTATION) != null) {
			rotation = Integer.parseInt(map.get(XcosConstants.STYLE_ROTATION));
		} else {
			rotation = 0;
		}
		
		flipped = Boolean.parseBoolean(map.get(XcosConstants.STYLE_FLIP));
		mirrored = Boolean.parseBoolean(map.get(XcosConstants.STYLE_MIRROR));

		// Calculate the rotation for this kind of port.
		rotation = orientation.getAbsoluteAngle(obj.getClass(), flipped, mirrored);

		map.put(XcosConstants.STYLE_ROTATION, Integer.toString(rotation));
	}
}
