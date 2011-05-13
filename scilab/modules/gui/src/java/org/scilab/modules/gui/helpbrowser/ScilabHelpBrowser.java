/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.bridge.helpbrowser.SwingScilabHelpBrowser;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.dockable.ScilabDockable;
import org.scilab.modules.gui.events.callback.ScilabCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.gui.utils.MenuBarBuilder;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.localization.Messages;

/**
 * Class for Scilab Help Browser in GUIs
 * @author Vincent COUVERT
 */
public class ScilabHelpBrowser extends ScilabDockable implements HelpBrowser {

    private static final String SCI = ScilabConstants.SCI.getPath();
    private static final String MENUBARXMLFILE = SCI + "/modules/gui/etc/helpbrowser_menubar.xml";
    private static final boolean isMac = System.getProperty("os.name").toLowerCase().indexOf("mac") != -1;

    private static HelpBrowser instance;

    private static Tab helpTab;

    private SimpleHelpBrowser component;

    /**
     * Constructor
     * @param helps help chapters and directories
     * @param language Scilab current language
     */
    protected ScilabHelpBrowser(String[] helps, String language) {
        component = ScilabBridge.createHelpBrowser(helps, language);
    }

    /**
     * Creates a Scilab Help Browser
     * @param helps help chapters and directories
     * @param language Scilab current language
     * @return the created Help Browser
     */
    public static HelpBrowser createHelpBrowser(String[] helps, String language) {
        if (instance == null) {

            instance = new ScilabHelpBrowser(helps, language);

            if (ScilabConsole.isExistingConsole() && ScilabConsole.getConsole().getInfoBar() != null) {
                if (ScilabConsole.getConsole().getInfoBar().getText().equals(Messages.gettext("Loading help browser..."))) {
                    // An error occured
                    ScilabConsole.getConsole().getInfoBar().setText("");
                    return null;
                }
            }

            helpTab = ScilabTab.createTab(Messages.gettext("Help Browser"));
            helpTab.addMember(instance);
            /* Action when the Browser tab is closed */
            helpTab.setCallback(ScilabCallBack
                                .createCallback("org.scilab.modules.gui.bridge.CallScilabBridge.closeHelpBrowser", ScilabCallBack.JAVA_OUT_OF_XCLICK_AND_XGETMOUSE));

            MenuBar menubar = MenuBarBuilder.buildMenuBar(MENUBARXMLFILE);
            helpTab.addMenuBar(menubar);

            TextBox infobar = ScilabTextBox.createTextBox();
            helpTab.addInfoBar(infobar);

            Window helpWindow = ScilabWindow.createWindow();
            helpWindow.addTab(helpTab);

            KeyboardFocusManager manager = KeyboardFocusManager.getCurrentKeyboardFocusManager();
            manager.addKeyEventDispatcher(new KeyEventDispatcher() {
                    // This is a workaround for Mac OS X where e.getKeyCode() sometimes returns a bad value
                    public boolean dispatchKeyEvent(KeyEvent e) {
                        if (e.getID() == KeyEvent.KEY_PRESSED) {
                            Container c = SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, (SwingScilabTab) helpTab.getAsSimpleTab());
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

            /* Set the dimension / position of the help window */
            helpWindow.setPosition(ConfigManager.getHelpWindowPosition());
            helpWindow.setDims(ConfigManager.getHelpWindowSize());

            helpWindow.draw();
        } else {
            SwingScilabWindow window = (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, (SwingScilabTab) helpTab.getAsSimpleTab());
            window.setVisible(true);
            window.toFront();
        }

        return instance;
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
                System.err.println("This error should not occured, please report it at bugzilla.scilab.org\n" + e);
            }
        }
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
    public void close() {
        ScilabBridge.close(this);
        helpTab.close();
        instance = null;
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
}
