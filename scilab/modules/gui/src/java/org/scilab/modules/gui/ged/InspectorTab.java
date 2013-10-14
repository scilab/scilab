/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged;

import java.util.List;
import java.util.ListIterator;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.utils.ClosingOperationsManager;

/**
* Class to create Inspector instances
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class InspectorTab {

    public static SwingScilabTab getInspectorInstance(Integer objectID) {
        final SwingScilabTab inspector = Inspector.createInspectorTab(objectID);
        ScilabTabFactory.getInstance().addToCache(inspector);

        ClosingOperationsManager.registerClosingOperation(inspector,
        new ClosingOperationsManager.ClosingOperation() {

            //@Override
            public int canClose() {
                return 1;
            }

            //@Override
            public void destroy() {
                Inspector.closeInspector();
            }

            //@Override
            public String askForClosing(final List<SwingScilabTab> list) {
                return null;
            }

            //@Override
            public void updateDependencies(List<SwingScilabTab> list,
            ListIterator<SwingScilabTab> it) {
            }
        });

        ClosingOperationsManager.addDependencyWithRoot(inspector);

        return inspector;
    }
}
