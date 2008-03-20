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

import java.io.File;
import java.io.FilenameFilter;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URL;

import javax.help.BadIDException;
import javax.help.DefaultHelpModel;
import javax.help.HelpSet;
import javax.help.HelpSetException;
import javax.help.JHelp;

import org.scilab.modules.gui.helpbrowser.SimpleHelpBrowser;

/**
 * Scilab Help Browser in GUIs
 * @author Vincent COUVERT
 */
public class SwingScilabHelpBrowser extends JHelp implements SimpleHelpBrowser {
	
	private static final long serialVersionUID = 5306766011092074961L;
	
	private String jarExtension = "_help.jar";
	private String mainJarPath = System.getenv("SCI") + "/modules/helptools/jar/scilab_";
	private String defaultLanguage = "en_US";
    private HelpSet helpSet;

    /**
	 * Constructor
	 * @param helps help chapters and directories
	 * @param language Scilab current language
	 */		
	public SwingScilabHelpBrowser(String[] helps, String language) {
		super();	
		
		File[] jarFiles;
		if (helps != null) {
			jarFiles = new File[helps.length + 1]; /* +1 because of Scilab main help jar file */
		} else {
			jarFiles = new File[1]; /* Scilab main help jar file */
		}

	    /* Main Jar file */
	    File mainJar = new File(mainJarPath + language + jarExtension);
	    if (!mainJar.exists()) {
	    	mainJar = new File(mainJarPath + defaultLanguage + jarExtension);
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

	    		/* Get the default language help and the current language help */
	    		int defaultHelpIndex = -1;
	    		int localeHelpIndex = -1;
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
				return;
			}
			this.getModel().getHelpSet().add(helpSet);
        }

	}
	
	/**
	 * Display the Help Browser
	 */
	public void display() {
		
	}
	
	/**
	 * Display the matching items for a specified keyword
	 * @param keyword the keyword
	 * @return true if the keyword exists
	 */
	public boolean searchKeywork(String keyword) {
		try {
			setCurrentID(keyword);
			return true;
		} catch (BadIDException e) {
			return false;
		}
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
