/* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xpad;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.nio.charset.Charset;
import java.util.List;
import java.util.Map;
import java.util.HashMap;

import javax.swing.ButtonGroup;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.JEditorPane;
import javax.swing.text.DefaultEditorKit;
import javax.swing.KeyStroke;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.xpad.actions.ActivateHelpOnTypingAction;
import org.scilab.modules.xpad.actions.GenerateHelpFromFunctionAction;
import org.scilab.modules.xpad.actions.AboutAction;
import org.scilab.modules.xpad.actions.AutoIndentAction;
import org.scilab.modules.xpad.actions.CloseAction;
import org.scilab.modules.xpad.actions.CloseAllAction;
import org.scilab.modules.xpad.actions.CloseAllButThisAction;
import org.scilab.modules.xpad.actions.CommentAction;
import org.scilab.modules.xpad.actions.UnCommentAction;
import org.scilab.modules.xpad.actions.CopyAction;
import org.scilab.modules.xpad.actions.CutAction;
import org.scilab.modules.xpad.actions.DeleteAction;
import org.scilab.modules.xpad.actions.EncodingAction;
import org.scilab.modules.xpad.actions.EndOfLineAction;
import org.scilab.modules.xpad.actions.EvaluateSelectionAction;
import org.scilab.modules.xpad.actions.ExecuteFileIntoScilabAction;
import org.scilab.modules.xpad.actions.ExitAction;
import org.scilab.modules.xpad.actions.FindAction;
import org.scilab.modules.xpad.actions.FindNextAction;
import org.scilab.modules.xpad.actions.FindPreviousAction;
import org.scilab.modules.xpad.actions.GotoLineAction;
import org.scilab.modules.xpad.actions.HelpAction;
import org.scilab.modules.xpad.actions.HelpOnKeywordAction;
import org.scilab.modules.xpad.actions.HighlightCurrentLineAction;
import org.scilab.modules.xpad.actions.IndentAction;
import org.scilab.modules.xpad.actions.LineNumbersAction;
import org.scilab.modules.xpad.actions.LoadIntoScilabAction;
import org.scilab.modules.xpad.actions.NewAction;
import org.scilab.modules.xpad.actions.OpenAction;
import org.scilab.modules.xpad.actions.OpenSourceFileOnKeywordAction;
import org.scilab.modules.xpad.actions.PageSetupAction;
import org.scilab.modules.xpad.actions.PasteAction;
import org.scilab.modules.xpad.actions.PrintAction;
import org.scilab.modules.xpad.actions.PrintPreviewAction;
import org.scilab.modules.xpad.actions.RecentFileAction;
import org.scilab.modules.xpad.actions.RedoAction;
import org.scilab.modules.xpad.actions.ResetFontAction;
import org.scilab.modules.xpad.actions.SaveAction;
import org.scilab.modules.xpad.actions.SaveAllAction;
import org.scilab.modules.xpad.actions.SaveAsAction;
import org.scilab.modules.xpad.actions.SelectAllAction;
import org.scilab.modules.xpad.actions.SetColorsAction;
import org.scilab.modules.xpad.actions.SetFontAction;
import org.scilab.modules.xpad.actions.TabifyAction;
import org.scilab.modules.xpad.actions.UnTabifyAction;
import org.scilab.modules.xpad.actions.UndoAction;
import org.scilab.modules.xpad.utils.ConfigXpadManager;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * Class XpadGUI handles menus, bar, ...
 */
public class XpadGUI {

        private static JRadioButtonMenuItem[] radioTypes;
        private static JRadioButtonMenuItem[] radioEolTypes;
        private static MenuItem evaluateSelectionMenuItem;
        private static TextBox infoBar;
        private static Map<String, KeyStroke> map;

