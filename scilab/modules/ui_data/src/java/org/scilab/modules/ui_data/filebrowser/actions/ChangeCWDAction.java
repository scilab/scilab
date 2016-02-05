/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
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

package org.scilab.modules.ui_data.filebrowser.actions;

import java.io.File;

import javax.swing.JButton;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.ui_data.FileBrowser;
import org.scilab.modules.ui_data.filebrowser.FileUtils;
import org.scilab.modules.ui_data.filebrowser.SwingScilabTreeTable;
import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 * Change cwd action
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class ChangeCWDAction extends CommonCallBack {

    private SwingScilabTreeTable table;

    /**
     * Default constructor
     * @param table the associated table
     */
    public ChangeCWDAction(SwingScilabTreeTable table) {
        super(null);
        this.table = table;
    }

    /**
     * {@inheritDoc}
     */
    public void callBack() {
        changeDir(table.getComboBox().getBaseDir());
    }

    /**
     * @return the button used to set the cwd in using a JFileChooser
     */
    public JButton createButton() {
        JButton button = new JButton();
        button.setToolTipText(UiDataMessages.SELECTDIR);
        button.addActionListener(new CommonCallBack(null) {
            public void callBack() {
                SwingScilabFileChooser filechooser = new SwingScilabFileChooser();
                filechooser.setCurrentDirectory(new File(table.getComboBox().getBaseDir()));
                filechooser.setTitle(UiDataMessages.SELECTADIR);
                filechooser.setMultipleSelection(false);
                filechooser.setDirectorySelectionOnly();
                filechooser.displayAndWait();
                String[] paths = filechooser.getSelection();
                if (paths != null && paths.length != 0) {
                    changeDir(paths[0]);
                }
            }
        });
        button.setIcon(FileUtils.getClosedDirIcon());
        ScilabLAF.setDefaultProperties(button);

        return button;
    }

    /**
     * @return the button used to jump to the parent directory
     */
    public JButton createParentDirButton() {
        JButton button = new JButton();
        button.setToolTipText(UiDataMessages.PARENTDIR);
        button.addActionListener(new CommonCallBack(null) {
            public void callBack() {
                File f = new File(table.getComboBox().getBaseDir());
                if (f.exists()) {
                    File parent = f.getParentFile();
                    if (parent != null && parent.exists() && parent.canRead()) {
                        changeDir(parent.getAbsolutePath());
                    }
                }
            }
        });
        button.setIcon(FileUtils.getUpDirIcon());

        return button;
    }

    /**
     * Change the dir
     * @param path the new path
     */
    private void changeDir(String path) {
        File f = new File(path);
        if (f.exists() && f.isDirectory() && f.canRead()) {
            InterpreterManagement.requestScilabExec("chdir('" + path + "');");
            FileBrowser.setBaseDir(path);
        }
    }
}
