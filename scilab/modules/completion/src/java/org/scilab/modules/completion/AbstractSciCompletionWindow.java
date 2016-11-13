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

package org.scilab.modules.completion;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Point;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import javax.swing.AbstractListModel;
import javax.swing.JComponent;
import javax.swing.text.JTextComponent;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

import com.artenum.rosetta.ui.CompletionItemListCellRenderer;
import com.artenum.rosetta.interfaces.core.CompletionItem;
import com.artenum.rosetta.interfaces.core.InputParsingManager;
import com.artenum.rosetta.interfaces.ui.CompletionWindow;

import org.scilab.modules.completion.Completion;
import org.scilab.modules.localization.Messages;

/**
 * Scilab completion window main class
 * @author Vincent COUVERT
 * @author Calixte DENIZET
 */
public abstract class AbstractSciCompletionWindow implements CompletionWindow, KeyListener, FocusListener, MouseMotionListener, MouseListener {

    private static final int WINDOW_WIDTH = 300;
    private static final int WINDOW_HEIGHT = 100;

    protected JPanel window;
    protected CompletionItemListModel model;
    protected JList listUI;
    protected JScrollPane scrollPane;
    protected InputParsingManager inputParsingManager;
    protected JComponent focusOutComponent;

    protected int currentCaretPosition;

    /**
     * Default constructor
     */
    public AbstractSciCompletionWindow() {
    }

    /**
     * @return the JTextComponent where the CompletionWindow will be drawn
     */
    public abstract JTextComponent getTextComponent();

    /**
     * @return the list of the possibilities to complete
     */
    public abstract List<CompletionItem> getCompletionItems();

    /**
     * Set the component that should take the focus when the completion window is hidden
     * @param component the component to focus
     * @see com.artenum.rosetta.interfaces.ui.CompletionWindow#setFocusOut(javax.swing.JComponent)
     */
    @Override
    public void setFocusOut(JComponent component) {
        focusOutComponent = component;
    }

    /**
     * Set the input parsing manager associated to this completion window
     * @param inputParsingManager the input parsing manager to set
     * @see com.artenum.rosetta.interfaces.ui.CompletionWindow#setInputParsingManager(com.artenum.rosetta.interfaces.core.InputParsingManager)
     */
    @Override
    public void setInputParsingManager(InputParsingManager inputParsingManager) {
        this.inputParsingManager = inputParsingManager;
    }

    /**
     * Set the parent component for the completion window
     * @param component the parent component
     * Caution, the component shouldn't be null otherwise the completion window
     * will never get the focus
     */
    @Override
    public void setGraphicalContext(Component component) {

        /* List to display all completion items */
        model = new CompletionItemListModel();
        listUI = new JList(model);
        listUI.setCellRenderer(new CompletionItemListCellRenderer());
        scrollPane = new JScrollPane(listUI, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

        /* Utility to able the user to resize the window */
        JLabel windowResizeCorner = new JLabel("~", JLabel.CENTER);
        windowResizeCorner.addMouseMotionListener(this);
        scrollPane.setCorner(JScrollPane.LOWER_RIGHT_CORNER, windowResizeCorner);

        /* Completion window */
        window = new JPanel(new BorderLayout());
        window.add(scrollPane, BorderLayout.CENTER);
        window.setSize(new Dimension(WINDOW_WIDTH, WINDOW_HEIGHT));
        window.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));

        /* Overide Listener */
        listUI.getInputMap().clear();
        scrollPane.getInputMap().clear();
        /* Item selection is available using the keyboard */
        listUI.addKeyListener(this);
        /* Item selection is available using the mouse */
        listUI.addFocusListener(this);
        listUI.addMouseListener(this);

        getTextComponent().add(window);

