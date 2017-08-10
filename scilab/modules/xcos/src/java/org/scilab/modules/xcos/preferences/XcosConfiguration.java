/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
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
