/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
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

package org.scilab.modules.gui.bridge.filechooser;

import java.awt.Component;
import java.awt.KeyboardFocusManager;
import java.io.File;
import java.util.StringTokenizer;

import javax.swing.ImageIcon;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;

import org.scilab.modules.gui.filechooser.FileChooserInfos;
import org.scilab.modules.gui.filechooser.SimpleFileChooser;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.gui.utils.SciFileFilter;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.localization.Messages;

/**
 * Swing implementation of a Scilab File ChooserS
 * @author Vincent COUVERT
 * @author Sylvestre KOUMAR
 */
public class SwingScilabFileChooser extends JFileChooser implements SimpleFileChooser {

    private static final long serialVersionUID = 1L;

    private String[] selection; // Path + filenames
    private String selectionPath; // Path
    private String[] selectionFileNames; // Filenames
    private int selectionSize;
    private int filterIndex;
    private int maskSize;
    private int dialogType;

    private String[] maskDescription;
    private JFrame parent;

    /**
     * Default constructor
     */
    public SwingScilabFileChooser() {
        super();

        //System.out.println("[Constructor] SwingScilabFileChooser");
        /** Bug 3231 fixed: do not explore all zip files on desktop under Windows */
        //putClientProperty("FileChooser.useShellFolder", Boolean.FALSE);
        /**
         * Bug 4187 fixed: uigetdir() opens on "Desktop" and not on "Computer" on windows
         * No need to use 'putClientProperty' anymore (bug 3231)
         */
        /* Bug 5111 : The Current directory have to be set before */
        super.setCurrentDirectory(new File(ConfigManager.getLastOpenedDirectory()));
    }

    /**
     * Set the title of the file chooser
     * @param title the title to set
     */
    @Override
    public void setTitle(String title) {
        super.setDialogTitle(title);
    }

    /**
     * Set the mask & the mask description for the filechooser
     * @param mask the mask to set
     * @param fileMaskDescription the maskDescription to set
     */
    public void addMask(String[] mask, String[] fileMaskDescription) {

        //size of the mask list
        maskSize = mask.length;

        //If the mask description is empty we allocate description
        //according to the extensions given
        if (fileMaskDescription == null || fileMaskDescription.length == 0) {
            for (int i = 0; i < mask.length; i++) {
                super.addChoosableFileFilter(new SciFileFilter(mask[i], null, i/*, maskSize*/));
            }
        } else {
            //If the mask description is filled
            //we use those descriptions given by the user
            this.maskDescription = fileMaskDescription;
            for (int i = 0; i < mask.length; i++) {
                super.addChoosableFileFilter(new SciFileFilter(mask[i], maskDescription[i], i/*, maskSize*/));
            }
        }
    }

    /**
     * Set the initial directory used for file search
     * @param path the default path
     */
    @Override
    public void setInitialDirectory(String path) {
        // When empty string given
        if (path.length() == 0) {
            return;
        }
        // Replace beginning of the path if is an environment variable
        String newPath = path;
        StringTokenizer tok = new StringTokenizer(path, File.separator);
        if (tok.hasMoreTokens()) {
            /* It is possible that we don't have any more token here when
                                          Scilab is started from / for example */
            String firstToken = tok.nextToken();
            if (firstToken != null && System.getenv(firstToken) != null)  {
                newPath = newPath.replaceFirst(firstToken, System.getenv(firstToken));
            }
        }
        super.setCurrentDirectory(new File(newPath));
    }

    /**
     * Set the parent frame
     * @param parent the parent frame
     */
    public void setParentFrame(JFrame parent) {
        this.parent = parent;
    }

