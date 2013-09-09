/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.uiwidget.components;

import java.awt.Component;
import java.awt.GridBagConstraints;
import java.util.LinkedList;
import java.util.List;

import javax.swing.JPanel;
import javax.swing.JPopupMenu;

import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIWidgetException;

public class Frame extends ScilabWidget {

    protected GridBagConstraints gbc = new GridBagConstraints();
    protected List<Component> enabledComponents;

    public Frame(UIComponent parent) throws UIWidgetException {
        super(parent);
        enabledComponents = new LinkedList<Component>();
    }

    public Object newInstance() {
        JPanel frame = new JPanel();
        frame.setLayout(new NoLayout());

        return frame;
    }

    public void initialize() {
        super.initialize();
        try {
            setRelief(ScilabRelief.RIDGE);
        } catch (UIWidgetException e) { }
    }

    public void setEnabled(boolean status) {
        JPanel frame = (JPanel) getModifiableComponent();
        UIPanel.setEnable(frame, status, enabledComponents);
    }

    /**
     * {@inheritDoc}
     */
    public void add(final UIComponent c) throws UIWidgetException {
        Object o = c.getComponent();
        if (o instanceof Component && !(o instanceof JPopupMenu)) {
            JPanel frame = (JPanel) getModifiableComponent();
            UIPanel.addToPanel(frame, c, gbc);
            if (frame.isVisible()) {
                frame.revalidate();
            }
        } else {
            super.add(c);
        }
    }
}
