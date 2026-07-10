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

import javax.swing.AbstractButton;

import org.flexdock.plaf.theme.ButtonUI;
import org.flexdock.view.Titlebar;
import org.flexdock.view.View;
import org.scilab.modules.commons.gui.ScilabCefPanel;

/**
 * Button UI matching {@link ScilabTitlebarUI}.
 */
public class ScilabButtonUI extends ButtonUI {
    @Override
    protected boolean isParentActive(AbstractButton button) {
        if (super.isParentActive(button)) {
            return true;
        }

        if (button.getParent() instanceof Titlebar) {
            View view = ((Titlebar) button.getParent()).getView();
            return view != null && Boolean.TRUE.equals(view.getClientProperty(ScilabCefPanel.CEF_NATIVE_FOCUS_PROPERTY));
        }

        return false;
    }
}