        /**
         * Constructor
         * @param mainWindow Windows
         * @param editorInstance Xpad
         * @param title the title
         */
        public XpadGUI(Window mainWindow, Xpad editorInstance, String title) {

                mainWindow.setTitle(title);
                mainWindow.addTab(editorInstance);

                // Set Xpad Window position /size
                mainWindow.setPosition(ConfigXpadManager.getMainWindowPosition());
                mainWindow.setDims(ConfigXpadManager.getMainWindowSize());

                map = new HashMap();
                ConfigXpadManager.addMapActionNameKeys(map);

                MenuBar menuBar = ScilabMenuBar.createMenuBar();
                //Create FILE menubar
                Menu fileMenu = ScilabMenu.createMenu();
                createFileMenu(fileMenu, editorInstance);
                menuBar.add(fileMenu);

                //Create EDIT menubar
                Menu editMenu = ScilabMenu.createMenu();
                createEditMenu(editMenu, editorInstance);
                menuBar.add(editMenu);

                Menu toolsMenu = ScilabMenu.createMenu();
                createToolsMenu(toolsMenu, editorInstance);
                menuBar.add(toolsMenu);

                // Create SEARCH menubar
                Menu searchMenu = ScilabMenu.createMenu();
                searchMenu.setText(XpadMessages.SEARCH);
                searchMenu.setMnemonic('S');
                searchMenu.add(FindAction.createMenu(editorInstance, map.get("FindAction")));
                searchMenu.add(FindNextAction.createMenu(editorInstance, map.get("FindNextAction")));
                searchMenu.add(FindPreviousAction.createMenu(editorInstance, map.get("FindPreviousAction")));
                searchMenu.add(GotoLineAction.createMenu(editorInstance));
                menuBar.add(searchMenu);

                // Create VIEW Menubar
                Menu viewMenu = ScilabMenu.createMenu();
                viewMenu.setText(XpadMessages.VIEW);
                viewMenu.setMnemonic('S');
                viewMenu.add(HighlightCurrentLineAction.createCheckBoxMenu(editorInstance, map.get("HighlightCurrentLineAction")));
                viewMenu.add(LineNumbersAction.createMenu(editorInstance, map.get("LineNumbersAction")));
                viewMenu.add(SetColorsAction.createMenu(editorInstance));
                viewMenu.add(SetFontAction.createMenu(editorInstance));
                viewMenu.add(ResetFontAction.createMenu(editorInstance));
                menuBar.add(viewMenu);

                // Create DOCUMENT MenuBar
                Menu documentMenu = ScilabMenu.createMenu();
                documentMenu.setText(XpadMessages.DOCUMENT);
                documentMenu.setMnemonic('D');
                Menu syntaxTypeMenu = ScilabMenu.createMenu();

                createEncodingSubMenu(documentMenu, editorInstance);
                createEolSubMenu(documentMenu, editorInstance);

                documentMenu.addSeparator();
                documentMenu.add(AutoIndentAction.createCheckBoxMenu(editorInstance));
                menuBar.add(documentMenu);

                // Create EXECUTE menubar
                Menu executeMenu = ScilabMenu.createMenu();
                executeMenu.setText(XpadMessages.EXECUTE);
                executeMenu.setMnemonic('e');
                executeMenu.add(LoadIntoScilabAction.createMenu(editorInstance, map.get("LoadIntoScilabAction")));
                evaluateSelectionMenuItem = EvaluateSelectionAction.createMenu(editorInstance, map.get("EvaluateSelectionAction"));
                executeMenu.add(evaluateSelectionMenuItem);
                executeMenu.add(ExecuteFileIntoScilabAction.createMenu(editorInstance, map.get("ExecuteFileIntoScilabAction")));
                menuBar.add(executeMenu);

                //Create HELP menubar
                Menu helpMenu = ScilabMenu.createMenu();
                helpMenu.setText(XpadMessages.QUESTION_MARK);
                helpMenu.add(HelpAction.createMenu(editorInstance));
                helpMenu.add(HelpOnKeywordAction.createMenu(editorInstance, map.get("HelpOnKeywordAction")));
                helpMenu.add(AboutAction.createMenu(editorInstance));
                menuBar.add(helpMenu);

                // Create Toolbar
                createToolBar(editorInstance);

                infoBar = ScilabTextBox.createTextBox();

                editorInstance.setMenuBar(menuBar);
                editorInstance.setInfoBar(infoBar);
                mainWindow.setTitle(title);
                mainWindow.setVisible(true);
        }

