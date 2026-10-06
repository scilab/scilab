/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.actions;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.util.Arrays;
import java.util.stream.Collectors;

import javax.swing.JButton;
import javax.swing.filechooser.FileFilter;
import javax.swing.filechooser.FileNameExtensionFilter;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.filechooser.Juigetfile;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.filechooser.FileChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.XcosFileType;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Management of "Diagram Saving"
 *
 * @author Vincent COUVERT
 */
public final class SaveAsAction extends DefaultAction {
    private static final long serialVersionUID = 1L;
    /** Name of the action */
    public static final String NAME = XcosMessages.SAVE_AS;
    /** Icon name of the action */
    public static final String SMALL_ICON = "document-save-as";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_S;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = KeyEvent.SHIFT_DOWN_MASK | Toolkit.getDefaultToolkit().getMenuShortcutKeyMaskEx();

    /**
     * Constructor
     *
     * @param scilabGraph
     *            associated Xcos diagram
     */
    public SaveAsAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Create menu for saving
     *
     * @param scilabGraph
     *            associated Xcos diagram
     * @return the menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, SaveAsAction.class);
    }

    /**
     * Create toolbar button for saving
     *
     * @param scilabGraph
     *            associated Xcos diagram
     * @return the button
     */
    public static JButton createButton(ScilabGraph scilabGraph) {
        return createButton(scilabGraph, SaveAsAction.class);
    }

    /**
     * @param e
     *            parameter
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        final XcosDiagram graph = (XcosDiagram) getGraph(null);
        if (graph.saveDiagramAs(null)) {
            graph.setModified(false);
        }
    }

    /*
     * Helpers functions to share file chooser code
     */

    public static FileChooser createFileChooser() {
        final FileChooser fc = ScilabFileChooser.createFileChooser();

        fc.setTitle(XcosMessages.SAVE_AS);
        fc.setUiDialogType(Juigetfile.SAVE_DIALOG);
        fc.setMultipleSelection(false);
        return fc;
    }

    public static void configureFileFilters(final FileChooser fc) {
        fc.setAcceptAllFileFilterUsed(false);

        final FileFilter[] filters = XcosFileType.getSavingFilters();
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
}

