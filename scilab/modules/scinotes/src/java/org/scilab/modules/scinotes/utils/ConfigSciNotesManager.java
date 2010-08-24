/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2009 - INRIA - Allan SIMON
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes.utils;

import java.awt.Color;
import java.awt.Font;
import java.awt.Toolkit;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.List;
import java.util.Hashtable;
import java.util.Properties;
import java.util.Enumeration;
import java.util.Map;
import java.util.UUID;

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

import javax.swing.KeyStroke;

import org.scilab.modules.console.GuiManagement;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

import org.scilab.modules.scinotes.ScilabView;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.TabManager;
import org.scilab.modules.scinotes.MatchingBlockManager;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

/**
 * Configuration class which interacts with the file etc/scinotesConfiguration.xml
 */
public final class ConfigSciNotesManager {
    private static final int BUFSIZE = 1024;

    private static final int MARGIN = 20;

    private static final String ERROR_READ = "Could not load file: ";
    private static final String ERROR_WRITE = "Could not save file: ";
    private static final String VALUE = "value";
    private static final String VERSION = "version";
    private static final String STYLE = "style";
    private static final String UNDERLINE = "underline";
    private static final String DEFAULTUNDERLINE = "defaultUnderline";
    private static final String STROKE = "stroke";
    private static final String DEFAULTSTROKE = "defaultStroke";
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
    private static final String AUTOINDENT = "AutoIndent";
    private static final String AUTOCOLORIZE = "AutoColorize";
    private static final String DEFAULTENCONDING = "DefaultEncoding";
    private static final String LINEHIGHLIGHTER = "LineHighlighter";
    private static final String HELPONTYPING = "HelpOnTyping";
    private static final String LINENUMBERING = "LineNumbering";
    private static final String EDITOR = "SciNotes";

    private static final String FOREGROUNDCOLOR = "ForegroundColor";
    private static final String BACKGROUNDCOLOR = "BackgroundColor";
    private static final String LINECOLOR = "linecolor";
    private static final String CONTOURCOLOR = "contourcolor";
    private static final String COLORPREFIX = "#";

    private static final String NAME = "name";
    private static final String NULL = "null";

    private static final String PROFILE = "Profile";

    private static final String RECENT_SEARCH = "recentSearch";
    private static final String SEARCH = "search";
    private static final String RECENT_REPLACE = "recentReplace";
    private static final String REPLACE = "replace";
    private static final String EXPRESSION = "exp";
    private static final String REGULAR_EXPRESION = "regularExp";
    private static final String CIRCULAR = "circularSearch";
    private static final String WORD_WARP = "wordWarp";
    private static final String WHOLE_WORD = "wholeWord";
    private static final String CASE_SENSITIVE = "caseSensitive";
    private static final String STATE_FLAG = "state";

    private static final String SETTING = "Setting";
    private static final String SCINOTES = "scinotes";
    private static final String TRUE = "true";
    private static final String FALSE = "false";
    private static final String DOCUMENT = "document";
    private static final String PATH = "path";
    private static final String RECENT_FILES = "recentFiles";
    private static final String OPEN_FILES = "openFiles";
    private static final String RESTOREFILES = "RestoreFiles";
    private static final String EDITORINST = "editorInstance";
    private static final String PANEINST = "paneInstance";
    private static final String PANEINST_EX = "paneInstanceExtra";

    private static final String FAVORITE_DIRS = "favoriteDirectories";
    private static final String DIRECTORY = "Directory";

    private static final String SCI = "SCI";
    private static final String SCINOTES_CONFIG_FILE = System.getenv(SCI) + "/modules/scinotes/etc/scinotesConfiguration.xml";
    private static final String SCINOTES_CONFIG_KEYS_FILE = System.getenv(SCI) + "/modules/scinotes/etc/keysConfiguration.xml";

    private static final String USER_SCINOTES_CONFIG_FILE = GuiManagement.getSCIHOME() + "/scinotesConfiguration.xml";
    private static final String USER_SCINOTES_CONFIG_KEYS_FILE = GuiManagement.getSCIHOME() + "/keysConfiguration.xml";

    private static final int PLAIN = 0;
    private static final int BOLD =  1;
    private static final int ITALIC = 2;
    private static final int BOLDITALIC = 3;

    private static final int DEFAULT_WIDTH = 650;
    private static final int DEFAULT_HEIGHT = 550;

    private static final int MAX_RECENT_FILES = 10;
    private static final int MAX_RECENT_SEARCH = 20;
    private static final int MAX_RECENT_REPLACE = 20;

    private static Document document;
    private static Properties keysMap;

    private static boolean updated;

    /**
     * Constructor
     */
    private ConfigSciNotesManager() {
        throw new UnsupportedOperationException();
    }

    /**
     * Create a copy of Scilab configuration file in the user directory
     */
    public static void createUserCopy() {
        if (checkVersion()) {
            /* Create a local copy of the configuration file */
            copyFile(new File(SCINOTES_CONFIG_FILE), new File(USER_SCINOTES_CONFIG_FILE));
            copyFile(new File(SCINOTES_CONFIG_KEYS_FILE), new File(USER_SCINOTES_CONFIG_KEYS_FILE));
            document = null;
            keysMap = null;
            updated = true;
        }
    }

    /**
     * Get the name of the user configuration file
     * @return the name of the configuration file
     */
    public static String getUserConfigFile() {
        return USER_SCINOTES_CONFIG_FILE;
    }

    /**
     * @return true if scinotesConfiguration.xml in etc has a version greater than the version in home
     */
    public static boolean checkVersion() {
        if (updated) {
            return false;
        }

        File fileConfig = new File(USER_SCINOTES_CONFIG_FILE);
        File keyConfig = new File(USER_SCINOTES_CONFIG_KEYS_FILE);
        if (!keyConfig.exists()) {
            return true;
        }
        if (fileConfig.exists()) {
            document = null;
            readDocument(SCINOTES_CONFIG_FILE, null);
            Node setting = getNodeChild(null, SETTING);
            String str = ((Element) setting).getAttribute(VERSION);
            if (str != null && str.length() != 0) {
                float versionEtc = Float.parseFloat(str);
                document = null;
                readDocument();
                setting = getNodeChild(null, SETTING);
                str = ((Element) setting).getAttribute(VERSION);
                document = null;

                if (str != null && str.length() != 0) {
                    float versionHome = Float.parseFloat(str);
                    return versionEtc != versionHome;
                }
            }
        }

        return true;
    }

    /**
     * Get all Style name
     * @return a array list of all style name
     */

    public static List<String> getAllStyleName() {
        List<String> stylesName = new ArrayList<String>();

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
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList fontNameElement = scinotesProfile.getElementsByTagName(FONT_NAME);
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
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList fontSizeElement = scinotesProfile.getElementsByTagName(FONT_SIZE);
        Element fontSize = (Element) fontSizeElement.item(0);
        return Integer.parseInt(fontSize.getAttribute(VALUE));
    }

    /**
     * @param type "Openers" or "Keywords"
     * @return true if help on typing for openers is active
     */
    public static boolean getHelpOnTyping(String type) {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allSizeElements = scinotesProfile.getElementsByTagName(HELPONTYPING + type);
        Element helpontyping = (Element) allSizeElements.item(0);

        return TRUE.equals(helpontyping.getAttribute(VALUE));
    }

