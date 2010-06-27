/* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
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
import javax.swing.text.BadLocationException;
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
import org.scilab.modules.scinotes.actions.SplitHorizontallyAction;
import org.scilab.modules.scinotes.actions.SplitVerticallyAction;
import org.scilab.modules.scinotes.actions.NoSplitAction;
import org.scilab.modules.scinotes.actions.ActivateHelpOnTypingAction;
import org.scilab.modules.scinotes.actions.GenerateHelpFromFunctionAction;
import org.scilab.modules.scinotes.actions.AboutAction;
import org.scilab.modules.scinotes.actions.AutoIndentAction;
import org.scilab.modules.scinotes.actions.CloseAction;
import org.scilab.modules.scinotes.actions.CloseAllAction;
import org.scilab.modules.scinotes.actions.CloseAllButThisAction;
import org.scilab.modules.scinotes.actions.CommentAction;
import org.scilab.modules.scinotes.actions.UnCommentAction;
import org.scilab.modules.scinotes.actions.RemoveTrailingWhiteAction;
import org.scilab.modules.scinotes.actions.CopyAction;
import org.scilab.modules.scinotes.actions.CutAction;
import org.scilab.modules.scinotes.actions.DeleteAction;
import org.scilab.modules.scinotes.actions.EncodingAction;
import org.scilab.modules.scinotes.actions.EndOfLineAction;
import org.scilab.modules.scinotes.actions.EvaluateSelectionAction;
import org.scilab.modules.scinotes.actions.ExecuteFileIntoScilabAction;
import org.scilab.modules.scinotes.actions.ExitAction;
import org.scilab.modules.scinotes.actions.OpenTabInNewWindowAction;
import org.scilab.modules.scinotes.actions.CCloseTabInNewWindowAction;
import org.scilab.modules.scinotes.actions.FindAction;
import org.scilab.modules.scinotes.actions.FindNextAction;
import org.scilab.modules.scinotes.actions.FindPreviousAction;
import org.scilab.modules.scinotes.actions.CodeNavigatorAction;
import org.scilab.modules.scinotes.actions.OverwriteAction;
import org.scilab.modules.scinotes.actions.ReloadAction;
import org.scilab.modules.scinotes.actions.HelpAction;
import org.scilab.modules.scinotes.actions.HelpOnKeywordAction;
import org.scilab.modules.scinotes.actions.HighlightCurrentLineAction;
import org.scilab.modules.scinotes.actions.IndentAction;
import org.scilab.modules.scinotes.actions.LineNumbersAction;
import org.scilab.modules.scinotes.actions.LoadIntoScilabAction;
import org.scilab.modules.scinotes.actions.NewAction;
import org.scilab.modules.scinotes.actions.OpenAction;
import org.scilab.modules.scinotes.actions.OpenSourceFileOnKeywordAction;
import org.scilab.modules.scinotes.actions.PageSetupAction;
import org.scilab.modules.scinotes.actions.PasteAction;
import org.scilab.modules.scinotes.actions.PrintAction;
import org.scilab.modules.scinotes.actions.PrintPreviewAction;
import org.scilab.modules.scinotes.actions.RedoAction;
import org.scilab.modules.scinotes.actions.ResetFontAction;
import org.scilab.modules.scinotes.actions.SaveAction;
import org.scilab.modules.scinotes.actions.SaveAllAction;
import org.scilab.modules.scinotes.actions.SaveAsAction;
import org.scilab.modules.scinotes.actions.SelectAllAction;
import org.scilab.modules.scinotes.actions.SetColorsAction;
import org.scilab.modules.scinotes.actions.SetFontAction;
import org.scilab.modules.scinotes.actions.TabifyAction;
import org.scilab.modules.scinotes.actions.UnTabifyAction;
import org.scilab.modules.scinotes.actions.UndoAction;
import org.scilab.modules.scinotes.actions.SciNotesCompletionAction;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * Class SciNotesGUI handles menus, bar, ...
 */
public class SciNotesGUI {

    private static JRadioButtonMenuItem[] radioTypes;
    private static JRadioButtonMenuItem[] radioEolTypes;
    private static TextBox infoBar;
    private static Map<String, KeyStroke> map = new HashMap();

