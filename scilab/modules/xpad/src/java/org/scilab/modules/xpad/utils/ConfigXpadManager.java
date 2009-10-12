/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2009 - INRIA - Allan SIMON
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.xpad.utils;

import java.awt.Color;
import java.awt.Font;
import java.awt.Toolkit;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Hashtable;

import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

import org.scilab.modules.console.GuiManagement;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.TransformerFactoryConfigurationError;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;


public final class ConfigXpadManager {
	private static final int BUFSIZE = 1024;
	
	private static final int MARGIN = 20;
	
	private static final String ERROR_READ = "Could not load file: ";
	private static final String ERROR_WRITE = "Could not save file: ";
	private static final String VALUE = "value";
	private static final String STYLE = "style";
	private static final String FONT_SIZE = "FontSize";
	private static final String FONT_STYLE = "FontStyle";
	private static final String FONT_NAME = "FontName";
	private static final String DEFAULT = "default";
	private static final String WIDTH = "width";
	private static final String HEIGHT = "height";
	private static final String XCOORD = "x";
	private static final String YCOORD = "y";
	private static final String MAINWINPOSITION = "MainWindowPosition";
	private static final String MAINWINSIZE = "MainWindowSize";
	
	private static final String FOREGROUNDCOLOR = "ForegroundColor";
	private static final String BACKGROUNDCOLOR = "BackgroundColor";
	private static final String COLORPREFIX = "#";
	
	private static final String NAME = "name";
	
	private static final String PROFILE = "Profile";
	private static final String XPAD_CONFIG_FILE = System.getenv("SCI") + "/modules/xpad/etc/xpadConfiguration.xml";
	
    private static final String USER_XPAD_CONFIG_FILE = GuiManagement.getSCIHOME() + "/xpadConfiguration.xml";
	private static final int PLAIN = 0;
	private static final int BOLD =  1;
	private static final int ITALIC = 2;
	private static final int BOLDITALIC = 3;
	
	private static final int DEFAULT_WIDTH = 650;
	private static final int DEFAULT_HEIGHT = 550;
	
