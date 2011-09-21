/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 -         Pierre GRADIT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.preferences;

import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.GraphicsEnvironment;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Hashtable;

import javax.swing.BorderFactory;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.border.Border;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.TransformerFactoryConfigurationError;
import javax.xml.transform.dom.DOMResult;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;

import org.scilab.modules.localization.Messages;

import org.scilab.modules.commons.ScilabCommons;
import org.scilab.modules.commons.xml.ScilabDocumentBuilderFactory;
import org.scilab.modules.commons.xml.ScilabTransformerFactory;

import org.scilab.modules.gui.console.ScilabConsole;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

/* This class is the common ancestor to both X_manager.
 */
public abstract class XCommonManager {

    /** XConfiguration management verbosity.*/
    public static final boolean performances = true;
    /** XConfiguration management verbosity.*/
    public static final boolean differential = true;

    /** Message for read error.*/
    protected static final String ERROR_READ = "Could not load file: ";
    /** Message for write error.*/
    protected static final String ERROR_WRITE = "Could not save file: ";
    /** Buffer size.*/
    protected static final int BUFSIZE = 1024;

    /** Main dialog.*/
    protected static       JDialog  dialog;
    //private static SwingScilabTab dialog;
    /** DOM Document.*/
    protected static         Document document;
    /** DOM Document.*/
    protected static         String documentAddress;
    /** Up-to-date flag.*/
    protected static         boolean  updated = false;

    /** Top level DOM Node.*/
    protected static Node topDOM;
    /** Top level Swing container.*/
    protected static Container topSwing;
    /** Container-Sentinel correspondence.*/
    protected static Hashtable<Component, XSentinel> correspondance;
    /** Last visitor.*/
    protected static XUpdateVisitor visitor;

    /** Last current time.
    */
    protected static long time = System.currentTimeMillis();
    /** Monitor time between calls.
    *
    */
    public static void printTimeStamp(final String msg) {
        long nextTime  = System.currentTimeMillis();
        long deltaTime = nextTime - time;
        if (performances) {
            System.out.println(
                    (msg.startsWith("*")?"":" |  ")
                    + msg + " in " + deltaTime + " ms."
            );
        }
        time   = nextTime;
    }

    /** Launch swing hierarchy update.
    *
    * @return whether XSL return a node or not.
    */
    public static boolean refreshDisplay() {
    	// Generate new view DOM.
       printTimeStamp("Context found");
       topDOM = generateViewDOM().getNode().getFirstChild();
       printTimeStamp("View XML generated");
       if (topDOM == null) {
           System.err.println("XSL does not give a node!");
           return false;
       }

       // Refresh correspondence
       //    TODO top layout changes
       visitor = new XUpdateVisitor(correspondance);
       visitor.visit(topSwing, topDOM, "\t");
       printTimeStamp("SWING refreshed");
       dialog.pack();
       printTimeStamp("Packing done");

       /* TODO contextual help */

       return true;
    }
    
    /** Horizontal space between parent and child in String representations.
     *
     */
    public static final String INCREMENT = "    ";

    /** Compute recursive string representation of DOM view tree.
     *
     * @param node : current node
     * @param indent : current indentation
     *   @return node representation
     *
     * TODO invisible View nodes used for control shall be replaced
     *       by blank lines in Swing composite
     */
    private static String viewDOM(final Node node, final String indent) {
        String signature = indent;
        if (node.hasAttributes()) {
            signature += XSentinel.signature(node, new String[0]);
        } else {
            String single = node.getNodeName() + ": " + node.getNodeValue();
            signature    += single.replaceAll("[ \t\n]+", " ");
        }
        signature += "\n";
        NodeList nodelist = node.getChildNodes();
        if (true) {
            for (int i = 0; i < nodelist.getLength(); i++) {
                Node item  = nodelist.item(i);
                signature += viewDOM(item, indent + INCREMENT);
            }
        }
        return signature;
    }

    /** Compute top-level string representation of DOM view tree.
     *
     * @return document representation
     */
    protected static String viewDOM() {
        return viewDOM(topDOM, "");
    }

    /** Compute recursive string representation of Swing tree.
     *
     * @param component : current component
     * @param indent : current indentation
     * @return component representation
     *
     * TODO invisible View nodes used for control shall be replaced
     *       by blank lines in Swing composite
     */
    private static String swingComposite(
          final Component component,
          final String indent) {
        String signature = indent;
        if (true) {
            signature       += component.toString();
        }
        signature       += "\n";
        if (component instanceof Container) {
            Container container = (Container) component;
            Component [] components = container.getComponents();
            for (int i = 0; i < components.length; i++) {
                Component child = components[i];
                signature += swingComposite(child, indent + INCREMENT);
            }
        }
        return signature;
    }