    static {
        ConfigSciNotesManager.addMapActionNameKeys(map);
    }

    /**
     * Constructor
     * @param mainWindow Windows
     * @param editorInstance SciNotes
     * @param title the title
     */
    public SciNotesGUI(Window mainWindow, final SciNotes editorInstance, String title) {
        mainWindow.setTitle(title);
        mainWindow.addTab(editorInstance);

        // Set SciNotes Window position /size
        mainWindow.setPosition(ConfigSciNotesManager.getMainWindowPosition());
        mainWindow.setDims(ConfigSciNotesManager.getMainWindowSize());

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
        searchMenu.setText(SciNotesMessages.SEARCH);
        searchMenu.setMnemonic('S');
        searchMenu.add(FindAction.createMenu(editorInstance, map.get("FindAction")));
        searchMenu.add(FindNextAction.createMenu(editorInstance, map.get("FindNextAction")));
        searchMenu.add(FindPreviousAction.createMenu(editorInstance, map.get("FindPreviousAction")));
        searchMenu.add(CodeNavigatorAction.createMenu(editorInstance, map.get("CodeNavigatorAction")));
        menuBar.add(searchMenu);

        // Create VIEW Menubar
        Menu viewMenu = ScilabMenu.createMenu();
        viewMenu.setText(SciNotesMessages.VIEW);
        viewMenu.setMnemonic('S');
        viewMenu.add(HighlightCurrentLineAction.createCheckBoxMenu(editorInstance, map.get("HighlightCurrentLineAction")));
        viewMenu.add(LineNumbersAction.createMenu(editorInstance, map.get("LineNumbersAction")));
        viewMenu.add(SetColorsAction.createMenu(editorInstance));
        viewMenu.add(SetFontAction.createMenu(editorInstance));
        viewMenu.add(ResetFontAction.createMenu(editorInstance));
        viewMenu.addSeparator();
        viewMenu.add(SplitVerticallyAction.createMenu(editorInstance, map.get("SplitVerticallyAction")));
        viewMenu.add(SplitHorizontallyAction.createMenu(editorInstance, map.get("SplitHorizontallyAction")));
        viewMenu.add(NoSplitAction.createMenu(editorInstance, map.get("NoSplitAction")));
        menuBar.add(viewMenu);

        // Create DOCUMENT MenuBar
        Menu documentMenu = ScilabMenu.createMenu();
        documentMenu.setText(SciNotesMessages.DOCUMENT);
        documentMenu.setMnemonic('D');
        Menu syntaxTypeMenu = ScilabMenu.createMenu();

        createEncodingSubMenu(documentMenu, editorInstance);
        createEolSubMenu(documentMenu, editorInstance);

        documentMenu.addSeparator();
        documentMenu.add(AutoIndentAction.createCheckBoxMenu(editorInstance));
        menuBar.add(documentMenu);

        // Create EXECUTE menubar
        Menu executeMenu = ScilabMenu.createMenu();
        executeMenu.setText(SciNotesMessages.EXECUTE);
        executeMenu.setMnemonic('e');
        executeMenu.add(LoadIntoScilabAction.createMenu(editorInstance, map.get("LoadIntoScilabAction")));
        final MenuItem evaluateSelectionMenuItem = EvaluateSelectionAction.createMenu(editorInstance, map.get("EvaluateSelectionAction"));
        if (!ScilabConsole.isExistingConsole()) { /* Only available in STD mode */
            ((JMenuItem) evaluateSelectionMenuItem.getAsSimpleMenuItem()).setEnabled(false);
        }
        PropertyChangeListener listenerEvalItem = new PropertyChangeListener() {
                public void propertyChange(PropertyChangeEvent arg0) {
                    String select = editorInstance.getTextPane().getSelectedText();
                    if (select == null) {
                        evaluateSelectionMenuItem.setText(SciNotesMessages.EVALUATE_FROM_BEGINNING);
                    } else {
                        evaluateSelectionMenuItem.setText(SciNotesMessages.EVALUATE_SELECTION);
                    }
                }
            };
        ((JMenuItem) evaluateSelectionMenuItem.getAsSimpleMenuItem()).addPropertyChangeListener(listenerEvalItem);
        executeMenu.add(evaluateSelectionMenuItem);
        executeMenu.add(ExecuteFileIntoScilabAction.createMenu(editorInstance, map.get("ExecuteFileIntoScilabAction")));
        menuBar.add(executeMenu);

        //Create HELP menubar
        Menu helpMenu = ScilabMenu.createMenu();
        helpMenu.setText(SciNotesMessages.QUESTION_MARK);
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
     * @return the Map containing the pairs (Action, KeyStroke).
     */
    public static Map<String, KeyStroke> getActionKeyMap() {
        return map;
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

        String eolUsedLabel = SciNotesMessages.EOL_LINUX;
        String eolUsed = scilabDocument.getEOL();

        if (eolUsed.compareTo(eolLinux) == 0) {
            eolUsedLabel = SciNotesMessages.EOL_LINUX;
        }

        if (eolUsed.compareTo(eolMacOs) == 0) {
            eolUsedLabel = SciNotesMessages.EOL_MACOS;
        }

        if (eolUsed.compareTo(eolWindows) == 0) {
            eolUsedLabel = SciNotesMessages.EOL_WINDOWS;
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
     * @param editorInstance SciNotes
     */
    private void createEolSubMenu(Menu documentMenu, SciNotes editorInstance) {
        String eolLinux = ScilabDocument.EOLUNIX;
        String eolMacOs = ScilabDocument.EOLMAC;
        String eolWindows = ScilabDocument.EOLWIN;
        String defaultEolLabel = SciNotesMessages.EOL_LINUX;

        // selected by default O.S
        String systemEolValue = System.getProperty("line.separator");

        if (systemEolValue.compareTo(eolLinux) == 0) {
            defaultEolLabel = SciNotesMessages.EOL_LINUX;
        }

        if (systemEolValue.compareTo(eolWindows) == 0) {
            defaultEolLabel = SciNotesMessages.EOL_WINDOWS;
        }

        if (systemEolValue.compareTo(eolMacOs) == 0) {
            defaultEolLabel = SciNotesMessages.EOL_MACOS;
        }

        Menu eolTypeMenu = ScilabMenu.createMenu();
        eolTypeMenu.setText(SciNotesMessages.EOL_TYPE);
        documentMenu.add(eolTypeMenu);

        radioEolTypes = new JRadioButtonMenuItem[3];
        ButtonGroup groupEol = new ButtonGroup();

        radioEolTypes[0] =  (new EndOfLineAction(SciNotesMessages.EOL_LINUX, editorInstance)).createRadioButtonMenuItem(editorInstance);
        groupEol.add(radioEolTypes[0]);
        ((JMenu) eolTypeMenu.getAsSimpleMenu()).add(radioEolTypes[0]);

        radioEolTypes[1] =  (new EndOfLineAction(SciNotesMessages.EOL_WINDOWS, editorInstance)).createRadioButtonMenuItem(editorInstance);
        groupEol.add(radioEolTypes[1]);
        ((JMenu) eolTypeMenu.getAsSimpleMenu()).add(radioEolTypes[1]);

        radioEolTypes[2] =  (new EndOfLineAction(SciNotesMessages.EOL_MACOS, editorInstance)).createRadioButtonMenuItem(editorInstance);
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
     * @param editorInstance SciNotes
     */
    private void createToolBar(SciNotes editorInstance) {
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
     * @param editorInstance SciNotes
     */
    private void createEditMenu(Menu editMenu, SciNotes editorInstance) {
        editMenu.setText(SciNotesMessages.EDIT);
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
     * @param editorInstance SciNotes
     */
    private void createToolsMenu(Menu toolsMenu, SciNotes editorInstance) {
        toolsMenu.setText(SciNotesMessages.TOOLS);
        toolsMenu.setMnemonic('o');
        toolsMenu.add(ActivateHelpOnTypingAction.createCheckBoxMenu(editorInstance, map.get("ActivateHelpOnTypingAction")));
        toolsMenu.addSeparator();
        toolsMenu.add(OpenTabInNewWindowAction.createMenu(editorInstance, map.get("OpenTabInNewWindowAction")));
        toolsMenu.add(CCloseTabInNewWindowAction.createMenu(editorInstance, map.get("CCloseTabInNewWindowAction")));
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
        toolsMenu.addSeparator();
        toolsMenu.add(RemoveTrailingWhiteAction.createMenu(editorInstance, map.get("RemoveTrailingWhiteAction")));
    }

    /**
     * createFileMenu
     * @param fileMenu Menu
     * @param editorInstance SciNotes
     */
    private void createFileMenu(Menu fileMenu, final SciNotes editorInstance) {
        fileMenu.setText(SciNotesMessages.FILE);
        fileMenu.setMnemonic('F');
        fileMenu.add(NewAction.createMenu(editorInstance, map.get("NewAction")));
        fileMenu.add(OpenAction.createMenu(editorInstance, map.get("OpenAction")));
        final MenuItem openSource = OpenSourceFileOnKeywordAction.createMenu(editorInstance, map.get("OpenSourceFileOnKeywordAction"));
        ((JMenuItem) openSource.getAsSimpleMenuItem()).addPropertyChangeListener(new PropertyChangeListener() {
                public void propertyChange(PropertyChangeEvent arg0) {
                    String keyword = editorInstance.getTextPane().getSelectedText();
                    if (keyword == null) {
                        KeywordEvent kwe = editorInstance.getTextPane().getKeywordEvent();
                        openSource.setEnabled(ScilabLexerConstants.isOpenable(kwe.getType()));
                    }
                }
            });

        fileMenu.add(openSource);

        Menu recentsMenu = editorInstance.getRecentsMenu();
        recentsMenu.setText(SciNotesMessages.RECENT_FILES);
        editorInstance.updateRecentOpenedFilesMenu();
        fileMenu.add(recentsMenu);

        fileMenu.addSeparator();
        fileMenu.add(SaveAction.createMenu(editorInstance, map.get("SaveAction")));
        fileMenu.add(SaveAsAction.createMenu(editorInstance, map.get("SaveAsAction")));
        fileMenu.add(SaveAllAction.createMenu(editorInstance, map.get("SaveAllAction")));
        fileMenu.addSeparator();
        final MenuItem overitem = OverwriteAction.createMenu(editorInstance, map.get("OverwriteAction"));
        ((JMenuItem) overitem.getAsSimpleMenuItem()).addPropertyChangeListener(new PropertyChangeListener() {
                public void propertyChange(PropertyChangeEvent arg0) {
                    overitem.setEnabled(editorInstance.getTextPane().checkExternalModif());
                }
            });
        fileMenu.add(overitem);

        final MenuItem reloaditem = ReloadAction.createMenu(editorInstance, map.get("ReloadAction"));
        ((JMenuItem) reloaditem.getAsSimpleMenuItem()).addPropertyChangeListener(new PropertyChangeListener() {
                public void propertyChange(PropertyChangeEvent arg0) {
                    reloaditem.setEnabled(editorInstance.getTextPane().checkExternalModif());
                }
            });
        fileMenu.add(reloaditem);

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
     * @param editorInstance SciNotes
     */
    private void createEncodingSubMenu(Menu documentMenu, SciNotes editorInstance) {

        Menu encodingTypeMenu = ScilabMenu.createMenu();
        encodingTypeMenu.setText(SciNotesMessages.ENCODING_TYPE);
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

    /**
     * Create the popup menu on the help
     * @param c The graphic component
     * @param editor the editor where the popup will occur
     */
    public static void createPopupMenu(final JEditorPane c, final SciNotes editor) {
        final JPopupMenu popup = new JPopupMenu();
        final JMenuItem evalMenuItem = new JMenuItem(SciNotesMessages.EVALUATE_SELECTION);
        JMenuItem menuItem = null;

        /* Execute into Scilab */
        ActionListener actionListenerExecuteIntoScilab = new ActionListener() {
                public void actionPerformed(ActionEvent actionEvent) {
                    String selection = ((ScilabEditorPane) c).getCodeToExecute();
                    if (selection == null) {
                        infoBar.setText(SciNotesMessages.NO_TEXT_TO_EXECUTE);
                    } else {
                        ScilabConsole.getConsole().getAsSimpleConsole().sendCommandsToScilab(selection, true, true);
                    }
                }
            };

        evalMenuItem.addActionListener(actionListenerExecuteIntoScilab);
        if (!ScilabConsole.isExistingConsole()) { /* Only available in STD mode */
            evalMenuItem.setEnabled(false);
        }
        PropertyChangeListener listenerEvalItem = new PropertyChangeListener() {
                public void propertyChange(PropertyChangeEvent arg0) {
                    String select = c.getSelectedText();
                    if (select == null) {
                        evalMenuItem.setText(SciNotesMessages.EVALUATE_FROM_BEGINNING);
                    } else {
                        evalMenuItem.setText(SciNotesMessages.EVALUATE_SELECTION);
                    }
                }
            };
        evalMenuItem.addPropertyChangeListener(listenerEvalItem);
        popup.add(evalMenuItem);

        /* Edit in the Scilab Text Editor */
        ActionListener actionListenerLoadIntoTextEditor = new ActionListener() {
                public void actionPerformed(ActionEvent actionEvent) {
                    String selection = c.getSelectedText();
                    if (selection == null) {
                        infoBar.setText(SciNotesMessages.NO_TEXT_SELECTED);
                    } else {
                        SciNotes.scinotesWithText(selection);
                    }
                }
            };

        menuItem = new JMenuItem(SciNotesMessages.EDIT_SELECTION);
        menuItem.addActionListener(actionListenerLoadIntoTextEditor);
        popup.add(menuItem);
        popup.addSeparator();

        /* Copy */
        menuItem = new JMenuItem(new DefaultEditorKit.CopyAction());
        menuItem.setText(SciNotesMessages.COPY);
        popup.add(menuItem);

        /* Cut */
        menuItem = new JMenuItem(new DefaultEditorKit.CutAction());
        menuItem.setText(SciNotesMessages.CUT);
        popup.add(menuItem);

        /* Paste */
        menuItem = new JMenuItem(new DefaultEditorKit.PasteAction());
        menuItem.setText(SciNotesMessages.PASTE);

        /* Completion */
        menuItem = new JMenuItem();
        menuItem.addActionListener(new ActionListener() {
                private SciNotesCompletionAction action;
                public void actionPerformed(ActionEvent actionEvent) {
                    if (action == null) {
                        action = new SciNotesCompletionAction(c, editor);
                    }
                    action.actionPerformed(actionEvent);
                }
            });
        menuItem.setAccelerator(map.get("SciNotesCompletionAction"));
        menuItem.setText(SciNotesMessages.COMPLETE);
        popup.add(menuItem);
        popup.addSeparator();

        /* Select all */
        ActionListener actionListenerSelectAll = new ActionListener() {
                public void actionPerformed(ActionEvent actionEvent) {
                    c.selectAll();
                }
            };
        menuItem = new JMenuItem(SciNotesMessages.SELECT_ALL);
        menuItem.addActionListener(actionListenerSelectAll);
        popup.add(menuItem);

        /* Edit in the Scilab Text Editor */
        final JMenuItem helpMenuItem = new JMenuItem(SciNotesMessages.HELP_ON_SELECTED);

        ActionListener actionListenerHelpOnKeyword = new ActionListener() {
                public void actionPerformed(ActionEvent actionEvent) {
                    String selection = c.getSelectedText();
                    if (selection == null) {
                        KeywordEvent kwe = ((ScilabEditorPane) c).getKeywordEvent();
                        if (ScilabLexerConstants.isHelpable(kwe.getType())) {
                            try {
                                selection = c.getDocument().getText(kwe.getStart(), kwe.getLength());
                            } catch (BadLocationException e) { }
                        } else {
                            infoBar.setText(SciNotesMessages.NO_TEXT_SELECTED);
                            return;
                        }
                    }
                    /* Double the quote/double quote in order to avoid
                     * and error with the call of help()
                     */
                    selection = selection.replaceAll("'", "''");
                    selection = selection.replaceAll("\"", "\"\"");

                    InterpreterManagement.requestScilabExec("help('" + selection + "')");
                }
            };

        /* Not sure it is the best listener */
        PropertyChangeListener listenerTextItem = new PropertyChangeListener() {
                public void propertyChange(PropertyChangeEvent arg0) {
                    String keyword = c.getSelectedText();
                    if (keyword == null) {
                        KeywordEvent kwe = ((ScilabEditorPane) c).getKeywordEvent();
                        if (ScilabLexerConstants.isHelpable(kwe.getType())) {
                            try {
                                keyword = c.getDocument().getText(kwe.getStart(), kwe.getLength());
                            } catch (BadLocationException e) { }
                        } else {
                            helpMenuItem.setText(SciNotesMessages.HELP_ON_FOO);
                            helpMenuItem.setEnabled(false);
                            return;
                        }
                    }
                    int nbOfDisplayedOnlyXChar = 10;
                    if (keyword.length() > nbOfDisplayedOnlyXChar) {
                        keyword = keyword.substring(0, nbOfDisplayedOnlyXChar) + SciNotesMessages.DOTS;
                    }
                    helpMenuItem.setText(SciNotesMessages.HELP_ABOUT + keyword + "'");
                    helpMenuItem.setEnabled(true);
                }
            };
        helpMenuItem.addPropertyChangeListener(listenerTextItem);
        helpMenuItem.addActionListener(actionListenerHelpOnKeyword);
        popup.add(helpMenuItem);

        /* Open source file in the Scilab Text Editor */
        final JMenuItem sourceMenuItem = new JMenuItem(SciNotesMessages.OPEN_SOURCE_FILE_ON_KEYWORD);

        ActionListener actionListenerOpenSource = new ActionListener() {
                public void actionPerformed(ActionEvent actionEvent) {
                    KeywordEvent kwe = ((ScilabEditorPane) c).getKeywordEvent(c.getSelectionEnd());
                    if (ScilabLexerConstants.isOpenable(kwe.getType())) {
                        try {
                            ScilabDocument doc = (ScilabDocument) ((ScilabEditorPane) c).getDocument();
                            String keyword = doc.getText(kwe.getStart(), kwe.getLength());
                            int pos = doc.searchFunctionByName(keyword);
                            if (pos != -1) {
                                ((ScilabEditorPane) c).scrollTextToPos(pos);
                            } else {
                                String path = "get_function_path('" + keyword + "')";
                                InterpreterManagement.requestScilabExec("if " + path + " ~=[] then editor(" + path + ");end");
                            }
                        } catch (BadLocationException e) { }
                    }
                }
            };

        /* Not sure it is the best listener */
        PropertyChangeListener listenerSourceItem = new PropertyChangeListener() {
                public void propertyChange(PropertyChangeEvent arg0) {
                    KeywordEvent kwe = ((ScilabEditorPane) c).getKeywordEvent(c.getSelectionEnd());
                    if (ScilabLexerConstants.isOpenable(kwe.getType())) {
                        try {
                            String keyword = c.getDocument().getText(kwe.getStart(), kwe.getLength());
                            int nbOfDisplayedOnlyXChar = 10;
                            if (keyword.length() > nbOfDisplayedOnlyXChar) {
                                keyword = keyword.substring(0, nbOfDisplayedOnlyXChar) + SciNotesMessages.DOTS;
                            }
                            sourceMenuItem.setText(SciNotesMessages.SOURCE_OF + keyword + "'");
                            sourceMenuItem.setEnabled(true);
                        } catch (BadLocationException e) { }
                    } else {
                        sourceMenuItem.setText(SciNotesMessages.OPEN_SOURCE_FILE_ON_KEYWORD);
                        sourceMenuItem.setEnabled(false);
                    }
                }
            };
        sourceMenuItem.addPropertyChangeListener(listenerSourceItem);
        sourceMenuItem.addActionListener(actionListenerOpenSource);
        popup.add(sourceMenuItem);

        /* Creates the Popupmenu on the component */
        c.setComponentPopupMenu(popup);
    }
}
