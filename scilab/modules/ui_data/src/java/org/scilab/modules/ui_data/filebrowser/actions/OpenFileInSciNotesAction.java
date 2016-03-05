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
import java.lang.reflect.Method;

import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.ui_data.filebrowser.SwingScilabTreeTable;

/**
 * Action to open the file in SciNotes
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class OpenFileInSciNotesAction extends CommonCallBack {

    private SwingScilabTreeTable table;

    /**
     * Default constructor
     * @param table the table associated with this action
     */
    public OpenFileInSciNotesAction(SwingScilabTreeTable table) {
        super(null);
        this.table = table;
    }

    /**
     * {@inheritDoc}
     */
    public void callBack() {
        Class scinotesClass = null;
        Method scinotes = null;
        try {
            scinotesClass = Class.forName("org.scilab.modules.scinotes.SciNotes");
            scinotes = scinotesClass.getMethod("scinotes", new Class[] {String.class});
        } catch (ClassNotFoundException e) {
            System.err.println("Could not find SciNotes class");
            e.printStackTrace();
        } catch (SecurityException e) {
            System.err.println("Security error: Could not access to SciNotes class");
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            System.err.println("Could not access to scinotesWithText method from object SciNotes");
            e.printStackTrace();
        }
        if (scinotesClass != null && scinotes != null) {
            final Method scin = scinotes;
            String[] paths = table.getSelectedPaths();
            for (final String path : paths) {
                File f = new File(path);
                if (f.exists() && f.canRead() && f.isFile()) {
                    Thread t = new Thread(new Runnable() {
                        public void run() {
                            try {
                                scin.invoke(null, new Object[] {path});
                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        }
                    });
                    try {
                        t.start();
                    } catch (Exception e) { }
                }
            }
        }
    }
}