    /**
     * Display this chooser and wait for user selection
     */
    @Override
    public void displayAndWait() {
        JFrame parentFrame = null;
        if (parent == null) {
            Component focused = KeyboardFocusManager.getCurrentKeyboardFocusManager().getFocusOwner();
            if (focused != null) {
                parentFrame = (JFrame) SwingUtilities.getAncestorOfClass(JFrame.class, focused);
            }
            if (parentFrame == null) {
                parentFrame = new JFrame();
            }
        } else {
            parentFrame = parent;
        }

        int returnValue = 0;
        if (maskSize > 0) {
            setFileFilter(getChoosableFileFilters()[maskSize]);
        }
        if (this.dialogType == JFileChooser.SAVE_DIALOG) {
            returnValue = this.showSaveDialog(parentFrame);
        } else {
            returnValue = this.showOpenDialog(parentFrame);
        }

        //User validate the uigetfile
        if (returnValue == APPROVE_OPTION) {
            if (this.isMultiSelectionEnabled()) {
                File[] files = this.getSelectedFiles();
                selection = new String[files.length];
                selectionFileNames = new String[files.length];
                selectionSize = files.length;
                for (int i = 0; i < files.length; i++) {
                    selection[i] = files[i].getAbsolutePath();
                    selectionPath = files[i].getParentFile().getPath();
                    selectionFileNames[i] = files[i].getName();
                }
            } else {
                File file = this.getSelectedFile();

                if (this.dialogType == JFileChooser.SAVE_DIALOG) {
                    //Test if there is a file with the same name
                    if (file.exists()) {
                        int actionDialog = JOptionPane.showConfirmDialog(this,
                                           Messages.gettext("Replace existing file?"),
                                           Messages.gettext("File already exists"),
                                           JOptionPane.YES_NO_OPTION);

                        if (actionDialog != JOptionPane.YES_OPTION) {
                            // Same as cancel case
                            selection = new String[1];
                            selection[0] = "";
                            selectionSize = 0;
                            selectionPath = "";
                            selectionFileNames = new String[1];
                            selectionFileNames[0] = "";
                            //set the filter index of the JFileChooser at 0 if "cancel" button was selected
                            filterIndex = 0;

                            //return the filechooser's information
                            //they are stocked into FileChooserInfos
                            FileChooserInfos.getInstance().setSelection(selection);
                            FileChooserInfos.getInstance().setSelectionPathName(selectionPath);
                            FileChooserInfos.getInstance().setSelectionFileNames(selectionFileNames);
                            FileChooserInfos.getInstance().setSelectionSize(selectionSize);
                            FileChooserInfos.getInstance().setFilterIndex(filterIndex);
                            return;
                        }
                    }
                }

                selection = new String[1];
                selection[0] = file.getAbsolutePath();
                if (getFileSelectionMode() == DIRECTORIES_ONLY) {
                    selectionPath = file.getPath();
                } else {
                    selectionPath = file.getParentFile().getPath();
                }
                selectionFileNames = new String[1];
                selectionFileNames[0] = file.getName();
                selectionSize = 1;
            }

            //return the filechooser's information
            //they are stocked into FileChooserInfos
            FileChooserInfos.getInstance().setSelection(selection);
            FileChooserInfos.getInstance().setSelectionPathName(selectionPath);
            FileChooserInfos.getInstance().setSelectionFileNames(selectionFileNames);
            FileChooserInfos.getInstance().setSelectionSize(selectionSize);

            //set the filter index at the last index
            //of the list box if the mask "All files" is selected
            javax.swing.filechooser.FileFilter allFilesSelected = getFileFilter();
            if (allFilesSelected.getDescription().equals("All Files")) {
                FileChooserInfos.getInstance().setFilterIndex(maskSize + 1);
            }
            //TODO
            ConfigManager.saveLastOpenedDirectory(selectionPath);
            //User cancel the uigetfile
        } else {
            selection = new String[1];
            selection[0] = "";
            selectionSize = 0;
            selectionPath = "";
            selectionFileNames = new String[1];
            selectionFileNames[0] = "";
            //set the filter index of the JFileChooser at 0 if "cancel" button was selected
            filterIndex = 0;

            //return the filechooser's information
            //they are stocked into FileChooserInfos
            FileChooserInfos.getInstance().setSelection(selection);
            FileChooserInfos.getInstance().setSelectionPathName(selectionPath);
            FileChooserInfos.getInstance().setSelectionFileNames(selectionFileNames);
            FileChooserInfos.getInstance().setSelectionSize(selectionSize);
            FileChooserInfos.getInstance().setFilterIndex(filterIndex);
        }
    }

    /**
     * Get the number of files selected
     * @return the number of files selected
     */
    @Override
    public int getSelectionSize() {
        return selectionSize;
    }

    /**
     * Get the names of selected files
     * @return the names of selected files
     */
    @Override
    public String[] getSelection() {
        return selection;
    }

    /**
     * Set the flag indicating that we want only select directories
     */
    @Override
    public void setDirectorySelectionOnly() {
        setFileSelectionMode(DIRECTORIES_ONLY);
    }

    /**
     * Set the flag indicating that we can select multiple files
     * @param multipleSelection is enable or not
     */
    @Override
    public void setMultipleSelection(boolean multipleSelection) {
        setMultiSelectionEnabled(multipleSelection);
    }

    /**
     * Get the path of selected files
     * @return selectionPath selected file(s) path
     */
    @Override
    public String getSelectionPathName() {
        return selectionPath;
    }

    /**
     * Get the names of selected files
     * @return selectionFileNnames selected file(s) path
     */
    @Override
    public String[] getSelectionFileNames() {
        return selectionFileNames;
    }

    /**
     * Set the dialog type (save or open a file ?)
     * @param dialogType the dialog type
     */
    @Override
    public void setUiDialogType(int dialogType) {
        this.dialogType = dialogType;
    }
}
