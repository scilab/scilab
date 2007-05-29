/**
 * @author Allan CORNET - INRIA 2007
 */
package org.scilab.modules.gui.utils;

import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

/**
 * Class used to modify look and feel in Scilab GUIs
 * @author Allan CORNET - INRIA 2007
 */
public class LookAndFeel {
	/**
	 * Constructor
	 */
	protected LookAndFeel() {
	/* Prevents calls from subclass */
	throw new UnsupportedOperationException(); 		
	}
	
	/**
	 * Get Installed Look and Feels
	 * @return a array of String
	 */
	public static String[] getInstalledLookAndFeels() {
	UIManager.LookAndFeelInfo[] info = UIManager.getInstalledLookAndFeels();
    String[] retStrings = new String[info.length];
		for (int i = 0; i < info.length; i++) {
			retStrings[i] =  info[i].getClassName();
		}
		return retStrings;
	}
	
	/**
	 * Get Look and Feel
	 * @return a String
	 */
	public static String getLookAndFeel()	{
		String lnfclassname =  UIManager.getLookAndFeel().getClass().getName();
		return lnfclassname;
	}
	
	/**
	 * Set Look and Feel
	 * @param lookandfeel a String 
	 * @return a boolean
	 */
	public static boolean setLookAndFeel(String lookandfeel) {
		boolean bok = false;
		boolean blnfexists = false;
		String[] lookandfeels = getInstalledLookAndFeels();
		
		for (int i = 0; i < lookandfeels.length; i++) {
			if (lookandfeels[i].compareTo(lookandfeel) == 0) {
				blnfexists = true;
			}
		}
		
		if (!blnfexists) {
			return false;
		}
		
		return doSetLookAndFeel(lookandfeel);
	}
	
	/**
	 * Set System Look and Feel
	 * @return a boolean
	 */
	public static boolean setSystemLookAndFeel() {
		return doSetLookAndFeel(UIManager.getSystemLookAndFeelClassName());
	}
	
	/**
	 * internal method for SetLookAndFeel
	 * @param lookandfeel a String
	 * @return a boolean
	 */
	private static boolean doSetLookAndFeel(String lookandfeel) {
		boolean bok = false;
		try {
			UIManager.setLookAndFeel(lookandfeel);
			bok = true;
		}
		
		catch (UnsupportedLookAndFeelException e) {
			// handle exception
			bok = false;
		}
		
		catch (ClassNotFoundException e) {
			// handle exception
			bok = false; 
		}
		
		catch (InstantiationException e) {
		// handle exception
			bok = false;
		}
		
		catch (IllegalAccessException e) {
		// handle exception
			bok = false;
		}
		return bok;
	}
}
 


