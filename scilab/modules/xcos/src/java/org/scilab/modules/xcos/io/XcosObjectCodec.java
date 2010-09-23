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

import org.scilab.modules.graph.utils.ScilabGraphConstants;
import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.xcos.link.commandcontrol.CommandControlLink;
import org.scilab.modules.xcos.link.explicit.ExplicitLink;
import org.scilab.modules.xcos.link.implicit.ImplicitLink;

import com.mxgraph.io.mxCellCodec;
import com.mxgraph.io.mxCodecRegistry;

/**
 * Codec for any xcos object
 */
public class XcosObjectCodec extends mxCellCodec {
	/**
	 * Refs field for codecs
	 */
	protected static final String[] REFS = {"parent", "source", "target"};
	
	/**
	 * Attribute name containing {@link com.mxgraph.model.mxCell} style.
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
     * Register all the links codecs
     */
    public static void registerLinks() {
    	XcosObjectCodec explicitlinkCodec = new XcosObjectCodec(new ExplicitLink() , null , null , null);
    	mxCodecRegistry.register(explicitlinkCodec);
    	XcosObjectCodec implicitlinkCodec = new XcosObjectCodec(new ImplicitLink() , null , null , null);
    	mxCodecRegistry.register(implicitlinkCodec);
    	XcosObjectCodec commandControllinkCodec = new XcosObjectCodec(new CommandControlLink() , null , null , null);
    	mxCodecRegistry.register(commandControllinkCodec);
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

		if (!style.containsKey(ScilabGraphConstants.STYLE_FLIP)) {
			style.put(ScilabGraphConstants.STYLE_FLIP, Boolean.FALSE.toString());
		}

		if (!style.containsKey(ScilabGraphConstants.STYLE_MIRROR)) {
			style.put(ScilabGraphConstants.STYLE_MIRROR, Boolean.FALSE.toString());
		}
	}
}
