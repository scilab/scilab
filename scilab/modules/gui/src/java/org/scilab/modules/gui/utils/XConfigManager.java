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

package org.scilab.modules.gui.utils;

import java.awt.Color;
import java.awt.Container;
import java.awt.Component;
import java.awt.BorderLayout;
import java.awt.Frame;
import java.awt.Font;
import java.awt.Toolkit;
import java.awt.Dimension;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Hashtable;

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

import org.scilab.modules.localization.Messages;

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
import javax.swing.JComponent;
import javax.swing.BorderFactory;
import javax.swing.border.Border;

/**
 * Extended management of the Console configuration file.
 *
 * @author Pierre GRADIT
 * @author Vincent COUVERT *
 */
public final class XConfigManager /*extends ConfigManager */ {

    /** Buffer size.*/
    private static final int BUFSIZE = 1024;
    /** Margin size.*/
    private static final int MARGIN = 20;
    /** Message for read error.*/
    private static final String ERROR_READ = "Could not load file: ";
    /** Message for write error.*/
    private static final String ERROR_WRITE = "Could not save file: ";
    /** Message for version.*/
    private static final String VERSION = "version";
    /** Message for value.*/
    private static final String VALUE = "value";
    /** Message for width.*/
    private static final String WIDTH = "width";
    /** Message for height.*/
    private static final String HEIGHT = "height";
    /** Message for x coordinate.*/
    private static final String XCOORD = "x";
    /** Message for y coordinate.*/
    private static final String YCOORD = "y";
    /** Message for main window position.*/
    private static final String MAINWINPOSITION = "MainWindowPosition";
    /** Message for main widow size.*/
    private static final String MAINWINSIZE = "MainWindowSize";
    /** Message for help window position.*/
    private static final String HELPWINPOSITION = "HelpWindowPosition";
    /** Message for help widow size.*/
    private static final String HELPWINSIZE = "HelpWindowSize";
    /** Message for help font size.*/
    private static final String HELPFONTSIZE = "HelpFontSize";
    /** Message for profile.*/
    private static final String PROFILE = "Profile";
    /** Message for foreground color.*/
    private static final String FOREGROUNDCOLOR = "ForegroundColor";
    /** Message for background color.*/
    private static final String BACKGROUNDCOLOR = "BackgroundColor";
    /** Message for color prefix.*/
    private static final String COLORPREFIX = "#";
    /** Message for maximum output size.*/
    private static final String MAXOUTPUTSIZE = "MaxOutputSize";
    /** Message for last opened dir.*/
    private static final String LASTOPENEDDIR = "LastOpenedDirectory";
    /** Default Width.*/
    private static final int DEFAULT_WIDTH = 650;
    /** Default height.*/
    private static final int DEFAULT_HEIGHT = 550;
    /** Default max output size.*/
    private static final int DEFAULT_MAXOUTPUTSIZE = 10000;
    /** Default help font size.*/
    private static final int DEFAULT_HELPFONTSIZE = 2;

    /** Scilab configuration file.*/
    private static final   String   SCILAB_CONFIG_FILE =
        System.getenv("SCI") + "/modules/console/etc/XConfiguration.xml";
    /** Scilab configuration stylesheet.*/
    private static final   String   SCILAB_CONFIG_XSL  =
        System.getenv("SCI") + "/modules/gui/src/xslt/XConfiguration.xsl";
    /** User configuration file.*/
    private static final   String   USER_CONFIG_FILE   =
        ScilabCommons.getSCIHOME() + "/XConfiguration.xml";

    /** Main dialog.*/
    private static       JDialog  dialog;
    //private static SwingScilabTab dialog;
    /** DOM Document.*/
    private static         Document document;
    /** Up-to-date flag.*/
    private static         boolean  updated            = false;

    /** Top level DOM Node.*/
    private static Node topDOM;
    /** Top level Swing container.*/
    private static Container topSwing;
    /** Container-Sentinel correspondence.*/
    private static Hashtable<Component, XSentinel> correspondance;
    /** Last visitor.*/
    private static XUpdateVisitor visitor;

