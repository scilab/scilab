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

package org.scilab.modules.history_manager;

import org.w3c.dom.Document;

import org.scilab.modules.commons.xml.ScilabXMLUtilities;
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