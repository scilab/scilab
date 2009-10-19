package org.scilab.modules.xcos.io;

import java.util.Collection;
import java.util.Hashtable;
import java.util.Map;

import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.hdf5.scilabTypes.ScilabTList;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;

public class ScilabListCodec  extends XcosObjectCodec {

    
    private static final String VALUE = "value";
    private static final String COLUMN = "column";
    private static final String LINE = "line";
    private static final String DATA = "data";
    private static final String HEIGHT = "height";
    private final static String WIDTH = "width";
    private final static String LENGTH = "length";
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
			System.out.println(node.getAttributes().getNamedItem(SCILAB_CLASS).getNodeValue());
			String scilabClass = node.getAttributes().getNamedItem(SCILAB_CLASS).getNodeValue();
			if ( scilabClass.equalsIgnoreCase("ScilabMList")){
				obj = new ScilabMList() ;
			}
			if ( scilabClass.equalsIgnoreCase("ScilabTList")){
				obj = new ScilabTList() ;
			}
			if ( scilabClass.equalsIgnoreCase("ScilabList")){
				obj = new ScilabList() ;
			}
			
		} else {
			
		}

		return obj;
	}
	
    private class UnrecognizeFormatException extends Exception {}

}
