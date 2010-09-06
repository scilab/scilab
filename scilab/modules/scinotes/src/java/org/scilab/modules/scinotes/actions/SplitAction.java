/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.scinotes.actions;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.StringTokenizer;

import javax.swing.ButtonGroup;
import javax.swing.JComponent;
import javax.swing.JMenu;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.JSplitPane;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.scinotes.SciNotes;

/**
 * SplitAction Class
 * @author Calixte DENIZET
 */
public final class SplitAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = 1L;

    private int state;

    /**
     * Construtor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public SplitAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        switch (state) {
        case 0 :
            getEditor().removeSplit();
            return;
        case 1 :
            getEditor().splitTab(false);
            return;
        default :
            getEditor().splitTab(true);
        }
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return createMenu
     */
    public static Menu createMenu(String label, final SciNotes editor, KeyStroke key) {
        StringTokenizer tokens = new StringTokenizer(label, ";");
        String labelSplitView = tokens.nextToken();
        String labelOff = tokens.nextToken();
        String labelH = tokens.nextToken();
        String labelV = tokens.nextToken();

        SplitAction ln = new SplitAction(labelSplitView, editor);
        Menu menu = ScilabMenu.createMenu();
        menu.setText(labelSplitView);

        ButtonGroup group = new ButtonGroup();
        JRadioButtonMenuItem radio;
        final JRadioButtonMenuItem[] arr = new JRadioButtonMenuItem[3];
        String[] labels = new String[]{labelOff, labelH, labelV};

        for (int i = 0; i < 3; i++) {
            radio = createRadioButtonMenuItem(ln, labels[i], i);
            group.add(radio);
            ((JMenu) menu.getAsSimpleMenu()).add(radio);
            arr[i] = radio;
        }

        arr[0].setSelected(true);

        ((JMenu) menu.getAsSimpleMenu()).addPropertyChangeListener(new PropertyChangeListener() {
                public void propertyChange(PropertyChangeEvent e) {
                    JComponent c = editor.getTextPane().getParentComponent();
                    int state = 0;
                    if (c instanceof JSplitPane) {
                        JSplitPane split = (JSplitPane) c;
                        if (split.getOrientation() == JSplitPane.HORIZONTAL_SPLIT) {
                            state = 1;
                        } else {
                            state = 2;
                        }
                    }
                    arr[state].setSelected(true);
                }
            });

        return menu;
    }

    /**
     * createRadioButtonMenuItem
     * @param split the SplitAction
     * @param title the label of the menuitem
     * @param state the state associated with the menuitem
     * @return JRadioButtonMenuItem
     */
    private static JRadioButtonMenuItem createRadioButtonMenuItem(final SplitAction split, String title, final int state) {
        JRadioButtonMenuItem radio = new JRadioButtonMenuItem(title);
        radio.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent arg0) {
                    split.state = state;
                    split.doAction();
                }
            });

        return radio;
    }
}
