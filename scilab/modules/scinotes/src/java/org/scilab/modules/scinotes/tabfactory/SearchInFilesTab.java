/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes.tabfactory;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.SearchFile;

/**
 * Class to create SciNotes instances
 * @author Calixte DENIZET
 */
public class SearchInFilesTab {

    /**
     * @param uuid the uuid to restore
     * @return a new SciNotes instance
     */
    public static SearchFile getSearchInFilesTabInstance(final SciNotes editor, String uuid) {
	final SearchFile sf;
        if (uuid != null) {
            sf = new SearchFile(editor, uuid);
        } else {
            sf = new SearchFile(editor);
	    ScilabTabFactory.getInstance().addToCache(sf);
        }

        ClosingOperationsManager.registerClosingOperation((SwingScilabTab) sf, new ClosingOperationsManager.ClosingOperation() {

                public boolean canClose() {
                    return true;
                }

                public void destroy() {
                    sf.closeSearchInFiles();
                }

                public String askForClosing() {
                    return null;
                }
            });

        WindowsConfigurationManager.registerEndedRestoration((SwingScilabTab) sf, new WindowsConfigurationManager.EndedRestoration() {

                public void finish() {
                    sf.changeToolBar();
                }
            });

        ClosingOperationsManager.addDependency((SwingScilabTab) editor, (SwingScilabTab) sf);

        return sf;
    }
}
