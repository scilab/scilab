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

    private HelpSet helpSet;

    /**
	 * Constructor
	 * @param helps help chapters and directories
	 */		
	public SwingScilabHelpBrowser(String[] helps) {
		super();	
			
		/* First half of helps is directories list */
		/* Second half of helps is chapters titles */
		
	    File[] jarFiles = new File[helps.length / 2];
	    for (int k = 0; k < helps.length; k++) {
			System.out.println("Loading help from: " + helps[k]);
			jarFiles[k] =  new File(helps[k] + "scilab_help.jar");
		}
	    this.setModel(new DefaultHelpModel(new HelpSet()));
        
	    for (int i = 0; i < jarFiles.length; ++i) {
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
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
            //this.setModel(new DefaultHelpModel(helpSet));
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
