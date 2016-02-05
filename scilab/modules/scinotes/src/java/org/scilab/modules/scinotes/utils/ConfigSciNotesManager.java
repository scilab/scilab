/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2009 - INRIA - Allan SIMON
 * Copyright (C) 2010 - Calixte DENIZET
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

package org.scilab.modules.scinotes.utils;

import java.awt.Color;
import java.awt.Font;
import java.awt.Toolkit;
import java.awt.print.Paper;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Arrays;
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
import javax.xml.transform.TransformerFactoryConfigurationError;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.scilab.modules.commons.ScilabCommons;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.ScilabCommonsUtils;
import org.scilab.modules.commons.xml.ScilabXMLUtilities;
import org.scilab.modules.commons.xml.ScilabDocumentBuilderFactory;
import org.scilab.modules.commons.xml.ScilabTransformerFactory;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
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
import org.w3c.dom.Text;
import org.xml.sax.SAXException;

/**
 * Configuration class which interacts with the file etc/scinotesConfiguration.xml
 */
public final class ConfigSciNotesManager {

    public static final String RECENTBASEDIR = "recentBaseDir";
    public static final String BASEDIR = "baseDir";
    public static final String RECENTFILEPATTERN = "recentFilePattern";
    public static final String FILEPATTERN = "filePattern";
    public static final String RECENTWORDPATTERN = "recentWordPattern";
    public static final String WORDPATTERN = "wordPattern";

    private static final int BUFSIZE = 1024;

    private static final int MARGIN = 20;

    private static final String ERROR_READ = "Could not load file: ";
    private static final String ERROR_WRITE = "Could not save file: ";
    private static final String VALUE = "value";
    private static final String VERSION = "version";
    private static final String STYLE = "style";
    private static final String DEFAULT = "default";
    private static final String WIDTH = "width";
    private static final String HEIGHT = "height";
    private static final String EDITOR = "SciNotes";

    private static final String FOREGROUNDCOLOR = "ForegroundColor";
    private static final String BACKGROUNDCOLOR = "BackgroundColor";
    private static final String ALTERNCOLORS = "AlternColors";
    private static final String COLOR1 = "color1";
    private static final String COLOR2 = "color2";
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
    private static final String RECURSIVE = "recursiveSearch";
    private static final String LINEBYLINE = "readLineByLine";
    private static final String FILECASE = "fileCase";
    private static final String SEARCHINFILES = "searchInFiles";

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
    private static final String EDITORUUID = "EditorUUID";
    private static final String PANEINST = "paneInstance";
    private static final String PANEINST_EX = "paneInstanceExtra";

    private static final String FAVORITE_DIRS = "favoriteDirectories";
    private static final String DIRECTORY = "Directory";

    private static final String PAPER = "PaperFormat";
    private static final String MARGINLEFT = "MarginLeft";
    private static final String MARGINRIGHT = "MarginRight";
    private static final String MARGINTOP = "MarginTop";
    private static final String MARGINBOTTOM = "MarginBottom";

    private static final String CODENAVIGATOR = "CodeNavigator";

    private static final String SCI = "SCI";
    private static final String SCINOTES_CONFIG_FILE = System.getenv(SCI) + "/modules/scinotes/etc/scinotesConfiguration.xml";

    private static final int MAXRECENT = 20;

    private static Document document;

    private static boolean updated;
    private static boolean mustSave = true;

    private static String USER_SCINOTES_CONFIG_FILE = ScilabConstants.SCIHOME.toString() + "/scinotesConfiguration.xml";