    /**
     * Save help on typing
     * @param type "Openers" or "Keywords"
     * @param activated active or not
     */
    public static void saveHelpOnTyping(String type, boolean activated) {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allSizeElements = scinotesProfile.getElementsByTagName(HELPONTYPING + type);
        Element helpOnTyping = (Element) allSizeElements.item(0);
        if (helpOnTyping == null) {
            Element help = document.createElement(HELPONTYPING + type);
            helpOnTyping.setAttribute(VALUE, new Boolean(activated).toString());
            helpOnTyping.appendChild((Node) help);
        } else {
            helpOnTyping.setAttribute(VALUE, new Boolean(activated).toString());
        }
        writeDocument();
    }

    /**
     * @return the default state
     */
    public static int getLineNumberingState() {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allSizeElements = scinotesProfile.getElementsByTagName(LINENUMBERING);
        Element lineNumbering = (Element) allSizeElements.item(0);

        return Integer.parseInt(lineNumbering.getAttribute(VALUE));
    }

    /**
     * Save line numbering state
     * @param state the state
     */
    public static void saveLineNumberingState(int state) {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allSizeElements = scinotesProfile.getElementsByTagName(LINENUMBERING);
        Element lineNumbering = (Element) allSizeElements.item(0);
        if (lineNumbering == null) {
            Element line = document.createElement(LINENUMBERING);
            lineNumbering.setAttribute(VALUE, Integer.toString(state));
            lineNumbering.appendChild((Node) line);
        } else {
            lineNumbering.setAttribute(VALUE, Integer.toString(state));
        }
        writeDocument();
    }

    /**
     * @return true if highlighted line is active
     */
    public static boolean getHighlightState() {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allSizeElements = scinotesProfile.getElementsByTagName(LINEHIGHLIGHTER);
        Element lineHighlight = (Element) allSizeElements.item(0);

        return TRUE.equals(lineHighlight.getAttribute(VALUE));
    }

    /**
     * Save highlight state
     * @param state the state
     */
    public static void saveHighlightState(boolean state) {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allSizeElements = scinotesProfile.getElementsByTagName(LINEHIGHLIGHTER);
        Element lineHighlighter = (Element) allSizeElements.item(0);
        if (lineHighlighter == null) {
            Element line = document.createElement(LINEHIGHLIGHTER);
            lineHighlighter.setAttribute(VALUE, Boolean.toString(state));
            lineHighlighter.appendChild((Node) line);
        } else {
            lineHighlighter.setAttribute(VALUE, Boolean.toString(state));
        }
        writeDocument();
    }

    /**
     * @return the color for the highlight and for the contour of the highlight
     */
    public static Color[] getHighlightColors() {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allSizeElements = scinotesProfile.getElementsByTagName(LINEHIGHLIGHTER);
        Element lineHighlight = (Element) allSizeElements.item(0);
        Color[] arr = new Color[2];

        Color c;
        if (NULL.equals(lineHighlight.getAttribute(LINECOLOR))) {
            c = null;
        } else {
            c = Color.decode(lineHighlight.getAttribute(LINECOLOR));
        }

        arr[0] = c;

        if (NULL.equals(lineHighlight.getAttribute(CONTOURCOLOR))) {
            c = null;
        } else {
            c = Color.decode(lineHighlight.getAttribute(CONTOURCOLOR));
        }

        arr[1] = c;
        return arr;
    }

