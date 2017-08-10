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

import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.JPanel;
import javax.swing.JTextPane;

/**
 * NO MORE USED BECAUSE NOW THE CARET OF THE OUTPUT VIEW IS USED TO DO THIS WORK
 * Mouse listener for the output view
 * Automatically forward the focus to input view
 * @author Vincent COUVERT
 */
public class FocusMouseListener implements MouseListener {

    private SciConsole c;

    /**
     * @param console the console object associated
     */
    public FocusMouseListener(SciConsole console) {
        super();
        c = console;
    }

    /**
     * User click
     * @param e the event to threat
     * @see java.awt.event.MouseListener#mouseClicked(java.awt.event.MouseEvent)
     */
    public void mouseClicked(MouseEvent e) {
        // If the user clicks in the Outputview then the focus is given to the input command view
        if (((JPanel) c.getConfiguration().getPromptView()).isVisible()) {
            ((JTextPane) c.getConfiguration().getInputCommandView()).grabFocus();
        } else {
            // In case of "more ?"
            c.grabFocus();
        }
    }

    /**
     * Mouse enters in the object
     * @param e the event to threat
     * @see java.awt.event.MouseListener#mouseEntered(java.awt.event.MouseEvent)
     */
    public void mouseEntered(MouseEvent e) {

    }

    /**
     * Mouse exists from the object
     * @param e the event to threat
     * @see java.awt.event.MouseListener#mouseExited(java.awt.event.MouseEvent)
     */
    public void mouseExited(MouseEvent e) {

    }

    /**
     * Mouse button pressed
     * @param e the event to threat
     * @see java.awt.event.MouseListener#mousePressed(java.awt.event.MouseEvent)
     */
    public void mousePressed(MouseEvent e) {

    }

    /**
     * Mouse button released
     * @param e the event to threat
     * @see java.awt.event.MouseListener#mouseReleased(java.awt.event.MouseEvent)
     */
    public void mouseReleased(MouseEvent e) {

    }

    /**
     * Mouse stay over the object
     * @param e the event to threat
     * @see java.awt.event.MouseListener#mouseReleased(java.awt.event.MouseEvent)
     */
    public void mouseOver(MouseEvent e) {
        // If the user clicks in the Outputview then the focus is given to the input command view
        if (((JPanel) c.getConfiguration().getPromptView()).isVisible()) {
            ((JTextPane) c.getConfiguration().getInputCommandView()).grabFocus();
        } else {
            // In case of "more ?"
            c.grabFocus();
        }
    }
}
