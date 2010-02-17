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

import java.io.File;
import java.io.IOException;
import java.io.StringReader;
import java.util.Map;
import java.util.WeakHashMap;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import net.sourceforge.jeuclid.swing.JMathComponent;

import org.apache.batik.bridge.BridgeContext;
import org.apache.batik.bridge.DocumentLoader;
import org.apache.batik.bridge.GVTBuilder;
import org.apache.batik.bridge.UserAgent;
import org.apache.batik.bridge.UserAgentAdapter;
import org.apache.batik.dom.svg.SAXSVGDocumentFactory;
import org.apache.batik.gvt.GraphicsNode;
import org.apache.batik.util.XMLResourceDescriptor;
import org.scilab.forge.jlatexmath.ParseException;
import org.scilab.forge.jlatexmath.TeXConstants;
import org.scilab.forge.jlatexmath.TeXFormula;
import org.scilab.forge.jlatexmath.TeXIcon;
import org.scilab.modules.graph.view.SupportedLabelType;
import org.w3c.dom.Document;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

import com.mxgraph.util.mxUtils;

/**
 * Utilities functions for ScilabGraph 
 */
public final class ScilabGraphUtils extends mxUtils {
	/**
	 * Cache for the generated latex icons
	 */
	private static Map<String, TeXIcon> generatedLatexIcons = new WeakHashMap<String, TeXIcon>();
	
	/**
	 * Cache for the generated MathML components
	 */
	private static Map<String, JMathComponent> generatedMathMLComponents = new WeakHashMap<String, JMathComponent>();
	
	/**
	 * Cache for the generated SVG components
	 */
	private static Map<File, GraphicsNode> generatedSVGComponents = new WeakHashMap<File, GraphicsNode>();
	
	/**
	 * Table conversion between escaped/unescaped HTML symbols
	 */
	private static final String [][] HTML_ESCAPE_TABLE =
	{
	      {"&lt;"     , "<" },
	      {"&gt;"     , ">" },
	      {"&amp;"    , "&" },
	      {"&quot;"   , "\"" },
	      {"&agrave;" , "à" },
	      {"&Agrave;" , "À" },
	      {"&acirc;"  , "â" },
	      {"&auml;"   , "ä" },
	      {"&Auml;"   , "Ä" },
	      {"&Acirc;"  , "Â" },
	      {"&aring;"  , "å" },
	      {"&Aring;"  , "Å" },
	      {"&aelig;"  , "æ" },
	      {"&AElig;"  , "Æ" },
	      {"&ccedil;" , "ç" },
	      {"&Ccedil;" , "Ç" },
	      {"&eacute;" , "é" },
	      {"&Eacute;" , "É" },
	      {"&egrave;" , "è" },
	      {"&Egrave;" , "È" },
	      {"&ecirc;"  , "ê" },
	      {"&Ecirc;"  , "Ê" },
	      {"&euml;"   , "ë" },
	      {"&Euml;"   , "Ë" },
	      {"&iuml;"   , "ï" },
	      {"&Iuml;"   , "Ï" },
	      {"&ocirc;"  , "ô" },
	      {"&Ocirc;"  , "Ô" },
	      {"&ouml;"   , "ö" },
	      {"&Ouml;"   , "Ö" },
	      {"&oslash;" , "ø" },
	      {"&Oslash;" , "Ø" },
	      {"&szlig;"  , "ß" },
	      {"&ugrave;" , "ù" },
	      {"&Ugrave;" , "Ù" },
	      {"&ucirc;"  , "û" },
	      {"&Ucirc;"  , "Û" }, 
	      {"&uuml;"   , "ü" },
	      {"&Uuml;"   , "Ü" },
	      {"&nbsp;"   , " " },
	      {"&reg;"    , "\u00a9" },
	      {"&copy;"   , "\u00ae" },
	      {"&euro;"   , "\u20a0" }
	};

	
	/**
	 * Return a cached or a new instance of a TexIcon generated from the text.
	 * @param text the latex formula
	 * @return the TeXIcon
	 * @throws ParseException when the text is not a valid formula
	 */
	public static TeXIcon getTexIcon(String text) throws ParseException {
		TeXIcon icon;
		String escapedText = SupportedLabelType.Latex.escape(text);
		
		icon = generatedLatexIcons.get(escapedText);
		if (icon == null) {
			TeXFormula tex = new TeXFormula(escapedText);
    		icon = tex.createTeXIcon(TeXConstants.STYLE_DISPLAY, 20);
    		generatedLatexIcons.put(escapedText, icon);
		}
		return icon;
	}
	
	/**
	 * Return a cached or a new instance of a JMathComponent generated from
	 * text.
	 * @param text the MathML formula
	 * @return the {@link JMathComponent} instance
	 * @throws SAXException when the text is not a valid XML file
	 */
	public static JMathComponent getMathMLComponent(String text) throws SAXException {
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
	 * Return a cached or a new instance of a {@link GraphicsNode} generated
	 * from the SVG file.
	 * 
	 * @param filename the file to parse
	 * @return the corresponding graphic node
	 */
	public static GraphicsNode getSVGComponent(File filename) {
		GraphicsNode node;
		
		node = generatedSVGComponents.get(filename);
		if (node == null) {
			try {
				String xmlParser = XMLResourceDescriptor.getXMLParserClassName();
				SAXSVGDocumentFactory df = new SAXSVGDocumentFactory(xmlParser);
				Document doc = df.createDocument(filename.getPath());
				UserAgent userAgent = new UserAgentAdapter();
				DocumentLoader loader = new DocumentLoader(userAgent);
				BridgeContext ctx = new BridgeContext(userAgent, loader);
				ctx.setDynamicState(BridgeContext.DYNAMIC);
				GVTBuilder builder = new GVTBuilder();
				
				node = builder.build(ctx, doc);
				generatedSVGComponents.put(filename, node);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		return node;
	}
	
	/**
	 * Remove the blank char on the beginning of the sequence.
	 * This method modify the {@link StringBuilder} in place.
	 *  
	 * @param seq the sequence
	 */
	public static void removeBlanks(StringBuilder seq) {
		int i;
		for (i = 0; i < seq.length(); i++) {
			char car = seq.charAt(i);
			if (car != ' ' && car != '\n') {
				break;
			}
		}
		
		seq.delete(0, i);
	}

	/**
	 * Unescape html
	 * 
	 * This method modify the {@link StringBuilder} in place.
	 * 
	 * @param escapedText the working text
	 * @param index the beginning index
	 */
	public static void unescape(StringBuilder escapedText, int index) {
		int start, end, table_index;

		start = escapedText.indexOf("&", index);
		if (start > -1) {
			end = escapedText.indexOf(";", start);
			// we don't start from the beginning
			// the next time, to handle the case of
			// the &
			index = start + 1;

			if (end > start) {
				String temp = escapedText.substring(start, end + 1);
				// search in HTML_ESCAPE_TABLE[][] if temp is there
				table_index = 0;
				while (table_index < HTML_ESCAPE_TABLE.length) {
					if (HTML_ESCAPE_TABLE[table_index][0].equals(temp)) {
						break;
					} else {
						table_index++;
					}
				}
				if (table_index < HTML_ESCAPE_TABLE.length) {
					escapedText.replace(start, end + 1, HTML_ESCAPE_TABLE[table_index][1]);
					unescape(escapedText, index); // recursive call
				}
			}
		}
		return;
	}
	
	/**
	 * This class is a static singleton thus it must not be instantiated
	 */
	private ScilabGraphUtils() { }
}
