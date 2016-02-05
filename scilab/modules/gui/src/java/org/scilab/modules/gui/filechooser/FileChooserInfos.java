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


/**
 * This class is to manage FileChooser information
 * @author Sylvestre KOUMAR
 *
 */
public final class FileChooserInfos {

    private static FileChooserInfos self;

    private String titleBox;
    private String[] mask;
    private String[] maskDescription;
    private String initialDirectory;
    private int selectionSize;
    private String[] selection;
    private boolean multipleSelection;
    private String selectionPathName;
    private String[] selectionFileNames;
    private int filterIndex;
    private String menuCallback;


    /**
     * SINGLETON
     */
    private FileChooserInfos() {
        init();
    }

    /**
     * Initialise FileChooserInfos attributes.
     */
    public void init() {

        this.titleBox = null;
        this.mask = null;
        this.maskDescription = null;
        this.initialDirectory = null;
        this.selectionSize = 0;
        this.selection = null;
        this.multipleSelection = true;
        this.selectionPathName = null;
        this.filterIndex = 0;
        this.menuCallback = "void";
    }

    /**
     * Singleton.
     * @return the unique FileChooserInfos
     */
    public static FileChooserInfos getInstance() {
        if (self == null) {
            self = new FileChooserInfos();
        }
        return self;
    }

    /**
     * @return the titleBox
     */
    public String getTitleBox() {
        return titleBox;
    }

    /**
     * @param titleBox the titleBox to set
     */
    public void setTitleBox(String titleBox) {
        this.titleBox = titleBox;
    }

    /**
     * @return the mask
     */
    public String[] getMask() {
        return mask;
    }

    /**
     * @param mask the mask to set
     */
    public void setMask(String[] mask) {
        this.mask = mask;
    }

    /**
     * @return the maskDescription
     */
    public String[] getMaskDescription() {
        return maskDescription;
    }

    /**
     * @param maskDescription the maskDescription to set
     */
    public void setMaskDescription(String[] maskDescription) {
        this.maskDescription = maskDescription;
    }

    /**
     * @return the initialDirectory
     */
    public String getInitialDirectory() {
        return initialDirectory;
    }

    /**
     * @param initialDirectory the initialDirectory to set
     */
    public void setInitialDirectory(String initialDirectory) {
        this.initialDirectory = initialDirectory;
    }

    /**
     * @return the selectionSize
     */
    public int getSelectionSize() {
        return selectionSize;
    }

    /**
     * @param selectionSize the selectionSize to set
     */
    public void setSelectionSize(int selectionSize) {
        this.selectionSize = selectionSize;
    }

    /**
     * @return the selection
     */
    public String[] getSelection() {
        return selection;
    }

    /**
     * @param selection the selection to set
     */
    public void setSelection(String[] selection) {
        this.selection = selection;
    }

    /**
     * @param selection the selection to set
     */
    public void setSelectionFileNames(String[] selectionFileNames) {
        this.selectionFileNames = selectionFileNames;
    }

    /**
     * @return the multipleSelection
     */
    public boolean isMultipleSelection() {
        return multipleSelection;
    }

    /**
     * @param multipleSelection the multipleSelection to set
     */
    public void setMultipleSelection(boolean multipleSelection) {
        this.multipleSelection = multipleSelection;
    }

    /**
     * @return the selectionPathName
     */
    public String getSelectionPathName() {
        return selectionPathName;
    }

    /**
     * Get the names of selected files
     * @return selectionFileNnames selected file(s) path
     */
    public String[] getSelectionFileNames() {
        return this.selectionFileNames;
    }

    /**
     * @param selectionPathName the selectionPathName to set
     */
    public void setSelectionPathName(String selectionPathName) {
        this.selectionPathName = selectionPathName;
    }

    /**
     * @return the filterIndex
     */
    public int getFilterIndex() {
        return filterIndex;
    }

    /**
     * @param filterIndex the filterIndex to set
     */
    public void setFilterIndex(int filterIndex) {
        this.filterIndex = filterIndex;
    }

    /**
     * @return the menuCallback
     */
    public String getMenuCallback() {
        return menuCallback;
    }

    /**
     * @param menuCallback the menuCallback to set
     */
    public void setMenuCallback(String menuCallback) {
        this.menuCallback = menuCallback;
    }
}
