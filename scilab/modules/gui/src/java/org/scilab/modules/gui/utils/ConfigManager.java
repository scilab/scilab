/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.gui.utils;

import java.awt.Color;
import java.awt.Font;
import java.io.File;

import org.scilab.modules.commons.ScilabCommonsUtils;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.xml.ScilabXMLUtilities;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

/**
 * Management of the Console configuration file
 * @author Vincent COUVERT
 */
public final class ConfigManager {

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
    private static final String HELPBROWSER = "HelpBrowser";
    private static final String PROFILE = "Profile";
    private static final String FOREGROUNDCOLOR = "ForegroundColor";
    private static final String BACKGROUNDCOLOR = "BackgroundColor";
    private static final String COLORPREFIX = "#";
    private static final String MAXOUTPUTSIZE = "MaxOutputSize";
    private static final String LASTOPENEDDIR = "LastOpenedDirectory";
    private static final String INDEX = "index";

    private static final String SCILAB_CONFIG_FILE = System.getenv("SCI") + "/modules/console/etc/configuration.xml";

    private static String USER_CONFIG_FILE = ScilabConstants.SCIHOME.toString() + "/configuration.xml";

    private static final int DEFAULT_WIDTH = 650;
    private static final int DEFAULT_HEIGHT = 550;

    private static final int DEFAULT_MAXOUTPUTSIZE = 10000;
    private static final int DEFAULT_HELPFONTSIZE = 2;

    private static Document document;

    private static boolean updated;
    private static boolean mustSave = true;

    static {
        if (ScilabConstants.SCIHOME != null && ScilabConstants.SCIHOME.canRead() && ScilabConstants.SCIHOME.canWrite()) {
            USER_CONFIG_FILE = ScilabConstants.SCIHOME.toString() + "/configuration.xml";
        } else {
            USER_CONFIG_FILE = SCILAB_CONFIG_FILE;
            mustSave = false;
        }
    }

    /**
     * Constructor
     */
    private ConfigManager() {
        throw new UnsupportedOperationException();
    }

