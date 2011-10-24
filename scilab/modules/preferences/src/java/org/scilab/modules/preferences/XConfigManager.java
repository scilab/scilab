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
import java.awt.BorderLayout;
import java.awt.Font;
import java.awt.Toolkit;

import java.io.File;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.List;
import java.util.Map;

import javax.swing.JDialog;
import javax.swing.JTextArea;
import javax.swing.JScrollPane;

import org.w3c.dom.Document;
import org.w3c.dom.DocumentFragment;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import org.scilab.modules.commons.ScilabCommons;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.preferences.ScilabPreferences.ToolboxInfos;

/**
 * Extended management of the Console configuration file.
 *
 * @author Pierre GRADIT
 * @author Vincent COUVERT *
 */
public final class XConfigManager extends XCommonManager {

    /** Scilab configuration file.*/
    private static final   String   SCILAB_CONFIG_FILE =
        System.getenv("SCI") + "/modules/preferences/etc/XConfiguration.xml";
    /** Scilab configuration stylesheet.*/
    private static final   String   SCILAB_CONFIG_XSL  =
        System.getenv("SCI") + "/modules/preferences/src/xslt/XConfiguration.xsl";
    /** User configuration file.*/
    private static final   String   USER_CONFIG_FILE   =
        ScilabCommons.getSCIHOME() + "/XConfiguration.xml";

    /**
     * Constructor blocked, singleton pattern.
     */
    private XConfigManager() {
        throw new UnsupportedOperationException();
    }
    
    /** Exclusive activity flag between all XCommonManager descendants.*/
    public static boolean active = false;

    /** Display dialog and wait for events.
     *
     */
    public static void displayAndWait() {
    	XConfigManager.active = true;
        XWizardManager.active = false;
        printTimeStamp("XConfigManager launched");
        System.out.println(" |  No active manager (" + XConfigManager.active + ", " + XWizardManager.active + ").");

        reloadTransformer(SCILAB_CONFIG_XSL);
        printTimeStamp("XSL loaded");

        // Set up Swing Side
        dialog      = new JDialog(getTopLevel(), "Scilab Preferences", true);
        topSwing    = dialog.getContentPane();
	//dialog.setResizable(false);

        //dialog = new SwingScilabTab("Scilab Preferences");
        //topSwing = new JPanel();
        dialog.setContentPane(topSwing);
        topSwing.setLayout(new BorderLayout());
        // AWT implies to set layout at construction time.

        // Set up DOM Side
        readUserDocuments();
        updated = false;
        printTimeStamp("Model XML loaded");

        // Set up correspondence
        correspondance = new Hashtable<Component, XSentinel>();

        // Let the show begin!
        if (refreshDisplay()) {
            dialog.setVisible(true);
        }

    }

    /**
     * Opens a dialog to setup preferences.
     */
    public static void openPreferences() {
        XConfigManager.displayAndWait();
    }

    /** Secondary dialog for help.*/
    private static JDialog  help = null;
    /** Help component.*/
    private static JTextArea xml = null;
    /** Help component text size.*/
    private static final int TEXT_SIZE = 30;

    /** Contextual help meant for developer so far.
     *  @deprecated
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

    /** Read files to modify (and possibly create it).
     */
    private static void readUserDocuments() {
        // Main file
        createUserCopy(SCILAB_CONFIG_FILE, USER_CONFIG_FILE);
        document = readDocument(USER_CONFIG_FILE);
        NodeList toolboxes = document.getElementsByTagName("toolboxes");
        if (toolboxes.getLength() != 1) {
             System.err.println("Can't hook toolboxes [1]");
             return;
        }
        
        // Toolboxes files
        Element toolbox          = (Element) toolboxes.item(0);
        toolboxes                = toolbox.getChildNodes();
        if (toolboxes.getLength()>0) {
            System.err.println("Recover from inconsistent state...");
            while (toolbox.hasChildNodes()) {
                toolbox.removeChild(toolbox.getFirstChild());
            }
        }
        
        List<ToolboxInfos> infos = ScilabPreferences.getToolboxesInfos();
        System.out.println(""+infos.size()+ " toolboxes loaded.");
        for (int i=0; i<infos.size(); i++) {
            ToolboxInfos info       = infos.get(i);
            String UserToolboxToken = info.getName().replace(' ','_');
            String UserToolboxFile  = ScilabCommons.getSCIHOME() + "/" 
                + UserToolboxToken + ".xml";
            createUserCopy(info.getPrefFile(), UserToolboxFile);
            // Building document fragment
            Element token             = document.createElement(UserToolboxToken);
            DocumentFragment fragment = document.createDocumentFragment();
            Document ToolboxDocument  = readDocument(UserToolboxFile);
            Node transferred          = ToolboxDocument.getDocumentElement();
            //-- System.out.println("-->" + transferred.getNodeName());
            transferred               = document.importNode(transferred, true);
            fragment.appendChild(transferred);
            token.insertBefore(fragment, null);
            toolbox.appendChild(token);
        }

        toolboxes                = toolbox.getChildNodes();
        if (infos.size() != toolboxes.getLength()) {
            System.err.println("Can't hook toolboxes [4]");
            return;
        }

    }

