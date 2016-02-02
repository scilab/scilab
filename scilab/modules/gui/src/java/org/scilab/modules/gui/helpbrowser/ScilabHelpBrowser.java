/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * Copyright (C) 2011 - Calixte DENIZET
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

package org.scilab.modules.gui.helpbrowser;

import java.awt.Component;
import java.awt.Container;
import java.awt.KeyEventDispatcher;
import java.awt.KeyboardFocusManager;
import java.awt.Toolkit;
import java.awt.event.KeyEvent;

import javax.swing.SwingUtilities;

import org.scilab.modules.commons.ScilabCommons;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.gui.ScilabGUIUtilities;
import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.bridge.helpbrowser.SwingScilabHelpBrowser;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.dockable.ScilabDockable;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.tabfactory.HelpBrowserTabFactory;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.gui.utils.MenuBarBuilder;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.localization.Messages;

/**
 * Class for Scilab Help Browser in GUIs
 * @author Vincent COUVERT
 * @author Calixte DENIZET
 */
public class ScilabHelpBrowser extends ScilabDockable implements HelpBrowser {

    public static final String HELPUUID = "d1ef5062-533e-4aee-b12a-66740820d6d1";

    private static final String SCI = ScilabConstants.SCI.getPath();
    private static final String MENUBARXMLFILE = SCI + "/modules/gui/etc/helpbrowser_menubar.xml";

    private static HelpBrowser instance;
    private static Tab helpTab;

    private static String language;
    private static String[] helps;

    static {
        ScilabTabFactory.getInstance().addTabFactory(HelpBrowserTabFactory.getInstance());
    }

    private SimpleHelpBrowser component;
    private SwingScilabWindow parentWindow;

    /**
     * Constructor
     * @param helps help chapters and directories
     * @param language Scilab current language
     */
    protected ScilabHelpBrowser(String[] helps, String language) {
        if (language == null) {
            language = ScilabCommons.getlanguage();
        }
        component = ScilabBridge.createHelpBrowser(helps, language);
    }

