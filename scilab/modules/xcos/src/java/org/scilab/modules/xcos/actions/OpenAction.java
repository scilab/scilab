/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 * Copyright (C) 2021 - UTC - Stéphane MOTTELET
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.xcos.actions;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.io.File;
import java.io.IOException;
import java.util.Arrays;
import java.util.stream.Collectors;

import javax.swing.JButton;
import javax.swing.filechooser.FileFilter;
import javax.swing.filechooser.FileNameExtensionFilter;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.filechooser.FileChooser;
import org.scilab.modules.gui.filechooser.Juigetfile;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.configuration.ConfigurationManager;
import org.scilab.modules.xcos.io.XcosFileType;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * File opening management
 */
public final class OpenAction extends DefaultAction {
    private static final long serialVersionUID = 1L;
    /** Name of the action */
    public static final String NAME = XcosMessages.OPEN;
    /** Icon name of the action */
    public static final String SMALL_ICON = "document-open";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_O;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMaskEx();

    /**
     * Constructor
     *
     * @param scilabGraph
     *            associated Scilab Graph
     */
    public OpenAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Create a menu to add in Scilab Graph menu bar
     *
     * @param scilabGraph
     *            associated Scilab Graph
     * @return the menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, OpenAction.class);
    }

    /**
     * Create a button to add in Scilab Graph tool bar
     *
     * @param scilabGraph
     *            associated Scilab Graph
     * @return the button
     */
    public static JButton createButton(ScilabGraph scilabGraph) {
        return createButton(scilabGraph, OpenAction.class);
    }

    /**
     * @param e
     *            parameter
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        final FileChooser fc = createFileChooser();
        /* Configure the file chooser */
        configureFileFilters(fc);
        ConfigurationManager.configureCurrentDirectory(fc);
        try {
            displayAndOpen(fc, getGraph(e).getAsComponent());
        } catch (IOException e1) {
            e1.printStackTrace();
        }
    }

    /*
     * Helpers functions to share file chooser code
     */

    public static FileChooser createFileChooser() {
        final FileChooser fc = ScilabFileChooser.createFileChooser();

        fc.setTitle(XcosMessages.OPEN);
        fc.setUiDialogType(Juigetfile.OPEN_DIALOG);
        fc.setMultipleSelection(true);
        return fc;
    }

    public static void configureFileFilters(final FileChooser fc) {
        fc.setAcceptAllFileFilterUsed(true);

        final FileFilter[] filters = XcosFileType.getLoadingFilters();
        String[] exts = new String[filters.length];
        String[] descr = new String[filters.length];
        for (int i = 0; i < filters.length; i++) {
            FileNameExtensionFilter filter = (FileNameExtensionFilter) filters[i];
            // addMask handle file extension through a regexp
            exts[i] = Arrays.stream(filter.getExtensions()).map(ext -> "*." + ext).collect( Collectors.joining( "|"));
            descr[i] = filter.getDescription();
        }
        fc.addMask(exts, descr);
    }

    protected static void displayAndOpen(final FileChooser fc, final java.awt.Component component) throws IOException {
        fc.displayAndWait();
        String[] selection = fc.getSelection();
        if (selection.length>0 && selection[0] != "") {
            for (int i=0; i<selection.length; i++) {
                 Xcos.getInstance().open(selection[i], 0);
            }
        }
    }

}
