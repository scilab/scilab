/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises -Calixte DENIZET
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

package org.scilab.modules.scinotes;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashSet;
import java.util.Set;

import javax.swing.Timer;

import org.scilab.modules.core.Scilab;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.scinotes.utils.SaveFile;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

public class SciNotesAutosave implements ActionListener {

    private static SciNotesAutosave instance;
    private static Set<File> toRemove = new HashSet<File>();
    private boolean isChecking = false;
    private Object lock = new Object();

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

    private boolean isChecking() {
        synchronized (lock) {
            return isChecking;
        }
    }

    private void isChecking(boolean b) {
        synchronized (lock) {
            isChecking = b ;
        }
    }

    private boolean checkSaveDir() {
        isChecking(true);
        SciNotesOptions.Autosave as = SciNotesOptions.getSciNotesAutosave();
        if (!as.singleDirectory.isEmpty()) {
            File dir = new File(as.singleDirectory);
            if (!dir.exists()) {
                if (AnswerOption.YES_OPTION == ScilabModalDialog.show(null, String.format(SciNotesMessages.NO_EXISTING_DIR, as.singleDirectory), SciNotesMessages.AUTOSAVE, IconType.QUESTION_ICON, ButtonType.YES_NO_CANCEL)) {
                    if (dir.mkdir()) {
                        isChecking(false);

                        return true;
                    }
                    ScilabModalDialog.show(null, String.format(SciNotesMessages.UNABLE_TO_CREATE_DIR, as.singleDirectory), SciNotesMessages.SCINOTES_ERROR, IconType.ERROR_ICON);
                }

                isChecking(false);

                return false;
            }

            if (!dir.isDirectory()) {
                ScilabModalDialog.show(null, String.format(SciNotesMessages.NOT_A_DIR, as.singleDirectory), SciNotesMessages.SCINOTES_ERROR, IconType.ERROR_ICON);
                isChecking(false);

                return false;
            }

            // we check if a file can be created in this directory
            try {
                File.createTempFile("autosave_test", null, dir).delete();
            } catch (Exception e) {
                System.err.println(e);
                ScilabModalDialog.show(null, String.format(SciNotesMessages.NOT_WRITABLE_DIR, as.singleDirectory), SciNotesMessages.SCINOTES_ERROR, IconType.ERROR_ICON);
                isChecking(false);

                return false;
            }
        }

        isChecking(false);

        return true;
    }

    public static File getBackupFile(String name) {
        SciNotesOptions.Autosave as = SciNotesOptions.getSciNotesAutosave();
        DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd_HHmmss");
        if (as.appendFilename) {
            name += as.appendWith.replaceAll("%date", dateFormat.format(new Date()));
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

        boolean identic = new File(name).equals(file);

        if (identic) {
            return null;
        }

        return file;
    }

    public void actionPerformed(ActionEvent e) {
        SciNotesOptions.Autosave as = SciNotesOptions.getSciNotesAutosave();
        if (!isChecking() && as.autoSave && checkSaveDir()) {
            boolean saved = false;
            DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd_HHmmss");
            for (SciNotes ed : SciNotes.getSciNotesList()) {
                int n = ed.getTabPane().getTabCount();
                for (int i = 0; i < n; i++) {
                    ScilabEditorPane sep = ed.getTextPane(i);
                    if (sep != null && sep.getName() != null && !sep.getName().isEmpty() && !sep.checkExternalModif() && ((ScilabDocument) sep.getDocument()).isContentModified() && ((ScilabDocument) sep.getDocument()).isContentModifiedSinceBackup()) {
                        String name = sep.getName();
                        File file = getBackupFile(name);
                        boolean identic = false;
                        if (file == null) {
                            identic = true;
                            file = new File(name);
                        }

                        boolean success = SaveFile.doSave(sep, i, file, ed.getEditorKit(), false, true, false);
                        String filename = file.getName();
                        if (!saved) {
                            saved = true;
                        }
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
                if (saved) {
                    ed.getInfoBar().setText(SciNotesMessages.AUTOSAVE_FINISHED);
                }
            }
        }
    }
}
