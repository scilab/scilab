package org.scilab.modules.xcos.utils;

import java.awt.Toolkit;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

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

/**
 * Implement configuration management for Xcos.
 */
public final class ConfigXcosManager {
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
     * Manage the configuration values for the MainWindow
     */
    private static final class MainWindowManagement {
    	
    	/** This class is a static singleton, thus it must not be instantiated */
        private MainWindowManagement() { }
    	
    /**
     * Get the position of XCOS Main Window
     * 
     * @return the position
     */
    public static Position getPosition() {
	/* Load file */
	IOManagement.readDocument();

	if (document != null) {
	    Element root = document.getDocumentElement();

	    NodeList profiles = root.getElementsByTagName(PROFILE);
	    Element xcosProfile = (Element) profiles.item(0);

	    NodeList allPositionElements = xcosProfile
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
    public static void savePosition(Position position) {
	/* Load file */
	IOManagement.readDocument();

	if (document != null) {
	    Element root = document.getDocumentElement();

	    NodeList profiles = root.getElementsByTagName(PROFILE);
	    Element xcosProfile = (Element) profiles.item(0);

	    NodeList allPositionElements = xcosProfile
		    .getElementsByTagName(MAINWINPOSITION);
	    Element mainWindowPosition = (Element) allPositionElements.item(0);

	    mainWindowPosition.setAttribute(XCOORD, Integer.toString(position
		    .getX()));
	    mainWindowPosition.setAttribute(YCOORD, Integer.toString(position
		    .getY()));

	    /* Save changes */
	    IOManagement.writeDocument();
	}
    }
    
    /**
     * Save the size of XCOS Main Window
     * 
     * @param size
     *            the size of XCOS main Window
     */
    public static void saveSize(Size size) {
	/* Load file */
	IOManagement.readDocument();

	if (document != null) {
	    Element root = document.getDocumentElement();

	    NodeList profiles = root.getElementsByTagName(PROFILE);
	    Element xcosProfile = (Element) profiles.item(0);

	    NodeList allPositionElements = xcosProfile
		    .getElementsByTagName(MAINWINSIZE);
	    Element mainWindowSize = (Element) allPositionElements.item(0);

	    mainWindowSize.setAttribute(WIDTH, Integer
		    .toString(size.getWidth()));
	    mainWindowSize.setAttribute(HEIGHT, Integer.toString(size
		    .getHeight()));

	    /* Save changes */
	    IOManagement.writeDocument();
	}
    }

    /**
     * Get the size of XCOS Main Window
     * 
     * @return the size
     */
    public static Size getSize() {

	/* Load file */
	IOManagement.readDocument();

	if (document != null) {

	    Element root = document.getDocumentElement();

	    NodeList profiles = root.getElementsByTagName(PROFILE);
	    Element xcosProfile = (Element) profiles.item(0);

	    NodeList allSizeElements = xcosProfile
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
    }
    
	/**
	 * Get the position of XCOS Main Window
	 * 
	 * @return the position
	 */
	public static Position getMainWindowPosition() {
		return MainWindowManagement.getPosition();
	}

	/**
	 * Save the position of XCOS Main Window
	 * 
	 * @param position
	 *            the position of XCOS main Window
	 */
	public static void saveMainWindowPosition(Position position) {
		MainWindowManagement.savePosition(position);
	}

	/**
	 * Get the size of XCOS Main Window
	 * 
	 * @return the size
	 */
	public static Size getMainWindowSize() {
		return MainWindowManagement.getSize();
	}

	/**
	 * Save the size of XCOS Main Window
	 * 
	 * @param size
	 *            the size of XCOS main Window
	 */
	public static void saveMainWindowSize(Size size) {
		MainWindowManagement.saveSize(size);
	}
    
    /**
     * Get all the recent opened files
     * 
     * @return a array of uri
     */
    public static List<File> getAllRecentOpenedFiles() {
	List<File> files = new ArrayList<File>();

	IOManagement.readDocument();

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
		IOManagement.writeDocument();
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

	Node root = XmlManagement.getXcosRoot();
	if (root == null) {
	    return;
	}
	
	Node recentFiles = XmlManagement.getNodeChild(root, RECENT_FILES);
	if (recentFiles == null) {
	    recentFiles = document.createElement(RECENT_FILES);
	    root.appendChild(recentFiles);
	}

	List<Node> recentFile = XmlManagement.getNodeChildren(recentFiles, DOCUMENT);

	//if file already in file no need to add it
	for (Node item : recentFile) {
	    if (filePath.compareTo(((Element) item).getAttribute(PATH)) == 0) {
		return;
	    }
	}

	//limit number of recent files
	if (recentFile.size() >= MAX_RECENT_FILES) {
	    int itemCount = recentFile.size() - (MAX_RECENT_FILES - 1);
	    for (int i = 0; i < itemCount; i++) {
		recentFiles.removeChild(recentFiles.getFirstChild());
	    }
	}

	Element newFile = document.createElement(DOCUMENT);

	newFile.setAttribute(PATH, filePath);

	recentFiles.appendChild((Node) newFile);

	/* Save changes */
	IOManagement.writeDocument();
    }

    /**
     * Implements the I/O specific operations
     */
    public static final class IOManagement {
    
    	/** This class is a static singleton, thus it must not be instantiated */
    	private IOManagement() { }
    	
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
    }

    /**
     * Add a file to recent Opened Files
     * 
     * @param filePath
     *            the path of the files to add
     */
    public static void saveUserDefinedPalettes(String filePath) {

	Node root = XmlManagement.getXcosRoot();
	if (root == null) {
	    return;
	}
	
	Node palettes = XmlManagement.getNodeChild(root, PALETTES);
	if (palettes == null) {
	    palettes = document.createElement(PALETTES);
	    root.appendChild(palettes);
	}

	List<Node> palette = XmlManagement.getNodeChildren(palettes, PALETTE);

	//if path already in file no need to add it
	for (Node item : palette) {
	    if (filePath.compareTo(((Element) item).getAttribute(PATH)) == 0) {
		return;
	    }
	}

	Element newFile = document.createElement(PALETTE);

	newFile.setAttribute(PATH, filePath);

	palettes.appendChild((Node) newFile);

	/* Save changes */
	IOManagement.writeDocument();
    }

    /**
     * Remove a previously saved user palette from the configuration.
     * @param filePath The file which contains the user palette.
     */
    public static void removeUserDefinedPalettes(String filePath) {

	Node root = XmlManagement.getXcosRoot();
	if (root == null) {
	    return;
	}
	
	Node palettes = XmlManagement.getNodeChild(root, PALETTES);
	List<Node> palette = XmlManagement.getNodeChildren(palettes, PALETTE);

	// remove node if exists
	for (Node file : palette) {
	    if (filePath.compareTo(((Element) file).getAttribute(PATH)) == 0) {
		palettes.removeChild(file);
		break;
	    }

	}
	/* Save changes */
	IOManagement.writeDocument();

    }

    /**
     * @return All the user palettes from the configuration
     */
    public static List<String> getUserDefinedPalettes() {
	List<String> files = new ArrayList<String>();

	Node root = XmlManagement.getXcosRoot();
	if (root == null) {
	    return files;
	}

	Node palettes = XmlManagement.getNodeChild(root, PALETTES);
	List<Node> palette = XmlManagement.getNodeChildren(palettes, PALETTE);
	for (Node file : palette) {
	    String path = ((Element) file).getAttribute(PATH);
	    if (path != null && path.compareTo("") != 0) {
		files.add(path);
	    }
	}
	
	return files;
    }
    
    /**
     * Implements the XML specific operations
     */
    private static final class XmlManagement { 
    
    	/** This class is a static singleton, thus it must not be instantiated */
        private XmlManagement() { }
    	
    /**
     * Implement a trivial DOM parser for getting a unique value.
     * @param parent The root node or null if for the whole document need to be parsed.
     * @param nodeName The node name to search for
     * @return The Node corresponding to the nodeName.
     */
    private static Node getNodeChild(Node parent, String nodeName) {
    	Node root = parent;
    	
	if (parent == null) {
	    if (document == null) {
		IOManagement.readDocument();
		if (document == null) {
		    return null;
		}
	    }
	    root = document;
	}

	Node currentNode = root.getFirstChild();
	while (currentNode != null) {
	    if (currentNode.getNodeName().compareTo(nodeName) == 0) {
		return currentNode;
	    }
	    currentNode = currentNode.getNextSibling();
	}
	return currentNode;
    }
    
    /**
     * Implement a trivial DOM parser for getting multiple values.
     * @param parent The root node or null if for the whole document need to be parsed.
     * @param childName The node name to search for
     * @return All the child nodes of the childName
     */
    private static List<Node> getNodeChildren(Node parent, String childName) {
	List<Node> nodes = new ArrayList<Node>();
	Node root = parent;
	
	if (parent == null) {
	    if (document == null) {
		IOManagement.readDocument();
		if (document == null) {
		    return nodes;
		}
	    }
	    root = document;
	}
	
	Node currentNode = root.getFirstChild();
	while (currentNode != null) {
	    if (currentNode.getNodeName().compareTo(childName) == 0) {
		nodes.add(currentNode);
	    }
	    currentNode = currentNode.getNextSibling();
	}
	return nodes;
	
    }
    
    /**
     * Get the root node for the xcos configuration.
     * @return The root Node of the Xcos configuration.
     */
    private static Node getXcosRoot() {

	if (document == null) {
	    IOManagement.readDocument();
	    if (document == null) {
		return null;
	    }
	}

	Node setting = getNodeChild(null, SETTING);
	
	if (setting != null) {
	    List<Node> nodes = getNodeChildren(setting, PROFILE);
	    for (Node node : nodes) {
		if (((Element) node).getAttribute(NAME).compareTo(XCOS) == 0) {
		    return node;
		}
	    }
	}
	return null;
    }
    }
}
