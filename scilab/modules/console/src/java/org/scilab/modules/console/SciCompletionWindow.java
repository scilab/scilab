/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010-2011 - Calixte DENIZET
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

import java.awt.Component;
import java.awt.Point;
import java.util.List;

import javax.swing.JScrollPane;
import javax.swing.text.JTextComponent;

import com.artenum.rosetta.interfaces.core.CompletionItem;

import org.scilab.modules.completion.AbstractSciCompletionWindow;

/**
 * Scilab completion window main class
 * @author Vincent COUVERT
 * @author Calixte DENIZET
 */
public class SciCompletionWindow extends AbstractSciCompletionWindow {

    private SciConsole sciConsole;

    /**
     * Default constructor
     */
    public SciCompletionWindow() {
    }

    /**
     * @return the JTextComponent where the CompletionWindow will be drawn
     */
    public JTextComponent getTextComponent() {
        return (JTextComponent) sciConsole.getConfiguration().getInputCommandView();
    }

    /**
     * @return the list of the possibilities to complete
     */
    public List<CompletionItem> getCompletionItems() {
        return sciConsole.getConfiguration().getCompletionManager().getCompletionItems();
    }

    /**
     * Set the console associated to this completion window
     * @param console the console object to set
     */
    public void setConsole(SciConsole console) {
        this.sciConsole = console;
    }

    /**
     * Set the parent component for the completion window
     * @param component the parent component
     * Caution, the component shouldn't be null otherwise the completion window
     * will never get the focus
     */
    public void setGraphicalContext(Component component) {

        /* When called from console builder */
        if (this.sciConsole == null) {
            return;
        }

        super.setGraphicalContext(component);
    }

    /**
     * Make the completion window visible and update its content
     * @param list list of items matching completion
     * @param location position of the top left corner of the window
     * @see com.artenum.rosetta.interfaces.ui.CompletionWindow#show(java.util.List, java.awt.Point)
     */
    public void show(List<CompletionItem> list, Point location) {
        currentCaretPosition = inputParsingManager.getCaretPosition();
        /* Display only in completion items list is not empty */
        if (list != null) {

            /* If completion window size bigger than input command view size */
            /* Change the size of the input command view */
            JScrollPane jSP = sciConsole.getJScrollPane();
            if (window.getHeight() > getTextComponent().getHeight()) {

                int newWidth = jSP.getWidth() - jSP.getVerticalScrollBar().getPreferredSize().width;

                int yCarPos = ((SciInputCommandView) sciConsole.getConfiguration().getInputCommandView()).getCaretLocation().y;
                int newHeight =  yCarPos +  window.getHeight();

                SciInputCommandView sicv = (SciInputCommandView) getTextComponent();
                sicv.setPreferredWidth(newWidth);
                sicv.setPreferredHeight(newHeight);

                getTextComponent().validate();
            }

            model.updateData(list);

            /* Display completion window */
            window.setLocation(location);
            setVisible(true);

            scrollPane.getViewport().setViewPosition(new Point(0, 0));

            if (model.getSize() > 0) {
                listUI.setSelectedIndex(0);
            }
            listUI.grabFocus();

            /* Scrollbar update after displaying completion window */
            if (window.getHeight() > getTextComponent().getHeight()) {
                sciConsole.updateScrollPosition();
            }

        }
    }

    /**
     * {@inheritDoc}
     */
    public void setVisible(boolean visible) {
        if (!visible) {
            SciInputCommandView sicv = (SciInputCommandView) getTextComponent();
            sicv.setPreferredWidth(-1);
            sicv.setPreferredHeight(-1);
            sicv.validate();
        }
        super.setVisible(visible);
    }

}