        /**
         * Update the selected item in the encoding pull down menu of the document.
         * @param scilabDocument the document for which the encoding menu should
         * be updated
         */
        public void updateEncodingMenu(ScilabDocument scilabDocument) {
            if (radioTypes != null) {
                for (int k = 0; k < radioTypes.length; k++) {
                    if (scilabDocument.getEncoding().equals(radioTypes[k].getText())) {
                        radioTypes[k].setSelected(true);
                    }
                }
            }
        }

        /**
         * Update the selected item in the EOL pull down menu of the document.
         * @param scilabDocument the document for which the End Of Line menu should
         * be updated
         */
        public void updateEolMenu(ScilabDocument scilabDocument) {
                String eolLinux = ScilabDocument.EOLUNIX;
                String eolMacOs = ScilabDocument.EOLMAC;
                String eolWindows = ScilabDocument.EOLWIN;

                String eolUsedLabel = XpadMessages.EOL_LINUX;
                String eolUsed = scilabDocument.getEOL();

                if (eolUsed.compareTo(eolLinux) == 0) {
                    eolUsedLabel = XpadMessages.EOL_LINUX;
                }

                if (eolUsed.compareTo(eolMacOs) == 0) {
                    eolUsedLabel = XpadMessages.EOL_MACOS;
                }

                if (eolUsed.compareTo(eolWindows) == 0) {
                    eolUsedLabel = XpadMessages.EOL_WINDOWS;
                }

                for (int k = 0; k < radioEolTypes.length; k++) {
                    if (radioEolTypes[k].getText().compareTo(eolUsedLabel) == 0) {
                        radioEolTypes[k].setSelected(true);
                    }
                }
        }

        /**
         * create End Of Line sub Menu
         * @param documentMenu Menu
         * @param editorInstance Xpad
         */
        private void createEolSubMenu(Menu documentMenu, Xpad editorInstance) {

                String eolLinux = ScilabDocument.EOLUNIX;
                String eolMacOs = ScilabDocument.EOLMAC;
                String eolWindows = ScilabDocument.EOLWIN;
                String defaultEolLabel = XpadMessages.EOL_LINUX;

                // selected by default O.S
                String systemEolValue = System.getProperty("line.separator");

                if (systemEolValue.compareTo(eolLinux) == 0) {
                        defaultEolLabel = XpadMessages.EOL_LINUX;
                }

                if (systemEolValue.compareTo(eolWindows) == 0) {
                        defaultEolLabel = XpadMessages.EOL_WINDOWS;
                }

                if (systemEolValue.compareTo(eolMacOs) == 0) {
                        defaultEolLabel = XpadMessages.EOL_MACOS;
                }

                Menu eolTypeMenu = ScilabMenu.createMenu();
                eolTypeMenu.setText(XpadMessages.EOL_TYPE);
                documentMenu.add(eolTypeMenu);

                radioEolTypes = new JRadioButtonMenuItem[3];
                ButtonGroup groupEol = new ButtonGroup();

                radioEolTypes[0] =  (new EndOfLineAction(XpadMessages.EOL_LINUX, editorInstance)).createRadioButtonMenuItem(editorInstance);
                groupEol.add(radioEolTypes[0]);
                ((JMenu) eolTypeMenu.getAsSimpleMenu()).add(radioEolTypes[0]);

                radioEolTypes[1] =  (new EndOfLineAction(XpadMessages.EOL_WINDOWS, editorInstance)).createRadioButtonMenuItem(editorInstance);
                groupEol.add(radioEolTypes[1]);
                ((JMenu) eolTypeMenu.getAsSimpleMenu()).add(radioEolTypes[1]);

                radioEolTypes[2] =  (new EndOfLineAction(XpadMessages.EOL_MACOS, editorInstance)).createRadioButtonMenuItem(editorInstance);
                groupEol.add(radioEolTypes[2]);
                ((JMenu) eolTypeMenu.getAsSimpleMenu()).add(radioEolTypes[2]);

                // selected menu
                for (int k = 0; k < radioEolTypes.length; k++) {
                        if (radioEolTypes[k].getText().compareTo(defaultEolLabel) == 0) {
                                radioEolTypes[k].setSelected(true);
                        }
                }
        }

