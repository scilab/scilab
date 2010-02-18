/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graph.utils;

import java.awt.Component;
import java.io.IOException;
import java.io.StringReader;
import java.util.Map;
import java.util.WeakHashMap;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import net.sourceforge.jeuclid.swing.JMathComponent;

import org.scilab.modules.graph.view.SupportedLabelType;
import org.w3c.dom.Document;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

/**
 * Contains useful method for rendering a MathML formula.
 * 
 * Before accessing this class you must load the jeuclid.jar file on the
 * classpath.
 */
public final class MathMLRenderUtils {
	/**
	 * Cache for the generated MathML components
	 */
	private static Map<String, JMathComponent> generatedMathMLComponents = new WeakHashMap<String, JMathComponent>();
	
	/**
	 * Return a cached or a new instance of a JMathComponent generated from
	 * text.
	 * @param text the MathML formula
	 * @return the {@link JMathComponent} instance
	 * @throws SAXException when the text is not a valid XML file
	 */
	public static Component getMathMLComponent(String text) throws SAXException {
		String escapedText = SupportedLabelType.MathML.escape(text);
		
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder builder;
		Document doc;
		JMathComponent comp;
		
		comp = generatedMathMLComponents.get(text);
		if (comp == null) {
			try {
				builder = factory.newDocumentBuilder();
				doc = builder.parse(new InputSource(new StringReader(escapedText)));
				comp = new JMathComponent();
				comp.setDocument(doc.getFirstChild());
				generatedMathMLComponents.put(text, comp);
			} catch (ParserConfigurationException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
		return comp;
	}
	
	/**
	 * This class is a static singleton
	 */
	private MathMLRenderUtils() { }
}
