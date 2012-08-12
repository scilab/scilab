/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
                editor.getTextPane().requestFocus();
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