    /**
     * Get all font style
     * @return true if the font style is bold , false otherwise
     */
    public static Map<String, Boolean> getAllisBold() {
        /*load file*/
        readDocument();
        Map<String, Boolean > stylesIsBoldTable = new Hashtable<String, Boolean>();

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
     * Get all default font style
     * @return true if the font style is bold , false otherwise
     */
    public static Map<String, Boolean> getDefaultAllisBold() {
        /*load file*/
        readDocument();
        Map<String, Boolean > stylesIsBoldTable = new Hashtable<String, Boolean>();

        Element root = document.getDocumentElement();
        NodeList styles = root.getElementsByTagName(STYLE);

        for (int i = 0; i < styles.getLength(); ++i) {
            Element style = (Element) styles.item(i);


            NodeList fontStyleElement = style.getElementsByTagName(FONT_STYLE);
            Element fontStyle = (Element) fontStyleElement.item(0);
            int value = Integer.parseInt(fontStyle.getAttribute(DEFAULT));

            if (value  == BOLD || value == BOLDITALIC) {
                stylesIsBoldTable.put(style.getAttribute(NAME), true);
            } else {
                stylesIsBoldTable.put(style.getAttribute(NAME), false);
            }
        }
        return stylesIsBoldTable;
    }

    /**
     * Get all font style
     * @return true if the font style is bold , false otherwise
     */
    public static Map<String, Boolean> getAllisItalic() {
        /*load file*/
        readDocument();
        Map<String, Boolean> stylesIsItalicTable = new Hashtable<String, Boolean>();

        Element root = document.getDocumentElement();
        NodeList styles = root.getElementsByTagName(STYLE);

        for (int i = 0; i < styles.getLength(); ++i) {
            Element style = (Element) styles.item(i);

            NodeList fontStyleElement = style.getElementsByTagName(FONT_STYLE);
            Element fontStyle = (Element) fontStyleElement.item(0);
            int value = Integer.parseInt(fontStyle.getAttribute(VALUE));

            if (value  == ITALIC || value == BOLDITALIC) {
                stylesIsItalicTable.put(style.getAttribute(NAME), true);
            } else {
                stylesIsItalicTable.put(style.getAttribute(NAME), false);
            }
        }
        return stylesIsItalicTable;
    }

    /**
     * Get all default font style
     * @return true if the font style is bold , false otherwise
     */
    public static Map<String, Boolean> getDefaultAllisItalic() {
        /*load file*/
        readDocument();
        Map<String, Boolean> stylesIsItalicTable = new Hashtable<String, Boolean>();

        Element root = document.getDocumentElement();
        NodeList styles = root.getElementsByTagName(STYLE);

        for (int i = 0; i < styles.getLength(); ++i) {
            Element style = (Element) styles.item(i);

            NodeList fontStyleElement = style.getElementsByTagName(FONT_STYLE);
            Element fontStyle = (Element) fontStyleElement.item(0);
            int value = Integer.parseInt(fontStyle.getAttribute(DEFAULT));

            if (value  == ITALIC || value == BOLDITALIC) {
                stylesIsItalicTable.put(style.getAttribute(NAME), true);
            } else {
                stylesIsItalicTable.put(style.getAttribute(NAME), false);
            }
        }
        return stylesIsItalicTable;
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
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList fontSizeElement = scinotesProfile.getElementsByTagName(FONT_SIZE);
        Element fontSize = (Element) fontSizeElement.item(0);
        int size = Integer.parseInt(fontSize.getAttribute(VALUE));

        NodeList fontNameElement = scinotesProfile.getElementsByTagName(FONT_NAME);
        Element fontName = (Element) fontNameElement.item(0);
        String name = fontName.getAttribute(VALUE);

        font = new Font(name, Font.PLAIN, size);

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
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList fontSizeElement = scinotesProfile.getElementsByTagName(FONT_SIZE);
        Element fontSize = (Element) fontSizeElement.item(0);
        int size = Integer.parseInt(fontSize.getAttribute(DEFAULT));

        NodeList fontNameElement = scinotesProfile.getElementsByTagName(FONT_NAME);
        Element fontName = (Element) fontNameElement.item(0);
        String name = fontName.getAttribute(DEFAULT);

        font = new Font(name, Font.PLAIN, size);

        return font;
    }

    /**
     * Save a new font setting
     * @param font the new font
     */
    public static void saveFont(Font font) {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList fontSizeElement = scinotesProfile.getElementsByTagName(FONT_SIZE);
        Element fontSize = (Element) fontSizeElement.item(0);
        fontSize.setAttribute(VALUE, Integer.toString(font.getSize()));

        NodeList fontNameElement = scinotesProfile.getElementsByTagName(FONT_NAME);
        Element fontName = (Element) fontNameElement.item(0);
        fontName.setAttribute(VALUE, font.getName());

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

        FileOutputStream fos = null;

        try {
            fis = new FileInputStream(in);
            fos = new FileOutputStream(out);
            byte[] buf = new byte[BUFSIZE];
            int i = 0;
            while ((i = fis.read(buf)) != -1) {
                fos.write(buf, 0, i);            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (fis != null) {
                    fis.close();
                }
                if (fos != null) {
                    fos.close();
                }
            } catch (IOException e) { }
        }
    }

    /**
     * Retrieve from scinotesConfiguration.xml the infos about a tabulation
     * @return a Tabulation containing infos
     */
    public static TabManager.Tabulation getDefaultTabulation() {
        /* <style name="Tabulation" rep="vertical" value="4" white="false"> */
        readDocument();

        Element root = document.getDocumentElement();
        NodeList styles = root.getElementsByTagName(STYLE);

        for (int i = 0; i < styles.getLength(); ++i) {
            Element style = (Element) styles.item(i);
            if ("Tabulation".equals(style.getAttribute(NAME))) {
                String rep = style.getAttribute("rep").toLowerCase();
                int type = ScilabView.TABNOTHING;
                char rrep = ' ';
                if ("vertical".equals(rep)) {
                    type = ScilabView.TABVERTICAL;
                } else if ("horizontal".equals(rep)) {
                    type = ScilabView.TABHORIZONTAL;
                } else if ("doublechevrons".equals(rep)) {
                    type = ScilabView.TABDOUBLECHEVRONS;
                } else if ("none".equals(rep)) {
                    type = ScilabView.TABNOTHING;
                } else if (rep.length() >= 1) {
                    type = ScilabView.TABCHARACTER;
                    rrep = rep.charAt(0);
                }

                char one;
                int value = Integer.parseInt(style.getAttribute(VALUE));
                String white = style.getAttribute("white").toLowerCase();
                if (FALSE.equals(white)) {
                    one = '\t';
                } else {
                    one = ' ';
                }

                return new TabManager.Tabulation(one, value, type, rrep);
            }
        }
        return null;
    }

    /**
     * Retrieve form scinotesConfiguration.xml the infos the matchers
     * @param kind should be "KeywordsHighlighter" or "OpenCloseHighlighter"
     * @return an Object containing infos
     */
    public static MatchingBlockManager.Parameters getDefaultForMatcher(String kind) {
        /* <KeywordsHighlighter color="#fff3d2" inside="true" strict="false" type="filled"/> */

        readDocument();

        Element root = document.getDocumentElement();
        NodeList matching = root.getElementsByTagName("Matching");
        Element elem = (Element) matching.item(0);
        boolean onmouseover = TRUE.equals(elem.getAttribute("onmouseover"));

        NodeList mat = elem.getElementsByTagName(kind);
        Element matcher = (Element) mat.item(0);

        Color color = Color.decode(matcher.getAttribute("color"));
        boolean inside = TRUE.equals(matcher.getAttribute("inside"));
        boolean strict = TRUE.equals(matcher.getAttribute("strict"));
        boolean included = TRUE.equals(matcher.getAttribute("included"));
        String stype = matcher.getAttribute("type");
        int type = 0;
        if ("filled".equals(stype)) {
            type = MatchingBlockManager.ScilabKeywordsPainter.FILLED;
        } else if ("underlined".equals(stype)) {
            type = MatchingBlockManager.ScilabKeywordsPainter.UNDERLINED;
        } else if ("framed".equals(stype)) {
            type = MatchingBlockManager.ScilabKeywordsPainter.FRAMED;
        }

        return new MatchingBlockManager.Parameters(color, inside, strict, included, type, onmouseover);
    }

    /**
     * Get the background Color
     * @return the background Color
     */
    public static Color getSciNotesBackgroundColor() {
        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(EDITOR);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allElements = scinotesProfile.getElementsByTagName(BACKGROUNDCOLOR);
        Element scinotesBackground = (Element) allElements.item(0);

        /*direct create a Color with "#FF00FF" string from the xml */
        return Color.decode(scinotesBackground.getAttribute(VALUE));
    }

    /**
     * Get the default background Color
     * @return the default background Color
     */
    public static Color getSciNotesDefaultBackgroundColor() {
        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(EDITOR);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allElements = scinotesProfile.getElementsByTagName(BACKGROUNDCOLOR);
        Element scinotesBackground = (Element) allElements.item(0);

        /*direct create a Color with "#FF00FF" string from the xml */
        return Color.decode(scinotesBackground.getAttribute(DEFAULT));
    }

    /**
     * Get the foreground Color
     * @return the foreground Color
     */
    public static Color getSciNotesForegroundColor() {
        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(EDITOR);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allElements = scinotesProfile.getElementsByTagName(FOREGROUNDCOLOR);
        Element scinotesForeground = (Element) allElements.item(0);

        /*direct create a Color with "#FF00FF" string from the xml */
        return Color.decode(scinotesForeground.getAttribute(VALUE));
    }

    /**
     * Get the default foreground Color
     * @return the foreground Color
     */
    public static Color getSciNotesDefaultForegroundColor() {
        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(EDITOR);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allElements = scinotesProfile.getElementsByTagName(FOREGROUNDCOLOR);
        Element scinotesForeground = (Element) allElements.item(0);

        /*direct create a Color with "#FF00FF" string from the xml */
        return Color.decode(scinotesForeground.getAttribute(DEFAULT));
    }

    /**
     * Save SciNotes BackgroundColor
     * @param color the new Color
     */
    public static void saveSciNotesBackground(Color color) {

        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(EDITOR);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allSizeElements = scinotesProfile.getElementsByTagName(BACKGROUNDCOLOR);
        Element scinotesBackground = (Element) allSizeElements.item(0);

        String rgb = Integer.toHexString(color.getRGB());
        scinotesBackground.setAttribute(VALUE, COLORPREFIX + rgb.substring(2, rgb.length()));

        /* Save changes */
        writeDocument();
    }

    /**
     * Save SciNotes foregroundColor
     * @param color the new Color
     */
    public static void saveSciNotesForeground(Color color) {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(EDITOR);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allSizeElements = scinotesProfile.getElementsByTagName(FOREGROUNDCOLOR);
        Element scinotesForeground = (Element) allSizeElements.item(0);

        String rgb = Integer.toHexString(color.getRGB());
        scinotesForeground.setAttribute(VALUE, COLORPREFIX + rgb.substring(2, rgb.length()));

        /* Save changes */
        writeDocument();
    }

    /**
     * Save SciNotes autoIndent or not
     * @param activated if autoIndent should be used or not
     */
    public static void saveAutoIndent(boolean activated) {
        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allSizeElements = scinotesProfile.getElementsByTagName(AUTOINDENT);
        Element scinotesAutoIndent = (Element) allSizeElements.item(0);
        if (scinotesAutoIndent == null) {
            Element autoIndent = document.createElement(AUTOINDENT);

            autoIndent.setAttribute(VALUE, new Boolean(activated).toString());

            scinotesProfile.appendChild((Node) autoIndent);
        } else {
            scinotesAutoIndent.setAttribute(VALUE, new Boolean(activated).toString());
        }
        /* Save changes */
        writeDocument();
    }

    /**
     * @return a boolean if autoIndent should be used or not
     */
    public static boolean getAutoIndent() {
        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();
        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);
        NodeList allSizeElements = scinotesProfile.getElementsByTagName(AUTOINDENT);
        Element autoIndent = (Element) allSizeElements.item(0);

        if (autoIndent == null) {
            return true;
        } else {
            return new Boolean(autoIndent.getAttribute(VALUE));
        }
    }