    /**
     * Creates a SwingScilabTab containing the help browser
     * @return the corresponding SwingScilabTab
     */
    public static SwingScilabDockablePanel createHelpBrowserTab() {
        helpTab = ScilabTab.createTab(Messages.gettext("Help Browser"), HELPUUID);
        String lastID = restoreHelpBrowserState();

        if (!SwingScilabHelpBrowser.isMainJarExists(language)) {
            System.err.println("No help file available.");
            return null;
        }

        instance = new ScilabHelpBrowser(helps, language);
        helpTab.addMember(instance);

        MenuBar menubar = MenuBarBuilder.buildMenuBar(MENUBARXMLFILE);
        helpTab.addMenuBar(menubar);

        TextBox infobar = ScilabTextBox.createTextBox();
        helpTab.addInfoBar(infobar);

        KeyboardFocusManager manager = KeyboardFocusManager.getCurrentKeyboardFocusManager();
        manager.addKeyEventDispatcher(new KeyEventDispatcher() {
            // This is a workaround for Mac OS X where e.getKeyCode() sometimes returns a bad value
            public boolean dispatchKeyEvent(KeyEvent e) {
                if (e.getID() == KeyEvent.KEY_PRESSED) {
                    Container c = SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, (SwingScilabDockablePanel) helpTab.getAsSimpleTab());
                    if (e.getSource() instanceof Component) {
                        Container cs = SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, (Component) e.getSource());
                        char chr = e.getKeyChar();

                        if (cs == c && ((chr == '-' || chr == '_' || chr == '=' || chr == '+')
                                        && (e.getModifiers() & Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()) != 0)) {
                            if (chr == '-' || chr == '_') {
                                ((SwingScilabHelpBrowser) ((ScilabHelpBrowser) instance).component).decreaseFont();
                            } else {
                                ((SwingScilabHelpBrowser) ((ScilabHelpBrowser) instance).component).increaseFont();
                            }
                            return true;
                        }
                    }
                }
                return false;
            }
        });

        SwingScilabHelpBrowser browser = (SwingScilabHelpBrowser) ((ScilabHelpBrowser) instance).component;
        browser.setCurrentID(lastID);
        ((SwingScilabDockablePanel) helpTab.getAsSimpleTab()).setAssociatedXMLIDForHelp("helpbrowser");
        WindowsConfigurationManager.restorationFinished((SwingScilabDockablePanel) helpTab.getAsSimpleTab());

        return (SwingScilabDockablePanel) helpTab.getAsSimpleTab();
    }

    /**
     * Creates a Scilab Help Browser
     * @param helps help chapters and directories
     * @param language Scilab current language
     * @return the created Help Browser
     */
    public static HelpBrowser createHelpBrowser(String[] helps, String language) {
        if (!SwingScilabHelpBrowser.isMainJarExists(language)) {
            System.err.println("No help file available.");
            return null;
        }

        if (instance == null) {
            ScilabHelpBrowser.language = language;
            ScilabHelpBrowser.helps = helps;
            boolean success = WindowsConfigurationManager.restoreUUID(HELPUUID);
            if (!success) {
                HelpBrowserTabFactory.getInstance().getTab(HELPUUID);
                ((ScilabHelpBrowser) instance).setParentWindow();
                ((ScilabHelpBrowser) instance).parentWindow.addTab((SwingScilabDockablePanel) helpTab.getAsSimpleTab());
                ((ScilabHelpBrowser) instance).parentWindow.setVisible(true);
            }

            if (ScilabConsole.isExistingConsole() && ScilabConsole.getConsole().getInfoBar() != null) {
                if (ScilabConsole.getConsole().getInfoBar().getText().equals(Messages.gettext("Loading help browser..."))) {
                    // An error occurred
                    ScilabConsole.getConsole().getInfoBar().setText("");
                    return null;
                }
            }
        }

        SwingScilabWindow window = (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, (SwingScilabDockablePanel) helpTab.getAsSimpleTab());
        ScilabGUIUtilities.toFront(window);

        return instance;
    }

    /**
     * Set a default parent window
     */
    public void setParentWindow() {
        this.parentWindow = SwingScilabWindow.createWindow(true);
        parentWindow.setLocation(0, 0);
        parentWindow.setSize(500, 500);
    }

    /**
     * Display the home page
     */
    public static void startHomePage() {
        if (instance != null) {
            SwingScilabHelpBrowser browser = (SwingScilabHelpBrowser) ((ScilabHelpBrowser) instance).component;
            try {
                browser.displayHomePage();
            } catch (Exception e) {
                System.err.println("This error should not occurred, please report it at bugzilla.scilab.org\n" + e);
            }
        }
    }

    /**
     * @return the current displayed url as String
     */
    public String getCurrentURL() {
        SwingScilabHelpBrowser browser = (SwingScilabHelpBrowser) ((ScilabHelpBrowser) instance).component;
        return browser.getCurrentURL();
    }

    /**
     * @return the current displayed url as String
     */
    public String getCurrentID() {
        SwingScilabHelpBrowser browser = (SwingScilabHelpBrowser) ((ScilabHelpBrowser) instance).component;
        return browser.getCurrentID();
    }

    /**
     * Increase the font of the help viewer
     */
    public void increaseFont() {
        SwingScilabHelpBrowser browser = (SwingScilabHelpBrowser) ((ScilabHelpBrowser) instance).component;
        browser.increaseFont();
    }

    /**
     * Decrease the font of the help viewer
     */
    public void decreaseFont() {
        SwingScilabHelpBrowser browser = (SwingScilabHelpBrowser) ((ScilabHelpBrowser) instance).component;
        browser.decreaseFont();
    }

    /**
     * Display the home page
     */
    public void showSearchField() {
        SwingScilabHelpBrowser browser = (SwingScilabHelpBrowser) ((ScilabHelpBrowser) instance).component;
        browser.showSearchField();
    }

    /**
     * Display the home page
     */
    public void hideSearchField() {
        SwingScilabHelpBrowser browser = (SwingScilabHelpBrowser) ((ScilabHelpBrowser) instance).component;
        browser.hideSearchField();
    }

    /**
     * Get the Scilab Help Browser
     * @return the console
     */
    public static HelpBrowser getHelpBrowser() {
        if (instance == null) {
            instance = new ScilabHelpBrowser(null, "en_US");
        }
        return instance;
    }

    /**
     * Get the Scilab Help Browser (does not create it is not existing)
     * @return the console
     */
    public static HelpBrowser getHelpBrowserWithoutCreation() {
        return instance;
    }

    /**
     * Display the Help Browser
     */
    public void display() {
        ScilabBridge.display(this);
    }

    /**
     * Display the help page for a specified keyword
     * @param keyword the keyword
     */
    public void searchKeywork(String keyword) {
        ScilabBridge.searchKeywork(this, keyword);
    }

    /**
     * Display the result of a full-text search for a specified word
     * @param keyword the keyword
     */
    public void fullTextSearch(String keyword) {
        ScilabBridge.fullTextSearch(this, keyword);
    }

    /**
     * Close the HelpBrowser
     */
    public void closeHelpBrowser() {
        saveHelpBrowserState();
        instance = null;
    }

    /**
     * Close the HelpBrowser
     */
    public static void closeHB() {
        ((ScilabHelpBrowser) instance).closeHelpBrowser();
    }

    /**
     * Close the HelpBrowser
     */
    public void close() {
        ClosingOperationsManager.startClosingOperation((SwingScilabDockablePanel) helpTab.getAsSimpleTab());
    }

    /**
     * Not used
     */
    public void draw() {
        throw new UnsupportedOperationException();
    }

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    public SimpleHelpBrowser getAsSimpleHelpBrowser() {
        return component;
    }

    /**
     * Not used
     * @return nothing
     */
    public Size getDims() {
        throw new UnsupportedOperationException();
    }

    /**
     * Not used
     * @return nothing
     */
    public Position getPosition() {
        throw new UnsupportedOperationException();
    }

    /**
     * Not used
     * @return nothing
     */
    public boolean isVisible() {
        throw new UnsupportedOperationException();
    }

    /**
     * Not used
     * @param newSize not used
     */
    public void setDims(Size newSize) {
        throw new UnsupportedOperationException();
    }

    /**
     * Not used
     * @param newPosition not used
     */
    public void setPosition(Position newPosition) {
        throw new UnsupportedOperationException();
    }

    /**
     * Not used
     * @param newVisibleState not used
     */
    public void setVisible(boolean newVisibleState) {
        throw new UnsupportedOperationException();
    }

    /**
     * Get associated InfoBar
     * @return the infobar of the parent tab
     * @see org.scilab.modules.gui.uielement.ScilabUIElement#getInfoBar()
     */
    public TextBox getInfoBar() {
        return helpTab.getInfoBar();
    }


    /**
     * Save the state of this help browser
     */
    public void saveHelpBrowserState() {
        String id = getCurrentID();
        if (id != null) {
            ConfigManager.saveHelpBrowserState(id);
        }
    }

    /**
     * Restore the state of this help browser
     * @return the last id displayed in the browser
     */
    public static String restoreHelpBrowserState() {
        return ConfigManager.getHelpBrowserState();
    }
}