    /** Display dialog and wait for events.
     *
     */
    public static void displayAndWait() {

        reloadTransformer();

        // Set up Swing Side
        dialog      = new JDialog(getTopLevel(), "Scilab Preferences", true);
        topSwing    = dialog.getContentPane();
        //dialog = new SwingScilabTab("Scilab Preferences");
        //topSwing = new JPanel();
        dialog.setContentPane(topSwing);
        topSwing.setLayout(new BorderLayout());
        // AWT implies to set layout at construction time.

        // Set up DOM Side
        readDocument();
        updated = false;

        // Set up correspondence
        correspondance = new Hashtable<Component, XSentinel>();

        // Let the show begin!
        if (refreshDisplay()) {
            dialog.setVisible(true);
        }
    }

    /** Launch swing hierarchy update.
     *
     * @return whether XSL return a node or not.
     */
    public static boolean refreshDisplay() {

        // Generate new view DOM.
        topDOM = generateViewDOM().getNode().getFirstChild();
        if (topDOM == null) {
            System.err.println("XSL does not give a node!");
            return false;
        }

        // Refresh correspondence
        //    TODO top layout changes
        visitor = new XUpdateVisitor(correspondance);
        visitor.visit(topSwing, topDOM);
        dialog.pack();

        // Refresh contextual help
        if (!(help == null)) {
            help();
        } else {
            /** TODO remove this! */
            //viewDOM and swingComposite code are intentionally homologous.
            //System.err.println(viewDOM());
            //System.err.println(swingComposite());
        }
        return true;
    }

    /** Secondary dialog for help.*/
    private static JDialog  help = null;
    /** Help component.*/
    private static JTextArea xml = null;
    /** Help component text size.*/
    private static final int TEXT_SIZE = 30;

    /** Contextual help meant for developer so far.
     *
     *  TODO should display the View DOM and the Swing composite
     *       on single scrolled pair of textarea to manifest correspondence
     *
     *  TODO build proper user contextual help.
     *
     */
    public static void help() {
        if (help == null) {
            JScrollPane scroll = new JScrollPane();
            help               = new JDialog(getTopLevel(), "XML view", false);
            xml                = new JTextArea(TEXT_SIZE, TEXT_SIZE);
            scroll.add(xml);
            help.add(scroll);
            help.setVisible(true);
        }
        String  view       = viewDOM();
        xml.setText(view);
    }

    /** Horizontal space between parent and child in String representations.
     *
     */
    public static final String INCREMENT = "    ";

    /** Compute recursive string representation of DOM view tree.
     *
     * @param node : current node
     * @param indent : current indentation
     * @return node representation
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
    private static String viewDOM() {
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
    private static String swingComposite() {
        return swingComposite(topSwing, "");
    }

    /** XSL Transformer factory.
     *
     */
    private static TransformerFactory
         factory  = ScilabTransformerFactory.newInstance();
    /** XML Document builder factory.
     *
     */
    private static DocumentBuilderFactory
         builder  = ScilabDocumentBuilderFactory.newInstance();

    /** XSL Transformer.
     *
     */
    private static Transformer transformer = null;

    /** Load XSL as XSL Transformer.
     *
     */
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

    /** Generate view by application of XSL on XConfiguration file.
     *
     * @return View DOM.
     */
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

