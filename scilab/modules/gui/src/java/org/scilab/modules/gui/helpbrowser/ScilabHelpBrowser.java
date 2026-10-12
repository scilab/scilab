/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * Copyright (C) 2011 - Calixte DENIZET
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
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
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import javax.swing.JButton;
import javax.swing.SwingUtilities;

import org.scilab.modules.commons.ScilabCommons;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabGUIUtilities;
import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.bridge.helpbrowser.SwingScilabHelpBrowser;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.toolbar.SwingScilabToolBar;
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
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
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
 * @author Antoine ELIAS
 */
public class ScilabHelpBrowser extends ScilabDockable implements HelpBrowser {

    public static final String HELPUUID = "d1ef5062-533e-4aee-b12a-66740820d6d1";

    private static final String SCI = ScilabConstants.SCI.getPath();
    private static final String MENUBARXMLFILE = SCI + "/modules/gui/etc/helpbrowser_menubar.xml";

    private static HelpBrowser instance;
    private static Tab helpTab;

    private static String language;
    private static String[] helps;

    /** Chapter titles given to add_help_chapter(), keyed by normalized path. */
    private static final Map<String, String> chapterTitles = new HashMap<String, String>();

    static {
        ScilabTabFactory.getInstance().addTabFactory(HelpBrowserTabFactory.getInstance());
    }

    /**
     * Register the titles of the help chapters declared through
     * add_help_chapter() (the %helps matrix). Called from Scilab
     * (helpbrowser_update.sci), possibly before the browser exists: titles are
     * kept in a static registry and looked up when the chapters are resolved.
     *
     * The two arguments are newline-separated blobs (paths and titles, same
     * order): a flat string avoids ambiguous Scilab->Java array marshalling.
     *
     * @param pathsBlob  newline-separated chapter directories (%helps(:,1))
     * @param titlesBlob newline-separated chapter titles (%helps(:,2))
     */
    public static void setChapterTitles(String pathsBlob, String titlesBlob) {
        String[] paths = (pathsBlob == null ? "" : pathsBlob).split("\n", -1);
        String[] titles = (titlesBlob == null ? "" : titlesBlob).split("\n", -1);
        synchronized (chapterTitles) {
            chapterTitles.clear();
            for (int i = 0; i < paths.length; i++) {
                String p = paths[i].trim();
                String t = (i < titles.length) ? titles[i].trim() : "";
                if (!p.isEmpty() && !t.isEmpty()) {
                    chapterTitles.put(normalizeChapterPath(p), t);
                }
            }
        }
    }

    /**
     * @param path a chapter directory as registered by add_help_chapter()
     * @return the title given to add_help_chapter(), or null if unknown
     */
    public static String getChapterTitle(String path) {
        if (path == null) {
            return null;
        }
        synchronized (chapterTitles) {
            return chapterTitles.get(normalizeChapterPath(path));
        }
    }

    private static String normalizeChapterPath(String p) {
        String n = new File(p).getAbsolutePath().replace('\\', '/');
        while (n.length() > 1 && n.endsWith("/")) {
            n = n.substring(0, n.length() - 1);
        }
        return n.toLowerCase();
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
        component = new SwingScilabHelpBrowser(helps, language);
    }