    static {
        if (ScilabConstants.SCIHOME != null && ScilabConstants.SCIHOME.canRead() && ScilabConstants.SCIHOME.canWrite()) {
            USER_SCINOTES_CONFIG_FILE = ScilabConstants.SCIHOME.toString() + "/scinotesConfiguration.xml";
        } else {
            USER_SCINOTES_CONFIG_FILE = SCINOTES_CONFIG_FILE;
            mustSave = false;
        }
    }

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
        if (checkVersion() && mustSave) {
            /* Create a local copy of the configuration file */
            ScilabCommonsUtils.copyFile(new File(SCINOTES_CONFIG_FILE), new File(USER_SCINOTES_CONFIG_FILE));
            document = null;
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

        if (fileConfig.exists()) {
            document = null;
            readDocument(SCINOTES_CONFIG_FILE);
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
     * @return the paper format saved in previous session
     */
    public static Paper getPaperFormat() {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allSizeElements = scinotesProfile.getElementsByTagName(PAPER);
        Element paper = (Element) allSizeElements.item(0);

        if (paper == null) {
            return new Paper();
        }

        Paper p = new Paper();
        double width = Double.parseDouble(paper.getAttribute(WIDTH));
        double height = Double.parseDouble(paper.getAttribute(HEIGHT));
        double marginLeft = Double.parseDouble(paper.getAttribute(MARGINLEFT));
        double marginRight = Double.parseDouble(paper.getAttribute(MARGINRIGHT));
        double marginTop = Double.parseDouble(paper.getAttribute(MARGINTOP));
        double marginBottom = Double.parseDouble(paper.getAttribute(MARGINBOTTOM));
        p.setSize(width, height);
        p.setImageableArea(marginLeft, marginTop, width - (marginLeft + marginRight), height - (marginTop + marginBottom));

        return p;
    }

    /**
     * Save the paper format
     * @param p the Paper to save
     */
    public static void savePaperFormat(Paper p) {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allSizeElements = scinotesProfile.getElementsByTagName(PAPER);
        Element paper = (Element) allSizeElements.item(0);

        if (paper == null) {
            paper = document.createElement(PAPER);
            scinotesProfile.appendChild((Node) paper);
        }

        double width = p.getWidth();
        double height = p.getHeight();
        double marginLeft = p.getImageableX();
        double marginRight = width - (marginLeft + p.getImageableWidth());
        double marginTop = p.getImageableY();
        double marginBottom = height - (marginTop + p.getImageableHeight());

        paper.setAttribute(WIDTH, Double.toString(width));
        paper.setAttribute(HEIGHT, Double.toString(height));
        paper.setAttribute(MARGINLEFT, Double.toString(marginLeft));
        paper.setAttribute(MARGINRIGHT, Double.toString(marginRight));
        paper.setAttribute(MARGINTOP, Double.toString(marginTop));
        paper.setAttribute(MARGINBOTTOM, Double.toString(marginBottom));

        writeDocument();
    }

    /**
     * @return the color the altern colors for inner function
     */
    public static Color[] getAlternColors() {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element scinotesProfile = (Element) profiles.item(0);

        NodeList allSizeElements = scinotesProfile.getElementsByTagName(ALTERNCOLORS);
        Element alternColors = (Element) allSizeElements.item(0);
        Color[] arr = new Color[2];

        Color c;
        if (NULL.equals(alternColors.getAttribute(COLOR1))) {
            c = null;
        } else {
            c = Color.decode(alternColors.getAttribute(COLOR1));
        }

        arr[0] = c;

        if (NULL.equals(alternColors.getAttribute(COLOR2))) {
            c = null;
        } else {
            c = Color.decode(alternColors.getAttribute(COLOR2));
        }

        arr[1] = c;
        return arr;
    }

    /**
     * Get all the recent opened files
     * @return an array of uri
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

        clean(root);
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

        clean(root);
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

        clean(root);
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

        clean(root);
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

        // if we have reached the maximum, we remove the oldest files
        while (recentFiles.getLength() >= MAXRECENT) {
            root.removeChild(root.getFirstChild());
        }

        Element newFile =  document.createElement(DOCUMENT);
        newFile.setAttribute(PATH, filePath);
        root.appendChild((Node) newFile);

        clean(root);
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

        clean(root);
        writeDocument();
    }

    /**
     * Return a count of the open files that exist. New files, for instance, do not.
     * @return count
     */
    public static int countExistingOpenFiles(UUID uuid) {
        int count = 0;
        readDocument();
        Element root = (Element) document.getDocumentElement().getElementsByTagName(OPEN_FILES).item(0);
        if (root != null) {
            NodeList openFiles = root.getElementsByTagName(DOCUMENT);
            for (int i = 0; i < openFiles.getLength(); ++i) {
                Element style = (Element) openFiles.item(i);
                if (style.getAttribute(EDITORINST).equals(uuid.toString())) {
                    File temp = new File(style.getAttribute(PATH));
                    if (temp.exists()) {
                        count++;
                    }
                }
            }
        }
        return count;
    }

    /**
     * Get the list of open files associated with an editor instance hashcode.
     * Only files that exist are returned.
     * @param editorID unique id of an editor instance
     * @return an array of uri
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
                Element doc = (Element) openFiles.item(i);

                if (editorID.equals(UUID.fromString(doc.getAttribute(EDITORINST)))) {
                    File temp = new File(doc.getAttribute(PATH));

                    /* Check that the file exists and add to file list or else remove the node. */
                    if (temp.exists() && !files.contains(temp)) {
                        files.add(temp);
                    } else {
                        root.removeChild((Node) doc);
                        i--;  // Adjust index to account for removed item.
                    }
                }
            }

