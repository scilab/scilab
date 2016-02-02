/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
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

package org.scilab.modules.gui.bridge.helpbrowser;

import java.awt.BorderLayout;
import java.awt.Cursor;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.io.File;
import java.io.FilenameFilter;
import java.lang.reflect.Field;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URL;
import java.util.Enumeration;
import java.util.Locale;

import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.SwingUtilities;

import javax.help.BadIDException;
import javax.help.DefaultHelpHistoryModel;
import javax.help.DefaultHelpModel;
import javax.help.HelpSet;
import javax.help.HelpSetException;
import javax.help.JHelp;
import javax.help.JHelpIndexNavigator;
import javax.help.JHelpSearchNavigator;
import javax.help.JHelpTOCNavigator;
import javax.help.SwingHelpUtilities;
import javax.help.event.HelpModelEvent;
import javax.help.event.HelpModelListener;
import javax.help.plaf.basic.BasicSearchNavigatorUI;
import javax.help.plaf.basic.BasicTOCNavigatorUI;
import javax.help.search.SearchQuery;

import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.helpbrowser.SimpleHelpBrowser;
import org.scilab.modules.gui.messagebox.MessageBox;
import org.scilab.modules.gui.messagebox.ScilabMessageBox;
import org.scilab.modules.gui.utils.HelpSearchField;
import org.scilab.modules.localization.Messages;

/**
 * Scilab Help Browser in GUIs
 * @author Vincent COUVERT
 */
public class SwingScilabHelpBrowser extends JPanel implements SimpleHelpBrowser, HelpModelListener {

    private static final long serialVersionUID = 5306766011092074961L;

    private static String defaultLanguage = "en_US";
    private static String jarExtension = "_help.jar";
    private static String mainJarPath = System.getenv("SCI") + "/modules/helptools/jar/scilab_";

    private String currentLanguage = "";
    private JHelp jhelp;
    private HelpSet helpSet;
    private URL homePageURL;
    private HelpSearchField searchField;

    /* We are storing the HelpHistory model to be able to
     * use the back/forward feature from the jpopupmenu
     */
    private static DefaultHelpHistoryModel helpHistory;

    static {
        SwingHelpUtilities.setContentViewerUI("org.scilab.modules.gui.bridge.helpbrowser.SwingScilabHelpBrowserViewer");
    }

    /**
     * Return the History model
     * @return The DefaultHelpHistoryModel which is called
     * from the popup menu
     */
    public static DefaultHelpHistoryModel getHelpHistory() {
        return helpHistory;
    }

