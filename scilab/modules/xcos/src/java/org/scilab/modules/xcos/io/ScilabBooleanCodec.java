package org.scilab.modules.xcos.io;

import java.util.Map;

import org.scilab.modules.hdf5.scilabTypes.ScilabBoolean;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;

public class ScilabBooleanCodec extends XcosObjectCodec {

    
    private static final String VALUE = "value";
    private static final String COLUMN = "column";
    private static final String LINE = "line";
    private static final String DATA = "data";
    private static final String HEIGHT = "height";
    private final static String WIDTH = "width";
    
    public ScilabBooleanCodec(Object template) {
	super(template);
    }


    public ScilabBooleanCodec(Object template, String[] exclude, String[] idrefs, Map mapping)
    {
	super(template, exclude, idrefs, mapping);

    }

    public Node encode(mxCodec enc, Object obj)
    {
	String name = mxCodecRegistry.getName(obj.getClass());
	Node node = enc.getDocument().createElement(name);

	ScilabBoolean scilabBoolean = (ScilabBoolean) obj;
	mxCodec.setAttribute(node, WIDTH, scilabBoolean.getWidth());
	mxCodec.setAttribute(node, HEIGHT, scilabBoolean.getHeight());

	for(int i = 0 ; i < scilabBoolean.getHeight() ; ++i) {
	    for(int j = 0 ; j < scilabBoolean.getHeight() ; ++j) {
		Node data = enc.getDocument().createElement(DATA);
		mxCodec.setAttribute(data, LINE, i);
		mxCodec.setAttribute(data, COLUMN, j);
		mxCodec.setAttribute(data, VALUE, scilabBoolean.getData()[i][j]);
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
	    for (int i = 0; i < attrs.getLength(); i++)
	    {
		Node attr = attrs.item(i);
		if (attr.getNodeName().compareToIgnoreCase(WIDTH) == 0) { widthXMLPosition = i; }
		if (attr.getNodeName().compareToIgnoreCase(HEIGHT) == 0) { heightXMLPosition = i; }
	    }
	    if (heightXMLPosition == -1 || widthXMLPosition == -1) { throw new UnrecognizeFormatException(); }

	    int height = Integer.parseInt(attrs.item(heightXMLPosition).getNodeValue());
	    int width = Integer.parseInt(attrs.item(widthXMLPosition).getNodeValue());

	    boolean[][] data = new boolean[height][width];
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
		
		if(dataAttributes.item(valueXMLPosition).getNodeValue() == "0"){
			data[line][column] = true;
		} else {
			data[line][column] = false;
		}
		
	    }

	    ((ScilabBoolean) obj).setData(data);
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
