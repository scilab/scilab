/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro Arthur dos S. Souza
 * Copyright (C) 2013 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.datatip;


import java.util.HashMap;
import java.util.Map;

/**
 * Manages all the datatipManagerMode.
 *
 * @author Pedro Souza <bygrandao@gmail.com>
 *
 * @since 2013-25-01
 */

public class DatatipManager {

    private static Map<String, DatatipManagerMode> allDatatipManagers = new HashMap<String, DatatipManagerMode>();

    /**
     * Create an DatatipManagerMode and put it on the map.
     * @param figureUid Figure which the DatatipManagerMode belongs.
     * @return The created DatatipManagerMode instance.
     */
    public static DatatipManagerMode newDatatipManagerMode(String figureUid) {
        DatatipManagerMode tipManagerMode = new DatatipManagerMode();
        tipManagerMode.setFigure(figureUid);
        allDatatipManagers.put(figureUid, tipManagerMode);
        return tipManagerMode;
    }

    /**
     * Enables the DatatipManagerMode which belongs to the given figure uid.
     * @param figureUid Figure wich the editor belongs.
     */
    public static void setEnabled(String uid, boolean b) {
        DatatipManagerMode tipManagerMode = allDatatipManagers.get(uid);
        if (tipManagerMode != null) {
            tipManagerMode.setEnabled(b);
        }
    }


    public static boolean isEnabled(String uid) {
        DatatipManagerMode tipManagerMode = allDatatipManagers.get(uid);
        if (tipManagerMode != null) {
            return tipManagerMode.isEnabled();
        }
        return false;
    }

    public static String getSelected(String uid) {
        DatatipManagerMode tipManagerMode = allDatatipManagers.get(uid);
        if (tipManagerMode != null) {
            return tipManagerMode.getSelectedTip();
        }
        return null;
    }

    public static void setSelected(String figureUid, String tipUid) {
        DatatipManagerMode tipManagerMode = allDatatipManagers.get(figureUid);
        if (tipManagerMode != null) {
            tipManagerMode.setSelectedTip(tipUid);
        }
    }


    /**
     * Remove the DatatipManagerMode from the map.
     */
    public static void deleteDatatipManager(String uid) {
        allDatatipManagers.remove(uid);
    }

    /**
     * Get the DatatipManagerMode by figure uid.
     * @param uid The figure uid.
     * @return The DatatipManagerMode instance.
     */
    public static DatatipManagerMode getFromUid(String uid) {
        return allDatatipManagers.get(uid);
    }
}
