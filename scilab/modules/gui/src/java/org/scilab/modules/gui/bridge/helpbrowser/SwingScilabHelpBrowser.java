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
package org.scilab.modules.gui.bridge.helpbrowser;
import java.awt.Cursor;
import java.io.File;
import java.io.FilenameFilter;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URL;
import java.util.Enumeration;
import java.util.Locale;
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
import javax.help.plaf.basic.BasicSearchNavigatorUI;
import javax.help.search.SearchQuery;

import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.helpbrowser.SimpleHelpBrowser;
import org.scilab.modules.gui.messagebox.MessageBox;
import org.scilab.modules.gui.messagebox.ScilabMessageBox;
import org.scilab.modules.localization.Messages;

/**
 * Scilab Help Browser in GUIs
 * @author Vincent COUVERT
 */
public class SwingScilabHelpBrowser extends JHelp implements SimpleHelpBrowser {
	
	private static final long serialVersionUID = 5306766011092074961L;
	
	private String jarExtension = "_help.jar";
	private String mainJarPath = System.getenv("SCI") + "/modules/helptools/jar/scilab_";
	private String defaultLanguage = "en_US";
	private String currentLanguage = "";
    private HelpSet helpSet;
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
		super();
			
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
	    	String message = "'" + mainJarPath + defaultLanguage + jarExtension + "' has not been found on the system. " 
	    		+ "" + "Are you sure you built it? The help will not be available.";
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
	    this.setModel(new DefaultHelpModel(new HelpSet()));
        
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
				System.out.println("Could not load file: " + jarFiles[i] + ". Please check its contents, must be a Java Help file.");
				/* Reinit status bar and cursor */
				if (ScilabConsole.isExistingConsole() && ScilabConsole.getConsole().getInfoBar() != null) {
					ScilabConsole.getConsole().getInfoBar().setText("");
					ScilabConsole.getConsole().setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
				}
				return;
			}
			this.getModel().getHelpSet().add(helpSet);
        }
	    
	    /** Disable Index navigator because no index in Scilab help files */
	    Enumeration navigators = getHelpNavigators();
	    navigators.nextElement(); /* TOC */
	    removeHelpNavigator((JHelpIndexNavigator) navigators.nextElement());
	    
		/* Reinit status bar and cursor */
		if (ScilabConsole.isExistingConsole() && ScilabConsole.getConsole().getInfoBar() != null) {
			ScilabConsole.getConsole().getInfoBar().setText("");
			ScilabConsole.getConsole().setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
		}
		helpHistory = new DefaultHelpHistoryModel(this);
	}
	
	/**
	 * Display the Help Browser
	 */
	public void display() {
		
	}
	
	/**
	 * Display the matching items for a specified keyword
	 * @param keyword the keyword
	 */
	public void searchKeywork(String keyword) {
		try {
			Enumeration navigators = getHelpNavigators();
			if (navigators.hasMoreElements()) {
				setCurrentNavigator((JHelpTOCNavigator) navigators.nextElement());
			} else {
				System.out.println("Scilab help files not loaded. Please check if " 
						+ mainJarPath + "*" + jarExtension + " files exist and are Java Help files.");
				return;
			}
			setCurrentID(keyword);
		} catch (BadIDException e) {
			fullTextSearch(keyword);
		}
	}
    
	/**
	 * Display the matching help pages for a specified keyword
	 * @param keyword the keyword to search
	 */
	public void fullTextSearch(String keyword) {
		Enumeration navigators = getHelpNavigators();
    
		navigators.nextElement(); // JHelpTOCNavigator
		//navigators.nextElement();	// JHelpIndexNavigator 
		JHelpSearchNavigator searchNav = (JHelpSearchNavigator) navigators.nextElement();

		setCurrentNavigator(searchNav);
    
		SearchQuery query = searchNav.getSearchEngine().createQuery();
		query.addSearchListener((BasicSearchNavigatorUI) searchNav.getUI());
		query.start(keyword, new Locale(currentLanguage));
	}
	
	/**
	 * Close the HelpBrowser
	 */
	public void close() {
		
	}

	
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
