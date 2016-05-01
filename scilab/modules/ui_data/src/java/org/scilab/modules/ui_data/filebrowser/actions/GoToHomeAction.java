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
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.ui_data.FileBrowser;
import org.scilab.modules.ui_data.filebrowser.FileUtils;
import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 * Action to go to user-home
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class GoToHomeAction extends CommonCallBack {

    /**
     * {@inheritDoc}
     */
    public GoToHomeAction() {
        super(null);
    }

    /**
     * {@inheritDoc}
     */
    public void callBack() {
        File f = new File(ScilabConstants.USERHOME);
        if (f.exists() && f.isDirectory() && f.canRead()) {
            InterpreterManagement.requestScilabExec("chdir('" + f.getAbsolutePath() + "')");
            FileBrowser.setBaseDir(f.getAbsolutePath());
        }
    }

    /**
     * @return the corresponding button
     */
    public static JButton createButton() {
        JButton button = new JButton();
        ScilabLAF.setDefaultProperties(button);
        button.addActionListener(new GoToHomeAction());
        button.setToolTipText(UiDataMessages.USERHOME);
        button.setIcon(FileUtils.getClosedUserHomeIcon());

        return button;
    }
}
