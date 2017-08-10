/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.modules.history_manager;

import org.w3c.dom.Document;

import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.commons.xml.XConfigurationEvent;
import org.scilab.modules.commons.xml.XConfigurationListener;
import static org.scilab.modules.commons.xml.XConfiguration.XConfAttribute;

public class HistoryPrefs implements XConfigurationListener {

    private static final String HISTORY_PATH = "//command-history/body/history-settings";
    private static final HistoryPrefs instance = new HistoryPrefs();

    private HistoryPrefs() { }

    public static HistoryPrefs getInstance() {
        return instance;
    }

    public void configurationChanged(XConfigurationEvent e) {
        if (e.getModifiedPaths().contains(HISTORY_PATH) || e.getModifiedPaths().contains("ALL")) {
            Document doc = XConfiguration.getXConfigurationDocument();
            HistoryOption opt = XConfiguration.get(HistoryOption.class, doc, HISTORY_PATH)[0];
            if (opt.enable) {
                HistoryManagement.InitializeHistoryManager();
                HistoryManagement.setFilenameScilabHistory(opt.historyFile);
                HistoryManagement.setSizeMaxScilabHistory(opt.historyLines);
            } else {
                HistoryManagement.TerminateHistoryManager();
            }
        }
    }

    @XConfAttribute
    private static class HistoryOption {

        public String historyFile;
        public int historyLines;
        public boolean enable;

        private HistoryOption() { }

        @XConfAttribute(attributes = {"history-file", "history-lines", "enable"})
        private void set(String historyFile, int historyLines, boolean enable) {
            this.historyFile = historyFile;
            this.historyLines = historyLines;
            this.enable = enable;
        }
    }
}