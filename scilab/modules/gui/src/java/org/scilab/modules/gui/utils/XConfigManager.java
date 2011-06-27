/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - INRIA - Pierre GRADIT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import java.awt.Color;
import java.awt.Container;
import java.awt.Component;
import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.Frame;
import java.awt.Font;
import java.awt.Toolkit;
import java.awt.event.MouseEvent;
import java.awt.event.ActionEvent;
import java.awt.event.MouseListener;
import java.awt.event.ActionListener;
import java.awt.Dimension;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.StringWriter;
import java.net.URL;
import java.util.Hashtable;
import java.util.Arrays;
import java.util.EventObject;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Constructor;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactoryConfigurationError;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.dom.DOMResult;
import javax.xml.transform.stream.StreamSource;
import javax.xml.transform.stream.StreamResult;

import org.scilab.modules.commons.ScilabCommons;
import org.scilab.modules.commons.xml.ScilabDocumentBuilderFactory;
import org.scilab.modules.commons.xml.ScilabTransformerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.w3c.dom.NamedNodeMap;
import org.xml.sax.SAXException;

import org.scilab.modules.gui.console.ScilabConsole;
import javax.swing.JDialog;
import javax.swing.JTextArea;
import javax.swing.JScrollPane;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JComponent;
import javax.swing.AbstractButton;
import javax.swing.JComboBox;
import javax.swing.Box;
import javax.swing.text.html.StyleSheet;
import javax.swing.BorderFactory;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;

import org.scilab.modules.gui.utils.Component.SCROLL;

/**
 * Extended management of the Console configuration file
 * @author Pierre Gradit
 */

