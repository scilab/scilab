/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.palette.view;

import java.awt.Component;

import javax.swing.BoxLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;

import org.scilab.modules.xcos.palette.PaletteBlockCtrl;
import org.scilab.modules.xcos.palette.model.PaletteBlock;
import org.scilab.modules.xcos.utils.XcosConstants.PaletteBlockSize;

/**
 * Implement a view to show the search results.
 * @author Marcos Cardinot <mcardinot@gmail.com>
 */
@SuppressWarnings(value = { "serial" })
public class PaletteSearchView extends JPanel {

    private JLabel header;
    private PaletteView paletteview;

    /**
     * Default constructor
     */
    public PaletteSearchView() {
        this.header = new JLabel();
        this.paletteview = new PaletteView();
    }

    /**
     * Setup component
     */
    public void initComponents() {
        paletteview.removeAll();
        removeAll();

        header.setAlignmentX(Component.LEFT_ALIGNMENT);
        paletteview.setAlignmentX(Component.LEFT_ALIGNMENT);

        setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
        add(header);
        add(paletteview);
    }

    /**
     * Set the header
     * @param str String
     */
    public void setText(String str) {
        header.setText(str);
    }

    /**
     * Adds a palette block
     * @param block PaletteBlock
     */
    public void addBlock(PaletteBlock block) {
        PaletteBlockSize pbs = PaletteManagerView.get().getPanel().getCurrentSize();
        paletteview.add(new PaletteBlockCtrl(pbs, block).getView());
    }
}
