/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
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

import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.SciNotesConfiguration;
import org.scilab.modules.scinotes.SciNotesOptions;
import org.scilab.modules.scinotes.SciNotesLineNumberPanel;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;

/**
 * LineNumbersAction Class
 * @author Calixte DENIZET
 */
public final class LineNumbersAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -2778300710964013775L;

    private static int state;
    private final JRadioButtonMenuItem[] arr = new JRadioButtonMenuItem[3];

    /**
     * Construtor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public LineNumbersAction(String name, SciNotes editor) {
        super(name, editor);
        init();
    }

    /**
     * doAction
     */
    public void doAction() { }

    private static int getState() {
        return SciNotesLineNumberPanel.getState(SciNotesOptions.getSciNotesDisplay().showLineNumbers, SciNotesOptions.getSciNotesDisplay().whereami);
    }

    private static void setState(int state) {
        boolean[] states = SciNotesLineNumberPanel.getState(state);
        SciNotesOptions.getSciNotesDisplay().showLineNumbers = states[0];
        SciNotesOptions.getSciNotesDisplay().whereami = states[1];
        XConfiguration.set(XConfiguration.getXConfigurationDocument(), SciNotesOptions.DISPLAYPATH + "/@show-line-numbers", Boolean.toString(states[0]));
        XConfiguration.set(XConfiguration.getXConfigurationDocument(), SciNotesOptions.DISPLAYPATH + "/@whereami", Boolean.toString(states[1]));
        SciNotes.setWhereamiLineNumbering();
    }

    private void init() {
        ButtonGroup group = new ButtonGroup();
        JRadioButtonMenuItem radio;

        for (int i = 0; i < 3; i++) {
            radio = createRadioButtonMenuItem(i);
            group.add(radio);
            arr[i] = radio;
        }
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return createMenu
     */
    public static Menu createMenu(String label, SciNotes editor, KeyStroke key) {
        StringTokenizer tokens = new StringTokenizer(label, ";\uff1b");
        String labelLineNumbering = tokens.nextToken();
        String labelOff = tokens.nextToken();
        String labelNormal = tokens.nextToken();
        String labelWhereami = tokens.nextToken();

        final LineNumbersAction ln = new LineNumbersAction(labelLineNumbering, editor);
        String[] labels = new String[] {labelOff, labelNormal, labelWhereami};

        final Menu menu = ScilabMenu.createMenu();
        menu.setText(labelLineNumbering);

        ln.arr[getState()].setSelected(true);

        for (int i = 0; i < 3; i++) {
            ((JMenu) menu.getAsSimpleMenu()).add(ln.arr[i]);
            ln.arr[i].setText(labels[i]);
        }

        ((JMenu) menu.getAsSimpleMenu()).addPropertyChangeListener(new PropertyChangeListener() {
            public void propertyChange(PropertyChangeEvent e) {
                ln.arr[getState()].setSelected(true);
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
    private static JRadioButtonMenuItem createRadioButtonMenuItem(final int state) {
        JRadioButtonMenuItem radio = new JRadioButtonMenuItem();
        radio.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent arg0) {
                LineNumbersAction.setState(state);
            }
        });

        return radio;
    }
}
