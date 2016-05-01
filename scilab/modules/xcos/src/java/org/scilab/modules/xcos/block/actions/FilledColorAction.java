/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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

package org.scilab.modules.xcos.block.actions;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import javax.swing.JColorChooser;

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.VertexSelectionDependantAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxUtils;

/**
 * Change the filled color of multiple blocks
 */
@SuppressWarnings(value = { "serial" })
public final class FilledColorAction extends VertexSelectionDependantAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.FILL_COLOR;
    /** Icon name of the action */
    public static final String SMALL_ICON = "color-fill";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = 0;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = 0;

    /**
     * @param scilabGraph
     *            graph
     */
    public FilledColorAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * @param scilabGraph
     *            graph
     * @return menu item
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, FilledColorAction.class);
    }

    /**
     * @param e
     *            parameter
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        final XcosDiagram graph = (XcosDiagram) getGraph(e);

        // action disabled when the cell is edited
        final ScilabComponent comp = ((ScilabComponent) graph.getAsComponent());
        if (comp.isEditing()) {
            return;
        }

        Object[] selectedCells = graph.getSelectionCells();

        // if no cells are selected : Do nothing
        if (selectedCells.length == 0) {
            return;
        }

        // Get the selected cells statistics values
        Map<String, Integer> colorStats = new HashMap<String, Integer>();
        for (Object object : selectedCells) {
            String color = (String) graph.getCellStyle(object).get(
                               mxConstants.STYLE_FILLCOLOR);
            if (colorStats.containsKey(color)) {
                colorStats.put(color, colorStats.get(color) + 1);
            } else {
                colorStats.put(color, 1);
            }
        }

        // Getting the most present color
        String color = "#FF0000";
        int max = 0;
        for (Entry<String, Integer> entry : colorStats.entrySet()) {
            final int current = entry.getValue();
            if (current > max) {
                color = entry.getKey();
                max = current;
            }
        }

        // Apply the most common color as the default color
        Color newColor = JColorChooser.showDialog(comp, NAME, mxUtils.parseColor(color));

        if (newColor != null) {
            graph.setCellStyles(mxConstants.STYLE_FILLCOLOR,
                                mxUtils.hexString(newColor));
        }
    }
}
