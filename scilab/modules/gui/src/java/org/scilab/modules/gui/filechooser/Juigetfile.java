/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Sylvestre KOUMAR
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

package org.scilab.modules.gui.filechooser;

import java.lang.reflect.InvocationTargetException;

import javax.swing.SwingUtilities;

import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.utils.ConfigManager;


/**
 * This class implements the ScilabFileChooser methods
 * by updating the FileChooserInfos attributes.
 * @author Sylvestre KOUMAR
 *
 */
public class Juigetfile {

    public static final String[] DEFAULT_MASK = {"*.bin", "*.sce", "*.xcos", "*.sci", "*.sc*"};
    public static final String DEFAULT_INITIAL_DIRECTORY = ConfigManager.getLastOpenedDirectory();
    public static final String DEFAULT_BOX_TITLE_OPEN = "uigetfile";
    public static final String DEFAULT_BOX_TITLE_SAVE = "uiputfile";
    public static final boolean DEFAULT_MULTIPLE_SELECTION = false;
    public static final int OPEN_DIALOG = 0;
    public static final int SAVE_DIALOG = 1;

    private static String[] mask;
    private static String[] description;
    private static String initialDirectory;
    private static String boxtTitle;
    private static boolean multipleSelection;
    private static int dialogType = SAVE_DIALOG;

    /**
     * PRIVATE constructor
     * We do not want a JScilabFileChooser object.
     */
    private Juigetfile() { }

    /**
     * uigetfile called with 0 arg
     */
    public static void uigetfile() {
        String[] mask = new String[DEFAULT_MASK.length];
        String[] description = new String[0];
        mask = DEFAULT_MASK;

        uigetfile(mask, description, DEFAULT_INITIAL_DIRECTORY, DEFAULT_BOX_TITLE_OPEN, DEFAULT_MULTIPLE_SELECTION, OPEN_DIALOG);
    }

    /**
     * uigetfile called with 1 arg (mask)
     * @param mask of the file chooser
     * @param description for each mask
     */
    public static void uigetfile(String[] mask, String[] description) {
        uigetfile(mask, description, DEFAULT_INITIAL_DIRECTORY, DEFAULT_BOX_TITLE_OPEN, DEFAULT_MULTIPLE_SELECTION, OPEN_DIALOG);
    }

    /**
     * uigetfile called with 2 args (mask, initial directory)
     * @param mask of the file chooser
     * @param description for each mask
     * @param initialDirectory of the opened file chooser
     */
    public static void uigetfile(String[] mask, String[] description, String initialDirectory) {
        uigetfile(mask, description, initialDirectory, DEFAULT_BOX_TITLE_OPEN, DEFAULT_MULTIPLE_SELECTION, OPEN_DIALOG);
    }

    /**
     * uigetfile called with 3 args (mask, initial directory, filechooser box title)
     * @param mask of the file chooser
     * @param description for each mask
     * @param initialDirectory of the opened file chooser
     * @param boxtTitle title of the opened file chooser
     */
    public static void uigetfile(String[] mask, String[] description, String initialDirectory, String boxtTitle) {
        uigetfile(mask, description, initialDirectory, boxtTitle, DEFAULT_MULTIPLE_SELECTION, OPEN_DIALOG);
    }

    /**
     * uiputfile called with 0 arg
     */
    public static void uiputfile() {
        String[] mask = new String[DEFAULT_MASK.length];
        String[] description = new String[0];
        mask = DEFAULT_MASK;

        uigetfile(mask, description, DEFAULT_INITIAL_DIRECTORY, DEFAULT_BOX_TITLE_SAVE, DEFAULT_MULTIPLE_SELECTION, SAVE_DIALOG);
    }

    /**
     * uiputfile called with 1 arg (mask)
     * @param mask of the file chooser
     * @param description for each mask
     */
    public static void uiputfile(String[] mask, String[] description) {
        uigetfile(mask, description, DEFAULT_INITIAL_DIRECTORY, DEFAULT_BOX_TITLE_SAVE, DEFAULT_MULTIPLE_SELECTION, SAVE_DIALOG);
    }

    /**
     * uiputfile called with 2 args (mask, initial directory)
     * @param mask of the file chooser
     * @param description for each mask
     * @param initialDirectory of the opened file chooser
     */
    public static void uiputfile(String[] mask, String[] description, String initialDirectory) {
        uigetfile(mask, description, initialDirectory, DEFAULT_BOX_TITLE_SAVE, DEFAULT_MULTIPLE_SELECTION, SAVE_DIALOG);
    }

    /**
     * uiputfile called with 3 args (mask, initial directory, filechooser box title)
     * @param mask of the file chooser
     * @param description for each mask
     * @param initialDirectory of the opened file chooser
     * @param boxtTitle title of the opened file chooser
     */
    public static void uiputfile(String[] mask, String[] description, String initialDirectory, String boxtTitle) {
        uigetfile(mask, description, initialDirectory, boxtTitle, DEFAULT_MULTIPLE_SELECTION, SAVE_DIALOG);
    }

