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
import java.util.HashMap;
import java.util.Map;

import javax.swing.Action;
import javax.swing.ActionMap;

import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.ui_data.filebrowser.FileUtils;
import org.scilab.modules.ui_data.filebrowser.SwingScilabTreeTable;

/**
 * Action to validate a file
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class ValidateAction extends CommonCallBack {

    private SwingScilabTreeTable table;
    private Map<String, Action> ext = new HashMap<String, Action>();

    /**
     * Default constructor
     * @param table the table associated with this action
     */
    public ValidateAction(SwingScilabTreeTable table) {
        super(null);
        this.table = table;
        ActionMap actions = table.getActionMap();
        ext.put("", actions.get("open"));
        ext.put("sce", actions.get("scinotes"));
        ext.put("sci", actions.get("scinotes"));
        ext.put("tst", actions.get("scinotes"));
        ext.put("dem", actions.get("scinotes"));
        ext.put("zcos", actions.get("xcos"));
        ext.put("xcos", actions.get("xcos"));
        ext.put("cos", actions.get("xcos"));
        ext.put("cosf", actions.get("xcos"));
        ext.put("bin", actions.get("load"));
        ext.put("scg", actions.get("load"));
        ext.put("sav", actions.get("load"));
        ext.put("sod", actions.get("load"));
        ext.put("graph", actions.get("graph"));
        ext.put("mat", actions.get("mat"));
    }

    /**
     * {@inheritDoc}
     */
    public void callBack() {
        File[] files = table.getSelectedFiles();
        if (files[0].isDirectory() && files[0].canRead()) {
            table.getComboBox().setBaseDirAndValidate(files[0].getAbsolutePath());
            return;
        }

        String extension = FileUtils.getFileExtension(files[0]).toLowerCase();
        CommonCallBack c = (CommonCallBack) ext.get(extension);
        if (c != null) {
            c.callBack();
        } else {
            ((CommonCallBack) ext.get("")).callBack();
        }
    }
}
