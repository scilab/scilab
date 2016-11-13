/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
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

package org.scilab.modules.console;

import java.awt.BorderLayout;

import javax.swing.JComponent;
import javax.swing.JPanel;

import com.artenum.rosetta.interfaces.ui.CommandLineView;
import com.artenum.rosetta.interfaces.ui.OutputView;

/**
 * Scilab Console UI where the user enters a new command to execute including the prompt
 * @author Vincent COUVERT
 */
public class SciCommandLineView extends JPanel implements CommandLineView {

    private static final long serialVersionUID = 1L;

    /**
     * Constructor
     */
    public SciCommandLineView() {
        setMinimumSize(OutputView.SMALL);
    }

    /**
     * Sets the component where the user will enter the command
     * @param command java commponent
     * @see com.artenum.rosetta.interfaces.ui.CommandLineView#setInputCommandView(javax.swing.JComponent)
     */
    public void setInputCommandView(JComponent command) {
        add(command, BorderLayout.CENTER);
    }

    /**
     * Sets the component where th eprompt will be displayed
     * @param prompt java component
     * @see com.artenum.rosetta.interfaces.ui.CommandLineView#setPromptView(javax.swing.JComponent)
     */
    public void setPromptView(JComponent prompt) {
        add(prompt, BorderLayout.WEST);
    }

    /**
     * Sets the visibility of the Command Line UI
     * @param visible the new state (true to set it visible, false else)
     * @see javax.swing.JComponent#setVisible(boolean)
     */
    public void setVisible(boolean visible) {
        if (visible) {
            expand();
        } else {
            shrink();
        }
    }

    /**
     * Sets the Command View invisible
     */
    private void shrink() {
        setMaximumSize(OutputView.SMALL);
        invalidate();
        doLayout();
    }

    /**
     * Sets the Command View visible
     */
    private void expand() {
        setMaximumSize(OutputView.BIG);
        invalidate();
        doLayout();
    }

}