        /**
         * Create TOOLBAR
         * @param editorInstance Xpad
         */
        private void createToolBar(Xpad editorInstance) {

                ToolBar toolBar = ScilabToolBar.createToolBar();
                toolBar.add(NewAction.createButton(editorInstance)); // NEW
                toolBar.add(OpenAction.createButton(editorInstance)); // OPEN
                toolBar.addSeparator();
                toolBar.add(SaveAction.createButton(editorInstance)); // SAVE
                toolBar.add(SaveAsAction.createButton(editorInstance)); // SAVE AS
                toolBar.addSeparator();
                toolBar.add(PrintAction.createButton(editorInstance)); // PRINT
                toolBar.addSeparator();
                toolBar.add(UndoAction.createButton(editorInstance));
                toolBar.add(RedoAction.createButton(editorInstance));
                toolBar.addSeparator();
                toolBar.add(CutAction.createButton(editorInstance)); // CUT
                toolBar.add(CopyAction.createButton(editorInstance)); // COPY
                toolBar.add(PasteAction.createButton(editorInstance)); // PASTE
                toolBar.addSeparator();
                toolBar.add(FindAction.createButton(editorInstance)); // FIND / REPLACE

                editorInstance.setToolBar(toolBar);
        }

        /**
         * createEditMenu
         * @param editMenu Menu
         * @param editorInstance Xpad
         */
        private void createEditMenu(Menu editMenu, Xpad editorInstance) {
                editMenu.setText(XpadMessages.EDIT);
                editMenu.setMnemonic('E');
                editMenu.add(UndoAction.createMenu(editorInstance, map.get("UndoAction")));
                editMenu.add(RedoAction.createMenu(editorInstance, map.get("RedoAction")));
                editMenu.addSeparator();
                editMenu.add(CutAction.createMenu(editorInstance, map.get("CutAction")));
                editMenu.add(CopyAction.createMenu(editorInstance, map.get("CopyAction")));
                editMenu.add(PasteAction.createMenu(editorInstance, map.get("PasteAction")));
                editMenu.addSeparator();
                editMenu.add(SelectAllAction.createMenu(editorInstance, map.get("SelectAllAction")));
                editMenu.add(DeleteAction.createMenu(editorInstance));
        }

        /**
         * createToolsMenu
         * @param toolsMenu Menu
         * @param editorInstance Xpad
         */
        private void createToolsMenu(Menu toolsMenu, Xpad editorInstance) {
                toolsMenu.setText(XpadMessages.TOOLS);
                toolsMenu.setMnemonic('o');
                toolsMenu.add(ActivateHelpOnTypingAction.createCheckBoxMenu(editorInstance, map.get("ActivateHelpOnTypingAction")));
                toolsMenu.addSeparator();
                toolsMenu.add(CommentAction.createMenu(editorInstance, map.get("CommentAction")));
                toolsMenu.add(UnCommentAction.createMenu(editorInstance, map.get("UnCommentAction")));
                toolsMenu.addSeparator();
                toolsMenu.add(TabifyAction.createMenu(editorInstance, map.get("TabifyAction")));
                toolsMenu.add(UnTabifyAction.createMenu(editorInstance, map.get("UnTabifyAction")));
                toolsMenu.addSeparator();
                toolsMenu.add(IndentAction.createMenu(editorInstance, map.get("IndentAction")));
                toolsMenu.addSeparator();
                toolsMenu.add(GenerateHelpFromFunctionAction.createMenu(editorInstance, map.get("GenerateHelpFromFunctionAction")));
        }

