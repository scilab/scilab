/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent Couvert
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

package org.scilab.modules.gui.bridge.console;

import java.awt.Dimension;
import java.awt.Toolkit;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.event.ActionEvent;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.StringTokenizer;
import java.util.logging.Logger;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.JCheckBox;
import javax.swing.SwingUtilities;
import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.console.SciConsole;
import org.scilab.modules.graphic_objects.graphicObject.CallBack;
import org.scilab.modules.gui.bridge.CallScilabBridge;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.console.SimpleConsole;
import org.scilab.modules.gui.events.callback.ScilabCallBack;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.history_manager.HistoryManagement;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.commons.xml.XConfiguration.XConfAttribute;

import static org.scilab.modules.commons.xml.XConfiguration.XConfAttribute;

/**
 * Swing implementation for Scilab Console in GUIs
 * @author Vincent COUVERT
 */
@SuppressWarnings(value = { "serial" })
public class SwingScilabConsole extends SciConsole implements SimpleConsole {

    private static final long serialVersionUID = 1L;

    private static final String CONFIRMATION_PATH = "//general/confirmation-dialogs/body/tools/tool[@id='console-clear']";
    private static final String CLEAR = Messages.gettext("Clear Console");
    private static final String CLEAR_CONFIRM = Messages.gettext("Are you sure you want to clear the console ?");
    private static final String DONT_SHOW = Messages.gettext("Do not show this message again");

    private static final Logger log = Logger.getLogger("org.scilab.modules.gui.bridge.console.SwingScilabConsole");
    