    /**
     * Save SciNotes autoColorize or not
     * @param activated if autoIndent should be used or not
     */
    public static void saveAutoColorize(boolean activated) {

        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allSizeElements = scinotesProfile.getElementsByTagName(AUTOCOLORIZE);
        Element scinotesAutoIndent = (Element) allSizeElements.item(0);
        if (scinotesAutoIndent == null) {
            Element autoColorize = document.createElement(AUTOCOLORIZE);

            autoColorize.setAttribute(VALUE, new Boolean(activated).toString());

            scinotesProfile.appendChild((Node) autoColorize);
        } else {
            scinotesAutoIndent.setAttribute(VALUE, new Boolean(activated).toString());
        }
        /* Save changes */
        writeDocument();
    }


    /**
     * @return a boolean to say if the doc is autocolorize
     */
    public static boolean getAutoColorize() {
        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allSizeElements = scinotesProfile.getElementsByTagName(AUTOCOLORIZE);
        Element autoColorize = (Element) allSizeElements.item(0);

        if (autoColorize == null) {
            return true;
        } else {
            return new Boolean(autoColorize.getAttribute(VALUE));
        }
    }


    /**
     * @param encoding the default encoding for the files
     */
    public static void saveDefaultEncoding(String encoding) {
        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();
        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);
        NodeList allSizeElements = scinotesProfile.getElementsByTagName(DEFAULTENCONDING);
        Element scinotesAutoIndent = (Element) allSizeElements.item(0);
        if (scinotesAutoIndent == null) {
            Element defaultEncoding = document.createElement(DEFAULTENCONDING);

            defaultEncoding.setAttribute(VALUE, encoding);

            scinotesProfile.appendChild((Node) defaultEncoding);
        } else {
            scinotesAutoIndent.setAttribute(VALUE, encoding);
        }
        /* Save changes */
        writeDocument();
    }


    /**
     * @return the default encoding
     */
    public static String getDefaultEncoding() {
        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();
        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);
        NodeList allSizeElements = scinotesProfile.getElementsByTagName(DEFAULTENCONDING);
        Element defaultEncoding = (Element) allSizeElements.item(0);

        if (defaultEncoding == null || defaultEncoding.getAttribute(VALUE).equals("")) {
            // If no default encoding read then used system default
            saveDefaultEncoding(Charset.defaultCharset().name());
            return Charset.defaultCharset().name();
        } else {
            return defaultEncoding.getAttribute(VALUE);
        }
    }

    /**
     * Get all the foreground Colors
     * @return a Hashtable with the styles and the associated colors.
     */
    public static Map<String, Color> getAllForegroundColors() {
        /* Load file */
        readDocument();

        Map<String, Color> stylesColorsTable = new Hashtable<String, Color>();

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
     * @return a map containing styles names and the associated default font correctly derivated
     */
    public static Map<String, Font> getAllFontStyle() {
        return getAllFontStyle(getFont());
    }

    /**
     * @param f the base font
     * @return a map containing styles names and the associated font correctly derivated
     */
    public static Map<String, Font> getAllFontStyle(Font f) {
        /* Load file */
        readDocument();

        Font font = f;
        Map<String, Font> stylesFontsTable = new Hashtable<String, Font>();

        Element root = document.getDocumentElement();
        NodeList styles = root.getElementsByTagName(STYLE);

        for (int i = 0; i < styles.getLength(); ++i) {
            Element fstyles = (Element) styles.item(i);

            NodeList allFontStyleElements = fstyles.getElementsByTagName(FONT_STYLE);
            Element fontStyle = (Element) allFontStyleElements.item(0);
            int style = Integer.parseInt(fontStyle.getAttribute(VALUE));
            String name = font.getName();
            int size = font.getSize();
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

            stylesFontsTable.put(fstyles.getAttribute(NAME), font);
        }

        return stylesFontsTable;
    }


    /**
     * get all default foreground colors of scinotes
     * @return a Hashtable with the styles and the associated default colors.
     */
    public static Map<String, Color> getAllDefaultForegroundColors() {
        /* Load file */
        readDocument();

        Map<String, Color> stylesDefaultColorsTable = new Hashtable<String, Color>();

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
     * @return a map containing styles names and an integer : 0 for nothing, 1 for underline, 2 for stroke
     * and 3 for stroke+underline
     */
    public static Map<String, Integer> getAllAttributes() {
        /* Load file */
        readDocument();

        Map<String, Integer> attr = new Hashtable<String, Integer>();

        Element root = document.getDocumentElement();
        NodeList styles = root.getElementsByTagName(STYLE);

        for (int i = 0; i < styles.getLength(); ++i) {
            Element style = (Element) styles.item(i);
            int at = 0;
            String underline = style.getAttribute(UNDERLINE);
            if (TRUE.equals(underline)) {
                at = 1;
            }
            String stroke = style.getAttribute(STROKE);
            if (TRUE.equals(stroke)) {
                at += 2;
            }
            attr.put(style.getAttribute(NAME), at);
        }

        return attr;
    }

    /**
     * @return a map containing all defaults attributes
     */
    public static Map<String, Integer> getDefaultAllAttributes() {
        /* Load file */
        readDocument();

        Map<String, Integer> attr = new Hashtable<String, Integer>();

        Element root = document.getDocumentElement();
        NodeList styles = root.getElementsByTagName(STYLE);

        for (int i = 0; i < styles.getLength(); ++i) {
            Element style = (Element) styles.item(i);
            int at = 0;
            String underline = style.getAttribute(DEFAULTUNDERLINE);
            if (TRUE.equals(underline)) {
                at = 1;
            }
            String stroke = style.getAttribute(DEFAULTSTROKE);
            if (TRUE.equals(stroke)) {
                at += 2;
            }
            attr.put(style.getAttribute(NAME), at);
        }

        return attr;
    }

    /**
     * save all foreground colors
     *@param stylesColorsTable a hashtable containing styles and the associated colors
     */
    public static void saveAllForegroundColors(Map<String, Color> stylesColorsTable) {
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
     * save all style for the font
     * @param boldTable a hashtable containing style names and a boolean for the bold style
     * @param italicTable a hashtable containing style names and a boolean for the italic style
     */
    public static void saveAllFontStyle(Map<String, Boolean> boldTable, Map<String, Boolean> italicTable) {
        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();
        NodeList styles = root.getElementsByTagName(STYLE);

        for (int i = 0; i < styles.getLength(); ++i) {
            Element style = (Element) styles.item(i);

            String styleName = style.getAttribute(NAME);
            NodeList fontStyleElements = style.getElementsByTagName(FONT_STYLE);
            Element fontStyle = (Element) fontStyleElements.item(0);

            int bold = 1;
            if (!boldTable.get(styleName)) {
                bold = 0;
            }
            int italic = 2;
            if (!italicTable.get(styleName)) {
                italic = 0;
            }

            fontStyle.setAttribute(VALUE, Integer.toString(bold + italic));
        }
        /* Save changes */
        writeDocument();
    }

    /**
     * save all style for the font
     * @param attTable a hashtable containing styles and the associated attribute
     */
    public static void saveAllAttributes(Map<String, Integer> attTable) {
        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();
        NodeList styles = root.getElementsByTagName(STYLE);

        for (int i = 0; i < styles.getLength(); ++i) {
            Element style = (Element) styles.item(i);

            String styleName = style.getAttribute(NAME);

            String underline = TRUE;
            if ((attTable.get(styleName) & 1) != 1) {
                underline = FALSE;
            }
            String stroke = TRUE;
            if ((attTable.get(styleName) & 2) != 2) {
                stroke = FALSE;
            }

            style.setAttribute(UNDERLINE, underline);
            style.setAttribute(STROKE, stroke);
        }
        /* Save changes */
        writeDocument();
    }

    /**
     * Get the position of SciNotes Main Window
     * @return the position
     */
    public static Position getMainWindowPosition() {
        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allPositionElements = scinotesProfile.getElementsByTagName(MAINWINPOSITION);
        Element mainWindowPosition = (Element) allPositionElements.item(0);
        if (mainWindowPosition != null) {
            int x = Integer.parseInt(mainWindowPosition.getAttribute(XCOORD));
            int y = Integer.parseInt(mainWindowPosition.getAttribute(YCOORD));
            /* Avoid SciNotes Main Window to be out of the screen */
            if (x <= (Toolkit.getDefaultToolkit().getScreenSize().width - MARGIN)
                && y <= (Toolkit.getDefaultToolkit().getScreenSize().height - MARGIN)) {
                return new Position(x, y);
            } else {
                return new Position(0, 0);
            }
        } else {
            return new Position(0, 0);
        }
    }


    /**
     * Save the position of SciNotes Main Window
     * @param position the position of SciNotes main Window
     */
    public static void saveMainWindowPosition(Position position) {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allPositionElements = scinotesProfile.getElementsByTagName(MAINWINPOSITION);
        Element mainWindowPosition = (Element) allPositionElements.item(0);


        mainWindowPosition.setAttribute(XCOORD, Integer.toString(position.getX()));
        mainWindowPosition.setAttribute(YCOORD, Integer.toString(position.getY()));

        /* Save changes */
        writeDocument();
    }


    /**
     * Save the size of SciNotes Main Window
     * @param size the size of SciNotes main Window
     */
    public static void saveMainWindowSize(Size size) {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allPositionElements = scinotesProfile.getElementsByTagName(MAINWINSIZE);
        Element mainWindowSize = (Element) allPositionElements.item(0);


        mainWindowSize.setAttribute(WIDTH, Integer.toString(size.getWidth()));
        mainWindowSize.setAttribute(HEIGHT, Integer.toString(size.getHeight()));

        /* Save changes */
        writeDocument();
    }

    /**
     * Get the size of SciNotes Main Window
     * @return the size
     */
    public static Size getMainWindowSize() {

        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allSizeElements = scinotesProfile.getElementsByTagName(MAINWINSIZE);
        Element mainWindowSize = (Element) allSizeElements.item(0);
        if (mainWindowSize != null) {
            return new Size(Integer.parseInt(mainWindowSize.getAttribute(WIDTH)), Integer.parseInt(mainWindowSize.getAttribute(HEIGHT)));
        } else {
            return new Size(DEFAULT_WIDTH, DEFAULT_HEIGHT);
        }
    }

    /**
     * Get all the recent opened files
     * @return a array of uri
     */
    public static List<File> getAllRecentOpenedFiles() {
        List<File> files = new ArrayList<File>();
        readDocument();
        Element root = (Element) document.getDocumentElement().getElementsByTagName(RECENT_FILES).item(0);
        if (root != null) {
            NodeList recentFiles = root.getElementsByTagName(DOCUMENT);
            for (int i = 0; i < recentFiles.getLength(); ++i) {
                Element style = (Element) recentFiles.item(i);

                File temp = new File(style.getAttribute(PATH));

                if (temp.exists()) {
                    files.add(temp);
                } else {
                    root.removeChild((Node) style);
                }
            }
        }

        /* Save changes */
        writeDocument();

        return files;
    }

    /**
     * Get all the favorite dirs
     * @return a list of File
     */
    public static List<File> getAllFavoriteDirs() {
        List<File> dirsList = new ArrayList<File>();
        readDocument();
        Element root = (Element) document.getDocumentElement().getElementsByTagName(FAVORITE_DIRS).item(0);
        if (root != null) {
            NodeList dirs = root.getElementsByTagName(DIRECTORY);
            for (int i = 0; i < dirs.getLength(); i++) {
                Element dir = (Element) dirs.item(i);
                File temp = new File(dir.getAttribute(PATH));

                if (temp.exists()) {
                    dirsList.add(temp);
                } else {
                    root.removeChild((Node) dir);
                }
            }
        }

        /* Save changes */
        writeDocument();

        return dirsList;
    }

    /**
     * Add a path to a favorite directory
     * @param path the path of the dir
     */
    public static void saveFavoriteDirectory(String path) {
        readDocument();

        Element root = (Element) document.getDocumentElement().getElementsByTagName(FAVORITE_DIRS).item(0);
        Element newDir =  document.createElement(DIRECTORY);
        newDir.setAttribute(PATH, path);
        root.appendChild((Node) newDir);

        /* Save changes */
        writeDocument();
    }

    /**
     * Remove the last favorite directory
     */
    public static void rmLastFavoriteDirectory() {
        readDocument();

        Element root = (Element) document.getDocumentElement().getElementsByTagName(FAVORITE_DIRS).item(0);
        NodeList dirs = root.getElementsByTagName(DIRECTORY);

        if (dirs.getLength() != 0) {
            root.removeChild(dirs.item(dirs.getLength() - 1));
        }

        /* Save changes */
        writeDocument();
    }

    /**
     * Add a file to recent Opened Files
     * @param filePath the path of the files to add
     */
    public static void saveToRecentOpenedFiles(String filePath) {
        readDocument();

        Element root = (Element) document.getDocumentElement().getElementsByTagName(RECENT_FILES).item(0);
        NodeList recentFiles = root.getElementsByTagName(DOCUMENT);
        int numberOfFiles = recentFiles.getLength();

        // we remove all the duplicate
        for (int i = 0; i < recentFiles.getLength();  ++i) {
            Element style = (Element) recentFiles.item(i);
            if (filePath.equals(style.getAttribute(PATH))) {
                root.removeChild((Node) style);
                numberOfFiles--;
            }
        }

        // if we have reached the maximun , we remove the oldest files
        while (recentFiles.getLength() >= MAX_RECENT_FILES) {
            root.removeChild(root.getFirstChild());
        }

        Element newFile =  document.createElement(DOCUMENT);
        newFile.setAttribute(PATH, filePath);
        root.appendChild((Node) newFile);

        /* Save changes */
        writeDocument();
    }

    /**
     * @return true if open files should be restored upon restart.
     */
    public static boolean getRestoreOpenedFiles() {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allSizeElements = scinotesProfile.getElementsByTagName(RESTOREFILES);
        Element restorefiles = (Element) allSizeElements.item(0);

        return TRUE.equals(restorefiles.getAttribute(VALUE));
    }

    /**
     * Active/deactive restoration of open files upon restart of scinotes
     * @param activated active or not
     */
    public static void saveRestoreOpenedFiles(boolean activated) {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allSizeElements = scinotesProfile.getElementsByTagName(RESTOREFILES);
        Element restorefiles = (Element) allSizeElements.item(0);
        if (restorefiles == null) {
            Element restoreElement = document.createElement(RESTOREFILES);
            restorefiles.setAttribute(VALUE, new Boolean(activated).toString());
            restorefiles.appendChild((Node) restoreElement);
        } else {
            restorefiles.setAttribute(VALUE, new Boolean(activated).toString());
        }
        writeDocument();
    }

    /**
     * Return a count of the open files that exist. New files, for instance, do not.
     * @return count
     */
    public static int countExistingOpenFiles() {
        int count = 0;
        readDocument();
        Element root = (Element) document.getDocumentElement().getElementsByTagName(OPEN_FILES).item(0);
        if (root != null) {
            NodeList openFiles = root.getElementsByTagName(DOCUMENT);

            for (int i = 0; i < openFiles.getLength(); ++i) {
                Element style = (Element) openFiles.item(i);

                File temp = new File(style.getAttribute(PATH));

                if (temp.exists()) {
                    count++;
                }
                writeDocument();
            }
        }
        return count;
    }

    /**
     * Get the list of open files associated with an editor instance hashcode.
     * Only files that exist are returned.
     * @param editorID unique id of an editor instance
     * @return a array of uri
     */
    public static List<File> getOpenFilesByEditor(UUID editorID) {
        List<File> files = new ArrayList<File>();
        readDocument();
        Element root = (Element) document.getDocumentElement().getElementsByTagName(OPEN_FILES).item(0);
        if (root != null) {
            NodeList openFiles = root.getElementsByTagName(DOCUMENT);

            /* Loop through the list and return only the files with a matching hash code. */
            int i = 0;
            for (; i < openFiles.getLength(); i++) {
                Element style = (Element) openFiles.item(i);

                if (editorID.equals(UUID.fromString(style.getAttribute(EDITORINST)))) {
                    File temp = new File(style.getAttribute(PATH));

                    /* Check that the file exists and add to file list or else remove the node. */
                    if (temp.exists()) {
                        files.add(temp);
                    } else {
                        root.removeChild((Node) style);
                        i--;  // Adjust index to account for removed item.
                    }
                }
            }
            /* Save any changes */
            writeDocument();
        }
        return files;
    }

    /**
     * Get a list of unique editor instance identifiers in the list of open files.
     * @return an array of editor instance identifiers
     */
    public static List<UUID> getOpenFilesEditorList() {
        List<UUID> editorIDlist = new ArrayList<UUID>();
        readDocument();
        Element root = (Element) document.getDocumentElement().getElementsByTagName(OPEN_FILES).item(0);
        if (root != null) {
            NodeList openFiles = root.getElementsByTagName(DOCUMENT);

            /* Loop through the list and return the list of IDs. */
            for (int i = 0; i < openFiles.getLength(); ++i) {
                Element style = (Element) openFiles.item(i);

                UUID editorID = UUID.fromString(style.getAttribute(EDITORINST));

                if (!editorIDlist.contains(editorID)) {
                    editorIDlist.add(editorID);
                }
            }
        }
        return editorIDlist;
    }

    /**
     * Add a file to currently open files
     * @param filePath the path of the files to add
     * @param editorInstance instance of the editor to associate with the open file
     * @param sep the pane
     */
    public static void saveToOpenFiles(String filePath, SciNotes editorInstance, ScilabEditorPane sep) {
        readDocument();
        UUID nil = new UUID(0, 0);

        // Find the element containing the list of open files
        Element root = (Element) document.getDocumentElement().getElementsByTagName(OPEN_FILES).item(0);
        // Get the list of open files
        NodeList openFiles = root.getElementsByTagName(DOCUMENT);
        int numberOfFiles = openFiles.getLength();

        Element newFile =  document.createElement(DOCUMENT);
        newFile.setAttribute(PATH, filePath);
        // Record the editor instance's hash code
        newFile.setAttribute(EDITORINST, editorInstance.getUUID().toString());
        root.appendChild((Node) newFile);
        // Record the text pane's hash code
        newFile.setAttribute(PANEINST, sep.getUUID().toString());
        newFile.setAttribute(PANEINST_EX, nil.toString());
        root.appendChild((Node) newFile);

        /* Save changes */
        writeDocument();
    }

    /**
     * Remove a tab with an open file from the list of open files
     * @param editorInstance instance of the editor
     * @param sep instance of the editor pane.
     */
    public static void removeFromOpenFiles(SciNotes editorInstance, ScilabEditorPane sep) {
        removeFromOpenFiles(editorInstance.getUUID(), sep.getUUID());
    }

    /**
     * Remove from the list of open files all files with a matching editor instance identifer
     * @param editorID editor instance identifer
     */
    public static void removeFromOpenFiles(UUID editorID) {
        removeFromOpenFiles(editorID, new UUID(0, 0) /* nil UUID */);
    }

    /**
     * Remove a tab with an open file from the list of open files
     * @param editorID editor instance identifer
     * @param sepID editor pane instance identifer. If a nil UUID is passed,
     * all files with a matching editor instance identifer are removed.
     */
    public static void removeFromOpenFiles(UUID editorID, UUID sepID) {
        readDocument();

        Element root = (Element) document.getDocumentElement().getElementsByTagName(OPEN_FILES).item(0);
        NodeList openFiles = root.getElementsByTagName(DOCUMENT);

        // Remove item with matching editorID and sepID.
        UUID nil = new UUID(0, 0);
        for (int i = openFiles.getLength() - 1; i >= 0; i--) {
            Element style = (Element) openFiles.item(i);
            UUID paneID1 = UUID.fromString(style.getAttribute(PANEINST));
            UUID paneID2 = UUID.fromString(style.getAttribute(PANEINST_EX));

            if (editorID.equals(UUID.fromString(style.getAttribute(EDITORINST)))
                && (sepID.equals(nil) || sepID.equals(paneID1) || sepID.equals(paneID2))) {
                root.removeChild((Node) style);
            }
        }

        /* Save changes */
        writeDocument();
    }

    /**
     * Change a filename.
     * @param newfilePath new pathname of the file
     * @param editorInstance instance of the editor
     * @param sep instance of the editor pane
     */
    public static void renameOpenFilesItem(String newfilePath, SciNotes editorInstance, ScilabEditorPane sep) {
        readDocument();

        Element root = (Element) document.getDocumentElement().getElementsByTagName(OPEN_FILES).item(0);
        Element style = findOpenFileItem(root, editorInstance.getUUID(), sep.getUUID());

        if (style != null) {
            style.setAttribute(PATH, newfilePath);
        }

        /* Save changes */
        writeDocument();
    }

    /**
     * Replace a single text pane ID with two pane IDs when a tab split occurs
     * @param editorInstance instance of the editor
     * @param old1 old instance of the editor pane
     * @param new1 first new instance of the tabbed editor pane
     * @param new2 second new instance of the tabbed editor pane
     */
    public static void tabSplitOpenFilesItem(SciNotes editorInstance, ScilabEditorPane old1, ScilabEditorPane new1, ScilabEditorPane new2) {
        readDocument();

        Element root = (Element) document.getDocumentElement().getElementsByTagName(OPEN_FILES).item(0);
        Element style = findOpenFileItem(root, editorInstance.getUUID(), old1.getUUID());

        if (style != null) {
            style.setAttribute(PANEINST, new1.getUUID().toString());
            style.setAttribute(PANEINST_EX, new2.getUUID().toString());
        }

        /* Save changes */
        writeDocument();
    }

    /**
     * Replace double pane IDs with a single ID when a tabbed pane is replaced by a single pane.
     * @param editorInstance instance of the editor
     * @param old1 one of the old tabbed editor pane
     * @param new1 new editor pane
     */
    public static void removeTabSplitInOpenFilesItem(SciNotes editorInstance, ScilabEditorPane old1, ScilabEditorPane new1) {
        readDocument();

        Element root = (Element) document.getDocumentElement().getElementsByTagName(OPEN_FILES).item(0);
        Element style = findOpenFileItem(root, editorInstance.getUUID(), old1.getUUID());

        if (style != null) {
            UUID nil = new UUID(0, 0);
            style.setAttribute(PANEINST, new1.getUUID().toString());
            style.setAttribute(PANEINST_EX, nil.toString());
        }

        /* Save changes */
        writeDocument();
    }

    /**
     * Find the first element with matching editor and pane identifiers
     * @param root Document root
     * @param editorID instance of the editor to find
     * @param sepID instance of the editor pane to find
     * @return the corresponding element
     */
    public static Element findOpenFileItem(Element root, UUID editorID, UUID sepID) {
        NodeList openFiles = root.getElementsByTagName(DOCUMENT);

        // Find item with matching editor and pane IDs
        for (int i = 0; i < openFiles.getLength(); i++) {
            Element style = (Element) openFiles.item(i);
            UUID paneID1 = UUID.fromString(style.getAttribute(PANEINST));
            UUID paneID2 = UUID.fromString(style.getAttribute(PANEINST_EX));

            if (editorID.equals(UUID.fromString(style.getAttribute(EDITORINST)))
                && (sepID.equals(paneID1) || sepID.equals(paneID2))) {
                return style;
            }
        }

        return null;
    }

    /**
     * Empty the list of open files. Performed when the editor is opened
     * and the user opts not to restore the open files.
     */
    public static void removeAllOpenFiles() {
        readDocument();

        Element root = (Element) document.getDocumentElement().getElementsByTagName(OPEN_FILES).item(0);
        NodeList openFiles = root.getElementsByTagName(DOCUMENT);

        // Remove all
        for (int i = openFiles.getLength() - 1; i >= 0;  --i) {
            Element style = (Element) openFiles.item(i);
            root.removeChild((Node) style);
        }
        /* Save changes */
        writeDocument();
    }

    /**
     * Reset the current conf file
     */
    public static void resetDocument() {
        document = null;
    }

    /**
     * Read the file to modify
     */
    private static void readDocument() {
        readDocument(USER_SCINOTES_CONFIG_FILE, USER_SCINOTES_CONFIG_KEYS_FILE);
    }

    /**
     * Read the file to modify
     */
    private static void readDocument(String pathConfSci, String pathConfKeys) {
        File xml = null;
        DocumentBuilder docBuilder = null;

        try {
            if (document == null) {
                DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
                docBuilder = factory.newDocumentBuilder();

                // read content of a XML file with DOM
                xml = new File(pathConfSci);
                document = docBuilder.parse(xml);
            }
        } catch (ParserConfigurationException pce) {
            System.err.println(ERROR_READ + pathConfSci);
        } catch (SAXException se) {
            System.err.println(ERROR_READ + pathConfSci);
        } catch (IOException ioe) {
            System.err.println(ERROR_READ + pathConfSci);
        }

        FileInputStream fis = null;

        try {
            if (keysMap == null && pathConfKeys != null) {
                xml = new File(pathConfKeys);
                keysMap = new Properties();
                fis = new FileInputStream(xml);
                keysMap.loadFromXML(fis);
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (fis != null) {
                    fis.close();
                }
            } catch (IOException e) { }
        }
    }

    /**
     * @param map the map to fill with the pairs (keystroke, action) found in file etc/keysConfiguration.xml
     */
    public static void addMapActionNameKeys(Map map) {
        for (Enumeration action = keysMap.propertyNames(); action.hasMoreElements();) {
            String name = (String) action.nextElement();
            KeyStroke ks = KeyStroke.getKeyStroke(keysMap.getProperty(name));
            map.put(name, ks);
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
            System.out.println(ERROR_WRITE + USER_SCINOTES_CONFIG_FILE);
        } catch (TransformerFactoryConfigurationError e1) {
            System.out.println(ERROR_WRITE + USER_SCINOTES_CONFIG_FILE);
        }
        transformer.setOutputProperty(OutputKeys.INDENT, "yes");

        StreamResult result = new StreamResult(new File(USER_SCINOTES_CONFIG_FILE));
        DOMSource source = new DOMSource(document);
        try {
            transformer.transform(source, result);
        } catch (TransformerException e) {
            System.out.println(ERROR_WRITE + USER_SCINOTES_CONFIG_FILE);
        }
    }

    /**
     * Add a file to recent Opened Files
     * @param exp the path of the files to add
     */
    public static void saveRecentSearch(String exp) {
        Node root = getXcosRoot();
        if (root == null || exp == null || exp.compareTo("") == 0) {
            return;
        }

        Node recents = getNodeChild(root, RECENT_SEARCH);
        if (recents == null) {
            recents = document.createElement(RECENT_SEARCH);
            root.appendChild(recents);
        }

        List<Node> search = getNodeChildren(recents, SEARCH);

        while (search.size() >= MAX_RECENT_SEARCH) {
            removeRecentSearch(((Element) search.get(0)).getAttribute(EXPRESSION));
            search = getNodeChildren(recents, SEARCH);
        }
        //if path already in file no need to add it
        for (Node item : search) {
            if (exp.compareTo(((Element) item).getAttribute(EXPRESSION)) == 0) {
                return;
            }
        }

        Element newSearch = document.createElement(SEARCH);
        newSearch.setAttribute(EXPRESSION, exp);
        recents.appendChild((Node) newSearch);

        /* Save changes */
        writeDocument();
    }

    /**
     * @param exp the expression to remove
     */
    public static void removeRecentSearch(String exp) {
        Node root = getXcosRoot();
        if (root == null) {
            return;
        }

        Node recent = getNodeChild(root, RECENT_SEARCH);
        List<Node> search = getNodeChildren(recent, SEARCH);

        // remove node if exists
        for (Node file : search) {
            if (exp.compareTo(((Element) file).getAttribute(EXPRESSION)) == 0) {
                recent.removeChild(file);
                break;
            }
        }
        /* Save changes */
        writeDocument();

    }

    /**
     * @return a list of the recent searches
     */
    public static List<String> getRecentSearch() {
        List<String> files = new ArrayList<String>();

        Node root = getXcosRoot();
        if (root == null) {
            return files;
        }

        Node recent = getNodeChild(root, RECENT_SEARCH);
        List<Node> searches = getNodeChildren(recent, SEARCH);
        for (Node search : searches) {
            String exp = ((Element) search).getAttribute(EXPRESSION);
            if (exp != null && exp.compareTo("") != 0) {
                files.add(exp);
            }
        }

        return files;
    }

    /**
     * @param exp the recent expression for a replacement
     */
    public static void saveRecentReplace(String exp) {
        Node root = getXcosRoot();
        if (root == null || exp == null || exp.compareTo("") == 0) {
            return;
        }

        Node recent = getNodeChild(root, RECENT_REPLACE);
        if (recent == null) {
            recent = document.createElement(RECENT_REPLACE);
            root.appendChild(recent);
        }

        List<Node> replace = getNodeChildren(recent, REPLACE);

        while (replace.size() >= MAX_RECENT_REPLACE) {
            removeRecentReplace(((Element) replace.get(0)).getAttribute(EXPRESSION));
            replace = getNodeChildren(recent, REPLACE);
        }
        //if path already in file no need to add it
        for (Node item : replace) {
            if (exp.compareTo(((Element) item).getAttribute(EXPRESSION)) == 0) {
                return;
            }
        }

        Element newReplace = document.createElement(REPLACE);
        newReplace.setAttribute(EXPRESSION, exp);
        recent.appendChild((Node) newReplace);

        /* Save changes */
        writeDocument();
    }

    /**
     * @param filePath remove recent replace in the this file
     */
    public static void removeRecentReplace(String filePath) {

        Node root = getXcosRoot();
        if (root == null) {
            return;
        }

        Node recent = getNodeChild(root, RECENT_REPLACE);
        List<Node> replace = getNodeChildren(recent, REPLACE);

        // remove node if exists
        for (Node exp : replace) {
            if (filePath.compareTo(((Element) exp).getAttribute(EXPRESSION)) == 0) {
                recent.removeChild(exp);
                break;
            }

        }
        /* Save changes */
        writeDocument();

    }

    /**
     * @return the recent replace
     */
    public static List<String> getRecentReplace() {
        List<String> exps = new ArrayList<String>();

        Node root = getXcosRoot();
        if (root == null) {
            return exps;
        }

        Node recent = getNodeChild(root, RECENT_REPLACE);
        List<Node> replace = getNodeChildren(recent, REPLACE);
        for (Node file : replace) {
            String exp = ((Element) file).getAttribute(EXPRESSION);
            if (exp != null && exp.compareTo("") != 0) {
                exps.add(exp);
            }
        }

        return exps;
    }

    /**
     * @return true for a regexp search
     */
    public static boolean getRegularExpression() {
        return getBooleanAttribute(REGULAR_EXPRESION, STATE_FLAG, false);
    }

    /**
     * @param regualExp for a regexp search
     */
    public static void saveRegularExpression(boolean regualExp) {
        saveBooleanAttribute(REGULAR_EXPRESION, STATE_FLAG, regualExp);
    }

    /**
     * @return true for a wholeWord search
     */
    public static boolean getWholeWord() {
        return getBooleanAttribute(WHOLE_WORD, STATE_FLAG, false);
    }

    /**
     * @param wholeWord for a wholeWord search
     */
    public static void saveWholeWord(boolean wholeWord) {
        saveBooleanAttribute(WHOLE_WORD, STATE_FLAG, wholeWord);
    }

    /**
     * @return true for a circular search
     */
    public static boolean getCircularSearch() {
        return getBooleanAttribute(CIRCULAR, STATE_FLAG, true);
    }

    /**
     * @param circular is true for a circular search
     */
    public static void saveCircularSearch(boolean circular) {
        saveBooleanAttribute(CIRCULAR, STATE_FLAG, circular);
    }

    /**
     * @return true for a case sensitive search
     */
    public static boolean getCaseSensitive() {
        return getBooleanAttribute(CASE_SENSITIVE, STATE_FLAG, false);
    }

    /**
     * @param caseSensitive for a case sensitive search
     */
    public static void saveCaseSensitive(boolean caseSensitive) {
        saveBooleanAttribute(CASE_SENSITIVE, STATE_FLAG, caseSensitive);
    }

    /**
     * getBooleanAttribute
     * @param node the node name
     * @param attrib the attribute of the node
     * @param defaultValue true or false
     * @return the found boolean value or defaultValue if not found
     */
    private static boolean getBooleanAttribute(String node, String attrib, boolean defaultValue) {
        boolean flag = false;
        Node root = getXcosRoot();
        if (root == null) {
            return flag;
        }
        Node recent = getNodeChild(root, node);
        if (recent != null) {
            String exp = ((Element) recent).getAttribute(attrib);
            if (exp.compareTo(TRUE) == 0) {
                flag = true;
            }
        } else {
            return defaultValue;
        }
        return flag;
    }

    /**
     * saveBooleanAttribute
     * @param node the node name
     * @param attrib the attribute of the node
     * @param state "true" or "false"
     */
    private static void saveBooleanAttribute(String node, String attrib, boolean state) {
        Node root = getXcosRoot();
        if (root == null) {
            return;
        }

        Node recent = getNodeChild(root, node);
        if (recent == null) {
            recent = document.createElement(node);
            root.appendChild(recent);
        }


        ((Element) recent).setAttribute(attrib, new Boolean(state).toString());

        root.appendChild((Node) recent);

        /* Save changes */
        writeDocument();
    }

    /**
     * getNodeChild
     * @param par parent
     * @param nodeName the name
     * @return the node
     */
    private static Node getNodeChild(Node par, String nodeName) {
        Node parent = par;
        if (parent == null) {
            if (document == null) {
                readDocument();
                if (document == null) {
                    return null;
                }
            }
            parent = document;
        }

        Node currentNode = parent.getFirstChild();
        while (currentNode != null) {
            if (currentNode.getNodeName().compareTo(nodeName) == 0) {
                return currentNode;
            }
            currentNode = currentNode.getNextSibling();
        }
        return currentNode;
    }

    /**
     * getNodeChildren
     * @param par parent
     * @param childName the name
     * @return a list of nodes
     */
    private static List<Node> getNodeChildren(Node par, String childName) {
        Node parent = par;
        List<Node> nodes = new ArrayList<Node>();
        if (parent == null) {
            if (document == null) {
                readDocument();
                if (document == null) {
                    return nodes;
                }
            }
            parent = document;
        }

        Node currentNode = parent.getFirstChild();
        while (currentNode != null) {
            if (currentNode.getNodeName().compareTo(childName) == 0) {
                nodes.add(currentNode);
            }
            currentNode = currentNode.getNextSibling();
        }
        return nodes;

    }

    /**
     * @return the root
     */
    private static Node getXcosRoot() {
        if (document == null) {
            readDocument();
            if (document == null) {
                return null;
            }
        }

        Node setting = getNodeChild(null, SETTING);

        if (setting != null) {
            List<Node> nodes = getNodeChildren(setting, PROFILE);
            for (Node node : nodes) {
                if (((Element) node).getAttribute(NAME).compareTo(SCINOTES) == 0) {
                    return node;
                }
            }
        }
        return null;
    }
}
