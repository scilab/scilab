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
    private static final String WIDTH = "width";
    private static final String HEIGHT = "height";
    private static final String XCOORD = "x";
    private static final String YCOORD = "y";
    private static final String MAINWINPOSITION = "MainWindowPosition";
    private static final String MAINWINSIZE = "MainWindowSize";

    private static final String RECENT_FILES = "recentFiles";
    private static final String DOCUMENT = "document";
    private static final String PATH = "path";
    
    
    private static final String PALETTES = "palettes";
    private static final String PALETTE = "palette";

    
    private static final String NAME = "name";
    private static final String XCOS = "xcos";
    private static final String SETTING = "Setting";
    
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
    private static void createUserCopy() {
    	File fileConfig = new File(USER_XCOS_CONFIG_FILE);
    	if (!fileConfig.exists() || (fileConfig.length() == 0)) {
    		/* Create a local copy of the configuration file */
    		try {
    			copyFile(new File(XCOS_CONFIG_FILE), new File(USER_XCOS_CONFIG_FILE));
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
		    .getElementsByTagName(RECENT_FILES).item(0);
	    if (root != null) {
		NodeList recentFiles = root.getElementsByTagName(DOCUMENT);

		for (int i = 0; i < recentFiles.getLength(); ++i) {
		    Element style = (Element) recentFiles.item(i);

		    File temp = new File(style.getAttribute(PATH));

		    if (temp.exists()) {
			files.add(0, temp);
		    } else {
			root.removeChild((Node) style);
		    }

		}
		/* Save changes */
		writeDocument();
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

	Node root = getXcosRoot();
	if(root == null) {
	    return;
	}
	
	Node recentFiles = getNodeChild(root, RECENT_FILES);
	if(recentFiles == null) {
	    recentFiles = document.createElement(RECENT_FILES);
	    root.appendChild(recentFiles);
	}

	ArrayList<Node> recentFile = getNodeChildren(recentFiles, DOCUMENT);

	//if file already in file no need to add it
	for(Node item : recentFile) {
	    if (filePath.compareTo(((Element)item).getAttribute(PATH)) == 0) {
		return;
	    }
	}

	//limit number of recent files
	if(recentFile.size() >= MAX_RECENT_FILES) {
	    int itemCount = recentFile.size() - (MAX_RECENT_FILES - 1);
	    for(int i = 0 ; i < itemCount ; i++) {
		recentFiles.removeChild(recentFiles.getFirstChild());
	    }
	}

	Element newFile = document.createElement(DOCUMENT);

	newFile.setAttribute(PATH, filePath);

	recentFiles.appendChild((Node) newFile);

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
	    DocumentBuilderFactory factory = DocumentBuilderFactory
		    .newInstance();
	    docBuilder = factory.newDocumentBuilder();

	    createUserCopy();
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

    /**
     * Add a file to recent Opened Files
     * 
     * @param filePath
     *            the path of the files to add
     */
    public static void saveUserDefinedPalettes(String filePath) {

	Node root = getXcosRoot();
	if(root == null) {
	    return;
	}
	
	Node palettes = getNodeChild(root, PALETTES);
	if(palettes == null) {
	    palettes = document.createElement(PALETTES);
	    root.appendChild(palettes);
	}

	ArrayList<Node> palette = getNodeChildren(palettes, PALETTE);

	//if path already in file no need to add it
	for(Node item : palette) {
	    if (filePath.compareTo(((Element)item).getAttribute(PATH)) == 0) {
		return;
	    }
	}

	Element newFile = document.createElement(PALETTE);

	newFile.setAttribute(PATH, filePath);

	palettes.appendChild((Node) newFile);

	/* Save changes */
	writeDocument();
    }

    public static void removeUserDefinedPalettes(String filePath) {

	Node root = getXcosRoot();
	if(root == null) {
	    return;
	}
	
	Node palettes = getNodeChild(root, PALETTES);
	ArrayList<Node> palette = getNodeChildren(palettes, PALETTE);

	// remove node if exists
	for(Node file : palette) {
	    if (filePath.compareTo(((Element)file).getAttribute(PATH)) == 0) {
		palettes.removeChild(file);
		break;
	    }

	}
	/* Save changes */
	writeDocument();

    }

    public static ArrayList<String> getUserDefinedPalettes() {
	ArrayList<String> files = new ArrayList<String>();

	Node root = getXcosRoot();
	if(root == null) {
	    return files;
	}

	Node palettes = getNodeChild(root, PALETTES);
	ArrayList<Node> palette = getNodeChildren(palettes, PALETTE);
	for(Node file : palette) {
	    String path = ((Element)file).getAttribute(PATH);
	    if(path != null && path.compareTo("") != 0) {
		files.add(path);
	    }
	}
	
	return files;
    }
    
    private static Node getNodeChild(Node parent, String nodeName) {
	
	if(parent == null) {
	    if(document == null) {
		readDocument();
		if(document == null) {
		    return null;
		}
	    }
	    parent = document;
	}

	Node currentNode = parent.getFirstChild();
	while(currentNode != null) {
	    if(currentNode.getNodeName().compareTo(nodeName) == 0){
		return currentNode;
	    }
	    currentNode = currentNode.getNextSibling();
	}
	return currentNode;
    }
    
    private static ArrayList<Node> getNodeChildren(Node parent, String childName) {
	ArrayList<Node> nodes = new ArrayList<Node>();
	
	if(parent == null) {
	    if(document == null) {
		readDocument();
		if(document == null) {
		    return nodes;
		}
	    }
	    parent = document;
	}
	
	Node currentNode = parent.getFirstChild();
	while(currentNode != null) {
	    if(currentNode.getNodeName().compareTo(childName) == 0){
		nodes.add(currentNode);
	    }
	    currentNode = currentNode.getNextSibling();
	}
	return nodes;
	
    }
    
    private static Node getXcosRoot() {

	if(document == null) {
	    readDocument();
	    if(document == null) {
		return null;
	    }
	}

	Node setting = getNodeChild(null, SETTING);
	
	if(setting != null) {
	    ArrayList<Node> nodes = getNodeChildren(setting, PROFILE);
	    for(Node node : nodes) {
		if(((Element)node).getAttribute(NAME).compareTo(XCOS) == 0) {
		    return node;
		}
	    }
	}
	return null;
    }
}