/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import javax.swing.JOptionPane;

/**
 * Class used to launch a Web Browser from Scilab
 * @author Vincent COUVERT
 */
public final class WebBrowser {
	
	private static final String ERROR_MSG = "Could not open a Web Browser";

	/**
	 * Constructor
	 */
	private WebBrowser() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Open the Browser with Given URL
	 * @param url the URL to open
	 */
	public static void openUrl(String url) {
		
		String os = System.getProperty("os.name");
		
		Runtime runtime = Runtime.getRuntime();
		
		try {
			if (os.startsWith("Windows")) { /* Windows */
				
				String cmd = "rundll32 url.dll,FileProtocolHandler " +  url;
				Process p = runtime.exec(cmd);
			
			} else if (os.startsWith("Mac OS")) { /* MacOS */
			
				Class fileMgr = Class.forName("com.apple.eio.FileManager");
				Method openURL = fileMgr.getDeclaredMethod("openURL", new Class[] {String.class});
				openURL.invoke(null, new Object[] {url});
			
			} else { /* Unix... */
				
				String[] browsers = {"firefox", "opera", "konqueror", "epiphany", "mozilla", "netscape" };
				String browser = null;
				
				for (int count = 0; count < browsers.length && browser == null; count++) {
					if (runtime.exec(new String[] {"which", browsers[count]}).waitFor() == 0) {
						browser = browsers[count];
					}
				}
				
				if (browser == null) {
					throw new IOException(ERROR_MSG);
				} else {
					runtime.exec(new String[] {browser, url});
				}
				
			}
		} catch (IOException x) {
			JOptionPane.showMessageDialog(null, ERROR_MSG);
		} catch (ClassNotFoundException x) {
			JOptionPane.showMessageDialog(null, ERROR_MSG);
		} catch (NoSuchMethodException x) {
			JOptionPane.showMessageDialog(null, ERROR_MSG);
		} catch (InvocationTargetException x) {
			JOptionPane.showMessageDialog(null, ERROR_MSG);
		} catch (IllegalAccessException x) {
			JOptionPane.showMessageDialog(null, ERROR_MSG);
		} catch (InterruptedException x) {
			JOptionPane.showMessageDialog(null, ERROR_MSG);
		}
	}
}