    /**
     * Constructor
     * @param helps help chapters and directories
     * @param language Scilab current language
     */
    public SwingScilabHelpBrowser(String[] helps, String language) {
        super(new BorderLayout());
        jhelp = new JHelp();
        add(jhelp);
        setFocusable(true);
        searchField = new HelpSearchField(this, null);

        /* Send information to the user using status bar and cursor */
        if (ScilabConsole.isExistingConsole() && ScilabConsole.getConsole().getInfoBar() != null) {
            ScilabConsole.getConsole().getInfoBar().setText(Messages.gettext("Loading help browser..."));
            ScilabConsole.getConsole().setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        }

        File[] jarFiles;
        if (helps != null) {
            jarFiles = new File[helps.length + 1]; /* +1 because of Scilab main help jar file */
        } else {
            jarFiles = new File[1]; /* Scilab main help jar file */
        }

        /* Main Jar file */
        File mainJar = new File(mainJarPath + language + jarExtension);
        currentLanguage = language;
        if (!mainJar.exists()) {
            mainJar = new File(mainJarPath + defaultLanguage + jarExtension);
            currentLanguage = defaultLanguage;
        }

        if (!mainJar.exists()) {
            String message = "'SCI/modules/helptools/jar/scilab_" + defaultLanguage + jarExtension + "' has not been found on the system.\n"
                             + "" + "Are you sure you built it?\nThe help will not be available.";
            if (ScilabConsole.isExistingConsole()) {
                MessageBox messageBox = ScilabMessageBox.createMessageBox();
                messageBox.setMessage(message);
                messageBox.setModal(true);
                messageBox.setIcon("error");
                messageBox.displayAndWait();
            } else {
                System.out.println(message);
            }
            return;
        }

        int nbFilesToLoad = 0;
        jarFiles[nbFilesToLoad++] = mainJar;

        /* Toolboxes jar files */
        if (helps != null) {
            for (int k = 0; k < helps.length; k++) {
                File toolboxJarPath = new File(helps[k]);

                /* Find all Jars */
                String[] all = toolboxJarPath.list(new FilenameFilter() {
                    public boolean accept(File pathname, String filename) {
                        return filename.endsWith(jarExtension);
                    }
                });

                // For toolboxes JAR files are stored in jar directory
                if (all == null || all.length == 0) {
                    toolboxJarPath = new File(helps[k] + "/../../jar");
                    all = toolboxJarPath.list(new FilenameFilter() {
                        public boolean accept(File pathname, String filename) {
                            return filename.endsWith(jarExtension);
                        }
                    });
                }

                /* Get the default language help and the current language help */
                int defaultHelpIndex = -1;
                int localeHelpIndex = -1;
                if (all != null) {
                    for (int fileIndex = 0; fileIndex < all.length; fileIndex++) {
                        if (all[fileIndex].indexOf(defaultLanguage) != -1) {
                            defaultHelpIndex = fileIndex;
                        }
                        if (all[fileIndex].indexOf(language) != -1) {
                            localeHelpIndex = fileIndex;
                        }
                    }

                    /* Add the toolbox help file */
                    if (localeHelpIndex != -1) {
                        jarFiles[nbFilesToLoad++] =  new File(toolboxJarPath + File.separator + all[localeHelpIndex]);
                    } else if (defaultHelpIndex != -1) {
                        jarFiles[nbFilesToLoad++] =  new File(toolboxJarPath + File.separator + all[defaultHelpIndex]);
                    } else if ((all != null) && (all.length != 0)) {
                        jarFiles[nbFilesToLoad++] =  new File(toolboxJarPath + File.separator + all[0]); /* First file as default */
                    }
                }
            }
        }
        jhelp.setModel(new DefaultHelpModel(new HelpSet()));

        for (int i = 0; i < nbFilesToLoad; ++i) {
            URI jarURI = jarFiles[i].toURI();

            StringBuilder buffer = new StringBuilder("jar:");
            buffer.append(jarURI);
            buffer.append("!/");
            buffer.append(rootName(jarURI));
            buffer.append("/jhelpset.hs");

            URL helpSetURL = null;
            try {
                helpSetURL = new URL(buffer.toString());
            } catch (MalformedURLException cannotHappen) {
                cannotHappen.printStackTrace();
                continue;
            }

            try {
                helpSet = new HelpSet(/*classLoader*/ null, helpSetURL);
            } catch (HelpSetException e) {
                System.err.println("Could not load file: " + jarFiles[i] + ". Please check its contents, must be a Java Help file.");
                System.err.println("Error message: " + e.getLocalizedMessage());
                /* Reinit status bar and cursor */
                if (ScilabConsole.isExistingConsole() && ScilabConsole.getConsole().getInfoBar() != null) {
                    ScilabConsole.getConsole().getInfoBar().setText("");
                    ScilabConsole.getConsole().setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
                }
                return;
            }
            jhelp.getModel().getHelpSet().add(helpSet);
        }

        /** Disable Index navigator because no index in Scilab help files */
        Enumeration navigators = jhelp.getHelpNavigators();
        while (navigators.hasMoreElements()) {
            Object nav = navigators.nextElement();
            if (nav instanceof JHelpIndexNavigator) {
                jhelp.removeHelpNavigator((JHelpIndexNavigator) nav);
                break;
            }
        }

        navigators = jhelp.getHelpNavigators();
        while (navigators.hasMoreElements()) {
            Object nav = navigators.nextElement();
            if (nav instanceof JHelpTOCNavigator) {
                BasicTOCNavigatorUI tocUI = (BasicTOCNavigatorUI) ((JHelpTOCNavigator) nav).getUI();
                JScrollPane scroll = null;
                try {
                    Field f = BasicTOCNavigatorUI.class.getDeclaredField("sp");
                    f.setAccessible(true);
                    scroll = (JScrollPane) f.get(tocUI);
                } catch (Exception e) { }

                if (scroll != null) {
                    final JScrollPane sp = scroll;
                    sp.addComponentListener(new ComponentAdapter() {
                        public void componentResized(ComponentEvent e) {
                            sp.getHorizontalScrollBar().setValue(0);
                            sp.getVerticalScrollBar().setValue(0);
                            sp.removeComponentListener(this);
                        }
                    });
                }
                break;
            }
        }

        /* Reinit status bar and cursor */
        if (ScilabConsole.isExistingConsole() && ScilabConsole.getConsole().getInfoBar() != null) {
            ScilabConsole.getConsole().getInfoBar().setText("");
            ScilabConsole.getConsole().setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
        }

        helpHistory = (DefaultHelpHistoryModel) jhelp.getHistoryModel();

        try {
            homePageURL = new URL(jhelp.getModel().getHelpSet().getHelpSetURL().toString().replace("jhelpset.hs", "ScilabHomePage.html"));
        } catch (MalformedURLException ex) { }

        jhelp.getContentViewer().addHelpModelListener(this);
        setVisible(true);
    }

    /**
     * @param language the preferred language
     * @return true if the main help jar file exists
     */
    public static boolean isMainJarExists(String language) {
        File mainJar = new File(mainJarPath + language + jarExtension);
        if (!mainJar.exists()) {
            mainJar = new File(mainJarPath + defaultLanguage + jarExtension);
            if (!mainJar.exists()) {
                return false;
            }
        }

        return true;
    }

