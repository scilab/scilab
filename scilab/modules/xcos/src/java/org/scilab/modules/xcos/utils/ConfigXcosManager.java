package org.scilab.modules.xcos.utils;

import java.awt.Toolkit;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;

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

import org.scilab.modules.console.GuiManagement;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

public class ConfigXcosManager {
    private static final int BUFSIZE = 1024;

    private static final int MARGIN = 20;

    private static final String ERROR_READ = "Could not load file: ";
    private static final String ERROR_WRITE = "Could not save file: ";
    private static final String VALUE = "value";
    private static final String WIDTH = "width";
    private static final String HEIGHT = "height";
    private static final String XCOORD = "x";
    private static final String YCOORD = "y";
    private static final String MAINWINPOSITION = "MainWindowPosition";
    private static final String MAINWINSIZE = "MainWindowSize";

    private static final String NAME = "name";

    private static final String PROFILE = "Profile";
    private static final String XCOS_CONFIG_FILE = System.getenv("SCI")
	    + "/modules/xcos/etc/xcosConfiguration.xml";

    private static final String USER_XCOS_CONFIG_FILE = GuiManagement
	    .getSCIHOME()
	    + "/xcosConfiguration.xml";
    private static final int DEFAULT_WIDTH = 650;
    private static final int DEFAULT_HEIGHT = 550;

    private static final int MAX_RECENT_FILES = 10;

    private static Document document;

    /**
     * Constructor
     */
    private ConfigXcosManager() {
	throw new UnsupportedOperationException();
    }

    /**
     * Create a copy of Scilab configuration file in the user directory
     */
    public static void createUserCopy() {
	File fileConfig = new File(USER_XCOS_CONFIG_FILE);
	if (!fileConfig.exists() || (fileConfig.length() == 0)) {
	    /* Create a local copy of the configuration file */
	    try {
		copyFile(new File(XCOS_CONFIG_FILE), new File(
			USER_XCOS_CONFIG_FILE));
	    } catch (IOException e) {
		System.err.println(ERROR_WRITE + USER_XCOS_CONFIG_FILE);
	    }

	}
    }

    /**
     * Get the name of the user configuration file
     * 
     * @return the name of the configuration file
     */
    public static String getUserConfigFile() {
	return USER_XCOS_CONFIG_FILE;
    }

    /**
     * Copy a file
     * 
     * @param in
     *            src file
     * @param out
     *            dest file
     * @throws IOException
     *             When an error occurs
     */
    private static void copyFile(File in, File out) throws IOException {
	FileInputStream fis = null;
	try {
	    fis = new FileInputStream(in);
	} catch (FileNotFoundException e) {
	    e.printStackTrace();
	}
	FileOutputStream fos = null;
	fos = new FileOutputStream(out);

	byte[] buf = new byte[BUFSIZE];
	int i = 0;
	while ((i = fis.read(buf)) != -1) {
	    fos.write(buf, 0, i);
	}
	fis.close();
	fos.close();
    }

    /**
     * Get the position of XCOS Main Window
     * 
     * @return the position
     */
    public static Position getMainWindowPosition() {
	/* Load file */
	readDocument();

	if (document != null) {
	    Element root = document.getDocumentElement();

	    NodeList profiles = root.getElementsByTagName(PROFILE);
	    Element XCOSProfile = (Element) profiles.item(0);

	    NodeList allPositionElements = XCOSProfile
		    .getElementsByTagName(MAINWINPOSITION);
	    Element mainWindowPosition = (Element) allPositionElements.item(0);
	    if (mainWindowPosition != null) {
		int x = Integer.parseInt(mainWindowPosition
			.getAttribute(XCOORD));
		int y = Integer.parseInt(mainWindowPosition
			.getAttribute(YCOORD));
		/* Avoid XCOS Main Window to be out of the screen */
		if (x <= (Toolkit.getDefaultToolkit().getScreenSize().width - MARGIN)
			&& y <= (Toolkit.getDefaultToolkit().getScreenSize().height - MARGIN)) {
		    return new Position(x, y);
		} else {
		    return new Position(0, 0);
		}
	    }
	}
	return new Position(0, 0);
    }

    /**
     * Save the position of XCOS Main Window
     * 
     * @param position
     *            the position of XCOS main Window
     */
    public static void saveMainWindowPosition(Position position) {
	/* Load file */
	readDocument();

	if (document != null) {
	    Element root = document.getDocumentElement();

	    NodeList profiles = root.getElementsByTagName(PROFILE);
	    Element XCOSProfile = (Element) profiles.item(0);

	    NodeList allPositionElements = XCOSProfile
		    .getElementsByTagName(MAINWINPOSITION);
	    Element mainWindowPosition = (Element) allPositionElements.item(0);

	    mainWindowPosition.setAttribute(XCOORD, Integer.toString(position
		    .getX()));
	    mainWindowPosition.setAttribute(YCOORD, Integer.toString(position
		    .getY()));

	    /* Save changes */
	    writeDocument();
	}
    }

