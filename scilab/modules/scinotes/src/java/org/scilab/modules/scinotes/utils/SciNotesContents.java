/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
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

package org.scilab.modules.scinotes.utils;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;

import javax.swing.JPanel;

import org.scilab.modules.scinotes.SciNotes;

/**
 * Content pane for SciNotes.
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class SciNotesContents extends JPanel {

    private ScilabTabbedPane stp;
    private Component bottom;

    /**
     * Default constructor
     * @param editor the editor where to put this pane
     */
    public SciNotesContents(final SciNotes editor) {
        super(new BorderLayout());
        stp = new ScilabTabbedPane(editor);
        add(stp);
        addFocusListener(new FocusListener() {
            public void focusGained(FocusEvent e) {
                if (editor.getTextPane() != null) {
                    editor.getTextPane().requestFocus();
                }
            }
            public void focusLost(FocusEvent e) { }
        });
    }

    /**
     * @return the tabbed pane used in this content pane
     */
    public ScilabTabbedPane getScilabTabbedPane() {
        return stp;
    }

    /**
     * @param comp the component to insert at the bottom of this panel
     */
    public void insertBottomComponent(Component comp) {
        if (comp == null) {
            if (bottom != null) {
                remove(bottom);
            }
        } else {
            bottom = comp;
            add(comp, BorderLayout.PAGE_END);
        }
        revalidate();
    }

    /**
     * @return the component at the bottom
     */
    public Component getBottom() {
        return bottom;
    }
}
