/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
 * Copyright (C) 2015 - Marcos CARDINOT
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
import javax.swing.border.EtchedBorder;

import org.flexdock.plaf.common.border.ShadowBorder;
import org.scilab.modules.xcos.palette.PaletteBlockCtrl;
import org.scilab.modules.xcos.palette.actions.ZoomAction;
import org.scilab.modules.xcos.utils.XcosConstants.PaletteBlockSize;

/**
 * The palette block view
 */
@SuppressWarnings(value = { "serial" })
public final class PaletteBlockView extends JLabel {

    private static final int DEFAULT_ICON_TEXT_GAP = 5;

    private static final Border SELECTED_BORDER = new ShadowBorder();
    private static final Border HOVER_BORDER = BorderFactory
            .createEtchedBorder(EtchedBorder.LOWERED);
    private static final Border NON_SELECTED_BORDER = BorderFactory
            .createEmptyBorder();
    private StatusUI statusUI;
    private PaletteBlockCtrl controller;

    /**
     * UI states
     */
    public enum StatusUI {
        /** block is selected **/
        SELECTED,
        /** block is not selected **/
        NON_SELECTED,
        /** hover effect **/
        HOVER
    }

    /**
     * Default constructor
     * @param controller The associated controller
     */
    public PaletteBlockView(PaletteBlockCtrl controller) {
        super(controller.getModel().getName(), SwingConstants.CENTER);
        this.controller = controller;
        setStatusUI(StatusUI.NON_SELECTED);
        ZoomAction.registerKeyAction(getActionMap(), getInputMap());
        initComponents();
    }

    /**
     * Set up the graphical properties
     */
    public void initComponents() {
        PaletteBlockSize palBlockSize = PaletteManagerPanel.getCurrentSize();

        // adjust the jlabel size
        setPreferredSize(palBlockSize.getBlockDimension());

        // adjust the font size
        setFont(new Font(getFont().getFamily(), 0, palBlockSize.getFontSize()));

        // reloads the icon with the correct size
        setIcon(controller.getModel().getLoadedIcon(palBlockSize.getMaxIconWidth(),
                                                    palBlockSize.getMaxIconHeight()));

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
     * @return border status
     */
    public StatusUI getStatusUI() {
        return statusUI;
    }

    /**
     * @param status Border status
     */
    public void setStatusUI(StatusUI status) {
        statusUI = status;
        if (status == StatusUI.SELECTED) {
            setBorder(SELECTED_BORDER);
        } else if (status == StatusUI.HOVER) {
            setBorder(HOVER_BORDER);
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