	private static final int MAX_RECENT_FILES = 10;
	
	
	private static Document document; 
	
	
	/**
	 * Constructor
	 */
	private ConfigXpadManager() {
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Create a copy of Scilab configuration file in the user directory
	 */
	public static void createUserCopy() {
		/*TODO*/
		File fileConfig = new File(USER_XPAD_CONFIG_FILE);
		if (!fileConfig.exists() || (fileConfig.length() == 0)) {
			/* Create a local copy of the configuration file */
			copyFile(new File(XPAD_CONFIG_FILE), new File(USER_XPAD_CONFIG_FILE));
			
		}
	}
	/**
	 * Get the name of the user configuration file
	 * @return the name of the configuration file
	 */
	public static String getUserConfigFile() {
		/*TODO*/
		return USER_XPAD_CONFIG_FILE;
	}
	
	/**
	 * Get all Style name 
	 * @return a array list of all style name
	 */
	
	public static ArrayList<String> getAllStyleName() {
		ArrayList<String> stylesName = new ArrayList<String>();

		readDocument();


		Element root = document.getDocumentElement();
		NodeList styles = root.getElementsByTagName(STYLE);
		
		for (int i = 0; i < styles.getLength(); ++i) {
			Element style = (Element) styles.item(i);
		
			stylesName.add(style.getAttribute(NAME));

			
		}		
		return stylesName;
		
		
	}
	
	/**
	 * Get the font name
	 * @return the name of the font
	 */
	public static String getFontName() {
		
		/*load file*/
		readDocument();
		
		Element root = document.getDocumentElement();
		
		NodeList profiles = root.getElementsByTagName(PROFILE);
		Element xpadProfile = (Element) profiles.item(0);
		
		NodeList fontNameElement = xpadProfile.getElementsByTagName(FONT_NAME);
		Element fontName = (Element) fontNameElement.item(0);
		return fontName.getAttribute(VALUE);
		
	}
	
	/**
	 * Get the font size
	 * @return the font size
	 */
	public static int getFontSize() {
		/*load file*/
		readDocument();
		
		Element root = document.getDocumentElement();
		
		NodeList profiles = root.getElementsByTagName(PROFILE);
		Element xpadProfile = (Element) profiles.item(0);
		
		NodeList fontSizeElement = xpadProfile.getElementsByTagName(FONT_SIZE);
		Element fontSize = (Element) fontSizeElement.item(0);
		return Integer.parseInt(fontSize.getAttribute(VALUE));
		
	}
	
	/**
	 * Get all font style 
	 * @return true if the font style is bold , false otherwise
	 */
	public static Hashtable<String, Boolean> getAllisBold() {
		/*load file*/
		readDocument();
		Hashtable<String, Boolean > stylesIsBoldTable = new Hashtable<String, Boolean>();

		Element root = document.getDocumentElement();
		NodeList styles = root.getElementsByTagName(STYLE);
		
		for (int i = 0; i < styles.getLength(); ++i) {
			Element style = (Element) styles.item(i);
				
		
			NodeList fontStyleElement = style.getElementsByTagName(FONT_STYLE);
			Element fontStyle = (Element) fontStyleElement.item(0);
			int value = Integer.parseInt(fontStyle.getAttribute(VALUE));
			
			if (value  == BOLD || value == BOLDITALIC) {
				stylesIsBoldTable.put(style.getAttribute(NAME), true);
			} else {
				stylesIsBoldTable.put(style.getAttribute(NAME), false);
			}
		}		
		return stylesIsBoldTable;
	}
	
	
	/**
	 * Get the font setting
	 * @return the font
	 */
	public static Font getFont() {
		
		/*load file*/
		readDocument();
		Font font;
		Element root = document.getDocumentElement();
		
		NodeList profiles = root.getElementsByTagName(PROFILE);
		Element xpadProfile = (Element) profiles.item(0);
		
		NodeList fontSizeElement = xpadProfile.getElementsByTagName(FONT_SIZE);
		Element fontSize = (Element) fontSizeElement.item(0);
		int size = Integer.parseInt(fontSize.getAttribute(VALUE));
		
		NodeList fontNameElement = xpadProfile.getElementsByTagName(FONT_NAME);
		Element fontName = (Element) fontNameElement.item(0);
		String name = fontName.getAttribute(VALUE);
		
		NodeList fontStyleElement = xpadProfile.getElementsByTagName(FONT_STYLE);
		Element fontStyle = (Element) fontStyleElement.item(0);
		int style = Integer.parseInt(fontStyle.getAttribute(VALUE));
		
		if (style == PLAIN) {
			font = new Font(name, Font.PLAIN, size); 
			
		} else if (style == BOLD) {
			font = new Font(name, Font.BOLD, size); 		
			
		} else if (style == ITALIC) {
			font = new Font(name, Font.ITALIC, size); 
			
		} else if (style == BOLDITALIC) {
			font = new Font(name, Font.BOLD | Font.ITALIC , size); 
			
		} else {
			font = new Font(name, Font.PLAIN, size); 
		}
			
		return font;
	}
	
	/**
	 * Get Default Font Settings
	 * @return the default font
	 */
	
	public static Font getDefaultFont() {
		/*load file*/
		readDocument();
		Font font;
		Element root = document.getDocumentElement();
		
		NodeList profiles = root.getElementsByTagName(PROFILE);
		Element xpadProfile = (Element) profiles.item(0);
		
		NodeList fontSizeElement = xpadProfile.getElementsByTagName(FONT_SIZE);
		Element fontSize = (Element) fontSizeElement.item(0);
		int size = Integer.parseInt(fontSize.getAttribute(DEFAULT));
		
		NodeList fontNameElement = xpadProfile.getElementsByTagName(FONT_NAME);
		Element fontName = (Element) fontNameElement.item(0);
		String name = fontName.getAttribute(DEFAULT);
		
		NodeList fontStyleElement = xpadProfile.getElementsByTagName(FONT_STYLE);
		Element fontStyle = (Element) fontStyleElement.item(0);
		int style = Integer.parseInt(fontStyle.getAttribute(DEFAULT));
		
		if (style == PLAIN) {
			font = new Font(name, Font.PLAIN, size); 
			
		} else if (style == BOLD) {
			font = new Font(name, Font.BOLD, size); 		
			
		} else if (style == ITALIC) {
			font = new Font(name, Font.ITALIC, size); 
			
		} else if (style == BOLDITALIC) {
			font = new Font(name, Font.BOLD | Font.ITALIC , size); 
			
		} else {
			font = new Font(name, Font.PLAIN, size); 
		}
		return font;
		
	}
	
	/**
	 * Save a new font setting
	 * @param font the new font
	 */
	public static void saveFont(Font font) {
		/*TODO*/
		/*load file */
		readDocument();
		
		Element root = document.getDocumentElement();
		
		NodeList profiles = root.getElementsByTagName(PROFILE);
		Element xpadProfile = (Element) profiles.item(0);
		
		NodeList fontSizeElement = xpadProfile.getElementsByTagName(FONT_SIZE);
		Element fontSize = (Element) fontSizeElement.item(0);
		fontSize.setAttribute(VALUE, Integer.toString(font.getSize()));		
		
		NodeList fontNameElement = xpadProfile.getElementsByTagName(FONT_NAME);
		Element fontName = (Element) fontNameElement.item(0);
		fontName.setAttribute(VALUE, font.getFontName());
		
		NodeList fontStyleElement = xpadProfile.getElementsByTagName(FONT_STYLE);
		Element fontStyle = (Element) fontStyleElement.item(0);
		
		if (!font.isBold() && !font.isItalic()) {
			fontStyle.setAttribute(VALUE, "0");
		} else if (font.isBold() && font.isItalic()) {
			fontStyle.setAttribute(VALUE, "3");
		} else if (font.isBold()) {
			fontStyle.setAttribute(VALUE, "1");
		} else {
			fontStyle.setAttribute(VALUE, "2");
		}
		
		/* Save changes */
		writeDocument();
	}
	
    /**
     * Copy a file
     * @param in src file
     * @param out dest file
     */
	private static void copyFile(File in, File out) {
		FileInputStream fis = null;
		try {
			fis = new FileInputStream(in);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		FileOutputStream fos = null;
		try {
			fos = new FileOutputStream(out);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		byte[] buf = new byte[BUFSIZE];
		int i = 0;
		try {
			while ((i = fis.read(buf)) != -1) {
				fos.write(buf, 0, i);
			}
			fis.close();
			fos.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		/*TODO*/
	}

	/**
	 * Get the background Color 
	 * @return the background Color
	 */
	public static Color getXpadBackgroundColor() {
		/* Load file */
		readDocument();
		
		Element root = document.getDocumentElement();
		
		NodeList profiles = root.getElementsByTagName(PROFILE);
		Element xpadProfile = (Element) profiles.item(0);
		
		NodeList allSizeElements = xpadProfile.getElementsByTagName(BACKGROUNDCOLOR);
		Element xpadBackground = (Element) allSizeElements.item(0);
	
		/*direct create a Color with "#FF00FF" string from the xml */
		return Color.decode(xpadBackground.getAttribute(VALUE));
	}
	
	/**
	 * Save Xpad BackgroundColor
	 * @param color the new Color
	 */
	public static void saveXpadBackground(Color color) {
		
		/* Load file */
		readDocument();
		
		Element root = document.getDocumentElement();
		
		NodeList profiles = root.getElementsByTagName(PROFILE);
		Element xpadProfile = (Element) profiles.item(0);
		
		NodeList allSizeElements = xpadProfile.getElementsByTagName(FOREGROUNDCOLOR);
		Element xpadForeground = (Element) allSizeElements.item(0);
		
		String rgb = Integer.toHexString(color.getRGB());
		xpadForeground.setAttribute(VALUE, COLORPREFIX + rgb.substring(2, rgb.length()));

		/* Save changes */
		writeDocument();	
		
	}
	
	
	/**
	 * Get all the foreground Colors 
	 * @return a Hashtable with the styles and the associated colors.
	 */
	
	public static Hashtable<String, Color> getAllForegroundColors() {
		/* Load file */
		readDocument();
		
		Hashtable<String, Color> stylesColorsTable = new Hashtable<String, Color>();
		
		Element root = document.getDocumentElement();
		NodeList styles = root.getElementsByTagName(STYLE);
		
		for (int i = 0; i < styles.getLength(); ++i) {
			Element style = (Element) styles.item(i);
			
			NodeList allForegroundElements = style.getElementsByTagName(FOREGROUNDCOLOR);
			Element styleForeground = (Element) allForegroundElements.item(0);
			Color styleColor = Color.decode(styleForeground.getAttribute(VALUE));

			stylesColorsTable.put(style.getAttribute(NAME), styleColor);
		}
		
		return stylesColorsTable;
	}
	
	/**
	 * get all default foreground colors of xpad
	 * @return a Hashtable with the styles and the associated default colors.
	 */
	public static Hashtable<String, Color> getAllDefaultForegroundColors() {
		/* Load file */
		readDocument();
		
		Hashtable<String, Color> stylesDefaultColorsTable = new Hashtable<String, Color>();
		
		Element root = document.getDocumentElement();
		NodeList styles = root.getElementsByTagName(STYLE);
		
		for (int i = 0; i < styles.getLength(); ++i) {
			Element style = (Element) styles.item(i);
			
			NodeList allForegroundElements = style.getElementsByTagName(FOREGROUNDCOLOR);
			Element styleForeground = (Element) allForegroundElements.item(0);
			Color styleColor = Color.decode(styleForeground.getAttribute(DEFAULT));

			stylesDefaultColorsTable.put(style.getAttribute(NAME), styleColor);
		}
		
		return stylesDefaultColorsTable;
	}	
	
	/**
	 * save all foreground colors
	 *@param stylesColorsTable a hashtable containing styles and the associated colors
	 */
	public static void saveAllForegroundColors(Hashtable<String, Color> stylesColorsTable) {
		/* Load file */
		readDocument();
		
		Element root = document.getDocumentElement();
		NodeList styles = root.getElementsByTagName(STYLE);
		
		for (int i = 0; i < styles.getLength(); ++i) {
			Element style = (Element) styles.item(i);
			
			String styleName = style.getAttribute(NAME);
			NodeList allForegroundElements = style.getElementsByTagName(FOREGROUNDCOLOR);
			Element styleForeground = (Element) allForegroundElements.item(0);
			
			Color color = stylesColorsTable.get(styleName);
			
			String rgb = Integer.toHexString(color.getRGB());
			styleForeground.setAttribute(VALUE, COLORPREFIX + rgb.substring(2, rgb.length()));

	
		}
		/* Save changes */
		writeDocument();
	}
	
	/**
	 * Get the position of Xpad Main Window
	 * @return the position
	 */
	public static Position getMainWindowPosition() {
		/* Load file */
		readDocument();
		
		Element root = document.getDocumentElement();
		
		NodeList profiles = root.getElementsByTagName(PROFILE);
		Element xpadProfile = (Element) profiles.item(0);
		
		NodeList allPositionElements = xpadProfile.getElementsByTagName(MAINWINPOSITION);
		Element mainWindowPosition = (Element) allPositionElements.item(0);
		if (mainWindowPosition != null) {
			int x = Integer.parseInt(mainWindowPosition.getAttribute(XCOORD));
			int y = Integer.parseInt(mainWindowPosition.getAttribute(YCOORD));
			/* Avoid Xpad Main Window to be out of the screen */
			if (x <= (Toolkit.getDefaultToolkit().getScreenSize().width - MARGIN)
					&& y <= (Toolkit.getDefaultToolkit().getScreenSize().height - MARGIN)) {
				return new Position(x, y);
			} else {
				return new Position(0, 0);
			}
		} else {
			return new Position(0, 0);
		}
		
		/*TODO*/
	}
	
	
	/**
	 * Save the position of Xpad Main Window
	 * @param position the position of Xpad main Window
	 */
	public static void saveMainWindowPosition(Position position) {
		/*TODO*/
		/* Load file */
		readDocument();
		
		Element root = document.getDocumentElement();
		
		NodeList profiles = root.getElementsByTagName(PROFILE);
		Element xpadProfile = (Element) profiles.item(0);
		
		NodeList allPositionElements = xpadProfile.getElementsByTagName(MAINWINPOSITION);
		Element mainWindowPosition = (Element) allPositionElements.item(0);
		
		
		mainWindowPosition.setAttribute(XCOORD, Integer.toString(position.getX()));
		mainWindowPosition.setAttribute(YCOORD, Integer.toString(position.getY()));
		
		/* Save changes */
		writeDocument();	
	}
	

	/**
	 * Save the size of Xpad Main Window
	 * @param size the size of Xpad main Window
	 */
	public static void saveMainWindowSize(Size size) {
		/*TODO*/
		/* Load file */
		readDocument();
		
		Element root = document.getDocumentElement();
		
		NodeList profiles = root.getElementsByTagName(PROFILE);
		Element xpadProfile = (Element) profiles.item(0);
		
		NodeList allPositionElements = xpadProfile.getElementsByTagName(MAINWINSIZE);
		Element mainWindowSize = (Element) allPositionElements.item(0);
		
		
		mainWindowSize.setAttribute(WIDTH, Integer.toString(size.getWidth()));
		mainWindowSize.setAttribute(HEIGHT, Integer.toString(size.getHeight()));
		
		/* Save changes */
		writeDocument();
		
	}
	
	/**
	 * Get the size of Xpad Main Window
	 * @return the size
	 */
	public static Size getMainWindowSize() {
		
		/* Load file */
		readDocument();
		
		Element root = document.getDocumentElement();
		
		NodeList profiles = root.getElementsByTagName(PROFILE);
		Element xpadProfile = (Element) profiles.item(0);
		
		NodeList allSizeElements = xpadProfile.getElementsByTagName(MAINWINSIZE);
		Element mainWindowSize = (Element) allSizeElements.item(0);
		if (mainWindowSize != null) {
			return new Size(Integer.parseInt(mainWindowSize.getAttribute(WIDTH)), Integer.parseInt(mainWindowSize.getAttribute(HEIGHT)));
		} else {
			return new Size(DEFAULT_WIDTH, DEFAULT_HEIGHT);
		}
	}
	
	/**
	 * Get all the  recent opened files
	 * @return a array of uri
	 */
	
	public static ArrayList<File> getAllRecentOpenedFiles() {
		ArrayList<File> files = new ArrayList<File>();

		readDocument();


		Element root = (Element)document.getDocumentElement().getElementsByTagName("recentFiles").item(0);
		if (root != null) {
		    NodeList recentFiles = root.getElementsByTagName("document");

		    for (int i = 0; i < recentFiles.getLength(); ++i) {
			Element style = (Element) recentFiles.item(i);

			File temp = new File(style.getAttribute("path"));

			if (temp.exists()) {
				files.add(temp);
			} else {
				root.removeChild((Node) style);
			}

			/* Save changes */
			writeDocument();
		    }		
		}
		return files;
	}
	
	/**
	 * Add a file to recent Opened Files
	 * @param filePath the path of the files to add 
	 */
	public static void saveToRecentOpenedFiles(String filePath) {
		
		readDocument();

		Element root = (Element) document.getDocumentElement().getElementsByTagName("recentFiles").item(0);
		NodeList recentFiles = root.getElementsByTagName("document");
		int numberOfFiles = recentFiles.getLength();
		
		// we remove all the duplicate
		for (int i = 0; i < recentFiles.getLength();  ++i) {
			Element style = (Element) recentFiles.item(i);
		
			
			if (filePath.equals(style.getAttribute("path"))) {
				root.removeChild((Node) style);
				numberOfFiles--;
			}
				
		}
		
		
		// if we have reached the maximun , we remove the oldest files
		while (recentFiles.getLength() >= MAX_RECENT_FILES) {
			root.removeChild(root.getFirstChild());
		}
			
		Element newFile =  document.createElement("document");
		
		newFile.setAttribute("path", filePath);
		
		root.appendChild((Node) newFile);
			
		/* Save changes */
		writeDocument();
			
				
		
	}
	
	
	
	/**
	 * Read the file to modify
	 */
	private static void readDocument() {
		
		File xml = null;
		DocumentBuilder docBuilder = null;
		
		try {
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			docBuilder = factory.newDocumentBuilder();
			
			// read content of a XML file with DOM
			xml = new File(USER_XPAD_CONFIG_FILE);
			document = docBuilder.parse(xml);
		
		} catch (ParserConfigurationException pce) {
			System.out.println(ERROR_READ + USER_XPAD_CONFIG_FILE);
		} catch (SAXException se) {
			System.out.println(ERROR_READ + USER_XPAD_CONFIG_FILE);
		} catch (IOException ioe) {
			System.out.println(ERROR_READ + USER_XPAD_CONFIG_FILE);
		}
		
	}
	/**
	 * Save the modifications
	 */
	private static void writeDocument() {
		
		Transformer transformer = null; 
		try {
			transformer = TransformerFactory.newInstance().newTransformer();
		} catch (TransformerConfigurationException e1) {
			System.out.println(ERROR_WRITE + USER_XPAD_CONFIG_FILE);
		} catch (TransformerFactoryConfigurationError e1) {
			System.out.println(ERROR_WRITE + USER_XPAD_CONFIG_FILE);
		}
		transformer.setOutputProperty(OutputKeys.INDENT, "yes");
		
		StreamResult result = new StreamResult(new File(USER_XPAD_CONFIG_FILE));
		DOMSource source = new DOMSource(document);
		try {
			transformer.transform(source, result);
		} catch (TransformerException e) {
			System.out.println(ERROR_WRITE + USER_XPAD_CONFIG_FILE);
		}
	
		
	}
	
}
