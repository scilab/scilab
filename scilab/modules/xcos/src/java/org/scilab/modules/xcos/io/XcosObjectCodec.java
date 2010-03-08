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

import org.scilab.modules.graph.io.ScilabObjectCodec;
import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;
import com.mxgraph.model.mxCell;

/**
 * Codec for any xcos object
 */
public class XcosObjectCodec extends ScilabObjectCodec {
	/**
	 * Attribute name containing {@link mxCell} style.
	 */
    protected static final String STYLE = "style";
    private static final String ROTATION = "rotation";
    private static final String DIRECTION = "direction";
    private static final String WEST = "west";
    private static final String SOUTH = "south";
    private static final String EAST = "east";
    
    private static final int DIRECTION_STEP = 90;

	/**
	 * The constructor used on for configuration
	 * @param template Prototypical instance of the object to be encoded/decoded.
	 * @param exclude Optional array of fieldnames to be ignored.
	 * @param idrefs Optional array of fieldnames to be converted to/from references.
	 * @param mapping Optional mapping from field- to attributenames.
	 */
    public XcosObjectCodec(Object template, String[] exclude, String[] idrefs,
	    Map<String, String> mapping) {
	super(template, exclude, idrefs, mapping);

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

    /**
     * @param style the style to be formatted
     */
	public void formatStyle(StyleMap style) {
		if (style.containsKey(DIRECTION)) {
			String direction = style.get(DIRECTION);

			int angle = 0;
			do {
				if (direction.compareTo(EAST) == 0) {
					break;
				}
				angle += DIRECTION_STEP;
				if (direction.compareTo(SOUTH) == 0) {
					break;
				}
				angle += DIRECTION_STEP;
				if (direction.compareTo(WEST) == 0) {
					break;
				}
				angle += DIRECTION_STEP;
			} while (false);

			style.remove(DIRECTION);
			style.put(ROTATION, Integer.toString(angle));

		}

		if (!style.containsKey(XcosConstants.STYLE_FLIP)) {
			style.put(XcosConstants.STYLE_FLIP, Boolean.FALSE.toString());
		}

		if (!style.containsKey(XcosConstants.STYLE_MIRROR)) {
			style.put(XcosConstants.STYLE_MIRROR, Boolean.FALSE.toString());
		}
	}
}