    /** Compute top-level string representation of swing composite.
     *
     * @return Graphical interface representation
     */
    protected static String swingComposite() {
        return swingComposite(topSwing, "");
    }

    /** XSL Transformer factory.
     *
     */
    protected static TransformerFactory
        factory  = ScilabTransformerFactory.newInstance();
    /** XML Document builder factory.
     *
     */
    protected static DocumentBuilderFactory
        builder  = ScilabDocumentBuilderFactory.newInstance();

    /** XSL Transformer.
     *
     */
    protected static Transformer transformer = null;

    /** Load XSL as XSL Transformer.
    *
    */
    protected static void reloadTransformer(String address) {
       try {
           StreamSource source = new StreamSource(address);
           transformer         = factory.newTransformer(source);
       } catch (TransformerConfigurationException e1) {
           System.out.println(ERROR_READ + address);
       } catch (TransformerFactoryConfigurationError e1) {
           System.out.println(ERROR_READ + address);
       }
   }

    /** Generate view by application of XSL on XConfiguration file.
     *
     * @return View DOM.
     */
    private static DOMResult generateViewDOM() {
        DOMResult result    = new DOMResult();
        DOMSource source    = new DOMSource(document);
        try {
            if (false) {
                //TODO transmit font family list to XSL
                GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
                String [] fonts = ge.getAvailableFontFamilyNames();
                for (int i=0; i< fonts.length; i++) {
                   System.out.println("		<option font-family='" + fonts[i] + "'/>");
                }
            }
            transformer.transform(source, result);
        } catch (TransformerException e) {
            // Just highlight clear transformer output.
            System.out.println("\n");
        }
        return result;
    }

    /** Identify an element with its context string.
    *
    * @see XConfiguration.xsl#context
    * @param context : the context string used to catch the element.
    * @return the corresponding node
    */
    public static Element getElementByContext(final String context) {
        String [] ids    = context.split("/");
        Element element = (Element) document.getDocumentElement();
        for (int i = 0; i < ids.length; i++) {
            Integer integer     = Integer.parseInt(ids[i]);
            int index           = integer.intValue();
            // get the element with corresponding index (filter text nodes)
            NodeList childNodes = element.getChildNodes();
            Node node           = null;
            int j = 0;
            while (index > 0 && j < childNodes.getLength()) {
                node = childNodes.item(j);
                if (node.getNodeName() != "#text" 
                 && node.getNodeName() != "#comment"
                 ) {
                    index--;
                }
                j++;
            }
            if (index == 0) {
                element = (Element) node;
            } else {
                // j == childNodes.getLength()
                System.err.println("'" + context + "' out of document!");
                return null;
            }
            //System.err.println( i + " = " + element.getNodeName());
        }
        return element;
    }


    /** Interpret action.
     *
     * @param action : to be interpreted.
     * @param source : component source of the action (only class is needed).
     */
    protected static boolean generixEvent(
            final Node [] actions,
            final Component source
        ) {
        printTimeStamp("*** Event occured");

        if (actions.length==0) {
            return false;
        }

        Node action = actions[0];
        // All actions must be of the same kind.
        
        if (differential) {
            System.out.print("*** " + action.getNodeName());
        }
        if (!getAttribute(action, "set").equals(NAV)) {
            for (int i=0; i < actions.length; i++) {
                action = actions[i];
                String context   = getAttribute(action, "context");
                if (differential) {
                    System.out.println(" hits " + context);
                }
                Element element  = getElementByContext(context);
                String value     = getAttribute(action, "value");
                String attribute = getAttribute(action, "set");
                if (!(element == null)) {
                    element.setAttribute(attribute, value);
                }
            }
            refreshDisplay();
            updated = true;
            return true;
        }

        if (source==null) {
            System.out.println(" lost! ");
            return false;
        }
        
        if (!getAttribute(action, "choose").equals(NAV)) {
            String context   = getAttribute(action, "context");
            if (differential) {
                System.out.println(" hits " + context);
            }
            Element element  = getElementByContext(context);
            if (source instanceof XChooser) {
                XChooser chooser   = (XChooser) source;
                String   value     = chooser.choose();
                if (value != null) {
                    String   attribute = getAttribute(action, "choose");
                    if (!(element == null)) {
                        element.setAttribute(attribute, value);
                    }
                    refreshDisplay();
                    updated = true;
                }
            } else {
                System.err.println("@choose attribute only valid on choosers "
                                 + "(SELECT, COLOR, FILE, ENTRY,...)");
            }
            return true;
        }
        return false;
    }

