package org.scilab.modules.gui.utils;

import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

/**
 * Class used to modify look and feel in Scilab GUIs
 * @author Allan CORNET - INRIA 2007
 * @author Sylvestre Ledru INRIA 2007
 */
public class LookAndFeel {
	private static UIManager.LookAndFeelInfo[] availableLookAndFeels;

	/**
	 * Constructor
	 */
	public LookAndFeel() {
		availableLookAndFeels = UIManager.getInstalledLookAndFeels();
	}
	
	/**
	 * Get Installed Look and Feels
	 * @return a array of String
	 */
	public static String[] getInstalledLookAndFeels() {
		String[] retStrings = new String[availableLookAndFeels.length];
		for (int i = 0; i < availableLookAndFeels.length; i++) {
			retStrings[i] =  availableLookAndFeels[i].getClassName();
		}		
		return retStrings;
	}
	
	/**
	 * Get Look and Feel
	 * @return a String
	 */
	public static String getCurrentLookAndFeel() {
		return UIManager.getLookAndFeel().getClass().getName();
	}

    /**
     * Is this look and feel exists or not
	 * @param lookandfeel the name of the lookandfeel we want check if exists
     * @return if it exists or not
	 */
	public static boolean isSupportedLookAndFeel(String lookandfeel) {
		for (int i = 0; i < availableLookAndFeels.length; i++) {
			if (availableLookAndFeels[i].getClassName().equals(lookandfeel)) {
				return true;
			}
		} 
		return false;
	}
	
	/**
	 * Set Look and Feel
	 * @param lookandfeel a String 
	 * @return a boolean
	 */
	public static boolean setLookAndFeel(String lookandfeel) {
		try {
			UIManager.setLookAndFeel(lookandfeel);
			return true;
			 } catch (UnsupportedLookAndFeelException e) {
				 System.err.println("Cannot find this look and feel:");
				 System.err.println(e.getLocalizedMessage());
			 } catch (ClassNotFoundException e) {
				 System.err.println("LookAndFeel class could not be found:");
				 System.err.println(e.getLocalizedMessage());
			 } catch (IllegalAccessException e) {
				 System.err.println("Illegal access while setting the Look And Feel:");
				 System.err.println(e.getLocalizedMessage());
			 } catch (InstantiationException e) {
				 System.err.println("Instantiation error while setting the Look And Feel:");
				 System.err.println(e.getLocalizedMessage());
			 }
	return false;
	}
		
	
	/**
	 * Set System Look and Feel
	 * @return a boolean
	 */
	public static boolean setSystemLookAndFeel() {
		return setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
	}
	
}
 


