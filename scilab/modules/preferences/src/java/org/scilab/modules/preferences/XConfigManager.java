/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 -         Pierre GRADIT
 * Copyright (C) 2012-2014 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.modules.preferences;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Frame;
import java.awt.Toolkit;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.swing.ImageIcon;
import javax.swing.JDialog;

import org.w3c.dom.Document;
import org.w3c.dom.DocumentFragment;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import org.scilab.modules.commons.ScilabCommons;
import org.scilab.modules.commons.ScilabCommonsUtils;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.preferences.ScilabPreferences.ToolboxInfos;

/**
 * Extended management of the Console configuration file.
 *
 * @author Pierre GRADIT
 * @author Vincent COUVERT
 */
public final class XConfigManager extends XCommonManager {

    private static final String RESET_DEFAULT_LAYOUT_QUESTION = Messages.gettext("Are you sure you want to restore the default layout at next startup?");
    private static final String RESET_LAYOUT_TITLE = Messages.gettext("Reset layout");

    /** Exclusive activity flag between all XCommonManager descendants.*/
    public static boolean active = false;

    /** Scilab configuration file.*/
    private static final String SCILAB_CONFIG_FILE = System.getenv("SCI") + "/modules/preferences/etc/XConfiguration.xml";

    /** Scilab configuration stylesheet.*/
    private static final String SCILAB_CONFIG_XSL = System.getenv("SCI") + "/modules/preferences/src/xslt/XConfiguration.xsl";

    /** User configuration file.*/
    private static String USER_CONFIG_FILE;
    private static boolean mustSave = true;

    static {
        //ScilabPreferences.addToolboxInfos("MyToolbox", System.getenv("SCI") + "/contrib/toolbox_skeleton/", System.getenv("SCI") + "/contrib/toolbox_skeleton/etc/toolbox_skeleton_preferences.xml");

        if (ScilabConstants.SCIHOME != null && ScilabConstants.SCIHOME.canRead() && ScilabConstants.SCIHOME.canWrite()) {
            USER_CONFIG_FILE = ScilabConstants.SCIHOME.toString() + "/XConfiguration.xml";
        } else {
            USER_CONFIG_FILE = SCILAB_CONFIG_FILE;
            mustSave = false;
        }
    }

    /**
     * Constructor blocked, singleton pattern.
     */
    private XConfigManager() {
        throw new UnsupportedOperationException();
    }