    /** Sentinel string for attribute consulting.
     *
     */
    public static final String NAV = "\"not an value'";

    /** Attribute consulting with default.
     *
     * @param node : consulted node
     * @param name : attribute key
     * @param value : default value
     * @return the consulted value
     */
   public static String getAttribute(
           final Node node,
           final String name,
           final String value
           ) {
       String response = getAttribute(node, name);
       if (response == NAV) {
           return value;
       }
       return response;
   }

    /** Attribute consulting without default.
     *
     * @param node : consulted node.
     * @param name : attribute key.
     * @return the consulted value (or NAV if attribute key is absent)
     */
    public static String getAttribute(final Node node, final String name) {
        NamedNodeMap attrs = node.getAttributes();
        if (attrs == null) {
            return NAV;
        }
        Node attr = attrs.getNamedItem(name);
        if (attr == null) {
            return NAV;
        }
        return attr.getNodeValue();
    }

    /** Typed attribute consulting with default.
     *
     * @param node : consulted node.
     * @param name : attribute key.
     * @param value : default value.
     * @return the value.
     */
    public static int getInt(
            final Node node,
            final String name,
            final int value
            ) {
        String response = getAttribute(node, name);
        if (response == NAV) {
            return value;
        }
        Integer integer = Integer.parseInt(response);
        return integer.intValue();
    }


    /** Manage color representation.
     *
     * @param source : the color.
     * @return the string representation.
     */
    public static String getColor(final Color source) {
    	String hexStr = Integer.toHexString(source.getRGB());
        return "#" + hexStr.substring(2);
    }

    /** Manage color representation.
     *
     * @param source : the string representation.
     * @return the corresponding color
     */
    public static Color getColor(final String source) {
        return Color.decode(source);
    }

   /** Get top level window for correct dialog opening.
    *
    * @return top-level frame.
    */
    protected static Frame getTopLevel() {
       Container main = (Container) ScilabConsole.getConsole().getAsSimpleConsole();
       return (Frame) main.getParent().getParent().getParent().getParent().getParent().getParent();
   }

    /** draw construction borders for layout debug.
    *
    * @param component : the marked component.
    */
    public static void drawConstructionBorders(final JComponent component) {
        if (false) {
            // hard-coded flag.
            Border construction = BorderFactory.createLineBorder(Color.red);
            component.setBorder(construction);
        }
    }

    /** Set a dimension for a component.
     *
     * @param component : the resized component.
     * @param peer : the node having the dimension information.
     */
    public static void setDimension(
            final JComponent component,
            final Node peer) {
        int      height     = XConfigManager.getInt(peer , "height", 0);
        int       width     = XConfigManager.getInt(peer , "width",  0);
        if (height > 0 && width > 0) {
            //System.err.println("Dimension: " + width + "x" + height);
            Dimension dimension = new Dimension(width, height);
           component.setPreferredSize(dimension);
        }
    }

    /** Create a copy of Scilab configuration file in the user directory.
    *
    */
    public static void createUserCopy(String original, String copy) {
        File fileConfig = new File(copy);
        if (!fileConfig.exists()
            /*|| (fileConfig.length() == 0) || checkVersion()*/
            ) {
            refreshUserCopy(original, copy);
        }
    }

    /** Refresh configuration file in the user directory with Scilab defaults.
     *
     */
    public static void refreshUserCopy(String original, String copy) {
            /* Create a local copy of the configuration file */
            try {
                copyFile(new File(original),
                         new File(copy));
            } catch (FileNotFoundException e) {
                System.out.println(ERROR_READ + copy);
            }
    }

    /**
     * Copy a file
     * @param in src file
     * @param out dest file
     * @throws FileNotFoundException
     */
    private static void copyFile(final File in, final File out) throws FileNotFoundException {
        FileInputStream fis = new FileInputStream(in);
        FileOutputStream fos = new FileOutputStream(out);

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
    protected static Document readDocument(final String fileName) {
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
     * Save the modifications
     */
    protected static void writeDocument(String filename) {
        Transformer transformer = null;
        try {
            transformer = ScilabTransformerFactory.newInstance().newTransformer();
        } catch (TransformerConfigurationException e1) {
            System.out.println(ERROR_WRITE + filename);
        } catch (TransformerFactoryConfigurationError e1) {
            System.out.println(ERROR_WRITE + filename);
        }
        transformer.setOutputProperty(OutputKeys.INDENT, "yes");

        StreamResult result = new StreamResult(new File(filename));
        DOMSource source = new DOMSource(document);
        try {
            transformer.transform(source, result);
        } catch (TransformerException e) {
            System.out.println(ERROR_WRITE + filename);
        }
    }
}
