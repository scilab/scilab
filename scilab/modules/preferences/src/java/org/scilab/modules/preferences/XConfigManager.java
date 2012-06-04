/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 -         Pierre GRADIT
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Frame;
import java.awt.Toolkit;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.io.File;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.swing.JDialog;

import org.w3c.dom.Document;
import org.w3c.dom.DocumentFragment;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import org.scilab.modules.commons.ScilabCommons;
import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.preferences.ScilabPreferences.ToolboxInfos;

/**
 * Extended management of the Console configuration file.
 *
 * @author Pierre GRADIT
 * @author Vincent COUVERT
 */
public final class XConfigManager extends XCommonManager {

    /** Exclusive activity flag between all XCommonManager descendants.*/
    public static boolean active = false;

    /** Scilab configuration file.*/
    private static final String SCILAB_CONFIG_FILE = System.getenv("SCI") + "/modules/preferences/etc/XConfiguration.xml";

    /** Scilab configuration stylesheet.*/
    private static final String SCILAB_CONFIG_XSL = System.getenv("SCI") + "/modules/preferences/src/xslt/XConfiguration.xsl";

    /** User configuration file.*/
    private static final String USER_CONFIG_FILE = ScilabCommons.getSCIHOME() + "/XConfiguration.xml";


    /**
     * Constructor blocked, singleton pattern.
     */
    private XConfigManager() {
        throw new UnsupportedOperationException();
    }

