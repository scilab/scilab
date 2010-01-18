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

import org.scilab.modules.xcos.utils.StyleMap;
import org.scilab.modules.xcos.utils.XcosConstants;
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
    
    private static final int DIRECTION_STEP = 90;
    private static final int MAX_ROTATION = 360;

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

    /**
     * @param style the style to be formatted
     */
	public void formatStyle(StyleMap style) {
		if (style.containsKey(DIRECTION)) {
			String direction = style.get(DIRECTION);

			int step = DIRECTION_STEP;
			int angle = 0;
			if (style.containsKey(COMMAND) || style.containsKey(CONTROL)) {
				step = -DIRECTION_STEP;
				angle = DIRECTION_STEP;
			}

			if (direction.compareTo(SOUTH) == 0) {
				angle = (angle + step) % MAX_ROTATION;
			} else if (direction.compareTo(WEST) == 0) {
				angle = (angle + step) % MAX_ROTATION;
			} else if (direction.compareTo(NORTH) == 0) {
				angle = (angle + step) % MAX_ROTATION;
			}

			style.remove(DIRECTION);
			style.put(ROTATION, Integer.toString(angle));

		}

		if (!style.containsKey(FLIP)) {
			style.put(XcosConstants.STYLE_FLIP, Boolean.FALSE.toString());
		}

		if (!style.containsKey(MIRROR)) {
			style.put(XcosConstants.STYLE_MIRROR, Boolean.FALSE.toString());
		}
	}
}
