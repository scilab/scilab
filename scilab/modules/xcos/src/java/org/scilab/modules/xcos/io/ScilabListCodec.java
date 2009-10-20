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

import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.hdf5.scilabTypes.ScilabTList;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;

public class ScilabListCodec  extends XcosObjectCodec {

    
    private final static String SCILAB_CLASS = "scilabClass";
    
    public ScilabListCodec(Object template) {
	super(template);
    }


    public ScilabListCodec(Object template, String[] exclude, String[] idrefs, Map mapping)
    {
	super(template, exclude, idrefs, mapping);

    }

	public Object beforeEncode(mxCodec enc, Object obj, Node node)
	{
		mxCodec.setAttribute(node, SCILAB_CLASS, obj.getClass().getSimpleName());
		
		return obj;
	}
	
	public Object afterDecode(mxCodec dec, Node node, Object obj)
	{
		return obj;
	}
	
	public Node beforeDecode(mxCodec dec, Node node, Object obj)
	{
		return node;
	}
	
	public Object cloneTemplate(Node node)
	{
		Object obj = null;
		if ( node.getAttributes().getNamedItem(SCILAB_CLASS) != null){
			String scilabClass = node.getAttributes().getNamedItem(SCILAB_CLASS).getNodeValue();
			if ( scilabClass.equalsIgnoreCase("ScilabMList")){
				obj = new ScilabMList() ;
			} else if ( scilabClass.equalsIgnoreCase("ScilabTList")){
				obj = new ScilabTList() ;
			} else if ( scilabClass.equalsIgnoreCase("ScilabList")){
				obj = new ScilabList() ;
			} else {
				
				obj = super.cloneTemplate(node);
			}
			
		} else {
			obj = super.cloneTemplate(node);
		}

		return obj;
	}
	
}