/* 
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    VIEW HIERARCHY       CONTROL SYSTEM      MODEL HIERARCHY
                                          .peer
   	Component ------------>  XSentinel -----------> Node
                Hashtable
                    =
      XUpdateVisitor.correspondance

	XConfigManager runs XUpdateVisitor.visit(_,_) 
		that initiates correspondance.
	while True:
		XSentinel receives events.
		XConfigManager may runs XUpdateVisitor.visit(_,_) 
			when correspondance needs to be refreshed.

	To avoid interface blinking, the set of add/remove is minimized, 
	   and the number of "actuations" (i.e. affectations) maximized.

   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/

class XSentinel implements MouseListener, ActionListener {
	/**
	  *    XSentinel is a class associated through a HashTable in 1-1 
	  *    correspondance with view component, this correspondance
 	  *    maintained after each observable event by XUpdateVisitor.
	  *
	  *    XSentinel has two functions :
	  *    - Checks if a given node is reachability through actuators
	  *        from the former one.
	  *    - Interprets incoming events and transmit corresponding  
	  *        action to XConfigManager.
	  *
      **/

	protected Node      peer;
	protected String [] actuators;
	public    String    reduced = null;

	public XSentinel(Component component, Node node) {
		peer = node;
		if (component instanceof XComponent) { 
			XComponent  xComponent = (XComponent) component;
			actuators = xComponent.actuators();
		} else {
			actuators = new String[0];
		}	
	}

	public static String signature(Node node, String [] actuators) {
		/**
		  *   Provide a signature string by reduction verifying that
		  *		string equality implies reachability through actuators.
		  *
		  *     i.e. node peer component can be updated
		  *     i.e. node peer component should not be deleted 
		  *
		  **/
		String signature        = "";
		if (!node.hasAttributes()) return "";
		NamedNodeMap attributes = node.getAttributes();
		int size                = attributes.getLength();
		String [] keys          = new String[size];

		for (int i=0; i< size; i++) {
			Node item = attributes.item(i);
			keys[i]   = item.getNodeName();
		}
		Arrays.sort(keys);
			/* as DOM attributes are not ordered, 
			   sort is needed to acheive proper reduction.
			*/
		signature += node.getNodeName();
		for (int i=0; i< size; i++) {
			String attrName = keys[i];
			if (Arrays.binarySearch(actuators, attrName)>=0) {
				signature   += " " + attrName;
			} else {
				Node item    = attributes.getNamedItem(attrName);
				String value = item.getNodeValue().replaceAll("[ \t\n]+"," ");
				signature   += " " + attrName + "='" + value + "'";				
			}
		}
		return signature;
	}	

	public boolean checks(Node next) {
		String             checker  = signature(next, actuators);
		if (reduced==null) reduced  = signature(peer, actuators);
		return reduced.equals(checker);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	void triggerEventNode(EventObject e, Node peer, String type) {
		/**
		  *  returns the first event node of given type.
		  **/
		NodeList nodelist = peer.getChildNodes();
		for (int i = 0; i < nodelist.getLength(); i++) {
			Node node = nodelist.item(i);
			if (node.getNodeName().equals(type))
				XConfigManager.xEvent(node, (Component) e.getSource());
				return;
		}
		return;//TODO: Exception or authorize multiple actions ?
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	//     M O U S E   L I S T E N E R
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	public void mouseClicked(MouseEvent e) {
		//System.err.println(e);
		triggerEventNode(e, peer, "mouseClicked");
	}
	public void mouseEntered(MouseEvent e) {}
	public void mouseExited(MouseEvent e) {}
	public void mousePressed(MouseEvent e) {}
	public void mouseReleased(MouseEvent e) {}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	//     A C T I O N   L I S T E N E R
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	public void actionPerformed(ActionEvent e) {
		//System.err.println(e);
		triggerEventNode(e,peer, "actionPerformed");
	}
}

class XUpdateVisitor {
    /**
      *   XUpdateVisitor unfolds update after each generation of model nodes.
      *
      *   At the end of the update, view and dom are in 1-1 correspondance,
      *     this correspondance is memorized in an hashtable storing one 
	  *     XSentinel par 1-1 edge.
      *
      *   This update recursively :
      *       - adds/deletes to reach tree structure correspondance
      *       - run the update method to reach attribute correspondance 
      *
      **/
	Hashtable correspondance;

	public XUpdateVisitor(Hashtable table) {
		// Table initialized by manager with dialog content panel
		//                   associated with document node.
		correspondance = table;
	}

	public Component build(Container view, Node peer, Node item, int index) {
		Component component  = buildPeerFor(item);
		Object constraints   = getLayoutConstraints(peer, item); 
		if (index>-1) {
			view.add(component, constraints, index);
		} else {
			view.add(component, constraints);
		}	
		return component;
	}

	public void forget(Container view, Component component) {
		/* removal alert
			if (!(sentinel==null) && !sentinel.checks(item)) 
				System.err.println("Replace:" +	sentinel.reduced 
				 +" by "+ XSentinel.signature(item, new String[0]));
		*/
		view.remove(component);
		correspondance.remove(component);
	}

	public void visit(Container view, Node peer) {
		/**
		  *  Computes a simple diff on tree structure.
		  *
		  **/
		int allIndex     = 0;
		int visibleIndex = 0;
		NodeList nodes = peer.getChildNodes();
		Component component;
		XSentinel sentinel;
		while (allIndex < nodes.getLength()) {
			Node item = nodes.item(allIndex);
			if (isVisible(item)) { 
				if (visibleIndex < view.getComponentCount()) {
					component = view.getComponent(visibleIndex);
					sentinel  = (XSentinel) correspondance.get(component);
					if (sentinel==null || !sentinel.checks(item)) {
						forget(view, component);
						component = build(view, peer, item, visibleIndex);
					}
				} else {
					component = build(view, peer, item, -1);
				}
				if (component instanceof Container) {
					// Rebuild container children.
					Container container = (Container) component;
					visit(container, item);
				} 
				visibleIndex += 1;
			} 
			allIndex += 1;
		}
		while (visibleIndex < view.getComponentCount()) {		
			component = view.getComponent(visibleIndex);			
			forget(view, component);		}			
		if (view instanceof XComponent) { 
			// Attribute correspondance once children rebuilt.
			XComponent xView = (XComponent) view;
			xView.refresh(peer);
			// Sentinel sets watch.
			sentinel   = new XSentinel(view, peer);
			correspondance.put(view, sentinel);
			addListeners(view, peer, sentinel);
		}
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	Object getLayoutConstraints(Node parent, Node current) {
		if (XConfigManager.getAttribute(parent, "layout").equals("border"))
			return XConfigManager.getAttribute(current, "border-side");
		return null;
		} 

	public boolean isVisible(Node node) {
		// a. Event nodes are invisibles.
		if (node.getNodeName().equals("mouseClicked"))    return false;
		if (node.getNodeName().equals("actionPerformed")) return false;

		// b. Text nodes with only invisible characters are invisible.
		if (node.getNodeName().equals("#text"))
			if (node.getNodeValue().replaceAll("^[ \t\n]+$","").equals(""))
				return false;

		// c. Chooser options are invisible.
		if (node.getNodeName().equals("option"))          return false;

		return true;


	}

	public void addListeners(Component component, Node node, XSentinel sentinel) {
		String listener = XConfigManager.getAttribute(node, "listener");
		if (listener.equals("MouseListener")) {
			 component.addMouseListener(sentinel);
		}
		if (listener.equals("ActionListener")) {
			if (component instanceof AbstractButton) {
				AbstractButton button = (AbstractButton) component;
				button.addActionListener(sentinel);
			}
			if (component instanceof XChooser) {
				XChooser chooser = (XChooser) component;
				chooser.addActionListener(sentinel);
			}

		}
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	static String xPackageName = "org.scilab.modules.gui.utils.Component.";
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	public Component buildPeerFor(Node node) {
		//System.err.println( "buildPeerFor:" + node);

		String tag = node.getNodeName();

		// Specific treatment for scrolls...
		if (tag.equals("SCROLL")) {
			JPanel container = new JPanel();
			return new SCROLL(node, container);
		}

		//1. Find the class with the same name.
		Class componentClass;
		try {
			componentClass    = Class.forName(xPackageName + tag);
		} catch (ClassNotFoundException e) {
			// Some classes are made directly
			//  - here labels for text node.
			if (tag.equals("#text")) {
				String value = node.getNodeValue();
				return new JLabel(value);
			}
			// - here boxes.
			if (tag.equals("HBOX")) {
				Box hbox = Box.createHorizontalBox();
				XConfigManager.drawConstructionBorders(hbox);
				XConfigManager.setDimension(hbox, node);
				return hbox;
			}
			if (tag.equals("VBOX")) {
				Box vbox          = Box.createVerticalBox();
				String background = XConfigManager.getAttribute(node, "background");
				if (!(background.equals(XConfigManager.NAV))) {
					Color color = XConfigManager.getColor(background);
					vbox.setOpaque(true);
					vbox.setBackground(color);
				}
				XConfigManager.drawConstructionBorders(vbox);
				XConfigManager.setDimension(vbox, node);
				return vbox;
			}
			if (tag.equals("VSPACE")) {
				int height = XConfigManager.getInt(node, "height", 5);
				return Box.createVerticalStrut(height);
			}
			if (tag.equals("HSPACE")) {
				int width = XConfigManager.getInt(node, "width", 5);
				return Box.createHorizontalStrut(width);
			}
			if (tag.equals("GLUE")) 
				return Box.createGlue();
			if (tag.equals("VGLUE")) 
				return Box.createVerticalGlue();

			// Declare failure due to class absence
			// System.err.println( "ClassNotFoundException:" + e);			
			return new XStub(tag, "ClassNotFoundException");
		}

		//2. Find the constructor.
		Constructor constructor;
		try {
				// First with a Node,
				constructor = componentClass.getConstructor(new Class[]{Node.class});
		} catch (NoSuchMethodException e) {
			try {
				// then without anything.
				constructor = componentClass.getConstructor(new Class[]{});
			} catch (NoSuchMethodException f) {
				// Declare failure due to constructor absence
				System.err.println( "NoSuchMethodException:" + f);			
				return new XStub(tag, "NoSuchMethodException");
			}
		} catch (SecurityException e) {
			// Declare failure due to constructor rights (it must be public)
			System.err.println( "SecurityException:" + e);			
			return new XStub(tag, "SecurityException");
		}

		//3. Invoke the constructor.
		Component component;
		try {		
			component = (Component) constructor.newInstance(new Object[]{node});
		} catch (InstantiationException e) {
			System.err.println( "InstantiationException:" + e);			
			return new XStub(tag, "InstantiationException");
		} catch (IllegalAccessException e) {
			System.err.println( "IllegalAccessException:" + e);			
			return new XStub(tag, "IllegalAccessException");
		} catch (IllegalArgumentException e) {
			System.err.println( "IllegalArgumentException:" + e);			
			return new XStub(tag, "IllegalArgumentException");
		} catch (InvocationTargetException e) {
			System.err.println( "InvocationTargetException:" + e);			
			return new XStub(tag, "InvocationTargetException");
		}
		return component;
	}
}

class XStub extends JPanel {
	/**
	  *    XStub indicates how it goes wrong in buildPeerFor(Node).
      **/

	public XStub(String tag, String cause) {
		super();
		Border       black  = BorderFactory.createLineBorder(Color.blue);
		TitledBorder title  = BorderFactory.createTitledBorder(black, tag);
		Dimension dimension = new Dimension(100, 50);
		setPreferredSize(dimension);
		setOpaque(false);

		title.setTitleColor(Color.blue);
		setBorder(title);
		setLayout(new FlowLayout());
		add(new JLabel(cause));
	}

	public String toString() {
		return "STUB";
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public final class XConfigManager /**extends ConfigManager **/ {

    private static final int BUFSIZE = 1024;

    private static final int MARGIN = 20;

    private static final String ERROR_READ = "Could not load file: ";
    private static final String ERROR_WRITE = "Could not save file: ";
    private static final String VERSION = "version";
    private static final String VALUE = "value";
    private static final String WIDTH = "width";
    private static final String HEIGHT = "height";
    private static final String XCOORD = "x";
    private static final String YCOORD = "y";
    private static final String MAINWINPOSITION = "MainWindowPosition";
    private static final String MAINWINSIZE = "MainWindowSize";
    private static final String HELPWINPOSITION = "HelpWindowPosition";
    private static final String HELPWINSIZE = "HelpWindowSize";
    private static final String HELPFONTSIZE = "HelpFontSize";
    private static final String PROFILE = "Profile";
    private static final String FOREGROUNDCOLOR = "ForegroundColor";
    private static final String BACKGROUNDCOLOR = "BackgroundColor";
    private static final String COLORPREFIX = "#";
    private static final String MAXOUTPUTSIZE = "MaxOutputSize";
    private static final String LASTOPENEDDIR = "LastOpenedDirectory";

    private static final int DEFAULT_WIDTH = 650;
    private static final int DEFAULT_HEIGHT = 550;

    private static final int DEFAULT_MAXOUTPUTSIZE = 10000;
    private static final int DEFAULT_HELPFONTSIZE = 2;

    /** -----------------------------------------------------------------------------------*/

    private static final   String   SCILAB_CONFIG_FILE = System.getenv("SCI") + "/modules/console/etc/XConfiguration.xml";
    private static final   String   SCILAB_CONFIG_XSL  = System.getenv("SCI") + "/modules/gui/src/xslt/XConfiguration.xsl";

    private static final   String   USER_CONFIG_FILE   = ScilabCommons.getSCIHOME() + "/XConfiguration.xml";

    protected static       JDialog  dialog;
    private static         Document document;
    private static         boolean  updated            = false;

    /** -----------------------------------------------------------------------------------
      *
      *   How to display the view object model (V-DOM) ?
      *
     \*/

    static Node           topDOM;
    static Container      topSwing;
    static Hashtable      correspondance;
	static XUpdateVisitor visitor;

	public static void displayAndWait() {

		reloadTransformer();

		// Set up Swing Side
		dialog          = new JDialog(getTopLevel(), "Scilab Preferences",true);
		topSwing        = dialog.getContentPane();		
		topSwing.setLayout(new BorderLayout());// AWT implies to set layout at construction time.

		// Set up DOM Side
		readDocument();
		updated = false;

		// Set up correspondance
		correspondance = new Hashtable();

		// Let the show begin!
		if (refreshDisplay())
			dialog.setVisible(true);
    }

	public static boolean refreshDisplay() {

		// Generate new view DOM.
		topDOM = generateViewDOM().getNode().getFirstChild();
		if (topDOM == null) {
			System.err.println("XSL does not give a node!");
			return false; 
		}

		// Refresh correspondance
		//    TODO: top layout changes
		correspondance.put(topSwing, topDOM); 
		visitor = new XUpdateVisitor(correspondance);
		visitor.visit(topSwing, topDOM);
		dialog.pack();

		// Refresh contextual help
		if (!(help==null)) {
			help();
		} else {
			//System.err.println(ViewDOM(document.getDocumentElement(),""));
			//System.err.println(ViewDOM());
			//System.err.println(SwingComposite());			
		}
		System.err.println("Refresh!");
		return true;
	}

    /** -----------------------------------------------------------------------------------
      *
      *   How to display the help ?
      *
     \*/

	static JDialog  help = null;
	static JTextArea xml = null;
    public static void help() {
		/**
		  *  Help is meant for developper so far, 
		  *    it displays an XML control string.
		  **/
		if (help==null) {
			JScrollPane scroll = new JScrollPane();
		    help               = new JDialog(getTopLevel(), "XML view", false);
			xml                = new JTextArea(30,30);
			scroll.add(xml);
			help.add(scroll);
			help.setVisible(true);
		}
		String  view       = ViewDOM();
		xml.setText(view);
    }

    /** -----------------------------------------------------------------------------------
      *
      *   How to produce contextual help for designers?
	  *
      *        It should display the View DOM and the Swing composite
	  *	       on single scrolled pair of textarea to manifest correspondance
	  *
	  *        ViewDOM and SwingComposite code are intentionally stricly homologous.
	  *
	  *        TODO: invisible View nodes used for control shall be replaced
	  *				 by blank lines in Swing composite
	  *
     \*/

	public static final String Increment = "    ";

    private static String ViewDOM(Node node, String indent) {
		String signature = indent;
		if (node.hasAttributes()) {
			signature += XSentinel.signature(node, new String[0]);
		} else {
			signature += (node.getNodeName() + ": " + node.getNodeValue()).replaceAll("[ \t\n]+"," ");
		}
		signature   += "\n";
		NodeList nodelist = node.getChildNodes();
		if (true) {
			for (int i=0; i< nodelist.getLength(); i++) {
				Node item  = nodelist.item(i); 
				signature += ViewDOM(item, indent+Increment);
			}
		}
		return signature;
	}
    private static String ViewDOM() {
		return ViewDOM(topDOM, "");
    }

    private static String SwingComposite(Component component, String indent) {
		String signature = indent;
		if (true) {
			signature       += component.toString();
		}
		signature       += "\n";
		if (component instanceof Container) {
			Container container = (Container) component;
			Component [] components = container.getComponents();
			for (int i=0; i < components.length; i++) {
				Component child = components[i];
				signature      += SwingComposite(child, indent+Increment);		
			}
		}
		return signature;
	}
    private static String SwingComposite() {
		return SwingComposite(topSwing, "");
    }
				 
    /** -----------------------------------------------------------------------------------
      *
      *   How to produce the view object model (V-DOM) ?
      *
     \*/

    private static TransformerFactory     factory  = ScilabTransformerFactory.newInstance();
    private static DocumentBuilderFactory builder  = ScilabDocumentBuilderFactory.newInstance();

	static Transformer transformer = null;

    private static void reloadTransformer() {
        try {
            StreamSource source = new StreamSource(SCILAB_CONFIG_XSL);
            transformer         = factory.newTransformer(source);
        } catch (TransformerConfigurationException e1) {
            System.out.println(ERROR_READ + SCILAB_CONFIG_XSL);
        } catch (TransformerFactoryConfigurationError e1) {
            System.out.println(ERROR_READ + SCILAB_CONFIG_XSL);
        }
		System.out.println("XSL reloaded!");
	}

    private static DOMResult generateViewDOM() {
        DOMResult result    = new DOMResult();
        DOMSource source    = new DOMSource(document);
        try {
            transformer.transform(source, result);
        } catch (TransformerException e) {
            System.out.println(ERROR_WRITE + USER_CONFIG_FILE);
        }
		return result;
    }

    /** -----------------------------------------------------------------------------------
      *
      *   How to identify an element with its context string ?
      *
      *
     \*/

	public static Element getElementByContext(String context) {
		String [] id = context.split("#");
		NodeList elements = document.getElementsByTagName(id[0]);
		if (elements.getLength()==1)
			return (Element) elements.item(0); 
		if (elements.getLength()==0) {
			System.err.println("'" + context + "' has no image in document!");
			return null;
			}
		if (id.length == 1) {
			System.err.println("'" + context + "' has not a unique response (use unique-id template)!");
			return null;
			}
		Integer integer = Integer.parseInt(id[1]);
		int occurence = integer.intValue();
		if (elements.getLength() <= occurence) {
			System.err.println("'" + context + "' has no image in document!");
			return null;
			}
		return (Element) elements.item(occurence);
	}

    /** -----------------------------------------------------------------------------------
      *
      *   How to manage incoming xEvent from sentinels ?
      *
      *
     \*/

	public static void xEvent(Node action, Component source) {

		if (!getAttribute(action,"set").equals(NAV)) {
			String context   = getAttribute(action,"context");
			Element element  = getElementByContext(context);

			String value     = getAttribute(action,"value");
			String attribute = getAttribute(action,"set");
			if (! (element==null)) {
				//System.err.println(context + "." + element + "@" + attribute + "= " + value);
				element.setAttribute(attribute, value);
			}
			refreshDisplay();
			updated = true;
			return;
		}

		if (!getAttribute(action,"choose").equals(NAV)) {
			String context   = getAttribute(action,"context");
			Element element  = getElementByContext(context);

			if (source instanceof XChooser) {
				XChooser chooser   = (XChooser) source;
				String   value     = chooser.choose();
				String   attribute = getAttribute(action,"choose");
				if (! (element==null)) {
					//System.err.println(context + "." + element + "@" + attribute + "= " + value);
					element.setAttribute(attribute, value);
				}
				refreshDisplay();
				updated = true;//TODO: not always real modification...
			} else {
				System.err.println("Choose attribute only valid on choosers (SELECT, COLOR, FILE, ENTRY,...)");
			}
			return;
		}

		String callback = getAttribute(action,"callback");
		if (callback.equals("Help")) {
			// TODO: it can be a contextual help.
			System.err.println("Help not implemented yet!");
			return;
		}
		if (callback.equals("Ok")) {
			writeDocument();
			dialog.dispose(); 
            updated = false;
			return;
		}
		if (callback.equals("Apply")) {
			System.err.println("User XML saved!");
            updated = false;
			writeDocument();
			return;
		}
		if (callback.equals("Default")) {
			System.out.println("Scilab XML reloaded!");
			reloadTransformer();
			refreshUserCopy();
			readDocument();
            updated = false;
			refreshDisplay();
			return;
		}
		if (callback.equals("Cancel")) {
			System.err.println("User XML reloaded!");
			readDocument();
			if (updated) {
				//TODO: advertise it!
				}
            updated = false;
			refreshDisplay();
			return;
		}
	}

    /** -----------------------------------------------------------------------------------
     *
     * Manage attribute consultation
     */

	public static final String NAV = "\"not an value'";

	public static final String getAttribute(Node node, String name, String value) {
		String response = getAttribute(node, name);
		if (response==NAV) return value;
		return response;
	}

	public static final String getAttribute(Node node, String name) {
		NamedNodeMap attrs = node.getAttributes();
		if (attrs==null)   return NAV;
		Node attr = attrs.getNamedItem(name);
		if (attr==null)	   return NAV;
		return attr.getNodeValue();
	}

	public static final int getInt(Node node, String name, int value) {
		String response = getAttribute(node, name);
		if (response==NAV) return value;
		Integer integer = Integer.parseInt(response);
		return integer.intValue();
	}

    /** -----------------------------------------------------------------------------------
     *
     * Manage [0-9a-f]{6} color representation 
     */

	public static final String getColor(Color source) {
		return Integer.toHexString(source.getRGB());
	}

	public static final Color getColor(String source) {
		return Color.decode(source);
	}

  /** -----------------------------------------------------------------------------------
      *
      *   TODO: How to impact modification of preferences ?
      *
     \*/
    public static void subscribeUpdate() {
    }

    public static void notifyUpdate() {
    }

    /** -----------------------------------------------------------------------------------
      *
      *   How to get top-level window identifier to open pop-up ?
      *
     \*/

    private static Frame getTopLevel() {
	Container main = (Container) ScilabConsole.getConsole().getAsSimpleConsole();
	return (Frame)main.getParent().getParent().getParent().getParent().getParent().getParent();
    }

 
    /** -----------------------------------------------------------------------------------
     *
     * Create a copy of Scilab configuration file in the user directory
     */
    public static void createUserCopy() {
        File fileConfig = new File(USER_CONFIG_FILE);
        if (!fileConfig.exists()/*|| (fileConfig.length() == 0) || checkVersion()*/) {
			refreshUserCopy();
        }
    }

   /**
     * Refresh configuration file in the user directory with Scilab defaults
     */
    public static void refreshUserCopy() {
            /* Create a local copy of the configuration file */
            try {
                copyFile(new File(SCILAB_CONFIG_FILE), new File(USER_CONFIG_FILE));
            } catch (FileNotFoundException e) {
                System.out.println(ERROR_READ + USER_CONFIG_FILE);
            }
    }

   /**
     * draw construction borders for layout debug
     */

	public static void drawConstructionBorders(JComponent component) {
		if (false) {
			Border       construction = BorderFactory.createLineBorder(Color.red);
			component.setBorder(construction);	
		}
	}
	
	public static void setDimension(JComponent component, Node peer) {
		int      height     = XConfigManager.getInt(peer , "height", 0);
		int       width     = XConfigManager.getInt(peer , "width",  0);
		if (height>0 && width>0) {
			//System.err.println("Dimension: " + width + "x" + height);
			Dimension dimension = new Dimension(width, height);
			component.setPreferredSize(dimension);
		}
	}

    /** =================================================================================== **/

    /** =================================================================================== **/

    /** =================================================================================== **/
    /**
     * Constructor blocked, singleton pattern.
     */
    private XConfigManager() {
        throw new UnsupportedOperationException();
    }

    /**
     * Copy a file
     * @param in src file
     * @param out dest file
     * @throws FileNotFoundException
     */
    private static void copyFile(File in, File out) throws FileNotFoundException {
        FileInputStream fis = new FileInputStream(in);
        FileOutputStream fos = new FileOutputStream(out);;

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
    }


    /**
     * Read the file to modify
     */
    private static Document readDocument(String fileName) {
        File xml = null;
        DocumentBuilder docBuilder = null;

        try {
            DocumentBuilderFactory factory = ScilabDocumentBuilderFactory.newInstance();
            docBuilder = factory.newDocumentBuilder();

            // lecture du contenu d'un fichier XML avec DOM
            xml = new File(fileName);
            return docBuilder.parse(xml);
        } catch (ParserConfigurationException pce) {
            System.out.println(ERROR_READ + fileName);
        } catch (SAXException se) {
            System.out.println(ERROR_READ + fileName);
        } catch (IOException ioe) {
            System.out.println(ERROR_READ + fileName);
        }
        return null;
    }

    /**
     * Read the file to modify
     */
    private static void readDocument() {
        createUserCopy();
        document = readDocument(USER_CONFIG_FILE);
    }

    /**
     * Save the modifications
     */
    private static void writeDocument() {
        Transformer transformer = null;
        try {
            transformer = ScilabTransformerFactory.newInstance().newTransformer();
        } catch (TransformerConfigurationException e1) {
            System.out.println(ERROR_WRITE + USER_CONFIG_FILE);
        } catch (TransformerFactoryConfigurationError e1) {
            System.out.println(ERROR_WRITE + USER_CONFIG_FILE);
        }
        transformer.setOutputProperty(OutputKeys.INDENT, "yes");

        StreamResult result = new StreamResult(new File(USER_CONFIG_FILE));
        DOMSource source = new DOMSource(document);
        try {
            transformer.transform(source, result);
        } catch (TransformerException e) {
            System.out.println(ERROR_WRITE + USER_CONFIG_FILE);
        }

    }
    /**
     * @return true if configuration.xml in etc has a version different of the version in home
     */
    public static boolean checkVersion() {
        if (updated) {
            return false;
        }

        File fileConfig = new File(USER_CONFIG_FILE);
        if (fileConfig.exists()) {
            Document doc = readDocument(USER_CONFIG_FILE);
            Element setting = doc.getDocumentElement();
            String str = ((Element) setting).getAttribute(VERSION);
            if (str != null && str.length() != 0) {
                float userVersion = Float.parseFloat(str);
                doc = readDocument(SCILAB_CONFIG_FILE);
                setting = doc.getDocumentElement();
                str = ((Element) setting).getAttribute(VERSION);

                if (str != null && str.length() != 0) {
                    float scilabVersion = Float.parseFloat(str);
                    return userVersion != scilabVersion;
                }
            }
        }

        return true;
    }

    /**
     * Get the name of the user configuration file
     * @return the name of the configuration file
     */
    public static String getUserConfigFile() {
        return USER_CONFIG_FILE;
    }

    /** ----------------------------------------------------------------------------------- **/
    /**
     * Save a new font setting
     * @param font the new font
     * @deprecated
     */
    public static void saveFont(Font font) {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList fontSizeElement = scilabProfile.getElementsByTagName("FontSize");
            Element fontSize = (Element) fontSizeElement.item(0);
            fontSize.setAttribute(VALUE, Integer.toString(font.getSize()));

            NodeList fontNameElement = scilabProfile.getElementsByTagName("FontName");
            Element fontName = (Element) fontNameElement.item(0);
            fontName.setAttribute(VALUE, font.getFontName());

            NodeList fontStyleElement = scilabProfile.getElementsByTagName("FontStyle");
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
    }


    /**
     * Get the maximum number of lines to keep in the output
     * @return the number of lines
     */
    public static int getMaxOutputSize() {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allPositionElements = scilabProfile.getElementsByTagName(MAXOUTPUTSIZE);
            Element maxOutputSize = (Element) allPositionElements.item(0);
            if (maxOutputSize != null) {
                return Integer.parseInt(maxOutputSize.getAttribute(VALUE));
            }
        }
        return DEFAULT_MAXOUTPUTSIZE;
    }

    /**
     * Get the font size in the help viewer
     * @return the font size
     */
    public static int getHelpFontSize() {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allPositionElements = scilabProfile.getElementsByTagName(HELPFONTSIZE);
            Element helpFontSize = (Element) allPositionElements.item(0);
            if (helpFontSize != null) {
                return Integer.parseInt(helpFontSize.getAttribute(VALUE));
            }
        }
        return DEFAULT_HELPFONTSIZE;
    }

    /**
     * Get the font size in the help viewer
     * @return the font size
     */
    public static void setHelpFontSize(int size) {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allPositionElements = scilabProfile.getElementsByTagName(HELPFONTSIZE);
            Element helpFontSize = (Element) allPositionElements.item(0);
            if (helpFontSize == null) {
                helpFontSize = document.createElement(HELPFONTSIZE);
                scilabProfile.appendChild(helpFontSize);
            }

            helpFontSize.setAttribute(VALUE, Integer.toString(size));
            writeDocument();
        }
    }
   /**
     * Save the console Foreground Color
     * @param color the new Color
     */
    public static void saveConsoleForeground(Color color) {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allSizeElements = scilabProfile.getElementsByTagName(FOREGROUNDCOLOR);
            Element consoleForeground = (Element) allSizeElements.item(0);

            String rgb = Integer.toHexString(color.getRGB());
            consoleForeground.setAttribute(VALUE, COLORPREFIX + rgb.substring(2, rgb.length()));

            /* Save changes */
            writeDocument();
        }
    }

    /**
     * Save the console Background Color
     * @param color the new Color
     */
    public static void saveConsoleBackground(Color color) {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allSizeElements = scilabProfile.getElementsByTagName(BACKGROUNDCOLOR);
            Element consoleBackground = (Element) allSizeElements.item(0);

            String rgb = Integer.toHexString(color.getRGB());
            consoleBackground.setAttribute(VALUE, COLORPREFIX + rgb.substring(2, rgb.length()));

            /* Save changes */
            writeDocument();
        }
    }
    /** ----------------------------------------------------------------------------------- **/
    /**         D E P R E C A T E D   S E S S I O N   P A R A M E T E R S                   **/
    /** ----------------------------------------------------------------------------------- **/
    /**
     * Get the position of Scilab Main Window
     * @return the position
     * @deprecated session parameter
     */
    public static Position getMainWindowPosition() {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allPositionElements = scilabProfile.getElementsByTagName(MAINWINPOSITION);
            Element mainWindowPosition = (Element) allPositionElements.item(0);
            if (mainWindowPosition != null) {
                int x = Integer.parseInt(mainWindowPosition.getAttribute(XCOORD));
                int y = Integer.parseInt(mainWindowPosition.getAttribute(YCOORD));
                /* Avoid Scilab Main Window to be out of the screen */
                if (x <= (Toolkit.getDefaultToolkit().getScreenSize().width - MARGIN)
                    && y <= (Toolkit.getDefaultToolkit().getScreenSize().height - MARGIN)) {
                    return new Position(x, y);
                }
            }
        }
        return new Position(0, 0);
    }

    /**
     * Save the position of Scilab Main Window
     * @param position the position of Scilab main Window
     * @deprecated session parameter
     */
    public static void saveMainWindowPosition(Position position) {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allPositionElements = scilabProfile.getElementsByTagName(MAINWINPOSITION);
            Element mainWindowPosition = (Element) allPositionElements.item(0);

            // Ascendant compatibility
            if (mainWindowPosition == null) {
                mainWindowPosition = document.createElement(MAINWINPOSITION);
                scilabProfile.appendChild(mainWindowPosition);
            }

            mainWindowPosition.setAttribute(XCOORD, Integer.toString(position.getX()));
            mainWindowPosition.setAttribute(YCOORD, Integer.toString(position.getY()));

            /* Save changes */
            writeDocument();
        }
    }

    /**
     * Save the size of Scilab Main Window
     * @param size the size of Scilab main Window
     * @deprecated session parameter
     */
    public static void saveMainWindowSize(Size size) {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allPositionElements = scilabProfile.getElementsByTagName(MAINWINSIZE);
            Element mainWindowSize = (Element) allPositionElements.item(0);

            // Ascendant compatibility
            if (mainWindowSize == null) {
                mainWindowSize = document.createElement(MAINWINSIZE);
                scilabProfile.appendChild(mainWindowSize);
            }

            mainWindowSize.setAttribute(WIDTH, Integer.toString(size.getWidth()));
            mainWindowSize.setAttribute(HEIGHT, Integer.toString(size.getHeight()));

            /* Save changes */
            writeDocument();
        }
    }

    /**
     * Get the size of Scilab Main Window
     * @return the size
     * @deprecated session parameter
     */
    public static Size getMainWindowSize() {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allSizeElements = scilabProfile.getElementsByTagName(MAINWINSIZE);
            Element mainWindowSize = (Element) allSizeElements.item(0);
            if (mainWindowSize != null) {
                return new Size(Integer.parseInt(mainWindowSize.getAttribute(WIDTH)), Integer.parseInt(mainWindowSize.getAttribute(HEIGHT)));
            }
        }
        return new Size(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    }

    /**
     * Get the position of Scilab Help Window
     * @return the position
     * @deprecated session parameter
     */
    public static Position getHelpWindowPosition() {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allPositionElements = scilabProfile.getElementsByTagName(HELPWINPOSITION);
            Element helpWindowPosition = (Element) allPositionElements.item(0);
            if (helpWindowPosition != null) {
                int x = Integer.parseInt(helpWindowPosition.getAttribute(XCOORD));
                int y = Integer.parseInt(helpWindowPosition.getAttribute(YCOORD));
                /* Avoid Scilab Help Window to be out of the screen */
                if (x <= (Toolkit.getDefaultToolkit().getScreenSize().width - MARGIN)
                    && y <= (Toolkit.getDefaultToolkit().getScreenSize().height - MARGIN)) {
                    return new Position(x, y);
                }
            }
        }
        return new Position(0, 0);
    }

    /**
     * Save the position of Scilab Help Window
     * @param position the position of Scilab help Window
     * @deprecated session parameter
     */
    public static void saveHelpWindowPosition(Position position) {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allPositionElements = scilabProfile.getElementsByTagName(HELPWINPOSITION);
            Element helpWindowPosition = (Element) allPositionElements.item(0);

            // Ascendant compatibility
            if (helpWindowPosition == null) {
                helpWindowPosition = document.createElement(HELPWINPOSITION);
                scilabProfile.appendChild(helpWindowPosition);
            }

            helpWindowPosition.setAttribute(XCOORD, Integer.toString(position.getX()));
            helpWindowPosition.setAttribute(YCOORD, Integer.toString(position.getY()));

            /* Save changes */
            writeDocument();
        }
    }

    /**
     * Save the size of Scilab Help Window
     * @param size the size of Scilab help Window
     * @deprecated session parameter
     */
    public static void saveHelpWindowSize(Size size) {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allPositionElements = scilabProfile.getElementsByTagName(HELPWINSIZE);
            Element helpWindowSize = (Element) allPositionElements.item(0);

            // Ascendant compatibility
            if (helpWindowSize == null) {
                helpWindowSize = document.createElement(HELPWINSIZE);
                scilabProfile.appendChild(helpWindowSize);
            }

            helpWindowSize.setAttribute(WIDTH, Integer.toString(size.getWidth()));
            helpWindowSize.setAttribute(HEIGHT, Integer.toString(size.getHeight()));

            /* Save changes */
            writeDocument();
        }
    }

    /**
     * Get the size of Scilab Help Window
     * @return the size
     * @deprecated session parameter
     */
    public static Size getHelpWindowSize() {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allSizeElements = scilabProfile.getElementsByTagName(HELPWINSIZE);
            Element helpWindowSize = (Element) allSizeElements.item(0);
            if (helpWindowSize != null) {
                return new Size(Integer.parseInt(helpWindowSize.getAttribute(WIDTH)), Integer.parseInt(helpWindowSize.getAttribute(HEIGHT)));
            }
        }
        return new Size(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    }

    /**
     * Save the Last Opened Directory in Scilab
     * @param the directory's path
     * @deprecated session parameter
     */

    public static void saveLastOpenedDirectory(String path ){
        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allSizeElements = scilabProfile.getElementsByTagName(LASTOPENEDDIR);
            Element lastOpenedDir = (Element) allSizeElements.item(0);

            lastOpenedDir.setAttribute(VALUE, path);

            writeDocument();
        }
    }

    /**
     * Get the Last Opened Directory in Scilab
     * @return the directory's path
     * @deprecated session parameter
     */

    public static String getLastOpenedDirectory(){
        /* Load file */
        /*System.getProperty("user.dir") if no path*/
        readDocument();
        String path = new String() ;

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allSizeElements = scilabProfile.getElementsByTagName(LASTOPENEDDIR);
            Element lastOpenedDir = (Element) allSizeElements.item(0);

            if (lastOpenedDir != null){

                path = lastOpenedDir.getAttribute(VALUE);

                if (path.length() == 0){
                    path = System.getProperty("user.dir") ;
                }
            }else{
                Element newLastOpenedDir =  document.createElement(LASTOPENEDDIR);
                path = System.getProperty("user.dir") ;
                newLastOpenedDir.setAttribute("useCache","true");
                newLastOpenedDir.setAttribute(VALUE, path);

                scilabProfile.appendChild(newLastOpenedDir);

                writeDocument();
            }
        }
        return path ;
    }
}