    /**
     * Creates a SwingScilabTab containing the help browser
     * @return the corresponding SwingScilabTab
     */
    public static SwingScilabDockablePanel createHelpBrowserTab() {
        helpTab = ScilabTab.createTab(Messages.gettext("Help Browser"), HELPUUID);
        String lastID = restoreHelpBrowserState();

        instance = new ScilabHelpBrowser(helps, language);
        helpTab.addMember(instance);

        MenuBar menubar = MenuBarBuilder.buildMenuBar(MENUBARXMLFILE);
        helpTab.addMenuBar(menubar);

        TextBox infobar = ScilabTextBox.createTextBox();
        helpTab.addInfoBar(infobar);

        // Keyboard shortcut for zoom (Ctrl+/Ctrl-)
        KeyboardFocusManager manager = KeyboardFocusManager.getCurrentKeyboardFocusManager();
        manager.addKeyEventDispatcher(new KeyEventDispatcher() {
            public boolean dispatchKeyEvent(KeyEvent e) {
                if (e.getID() == KeyEvent.KEY_PRESSED) {
                    Container c = SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, (SwingScilabDockablePanel) helpTab.getAsSimpleTab());
                    if (e.getSource() instanceof Component) {
                        Container cs = SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, (Component) e.getSource());
                        char chr = e.getKeyChar();

                        if (cs == c && ((chr == '-' || chr == '_' || chr == '=' || chr == '+' || chr == '0')
                                        && (e.getModifiersEx() & Toolkit.getDefaultToolkit().getMenuShortcutKeyMaskEx()) != 0)) {
                            SwingScilabHelpBrowser browser = (SwingScilabHelpBrowser) ((ScilabHelpBrowser) instance).component;
                            if (chr == '-' || chr == '_') {
                                browser.decreaseFont();
                            } else if (chr == '0') {
                                browser.resetFont();
                            } else {
                                browser.increaseFont();
                            }
                            return true;
                        }
                    }
                }
                return false;
            }
        });

        // Navigate to last viewed page if available
        if (lastID != null && !lastID.isEmpty()) {
            ((SwingScilabHelpBrowser) ((ScilabHelpBrowser) instance).component).searchKeywork(lastID);
        }

        // Toolbar: document zoom buttons
        ToolBar toolbar = ScilabToolBar.createToolBar();
        SwingScilabToolBar swingToolbar = (SwingScilabToolBar) toolbar.getAsSimpleToolBar();
        final SwingScilabHelpBrowser jcefBrowser = (SwingScilabHelpBrowser) ((ScilabHelpBrowser) instance).component;
        swingToolbar.add(createToolButton("zoom-out", Messages.gettext("Zoom Out"), new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                jcefBrowser.decreaseFont();
            }
        }));
        swingToolbar.add(createToolButton("zoom-original", Messages.gettext("Normal 100%"), new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                jcefBrowser.resetFont();
            }
        }));
        swingToolbar.add(createToolButton("zoom-in", Messages.gettext("Zoom In"), new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                jcefBrowser.increaseFont();
            }
        }));

        helpTab.addToolBar(toolbar);

        ((SwingScilabDockablePanel) helpTab.getAsSimpleTab()).setAssociatedXMLIDForHelp("doc");
        WindowsConfigurationManager.restorationFinished((SwingScilabDockablePanel) helpTab.getAsSimpleTab());

        return (SwingScilabDockablePanel) helpTab.getAsSimpleTab();
    }

    /**
     * Create an icon-only toolbar button
     * @param iconName icon name (resolved by FindIconHelper)
     * @param tooltip tooltip text
     * @param listener action
     * @return the button
     */
    private static JButton createToolButton(String iconName, String tooltip, ActionListener listener) {
        JButton btn = new JButton();
        try {
            btn.setIcon(FindIconHelper.loadIcon(iconName));
        } catch (IOException e) {
            btn.setText(tooltip);
        }
        btn.setToolTipText(tooltip);
        btn.addActionListener(listener);
        return btn;
    }

    /**
     * Creates a Scilab Help Browser
     * @param helps help chapters and directories
     * @param language Scilab current language
     * @return the created Help Browser
     */
    public static HelpBrowser createHelpBrowser(String[] helps, String language) {
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
                    ScilabConsole.getConsole().getInfoBar().setText("");
                    return null;
                }
            }
        } else if (!language.equals(ScilabHelpBrowser.language)) {
            // Language changed — recreate the browser
            ScilabHelpBrowser.language = language;
            // TODO: reload with new language
        } else if (helps != null && !java.util.Arrays.deepEquals(helps, ScilabHelpBrowser.helps)) {
            // A new help chapter was registered (e.g. a toolbox loaded after the
            // browser was opened): refresh the TOC in the running browser.
            ScilabHelpBrowser.helps = helps;
            ((SwingScilabHelpBrowser) ((ScilabHelpBrowser) instance).component).refresh(helps);
        }

        ScilabGUIUtilities.toFront((SwingScilabDockablePanel) helpTab.getAsSimpleTab(), Messages.gettext("Help Browser"));

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
        // The JCEF browser loads index.html automatically
    }

    /**
     * @return the current displayed url as String
     */
    public String getCurrentURL() {
        SwingScilabHelpBrowser browser = (SwingScilabHelpBrowser) ((ScilabHelpBrowser) instance).component;
        return browser.getCurrentURL();
    }

    /**
     * @return the current displayed id
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
     * Reset the font of the help viewer to its default size
     */
    public void resetFont() {
        SwingScilabHelpBrowser browser = (SwingScilabHelpBrowser) ((ScilabHelpBrowser) instance).component;
        browser.resetFont();
    }

    /**
     * Show the search field
     */
    public void showSearchField() {
        SwingScilabHelpBrowser browser = (SwingScilabHelpBrowser) ((ScilabHelpBrowser) instance).component;
        browser.showSearchField();
    }

    /**
     * Hide the search field
     */
    public void hideSearchField() {
        // Search field is always visible in JCEF browser
    }

    /**
     * Get the Scilab Help Browser
     * @return the help browser instance
     */
    public static HelpBrowser getHelpBrowser() {
        if (instance == null) {
            instance = new ScilabHelpBrowser(null, "en_US");
        }
        return instance;
    }

    /**
     * Get the Scilab Help Browser (does not create it if not existing)
     * @return the help browser instance or null
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

    public Size getDims() {
        throw new UnsupportedOperationException();
    }

    public Position getPosition() {
        throw new UnsupportedOperationException();
    }

    public boolean isVisible() {
        throw new UnsupportedOperationException();
    }

    public void setDims(Size newSize) {
        throw new UnsupportedOperationException();
    }

    public void setPosition(Position newPosition) {
        throw new UnsupportedOperationException();
    }

    public void setVisible(boolean newVisibleState) {
        throw new UnsupportedOperationException();
    }

    /**
     * Get associated InfoBar
     * @return the infobar of the parent tab
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
