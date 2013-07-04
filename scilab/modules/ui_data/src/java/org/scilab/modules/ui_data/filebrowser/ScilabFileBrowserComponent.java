/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.filebrowser;

import java.awt.BorderLayout;
import java.awt.Color;

import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JViewport;
import javax.swing.SwingUtilities;
import javax.swing.border.EmptyBorder;

import org.scilab.modules.gui.pushbutton.PushButton;

/**
 * A JPanel containing the different elements composing the File Browser
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class ScilabFileBrowserComponent extends JPanel {

    private static final int GAP = 3;

    private SwingScilabFileBrowser filebrowser;
    private SwingScilabTreeTable stt;

    /**
     * Default constructor
     */
    public ScilabFileBrowserComponent() {
        super(new BorderLayout());
        setBorder(new EmptyBorder(GAP, GAP, GAP, GAP));
        ScilabFileSelectorComboBox combobox = new ScilabFileSelectorComboBox();
        stt = new SwingScilabTreeTable(new ScilabFileBrowserModel(), combobox);
        Color bg = Color.WHITE;
        stt.setBackground(bg);

        add(new ScilabFileSelectorPanel(stt), BorderLayout.PAGE_START);
        add(new ScilabFileSelectorFilter(stt), BorderLayout.PAGE_END);

        JScrollPane jsp = new JScrollPane(stt);
        jsp.getViewport().setBackground(bg);
        add(jsp, BorderLayout.CENTER);
    }

    /**
     * Set the base directory
     * @param baseDir the base directory
     */
    public void setBaseDir(final String baseDir) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                stt.setBaseDir(baseDir);
            }
        });
    }

    /**
     * @return the file browser (as Tab) instance
     */
    public SwingScilabFileBrowser getFileBrowser() {
        return filebrowser;
    }

    /**
     * @return the next button in the history
     */
    public PushButton getNextButton() {
        return stt.getNextButton();
    }

    /**
     * @return the previous button in the history
     */
    public PushButton getPreviousButton() {
        return stt.getPreviousButton();
    }
}
