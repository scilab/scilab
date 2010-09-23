/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Clément DAVID
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

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.types.ScilabDouble;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;

/**
 * Define serialization for a {@link ScilabDouble} instance.
 */
public class ScilabDoubleCodec extends ScilabObjectCodec {

	private static final String REALPART = "realPart";
	private static final String IMGPART = "imaginaryPart";

    /**
     * Default constructor
	 * @param template Prototypical instance of the object to be encoded/decoded.
	 * @param exclude Optional array of fieldnames to be ignored.
	 * @param idrefs Optional array of fieldnames to be converted to/from references.
	 * @param mapping Optional mapping from field- to attributenames.
     */
	public ScilabDoubleCodec(Object template, String[] exclude, String[] idrefs, Map<String, String> mapping)
	{
		super(template, exclude, idrefs, mapping);

	}

	/**
	 * Encodes the specified object and returns a node representing then given
	 * object. Calls beforeEncode after creating the node and afterEncode
	 * with the resulting node after processing.
	 * 
	 * @param enc Codec that controls the encoding process.
	 * @param obj Object to be encoded.
	 * @return Returns the resulting XML node that represents the given object. 
	 */
    @Override
	public Node encode(mxCodec enc, Object obj) {
		String name = mxCodecRegistry.getName(obj);
		Node node = enc.getDocument().createElement(name);

		ScilabDouble scilabDouble = (ScilabDouble) obj;
		mxCodec.setAttribute(node, WIDTH, scilabDouble.getWidth());
		mxCodec.setAttribute(node, HEIGHT, scilabDouble.getHeight());

		for (int i = 0; i < scilabDouble.getHeight(); ++i) {
			for (int j = 0; j < scilabDouble.getWidth(); ++j) {
				Node data = enc.getDocument().createElement(DATA);
				mxCodec.setAttribute(data, LINE, i);
				mxCodec.setAttribute(data, COLUMN, j);
				if (scilabDouble.getRealPart() != null) {
					mxCodec.setAttribute(data, REALPART, scilabDouble.getRealPart()[i][j]);
				}
				if (scilabDouble.getImaginaryPart() != null) {
					mxCodec.setAttribute(data, IMGPART, scilabDouble.getImaginaryPart()[i][j]);
				}
				node.appendChild(data);
			}
		}
		return node;
	}

    /**
     * Parses the given node into the object or returns a new object
	 * representing the given node.
	 * 
	 * @param dec Codec that controls the encoding process.
	 * @param node XML node to be decoded.
	 * @param into Optional object to encode the node into.
	 * @return Returns the resulting object that represents the given XML node
	 * or the object given to the method as the into parameter.
	 */
    @Override
	public Object decode(mxCodec dec, Node node, Object into) {
		ScilabDouble obj = null;
		
		try {
			if (node.getNodeType() != Node.ELEMENT_NODE) {
				throw new UnrecognizeFormatException();
			}
			obj = (ScilabDouble) cloneTemplate(node);

			// attrs = {"as", "height", "width"}
			final NamedNodeMap attrs = node.getAttributes();
			if (attrs == null) {
				throw new UnrecognizeFormatException();
			}

			final int height = getHeight(attrs);
			final int width = getWidth(attrs);

			if (height * width == 0) {
				return obj;
			}
			
			final double[][] realData = new double[height][width];
			final double[][] imgData = new double[height][width];
			final boolean isRealOnly = fillData(node, realData, imgData);
			
			obj.setRealPart(realData);
			if (!isRealOnly) {
				obj.setImaginaryPart(imgData);
			}

		} catch (UnrecognizeFormatException e) {
			LogFactory.getLog(ScilabDoubleCodec.class).error(e);
		} catch (NumberFormatException e) {
			LogFactory.getLog(ScilabDoubleCodec.class).error(e);
		}
		
		return obj;
	}

	/**
	 * Fill the data from the node.
	 * 
	 * @param node the ScilabDouble node
	 * @param realData the real data
	 * @param imgData the optional imaginary data
	 * @return true if the imaginary data is empty, false otherwise.
	 * @throws UnrecognizeFormatException on decoding error
	 */
	private boolean fillData(Node node, double[][] realData, double[][] imgData)
			throws UnrecognizeFormatException {
		boolean isRealOnly = true;
		for (Node n = node.getFirstChild(); n != null; n = n.getNextSibling()) {
			if (n.getNodeType() != Node.ELEMENT_NODE) {
				continue;
			}

			final NamedNodeMap dataAttrs = n.getAttributes();
			if (dataAttrs == null) {
				throw new UnrecognizeFormatException();
			}

			final int column = getColumnIndex(dataAttrs);
			final int line = getLineIndex(dataAttrs);

			final Node real = dataAttrs.getNamedItem(REALPART);
			if (real == null) {
				throw new UnrecognizeFormatException();
			}
			realData[line][column] = Double.parseDouble(real.getNodeValue());
			
			final Node img = dataAttrs.getNamedItem(IMGPART);
			if (img != null) {
				isRealOnly = false;
				imgData[line][column] = Double.parseDouble(img.getNodeValue());
			}
		}
		return isRealOnly;
	}
}
