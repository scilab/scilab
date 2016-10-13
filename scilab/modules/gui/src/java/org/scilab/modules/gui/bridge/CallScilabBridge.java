/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2008 - DIGITEO - Sylvestre KOUMAR
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 * Copyright (C) 2010-2011 - DIGITEO - Vincent COUVERT
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


package org.scilab.modules.gui.bridge;

import java.awt.Color;
import java.awt.Font;
import java.awt.Image;
import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.StringSelection;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.image.BufferedImage;
import java.awt.print.PrinterJob;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;

import javax.imageio.ImageIO;
import javax.print.Doc;
import javax.print.DocFlavor;
import javax.print.DocPrintJob;
import javax.print.PrintException;
import javax.print.PrintService;
import javax.print.SimpleDoc;
import javax.print.attribute.HashPrintRequestAttributeSet;
import javax.print.attribute.PrintRequestAttribute;
import javax.print.attribute.PrintRequestAttributeSet;
import javax.print.attribute.standard.OrientationRequested;
import javax.swing.JEditorPane;
import javax.swing.JTextPane;
import javax.swing.SwingUtilities;
import javax.swing.text.BadLocationException;
import javax.swing.text.Document;

import org.scilab.modules.commons.ScilabCommons;
import org.scilab.modules.console.SciConsole;
import org.scilab.modules.graphic_export.FileExporter;
import org.scilab.modules.graphic_objects.ScilabNativeView;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.SwingView;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvasImpl;
import org.scilab.modules.gui.bridge.console.SwingScilabConsole;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.bridge.frame.SwingScilabFrame;
import org.scilab.modules.gui.bridge.frame.SwingScilabScrollableFrame;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.tab.SwingScilabPanel;
import org.scilab.modules.gui.colorchooser.ColorChooser;
import org.scilab.modules.gui.colorchooser.ScilabColorChooser;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.filechooser.FileChooser;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.fontchooser.FontChooser;
import org.scilab.modules.gui.fontchooser.ScilabFontChooser;
import org.scilab.modules.gui.helpbrowser.HelpBrowser;
import org.scilab.modules.gui.helpbrowser.ScilabHelpBrowser;
import org.scilab.modules.gui.messagebox.MessageBox;
import org.scilab.modules.gui.messagebox.ScilabMessageBox;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.gui.utils.ImageExporter;
import org.scilab.modules.gui.utils.PrinterHelper;
import org.scilab.modules.gui.utils.ScilabAboutBox;
import org.scilab.modules.gui.utils.ScilabPrint;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.utils.WebBrowser;
import org.scilab.modules.gui.widget.Widget;
import org.scilab.modules.localization.Messages;

/**
 * This class is used to call Scilab GUIs objects from Scilab
 * @author Vincent COUVERT
 */
public class CallScilabBridge {

    private static final int NB_COLORS = 3;

    private static PrintRequestAttributeSet scilabPageFormat = new HashPrintRequestAttributeSet();

    private static final String CONSOLE = "Console";