    /**
     * Constructor
     */
    public SwingScilabConsole() {
        //super(ConfigManager.getUserConfigFile());
        super();

        SwingScilabContextMenu menu = new SwingScilabContextMenu();

        SwingScilabMenuItem cutMenu = new SwingScilabMenuItem();
        cutMenu.setText(Messages.gettext("Cut"));
        cutMenu.setCallback(ScilabCallBack.createCallback(
                                "org.scilab.modules.gui.bridge.CallScilabBridge.cutConsoleSelection",
                                CallBack.JAVA));
        cutMenu.setMnemonic('U');

        SwingScilabMenuItem copyMenu = new SwingScilabMenuItem();
        copyMenu.setText(Messages.gettext("Copy"));
        copyMenu.setCallback(ScilabCallBack.createCallback(
                                 "org.scilab.modules.gui.bridge.CallScilabBridge.copyConsoleSelection",
                                 CallBack.JAVA));
        copyMenu.setMnemonic('C');

        SwingScilabMenuItem pasteMenu = new SwingScilabMenuItem();
        pasteMenu.setText(Messages.gettext("Paste"));
        pasteMenu.setCallback(ScilabCallBack.createCallback(
                                  "org.scilab.modules.gui.bridge.CallScilabBridge.pasteClipboardIntoConsole",
                                  CallBack.JAVA));
        pasteMenu.setMnemonic('P');

        SwingScilabMenuItem clearHistoryMenu = new SwingScilabMenuItem();
        clearHistoryMenu.setText(Messages.gettext("Clear History"));
        clearHistoryMenu.setCallback(ScilabCallBack.createCallback(
                                         "org.scilab.modules.gui.bridge.CallScilabBridge.clearHistory",
                                         CallBack.JAVA));
        clearHistoryMenu.setMnemonic('H');

        SwingScilabMenuItem clearMenu = new SwingScilabMenuItem();
        clearMenu.setText(Messages.gettext("Clear Console"));
        clearMenu.setCallback(ScilabCallBack.createCallback(
                                  "org.scilab.modules.gui.bridge.CallScilabBridge.clear",
                                  CallBack.JAVA));
        clearMenu.setMnemonic('O');

        SwingScilabMenuItem selectMenu = new SwingScilabMenuItem();
        selectMenu.setText(Messages.gettext("Select All"));
        selectMenu.setCallback(ScilabCallBack.createCallback(
                                   "org.scilab.modules.gui.bridge.CallScilabBridge.selectAllConsoleContents",
                                   CallBack.JAVA));
        selectMenu.setMnemonic('S');


        final SwingScilabMenuItem helpMenu = new SwingScilabMenuItem();
        helpMenu.setText(Messages.gettext("Help on a selected keyword"));
        helpMenu.setCallback(ScilabCallBack.createCallback(
                                 "org.scilab.modules.gui.bridge.CallScilabBridge.helpOnTheKeyword",
                                 CallBack.JAVA));
        helpMenu.setMnemonic('M');
        PropertyChangeListener listener = new PropertyChangeListener() {
            public void propertyChange(PropertyChangeEvent arg0) {
                String keyword = getSelectedText();
                if (keyword == null || keyword.length() == 0) {
                    helpMenu.setText(Messages.gettext("Help about a selected text"));
                } else {
                    int nbOfDisplayedOnlyXChar = 10;
                    if (keyword.length() > nbOfDisplayedOnlyXChar) {
                        keyword = keyword.substring(0, nbOfDisplayedOnlyXChar) + "...";
                    }
                    helpMenu.setText(Messages.gettext("Help about '") + keyword + "'");
                }
            }
        };
        helpMenu.addPropertyChangeListener(listener);

        final SwingScilabMenuItem evalWithEchoMenu = new SwingScilabMenuItem();
        evalWithEchoMenu.setText(Messages.gettext("Evaluate selection with echo"));
        evalWithEchoMenu.setCallback(ScilabCallBack.createCallback("org.scilab.modules.gui.bridge.CallScilabBridge.evaluateSelectionWithEcho",
                                     CallBack.JAVA));
        evalWithEchoMenu.setMnemonic('E');
        listener = new PropertyChangeListener() {
            public void propertyChange(PropertyChangeEvent arg0) {
                String str = getSelectedText();
                evalWithEchoMenu.setEnabled(str != null && !str.isEmpty());
            }
        };
        evalWithEchoMenu.addPropertyChangeListener(listener);

        final SwingScilabMenuItem evalWithNoEchoMenu = new SwingScilabMenuItem();
        evalWithNoEchoMenu.setText(Messages.gettext("Evaluate selection with no echo"));
        evalWithNoEchoMenu.setCallback(ScilabCallBack.createCallback("org.scilab.modules.gui.bridge.CallScilabBridge.evaluateSelectionWithNoEcho",
                                       CallBack.JAVA));
        evalWithNoEchoMenu.setMnemonic('N');
        listener = new PropertyChangeListener() {
            public void propertyChange(PropertyChangeEvent arg0) {
                String str = getSelectedText();
                evalWithNoEchoMenu.setEnabled(str != null && !str.isEmpty());
            }
        };
        evalWithEchoMenu.addPropertyChangeListener(listener);

        menu.add(helpMenu);

        menu.addSeparator();

        menu.add(cutMenu);
        menu.add(copyMenu);
        menu.add(pasteMenu);

        menu.addSeparator();

        menu.add(clearHistoryMenu);
        menu.add(clearMenu);

        menu.addSeparator();

        menu.add(selectMenu);
        menu.addSeparator();

        menu.add(evalWithEchoMenu);
        menu.add(evalWithNoEchoMenu);

        setComponentPopupMenu(menu);
    }

    /**
     * Displays data in the console
     * @param dataToDisplay the data to be displayed
     * @see fr.scilab.console.HelpBrowser#display(java.lang.String)
     */
    public void display(String dataToDisplay) {
        append(dataToDisplay);
    }

    /**
     * Displays data in the console with a severity
     * @param dataToDisplay the data to be displayed
     * @param severity severity of the message (0: output, 1: warning, 2: error)
     * @see fr.scilab.console.HelpBrowser#display(java.lang.String)
     */
    @Override
    public void display(String dataToDisplay, int severity) {
        switch (severity) {
            case 2:
                append(dataToDisplay, "error");
                break;
            case 1:
                append(dataToDisplay, "warning");
                break;
            default:
                append(dataToDisplay);
                break;
        }
    }

    @Override
    public int getCharWithoutOutput() {
        int retChar = this.getUserInputValue();
        
        // Remove the "more" message and replace it by an empty line
        this.clear(1);

        return retChar;
    }

    /**
     * Gets the dimensions (width and height) of a Scilab console
     * @return the size of the console
     */
    public Size getDims() {
        return new Size(super.getWidth(), super.getHeight());
    }

    /**
     * Gets the position (X-coordinate and Y-coordinates) of a Scilab console
     * @return the position of the console
     */
    public Position getPosition() {
        return new Position(this.getX(), this.getY());
    }

    /**
     * Gets the visibility status of a console
     * @return the visibility status of the console (true if the console is visible, false if not)
     */
    public boolean isVisible() {
        return super.isVisible();
    }

