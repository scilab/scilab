/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
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

package org.scilab.modules.gui.editor;

import java.util.HashMap;
import java.util.Map;

import org.scilab.modules.graphic_objects.ScilabNativeView;

/**
 * Manages all the editors.
 *
 * @author Caio Souza <caioc2bolado@gmail.com>
 * @author Pedro Souza <bygrandao@gmail.com>
 *
 * @since 2012-067-12
 */

public class EditorManager {

    private static Map<Integer, Editor> allEditors = new HashMap<Integer, Editor>();

    /**
     * Create an editor and put it on the map.
     * @param figureUid Figure which the editor belongs.
     * @return The created editor instance.
     */
    public static Editor newEditor(Integer figureUid) {
        Editor editor = new Editor();
        editor.setFigure(figureUid);
        allEditors.put(figureUid, editor);
        return editor;
    }

    public static void start(int figureId) {
        enableModify(ScilabNativeView.ScilabNativeView__getFigureFromIndex(figureId));
    }

    public static void stop(int figureId) {
        disableModify(ScilabNativeView.ScilabNativeView__getFigureFromIndex(figureId));
    }

    /**
     * Enables the editor which belongs to the given figure uid.
     * @param figureUid Figure wich the editor belongs.
     */
    public static void enableModify(int uid) {
        Editor editor = allEditors.get(uid);
        if (editor != null) {
            editor.setDataModifyEnabled(true);
        }
    }

    /**
     * Disables the editor which belongs to the given figure uid.
     * @param figureUid Figure wich the editor belongs.
     */
    public static void disableModify(int uid) {
        Editor editor = allEditors.get(uid);
        if (editor != null) {
            editor.setDataModifyEnabled(false);
        }
    }

    /**
     * Checks if the editor which belongs to the given figure uid is enabled.
     * @return True if the editor is enableb, false otherwise.
     */
    public static boolean isModifyEnabled(int uid) {
        Editor editor = allEditors.get(uid);
        if (editor != null) {
            return editor.isDataModifyEnabled();
        }
        return false;
    }

    /**
     * Remove the editor from the map.
     */
    public static void deleteEditor(int uid) {
        allEditors.remove(uid);
    }

    /**
     * Get the editor by figure uid.
     * @param uid The figure uid.
     * @return The editor instance.
     */
    public static Editor getFromUid(int uid) {
        return allEditors.get(uid);
    }
}
