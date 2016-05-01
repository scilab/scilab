/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 -         Pierre GRADIT
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
import java.awt.Component;
import java.util.HashMap;

import javax.swing.JDialog;

import org.scilab.modules.gui.filechooser.Juigetfile;
import org.w3c.dom.Node;

public final class XWizardManager extends XCommonManager {
    /** Scilab wizard header file.*/
    private static final   String   SCILAB_WIZARD_FILE =
        System.getenv("SCI") + "/modules/preferences/etc/XWizard.xml";

    /** Scilab wizard prefix.*/
    private static final   String   SCILAB_WIZARD_PREFIX =
        System.getenv("SCI") + "/modules/preferences/etc/XWizard-";

    /** Scilab configuration stylesheet.*/
    private static final   String   SCILAB_WIZARD_XSL  =
        System.getenv("SCI") + "/modules/preferences/src/xslt/XWizard.xsl";

    /** Current file name.*/
    private static         String   CURRENT_WIZARD_FILE = NAV;

    /**
     * Constructor blocked, singleton pattern.
     */
    private XWizardManager() {
        throw new UnsupportedOperationException();
    }

    /** Exclusive activity flag between all XCommonManager descendants.*/
    public static boolean active = false;

    /** Display dialog and wait for events.
     *
     */
    public static void displayAndWait() {
        XWizardManager.active = true;
        XConfigManager.active = false;
        printTimeStamp("XWizardManager launched");
        System.out.println(" |  No active manager (" + XConfigManager.active + ", " + XWizardManager.active + ").");

        reloadTransformer(SCILAB_WIZARD_XSL);
        printTimeStamp("XSL loaded");

        // Set up Swing Side
        dialog      = new JDialog(getTopLevel(), "Scilab wizard", true);
        topSwing    = dialog.getContentPane();
        //dialog = new SwingScilabTab("Scilab Preferences");
        //topSwing = new JPanel();
        dialog.setContentPane(topSwing);
        topSwing.setLayout(new BorderLayout());
        // AWT implies to set layout at construction time.

        // Set up DOM Side
        document = readDocument(SCILAB_WIZARD_FILE);
        updated  = false;
        printTimeStamp("Model XML loaded");

        // Set up correspondence
        correspondance = new HashMap<Component, XSentinel>();

        // Let the show begin!
        if (refreshDisplay()) {
            dialog.setVisible(true);
        }
    }

    /**
     * Opens a dialog to setup user blocks.
     */
    public static void openUserBlockWizard() {
        //TODO temporary plug.
        XWizardManager.displayAndWait();
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

        if (actions.length == 0) {
            //C            System.out.println("No actions processed!");
            return false;
        }
        Node action = actions[0];
        String callback = getAttribute(action, "callback");
        if (callback.equals("load")) {
            String name = getAttribute(action, "name");
            if (differential) {
                System.out.println(": Load '" + SCILAB_WIZARD_PREFIX + name + ".xml'.");
            }
            dialog.setTitle("Scilab wizard - (" + name + " mode)");
            document = readDocument(SCILAB_WIZARD_PREFIX + name + ".xml" );
            printTimeStamp("XML loaded");
            refreshDisplay();
            updated = false;
            return true;
        }
        if (callback.equals("New")) {
            // TODO it can be a contextual help.
            //System.err.println("Help not implemented yet!");
            if (differential) {
                System.out.println(": New.");
            }
            reloadTransformer(SCILAB_WIZARD_XSL);
            printTimeStamp("XSL loaded");
            document = readDocument(SCILAB_WIZARD_FILE);
            printTimeStamp("XML loaded");
            updated  = false;
            refreshDisplay();
            return true;
        }

        if (callback.equals("Open")) {
            if (differential) {
                System.out.println(": Open.");
            }
            String mask[] = {"*.xml"};
            String desc[] = {"Block description"};
            Juigetfile.uigetfile(mask, desc);
            String selection[] = Juigetfile.getSelection();
            if (selection.length > 0) {
                document = readDocument(selection[0]);
                dialog.setTitle("Scilab wizard - <<" + selection[0] + ">>");
                printTimeStamp("XML loaded");
                updated  = false;
                refreshDisplay();
            }
            return true;
        }

        if (callback.equals("SaveAs")) {
            if (differential) {
                System.out.println(": Save as.");
            }
            String mask[] = {"*.xml"};
            String desc[] = {"Block description"};
            Juigetfile.uiputfile(mask, desc);
            String selection[] = Juigetfile.getSelection();
            if (selection.length > 0) {
                CURRENT_WIZARD_FILE = selection[0];
            }
            if (CURRENT_WIZARD_FILE != NAV) {
                writeDocument(CURRENT_WIZARD_FILE, document);
                dialog.setTitle("Scilab wizard - [" + CURRENT_WIZARD_FILE + "]");
            }
            return true;
        }

        if (callback.equals("Save")) {
            if (differential) {
                System.out.println(": Save.");
            }
            if (CURRENT_WIZARD_FILE == NAV) {
                String mask[] = {"*.xml"};
                String desc[] = {"Block description"};
                Juigetfile.uiputfile(mask, desc);
                String selection[] = Juigetfile.getSelection();
                if (selection.length > 0) {
                    CURRENT_WIZARD_FILE = selection[0];
                }
            }
            if (CURRENT_WIZARD_FILE != NAV) {
                writeDocument(CURRENT_WIZARD_FILE, document);
            }
            return true;
        }

        /****
        if (callback.equals("Help")) {
            // TODO it can be a contextual help.
            //System.err.println("Help not implemented yet!");
            if (differential) {
                System.out.println(": Help.");
            }
            return true;
        }
        if (callback.equals("Apply")) {
            //System.err.println("User XML saved!");
            updated = false;
            if (differential) {
                System.out.println(": Apply.");
            }
            writeDocument(USER_CONFIG_FILE);
            return true;
        }
        if (callback.equals("Default")) {
            //System.out.println("Scilab XML reloaded!");
            if (differential) {
                System.out.println(": Default.");
            }
            reloadTransformer(SCILAB_CONFIG_XSL);
            refreshUserCopy(SCILAB_CONFIG_FILE, USER_CONFIG_FILE);
            readUserDocument();
            printTimeStamp("XSL Reloaded");
            updated = false;
            refreshDisplay();
            return true;
        }
        if (callback.equals("Cancel")) {
            //System.err.println("User XML reloaded!");
            readUserDocument();
            // TODO advertise it!
            //if (updated) {
            //    <<some advertising statement>>
            //    }
            updated = false;
            if (differential) {
                System.out.println(": Cancel.");
            }
            refreshDisplay();
            return true;
        }*****/
        if (callback.equals("Ok")) {
            if (differential) {
                System.out.println(": Ok.");
            }
            dialog.dispose();
            updated = false;
            return true;
        }
        if (callback.equals("Quit")) {
            if (differential) {
                System.out.println(": Quit.");
            }
            dialog.dispose();
            updated = false;
            return true;
        }

        return false;
    }

    /** TODO How to impact modification of wizard ?
    *   clearly an issue in this case as activity is lost so far!
    */
    public static void subscribeUpdate() {
    }

    /** TODO How to impact modification of preferences ?
    *
    */
    public static void notifyUpdate() {
    }

}