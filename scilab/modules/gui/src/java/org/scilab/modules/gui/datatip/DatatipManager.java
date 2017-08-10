/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro Arthur dos S. Souza
 * Copyright (C) 2013 - Caio Lucas dos S. Souza
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

package org.scilab.modules.gui.datatip;

import java.util.HashMap;
import java.util.Map;

import org.scilab.modules.graphic_objects.ScilabNativeView;

/**
 * Manages all the datatipManagerMode.
 *
 * @author Pedro Souza <bygrandao@gmail.com>
 *
 * @since 2013-25-01
 */

public class DatatipManager {

    private static Map<Integer, DatatipManagerMode> allDatatipManagers = new HashMap<Integer, DatatipManagerMode>();

    /**
     * Create an DatatipManagerMode and put it on the map.
     * @param figureUid Figure which the DatatipManagerMode belongs.
     * @return The created DatatipManagerMode instance.
     */
    public static DatatipManagerMode newDatatipManagerMode(int figureUid) {
        DatatipManagerMode tipManagerMode = new DatatipManagerMode();
        tipManagerMode.setFigure(figureUid);
        allDatatipManagers.put(figureUid, tipManagerMode);
        return tipManagerMode;
    }

    public static void start(int figureId) {
        setEnabled(ScilabNativeView.ScilabNativeView__getFigureFromIndex(figureId), true);
    }

    public static void stop(int figureId) {
        setEnabled(ScilabNativeView.ScilabNativeView__getFigureFromIndex(figureId), false);
    }

    /**
     * Enables the DatatipManagerMode which belongs to the given figure uid.
     * @param figureUid Figure wich the editor belongs.
     */
    public static void setEnabled(int uid, boolean b) {
        DatatipManagerMode tipManagerMode = allDatatipManagers.get(uid);
        if (tipManagerMode != null) {
            tipManagerMode.setEnabled(b);
        }
    }


    public static boolean isEnabled(int uid) {
        DatatipManagerMode tipManagerMode = allDatatipManagers.get(uid);
        if (tipManagerMode != null) {
            return tipManagerMode.isEnabled();
        }
        return false;
    }

    public static int getSelected(int uid) {
        DatatipManagerMode tipManagerMode = allDatatipManagers.get(uid);
        if (tipManagerMode != null) {
            return tipManagerMode.getSelectedTip();
        }
        return 0;
    }

    public static void setSelected(int figureUid, int tipUid) {
        DatatipManagerMode tipManagerMode = allDatatipManagers.get(figureUid);
        if (tipManagerMode != null) {
            tipManagerMode.setSelectedTip(tipUid);
        }
    }

    /**
     * Remove the DatatipManagerMode from the map.
     */
    public static void deleteDatatipManager(int uid) {
        allDatatipManagers.remove(uid);
    }

    /**
     * Get the DatatipManagerMode by figure uid.
     * @param uid The figure uid.
     * @return The DatatipManagerMode instance.
     */
    public static DatatipManagerMode getFromUid(int uid) {
        return allDatatipManagers.get(uid);
    }
}
