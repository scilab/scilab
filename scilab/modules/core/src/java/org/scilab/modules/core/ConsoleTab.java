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

package org.scilab.modules.core;

import java.util.List;
import java.util.ListIterator;
import java.util.UUID;

import javax.swing.JTextArea;

import org.scilab.modules.gui.ScilabTermination;
import org.scilab.modules.gui.bridge.CallScilabBridge;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.jvm.LoadClassPath;
import org.scilab.modules.localization.Messages;

/**
 *
 * @author Calixte DENIZET
 */
public class ConsoleTab {

    private static final String CLASS_NOT_FOUND = "Could not find class: ";
    private static final String SEE_DEFAULT_PATHS = "See SCI/etc/classpath.xml for default paths.";
    private static final String NOCONSOLE = Messages.gettext("No available console !\nPlease use STD mode.");
    private static final String EMPTYTAB = Messages.gettext("Empty tab");

    /**
     * Create a console tab
     * @param uuid the console uuid
     * @return the corresponding tab
     */
    public static Tab getConsoleTab(String uuid) {
        if (Scilab.getMode() != 2) {
            Tab tab = ScilabTab.createTab(EMPTYTAB, uuid);
            JTextArea textarea = new JTextArea(NOCONSOLE);
            textarea.setEditable(false);
            ((SwingScilabTab) tab.getAsSimpleTab()).setContentPane(textarea);
            ((SwingScilabTab) tab.getAsSimpleTab()).setAssociatedXMLIDForHelp("console");

            ClosingOperationsManager.registerClosingOperation(tab, new ClosingOperationsManager.ClosingOperation() {

                    public int canClose() {
                        return 1;
                    }

                    public void destroy() { }

                    public String askForClosing(final List<SwingScilabTab> list) {
                        return null;
                    }

                    @Override
                    public void updateDependencies(List<SwingScilabTab> list,
                                                   ListIterator<SwingScilabTab> it) {
                    }
                });

            ClosingOperationsManager.addDependencyWithRoot(tab);
            WindowsConfigurationManager.restorationFinished((SwingScilabTab) tab.getAsSimpleTab());

            return tab;
        }

        /* Create the console */
        Tab consoleTab = null;
        try {
            /* CONSOLE */
            /* Create a tab to put console into */
            LoadClassPath.loadOnUse("Console");
            if (uuid == null) {
                consoleTab = ScilabTab.createTab(Messages.gettext("Scilab Console"), UUID.randomUUID().toString());
            } else {
                consoleTab = ScilabTab.createTab(Messages.gettext("Scilab Console"), uuid);
            }

            ClosingOperationsManager.registerClosingOperation(consoleTab, new ClosingOperationsManager.ClosingOperation() {

                    public int canClose() {
                        CallScilabBridge.unblockConsole();

                        return 1;
                    }

                    public void destroy() {
                        if (!Scilab.getExitCalled()) {
                            ScilabTermination.ScilabExit();
                        }
                    }

                    public String askForClosing(final List<SwingScilabTab> list) {
                        return "Scilab";
                    }

                    @Override
                    public void updateDependencies(List<SwingScilabTab> list,
                                                   ListIterator<SwingScilabTab> it) {
                    }
                });

            ClosingOperationsManager.setRoot(consoleTab);

            ScilabConsole.createConsole();
        } catch (NoClassDefFoundError exception) {
            System.err.println("Cannot create Scilab Console.\nCheck if the thirdparties are available (JoGL/JRosetta...).\n"
                               + SEE_DEFAULT_PATHS);
            System.err.println(CLASS_NOT_FOUND + exception.getLocalizedMessage());
            System.exit(-1);
        }

        /** Adding content into container */
        ScilabConsole.getConsole().setMaxOutputSize(ConfigManager.getMaxOutputSize());
        consoleTab.addMember(ScilabConsole.getConsole());
        ((SwingScilabTab) consoleTab.getAsSimpleTab()).setAssociatedXMLIDForHelp("console");
        WindowsConfigurationManager.restorationFinished((SwingScilabTab) consoleTab.getAsSimpleTab());

        MenuBar menuBar = ScilabMenuBar.createMenuBar();
        ((SwingScilabTab) consoleTab.getAsSimpleTab()).setMenuBar(menuBar);
        ScilabConsole.getConsole().addMenuBar(menuBar);

        return consoleTab;
    }
}
