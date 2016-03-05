/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
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

package org.scilab.modules.xcos.palette.actions;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.io.File;

import javax.swing.JButton;

import org.scilab.modules.commons.CommonFileUtils;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.actions.OpenAction;
import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.xcos.palette.model.Category;
import org.scilab.modules.xcos.palette.model.Custom;
import org.scilab.modules.xcos.palette.model.PaletteNode;
import org.scilab.modules.xcos.palette.model.VariablePath;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Load a diagram on the palette.
 *
 * It will load each block as an independent block and produce a visible panel
 * similar to {@link org.scilab.modules.xcos.palette.model.Palette}.
 */
@SuppressWarnings(value = { "serial" })
public final class LoadAsPalAction extends DefaultAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.OPEN;
    /** Icon name of the action */
    public static final String SMALL_ICON = "document-open";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_O;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

    /**
     * Constructor
     *
     * @param scilabGraph
     *            associated Scilab Graph
     */
    public LoadAsPalAction(final ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Create a menu to add in Scilab Graph menu bar
     *
     * @param scilabGraph
     *            associated Scilab Graph
     * @return the menu
     */
    public static MenuItem createMenu(final ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, LoadAsPalAction.class);
    }

    /**
     * Create a button to add in Scilab Graph tool bar
     *
     * @param scilabGraph
     *            associated Scilab Graph
     * @return the button
     */
    public static JButton createButton(final ScilabGraph scilabGraph) {
        return createButton(scilabGraph, LoadAsPalAction.class);
    }

    /**
     * @param e
     *            parameter
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(final ActionEvent e) {
        final SwingScilabFileChooser fc = OpenAction.createFileChooser();

        /* Configure the file chooser */
        OpenAction.configureFileFilters(fc);
        fc.setCurrentDirectory(new File(CommonFileUtils.getCWD()));

        fc.displayAndWait();

        if ((fc.getSelection() == null) || (fc.getSelection().length == 0) || fc.getSelection()[0].equals("")) {
            return;
        }

        final String file = fc.getSelection()[0];
        final Custom c = new Custom();
        c.setName(file);
        c.setEnable(true);
        final VariablePath v = new VariablePath();
        v.setPath(file);
        c.setPath(v);

        final Category root = PaletteManager.getInstance().getRoot();
        root.getNode().add(c);
        c.setParent(root);
        PaletteNode.refreshView(root, c);
    }
}
