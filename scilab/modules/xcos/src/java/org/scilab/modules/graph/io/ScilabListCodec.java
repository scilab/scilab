/*
 * Scilab (http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graph.io;


import java.util.Map;

import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.hdf5.scilabTypes.ScilabTList;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;

/**
 * Define serialization for a {@link ScilabList} instance.
 */
public class ScilabListCodec  extends ScilabObjectCodec {

    
    private static final String SCILAB_CLASS = "scilabClass";

    /**
     * Default constructor
	 * @param template Prototypical instance of the object to be encoded/decoded.
	 * @param exclude Optional array of fieldnames to be ignored.
	 * @param idrefs Optional array of fieldnames to be converted to/from references.
	 * @param mapping Optional mapping from field- to attributenames.
     */
    public ScilabListCodec(Object template, String[] exclude, String[] idrefs, Map<String, String> mapping)
    {
	super(template, exclude, idrefs, mapping);

    }

    /**
     * Add the class name as an attribute.
     * 
	 * @param enc Codec that controls the encoding process.
	 * @param obj Object to be encoded.
	 * @param node XML node that represents the default encoding.
	 * @return Returns the resulting node of the encoding.
     * @see com.mxgraph.io.mxObjectCodec#beforeEncode(com.mxgraph.io.mxCodec, java.lang.Object, org.w3c.dom.Node)
     */
	public Object beforeEncode(mxCodec enc, Object obj, Node node) {
		mxCodec.setAttribute(node, SCILAB_CLASS, obj.getClass().getSimpleName());
		
		return obj;
	}
	
	/**
	 * Instantiate defined class for the attribute 
	 * 
	 * @param node the node we are working on
	 * @return the instance of the node.
	 * @see com.mxgraph.io.mxObjectCodec#cloneTemplate(org.w3c.dom.Node)
	 */
	public Object cloneTemplate(Node node) {
		Object obj = null;
		if (node.getAttributes().getNamedItem(SCILAB_CLASS) != null) { 
			String scilabClass = node.getAttributes().getNamedItem(SCILAB_CLASS).getNodeValue();
			if (scilabClass.equalsIgnoreCase("ScilabMList")) { 
				obj = new ScilabMList();
			} else if (scilabClass.equalsIgnoreCase("ScilabTList")) { 
				obj = new ScilabTList();
			} else if (scilabClass.equalsIgnoreCase("ScilabList")) { 
				obj = new ScilabList();
			} else {
				
				obj = super.cloneTemplate(node);
			}
			
		} else {
			obj = super.cloneTemplate(node);
		}

		return obj;
	}
	
}
