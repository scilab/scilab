/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
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

package org.scilab.modules.core;

import java.util.List;
import java.util.ListIterator;
import java.util.UUID;

import javax.swing.JTextArea;

import org.scilab.modules.commons.ScilabCommonsUtils;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.gui.ScilabTermination;
import org.scilab.modules.gui.bridge.CallScilabBridge;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.gui.utils.ToolBarBuilder;
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
    private static final String MAINTOOLBARXMLFILE = ScilabConstants.SCI + "/modules/gui/etc/main_toolbar.xml";

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
            ((SwingScilabDockablePanel) tab.getAsSimpleTab()).setContentPane(textarea);
            ((SwingScilabDockablePanel) tab.getAsSimpleTab()).setAssociatedXMLIDForHelp("console");

            ClosingOperationsManager.registerClosingOperation(tab, new ClosingOperationsManager.ClosingOperation() {

                public int canClose() {
                    return 1;
                }

                public void destroy() { }

                public String askForClosing(final List<SwingScilabDockablePanel> list) {
                    return null;
                }

                @Override
                public void updateDependencies(List<SwingScilabDockablePanel> list,
                                               ListIterator<SwingScilabDockablePanel> it) {
                }
            });

            ClosingOperationsManager.addDependencyWithRoot(tab);
            WindowsConfigurationManager.restorationFinished((SwingScilabDockablePanel) tab.getAsSimpleTab());

            return tab;
        }

        /* Create the console */
        Tab consoleTab = null;
        try {
            /* CONSOLE */
            /* Create a tab to put console into */
            LoadClassPath.loadOnUse("Console");
            String tabName = String.format(Messages.gettext("Scilab %s Console"), ScilabCommonsUtils.getScilabVersionAsString());
            if (uuid == null) {
                consoleTab = ScilabTab.createTab(tabName, UUID.randomUUID().toString());
            } else {
                consoleTab = ScilabTab.createTab(tabName, uuid);
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

                public String askForClosing(final List<SwingScilabDockablePanel> list) {
                    return "Scilab";
                }

                @Override
                public void updateDependencies(List<SwingScilabDockablePanel> list,
                                               ListIterator<SwingScilabDockablePanel> it) {
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
        ((SwingScilabDockablePanel) consoleTab.getAsSimpleTab()).setAssociatedXMLIDForHelp("console");

        MenuBar menuBar = ScilabMenuBar.createMenuBar();
        ((SwingScilabDockablePanel) consoleTab.getAsSimpleTab()).setMenuBar(menuBar);
        ScilabConsole.getConsole().addMenuBar(menuBar);

        ToolBar toolBar = ToolBarBuilder.buildToolBar(MAINTOOLBARXMLFILE);
        TextBox infoBar = ScilabTextBox.createTextBox();

        toolBar.setVisible(true); // Enabled in scilab.start

        ((SwingScilabDockablePanel) consoleTab.getAsSimpleTab()).setToolBar(toolBar);
        ((SwingScilabDockablePanel) consoleTab.getAsSimpleTab()).setInfoBar(infoBar);

        ScilabConsole.getConsole().addMenuBar(consoleTab.getMenuBar());
        ScilabConsole.getConsole().addToolBar(toolBar);
        ScilabConsole.getConsole().addInfoBar(infoBar);

        return consoleTab;
    }
}
