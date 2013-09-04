/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
