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
import javax.swing.JLabel;

import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.types.ScilabTypeEnumDescription;
import org.scilab.modules.ui_data.variablebrowser.ScilabVariableBrowser;

/**
 *
 * Static class to open/close Scilab Variable browser
 *
 */
public class BrowseVar {

    public static final int ICON_COLUMN_INDEX = 0;
    public static final int NAME_COLUMN_INDEX = 1;
    public static final int SIZE_COLUMN_INDEX = 2;
    public static final int TYPE_DESC_COLUMN_INDEX = 3;
    public static final int VISIBILITY_COLUMN_INDEX = 4;
    public static final int BYTES_COLUMN_INDEX = 5;
    public static final int FROM_SCILAB_COLUMN_INDEX = 6;
    public static final int TYPE_COLUMN_INDEX = 7;

    public static final String[] COLUMNNAMES = new String[]{"", /* Icon */
                                                            Messages.gettext("Name"),
                                                            Messages.gettext("Dimension"),
                                                            Messages.gettext("Type"),
                                                            Messages.gettext("Visibility"),
                                                            Messages.gettext("Bytes"),
                                                            Messages.gettext("User"),
                                                            Messages.gettext("Type int value")
    };

    public static final int[] COLUMNSALIGNMENT = new int[]{-1, JLabel.LEFT, JLabel.RIGHT, JLabel.RIGHT, JLabel.RIGHT, JLabel.RIGHT, JLabel.RIGHT, JLabel.RIGHT};

    private static final ImageIcon NO_ICON = new ImageIcon(ScilabSwingUtilities.findIcon("noicon"));
    private static final ImageIcon DOUBLE_ICON = new ImageIcon(ScilabSwingUtilities.findIcon("double"));
    private static final ImageIcon POLYNOMIAL_ICON = new ImageIcon(ScilabSwingUtilities.findIcon("polynomial"));
    private static final ImageIcon BOOLEAN_ICON = new ImageIcon(ScilabSwingUtilities.findIcon("boolean"));
    private static final ImageIcon SPARSE_ICON = new ImageIcon(ScilabSwingUtilities.findIcon("sparse"));
    private static final ImageIcon INT_ICON = new ImageIcon(ScilabSwingUtilities.findIcon("int"));
    private static final ImageIcon HANDLE_ICON = new ImageIcon(ScilabSwingUtilities.findIcon("handle"));
    private static final ImageIcon STRING_ICON = new ImageIcon(ScilabSwingUtilities.findIcon("string"));
    private static final ImageIcon FUNCTION_ICON = new ImageIcon(ScilabSwingUtilities.findIcon("function"));
    private static final ImageIcon LIST_ICON = new ImageIcon(ScilabSwingUtilities.findIcon("list"));
    private static final ImageIcon TLIST_ICON = new ImageIcon(ScilabSwingUtilities.findIcon("tlist"));
    private static final ImageIcon MLIST_ICON = new ImageIcon(ScilabSwingUtilities.findIcon("mlist"));
    private static final ImageIcon USER_ICON = new ImageIcon(ScilabSwingUtilities.findIcon("user"));
    private static final ImageIcon FPTR_ICON = new ImageIcon(ScilabSwingUtilities.findIcon("fptr"));

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
            return DOUBLE_ICON;
        case 2:
            return POLYNOMIAL_ICON;
        case 4:
            return BOOLEAN_ICON;
        case 5:
        case 6:
        case 7:
            return SPARSE_ICON;
        case 8:
            return INT_ICON;
        case 9:
            return HANDLE_ICON;
        case 10:
            return STRING_ICON;
        case 11:
        case 13:
            return FUNCTION_ICON;
            /*case 14:
              return LIBRARY_ICON;*/
        case 15:
            return LIST_ICON;
        case 16:
            return TLIST_ICON;
        case 17:
            return MLIST_ICON;
        case 128:
            return USER_ICON;
        case 130:
            return FPTR_ICON;
        default:
            return NO_ICON;
        }
    }

    /**
     * Open variable Browser
     */
    public static void openVariableBrowser(boolean update) {
        ScilabVariableBrowser.getVariableBrowser(update);
    }

    /**
     * Open variable Browser with information given by Scilab
     * @param dataNames : scilab variable name
     * @param dataBytes : scilab variable size in bytes
     * @param dataTypes : scilab variable type (as integer)
     * @param dataSizes : scilab variable size under the form "XxX"
     * @param dataVisibility : local or global variable
     * @param dataFromUser : Scilab data or user data
     */
    public static void openVariableBrowser(boolean update, String[] dataNames, int[] dataBytes, int[] dataTypes, String[] dataSizes, String[] dataVisibility, boolean[] dataFromUser) {
        Object[][] data = new Object[dataNames.length][COLUMNNAMES.length];
        for (int i = 0; i < dataNames.length; ++i) {
            data[i][ICON_COLUMN_INDEX] = getIconFromType(dataTypes[i]);
            data[i][NAME_COLUMN_INDEX] = dataNames[i];
            data[i][SIZE_COLUMN_INDEX] = dataSizes[i];
            data[i][TYPE_DESC_COLUMN_INDEX] = ScilabTypeEnumDescription.getTypeDescriptionFromId(dataTypes[i]);
            data[i][VISIBILITY_COLUMN_INDEX] = dataVisibility[i];
            data[i][BYTES_COLUMN_INDEX] = dataBytes[i];
            data[i][FROM_SCILAB_COLUMN_INDEX] = dataFromUser[i]; /* Tag if it is a variable from the user or from Scilab (%pi, %eps, etc) */
            data[i][TYPE_COLUMN_INDEX] = dataTypes[i];
        }
        ScilabVariableBrowser.getVariableBrowser(update, data);
    }

    /**
     * @return true if an instance of BrowseVar already exists.
     */
    public static boolean isVariableBrowserOpened() {
        return ScilabVariableBrowser.isBrowseVarOpened();
    }

    /**
     * Close Variable Browser
     */
    public static void closeVariableBrowser() {
        ScilabVariableBrowser.closeVariableBrowser();
    }
}