        setVisible(false);
    }

    /**
     * Make the completion window visible and update its content
     * @param list list of items matching completion
     * @param location position of the top left corner of the window
     * @see com.artenum.rosetta.interfaces.ui.CompletionWindow#show(java.util.List, java.awt.Point)
     */
    @Override
    public abstract void show(List<CompletionItem> list, Point location);

    /**
     * Get the character string selected by the user as a completion
     * @return the character string
     * @see com.artenum.rosetta.interfaces.ui.CompletionWindow#getCompletionResult()
     */
    @Override
    public String getCompletionResult() {
        return ((CompletionItem) listUI.getSelectedValue()).getReturnValue();
    }

    /**
     * Get type of the character string selected by the user as a completion
     * @return the character string
     */

    public String getCompletionResultType() {
        return ((CompletionItem) listUI.getSelectedValue()).getType();
    }

    /**
     * Get the visibility status of the completion window
     * @return true if the window is visible (false else)
     */
    public boolean isVisible() {
        return window.isVisible();
    }

    /**
     * Set the visibility status of the completion window
     * @param status true if the window is visible (false else)
     */
    public void setVisible(boolean status) {
        window.setVisible(status);
        if (!status) {
            focusOutComponent.grabFocus();
        }
    }
    /**
     * List model which allow filter on completion item
     * In Scilab, filter and sort are done by Scilab and have not to be done here
     */
    protected static class CompletionItemListModel extends AbstractListModel {
        private static final long serialVersionUID = 1L;
        private List<CompletionItem> data;
        private String filter;

        /**
         * Default constructor
         */
        public CompletionItemListModel() {
            data = new ArrayList<CompletionItem>();
        }

        /**
         * Get element in the data list
         * @param index the index of the element to get
         * @return the corresponding element
         * @see javax.swing.ListModel#getElementAt(int)
         */
        @Override
        public Object getElementAt(int index) {
            /* Filtering is done by Scilab */
            return data.get(index);
        }

        /**
         * Get the number of entries in the list
         * @return the size
         * @see javax.swing.ListModel#getSize()
         */
        @Override
        public int getSize() {
            /* Filtering is done by Scilab */
            return data.size();
        }

        /**
         * Set the filter to apply to the dictionary
         * @param filterToSet the filter to set
         */
        public void setFilter(String filterToSet) {

            if ((filterToSet == null) || ((filterToSet != null) && (filterToSet.length() == 0))) {
                filter = null;
            } else {
                filter = filterToSet;
            }
            fireContentsChanged(this, 0, getSize());
        }

        /**
         * Update the list items
         * @param list list of item to set to the list
         * @param list
         */
        public void updateData(List<CompletionItem> list) {
            data.clear();
            data.addAll(list);
            Collections.sort(data);
            setFilter(null);
        }
    }

    /**
     * Add the completed word in the TextComponent
     * @param position where to append the completed word
     */
    public void addCompletedWord(int position) {
        String currentLine = inputParsingManager.getCommandLine();
        String lineBeforeCaret = currentLine.substring(0, position);
        String lineAfterCaret = currentLine.substring(position);

        String stringToAdd = getCompletionResult();
        String stringToAddType = getCompletionResultType();

        boolean typeStringIsFile = false;

        if (stringToAddType.equals(Messages.gettext("File")) || stringToAddType.equals(Messages.gettext("Directory"))) {
            typeStringIsFile = true;
        }

        String newLine = Completion.completelineforjava(lineBeforeCaret, stringToAdd, typeStringIsFile, lineAfterCaret);

        inputParsingManager.reset();
        inputParsingManager.append(newLine);
    }

    /**
     * Add the completed word in the TextComponent
     * @param stringToAdd the string to add
     * @param position where to append the completed word
     */
    public void addCompletedWord(String stringToAdd, int position) {
        String currentLine = inputParsingManager.getCommandLine();
        String lineBeforeCaret = currentLine.substring(0, position);
        String lineAfterCaret = currentLine.substring(position);

        String newLine = Completion.completelineforjava(lineBeforeCaret, stringToAdd, false, lineAfterCaret);

        inputParsingManager.reset();
        inputParsingManager.append(newLine);
    }

    /**
     * Management of the key typing for the filtering
     * @param e event
     */
    @Override
    public void keyPressed(KeyEvent e) {
        if (e.getKeyCode() == KeyEvent.VK_ENTER) {
            /* The user validates an entry in the list */

            /* Add text to the input command view */
            if (listUI.getModel().getSize() != 0) {
                addCompletedWord(currentCaretPosition);
            }

            /* Hide the completion window and give the focus to the console */
            setVisible(false);
            focusOutComponent.grabFocus();
        } else if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
            /* The user want to exit from completion mode */

            /* Hide the completion window and give the focus to the console */
            setVisible(false);
            focusOutComponent.grabFocus();
        } else if (e.getKeyCode() == KeyEvent.VK_DOWN) {
            /* The user want to select next item */

            if (model.getSize() > 0) {
                listUI.setSelectedIndex((listUI.getSelectedIndex()) % model.getSize());
            }
        } else if (e.getKeyCode() == KeyEvent.VK_UP) {
            /* The user want to select previous item */

            if (model.getSize() > 0) {
                listUI.setSelectedIndex((model.getSize() + listUI.getSelectedIndex()) % model.getSize());
            }
        } else if (e.getKeyCode() == KeyEvent.VK_BACK_SPACE) {
            /* Delete a character in Scilab input command view */
            if (inputParsingManager.getPartLevel(inputParsingManager.getCompletionLevel()) != null) {
                // Remove a key in input command line
                inputParsingManager.backspace();

                // Display new completion list
                Point location = inputParsingManager.getWindowCompletionLocation();
                this.show(getCompletionItems(), location);
                listUI.setSelectedIndex(0);
            } else {
                /* Hide the completion window and give the focus to the console */
                setVisible(false);
                focusOutComponent.grabFocus();
            }
        } else if (e.getKeyCode() != KeyEvent.VK_TAB) { // IGNORE TAB KEYS BECAUSE ALREADY IN COMPLETION MODE
            // Add a character in Scilab input command view
            if (e.getKeyChar() != KeyEvent.CHAR_UNDEFINED) {
                // Add a key in input command line
                inputParsingManager.append(String.valueOf(e.getKeyChar()));

                // Display new completion list
                Point location = inputParsingManager.getWindowCompletionLocation();
                this.show(getCompletionItems(), location);
                listUI.setSelectedIndex(0);
            }
        }
    }

    /**
     * What to do when a key is released ?
     * @param e event
     */
    @Override
    public void keyReleased(KeyEvent e) {
    }

    /**
     * What to do when a key is typed ?
     * @param e event
     */
    @Override
    public void keyTyped(KeyEvent e) {
    }

    /**
     * What to do when the completion window gets the focus ?
     * @param e event
     */
    @Override
    public void focusGained(FocusEvent e) {
    }

    /**
     * To support the auto hide when focus is lost
     * @param e event
     */
    @Override
    public void focusLost(FocusEvent e) {
        setVisible(false);
    }

    /**
     * To support the completion window resize
     * @param e event
     */
    @Override
    public void mouseDragged(MouseEvent e) {
        Point origine = window.getLocationOnScreen();
        Point destination = ((Component) e.getSource()).getLocationOnScreen();
        destination.x += e.getX();
        destination.y += e.getY();
        destination.translate(-origine.x, -origine.y);
        window.setSize(destination.x, destination.y);
    }

    /**
     * What to do when the mouse is moved over the completion window ?
     * @param e event
     */
    @Override
    public void mouseMoved(MouseEvent e) {
    }

    /**
     * What to do when a mouse button is clicked over the completion window ?
     * @param e event
     */
    @Override
    public void mouseClicked(MouseEvent e) {
        if (model.getSize() > 0) {
            /* Select the list item under the mouse */
            listUI.setSelectedIndex((listUI.getSelectedIndex()) % model.getSize());

            if (e.getClickCount() >= 2) { /* Double click = the user validates the item */
                addCompletedWord(currentCaretPosition);

                /* Hide the completion window and give the focus to the console */
                setVisible(false);
                focusOutComponent.grabFocus();
            }
        }
    }

    /**
     * What to do when the mouse enters the completion window ?
     * @param e event
     */
    @Override
    public void mouseEntered(MouseEvent e) {
    }

    /**
     * What to do when the mouse exits over the completion window ?
     * @param e event
     */
    @Override
    public void mouseExited(MouseEvent e) {
    }

    /**
     * What to do when a mouse button mouse is pressed over the completion window ?
     * @param e event
     */
    @Override
    public void mousePressed(MouseEvent e) {
    }

    /**
     * What to do when a mouse button mouse is released over the completion window ?
     * @param e event
     */
    @Override
    public void mouseReleased(MouseEvent e) {
        window.validate();
    }
}