    /**
     * @return the current URL as String being displayed
     */
    public String getCurrentURL() {
        return ((SwingScilabHelpBrowserViewer) jhelp.getContentViewer().getUI()).getCurrentURL();
    }

    /**
     * @return the current id as String being displayed
     */
    public String getCurrentID() {
        if (jhelp.getModel().getCurrentID() != null) {
            String id = jhelp.getModel().getCurrentID().toString();
            int whitePos = id.indexOf(" ");
            int commaPos = id.indexOf(",");

            if (whitePos != -1 && commaPos != -1) {
                id = id.substring(whitePos + 1, commaPos);
            }

            return id;
        } else {
            return null;
        }
    }

    /**
     * @return the current id as String being displayed
     */
    public void setCurrentID(final String id) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                try {
                    jhelp.setCurrentID(id);
                } catch (BadIDException e) {
                    jhelp.setCurrentURL(homePageURL);
                }
            }
        });
    }

    /**
     * Show the search field
     */
    public void showSearchField() {
        searchField.setTextComponent(((SwingScilabHelpBrowserViewer) jhelp.getContentViewer().getUI()).getAccessibleHTML());
        searchField.showField();
    }

    /**
     * Hide the search field
     */
    public void hideSearchField() {
        searchField.hideField();
    }

    /**
     * Increase the font of the help viewer
     */
    public void increaseFont() {
        ((SwingScilabHelpBrowserViewer) jhelp.getContentViewer().getUI()).increaseFont();
    }

    /**
     * Decrease the font of the help viewer
     */
    public void decreaseFont() {
        ((SwingScilabHelpBrowserViewer) jhelp.getContentViewer().getUI()).decreaseFont();
    }

    /**
     * {@inheritDoc}
     */
    public void idChanged(HelpModelEvent e) {
        searchField.setTextComponent(((SwingScilabHelpBrowserViewer) jhelp.getContentViewer().getUI()).getAccessibleHTML());
    }

    /**
     * Display the home page
     */
    public void displayHomePage() {
        if (homePageURL != null) {
            setCurrentURL(homePageURL);
        }
    }

    /**
     * Sets the current URL
     * @param url the URL to display
     */
    public void setCurrentURL(final URL url) {
        SwingUtilities.invokeLater(new Runnable() {

            public void run() {
                jhelp.setCurrentURL(url);
            }
        });
    }

    /**
     * Sets the current url
     * @param url the url as String to display
     */
    public void setCurrentURL(String url) {
        URL u = homePageURL;
        try {
            if (url != null) {
                u = new URL(url);
            }
        } catch (MalformedURLException e) { }
        setCurrentURL(u);
    }

    /**
     * Display the Help Browser
     */
    public void display() { }

    /**
     * Display the matching items for a specified keyword
     * @param keyword the keyword
     */
    public void searchKeywork(String keyword) {
        if (keyword == null) {
            displayHomePage();
            return;
        }

        if (keyword.length() > 0 && keyword.charAt(0) == '%') {
            keyword = keyword.replace("%", "percent");
        }
        Enumeration navigators = jhelp.getHelpNavigators();
        if (navigators.hasMoreElements()) {
            jhelp.setCurrentNavigator((JHelpTOCNavigator) navigators.nextElement());
        } else {
            System.out.println("Scilab help files not loaded. Please check if "
                               + mainJarPath + "*" + jarExtension + " files exist and are Java Help files.");
            return;
        }
        final String kw = keyword;
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                try {
                    jhelp.setCurrentID(kw);
                } catch (BadIDException e) {
                    fullTextSearch(kw);
                }
            }
        });
    }

    /**
     * Display the matching help pages for a specified keyword
     * @param keyword the keyword to search
     */
    public void fullTextSearch(String keyword) {
        Enumeration navigators = jhelp.getHelpNavigators();
        while (navigators.hasMoreElements()) {
            Object nav = navigators.nextElement();
            if (nav instanceof JHelpSearchNavigator) {
                JHelpSearchNavigator searchNav = (JHelpSearchNavigator) nav;
                jhelp.setCurrentNavigator(searchNav);
                SearchQuery query = searchNav.getSearchEngine().createQuery();
                query.addSearchListener((BasicSearchNavigatorUI) searchNav.getUI());
                query.start(keyword, new Locale(currentLanguage));
                break;
            }
        }
    }

    /**
     * Close the HelpBrowser
     */
    public void close() { }

    /**
     * Get teh root name of an URI
     * @param uri the URI
     * @return the root name
     */
    private static String rootName(URI uri) {
        String path = uri.toString();

        String baseName;
        int pos = path.lastIndexOf('/');
        if (pos < 0) {
            baseName = path;
        } else {
            baseName = path.substring(pos + 1);
        }
        String rootName;
        pos = baseName.lastIndexOf('.');
        if (pos < 0) {
            rootName = baseName;
        } else {
            rootName = baseName.substring(0, pos);
        }

        return rootName;
    }
}
