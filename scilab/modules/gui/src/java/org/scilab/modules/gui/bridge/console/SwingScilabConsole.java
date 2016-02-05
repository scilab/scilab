/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

import java.awt.Component;
import java.awt.Dimension;
import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.StringSelection;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.event.ActionEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.KeyboardFocusManager;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.IOException;
import java.util.StringTokenizer;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.JCheckBox;
import javax.swing.JEditorPane;
import javax.swing.JPanel;
import javax.swing.JTextPane;
import javax.swing.SwingUtilities;
import javax.swing.text.BadLocationException;
import javax.swing.text.StyleContext;
import javax.swing.text.StyledDocument;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.console.OneCharKeyEventListener;
import org.scilab.modules.console.SciConsole;
import org.scilab.modules.console.SciHistoryManager;
import org.scilab.modules.console.SciInputCommandView;
import org.scilab.modules.console.SciOutputView;
import org.scilab.modules.graphic_objects.graphicObject.CallBack;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.console.SimpleConsole;
import org.scilab.modules.gui.events.callback.ScilabCallBack;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.history_manager.HistoryManagement;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.commons.xml.XConfiguration;

import static org.scilab.modules.commons.xml.XConfiguration.XConfAttribute;

import com.artenum.rosetta.interfaces.ui.InputCommandView;
import com.artenum.rosetta.util.StringConstants;

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

    /**
     * Constructor
     */
    public SwingScilabConsole() {
        super(ConfigManager.getUserConfigFile());

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

        ((JEditorPane) getConfiguration().getOutputView()).setComponentPopupMenu(menu);
        ((JTextPane) getConfiguration().getInputCommandView()).setComponentPopupMenu(menu);
        ((JPanel) getConfiguration().getPromptView()).setComponentPopupMenu(menu);

        ((JTextPane) getConfiguration().getInputCommandView()).requestFocus();

        addFocusListener(new FocusListener() {
            public void focusGained(FocusEvent e) {
                ((JTextPane) getConfiguration().getInputCommandView()).requestFocus();
            }

            public void focusLost(FocusEvent e) { }
        });
    }

    /**
     * Displays data in the console
     * @param dataToDisplay the data to be displayed
     * @see fr.scilab.console.HelpBrowser#display(java.lang.String)
     */
    public void display(String dataToDisplay) {
        this.getConfiguration().getOutputView().append(dataToDisplay);
    }

    /**
     * This method is used to display the prompt
     */
    public void displayPrompt() {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                InputCommandView inputCmdView = SwingScilabConsole.this.getConfiguration().getInputCommandView();

                // Show the prompt
                SwingScilabConsole.this.getConfiguration().getPromptView().setVisible(true);

                // Show the input command view and its hidden components
                inputCmdView.setEditable(true);
                JTextPane tp = (JTextPane) inputCmdView;
                if (tp.isFocusOwner()) {
                    tp.getCaret().setVisible(true);
                }
                setToHome();
            }
        });

        ((SciOutputView) this.getConfiguration().getOutputView()).resetLastEOL();

        updateScrollPosition();
    }

    /**
     * Unblock the console if needed
     */
    public void unblock() {
        if (getCanReadUserInputValue().availablePermits() == 0) {
            setUserInputValue((int) 'n');
        }

        // interrupt any mscanf call (input, halt and so on)
        ((SciInputCommandView) this.getConfiguration().getInputCommandView()).interrupt();
    }

    /**
     * Reads one user input char
     * @return the data entered by the user
     * @see fr.scilab.console.HelpBrowser#getCharWithoutOutput()
     */
    public int getCharWithoutOutput() {
        int retChar;

        updateScrollPosition();

        // Avoids reading of an empty buffer
        try {
            ((SciConsole) this).getCanReadUserInputValue().acquire();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        this.getConfiguration().getPromptView().setVisible(false);
        this.getConfiguration().getInputCommandView().setEditable(false);

        // Add a keylistener which will set the returned char
        OneCharKeyEventListener keyListener = new OneCharKeyEventListener(this);
        ((JTextPane) this.getConfiguration().getInputCommandView()).addKeyListener(keyListener);
        ((JEditorPane) this.getConfiguration().getOutputView()).addKeyListener(keyListener);

        // Reads the buffer
        retChar = this.getUserInputValue();
        ((SciConsole) this).getCanReadUserInputValue().release();

        // Remove the "more" message and replace it by an empty line
        this.clear(-1);
        this.display(StringConstants.NEW_LINE);

        // Remove the key listener
        ((JTextPane) this.getConfiguration().getInputCommandView()).removeKeyListener(keyListener);
        ((JEditorPane) this.getConfiguration().getOutputView()).removeKeyListener(keyListener);

        this.getConfiguration().getPromptView().setVisible(true);
        this.getConfiguration().getInputCommandView().setEditable(true);

        // Send back the focus of the input view
        this.getConfiguration().getInputCommandView().reset();
        this.getConfiguration().getInputCommandView().requestFocus();

        final JTextPane cmdView = (JTextPane) this.getConfiguration().getInputCommandView();
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                cmdView.getCaret().setVisible(true);
            }
        });

        return retChar;
    }

    /**
     * Draw a console
     */
    public void draw() {
        super.setVisible(true);
        super.doLayout();
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
        this.setPreferredSize(new Dimension(newSize.getWidth(), newSize.getHeight()));
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a Scilab console
     * @param newPosition the position we want to set to the console
     */
    public void setPosition(Position newPosition) {
        this.setLocation(newPosition.getX(), newPosition.getY());
    }

    /**
     * Sets the visibility status of a Scilab console
     * @param newVisibleState the visibility status we want to set to the console
     */
    public void setVisible(boolean newVisibleState) {
        super.setVisible(newVisibleState);
    }

    /**
     * Clears the Console
     */
    public void clear() {
        CheckClearConfirmation ccc = XConfiguration.get(CheckClearConfirmation.class, XConfiguration.getXConfigurationDocument(), CONFIRMATION_PATH)[0];
        if (ccc.checked) {
            final Component focused = KeyboardFocusManager.getCurrentKeyboardFocusManager().getFocusOwner();
            final boolean[] checked = new boolean[1];
            final Action action = new AbstractAction() {
                public void actionPerformed(ActionEvent e) {
                    checked[0] = ((JCheckBox) e.getSource()).isSelected();
                }
            };

            boolean isNo = ScilabModalDialog.show(this, new String[] { CLEAR_CONFIRM }, CLEAR, IconType.WARNING_ICON, ButtonType.YES_NO, DONT_SHOW, action) == AnswerOption.NO_OPTION;
            if (focused != null) {
                SwingUtilities.invokeLater(new Runnable() {
                    public void run() {
                        focused.requestFocus();
                    }
                });
            }

            if (isNo) {
                if (checked[0]) {
                    XConfiguration.set(XConfiguration.getXConfigurationDocument(), CONFIRMATION_PATH + "/@state", "unchecked");
                }
                return;
            }

            if (checked[0]) {
                XConfiguration.set(XConfiguration.getXConfigurationDocument(), CONFIRMATION_PATH + "/@state", "unchecked");
            }
        }

        super.clear();
    }

    /**
     * Sets the prompt displayed in the console
     * @param prompt the prompt to be displayed in the console
     */
    public void setPrompt(String prompt) {
        this.getConfiguration().getPromptView().setDefaultPrompt(prompt);
    }

    /**
     * Clear the commands history
     */
    public void clearHistory() {
        ((SciHistoryManager) this.getConfiguration().getHistoryManager()).reset();
    }

    /**
     * Paste clipboard contents in Console input line
     */
    public void pasteClipboard() {
        // Gets the contents of the clipboard
        Toolkit toolkit = Toolkit.getDefaultToolkit();
        Clipboard systemClipboard = toolkit.getSystemClipboard();

        // Verify that clibpboard data is of text type
        boolean dataAvailable;
        try {
            dataAvailable = systemClipboard.isDataFlavorAvailable(DataFlavor.stringFlavor);
        } catch (IllegalStateException exception) {
            return;
        }

        // Exit if text data not available
        if (!dataAvailable) {
            return;
        }

        // Read data
        String clipboardContents = null;
        try {
            clipboardContents = (String) systemClipboard.getData(DataFlavor.stringFlavor);
        } catch (UnsupportedFlavorException e1) {
            // Should never be here
            e1.printStackTrace();
        } catch (IOException e1) {
            // Should never be here
            e1.printStackTrace();
        }

        JTextPane input = ((JTextPane) this.getConfiguration().getInputCommandView());
        StyledDocument doc = input.getStyledDocument();

        // If some text selected then it is replaced
        if (input.getSelectedText() != null) {
            try {
                doc.remove(input.getSelectionStart(), input.getSelectionEnd() - input.getSelectionStart());
            } catch (BadLocationException e) {
                e.printStackTrace();
            }
        }
        // Insert clipboard contents
        try {
            doc.insertString(((JTextPane) this.getConfiguration().getInputCommandView()).getCaretPosition(),
                             clipboardContents, doc.getStyle(StyleContext.DEFAULT_STYLE));
        } catch (BadLocationException e) {
            e.printStackTrace();
        }
    }

    /**
     * Select all the console contents
     */
    public void selectAll() {
        JEditorPane output = (JEditorPane) this.getConfiguration().getOutputView();
        output.setSelectionStart(0);
        output.setSelectionEnd(output.getText().length());
        // TODO should also select the prompt and the input
    }

    /**
     * Return the selected text in the console
     * @return The selected text in the console
     */
    private String getSelectedText() {
        JEditorPane output = (JEditorPane) this.getConfiguration().getOutputView();
        JTextPane input = (JTextPane) this.getConfiguration().getInputCommandView();

        String selection = "";
        if (output.getSelectedText() != null) {
            selection += output.getSelectedText();
        }
        // TODO should also copy the prompt
        if (input.getSelectedText() != null) {
            selection += input.getSelectedText();
        }
        return selection;

    }

    /**
     * Launch the help on the selected text into the Console
     */
    public void helpOnTheKeyword() {
        String keyword = getSelectedText();
        /* Double the quote/double quote in order to avoid
         * and error with the call of help()
         */
        keyword = keyword.replaceAll("'", "''");
        keyword = keyword.replaceAll("\"", "\"\"");

        /* @TODO: Check if it is possible to call directly
         * from the Java engine the help
         * Last time I check, we needed some information
         * provided by the Scilab native engine
         */
        InterpreterManagement.requestScilabExec("help('" + keyword + "')");
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
        String selection = getSelectedText();
        Toolkit.getDefaultToolkit().getSystemClipboard().setContents(new StringSelection(selection), null);
    }

    /**
     * Cut selected text in the Console input line
     */
    public void cutSelection() {
        JTextPane input = (JTextPane) this.getConfiguration().getInputCommandView();
        StyledDocument doc = input.getStyledDocument();

        // If some text selected then it is replaced
        if (input.getSelectedText() != null) {
            try {
                /* Put the selection in the clipboard */
                Toolkit.getDefaultToolkit().getSystemClipboard().setContents(new StringSelection(input.getSelectedText()), null);
                /* Remove selected text */
                doc.remove(input.getSelectionStart(), input.getSelectionEnd() - input.getSelectionStart());
            } catch (BadLocationException e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * Set the maximum number of lines stored in the Output
     * @param nbLines the number of lines
     */
    public void setMaxOutputSize(int nbLines) {
        ((SciOutputView) this.getConfiguration().getOutputView()).setMaxSize(nbLines);
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
}