        /**
         * createFileMenu
         * @param fileMenu Menu
         * @param editorInstance Xpad
         */
        private void createFileMenu(Menu fileMenu, Xpad editorInstance) {
                List<File> recentFiles = ConfigXpadManager.getAllRecentOpenedFiles();

                fileMenu.setText(XpadMessages.FILE);
                fileMenu.setMnemonic('F');
                fileMenu.add(NewAction.createMenu(editorInstance, map.get("NewAction")));
                fileMenu.add(OpenAction.createMenu(editorInstance, map.get("OpenAction")));
                fileMenu.add(OpenSourceFileOnKeywordAction.createMenu(editorInstance, map.get("OpenSourceFileOnKeywordAction")));

                Menu recentsMenu = editorInstance.getRecentsMenu();
                recentsMenu.setText(XpadMessages.RECENT_FILES);
                for (int i = 0; i < recentFiles.size(); i++) {
                        recentsMenu.add(RecentFileAction.createMenu(editorInstance , recentFiles.get(i)));
                }

                fileMenu.add(recentsMenu);

                fileMenu.addSeparator();
                fileMenu.add(SaveAction.createMenu(editorInstance, map.get("SaveAction")));
                fileMenu.add(SaveAsAction.createMenu(editorInstance, map.get("SaveAsAction")));
                fileMenu.add(SaveAllAction.createMenu(editorInstance, map.get("SaveAllAction")));
                fileMenu.addSeparator();
                fileMenu.add(PageSetupAction.createMenu(editorInstance));
                fileMenu.add(PrintPreviewAction.createMenu(editorInstance, map.get("PrintPreviewAction")));
                fileMenu.add(PrintAction.createMenu(editorInstance, map.get("PrintAction")));
                fileMenu.addSeparator();
                fileMenu.add(CloseAction.createMenu(editorInstance, map.get("CloseAction")));
                fileMenu.add(CloseAllAction.createMenu(editorInstance));
                fileMenu.add(CloseAllButThisAction.createMenu(editorInstance));
                fileMenu.addSeparator();
                fileMenu.add(ExitAction.createMenu(editorInstance, map.get("ExitAction")));
        }

        /**
         * createEncodingSubMenu
         * @param documentMenu Menu
         * @param editorInstance Xpad
         */
        private void createEncodingSubMenu(Menu documentMenu, Xpad editorInstance) {

                Menu encodingTypeMenu = ScilabMenu.createMenu();
                encodingTypeMenu.setText(XpadMessages.ENCODING_TYPE);
                documentMenu.add(encodingTypeMenu);

                List<String> encodings = EncodingAction.getEcodings();

                radioTypes = new JRadioButtonMenuItem[encodings.size()];
                ButtonGroup group = new ButtonGroup();
                for (int i = 0; i < encodings.size(); i++) {
                        radioTypes[i] = (new EncodingAction(encodings.get(i).toString(), editorInstance)).createRadioButtonMenuItem(editorInstance);
                        group.add(radioTypes[i]);
                        ((JMenu) encodingTypeMenu.getAsSimpleMenu()).add(radioTypes[i]);

                        if (encodings.get(i).toString().equals(Charset.defaultCharset().toString())) {
                                radioTypes[i].setSelected(true);
                        }
                }
        }

        public static MenuItem getEvaluateSelectionMenuItem() {
                return evaluateSelectionMenuItem;
        }

