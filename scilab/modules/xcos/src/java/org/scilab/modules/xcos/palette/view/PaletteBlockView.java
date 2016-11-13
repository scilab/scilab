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

package org.scilab.modules.xcos.palette.view;

import java.awt.Dimension;
import java.awt.Font;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.SwingConstants;
import javax.swing.border.Border;

import org.flexdock.plaf.common.border.ShadowBorder;
import org.scilab.modules.xcos.palette.PaletteBlockCtrl;
import org.scilab.modules.xcos.utils.XcosConstants;

/**
 * The palette block view
 */
@SuppressWarnings(value = { "serial" })
public final class PaletteBlockView extends JLabel {

    private static final Dimension PREFERRED_SIZE = new Dimension(
        XcosConstants.PALETTE_BLOCK_WIDTH,
        XcosConstants.PALETTE_BLOCK_HEIGHT);
    private static final int DEFAULT_FONT_SIZE = 12;
    private static final int DEFAULT_ICON_TEXT_GAP = 5;

    private static final Border SELECTED_BORDER = new ShadowBorder();

    private static final Border NON_SELECTED_BORDER = BorderFactory
            .createEmptyBorder();

    private PaletteBlockCtrl controller;

    /**
     * Default constructor
     *
     * @param controller
     *            the associated controller
     */
    public PaletteBlockView(PaletteBlockCtrl controller) {
        super(controller.getModel().getName(), controller.getModel()
              .getLoadedIcon(), SwingConstants.CENTER);
        this.controller = controller;
        initComponents();
    }

    /** Set up the graphical properties */
    private void initComponents() {
        setPreferredSize(PREFERRED_SIZE);
        setSelectedUI(false);
        setFont(new Font(getFont().getFamily(), 0, DEFAULT_FONT_SIZE));

        setVerticalTextPosition(SwingConstants.BOTTOM);
        setHorizontalTextPosition(SwingConstants.CENTER);
        setIconTextGap(DEFAULT_ICON_TEXT_GAP);

        final String text = controller.getModel().getName();
        setToolTipText(text);
        setText(text);
    }

    /**
     * @return The associated controller
     */
    public PaletteBlockCtrl getController() {
        return controller;
    }

    /**
     * @param status
     *            true To set the UI state selected, false otherwise
     */
    public void setSelectedUI(boolean status) {
        if (status) {
            setBorder(SELECTED_BORDER);
        } else {
            setBorder(NON_SELECTED_BORDER);
        }
    }

    /**
     * @return {@link javax.swing.JComponent#getPreferredSize()}
     * @see javax.swing.JComponent#getMinimumSize()
     */
    @Override
    public Dimension getMinimumSize() {
        return super.getPreferredSize();
    }
}