            clean(root);
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
        saveToOpenFiles(filePath, editorInstance, sep, -1);
    }

    /**
     * Add a file to currently open files
     * @param filePath the path of the files to add
     * @param editorInstance instance of the editor to associate with the open file
     * @param sep the pane
     */
    public static void saveToOpenFiles(String filePath, SciNotes editorInstance, ScilabEditorPane sep, int pos) {
        readDocument();
        removeFromOpenFiles(editorInstance.getUUID(), Arrays.asList(new String[] {filePath}));
        UUID nil = new UUID(0, 0);

        // Find the element containing the list of open files
        Element root = (Element) document.getDocumentElement().getElementsByTagName(OPEN_FILES).item(0);
        // Get the list of open files
        NodeList openFiles = root.getElementsByTagName(DOCUMENT);
        int numberOfFiles = openFiles.getLength();

        Node bef = null;
        if (pos != - 1 && pos < numberOfFiles) {
            bef = openFiles.item(pos);
        }

        Element newFile =  document.createElement(DOCUMENT);
        newFile.setAttribute(PATH, filePath);
        // Record the editor instance's hash code
        newFile.setAttribute(EDITORINST, editorInstance.getUUID().toString());
        //root.appendChild((Node) newFile);
        // Record the text pane's hash code
        newFile.setAttribute(PANEINST, sep.getUUID().toString());
        newFile.setAttribute(PANEINST_EX, nil.toString());
        if (bef != null) {
            root.insertBefore((Node) newFile, bef);
        } else {
            root.appendChild((Node) newFile);
        }

        clean(root);
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
    public static void removeFromOpenFiles(UUID editorID, List<String> toRemove) {
        readDocument();

        Element root = (Element) document.getDocumentElement().getElementsByTagName(OPEN_FILES).item(0);
        NodeList openFiles = root.getElementsByTagName(DOCUMENT);

        // Remove item with matching editorID and sepID.
        for (int i = openFiles.getLength() - 1; i >= 0; i--) {
            Element doc = (Element) openFiles.item(i);
            if (editorID.equals(UUID.fromString(doc.getAttribute(EDITORINST)))
                    && toRemove.contains(doc.getAttribute(PATH))) {
                root.removeChild((Node) doc);
            }
        }

        clean(root);
        writeDocument();
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

        clean(root);
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

        clean(root);
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
        readDocument(USER_SCINOTES_CONFIG_FILE);
    }

    /**
     * Read the file to modify
     */
    private static void readDocument(String pathConfSci) {
        File fileConfig = new File(USER_SCINOTES_CONFIG_FILE);
        if (!fileConfig.exists()) {
            createUserCopy();
        }
        File xml = null;
        DocumentBuilder docBuilder = null;
        String factoryName = ScilabDocumentBuilderFactory.useDefaultDocumentBuilderFactoryImpl();

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

        ScilabDocumentBuilderFactory.restoreDocumentBuilderFactoryImpl(factoryName);

        if (document == null && fileConfig.exists()) {
            ScilabModalDialog.show(null, SciNotesMessages.CORRUPTED_CONF_FILE, SciNotesMessages.SCINOTES_ERROR, IconType.ERROR_ICON);
            fileConfig.delete();
            createUserCopy();
            readDocument(pathConfSci);
        }
    }

    /**
     * Save the modifications
     */
    private static void writeDocument() {
        if (mustSave) {
            ScilabXMLUtilities.writeDocument(document, USER_SCINOTES_CONFIG_FILE);
        }
    }

    /**
     *
     */
    public static void saveCodeNavigatorState(String editorUUID, String navUUID) {
        readDocument();

        Element root = document.getDocumentElement();
        NodeList navs = root.getElementsByTagName(CODENAVIGATOR);
        boolean update = false;
        for (int i = 0; i < navs.getLength(); i++) {
            Element nav = (Element) navs.item(i);
            if (nav.getAttribute("uuid").equals(navUUID)) {
                nav.setAttribute("depends", editorUUID);
                update = true;
                break;
            }
        }

        if (!update) {
            ScilabXMLUtilities.createNode(document, root, CODENAVIGATOR, new String[] {"uuid", navUUID, "depends", editorUUID});
        }

        writeDocument();
    }

    /**
     *
     */
    public static String getCodeNavigatorState(String navUUID) {
        readDocument();

        Element root = document.getDocumentElement();
        NodeList navs = root.getElementsByTagName(CODENAVIGATOR);
        for (int i = 0; i < navs.getLength(); i++) {
            Element nav = (Element) navs.item(i);
            if (nav.getAttribute("uuid").equals(navUUID)) {
                return nav.getAttribute("depends");
            }
        }
        return null;
    }

    /**
     *
     */
    public static String getCodeNavigatorStateForEditor(String editorUUID) {
        readDocument();

        Element root = document.getDocumentElement();
        NodeList navs = root.getElementsByTagName(CODENAVIGATOR);
        for (int i = 0; i < navs.getLength(); i++) {
            Element nav = (Element) navs.item(i);
            if (nav.getAttribute("depends").equals(editorUUID)) {
                return nav.getAttribute("uuid");
            }
        }
        return null;
    }

    /**
     *
     */
    public static void saveSearchInFilesState(String editorUUID, String sfUUID) {
        readDocument();

        Element root = document.getDocumentElement();
        NodeList sfs = root.getElementsByTagName(SEARCHINFILES);
        boolean update = false;
        for (int i = 0; i < sfs.getLength(); i++) {
            Element sf = (Element) sfs.item(i);
            if (sf.getAttribute("uuid").equals(sfUUID)) {
                sf.setAttribute("depends", editorUUID);
                update = true;
                break;
            }
        }

        if (!update) {
            ScilabXMLUtilities.createNode(document, root, SEARCHINFILES, new String[] {"uuid", sfUUID, "depends", editorUUID});
        }

        writeDocument();
    }

    /**
     *
     */
    public static String getSearchInFilesState(String sfUUID) {
        readDocument();

        Element root = document.getDocumentElement();
        NodeList sfs = root.getElementsByTagName(SEARCHINFILES);
        for (int i = 0; i < sfs.getLength(); i++) {
            Element sf = (Element) sfs.item(i);
            if (sf.getAttribute("uuid").equals(sfUUID)) {
                return sf.getAttribute("depends");
            }
        }
        return null;
    }

    /**
     *
     */
    public static String getSearchInFilesStateForEditor(String editorUUID) {
        readDocument();

        Element root = document.getDocumentElement();
        NodeList sfs = root.getElementsByTagName(SEARCHINFILES);
        for (int i = 0; i < sfs.getLength(); i++) {
            Element sf = (Element) sfs.item(i);
            if (sf.getAttribute("depends").equals(editorUUID)) {
                return sf.getAttribute("uuid");
            }
        }
        return null;
    }

    /**
     *
     */
    public static void saveEditorUUID(String editorUUID) {
        readDocument();

        Element root = document.getDocumentElement();
        NodeList eis = root.getElementsByTagName(EDITORUUID);
        for (int i = 0; i < eis.getLength(); i++) {
            Element ei = (Element) eis.item(i);
            if (ei.getAttribute("uuid").equals(editorUUID)) {
                return;
            }
        }

        ScilabXMLUtilities.createNode(document, root, EDITORUUID, new String[] {"uuid", editorUUID});

        writeDocument();
    }

    /**
     *
     */
    public static void removeEditorUUID(String editorUUID) {
        readDocument();

        Element root = document.getDocumentElement();
        NodeList eis = root.getElementsByTagName(EDITORUUID);
        for (int i = 0; i < eis.getLength(); i++) {
            Element ei = (Element) eis.item(i);
            if (ei.getAttribute("uuid").equals(editorUUID)) {
                root.removeChild(ei);
            }
        }

        root = (Element) document.getDocumentElement().getElementsByTagName(OPEN_FILES).item(0);
        if (root != null) {
            NodeList openFiles = root.getElementsByTagName(DOCUMENT);
            for (int i = 0; i < openFiles.getLength(); ++i) {
                Element of = (Element) openFiles.item(i);
                if (of.getAttribute(EDITORINST).equals(editorUUID)) {
                    root.removeChild(of);
                }
            }
        }

        writeDocument();
    }

    /**
     *
     */
    public static List<String> getEditorsUUID() {
        readDocument();

        Element root = document.getDocumentElement();
        NodeList eis = root.getElementsByTagName(EDITORUUID);
        List<String> list = new ArrayList<String>();
        for (int i = 0; i < eis.getLength(); i++) {
            Element ei = (Element) eis.item(i);
            list.add(ei.getAttribute("uuid"));
        }

        return list;
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

        while (search.size() >= MAXRECENT) {
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

        clean(recents);
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

        clean(recent);
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
     * Add a file to recent Opened Files
     * @param exp the path of the files to add
     */
    public static void saveRecent(String exp, String nodeName, String childNodeName) {
        Node root = getXcosRoot();
        if (root == null || exp == null || exp.compareTo("") == 0) {
            return;
        }

        Node recents = getNodeChild(root, nodeName);
        if (recents == null) {
            recents = document.createElement(nodeName);
            root.appendChild(recents);
        }

        List<Node> list = getNodeChildren(recents, childNodeName);

        while (list.size() >= MAXRECENT) {
            removeRecent(((Element) list.get(0)).getAttribute(EXPRESSION), nodeName, childNodeName);
            list = getNodeChildren(recents, childNodeName);
        }
        //if path already in file no need to add it
        for (Node item : list) {
            if (exp.compareTo(((Element) item).getAttribute(EXPRESSION)) == 0) {
                return;
            }
        }

        Element newNode = document.createElement(childNodeName);
        newNode.setAttribute(EXPRESSION, exp);
        recents.appendChild((Node) newNode);

        clean(recents);
        writeDocument();
    }

    /**
     * @param exp the expression to remove
     */
    public static void removeRecent(String exp, String nodeName, String childNodeName) {
        Node root = getXcosRoot();
        if (root == null) {
            return;
        }

        Node recent = getNodeChild(root, nodeName);
        List<Node> list = getNodeChildren(recent, childNodeName);

        // remove node if exists
        for (Node item : list) {
            if (exp.compareTo(((Element) item).getAttribute(EXPRESSION)) == 0) {
                recent.removeChild(item);
                break;
            }
        }

        clean(recent);
        writeDocument();
    }

    /**
     * @return a list of the recent searches
     */
    public static List<String> getRecent(String nodeName, String childNodeName) {
        List<String> files = new ArrayList<String>();

        Node root = getXcosRoot();
        if (root == null) {
            return files;
        }

        Node recent = getNodeChild(root, nodeName);
        List<Node> list = getNodeChildren(recent, childNodeName);
        for (Node node : list) {
            String exp = ((Element) node).getAttribute(EXPRESSION);
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

        while (replace.size() >= MAXRECENT) {
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

        clean(recent);
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

        clean(recent);
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
     * @return true for a recursive search
     */
    public static boolean getRecursive() {
        return getBooleanAttribute(RECURSIVE, STATE_FLAG, true);
    }

    /**
     * @param recursive for a recursive search
     */
    public static void saveRecursive(boolean recursive) {
        saveBooleanAttribute(RECURSIVE, STATE_FLAG, recursive);
    }

    /**
     * @return true for a line by line search
     */
    public static boolean getLineByLine() {
        return getBooleanAttribute(LINEBYLINE, STATE_FLAG, true);
    }

    /**
     * @param lineByLine for a line by line search
     */
    public static void saveLineByLine(boolean lineByLine) {
        saveBooleanAttribute(LINEBYLINE, STATE_FLAG, lineByLine);
    }

    /**
     * @return true for a case sensitive file name
     */
    public static boolean getFileCase() {
        return getBooleanAttribute(FILECASE, STATE_FLAG, false);
    }

    /**
     * @param fileCase for a case sensitive file name
     */
    public static void saveFileCase(boolean fileCase) {
        saveBooleanAttribute(FILECASE, STATE_FLAG, fileCase);
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

        root.appendChild(recent);

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

    /**
     * Remove text at the beginning and at the end
     * @param r the element to clean
     */
    private static void clean(Node r) {
        ScilabXMLUtilities.removeEmptyLines(r);
    }
}