    /**
     * Create a copy of Scilab configuration file in the user directory
     */
    public static void createUserCopy() {
        if (mustSave) {
            File fileConfig = new File(USER_CONFIG_FILE);
            if (!fileConfig.exists() || (fileConfig.length() == 0) || checkVersion()) {
                /* Create a local copy of the configuration file */
                updated = ScilabCommonsUtils.copyFile(new File(SCILAB_CONFIG_FILE), new File(USER_CONFIG_FILE));
            }
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
            Document doc = ScilabXMLUtilities.readDocument(USER_CONFIG_FILE);
            Element setting = doc.getDocumentElement();
            String str = setting.getAttribute(VERSION);
            if (str != null && str.length() != 0) {
                float userVersion = Float.parseFloat(str);
                doc = ScilabXMLUtilities.readDocument(SCILAB_CONFIG_FILE);
                setting = doc.getDocumentElement();
                str = setting.getAttribute(VERSION);

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

    /**
     * Save a new font setting
     * @param font the new font
     */
    public static void saveFont(Font font) {
        readDocument();

        if (document != null) {
            ScilabXMLUtilities.replaceNamedNode(document, PROFILE, "FontSize", new Object[] {VALUE, font.getSize()});
            ScilabXMLUtilities.replaceNamedNode(document, PROFILE, "FontName", new Object[] {VALUE, font.getFontName()});

            String style;
            if (!font.isBold() && !font.isItalic()) {
                style = "0";
            } else if (font.isBold() && font.isItalic()) {
                style = "3";
            } else if (font.isBold()) {
                style = "1";
            } else {
                style = "2";
            }
            ScilabXMLUtilities.replaceNamedNode(document, PROFILE, "FontStyle", new Object[] {VALUE, style});

            writeDocument();
        }
    }

    /**
     * Get the maximum number of lines to keep in the output
     * @return the nulber of lines
     */
    public static int getMaxOutputSize() {
        readDocument();

        if (document != null) {
            Object[] attr = new Object[] {VALUE, int.class};
            Element elem = ScilabXMLUtilities.readNodeAttributes(document, MAXOUTPUTSIZE, attr);

            if (elem != null) {
                return ((Integer) attr[1]).intValue();
            }
        }

        return DEFAULT_MAXOUTPUTSIZE;
    }

    /**
     * Get the font size in the help viewer
     * @return the font size
     */
    public static int getHelpFontSize() {
        readDocument();

        if (document != null) {
            Object[] attr = new Object[] {VALUE, int.class};
            Element elem = ScilabXMLUtilities.readNodeAttributes(document, HELPFONTSIZE, attr);

            if (elem != null) {
                return ((Integer) attr[1]).intValue();
            }
        }

        return DEFAULT_HELPFONTSIZE;
    }

    /**
     * Get the font size in the help viewer
     * @return the font size
     */
    public static void setHelpFontSize(int size) {
        readDocument();

        if (document != null) {
            ScilabXMLUtilities.replaceNamedNode(document, PROFILE, HELPFONTSIZE, new Object[] {VALUE, size});
            writeDocument();
        }
    }

    /**
     * Save the help browser current page
     * @param index the address
     */
    public static void saveHelpBrowserState(String index) {
        readDocument();

        if (document != null) {
            ScilabXMLUtilities.replaceNamedNode(document, PROFILE, HELPBROWSER, new Object[] {INDEX, index});
            writeDocument();
        }
    }

    /**
     * Get the previous index to restore
     * @return the previous index
     */
    public static String getHelpBrowserState() {
        readDocument();

        if (document != null) {
            Object[] attr = new Object[] {INDEX, String.class};
            Element elem = ScilabXMLUtilities.readNodeAttributes(document, HELPBROWSER, attr);

            if (elem != null) {
                return (String) attr[1];
            }
        }

        return null;
    }

    /**
     * Save the Last Opened Directory in Scilab
     * @param the directory's path
     */
    public static void saveLastOpenedDirectory(String path ) {
        readDocument();

        if (document != null) {
            ScilabXMLUtilities.replaceNamedNode(document, PROFILE, LASTOPENEDDIR, new Object[] {VALUE, path});
            writeDocument();
        }
    }

    /**
     * Get the Last Opened Directory in Scilab
     * @return the directory's path
     */

    public static String getLastOpenedDirectory() {
        /*System.getProperty("user.dir") if no path*/
        readDocument();
        String path = new String() ;

        if (document != null) {
            Object[] attr = new Object[] {VALUE, String.class};
            Element elem = ScilabXMLUtilities.readNodeAttributes(document, LASTOPENEDDIR, attr);

            if (elem != null) {
                path = (String) attr[1];
            }

            if (elem == null || path.isEmpty()) {
                path = System.getProperty("user.dir") ;
                ScilabXMLUtilities.replaceNamedNode(document, PROFILE, LASTOPENEDDIR, new Object[] {VALUE, path, "useCache", "true"});
                writeDocument();
            }
        }

        return path ;
    }

    /**
     * Save the console Foreground Color
     * @param color the new Color
     */
    public static void saveConsoleForeground(Color color) {
        readDocument();

        if (document != null) {
            String rgb = Integer.toHexString(color.getRGB());
            ScilabXMLUtilities.replaceNamedNode(document, PROFILE, FOREGROUNDCOLOR, new Object[] {VALUE, COLORPREFIX + rgb.substring(2, rgb.length())});
            writeDocument();
        }
    }

    /**
     * Save the console Background Color
     * @param color the new Color
     */
    public static void saveConsoleBackground(Color color) {
        readDocument();

        if (document != null) {
            String rgb = Integer.toHexString(color.getRGB());
            ScilabXMLUtilities.replaceNamedNode(document, PROFILE, BACKGROUNDCOLOR, new Object[] {VALUE, COLORPREFIX + rgb.substring(2, rgb.length())});
            writeDocument();
        }
    }

    /**
     * Read the file to modify
     */
    private static void readDocument() {
        createUserCopy();
        document = ScilabXMLUtilities.readDocument(USER_CONFIG_FILE);
    }

    /**
     * Save the modifications
     */
    private static void writeDocument() {
        if (mustSave) {
            ScilabXMLUtilities.writeDocument(document, USER_CONFIG_FILE);
        }
    }
}
