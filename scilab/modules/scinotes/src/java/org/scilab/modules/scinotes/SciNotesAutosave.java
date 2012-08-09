/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises -Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.util.HashSet;
import java.util.Set;

import javax.swing.Timer;

import org.scilab.modules.core.Scilab;
import org.scilab.modules.scinotes.utils.SaveFile;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

public class SciNotesAutosave implements ActionListener {

    private static SciNotesAutosave instance;
    private static Set<File> toRemove = new HashSet<File>();

    static {
        Scilab.registerFinalHook(new Runnable() {
                public void run() {
                    for (File f : toRemove) {
                        if (f.exists()) {
                            try {
                                f.delete();
                            } catch (Exception e) { }
                        }
                    }
                }
            });
    }

    private Timer timer;

    private SciNotesAutosave() {
        timer = new Timer(SciNotesOptions.getSciNotesAutosave().saveEvery * 60 * 1000, this);
        timer.setRepeats(true);
        timer.start();
    }

    public static void autosave() {
        if (SciNotesOptions.getSciNotesAutosave().autoSave) {
            startAutosave();
        } else {
            stopAutosave();
        }
    }

    static void startAutosave() {
        if (instance == null && SciNotesOptions.getSciNotesAutosave().autoSave) {
            instance = new SciNotesAutosave();
        }
    }

    static void stopAutosave() {
        if (instance != null) {
            instance.timer.stop();
            instance = null;
        }
    }

    public void actionPerformed(ActionEvent e) {
        SciNotesOptions.Autosave as = SciNotesOptions.getSciNotesAutosave();
        if (as.autoSave) {
            for (SciNotes ed : SciNotes.getSciNotesList()) {
                int n = ed.getTabPane().getTabCount();
                for (int i = 0; i < n; i++) {
                    ScilabEditorPane sep = ed.getTextPane(i);
                    if (sep.getName() != null && !sep.getName().isEmpty() && !sep.checkExternalModif() && ((ScilabDocument) sep.getDocument()).isContentModified() && ((ScilabDocument) sep.getDocument()).isContentModifiedSinceBackup()) {
                        String name = sep.getName();
                        if (as.appendFilename) {
                            name += as.appendWith;
                        } else {
                            int dotpos = name.lastIndexOf('.');
                            if (dotpos != -1) {
                                name = name.substring(0, dotpos + 1) + as.replaceWith;
                            }
                        }
                        File file = new File(name);
                        String filename = file.getName();
                        if (!as.sourceFlag) {
                            file = new File(as.singleDirectory, filename);
                        }
                        boolean identic = new File(sep.getName()).equals(file);

                        boolean success = SaveFile.doSave(sep, i, file, ed.getEditorKit(), false, true);
                        if (!success) {
                            ed.getInfoBar().setText(String.format(SciNotesMessages.AUTOSAVE_ERROR, filename));
                            return;
                        } else {
                            ((ScilabDocument) sep.getDocument()).setContentModifiedSinceBackup(false);
                            if (identic) {
                                ((ScilabDocument) sep.getDocument()).setContentModified(false);
                                sep.setLastModified(file.lastModified());
                            } else {
                                if (as.automaticDelete) {
                                    toRemove.add(file);
                                }
                            }
                        }
                    }
                }
                ed.getInfoBar().setText(SciNotesMessages.AUTOSAVE_FINISHED);
            }
        }
    }
}
