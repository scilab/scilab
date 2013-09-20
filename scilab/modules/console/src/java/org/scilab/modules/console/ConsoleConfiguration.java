/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.console;

import java.util.Set;

import org.scilab.modules.commons.xml.XConfigurationEvent;
import org.scilab.modules.commons.xml.XConfigurationListener;
import org.scilab.modules.console.utils.ScilabLaTeXViewer;

public class ConsoleConfiguration implements XConfigurationListener {

    private SciConsole sciConsole;

    public ConsoleConfiguration(SciConsole sciConsole) {
        this.sciConsole = sciConsole;
    }

    /**
     * Prevent the listener that configuration has changed
     *
     * @param e the event
     */
    public void configurationChanged(XConfigurationEvent e) {
        Conf conf = new Conf(e.getModifiedPaths());
        if (conf.changed()) {
            ConsoleOptions.invalidate(conf);
            sciConsole.configurationChanged(conf);
            ScilabLaTeXViewer.configurationChanged(conf);
        }
    }

    public static class Conf {

        public boolean latex;
        public boolean font;
        public boolean color;
        public boolean display;
        public boolean keymap;

        public Conf(Set<String> path) {
            if (path.contains("ALL")) {
                font = true;
                color = true;
                display = true;
                keymap = true;
                latex = true;
            } else {
                latex = path.contains(ConsoleOptions.LATEXPATH);
                font = path.contains(ConsoleOptions.CONSOLEFONTPATH) || path.contains(ConsoleOptions.FONTPATH);
                color = path.contains(ConsoleOptions.COLORSPATH);
                display = path.contains(ConsoleOptions.DISPLAYPATH);
                keymap = path.contains(ConsoleOptions.KEYMAPPATH);
            }
        }

        public boolean changed() {
            return font || color || display || keymap || latex;
        }
    }
}