    /** Display dialog and wait for events.
     *
     */
    public static void displayAndWait() {
        XConfigManager.active = true;
        XWizardManager.active = false;

        reloadTransformer(SCILAB_CONFIG_XSL);

        // Set up Swing Side
        dialog = new JDialog(getTopLevel(), "Scilab Preferences", true);
        topSwing = dialog.getContentPane();
        topSwing.setLayout(new BorderLayout());
        // AWT implies to set layout at construction time.

        // Set up DOM Side
        readUserDocuments();
        updated = false;

        // Plug in resize
        //dialog.setResizable(false);
        dialog.addComponentListener(new ComponentAdapter() {
            public void componentResized(ComponentEvent e) {
                Element element = (Element) document.getDocumentElement();
                Dimension dimension = dialog.getSize();
                int height = XConfigManager.getInt(element, "height", 0);
                int width = XConfigManager.getInt(element, "width",  0);
                if (Math.abs(((double) height) - dimension.getHeight()) > 0.1 && Math.abs(((double) width) - dimension.getWidth()) > 0.1 ) {
                    element.setAttribute("height", Integer.toString((int) dimension.getHeight()));
                    element.setAttribute("width", Integer.toString((int) dimension.getWidth()));
                }
            }
        });

        // Set up correspondence
        correspondance = new HashMap<Component, XSentinel>();

        // Let the show begin!
        if (refreshDisplay()) {
            // Center the dialog on the parent window
            Frame topWindow = XCommonManager.getTopLevel();
            int x = topWindow.getX() + (topWindow.getWidth() - dialog.getWidth()) / 2;
            int y = topWindow.getY() + (topWindow.getHeight() - dialog.getHeight()) / 2;
            dialog.setLocation(x, y);

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
    private static JDialog help = null;

    /** Read files to modify (and possibly create it).
     */
    private static void readUserDocuments() {
        document = XConfiguration.getXConfigurationDocument();
        NodeList toolboxes = document.getElementsByTagName("toolboxes");
        if (toolboxes.getLength() != 1) {
            System.err.println("Can't hook toolboxes [1]");
            return;
        }

        // Toolboxes files
        Element toolbox = (Element) toolboxes.item(0);
        toolboxes = toolbox.getChildNodes();

        if (toolboxes.getLength() > 0) {
            System.err.println("Recover from inconsistent state...");
            while (toolbox.hasChildNodes()) {
                toolbox.removeChild(toolbox.getFirstChild());
            }
        }

        // Body (rendered as XConfiguration.xsl example)
        Element body = document.createElement("body");
        Element toolboxInfo = document.createElement("toolbox-info");
        body.appendChild(toolboxInfo);
        toolbox.appendChild(body);

        List<ToolboxInfos> infos = ScilabPreferences.getToolboxesInfos();
        System.out.println("" + infos.size() + " toolboxes loaded.");
        for (int i = 0; i < infos.size(); i++) {
            ToolboxInfos info = infos.get(i);
            String UserToolboxToken = info.getName().replace(' ', '_');
            String UserToolboxFile = ScilabCommons.getSCIHOME() + "/" + UserToolboxToken + ".xml";
            createUserCopy(info.getPrefFile(), UserToolboxFile);
            // Building document fragment
            Element token = document.createElement(UserToolboxToken);
            DocumentFragment fragment = document.createDocumentFragment();
            Document ToolboxDocument = readDocument(UserToolboxFile);
            Node transferred = ToolboxDocument.getDocumentElement();
            //-- System.out.println("-->" + transferred.getNodeName());
            transferred = document.importNode(transferred, true);
            fragment.appendChild(transferred);
            token.insertBefore(fragment, null);
            toolbox.appendChild(token);
        }

        toolboxes = toolbox.getChildNodes();
        if (infos.size() + 1 != toolboxes.getLength()) {
            System.err.println("Can't hook toolboxes [4]");
            return;
        }
    }

    private static void WriteUserDocuments() {
        // Toolboxes files
        NodeList toolboxes = document.getElementsByTagName("toolboxes");
        Element toolbox = (Element) toolboxes.item(0);
        if (toolbox.getFirstChild() != null) {
            toolbox.removeChild(toolbox.getFirstChild()); // body
            List<ToolboxInfos> infos = ScilabPreferences.getToolboxesInfos();
            toolboxes = toolbox.getChildNodes();
            if (infos.size() != toolboxes.getLength()) {
                System.err.println("Can't hook toolboxes [3]");
                return;
            }
            for (int i = 0; i < infos.size(); i++) {
                Node ToolboxNode = toolboxes.item(i);
                if (ToolboxNode != null) {
                    ToolboxInfos info = infos.get(i);
                    String UserToolboxFile = ScilabCommons.getSCIHOME() + "/" + info.getName().replace(' ', '_') + ".xml";
                    XConfiguration.writeDocument(UserToolboxFile, ToolboxNode.getFirstChild());
                    //toolbox.removeChild(ToolboxNode);
                }
            }
        }
        // Main file
        XConfiguration.writeDocument(USER_CONFIG_FILE, document);
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
        if (callback.equals("Ok")) {
            WriteUserDocuments();
            dialog.dispose();
            updated = false;
            XConfiguration.fireXConfigurationEvent();
            return true;
        }
        if (callback.equals("Apply")) {
            updated = false;
            WriteUserDocuments();
            XConfiguration.fireXConfigurationEvent();
            return true;
        }
        if (callback.equals("Default")) {
            reloadTransformer(SCILAB_CONFIG_XSL);
            document = XConfiguration.createDocument();
            writeDocument(USER_CONFIG_FILE, document);
            List<ToolboxInfos> infos = ScilabPreferences.getToolboxesInfos();
            for (int i = 0; i < infos.size(); i++) {
                ToolboxInfos info = infos.get(i);
                String UserToolboxFile = ScilabCommons.getSCIHOME() + "/" + info.getName().replace(' ', '_') + ".xml";
                refreshUserCopy(info.getPrefFile(), UserToolboxFile);
            }
            readUserDocuments();
            updated = false;
            refreshDisplay();

            return true;
        }
        if (callback.equals("Cancel")) {
            dialog.dispose();
            XConfiguration.clearModifiedPath();
            updated = false;
            refreshDisplay();
            return true;
        }
        return false;
    }
}