    /** Identify an element with its context string.
     *
     *     TODO logarithmic id (3/3/6/1...) instead of linear (SELECT[3]) one!
     *
     * @param context : the context string used to catch the element.
     * @return the corresponding node
     */
    public static Element getElementByContext(final String context) {
        String [] id = context.split("#");
        NodeList elements = document.getElementsByTagName(id[0]);
        if (elements.getLength() == 1) {
            return (Element) elements.item(0);
        }
        if (elements.getLength() == 0) {
            System.err.println("'" + context + "' has no image in document!");
            return null;
            }
        if (id.length == 1) {
            System.err.println("'" + context + "' has not a unique response "
                                             + "(use unique-id template)!");
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


    /** Interpret action.
     *
     * @param action : to be interpreted.
     * @param source : component source of the action (only class is needed).
     */
    public static void xEvent(final Node action, final Component source) {

        if (!getAttribute(action, "set").equals(NAV)) {
            String context   = getAttribute(action, "context");
            Element element  = getElementByContext(context);

            String value     = getAttribute(action, "value");
            String attribute = getAttribute(action, "set");
            if (!(element == null)) {
                element.setAttribute(attribute, value);
            }
            refreshDisplay();
            updated = true;
            return;
        }

        if (!getAttribute(action, "choose").equals(NAV)) {
            String context   = getAttribute(action, "context");
            Element element  = getElementByContext(context);

            if (source instanceof XChooser) {
                XChooser chooser   = (XChooser) source;
                String   value     = chooser.choose();
                String   attribute = getAttribute(action, "choose");
                if (!(element == null)) {
                    element.setAttribute(attribute, value);
                }
                refreshDisplay();
                updated = true;
                //TODO not always real modification...
            } else {
                System.err.println("Choose attribute only valid on choosers "
                                 + "(SELECT, COLOR, FILE, ENTRY,...)");
            }
            return;
        }

        String callback = getAttribute(action, "callback");
        if (callback.equals("Help")) {
            // TODO it can be a contextual help.
            //System.err.println("Help not implemented yet!");
            return;
        }
        if (callback.equals("Ok")) {
            writeDocument();
            dialog.dispose();
            updated = false;
            return;
        }
        if (callback.equals("Apply")) {
            //System.err.println("User XML saved!");
            updated = false;
            writeDocument();
            return;
        }
        if (callback.equals("Default")) {
            //System.out.println("Scilab XML reloaded!");
            reloadTransformer();
            refreshUserCopy();
            readDocument();
            updated = false;
            refreshDisplay();
            return;
        }
        if (callback.equals("Cancel")) {
            //System.err.println("User XML reloaded!");
            readDocument();
            if (updated) {
                //TODO advertise it!
                }
            updated = false;
            refreshDisplay();
            return;
        }
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
        return Integer.toHexString(source.getRGB());
    }

    /** Manage color representation.
     *
     * @param source : the string representation.
     * @return the corresponding color
     */
    public static Color getColor(final String source) {
        return Color.decode(source);
    }

    /** TODO How to impact modification of preferences ?
     *
     */
    public static void subscribeUpdate() {
    }

    /** TODO How to impact modification of preferences ?
    *
    */
    public static void notifyUpdate() {
    }

    /** Get top level window for correct dialog opening.
     *
     * @return top-level frame.
     */
    private static Frame getTopLevel() {
        Container main = (Container) ScilabConsole.getConsole().getAsSimpleConsole();
        return (Frame)main.getParent().getParent().getParent().getParent().getParent().getParent();
    }

    /** Create a copy of Scilab configuration file in the user directory.
     *
     */
    public static void createUserCopy() {
        File fileConfig = new File(USER_CONFIG_FILE);
        if (!fileConfig.exists()
            /*|| (fileConfig.length() == 0) || checkVersion()*/
            ) {
            refreshUserCopy();
        }
    }

    /** Refresh configuration file in the user directory with Scilab defaults.
     *
     */
    public static void refreshUserCopy() {
            /* Create a local copy of the configuration file */
            try {
                copyFile(new File(SCILAB_CONFIG_FILE),
                         new File(USER_CONFIG_FILE));
            } catch (FileNotFoundException e) {
                System.out.println(ERROR_READ + USER_CONFIG_FILE);
            }
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

    // BELOW THIS LINE IS THE ORIGINAL CODE LEFT FOR REUSE.

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
    private static Document readDocument(final String fileName) {
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
     * @return true if configuration.xml in etc has a version different.
     * of the version in home
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

    /** Get the name of the user configuration file.
     * @return the name of the configuration file
     */
    public static String getUserConfigFile() {
        return USER_CONFIG_FILE;
    }

    /** Save a new font setting.
     * @param font the new font
     * @deprecated
     */
    public static void saveFont(final Font font) {

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
    public static void setHelpFontSize(final int size) {

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
    public static void saveConsoleForeground(final Color color) {

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
    public static void saveConsoleBackground(final Color color) {

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


    // BELOW THIS LINE IS THE ORIGINAL CODE LEFT FOR SESSION PARAMETERS.

    
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
    public static void saveMainWindowPosition(final Position position) {

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
    public static void saveMainWindowSize( final Size size) {

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
    public static void saveHelpWindowPosition(final Position position) {

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
    public static void saveHelpWindowSize(final Size size) {

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

    public static void saveLastOpenedDirectory(final String path ){
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



