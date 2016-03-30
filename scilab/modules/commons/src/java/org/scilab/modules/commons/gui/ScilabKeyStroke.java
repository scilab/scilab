/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
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

package org.scilab.modules.commons.gui;

import java.awt.GraphicsEnvironment;
import java.awt.Toolkit;
import java.awt.event.KeyEvent;

import javax.swing.KeyStroke;

import org.scilab.modules.commons.OS;

/**
 * Class to handle the keystroke and the default menu shortcut key.
 */
public final class ScilabKeyStroke {

    private static final String oSKey;

    static {
        String key = "";
        int mask;
        if (!GraphicsEnvironment.isHeadless()) {
            mask = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();
        } else {
            switch (OS.get()) {
                case MAC:
                    mask = KeyEvent.META_MASK;
                    break;
                default:
                    mask = KeyEvent.CTRL_MASK;
            }
        }

        if ((mask & KeyEvent.CTRL_MASK) != 0) {
            key += " Ctrl";
        }
        if ((mask & KeyEvent.META_MASK) != 0) {
            key += " Meta";
        }
        if ((mask & KeyEvent.SHIFT_MASK) != 0) {
            key += " Shift";
        }
        if ((mask & KeyEvent.ALT_MASK) != 0) {
            key += " Alt";
        }
        if ((mask & KeyEvent.ALT_GRAPH_MASK) != 0) {
            key += " AltGraph";
        }

        if (key.length() > 0 && key.startsWith(" ")) {
            key = key.substring(1);
        }

        oSKey = key;
    }

    /**
     * Get the OS meta key: usually Ctrl for Linux or Windows OS, and Meta (Apple) for Mac OS
     * @return the OS special meta key
     */
    public static final String getOSMetaKey() {
        return oSKey;
    }

    /**
     * @param key String representation of the key "control A" or
     * "OSSCKEY A" where OSSCKEY is replaced by the default meta key of
     * the OS.
     * @return the corresponding key
     */
    public static KeyStroke getKeyStroke(String key) {
        String str = key.replaceAll("OSSCKEY", oSKey.toLowerCase());

        return KeyStroke.getKeyStroke(str);
    }
}
