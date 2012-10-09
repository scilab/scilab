/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes;

import java.util.Set;

import org.scilab.modules.commons.xml.XConfigurationEvent;
import org.scilab.modules.commons.xml.XConfigurationListener;

public class SciNotesConfiguration implements XConfigurationListener {

    public SciNotesConfiguration() { }

    /**
     * Prevent the listener that configuration has changed
     *
     * @param e the event
     */
    public void configurationChanged(XConfigurationEvent e) {
        Conf conf = new Conf(e.getModifiedPaths());
        if (conf.changed()) {
            SciNotesOptions.invalidate(conf);
            SciNotes.configurationChanged(conf);
        }
    }

    public static class Conf {

        public boolean preferences;
        public boolean display;
        public boolean autosave;
        public boolean colors;
        public boolean systemfont;
        public boolean font;
        public boolean keymap;
        public boolean header;

        public Conf(Set<String> path) {
            if (path.contains("ALL")) {
                preferences = true;
                display = true;
                autosave = true;
                colors = true;
                systemfont = true;
                font = true;
                keymap = true;
                header = true;
            } else {
                preferences = path.contains(SciNotesOptions.PREFERENCESPATH);
                display = path.contains(SciNotesOptions.DISPLAYPATH);
                autosave = path.contains(SciNotesOptions.AUTOSAVEPATH);
                colors = path.contains(ScilabContext.COLORSPATH);
                systemfont = path.contains(ScilabContext.SYSTEMFONTPATH);
                font = path.contains(ScilabContext.XCONFFONTPATH);
                keymap = path.contains(SciNotesOptions.KEYMAPPATH);
                header = path.contains(SciNotesOptions.HEADERPATH);
            }
        }

        public boolean changed() {
            return preferences || display || autosave || colors || systemfont || font || keymap || header;
        }
    }
}
