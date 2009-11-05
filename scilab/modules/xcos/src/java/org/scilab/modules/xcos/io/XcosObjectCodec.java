package org.scilab.modules.xcos.io;

import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.Collection;
import java.util.Map;

import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxObjectCodec;
import com.mxgraph.model.mxCell;

public class XcosObjectCodec extends mxObjectCodec {
	
	public XcosObjectCodec(Object template) {
		super(template);
	}
	
	
	public XcosObjectCodec(Object template, String[] exclude, String[] idrefs,
			Map mapping)
	{
		super(template, exclude, idrefs, mapping);
		
	}
	
	public Object afterDecode(mxCodec dec, Node node, Object obj)
	{
		if(node.getNodeName().equals("mxCell")){
			NamedNodeMap attrs = node.getAttributes();
    		for (int i = 0; i < attrs.getLength(); i++)
    		{
    			Node attr = attrs.item(i);
    			if (attr.getNodeName().compareToIgnoreCase("id") == 0) {
    				((mxCell)obj).setId(attr.getNodeValue());
    			}
    		}
		}
		return obj;
	}
	
}