    /**
     * Constructor
     */
    protected CallScilabBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    }

    /******************/
    /*                */
    /* CONSOLE BRIDGE */
    /*                */
    /******************/

    /**
     * Read a line from the Console
     * @return the line read
     */
    public static String readLine() {
        return ScilabConsole.getConsole().readLine();
    }

    /**
     * Display a line in the Console
     * @param dataToDisplay the line to display
     */
    public static void display(String dataToDisplay) {
        ScilabConsole.getConsole().display(dataToDisplay);
    }

    /**
     * Get the status of the Console
     * @return true if the console is at the prompt
     */
    public static boolean isWaitingForInput() {
        return ScilabConsole.getConsole().isWaitingForInput();
    }

    /**
     * Update the number of lines and columns that Scilab use to format data to display
     */
    public static void scilabLinesUpdate() {
        ScilabConsole.getConsole().scilabLinesUpdate();
    }

    /**
     * Clear the Console
     */
    public static void clear() {
        ScilabConsole.getConsole().clear();
    }

    /**
     * Clear some lines in the Console
     * @param nbLines the number of lines to clear
     */
    public static void clear(int nbLines) {
        ScilabConsole.getConsole().clear(nbLines);
    }

    /**
     * Get a char to know if the user wants more data to be displayed
     * @return the user answer
     */
    public static int getCharWithoutOutput() {
        return ScilabConsole.getConsole().getAsSimpleConsole().getCharWithoutOutput();
    }

    /**
     * Put the prompt on the top left corner
     */
    public static void toHome() {
        ScilabConsole.getConsole().toHome();
    }

    /**
     * Set the contents of the prompt
     * @param prompt the content to set (default is -->)
     */
    public static void setPrompt(String prompt) {
        ScilabConsole.getConsole().setPrompt(prompt);
    }

    /**
     * Clear the commands history
     */
    public static void clearHistory() {
        ScilabConsole.getConsole().clearHistory();
    }

    /**************************/
    /*                        */
    /* OBJECT CREATION BRIDGE */
    /*                        */
    /**************************/

    /**
     * Create a new Color Chooser in Scilab GUIs
     * @return the ID of the Color Chooser in the UIElementMapper
     */
    public static int newColorChooser() {
        ColorChooser colorChooser = ScilabColorChooser.createColorChooser();
        return UIElementMapper.add(colorChooser);
    }

    /**
     * Create a new Font Chooser in Scilab GUIs
     * @return the ID of the Font Chooser in the UIElementMapper
     */
    public static int newFontChooser() {
        FontChooser fontChooser = ScilabFontChooser.createFontChooser();
        return UIElementMapper.add(fontChooser);
    }

    /**
     * Create a new MessageBox in Scilab GUIs
     * @return the ID of the MessageBox in the UIElementMapper
     */
    public static int newMessageBox() {
        MessageBox messageBox = ScilabMessageBox.createMessageBox();
        return UIElementMapper.add(messageBox);
    }

    /**
     * Create a new ContextMenu in Scilab GUIs
     * and wait for a user answer
     * @param uicontextmenuUID the id of the Context Menu
     * @return the item of the menu selected
     */
    public static String displayAndWaitContextMenu(int uicontextmenuUID) {
        SwingViewObject uicontextmenu = SwingView.getFromId(uicontextmenuUID);
        return ((SwingScilabContextMenu) uicontextmenu).displayAndWait();
    }

    /************************/
    /*                      */
    /* ENABLE/DISABLE MENUS */
    /*                      */
    /************************/

    /**
     * Enable/Disable a menu of a Scilab figure or console giving its name
     * @param parentUID the UID of the figure or console
     * @param menuName the name of the menu
     * @param status true to set the menu enabled
     */
    public static void setMenuEnabled(int parentUID, String menuName, boolean status) {
        SwingScilabDockablePanel parentTab = (SwingScilabDockablePanel) SwingView.getFromId(parentUID);
        if (parentTab != null) { /** Parent must exist */
            parentTab.getMenuBar().getAsSimpleMenuBar().setMenuEnabled(menuName, status);
        }
    }

    /**
     * Disable a MenuItem of a Scilab figure or console giving its parent name and position
     * @param parentUID the UID of the figure or console
     * @param parentMenuName the name of the parent menu
     * @param menuItemPosition the name of the parent menu
     * @param status true to set the menu enabled
     */
    public static void setSubMenuEnabled(int parentUID, String parentMenuName, int menuItemPosition, boolean status) {
        SwingScilabDockablePanel parentTab = (SwingScilabDockablePanel) SwingView.getFromId(parentUID);
        if (parentTab != null) { /** Parent must exist */
            parentTab.getMenuBar().getAsSimpleMenuBar().setSubMenuEnabled(parentMenuName, menuItemPosition, status);
        }
    }

    /****************/
    /*              */
    /* REMOVE MENUS */
    /*              */
    /****************/

    /**
     * Delete a menu of a Scilab figure or console giving its name
     * @param parentUID the UID of the figure or console
     * @param menuName the name of the menu
     */
    public static void removeMenu(int parentUID, String menuName) {
        SwingScilabPanel parentTab = (SwingScilabPanel) SwingView.getFromId(parentUID);
        if (parentTab != null) { /** Parent must exist */
            parentTab.getMenuBar().getAsSimpleMenuBar().removeMenu(menuName);
        }
    }

    /***********************/
    /*                     */
    /* FILE CHOOSER BRIDGE */
    /*                     */
    /***********************/



    /**
     * Create a new Graphic Export File Chooser in Scilab GUIs
     * @param figureId id of the figure to export
     * @return the ID of the File Chooser in the UIElementMapper
     */
    public static int newExportFileChooser(int figureId) {
        FileChooser fileChooser = ScilabFileChooser.createExportFileChooser(figureId);
        return 0;
    }

    /**
     * Create a new Graphic Export File Chooser in Scilab GUIs
     * @param figureId id of the figure to export
     * @return the ID of the File Chooser in the UIElementMapper
     */
    public static int exportUI(int figureId) {
        FileChooser fileChooser = ScilabFileChooser.createExportFileChooser(ScilabNativeView.ScilabNativeView__getFigureFromIndex(figureId));
        return 0;
    }

    /**********************/
    /*                    */
    /* MESSAGE BOX BRIDGE */
    /*                    */
    /**********************/

    /**
     * Set the MessageBox title
     * @param id the id of the messageBox
     * @param title the title of the messageBox
     */
    public static void setMessageBoxTitle(int id, String title) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setTitle(title);
    }

    /**
     * Set the MessageBox message
     * @param id the id of the messageBox
     * @param message the message of the messageBox
     */
    public static void setMessageBoxMessage(int id, String message) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setMessage(message);
    }

    /**
     * Set the MessageBox message (multi-line)
     * @param id the id of the messageBox
     * @param message the message of the messageBox
     */
    public static void setMessageBoxMessage(int id, String[] message) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setMessage(message);
    }

    /**
     * Display this chooser and wait for user selection
     * @param id the id of the messageBox
     */
    public static void messageBoxDisplayAndWait(int id) {
        final int finalId = id;
        try {
            SwingUtilities.invokeAndWait(new Runnable() {

                @Override
                public void run() {
                    ((MessageBox) UIElementMapper.getCorrespondingUIElement(finalId)).displayAndWait();
                }
            });
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    /**
     * Get the index of the selected button
     * @param id the id of the messageBox
     * @return the index of the selected button
     */
    public static int getMessageBoxSelectedButton(int id) {
        return ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).getSelectedButton();
    }

    /**
     * Set the indices of the default selected buttons (x_choices)
     * @param id the id of the MessageBox
     * @param indices the indices of the default selected buttons
     */
    public static void setMessageBoxDefaultSelectedButtons(int id, int[] indices) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setDefaultSelectedButtons(indices);
    }

    /**
     * Get the indices of the user selected buttons (x_choices)
     * @param id the id of the MessageBox
     * @return the indices of the user selected buttons
     */
    public static int[] getMessageBoxUserSelectedButtons(int id) {
        return ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).getUserSelectedButtons();
    }

    /**
     * Set the labels of the buttons in the MessageBox
     * @param id the id of the MessageBox
     * @param labels the labels of the buttons
     */
    public static void setMessageBoxButtonsLabels(int id, String[] labels) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setButtonsLabels(labels);
    }

    /**
     * Set the initial value for the editable part of the MessageBox
     * @param id the id of the MessageBox
     * @param value the initial value
     */
    public static void setMessageBoxInitialValue(int id, String[] value) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setInitialValue(value);
    }

    /**
     * Get the value of the editable zone in the MessageBox
     * @param id the id of the MessageBox
     * @return the value
     */
    public static String[] getMessageBoxValue(int id) {
        return ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).getValue();
    }

    /**
     * Get the size of the value of the editable zone in the MessageBox
     * @param id the id of the MessageBox
     * @return the value size
     */
    public static int getMessageBoxValueSize(int id) {
        return ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).getValueSize();
    }

    /**
     * Set the items of the listbox in the MessageBox
     * @param id the id of the MessageBox
     * @param items the items to set
     */
    public static void setMessageBoxListBoxItems(int id, String[] items) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setListBoxItems(items);
    }

    /**
     * Get the index of the selected item in the listbox in the MessageBox
     * @param id the id of the MessageBox
     * @return the index
     */
    public static int getMessageBoxSelectedItem(int id) {
        return ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).getSelectedItem();
    }

    /**
     * Set the name of the lines labels in the editable zone in the MessageBox
     * @param id the id of the MessageBox
     * @param labels the labels
     */
    public static void setMessageBoxLineLabels(int id, String[] labels) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setLineLabels(labels);
    }

    /**
     * Set the name of the columns labels in the editable zone in the MessageBox
     * @param id the id of the MessageBox
     * @param labels the labels
     */
    public static void setMessageBoxColumnLabels(int id, String[] labels) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setColumnLabels(labels);
    }

    /**
     * Set the default values of a multi-value editable zone in the MessageBox
     * @param id the id of the MessageBox
     * @param values the values
     */
    public static void setMessageBoxDefaultInput(int id, String[] values) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setDefaultInput(values);
    }

    /**
     * Set a MessageBox modal or not
     * @param id the id of the MessageBox
     * @param status true to set the MessageBox modal and false else
     */
    public static void setMessageBoxModal(int id, boolean status) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setModal(status);
    }

    /**
     * Set the MessageBox icon
     * @param id the id of the MessageBox
     * @param name the name of the icon
     */
    public static void setMessageBoxIcon(int id, String name) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setIcon(name);
    }

    /**********************/
    /*                    */
    /* HELPBROWSER BRIDGE */
    /*                    */
    /**********************/

    /**
     * Launch Scilab help browser if not already created
     * @param helps help chapters and directories
     * @param language Scilab current language
     */
    public static void launchHelpBrowser(final String[] helps, final String language) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                try {
                    ScilabHelpBrowser.createHelpBrowser(helps, language);
                    ScilabHelpBrowser.startHomePage();
                } catch (Exception e) {
                    System.err.println(e);
                }
            }
        });
    }

    /**
     * Search a keyword in Scilab Help Browser
     * @param keyword the keyword to search
     * @param helps help chapters and directories
     * @param language Scilab current language
     * @param fullText true for a full-text search
     */
    public static void searchKeyword(final String[] helps, final String keyword, final String language, final boolean fullText) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                try {
                    if (fullText) {
                        HelpBrowser helpBrowser = ScilabHelpBrowser.createHelpBrowser(helps, language);
                        if (helpBrowser != null) {
                            helpBrowser.fullTextSearch(keyword);
                        }
                    } else {
                        HelpBrowser helpBrowser = ScilabHelpBrowser.createHelpBrowser(helps, language);
                        if (helpBrowser != null) {
                            helpBrowser.searchKeywork(keyword);
                        }
                    }
                } catch (Exception e) {
                    System.err.println(e);
                }
            }
        });
    }

    /**
     * Open HelpBrowser on the page with the given xmlID
     * @param xmlID the xml id
     */
    public static void openHelp(final String xmlID) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                try {
                    HelpBrowser helpBrowser = ScilabHelpBrowser.createHelpBrowser(null, ScilabCommons.getlanguage());
                    if (helpBrowser != null) {
                        helpBrowser.searchKeywork(xmlID);
                    }
                } catch (Exception e) {
                    System.err.println(e);
                }
            }
        });
    }

    /**
     * Close Scilab Help Browser
     */
    public static void closeHelpBrowser() {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                try {
                    ScilabHelpBrowser.getHelpBrowser().close();
                } catch (Exception e) {
                    System.err.println(e);
                }
            }
        });
    }

    /**
     * Show search field in Scilab Help Browser
     */
    public static void showSearchFieldInHelp() {
        ScilabHelpBrowser.getHelpBrowser().showSearchField();
    }

    /**
     * Increase the font in the help viewer
     */
    public static void increaseFontInHelpViewer() {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                ScilabHelpBrowser.getHelpBrowser().increaseFont();
            }
        });
    }

    /**
     * Decrease the font in the help viewer
     */
    public static void decreaseFontInHelpViewer() {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                ScilabHelpBrowser.getHelpBrowser().decreaseFont();
            }
        });
    }

    /************/
    /*          */
    /* WEBLINKS */
    /*          */
    /************/

    /**
     * Open a Browser on Scilab Web Site
     */
    public static void openScilabWebSite() {
        WebBrowser.openUrl("http://www.scilab.org/");
    }

    /**
     * Open a Browser on Wiki Web Site
     */
    public static void openWiki() {
        WebBrowser.openUrl("http://wiki.scilab.org/");
    }

    /**
     * Open a Browser on ATOMS Web Site
     */
    public static void openAtomsScilab() {
        WebBrowser.openUrl("http://atoms.scilab.org/");
    }

    /**
     * Open a Browser on File Exchange Web Site
     */
    public static void openFileExchange() {
        WebBrowser.openUrl("http://fileexchange.scilab.org/");
    }

    /**
     * Open a Browser on Bugzilla Web Site
     */
    public static void openBugzilla() {
        WebBrowser.openUrl("http://bugzilla.scilab.org/");
    }

    /**
     * Open a Browser on Forge Web Site
     */
    public static void openForge() {
        WebBrowser.openUrl("http://forge.scilab.org/");
    }

    /**
     * Open a Browser on Scilab Online Help
     */
    public static void openOnlineHelp() {
        WebBrowser.openUrl("http://help.scilab.org/");
    }

    /**
     * Open a Browser on Mailing List info
     */
    public static void openMailingList() {
        WebBrowser.openUrl("http://www.scilab.org/communities/developer_zone/tools/mailing_list");
    }

    /**
     * Open a Browser on Mailing List Archives
     */
    public static void openMailingListArchives() {
        WebBrowser.openUrl("http://mailinglists.scilab.org/");
    }

    /**
     * Open a Browser on S/E
     */
    public static void openSE() {
        WebBrowser.openUrl("http://www.scilab.io");
    }

    /***************************/
    /*                         */
    /* JAVA CALLBACKS FOR GUIS */
    /*                         */
    /***************************/

    /**
     * Select all the console contents
     */
    public static void selectAllConsoleContents() {
        ScilabConsole.getConsole().selectAll();
    }

    /**
     * Select all the console contents
     */
    public static void helpOnTheKeyword() {
        ScilabConsole.getConsole().helpOnTheKeyword();
    }

    /**
     * Put the console selected text in the clipboard
     */
    public static void copyConsoleSelection() {
        ScilabConsole.getConsole().copyToClipboard();
    }

    /**
     * Cut the console selected text in the clipboard
     */
    public static void cutConsoleSelection() {
        ScilabConsole.getConsole().cutSelection();
    }

    /**
     * Paste clipboard contents in Console input line
     */
    public static void pasteClipboardIntoConsole() {
        ScilabConsole.getConsole().pasteClipboard();
    }

    /**
     * Make the clipboard contents empty
     */
    public static void emptyClipboard() {
        Transferable contents = new StringSelection("");
        Toolkit.getDefaultToolkit().getSystemClipboard().setContents(contents, null);
    }

    /**
     * Evaluate the selection with echo
     */
    public static void evaluateSelectionWithEcho() {
        ScilabConsole.getConsole().evaluateSelectionWithEcho();
    }

    /**
     * Evaluate the selection with no echo
     */
    public static void evaluateSelectionWithNoEcho() {
        ScilabConsole.getConsole().evaluateSelectionWithNoEcho();
    }

    /**
     * Opens a dialog to selected a new font for the console
     */
    public static void changeConsoleFont() {
        FontChooser fontChooser = ScilabFontChooser.createFontChooser(ScilabConsole.getConsole().getFont());
        fontChooser.displayAndWait();

        Font selectedFont = fontChooser.getSelectedFont();

        if (selectedFont != null) {
            /* Change console font */
            ScilabConsole.getConsole().setFont(selectedFont);

            /* Save new settings */
            ConfigManager.saveFont(selectedFont);
        }
    }

    /**
     * Unblock the console if it is in "Continue display..." mode
     */
    public static void unblockConsole() {
        SwingScilabConsole sciConsole = ((SwingScilabConsole) ScilabConsole.getConsole().getAsSimpleConsole());
        sciConsole.unblock();
    }

    /**
     * Opens a dialog to selected a new Foreground Color for the console
     */
    public static void changeConsoleForeground() {
        ColorChooser colorChooser = ScilabColorChooser.createColorChooser(ScilabConsole.getConsole().getForeground());
        colorChooser.setTitle(Messages.gettext("Console Font..."));
        colorChooser.displayAndWait();

        Color selectedColor = colorChooser.getSelectedColor();

        if (selectedColor != null) {
            /* Change console foreground */
            ScilabConsole.getConsole().setForeground(selectedColor);

            /* Save new settings */
            ConfigManager.saveConsoleForeground(selectedColor);
        }
    }

    /**
     * Opens a dialog to selected a new Background Color for the console
     */
    public static void changeConsoleBackground() {
        ColorChooser colorChooser = ScilabColorChooser.createColorChooser(ScilabConsole.getConsole().getBackground());
        colorChooser.setTitle(Messages.gettext("Console Background..."));
        colorChooser.displayAndWait();

        Color selectedColor = colorChooser.getSelectedColor();

        if (selectedColor != null) {
            /* Change console background */
            ScilabConsole.getConsole().setBackground(selectedColor);

            /* Save new settings */
            ConfigManager.saveConsoleBackground(selectedColor);
        }
    }

    /**
     * Display a dialog to print the console text contents
     */
    public static void printConsoleContents() {

        SciConsole scilabConsole = ((SciConsole) ScilabConsole.getConsole().getAsSimpleConsole());
        Document doc = ((JEditorPane) scilabConsole.getConfiguration().getOutputView()).getDocument();
        String textToPrint = null;

        /* Text selected in the input */
        String strInputSelected = ((JTextPane) scilabConsole.getConfiguration().getInputCommandView()).getSelectedText();
        /* Text selected in the output */
        String strOutputSelected = ((JEditorPane) scilabConsole.getConfiguration().getOutputView()).getSelectedText();

        try {
            textToPrint = doc.getText(0, doc.getLength());
        } catch (BadLocationException e) {
            e.printStackTrace();
        }
        if (strInputSelected != null) {
            printString(strInputSelected, new String(CONSOLE));
        } else if (strOutputSelected != null) {
            printString(strOutputSelected, new String(CONSOLE));
        } else {
            printString(textToPrint, new String(CONSOLE));
        }
    }

    /**
     * Print a character string
     * @param theString the string to print
     * @param pageHeader header for printed pages
     * @return execution status
     */
    public static boolean printString(String theString, String pageHeader) {
        return PrinterHelper.printString(theString, pageHeader);
    }

    /**
     * Display a dialog to print a file
     * @param fileName the name of the file
     * @return execution status
     */
    public static boolean printFile(String fileName) {
        return PrinterHelper.printFile(fileName);
    }

    /**
     * Display a dialog to print a figure (Called from graphics figures menus)
     * @param figID the ID of the figure to print
     * @return execution status
     */
    public static boolean print_figure(int figureId) {
        return printFigure(ScilabNativeView.ScilabNativeView__getFigureFromIndex(figureId), true, true);
    }

    /**
     * Display a dialog to print a figure
     * @param figID the ID of the figure to print
     * @param postScript true to print in postscript mode
     * @param displayDialog true to display a print setup dialog
     * @return execution status
     */
    public static boolean printFigure(int figID, boolean postScript, boolean displayDialog) {
        // Get the PrinterJob object
        PrinterJob printerJob = PrinterJob.getPrinterJob();

        boolean userOK = true;
        if (displayDialog) {
            userOK = printerJob.printDialog(scilabPageFormat);
        }

        if (userOK) {
            //If the OS is Windows
            if (isWindowsPlateform()) {
                Figure figure = (Figure) GraphicController.getController().getObjectFromId(figID);
                int figureID = figure.getId();
                BufferedImage bimage = null;
                if (figure.getVisible()) {
                    bimage = ((SwingScilabDockablePanel) SwingView.getFromId(figID)).getContentCanvas().dumpAsBufferedImage();
                } else {
                    try {
                        File tmpPrinterFile = File.createTempFile("scilabfigure", ".png");
                        tmpPrinterFile.delete();
                        FileExporter.fileExport(figID, tmpPrinterFile.getAbsolutePath(), "PNG", 1, 0);
                        bimage = ImageIO.read(tmpPrinterFile);
                    } catch (IOException e) {
                        e.printStackTrace();
                        return false;
                    }
                }

                if (bimage != null) {
                    ScilabPrint scilabPrint = new ScilabPrint(bimage, printerJob, scilabPageFormat);
                    if (scilabPrint != null) {
                        return true;
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }

                //If the OS is Linux
            } else {
                DocFlavor printDocFlavor = DocFlavor.INPUT_STREAM.POSTSCRIPT;
                String fileExtension = ".ps";

                try {
                    String tmpPrinterFile = File.createTempFile("scilabfigure", "").getAbsolutePath();
                    // Export image to PostScript
                    if (((PrintRequestAttribute) scilabPageFormat.get(OrientationRequested.class)) == OrientationRequested.PORTRAIT) {
                        FileExporter.fileExport(figID,
                                                tmpPrinterFile + fileExtension,
                                                "PS", 1, 0); // 1 is the quality. Useless in this context
                    } else {
                        FileExporter.fileExport(figID,
                                                tmpPrinterFile + fileExtension,
                                                "PS", 1, 1); // 1 is the quality. Useless in this context
                    }

                    // Read file
                    FileInputStream psStream = null;

                    try {
                        psStream = new FileInputStream(tmpPrinterFile + fileExtension);
                    } catch (FileNotFoundException ffne) {
                        ffne.printStackTrace();
                        return false;
                    }

                    Doc myDoc = new SimpleDoc(psStream, printDocFlavor, null);
                    PrintService printService = printerJob.getPrintService();

                    if (printService == null) {
                        // Could not find the print service
                        MessageBox messageBox = ScilabMessageBox.createMessageBox();
                        messageBox.setMessage(Messages.gettext("No print service found."));
                        messageBox.setModal(true);
                        messageBox.setIcon("error");
                        messageBox.displayAndWait();
                        return false;
                    }
                    DocPrintJob job = printService.createPrintJob();

                    // Remove Orientation option from page setup because already managed in FileExporter
                    PrintRequestAttributeSet aset = new HashPrintRequestAttributeSet(scilabPageFormat);
                    aset.add(OrientationRequested.PORTRAIT);
                    job.print(myDoc, aset);
                    return true;
                } catch (IOException e) {
                    System.err.println("Could not create temporary file " + e.getLocalizedMessage());
                    return false;
                } catch (PrintException e) {
                    e.printStackTrace();
                    return false;
                }
            }
        }
        return false;
    }

    /**
     * Display a page setup dialog for printing
     * @return true if the user clicked the OK button
     */
    public static boolean pageSetup() {
        // Get the PrinterJob object
        PrinterJob job = PrinterJob.getPrinterJob();

        // Get the default page format, then allow the user to modify it
        return (job.pageDialog(scilabPageFormat) != null);
    }

    /***********************/
    /*                     */
    /* FONT CHOOSER BRIDGE */
    /*                     */
    /***********************/

    /**
     * Set the default font name for a FileChooser
     * @param id the id of the FileChooser
     * @param fontName the name of the font
     */
    public static void setFontChooserFontName(int id, String fontName) {
        ((FontChooser) UIElementMapper.getCorrespondingUIElement(id)).setDefaultFontName(fontName);
    }

    /**
     * Set the default font size for a FileChooser
     * @param id the id of the FileChooser
     * @param fontSize the size of the font
     */
    public static void setFontChooserFontSize(int id, int fontSize) {
        ((FontChooser) UIElementMapper.getCorrespondingUIElement(id)).setDefaultFontSize(fontSize);
    }

    /**
     * Set the default bold attribute for a FileChooser
     * @param id the id of the FileChooser
     * @param bold the bold attribute of the font
     */
    public static void setFontChooserBold(int id, boolean bold) {
        ((FontChooser) UIElementMapper.getCorrespondingUIElement(id)).setDefaultBold(bold);
    }

    /**
     * Set the default italic attribute for a FileChooser
     * @param id the id of the FileChooser
     * @param italic the italic attribute of the font
     */
    public static void setFontChooserItalic(int id, boolean italic) {
        ((FontChooser) UIElementMapper.getCorrespondingUIElement(id)).setDefaultItalic(italic);
    }

    /**
     * Get the selected font name for a FileChooser
     * @param id the id of the FileChooser
     * @return the name of the font
     */
    public static String getFontChooserFontName(int id) {
        Font selectedFont = ((FontChooser) UIElementMapper.getCorrespondingUIElement(id)).getSelectedFont();
        if (selectedFont != null) {
            return selectedFont.getFamily();
        } else {
            return "";
        }
    }

    /**
     * Get the selected font size for a FileChooser
     * @param id the id of the FileChooser
     * @return the size of the font
     */
    public static int getFontChooserFontSize(int id) {
        return ((FontChooser) UIElementMapper.getCorrespondingUIElement(id)).getSelectedFont().getSize();
    }

    /**
     * Get the selected bold attribute for a FileChooser
     * @param id the id of the FileChooser
     * @return the bold attribute of the font
     */
    public static boolean getFontChooserBold(int id) {
        return ((FontChooser) UIElementMapper.getCorrespondingUIElement(id)).getSelectedFont().isBold();
    }

    /**
     * Get the selected italic attribute for a FileChooser
     * @param id the id of the FileChooser
     * @return the italic attribute of the font
     */
    public static boolean getFontChooserItalic(int id) {
        return ((FontChooser) UIElementMapper.getCorrespondingUIElement(id)).getSelectedFont().isItalic();
    }

    /**
     * Display this chooser and wait for user selection
     * @param id the id of the FontChooser
     */
    public static void fontChooserDisplayAndWait(int id) {
        ((FontChooser) UIElementMapper.getCorrespondingUIElement(id)).displayAndWait();
    }

    /************************/
    /*                      */
    /* COLOR CHOOSER BRIDGE */
    /*                      */
    /************************/

    /**
     * Set the title of a ColorChooser
     * @param id the id of the ColorChooser
     * @param title the title
     */
    public static void setColorChooserTitle(int id, String title) {
        ((ColorChooser) UIElementMapper.getCorrespondingUIElement(id)).setTitle(title);
    }

    /**
     * Set the default Color for a ColorChooser
     * @param id the id of the ColorChooser
     * @param rgb the default color
     */
    public static void setColorChooserDefaultColor(int id, int[] rgb) {
        ((ColorChooser) UIElementMapper.getCorrespondingUIElement(id)).setDefaultColor(new Color(rgb[0], rgb[1], rgb[2]));
    }

    /**
     * Get the selected Color for a ColorChooser
     * @param id the id of the ColorChooser
     * @return the selected color
     */
    public static int[] getColorChooserSelectedColor(int id) {
        Color selectedColor = ((ColorChooser) UIElementMapper.getCorrespondingUIElement(id)).getSelectedColor();
        int[] returnedValues = new int[NB_COLORS];
        if (selectedColor != null) {
            returnedValues[0] = selectedColor.getRed();
            returnedValues[1] = selectedColor.getGreen();
            returnedValues[2] = selectedColor.getBlue();
        } else {
            returnedValues[0] = -1;
            returnedValues[1] = -1;
            returnedValues[2] = -1;
        }
        return returnedValues;

    }

    /**
     * Display this chooser and wait for user selection
     * @param id the id of the ColorChooser
     */
    public static void colorChooserDisplayAndWait(int id) {
        ((ColorChooser) UIElementMapper.getCorrespondingUIElement(id)).displayAndWait();
    }

    /************************/
    /*                      */
    /* CLIPBOARD MANAGEMENT */
    /*                      */
    /************************/

    /**
     * Get the contents of the clipboard
     * @return the string contained in the clipboard or null
     */
    public static String getClipboardContents() {
        // Gets the contents of the clipboard
        Toolkit toolkit = Toolkit.getDefaultToolkit();
        Clipboard systemClipboard = toolkit.getSystemClipboard();
        String clipboardContents = "";

        // Verify that clibpboard data is of text type
        boolean dataAvailable;
        try {
            dataAvailable = systemClipboard.isDataFlavorAvailable(DataFlavor.stringFlavor);
        } catch (IllegalStateException exception) {
            return clipboardContents;
        }

        // Exit if text data not available
        if (!dataAvailable) {
            return clipboardContents;
        }

        // Read data
        try {
            clipboardContents = (String) systemClipboard.getData(DataFlavor.stringFlavor);
        } catch (UnsupportedFlavorException e1) {
            // Should never be here
            e1.printStackTrace();
        } catch (IOException e1) {
            // Should never be here
            e1.printStackTrace();
        }

        return clipboardContents;
    }

    /**
     * Set the contents of the clipboard
     * @param text the string to put in the clipboard
     */
    public static void setClipboardContents(String text) {
        Transferable contents = new StringSelection(text);
        Toolkit.getDefaultToolkit().getSystemClipboard().setContents(contents, null);
    }

    /**
     * Copy figure to clipboard
     * @param figID the ID of the figure
     */
    public static void clipboard_figure(int figureId) {
        copyFigureToClipBoard(ScilabNativeView.ScilabNativeView__getFigureFromIndex(figureId));
    }

    /**
     * Copy figure to clipboard
     * @param figID the ID of the figure
     */
    public static void copyFigureToClipBoard(int figID) {
        Image figureImage = ImageExporter.imageExport(figID);
        if (figureImage != null) {
            Transferable clipboardImage = new ClipboardImage(figureImage);
            Toolkit.getDefaultToolkit().getSystemClipboard().setContents(clipboardImage, null);
        }
    }

    /**
     * Class used to store Images in the clipboard
     */
    public static class ClipboardImage implements Transferable {
        private final DataFlavor[] flavors = new DataFlavor[] {DataFlavor.imageFlavor};
        private final Image image;

        /**
         * Default constructor
         * @param image the image
         */
        public ClipboardImage(Image image) {
            this.image = image;
        }

        /**
         * DataFlavors of this transferable
         * @return the DataFlavors accepeted
         */
        @Override
        public DataFlavor[] getTransferDataFlavors() {
            return flavors;
        }

        /**
         * Test supported DataFlavors
         * @param flavor the flavor to test
         * @return true if the flavor is supported
         */
        @Override
        public boolean isDataFlavorSupported(DataFlavor flavor) {
            return flavors[0].equals(flavor);
        }

        /**
         * Get the contents of this transferable
         * @param flavor the flavor to test
         * @return the contents
         * @throws UnsupportedFlavorException if the flavor is not supported by this transferable
         */
        @Override
        public Object getTransferData(DataFlavor flavor) throws UnsupportedFlavorException {
            if (!flavors[0].equals(flavor)) {
                throw new UnsupportedFlavorException(flavor);
            }

            return image;
        }
    }

    /********/
    /*      */
    /* MISC */
    /*      */
    /********/

    /**
     * Get the screen resolution
     * @return the number of dot per inch
     */
    public static int getScreenResolution() {
        return Toolkit.getDefaultToolkit().getScreenResolution();
    }

    /**
     * Get the screen Width in pixels
     * @return the screen Width
     */
    public static double getScreenWidth() {
        return Toolkit.getDefaultToolkit().getScreenSize().getWidth();
    }

    /**
     * Get the screen Height in pixels
     * @return the screen Height
     */
    public static double getScreenHeight() {
        return Toolkit.getDefaultToolkit().getScreenSize().getHeight();
    }

    /**
     * Get the screen Depth
     * @return the screen Depth
     */
    public static int getScreenDepth() {
        return Toolkit.getDefaultToolkit().getColorModel().getPixelSize();
    }

    /**
     * Give the focus to a uicontrol
     * @param uicontrolUID the uicontrolUID of the Widget
     */
    public static void requestFocus(int uicontrolUID) {
        SwingViewObject uicontrol = SwingView.getFromId(uicontrolUID);
        if (uicontrol instanceof SwingScilabScrollableFrame) {
            ((SwingScilabScrollableFrame) uicontrol).requestFocus();
        } else if (uicontrol instanceof SwingScilabFrame) {
            ((SwingScilabFrame) uicontrol).requestFocus();
        } else {
            ((Widget) uicontrol).requestFocus();
        }

    }

    /**
     * Raise the window specified by the user
     * @param id the id of the figure
     */
    public static void raiseWindow(int id) {
        /*((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(id).getRendererProperties()).getParentTab().getParentWindow().raise();
          ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(id).getRendererProperties()).getParentTab().setCurrent();*/
    }

    /**
     * @return true if the os is windows, false otherwise
     */
    public static boolean isWindowsPlateform() {
        // get os name
        return System.getProperty("os.name").toLowerCase().contains("windows");
    }

    /**
     * Change object used for graphic figure (GLCanvas or GLJPanel)
     * @param onOrOff true to set GLCanvas display
     */
    public static void useCanvasForDisplay(boolean onOrOff) {
        SwingScilabCanvasImpl.switchToGLCanvas(onOrOff);
    }

    /**
     * Get object used for graphic figure (GLCanvas or GLJPanel)
     * @return true if GLCanvas display is set
     */
    public static boolean useCanvasForDisplay() {
        return SwingScilabCanvasImpl.isGLCanvasEnabled();
    }

    /**
     * Display Scilab about box
     */
    public static void scilabAboutBox() {
        ScilabAboutBox.displayAndWait();
    }

    /******************/
    /*                */
    /* UITABLE BRIDGE */
    /*                */
    /******************/

    public static void fireClosingFinished(int figUID) {
        SwingScilabDockablePanel parentTab = (SwingScilabDockablePanel) SwingView.getFromId(figUID);
        ClosingOperationsManager.removeFromDunnoList(parentTab);
    }
}