        /**
         * Create the popup menu on the help
         * @param c The graphic component
         */
        public static void createPopupMenu(final JEditorPane c) {

                final JPopupMenu popup = new JPopupMenu();

                JMenuItem menuItem = null;

                /* Execute into Scilab */
                ActionListener actionListenerExecuteIntoScilab = new ActionListener() {
                        public void actionPerformed(ActionEvent actionEvent) {
                                String selection = ((ScilabEditorPane) c).getCodeToExecute();
                                if (selection == null) {
                                        infoBar.setText(Messages.gettext("No text to execute"));
                                } else {
                                        ScilabConsole.getConsole().getAsSimpleConsole().sendCommandsToScilab(selection, true /* display */, true /* store in history */);
                                }
                        }
                };
                menuItem = new JMenuItem(XpadMessages.EVALUATE_SELECTION);
                menuItem.addActionListener(actionListenerExecuteIntoScilab);
                if (!ScilabConsole.isExistingConsole()) { /* Only available in STD mode */
                        menuItem.setEnabled(false);
                }
                popup.add(menuItem);


                /* Edit in the Scilab Text Editor */
                ActionListener actionListenerLoadIntoTextEditor = new ActionListener() {
                        public void actionPerformed(ActionEvent actionEvent) {
                                String selection = c.getSelectedText();
                                if (selection == null) {
                                        infoBar.setText(Messages.gettext("No text selected"));
                                } else {
                                        Xpad.xpadWithText(selection);
                                }
                        }
                };



                menuItem = new JMenuItem(Messages.gettext("Edit selection in a new tab"));
                menuItem.addActionListener(actionListenerLoadIntoTextEditor);
                popup.add(menuItem);
                popup.addSeparator();


                /* Copy */
                menuItem = new JMenuItem(new DefaultEditorKit.CopyAction());
                menuItem.setText(Messages.gettext("Copy"));
                popup.add(menuItem);
                popup.addSeparator();

                /* Cut */
                menuItem = new JMenuItem(new DefaultEditorKit.CutAction());
                menuItem.setText(Messages.gettext("Cut"));
                popup.add(menuItem);
                popup.addSeparator();

                /* Paste */
                menuItem = new JMenuItem(new DefaultEditorKit.PasteAction());
                menuItem.setText(Messages.gettext("Paste"));
                popup.add(menuItem);
                popup.addSeparator();

                /* Select all */
                ActionListener actionListenerSelectAll = new ActionListener() {
                        public void actionPerformed(ActionEvent actionEvent) {
                                c.selectAll();
                        }
                };
                menuItem = new JMenuItem(Messages.gettext("Select All"));
                menuItem.addActionListener(actionListenerSelectAll);
                popup.add(menuItem);

                /* Edit in the Scilab Text Editor */
                final JMenuItem helpMenuItem = new JMenuItem("Help on the selected text");

                ActionListener actionListenerHelpOnKeyword = new ActionListener() {
                        public void actionPerformed(ActionEvent actionEvent) {
                                String selection = c.getSelectedText();
                                if (selection == null) {
                                        infoBar.setText(Messages.gettext("No text selected"));
                                } else {
                                        /* Double the quote/double quote in order to avoid
                                         * and error with the call of help()
                                         */
                                        selection = selection.replaceAll("'", "''");
                                        selection = selection.replaceAll("\"", "\"\"");

                                        /* @TODO: Check if it is possible to call directly
                                         * from the Java engine the help
                                         * Last time I check, we needed some information
                                         * provided by the Scilab native engine
                                         */
                                        InterpreterManagement.requestScilabExec("help('" + selection + "')");
                                }
                        }
                };

                /* Not sure it is the best listener */
                PropertyChangeListener listenerTextItem = new PropertyChangeListener() {
                        public void propertyChange(PropertyChangeEvent arg0) {
                                String keyword = c.getSelectedText();
                                if (keyword == null) {
                                        helpMenuItem.setText(Messages.gettext("Help about a selected text"));
                                } else {
                                        int nbOfDisplayedOnlyXChar = 10;
                                        if (keyword.length() > nbOfDisplayedOnlyXChar) {
                                                keyword = keyword.substring(0, nbOfDisplayedOnlyXChar) + "...";
                                        }
                                        helpMenuItem.setText(Messages.gettext("Help about '") + keyword + "'");
                                }
                        }
                };
                helpMenuItem.addPropertyChangeListener(listenerTextItem);
                helpMenuItem.addActionListener(actionListenerHelpOnKeyword);
                popup.add(helpMenuItem);

                /* Creates the Popupmenu on the component */
                c.setComponentPopupMenu(popup);
        }
}
