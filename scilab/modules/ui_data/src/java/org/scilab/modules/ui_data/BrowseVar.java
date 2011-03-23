/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Sylvestre KOUMAR
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.ui_data;

import javax.swing.ImageIcon;

import org.scilab.modules.ui_data.variablebrowser.ScilabVariableBrowser;

/**
 * 
 * Static class to open/close Scilab Variable browser
 *
 */
public class BrowseVar {

	public static final int ICON_COLUMN_INDEX = 0;
	public static final int NAME_COLUMN_INDEX = 1;
	public static final int BYTES_COLUMN_INDEX = 2;
	public static final int TYPE_COLUMN_INDEX = 3;
	public static final int VISIBILITY_COLUMN_INDEX = 4;

	private static final String ICON_PATH = System.getenv("SCI") + "/modules/ui_data/images/icons/";

	private static final ImageIcon TP00 = new ImageIcon(ICON_PATH + "tp00.gif");
	private static final ImageIcon TP01 = new ImageIcon(ICON_PATH + "tp01gif");
	private static final ImageIcon TP02 = new ImageIcon(ICON_PATH + "tp02.gif");
	private static final ImageIcon TP04 = new ImageIcon(ICON_PATH + "tp04.gif");
	private static final ImageIcon TP05 = new ImageIcon(ICON_PATH + "tp05.gif");
	private static final ImageIcon TP06 = new ImageIcon(ICON_PATH + "tp06.gif");
	private static final ImageIcon TP08 = new ImageIcon(ICON_PATH + "tp08.gif");
	private static final ImageIcon TP10 = new ImageIcon(ICON_PATH + "tp10.gif");
	private static final ImageIcon TP15 = new ImageIcon(ICON_PATH + "tp15.gif");
	private static final ImageIcon TP16 = new ImageIcon(ICON_PATH + "tp16.gif");
	private static final ImageIcon TP17 = new ImageIcon(ICON_PATH + "tp17.gif");

	/**
	 * Default private constructor for utility class
	 */
	private BrowseVar() { }

	/**
	 * Get ImageIcon instance from Scilab type (as int)
	 * @param type : scilab type as integer
	 * @return instance of type Icon
	 */
	private static ImageIcon getIconFromType(int type) {
		switch (type) {
		case 1:
			return TP01;
		case 2:
			return TP02;
		case 4:
			return TP04;
		case 5:
			return TP05;
		case 6:
			return TP06;
		case 8:
			return TP08;
		case 10:
			return TP10;
		case 15:
			return TP15;
		case 16:
			return TP16;
		case 17:
			return TP17;
		default:
			return TP00;
		}
	}

	/**
	 * Open variable Browser with information given columnsNames
	 * @param columnNames titles of the JTable
	 */

	public static void openVariableBrowser(String[] columnNames) {
		ScilabVariableBrowser.getVariableBrowser(columnNames);
	}

	/**
	 * Open variable Browser with information given by Scilab
	 * @param columnNames : titles of the JTable
	 * @param dataNames : scilab variable name
	 * @param dataBytes : scilab variable size in bytes
	 * @param dataTypes : scilab variable type (as integer)
	 * @param dataVisibility : local or global variable
	 */
	public static void openVariableBrowser(String[] columnNames, String[] dataNames, int[] dataBytes, int[] dataTypes, String[] dataVisibility) {
		Object[][] data = new Object[dataNames.length][columnNames.length];
		for (int i = 0; i < dataNames.length; ++i) {
			data[i][ICON_COLUMN_INDEX] = getIconFromType(dataTypes[i]);
			data[i][NAME_COLUMN_INDEX] = dataNames[i];
			data[i][BYTES_COLUMN_INDEX] = dataBytes[i];
			data[i][TYPE_COLUMN_INDEX] = dataTypes[i];
			data[i][VISIBILITY_COLUMN_INDEX] = dataVisibility[i];
		}
		ScilabVariableBrowser.getVariableBrowser(columnNames, data);
	}

	/**
	 * Open Variable browser
	 */
	public static void openVariableBrowser() {
		ScilabVariableBrowser.getVariableBrowser();
	}

	/**
	 * Close Variable Browser
	 */
	public static void closeVariableBrowser() {
		ScilabVariableBrowser.getVariableBrowser().close();
	}
}