    /**
     * Scilab call point for uigetfiles (OPEN dialog type).
     * uigetfile called with all args (mask, initial directory, filechooser box title, multiple selection)
     * @param mask list of type of file(s)
     * @param description of type of file(s)
     * @param initialDirectory initial directory of uigetfile
     * @param boxtTitle title of the file chooser
     * @param multipleSelection enable or not the multiple selection
     */
    public static void uigetfile(String[] mask, String[] description, String initialDirectory, String boxtTitle, boolean multipleSelection) {
        uigetfile(mask, description, initialDirectory, boxtTitle, multipleSelection, OPEN_DIALOG);
    }

    /**
     * Scilab call point for uigetfiles.
     * uigetfile called with all args (mask, initial directory, filechooser box title, multiple selection)
     * @param mask list of type of file(s)
     * @param description of type of file(s)
     * @param initialDirectory initial directory of uigetfile
     * @param boxtTitle title of the file chooser
     * @param multipleSelection enable or not the multiple selection
     * @param dialogType SAVE or OPEN a file ?
     */
    public static void uigetfile(String[] mask, String[] description, String initialDirectory,
                                 String boxtTitle, boolean multipleSelection, int dialogType) {
        Juigetfile.mask = mask;
        Juigetfile.initialDirectory = initialDirectory;
        Juigetfile.boxtTitle = boxtTitle;
        Juigetfile.multipleSelection = multipleSelection;
        Juigetfile.dialogType = dialogType;

        if (description.length != 0) {
            Juigetfile.description = description;
        } else {
            Juigetfile.description = new String[mask.length];
        }

        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                public void run() {
                    SwingScilabFileChooser ssfc = new SwingScilabFileChooser();
                    ssfc.setAcceptAllFileFilterUsed(true);
                    ssfc.addMask(Juigetfile.mask, Juigetfile.description);
                    ssfc.setInitialDirectory(Juigetfile.initialDirectory);
                    ssfc.setTitle(Juigetfile.boxtTitle);
                    ssfc.setMultipleSelection(Juigetfile.multipleSelection);
                    ssfc.setUiDialogType(Juigetfile.dialogType);
                    ssfc.displayAndWait();

                    //return the filechooser's information
                    //they are stocked into FileChooserInfos
                    FileChooserInfos.getInstance().setMultipleSelection(Juigetfile.multipleSelection);
                    ssfc.invalidate();
                }
            });
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.getCause().printStackTrace();
        }
    }


    /**
     * @return the file chooser title box
     */
    public static String getTitleBox() {
        return FileChooserInfos.getInstance().getTitleBox();
    }

    /**
     * @return the file chooser mask
     */
    public static String[] getMask() {
        return FileChooserInfos.getInstance().getMask();
    }

    /**
     * @return the file chooser mask description
     */
    public static String[] getMaskDescription() {
        return FileChooserInfos.getInstance().getMaskDescription();
    }

    /**
     * @return the file chooser initial directory
     */
    public static String getInitialDirectory() {
        return FileChooserInfos.getInstance().getInitialDirectory();
    }

    /**
     * @return the file chooser selection size
     */
    public static int getSelectionSize() {
        return FileChooserInfos.getInstance().getSelectionSize();
    }

    /**
     * @return the file chooser selection
     */
    public static String[] getSelection() {
        return FileChooserInfos.getInstance().getSelection();
    }

    /**
     * @return the file chooser selection
     */
    public static String[] getSelectionFileNames() {
        return FileChooserInfos.getInstance().getSelectionFileNames();
    }

    /**
     * @return if the file chooser allows multiple selection
     */
    public static boolean isMultipleSelection() {
        return FileChooserInfos.getInstance().isMultipleSelection();
    }

    /**
     * @return the file chooser selection path name
     */
    public static String getSelectionPathName() {
        return FileChooserInfos.getInstance().getSelectionPathName();
    }

    /**
     * @return the file chooser filter index
     */
    public static int getFilterIndex() {
        return FileChooserInfos.getInstance().getFilterIndex();
    }

    /**
     * @return the menuCallback
     */
    public static String getMenuCallback() {
        return FileChooserInfos.getInstance().getMenuCallback();
    }

    /**
     * uigetdir called without arguments
     */
    public static void uigetdir() {
        uigetdir(null, null);
    }

    /**
     * uigetdir called with 1 input argument
     * @param initialDirectory initial directory of uigetdir
     */
    public static void uigetdir(String initialDirectory) {
        uigetdir(initialDirectory, null);
    }

    /**
     * Scilab call point for uigetdir
     * uigetdir called with all args (initial directory, filechooser box title)
     * @param initialDirectory initial directory of uigetdir
     * @param title title of the file chooser
     */
    public static void uigetdir(String initialDirectory, String title) {
        Juigetfile.initialDirectory = initialDirectory;
        Juigetfile.boxtTitle = title;

        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                public void run() {
                    SwingScilabFileChooser ssfc = new SwingScilabFileChooser();
                    ssfc.setDirectorySelectionOnly();
                    if (Juigetfile.initialDirectory != null) {
                        //System.out.println("initialDir = " + Juigetfile.initialDirectory);
                        ssfc.setInitialDirectory(Juigetfile.initialDirectory);
                    }
                    if (Juigetfile.boxtTitle != null) {
                        ssfc.setTitle(Juigetfile.boxtTitle);
                    }
                    ssfc.displayAndWait();
                    ssfc.invalidate();
                }
            });

        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.getCause().printStackTrace();
        }
    }
}
