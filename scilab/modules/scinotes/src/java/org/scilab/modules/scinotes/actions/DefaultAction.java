/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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

package org.scilab.modules.scinotes.actions;

import java.awt.event.ActionListener;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JOptionPane;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;

import org.scilab.modules.commons.OS;
import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabCefPanel;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.scinotes.MonacoEditorPane;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * Default action in SciNotes
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 */
public class DefaultAction extends CommonCallBack {

    private static final long serialVersionUID = 3597772070169671017L;

    private final SciNotes editor;

    /**
     * Constructor
     * @param editor associated SciNotes instance
     */
    private DefaultAction(SciNotes editor) {
        super(SciNotesMessages.DEFAULT + SciNotesMessages.DOTS);
        this.editor = editor;
    }

    /**
     * Constructor
     * @param label action name
     * @param editor associated editor
     */
    protected DefaultAction(String label, SciNotes editor) {
        super(label);
        this.editor = editor;
    }

    /**
     * Get associated editor
     * @return editor instance
     */
    public SciNotes getEditor() {
        return editor;
    }

    /**
     * On macOS, run an Edit menu command in the browser of this window holding
     * the keyboard focus when it is not the editor (e.g. the assistant): the
     * menu bar takes Cmd+V and the like before Chromium sees them.
     * @param command the command
     * @return true if that browser ran it
     */
    protected boolean runInFocusedBrowser(ScilabCefPanel.EditCommand command) {
        if (OS.get() != OS.MAC) {
            return false;
        }
        ScilabCefPanel focused = ScilabCefPanel.getFocusedPanel();
        if (editor == null || focused == null || !focused.isShowing()
                || SwingUtilities.getWindowAncestor(focused) != SwingUtilities.getWindowAncestor(editor)) {
            return false;
        }
        ScilabEditorPane pane = editor.getTextPane();
        if (pane instanceof MonacoEditorPane && ((MonacoEditorPane) pane).isMonacoBrowser(focused)) {
            return false;
        }
        focused.runEditCommand(command);
        return true;
    }

    /**
     * Create a button for a tool bar
     * @param title tooltip for the button
     * @param icon image icon
     * @param listener action listener
     * @return the button
     */
    protected static JButton createButton(String title, String icon, ActionListener listener) {
        JButton button = new JButton();
        button.addActionListener(listener);
        button.setToolTipText(title);
        if (icon == null) {
            button.setText(title);
        } else {
            button.setIcon(new ImageIcon(FindIconHelper.findIcon(icon)));
        }
        return button;
    }

    /**
     * Create a menu for a menu bar
     * @param title label for the menu
     * @param icon image icon
     * @param listener action listener
     * @param keyStroke menu shortcut
     * @return the button
     */
    protected static MenuItem createMenu(String title, String icon, DefaultAction listener, KeyStroke keyStroke) {
        MenuItem menu = ScilabMenuItem.createMenuItem();
        menu.setCallback(listener);
        menu.setText(title);

        if (keyStroke != null) {
            ((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).setAccelerator(keyStroke);
        }

        if (icon != null) {
            ((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).setIcon(new ImageIcon(FindIconHelper.findIcon(icon)));
        }

        return menu;
    }

    /**
     * Default action
     */
    public void doAction() {
        JOptionPane.showMessageDialog(getEditor(), "Not Implemented Now !!!", null, JOptionPane.ERROR_MESSAGE);
    }

    /**
     * Default action
     */
    @Override
    public void callBack() {
        doAction();
    }

}
