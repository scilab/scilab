/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.gui.bridge.tab;

import java.awt.Graphics;
import java.awt.Rectangle;

import javax.swing.JComponent;

import org.flexdock.plaf.theme.TitlebarUI;
import org.flexdock.view.Titlebar;
import org.flexdock.view.View;
import org.scilab.modules.commons.gui.ScilabCefPanel;

/**
 * FlexDock titlebar UI used by Scilab.
 *
 * CEF browsers hold native focus outside of the regular Swing focus owner
 * chain. This UI keeps the normal FlexDock active rendering, and also paints a
 * titlebar as active when its view explicitly reports native CEF focus.
 */
public class ScilabTitlebarUI extends TitlebarUI {
    private boolean paintCefFocused;

    private boolean isCefFocused(Titlebar titlebar) {
        View view = titlebar.getView();
        return view != null && Boolean.TRUE.equals(view.getClientProperty(ScilabCefPanel.CEF_NATIVE_FOCUS_PROPERTY));
    }

    @Override
    public void paint(Graphics graphics, JComponent component) {
        Titlebar titlebar = (Titlebar) component;
        boolean previousPaintCefFocused = paintCefFocused;
        paintCefFocused = isCefFocused(titlebar);
        try {
            super.paint(graphics, component);
        } finally {
            paintCefFocused = previousPaintCefFocused;
        }
    }

    @Override
    protected void paintBackground(Graphics graphics, Titlebar titlebar) {
        if (!paintCefFocused) {
            super.paintBackground(graphics, titlebar);
            return;
        }

        Rectangle paintRect = getPaintRect(titlebar);
        graphics.translate(paintRect.x, paintRect.y);
        painter.paint(graphics, paintRect.width, paintRect.height, true, titlebar);
        graphics.translate(-paintRect.x, -paintRect.y);
    }

    @Override
    protected java.awt.Color getFontColor(boolean active) {
        return super.getFontColor(active || paintCefFocused);
    }

    @Override
    protected void paintBorder(Graphics graphics, Titlebar titlebar) {
        if (!paintCefFocused) {
            super.paintBorder(graphics, titlebar);
            return;
        }

        javax.swing.border.Border border = getActiveBorder();
        if (border != null) {
            Rectangle paintRect = getPaintRect(titlebar);
            graphics.translate(paintRect.x, paintRect.y);
            border.paintBorder(titlebar, graphics, 0, 0, paintRect.width, paintRect.height);
            graphics.translate(-paintRect.x, -paintRect.y);
        }
    }
}