    /**
     * Save the size of XCOS Main Window
     * 
     * @param size
     *            the size of XCOS main Window
     */
    public static void saveMainWindowSize(Size size) {
	/* Load file */
	readDocument();

	if (document != null) {
	    Element root = document.getDocumentElement();

	    NodeList profiles = root.getElementsByTagName(PROFILE);
	    Element XCOSProfile = (Element) profiles.item(0);

	    NodeList allPositionElements = XCOSProfile
		    .getElementsByTagName(MAINWINSIZE);
	    Element mainWindowSize = (Element) allPositionElements.item(0);

	    mainWindowSize.setAttribute(WIDTH, Integer
		    .toString(size.getWidth()));
	    mainWindowSize.setAttribute(HEIGHT, Integer.toString(size
		    .getHeight()));

	    /* Save changes */
	    writeDocument();
	}
    }

    /**
     * Get the size of XCOS Main Window
     * 
     * @return the size
     */
    public static Size getMainWindowSize() {

	/* Load file */
	readDocument();

	if (document != null) {

	    Element root = document.getDocumentElement();

	    NodeList profiles = root.getElementsByTagName(PROFILE);
	    Element XCOSProfile = (Element) profiles.item(0);

	    NodeList allSizeElements = XCOSProfile
		    .getElementsByTagName(MAINWINSIZE);
	    Element mainWindowSize = (Element) allSizeElements.item(0);
	    if (mainWindowSize != null) {
		return new Size(Integer.parseInt(mainWindowSize
			.getAttribute(WIDTH)), Integer.parseInt(mainWindowSize
			.getAttribute(HEIGHT)));
	    }
	}
	return new Size(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    }

    /**
     * Get all the recent opened files
     * 
     * @return a array of uri
     */

    public static ArrayList<File> getAllRecentOpenedFiles() {
	ArrayList<File> files = new ArrayList<File>();

	readDocument();

	if (document != null) {
	    Element root = (Element) document.getDocumentElement()
		    .getElementsByTagName("recentFiles").item(0);
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
	}
	return files;
    }

    /**
     * Add a file to recent Opened Files
     * 
     * @param filePath
     *            the path of the files to add
     */
    public static void saveToRecentOpenedFiles(String filePath) {

	readDocument();

	if (document != null) {
	    Element root = (Element) document.getDocumentElement()
		    .getElementsByTagName("recentFiles").item(0);
	    NodeList recentFiles = root.getElementsByTagName("document");
	    int numberOfFiles = recentFiles.getLength();

	    // we remove all the duplicate
	    for (int i = 0; i < recentFiles.getLength(); ++i) {
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

	    Element newFile = document.createElement("document");

	    newFile.setAttribute("path", filePath);

	    root.appendChild((Node) newFile);

	    /* Save changes */
	    writeDocument();
	}

    }

    /**
     * Read the file to modify
     */
    private static void readDocument() {

	File xml = null;
	DocumentBuilder docBuilder = null;

	try {
	    DocumentBuilderFactory factory = DocumentBuilderFactory
		    .newInstance();
	    docBuilder = factory.newDocumentBuilder();

	    // read content of a XML file with DOM
	    xml = new File(USER_XCOS_CONFIG_FILE);
	    document = docBuilder.parse(xml);

	} catch (ParserConfigurationException pce) {
	    System.err.println(ERROR_READ + USER_XCOS_CONFIG_FILE);
	} catch (SAXException se) {
	    System.err.println(ERROR_READ + USER_XCOS_CONFIG_FILE);
	} catch (IOException ioe) {
	    System.err.println(ERROR_READ + USER_XCOS_CONFIG_FILE);
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
	    System.err.println(ERROR_WRITE + USER_XCOS_CONFIG_FILE);
	} catch (TransformerFactoryConfigurationError e1) {
	    System.err.println(ERROR_WRITE + USER_XCOS_CONFIG_FILE);
	}
	transformer.setOutputProperty(OutputKeys.INDENT, "yes");

	StreamResult result = new StreamResult(new File(USER_XCOS_CONFIG_FILE));
	DOMSource source = new DOMSource(document);
	try {
	    transformer.transform(source, result);
	} catch (TransformerException e) {
	    System.err.println(ERROR_WRITE + USER_XCOS_CONFIG_FILE);
	}

    }

}
