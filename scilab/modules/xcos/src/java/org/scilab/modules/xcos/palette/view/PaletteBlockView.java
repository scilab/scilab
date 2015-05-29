/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.palette.view;

import java.awt.Dimension;
import java.awt.Font;
import java.awt.Image;

import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.SwingConstants;
import javax.swing.border.Border;

import org.flexdock.plaf.common.border.ShadowBorder;
import org.scilab.modules.xcos.palette.PaletteBlockCtrl;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosConstants.PaletteBlockSize;

/**
 * The palette block view
 */
@SuppressWarnings(value = { "serial" })
public final class PaletteBlockView extends JLabel {

    private static final int DEFAULT_ICON_TEXT_GAP = 5;

    private static final Border SELECTED_BORDER = new ShadowBorder();

    private static final Border NON_SELECTED_BORDER = BorderFactory
            .createEmptyBorder();

    private PaletteBlockCtrl controller;

    private ImageIcon originalIcon;

    /**
     * Default constructor
     * @param controller The associated controller
     */
    public PaletteBlockView(PaletteBlockCtrl controller) {
        super(controller.getModel().getName(), controller.getModel()
              .getLoadedIcon(), SwingConstants.CENTER);
        this.controller = controller;
        this.originalIcon = (ImageIcon) getIcon();
        initComponents(controller.getPaletteBlockSize());
    }

    /**
     * Set up the graphical properties
     * @param palBlockSize The PaletteBlockSize
     */
    private void initComponents(PaletteBlockSize palBlockSize) {
        setSelectedUI(false);
        setFontSize(palBlockSize.getFontSize());
        setPreferredSize(new Dimension(palBlockSize.getBlockWidth(),
                                       palBlockSize.getBlockHeight()));

        setVerticalTextPosition(SwingConstants.BOTTOM);
        setHorizontalTextPosition(SwingConstants.CENTER);
        setIconTextGap(DEFAULT_ICON_TEXT_GAP);
        setIconSize(palBlockSize.getIconScale());

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
     * Set the font size
     * @param fontSize font size
     */
    public void setFontSize(int fontSize) {
        setFont(new Font(getFont().getFamily(), 0, fontSize));
    }

    /**
     * Set the icon size
     * @param scale new scale
     */
    public void setIconSize(float scale) {
        int width = (int) (this.originalIcon.getIconWidth() * scale);
        int height = (int) (this.originalIcon.getIconHeight() * scale);
        setIcon(new ImageIcon(this.originalIcon.getImage().getScaledInstance(
        		width, height, Image.SCALE_SMOOTH)));
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
