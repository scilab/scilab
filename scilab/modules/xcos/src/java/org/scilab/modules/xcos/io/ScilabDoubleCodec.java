package org.scilab.modules.xcos.io;

import java.util.Map;

import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;

public class ScilabDoubleCodec extends XcosObjectCodec {

	private static final String COLUMN = "column";
	private static final String LINE = "line";
	private static final String DATA = "data";
	private static final String REALPART = "realPart";
	private static final String IMGPART = "imaginaryPart";
	private static final String HEIGHT = "height";
	private final static String WIDTH = "width";

	public ScilabDoubleCodec(Object template) {
		super(template);
	}

	public ScilabDoubleCodec(Object template, String[] exclude, String[] idrefs, Map mapping)
	{
		super(template, exclude, idrefs, mapping);

	}

	public Node encode(mxCodec enc, Object obj)
	{
		String name = mxCodecRegistry.getName(obj.getClass());
		Node node = enc.getDocument().createElement(name);

		ScilabDouble scilabDouble = (ScilabDouble) obj;
		mxCodec.setAttribute(node, WIDTH, scilabDouble.getWidth());
		mxCodec.setAttribute(node, HEIGHT, scilabDouble.getHeight());

		for(int i = 0 ; i < scilabDouble.getHeight() ; ++i) {
			for(int j = 0 ; j < scilabDouble.getWidth(); ++j) {
				Node data = enc.getDocument().createElement(DATA);
				mxCodec.setAttribute(data, LINE, i);
				mxCodec.setAttribute(data, COLUMN, j);
				if (scilabDouble.getRealPart() != null ){
					mxCodec.setAttribute(data, REALPART, scilabDouble.getRealPart()[i][j]);
				}
				if (scilabDouble.getImaginaryPart() != null ){
					mxCodec.setAttribute(data, IMGPART, scilabDouble.getImaginaryPart()[i][j]);
				}
				node.appendChild(data);
			}
		}
		return node;
	}

	public Object decode(mxCodec dec, Node node, Object into)
	{
		Object obj = null;
		boolean isRealOnly = true ;
		try {
			if (!(node instanceof Element)) { return null; }
			obj = cloneTemplate(node);

			//System.err.println("*** clone Template = "+obj);

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
			if (height != 0 && width != 0  ){ 
				double[][] realData = new double[height][width];
				double[][] imgData = new double[height][width];
				NodeList allValues = node.getChildNodes();
				for (int i = 0 ; i < allValues.getLength() ; ++i) {
					int lineXMLPosition = -1;
					int columnXMLPosition = -1;
					int realPartXMLPosition = -1;
					int imaginaryPartXMLPosition = -1;
					NamedNodeMap dataAttributes = allValues.item(i).getAttributes();
					for (int j = 0; j < dataAttributes.getLength(); j++)
					{
						Node attr = dataAttributes.item(j);
						if (attr.getNodeName().compareToIgnoreCase(LINE) == 0) { lineXMLPosition = j; }
						if (attr.getNodeName().compareToIgnoreCase(COLUMN) == 0) { columnXMLPosition = j; }
						if (attr.getNodeName().compareToIgnoreCase(REALPART) == 0) { realPartXMLPosition = j; }
						if (attr.getNodeName().compareToIgnoreCase(IMGPART) == 0) { imaginaryPartXMLPosition = j; }
					}
	
					if (lineXMLPosition == -1 || columnXMLPosition == -1 || realPartXMLPosition == -1) { throw new UnrecognizeFormatException(); }
					int line = Integer.parseInt(dataAttributes.item(lineXMLPosition).getNodeValue());
					int column = Integer.parseInt(dataAttributes.item(columnXMLPosition).getNodeValue());
	
					realData[line][column] = Double.parseDouble(dataAttributes.item(realPartXMLPosition).getNodeValue());
					if (imaginaryPartXMLPosition != -1){
						isRealOnly = false ;
						imgData[line][column] = Double.parseDouble(dataAttributes.item(imaginaryPartXMLPosition).getNodeValue());
					}
				}
	
				((ScilabDouble) obj).setRealPart(realData);
				if (!isRealOnly){
					((ScilabDouble) obj).setImaginaryPart(imgData);
				}
			}

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
