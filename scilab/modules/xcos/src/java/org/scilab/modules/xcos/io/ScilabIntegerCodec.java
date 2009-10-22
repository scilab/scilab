package org.scilab.modules.xcos.io;

import java.util.Map;

import org.scilab.modules.hdf5.scilabTypes.ScilabInteger;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;

public class ScilabIntegerCodec extends XcosObjectCodec {

    private static final String BUNSIGNED = "bUnsigned";
	
    private static final String VALUE = "value";
    private static final String COLUMN = "column";
    private static final String LINE = "line";
    private static final String DATA = "data";
    private static final String HEIGHT = "height";
    private final static String WIDTH = "width";
    
    public ScilabIntegerCodec(Object template) {
	super(template);
    }


    public ScilabIntegerCodec(Object template, String[] exclude, String[] idrefs, Map mapping)
    {
	super(template, exclude, idrefs, mapping);

    }

    public Node encode(mxCodec enc, Object obj)
    {
	String name = mxCodecRegistry.getName(obj.getClass());
	Node node = enc.getDocument().createElement(name);

	ScilabInteger scilabInteger = (ScilabInteger) obj;
	mxCodec.setAttribute(node, WIDTH, scilabInteger.getWidth());
	mxCodec.setAttribute(node, HEIGHT, scilabInteger.getHeight());

	for(int i = 0 ; i < scilabInteger.getHeight() ; ++i) {
	    for(int j = 0 ; j < scilabInteger.getWidth() ; ++j) {
		Node data = enc.getDocument().createElement(DATA);
		mxCodec.setAttribute(data, LINE, i);
		mxCodec.setAttribute(data, COLUMN, j);
		mxCodec.setAttribute(data, VALUE, scilabInteger.getData()[i][j]);
		mxCodec.setAttribute(data, BUNSIGNED, scilabInteger.isUnsigned());
		node.appendChild(data);
	    }
	}
	return node;
    }

    public Object decode(mxCodec dec, Node node, Object into)
    {
	Object obj = null;
	try {
	    if (!(node instanceof Element)) { return null; }
	    obj = cloneTemplate(node);


	    // attrs = {"as", "height", "width"}
	    NamedNodeMap attrs = node.getAttributes();
	    int heightXMLPosition = -1;
	    int widthXMLPosition = -1;
		int bUnsignedXMLPosition = -1;
	    for (int i = 0; i < attrs.getLength(); i++)
	    {
		Node attr = attrs.item(i);
		if (attr.getNodeName().compareToIgnoreCase(WIDTH) == 0) { widthXMLPosition = i; }
		if (attr.getNodeName().compareToIgnoreCase(HEIGHT) == 0) { heightXMLPosition = i; }
		if (attr.getNodeName().compareToIgnoreCase(BUNSIGNED) == 0) { bUnsignedXMLPosition = i; }
	    }
	    if (heightXMLPosition == -1 || widthXMLPosition == -1 ) { throw new UnrecognizeFormatException(); }

	    int height = Integer.parseInt(attrs.item(heightXMLPosition).getNodeValue());
	    int width = Integer.parseInt(attrs.item(widthXMLPosition).getNodeValue());
	    boolean bUnsigned  = false; 

	    long[][] data = new long[height][width];
	    NodeList allValues = node.getChildNodes();
	    for (int i = 0 ; i < allValues.getLength() ; ++i) {
		int lineXMLPosition = -1;
		int columnXMLPosition = -1;
		int valueXMLPosition = -1;

		NamedNodeMap dataAttributes = allValues.item(i).getAttributes();
		for (int j = 0; j < dataAttributes.getLength(); j++)
		{
		    Node attr = dataAttributes.item(j);
		    if (attr.getNodeName().compareToIgnoreCase(LINE) == 0) { lineXMLPosition = j; }
		    if (attr.getNodeName().compareToIgnoreCase(COLUMN) == 0) { columnXMLPosition = j; }
		    if (attr.getNodeName().compareToIgnoreCase(VALUE) == 0) { valueXMLPosition = j; }
		   
		}

		if (lineXMLPosition == -1 || columnXMLPosition == -1 || valueXMLPosition == -1) { throw new UnrecognizeFormatException(); }
		int line = Integer.parseInt(dataAttributes.item(lineXMLPosition).getNodeValue());
		int column = Integer.parseInt(dataAttributes.item(columnXMLPosition).getNodeValue());
		bUnsigned  = Boolean.parseBoolean(dataAttributes.item(widthXMLPosition).getNodeValue());
		data[line][column] = Long.parseLong(dataAttributes.item(valueXMLPosition).getNodeValue());
	    }

	    ((ScilabInteger) obj).setData(data,bUnsigned);
	}
	catch (UnrecognizeFormatException e) {
	    e.printStackTrace();
	}
	finally {
	    return obj;
	}

    }

    private class UnrecognizeFormatException extends Exception {}

}