    private static void WriteUserDocuments() {
        // Toolboxes files
        NodeList toolboxes       = document.getElementsByTagName("toolboxes");
        Element toolbox          = (Element) toolboxes.item(0);
        List<ToolboxInfos> infos = ScilabPreferences.getToolboxesInfos();
        toolboxes                = toolbox.getChildNodes();
        if (infos.size() != toolboxes.getLength()) {
            System.err.println("Can't hook toolboxes [3]");
            return;
        }
        for (int i=0; i<infos.size(); i++) {
            Node ToolboxNode = toolboxes.item(i);
            if (ToolboxNode != null) {
                ToolboxInfos info = infos.get(i);
                String UserToolboxFile = ScilabCommons.getSCIHOME() + "/" 
                    + info.getName().replace(' ','_') + ".xml";
                writeDocument(UserToolboxFile, ToolboxNode.getFirstChild());
                //toolbox.removeChild(ToolboxNode);
            }
        }
        // Main file
        writeDocument(USER_CONFIG_FILE, document);
    }
    
    /** Interpret action.
     *
     * @param action : to be interpreted.
     * @param source : component source of the action (only class is needed).
     * @return whether event was treated or not.
     */
    public static boolean xEvent(final Node [] actions, final Component source) {
        if (generixEvent(actions, source)) {
            return true;
        }
        if (actions.length==0) {
//C            System.out.println("No actions processed!");
            return false;
        }
        Node action = actions[0];
        String callback = getAttribute(action, "callback");
        /** help deprecated
        if (callback.equals("Help")) {
            // TODO it can be a contextual help.
            //System.err.println("Help not implemented yet!");
            if (differential) {
                System.out.println(": Help.");
            }
            return true;
        }
        **/
        if (callback.equals("Ok")) {
            if (differential) {
                System.out.println(": Ok.");
            }
            WriteUserDocuments();
            dialog.dispose();
            updated = false;
            return true;
        }
        if (callback.equals("Apply")) {
            //System.err.println("User XML saved!");
            updated = false;
            if (differential) {
                System.out.println(": Apply.");
            }
            WriteUserDocuments();
            return true;
        }
        if (callback.equals("Default")) {
            //System.out.println("Scilab XML reloaded!");
            if (differential) {
                System.out.println(": Default.");
            }

            reloadTransformer(SCILAB_CONFIG_XSL);
            refreshUserCopy(SCILAB_CONFIG_FILE, USER_CONFIG_FILE);
            List<ToolboxInfos> infos = ScilabPreferences.getToolboxesInfos();
            for (int i=0; i<infos.size(); i++) {
                ToolboxInfos info = infos.get(i);
                String UserToolboxFile = ScilabCommons.getSCIHOME() + "/" 
                    + info.getName().replace(' ','_') + ".xml";
                refreshUserCopy(info.getPrefFile(), UserToolboxFile);    
            }
            readUserDocuments();
            printTimeStamp("XSL Reloaded");
            updated = false;
            refreshDisplay();
            return true;
        }
        if (callback.equals("Cancel")) {
            //System.err.println("User XML reloaded!");
            readUserDocuments();
            /* TODO advertise it!
            if (updated) {
                <<some advertising statement>>
                }
            */
            updated = false;
            if (differential) {
                System.out.println(": Cancel.");
            }
            refreshDisplay();
            return true;
        }
        return false;
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

    // -------------------------------------------------------------------------------------------------------
    // BELOW THIS LINE IS DEPRECATED CODE LEFT FOR UPWARD COMPATIBILITY.

    /** Margin size.*/
    private static final int MARGIN = 20;
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
     * @deprecated
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
        createUserCopy(SCILAB_CONFIG_FILE, USER_CONFIG_FILE);
        document = readDocument(USER_CONFIG_FILE);
        
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
            writeDocument(USER_CONFIG_FILE, document);
        }
    }


    /**
     * Get the maximum number of lines to keep in the output
     * @return the number of lines
     * @deprecated
     */
    public static int getMaxOutputSize() {

        /* Load file */
        createUserCopy(SCILAB_CONFIG_FILE, USER_CONFIG_FILE);
        document = readDocument(USER_CONFIG_FILE);
        
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
     * @deprecated
     */
    public static int getHelpFontSize() {

        /* Load file */
        createUserCopy(SCILAB_CONFIG_FILE, USER_CONFIG_FILE);
        document = readDocument(USER_CONFIG_FILE);
        
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
     * @deprecated
     */
    public static void setHelpFontSize(final int size) {

        /* Load file */
        createUserCopy(SCILAB_CONFIG_FILE, USER_CONFIG_FILE);
        document = readDocument(USER_CONFIG_FILE);
        
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
            writeDocument(USER_CONFIG_FILE, document);
        }
    }
   /**
     * Save the console Foreground Color
     * @param color the new Color
     * @deprecated
     */
    public static void saveConsoleForeground(final Color color) {

        /* Load file */
        createUserCopy(SCILAB_CONFIG_FILE, USER_CONFIG_FILE);
        document = readDocument(USER_CONFIG_FILE);
        
        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allSizeElements = scilabProfile.getElementsByTagName(FOREGROUNDCOLOR);
            Element consoleForeground = (Element) allSizeElements.item(0);

            String rgb = Integer.toHexString(color.getRGB());
            consoleForeground.setAttribute(VALUE, COLORPREFIX + rgb.substring(2, rgb.length()));

            /* Save changes */
            writeDocument(USER_CONFIG_FILE, document);
        }
    }

    /**
     * Save the console Background Color
     * @param color the new Color
     * @deprecated
     */
    public static void saveConsoleBackground(final Color color) {

        /* Load file */
        createUserCopy(SCILAB_CONFIG_FILE, USER_CONFIG_FILE);
        document = readDocument(USER_CONFIG_FILE);
        
        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allSizeElements = scilabProfile.getElementsByTagName(BACKGROUNDCOLOR);
            Element consoleBackground = (Element) allSizeElements.item(0);

            String rgb = Integer.toHexString(color.getRGB());
            consoleBackground.setAttribute(VALUE, COLORPREFIX + rgb.substring(2, rgb.length()));

            /* Save changes */
            writeDocument(USER_CONFIG_FILE, document);
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
        createUserCopy(SCILAB_CONFIG_FILE, USER_CONFIG_FILE);
        document = readDocument(USER_CONFIG_FILE);
        
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
        createUserCopy(SCILAB_CONFIG_FILE, USER_CONFIG_FILE);
        document = readDocument(USER_CONFIG_FILE);
        
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
            writeDocument(USER_CONFIG_FILE, document);
        }
    }

    /**
     * Save the size of Scilab Main Window
     * @param size the size of Scilab main Window
     * @deprecated session parameter
     */
    public static void saveMainWindowSize( final Size size) {

        /* Load file */
        createUserCopy(SCILAB_CONFIG_FILE, USER_CONFIG_FILE);
        document = readDocument(USER_CONFIG_FILE);
        
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
            writeDocument(USER_CONFIG_FILE, document);
        }
    }

    /**
     * Get the size of Scilab Main Window
     * @return the size
     * @deprecated session parameter
     */
    public static Size getMainWindowSize() {

        /* Load file */
        createUserCopy(SCILAB_CONFIG_FILE, USER_CONFIG_FILE);
        document = readDocument(USER_CONFIG_FILE);
        
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
        createUserCopy(SCILAB_CONFIG_FILE, USER_CONFIG_FILE);
        document = readDocument(USER_CONFIG_FILE);
        
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
        createUserCopy(SCILAB_CONFIG_FILE, USER_CONFIG_FILE);
        document = readDocument(USER_CONFIG_FILE);
        
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
            writeDocument(USER_CONFIG_FILE, document);
        }
    }

    /**
     * Save the size of Scilab Help Window
     * @param size the size of Scilab help Window
     * @deprecated session parameter
     */
    public static void saveHelpWindowSize(final Size size) {

        /* Load file */
        createUserCopy(SCILAB_CONFIG_FILE, USER_CONFIG_FILE);
        document = readDocument(USER_CONFIG_FILE);
        
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
            writeDocument(USER_CONFIG_FILE, document);
        }
    }

    /**
     * Get the size of Scilab Help Window
     * @return the size
     * @deprecated session parameter
     */
    public static Size getHelpWindowSize() {

        /* Load file */
        createUserCopy(SCILAB_CONFIG_FILE, USER_CONFIG_FILE);
        document = readDocument(USER_CONFIG_FILE);
        
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
        createUserCopy(SCILAB_CONFIG_FILE, USER_CONFIG_FILE);
        document = readDocument(USER_CONFIG_FILE);
        
        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allSizeElements = scilabProfile.getElementsByTagName(LASTOPENEDDIR);
            Element lastOpenedDir = (Element) allSizeElements.item(0);

            lastOpenedDir.setAttribute(VALUE, path);

            writeDocument(USER_CONFIG_FILE, document);
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
        createUserCopy(SCILAB_CONFIG_FILE, USER_CONFIG_FILE);
        document = readDocument(USER_CONFIG_FILE);
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

                writeDocument(USER_CONFIG_FILE, document);
            }
        }
        return path ;
    }
}