    /**
     * Sets the dimensions (width and height) of a Scilab console
     * @param newSize the size we want to set to the console
     */
    public void setDims(Size newSize) {
        log.info("setDims: " + newSize.getWidth() + " " + newSize.getHeight());
        this.setPreferredSize(new Dimension(newSize.getWidth(), newSize.getHeight()));
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a Scilab console
     * @param newPosition the position we want to set to the console
     */
    public void setPosition(Position newPosition) {
        log.info("setPosition: " + newPosition.getX() + " " + newPosition.getY());
        this.setLocation(newPosition.getX(), newPosition.getY());
    }

    /**
     * Sets the visibility status of a Scilab console
     * @param newVisibleState the visibility status we want to set to the console
     */
    public void setVisible(boolean newVisibleState) {
        super.setVisible(newVisibleState);
    }

    public void close() {
        closeConsole();
    }

    /**
     * Clears the Console
     */
    public void clear() {
        SwingUtilities.invokeLater(() -> {
            CheckClearConfirmation ccc = XConfiguration.get(CheckClearConfirmation.class, XConfiguration.getXConfigurationDocument(), CONFIRMATION_PATH)[0];
            if (ccc.checked) {
                final boolean[] checked = new boolean[1];
                final Action action = new AbstractAction() {
                    public void actionPerformed(ActionEvent e) {
                        checked[0] = ((JCheckBox) e.getSource()).isSelected();
                    }
                };

                AnswerOption ans = ScilabModalDialog.show(SwingScilabConsole.this, new String[] { CLEAR_CONFIRM }, CLEAR, IconType.WARNING_ICON, ButtonType.YES_NO, DONT_SHOW, action);
                if (checked[0]) {
                    XConfiguration.set(XConfiguration.getXConfigurationDocument(), CONFIRMATION_PATH + "/@state", "unchecked");
                }

                if (ans == AnswerOption.YES_OPTION)
                {
                    SwingScilabConsole.super.clear();
                }
            } else {
                SwingScilabConsole.super.clear();
            }
        });
    }

    /**
     * Paste clipboard contents in Console input line
     */
    public void pasteClipboard() {
        try {
            Transferable t = Toolkit.getDefaultToolkit().getSystemClipboard().getContents(null);
            Object data = t.getTransferData(DataFlavor.stringFlavor);
            sendToHTML("paste", data.toString());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * Launch the help on the selected text into the Console
     */
    public void helpOnTheKeyword() {
        String keyword = getSelectedText();
        /* Double the quote/double quote in order to avoid
         * and error with the call of doc()
         */
        keyword = keyword.replaceAll("'", "''");
        keyword = keyword.replaceAll("\"", "\"\"");

        InterpreterManagement.requestScilabExec("doc('" + keyword + "')");
    }

    /**
     * Evaluate the selection with echo
     */
    public void evaluateSelectionWithEcho() {
        String selection = getSelectedText();
        if (selection.compareTo("") != 0) {
            StringTokenizer tokens = new StringTokenizer(selection, "\n");
            String[] lines = new String[tokens.countTokens()];
            int i = 0;
            while (tokens.hasMoreTokens()) {
                lines[i++] = tokens.nextToken();
            }
            HistoryManagement.appendLinesToScilabHistory(lines, lines.length);
            sendCommandsToScilab(selection, true, false);
        }
    }

    /**
     * Evaluate the selection with no echo
     */
    public void evaluateSelectionWithNoEcho() {
        sendCommandsToScilab(getSelectedText(), false, false);
    }

    /**
     * Put the console selected text in the clipboard
     */
    public void copyToClipboard() {
        sendToHTML("copy");
    }

    /**
     * Cut selected text in the Console input line
     */
    public void cutSelection() {
        sendToHTML("cut");
    }

    /**
     * Set the maximum number of lines stored in the Output
     * @param nbLines the number of lines
     */
    public void setMaxOutputSize(int nbLines) {
        setMaxSize(nbLines);
    }

    @XConfAttribute
    private static class CheckClearConfirmation {

        public boolean checked;

        private CheckClearConfirmation() { }

        @XConfAttribute(attributes = {"state"})
        private void set(String checked) {
            this.checked = checked.equals("checked");
        }
    }

    public String getClipboardContents() {
        return CallScilabBridge.getClipboardContents();
    }
}
