/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.preferences;

import java.util.Set;

import org.scilab.modules.commons.xml.XConfigurationEvent;
import org.scilab.modules.commons.xml.XConfigurationListener;

public class XcosConfiguration implements XConfigurationListener {

    public XcosConfiguration() {
    }

    /**
     * Prevent the listener that configuration has changed
     *
     * @param e
     *            the event
     */
    @Override
    public void configurationChanged(XConfigurationEvent e) {
        final Options options = new Options(e.getModifiedPaths());
        if (options.changed()) {
            XcosOptions.invalidate(options);
        }

        final KeyMap keymap = new KeyMap(e.getModifiedPaths());
        if (keymap.changed()) {
            XcosKeyMap.invalidate(keymap);

            XcosKeyMap.updateActionKeys();
        }
    }

    protected static class Options {
        public boolean preferences;
        public boolean edition;
        public boolean simulation;

        public Options(Set<String> path) {
            if (path.contains("ALL")) {
                preferences = true;
                edition = true;
                simulation = true;
            } else {
                preferences = path.contains(XcosOptions.PREFERENCES_XPATH);
                edition = path.contains(XcosOptions.EDITION_XPATH);
                simulation = path.contains(XcosOptions.SIMULATION_XPATH);
            }
        }

        public boolean changed() {
            return preferences || edition || simulation;
        }
    }

    protected static class KeyMap {
        public boolean keymap;

        public KeyMap(Set<String> path) {
            if (path.contains("ALL")) {
                keymap = true;
            } else {
                keymap = path.contains(XcosKeyMap.KEYMAP_XPATH);
            }
        }

        public boolean changed() {
            return keymap;
        }
    }
}
