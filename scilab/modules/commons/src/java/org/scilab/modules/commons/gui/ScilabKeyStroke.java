/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.commons.gui;

import java.awt.event.KeyEvent;
import java.awt.Toolkit;

import javax.swing.KeyStroke;

/**
 * Class to handle the keystroke and the default menu shortcut key.
 */
public final class ScilabKeyStroke {

    private static final String oSKey;

    static {
        String key = "";
        int mask = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();
        if ((mask & KeyEvent.CTRL_MASK) != 0) {
            key += " control";
        }
        if ((mask & KeyEvent.META_MASK) != 0) {
            key += " meta";
        }
        if ((mask & KeyEvent.SHIFT_MASK) != 0) {
            key += " shift";
        }
        if ((mask & KeyEvent.ALT_MASK) != 0) {
            key += " alt";
        }
        if ((mask & KeyEvent.ALT_GRAPH_MASK) != 0) {
            key += " altGraph";
        }

        if (key.length() > 0 && key.startsWith(" ")) {
            key = key.substring(1);
        }

        oSKey = key;
    }

    /**
     * @param key String representation of the key "control A" or
     * "OSSCKEY A" where OSSCKEY is replaced by the default meta key of
     * the OS.
     * @return the corresponding key
     */
    public static KeyStroke getKeyStroke(String key) {
        String str = key.replaceAll("OSSCKEY", oSKey);

        return KeyStroke.getKeyStroke(str);
    }
}
