/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
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

package org.scilab.modules.history_browser;

import java.awt.MouseInfo;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.SwingUtilities;

import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenu;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.history_browser.actions.ClearAction;
import org.scilab.modules.history_browser.actions.CopyAction;
import org.scilab.modules.history_browser.actions.CutAction;
import org.scilab.modules.history_browser.actions.DeleteAction;
import org.scilab.modules.history_browser.actions.EditInScinotesAction;
import org.scilab.modules.history_browser.actions.EvaluateAction;

/**
 * Command History Mouse Event management
 * @author Vincent COUVERT
 */
public class CommandHistoryMouseListener implements MouseListener {

    /**
     * Constructor
     */
    public CommandHistoryMouseListener() {
    }

    /**
     * Mouse Clicked event management
     * @param e the event
     * @see java.awt.event.MouseListener#mouseClicked(java.awt.event.MouseEvent)
     */
    public void mouseClicked(MouseEvent e) {

        // Right click management
        if ((e.getClickCount() == 1 && SwingUtilities.isRightMouseButton(e)) || e.isPopupTrigger() || isMacOsPopupTrigger(e)) {

            // Does nothing if no node selected
            if (CommandHistory.getTree().getSelectionPath() != null) {
                displayContextMenu();
            }
        }

        if ((e.getClickCount() == 2 && SwingUtilities.isLeftMouseButton(e))) {
            // Executed the leaf command
            CommonCallBack action = new EvaluateAction();
            action.callBack();
        }
    }

    /**
     * Mouse Entered event management
     * @param e the event
     * @see java.awt.event.MouseListener#mouseEntered(java.awt.event.MouseEvent)
     */
    public void mouseEntered(MouseEvent e) {
    }

    /**
     * Mouse Exited event management
     * @param e the event
     * @see java.awt.event.MouseListener#mouseExited(java.awt.event.MouseEvent)
     */
    public void mouseExited(MouseEvent e) {
    }

    /**
     * Mouse Pressed event management
     * @param e the event
     * @see java.awt.event.MouseListener#mousePressed(java.awt.event.MouseEvent)
     */
    public void mousePressed(MouseEvent e) {
    }

    /**
     * Mouse Released event management
     * @param e the event
     * @see java.awt.event.MouseListener#mouseReleased(java.awt.event.MouseEvent)
     */
    public void mouseReleased(MouseEvent e) {
    }

    /**
     * Create and display the context menu
     */
    private void displayContextMenu() {
        ContextMenu menu = ScilabContextMenu.createContextMenu();
        menu.add(CopyAction.createMenuItem());
        menu.add(CutAction.createMenuItem());
        menu.add(EvaluateAction.createMenuItem());
        menu.add(EditInScinotesAction.createMenuItem());
        menu.getAsSimpleContextMenu().addSeparator();
        menu.add(DeleteAction.createMenuItem());
        menu.add(ClearAction.createMenuItem());

        menu.setVisible(true);

        ((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(
            MouseInfo.getPointerInfo().getLocation().x,
            MouseInfo.getPointerInfo().getLocation().y);

    }

    /**
     * This function checks for the popup menu activation under MacOS with Java version 1.5
     * @param e Click event
     * @return true if Java 1.5 and MacOS and mouse clic and ctrl activated
     */
    public static boolean isMacOsPopupTrigger(MouseEvent e) {
        return (SwingUtilities.isLeftMouseButton(e)
                && e.isControlDown()
                && (System.getProperty("os.name").toLowerCase().indexOf("mac") != -1) && (System
                        .getProperty("java.specification.version").equals("1.5") || System
                        .getProperty("java.specification.version").equals("1.6")));
    }

}