    /** Display dialog and wait for events.
     *
     */
    public static void displayAndWait(String initialPath) {
        XConfigManager.active = true;
        XWizardManager.active = false;

        reloadTransformer(SCILAB_CONFIG_XSL);
        Frame topWindow = XCommonManager.getTopLevel();

        // Set up Swing Side
        dialog = new JDialog(topWindow, Messages.gettext("Scilab Preferences"), true);
        dialog.setIconImage(new ImageIcon(FindIconHelper.findIcon("preferences-system", "256x256")).getImage());
        topSwing = dialog.getContentPane();
        topSwing.setLayout(new BorderLayout());
        // AWT implies to set layout at construction time.

        // Set up DOM Side
        readUserDocuments();
        if (initialPath != null && !initialPath.equals("")) {
            String path = getPath(initialPath);
            document.getDocumentElement().setAttribute("path", path);
        }
        updated = false;

        // Plug in resize
        //dialog.setResizable(false);
        dialog.addComponentListener(new ComponentAdapter() {
            public void componentResized(ComponentEvent e) {
                Element element = document.getDocumentElement();
                Dimension dimension = dialog.getSize();
                int height = XConfigManager.getInt(element, "height", 0);
                int width = XConfigManager.getInt(element, "width",  0);
                if (Math.abs(((double) height) - dimension.getHeight()) > 0.1 || Math.abs(((double) width) - dimension.getWidth()) > 0.1 ) {
                    element.setAttribute("height", Integer.toString((int) dimension.getHeight()));
                    element.setAttribute("width", Integer.toString((int) dimension.getWidth()));
                }
            }
        });

        dialog.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                cancel();
            }
        });
        ScilabSwingUtilities.closeOnEscape(dialog);

        // Set up correspondence
        correspondance = new HashMap<Component, XSentinel>();

        // Let the show begin!
        if (refreshDisplay()) {
            // Center the dialog on the parent window
            if (topWindow != null) {
                int x = topWindow.getX() + (topWindow.getWidth() - dialog.getWidth()) / 2;
                int y = topWindow.getY() + (topWindow.getHeight() - dialog.getHeight()) / 2;
                dialog.setLocation(x, y);
            }

            dialog.setVisible(true);
        }
    }

    /**
     * Opens a dialog to setup preferences.
     */
    public static void openPreferences(String initialPath) {
        XConfigManager.displayAndWait(initialPath);
    }

    /**
     * Opens a dialog to setup preferences.
     */
    public static void openPreferences() {
        XConfigManager.displayAndWait(null);
    }

    /** Secondary dialog for help.*/
    private static JDialog help = null;

    /** Read files to modify (and possibly create it).
     */
    private static void readUserDocuments() {
        /*
         * The default document just contains an empty tag <toolboxes/>
         * We add a body and the different toolbox entries to have something like
         * <toolboxes>
         *   <body/>
         *   <mytoolbox1><body>...</body></mytoolbox1>
         *   <mytoolbox2><body>...</body></mytoolbox2>
         * </toolboxes>
         */

        document = XConfiguration.getXConfigurationDocument();

        // We remove all the previous tags (should be empty but an error could have occurred...)
        NodeList toolboxes = document.getElementsByTagName("toolboxes");
        for (int i = 0; i < toolboxes.getLength(); i++) {
            try {
                document.getDocumentElement().removeChild(toolboxes.item(i));
            } catch (Exception e) {
                // Maybe root is not the good parent...
            }
        }

        Element tbxs = document.createElement("toolboxes");
        document.getDocumentElement().appendChild(tbxs);

        // Body (rendered as XConfiguration.xsl example)
        Element body = document.createElement("body");
        tbxs.appendChild(body);

        List<ToolboxInfos> infos = ScilabPreferences.getToolboxesInfos();
        for (ToolboxInfos info : infos) {
            String filename = new File(info.getPrefFile()).getName();
            String tbxTag = info.getName().replace(' ', '_');
            String tbxFile = ScilabConstants.SCIHOME.toString() + "/" + filename;

            // the copy is made only if needed
            createUserCopy(info.getPrefFile(), tbxFile);

            // Building document fragment
            Element token = document.createElement(tbxTag);
            token.setAttribute("title", info.getName());
            DocumentFragment fragment = document.createDocumentFragment();
            Document tbxDocument = readDocument(tbxFile);
            Node transferred = tbxDocument.getDocumentElement();

            transferred = document.importNode(transferred, true);
            fragment.appendChild(transferred);
            token.insertBefore(fragment, null);
            tbxs.appendChild(token);
        }
    }

    private static void WriteUserDocuments() {
        // Toolboxes files
        NodeList toolboxes = document.getElementsByTagName("toolboxes");
        Element tbxs = null;
        Element emptyTbxs;
        if (toolboxes != null && toolboxes.getLength() != 0) {
            tbxs = (Element) toolboxes.item(0);
            List<ToolboxInfos> infos = ScilabPreferences.getToolboxesInfos();
            for (ToolboxInfos info : infos) {
                String tbxTag = info.getName().replace(' ', '_');
                NodeList list = tbxs.getElementsByTagName(tbxTag);
                if (list != null && list.getLength() != 0) {
                    Element mytbx = (Element) list.item(0);
                    if (mytbx.hasChildNodes()) {
                        String filename = new File(info.getPrefFile()).getName();
                        String tbxFile = ScilabConstants.SCIHOME.toString() + "/" + filename;
                        XConfiguration.writeDocument(tbxFile, mytbx.getFirstChild());
                    }
                }
            }
        }

        if (tbxs != null) {
            document.getDocumentElement().removeChild(tbxs);
        }

        XConfiguration.writeDocument(USER_CONFIG_FILE, document);

        if (tbxs != null) {
            document.getDocumentElement().appendChild(tbxs);
        }
    }

    private static void cancel() {
        dialog.dispose();
        XCommonManager.invalidateXSL();
        XConfiguration.invalidate();
        NodeList toolboxes = document.getElementsByTagName("toolboxes");
        if (toolboxes != null && toolboxes.getLength() != 0) {
            Element tbxs = (Element) toolboxes.item(0);
            document.getDocumentElement().removeChild(tbxs);
        }
        correspondance = null;
        updated = false;
    }

    /** Interpret action.
     *
     * @param action : to be interpreted.
     * @param source : component source of the action (only class is needed).
     * @return whether event was treated or not.
     */
    public static boolean xEvent(final Node[] actions, final Component source) {
        if (generixEvent(actions, source)) {
            return true;
        }

        if (actions.length == 0) {
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
        if (callback.equals("Save Backup")) {
            String path = getAttribute(action, "path");
            writeDocument(ScilabCommonsUtils.getCorrectedPath(path), document);

            return true;
        }

        if (callback.equals("Reset layout")) {
            if (ScilabModalDialog.show(dialog, new String[] {RESET_DEFAULT_LAYOUT_QUESTION}, RESET_LAYOUT_TITLE, IconType.QUESTION_ICON, ButtonType.YES_NO) == AnswerOption.NO_OPTION) {
                return false;
            }

            WindowsConfigurationManager.resetLayout();

            return true;
        }

        if (callback.equals("Restore Backup")) {
            String path = getAttribute(action, "path");
            try {
                copyFile(new File(ScilabCommonsUtils.getCorrectedPath(path)), new File(USER_CONFIG_FILE));
            } catch (IOException e) {
                System.err.println(e);
            }

            XConfiguration.invalidate();
            XConfiguration.addModifiedPath("ALL");
            reloadTransformer(SCILAB_CONFIG_XSL);
            document = XConfiguration.createDocument();

            List<ToolboxInfos> infos = ScilabPreferences.getToolboxesInfos();
            for (ToolboxInfos info : infos) {
                String filename = new File(info.getPrefFile()).getName();
                String tbxFile = ScilabConstants.SCIHOME.toString() + "/" + filename;
                refreshUserCopy(info.getPrefFile(), tbxFile);
            }
            readUserDocuments();
            updated = false;
            refreshDisplay();

            return true;
        }

        if (callback.equals("Ok")) {
            WriteUserDocuments();
            dialog.dispose();
            XCommonManager.invalidateXSL();
            updated = false;
            XConfiguration.fireXConfigurationEvent();
            XConfiguration.invalidate();
            return true;
        }
        if (callback.equals("Apply")) {
            updated = false;
            WriteUserDocuments();
            XConfiguration.fireXConfigurationEvent();
            return true;
        }
        if (callback.equals("Default")) {
            if (ScilabModalDialog.show(dialog, new String[] {Messages.gettext("Are you sure you want to reset all settings to the default values?")}, Messages.gettext("Reset"), IconType.QUESTION_ICON, ButtonType.YES_NO) == AnswerOption.NO_OPTION) {
                return false;
            }

            Cursor oldCursor = dialog.getContentPane().getCursor();
            dialog.getContentPane().setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));

            XConfiguration.invalidate();
            XConfiguration.addModifiedPath("ALL");
            reloadTransformer(SCILAB_CONFIG_XSL);
            document = XConfiguration.createDocument();
            writeDocument(USER_CONFIG_FILE, document);
            List<ToolboxInfos> infos = ScilabPreferences.getToolboxesInfos();
            for (ToolboxInfos info : infos) {
                String filename = new File(info.getPrefFile()).getName();
                String tbxFile = ScilabConstants.SCIHOME.toString() + "/" + filename;
                refreshUserCopy(info.getPrefFile(), tbxFile);
            }
            readUserDocuments();
            updated = false;
            refreshDisplay();

            dialog.getContentPane().setCursor(oldCursor);

            return true;
        }
        if (callback.equals("Cancel")) {
            cancel();
            return true;
        }
        return false;
    }

    /**
     * Save the modifications
     */
    protected static void writeDocument(String filename, Node written) {
        if (!SCILAB_CONFIG_FILE.equals(filename)) {
            XCommonManager.writeDocument(filename, written);
        }
    }
}
