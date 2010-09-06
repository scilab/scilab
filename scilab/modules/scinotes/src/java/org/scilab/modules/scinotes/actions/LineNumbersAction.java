/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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
import javax.swing.JMenu;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;

/**
 * LineNumbersAction Class
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 */
public final class LineNumbersAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -2778300710964013775L;

    private int state;

    /**
     * Construtor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public LineNumbersAction(String name, SciNotes editor) {
        super(name, editor);
        state = ConfigSciNotesManager.getLineNumberingState();
    }

    /**
     * doAction
     */
    public void doAction() {
        SciNotes.setWhereamiLineNumbering(state);
        ConfigSciNotesManager.saveLineNumberingState(state);
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return createMenu
     */
    public static Menu createMenu(String label, SciNotes editor, KeyStroke key) {
        StringTokenizer tokens = new StringTokenizer(label, ";");
        String labelLineNumbering = tokens.nextToken();
        String labelOff = tokens.nextToken();
        String labelNormal = tokens.nextToken();
        String labelWhereami = tokens.nextToken();

        LineNumbersAction ln = new LineNumbersAction(labelLineNumbering, editor);
        final JRadioButtonMenuItem[] arr = new JRadioButtonMenuItem[3];
        String[] labels = new String[]{labelOff, labelNormal, labelWhereami};

        final Menu menu = ScilabMenu.createMenu();
        menu.setText(labelLineNumbering);

        ButtonGroup group = new ButtonGroup();
        JRadioButtonMenuItem radio;

        for (int i = 0; i < 3; i++) {
            radio = createRadioButtonMenuItem(ln, labels[i], i);
            group.add(radio);
            ((JMenu) menu.getAsSimpleMenu()).add(radio);
            arr[i] = radio;
        }

        arr[ln.state].setSelected(true);

        ((JMenu) menu.getAsSimpleMenu()).addPropertyChangeListener(new PropertyChangeListener() {
                public void propertyChange(PropertyChangeEvent e) {
                    arr[ConfigSciNotesManager.getLineNumberingState()].setSelected(true);
                }
            });

        return menu;
    }

    /**
     * createRadioButtonMenuItem
     * @param ln the LineNumbersAction
     * @param title the label of the menuitem
     * @param state the state associated with the menuitem
     * @return JRadioButtonMenuItem
     */
    private static JRadioButtonMenuItem createRadioButtonMenuItem(final LineNumbersAction ln, String title, final int state) {
        JRadioButtonMenuItem radio = new JRadioButtonMenuItem(title);
        radio.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent arg0) {
                    ln.state = state;
                    ln.doAction();
                }
            });

        return radio;
    }
}
