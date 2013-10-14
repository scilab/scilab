/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.uiwidget.components;

import java.util.HashSet;
import java.util.Set;

import javax.swing.ActionMap;
import javax.swing.InputMap;
import javax.swing.JComponent;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

/**
 * Wrapper for a keymap object
 */
public class UIKeymap extends UIComponent {

    private Set<UIKeystroke> set;

    /**
     * {@inheritDoc}
     */
    public UIKeymap(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        set = new HashSet<UIKeystroke>();
        return this;
    }

    /**
     * {@inheritDoc}
     */
    public void add(UIComponent c) throws UIWidgetException {
        if (c instanceof UIKeystroke) {
            set.add((UIKeystroke) c);
            update((UIKeystroke) c);
        } else {
            throw new UIWidgetException("Cannot insert a " + c.getClass().getName() + " in a UIKeymap");
        }
    }

    /**
     * Update the input keymap
     * @param c the keystroke
     */
    public void update(UIKeystroke c) {
        if (!isRoot()) {
            Object o = getParent().getModifiableComponent();
            if (o instanceof JComponent) {
                UIKeystroke key = (UIKeystroke) c;
                JComponent jc = (JComponent) o;
                InputMap input = jc.getInputMap();//JComponent.WHEN_IN_FOCUSED_WINDOW);
                ActionMap action = jc.getActionMap();
                input.put(key.getKey(), key);
                action.put(key, key.getRealAction());
            }
        }
    }

    /**
     * Update the input keymap
     */
    public void update() {
        if (!isRoot()) {
            Object o = getParent().getModifiableComponent();
            if (o instanceof JComponent) {
                JComponent jc = (JComponent) o;
                InputMap input = jc.getInputMap();//JComponent.WHEN_IN_FOCUSED_WINDOW);
                ActionMap action = jc.getActionMap();
                for (UIKeystroke key : set) {
                    input.put(key.getKey(), key);
                    action.put(key, key.getRealAction());
                }
            }
        }
    }

    /**
     * Replace a key
     * @param oldKey the old key
     * @param newKey the new key
     */
    void replaceKey(KeyStroke oldKey, KeyStroke newKey) {
        Object o = getParent().getModifiableComponent();
        if (o instanceof JComponent) {
            JComponent jc = (JComponent) o;
            InputMap input = jc.getInputMap();
            Object v = input.get(oldKey);
            input.put(oldKey, null);
            input.put(newKey, v);
        }
    }
}
