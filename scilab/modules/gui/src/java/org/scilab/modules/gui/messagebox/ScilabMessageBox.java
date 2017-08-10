/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
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

package org.scilab.modules.gui.messagebox;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.uielement.ScilabUIElement;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.UIElementMapper;

/**
 * Interface for Message Boxes used by Scilab functions such as messagebox, message, ...
 * @author Vincent COUVERT
 */
public class ScilabMessageBox extends ScilabUIElement implements MessageBox {

    private SimpleMessageBox component;

    //private int chosenButton;

    /**
     * Constructor
     */
    protected ScilabMessageBox() {
        component = ScilabBridge.createMessageBox();
        component.setElementId(UIElementMapper.add(this));
    }

    /**
     * Creates a Scilab MessageBox
     * @return the created MessageBox
     */
    public static MessageBox createMessageBox() {
        return new ScilabMessageBox();
    }

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    public SimpleMessageBox getAsSimpleMessageBox() {
        return component;
    }

    /**
     * Set the title of the file chooser
     * @param title the title to set
     */
    public void setTitle(String title) {
        ScilabBridge.setTitle(this, title);
    }

    /**
     * Set the message of the MessageBox
     * @param message the message to set
     */
    public void setMessage(String message) {
        ScilabBridge.setMessage(this, message);
    }

    /**
     * Set the message of the MessageBox (multi-line)
     * @param message the message to set
     */
    public void setMessage(String[] message) {
        ScilabBridge.setMessage(this, message);
    }

    /**
     * Get the index of the button clicked
     * @return the index of the button clicked
     */
    public int getSelectedButton() {
        return ScilabBridge.getSelectedButton(this);

    }

    /**
     * Set the indices of the default selected buttons (x_choices)
     * @param indices the indices of the default selected buttons
     */
    public void setDefaultSelectedButtons(int[] indices) {
        ScilabBridge.setDefaultSelectedButtons(this, indices);
    }

    /**
     * Get the indices of the user selected buttons (x_choices)
     * @return the indices of the user selected buttons
     */
    public int[] getUserSelectedButtons() {
        return ScilabBridge.getUserSelectedButtons(this);
    }

    /**
     * Set the labels of the buttons in the MessageBox
     * @param labels the labels of the buttons
     */
    public void setButtonsLabels(String[] labels) {
        ScilabBridge.setButtonsLabels(this, labels);
    }

    /**
     * Display this MessageBox and wait for user selection
     */
    public void displayAndWait() {
        ScilabBridge.displayAndWait(this);
    }

    /**
     * Set the initial values of the editable zone in the MessageBox
     * @param value the initial values
     */
    public void setInitialValue(String[] value) {
        ScilabBridge.setInitialValue(this, value);
    }

    /**
     * Get the value of the editable zone in the MessageBox
     * @return the value
     */
    public String[] getValue() {
        return ScilabBridge.getValue(this);
    }

    /**
     * Get the size of the value of the editable zone in the MessageBox
     * @return the value size
     */
    public int getValueSize() {
        return ScilabBridge.getValueSize(this);
    }

    /**
     * Set the items of the listbox in the MessageBox
     * @param items the items to set
     */
    public void setListBoxItems(String[] items) {
        ScilabBridge.setListBoxItems(this, items);
    }

    /**
     * Get the index of the selected item in the listbox in the MessageBox
     * @return the index
     */
    public int getSelectedItem() {
        return ScilabBridge.getSelectedItem(this);
    }

    /**
     * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
     */
    public void draw() {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#getDims()
     * @return nothing
     */
    public Size getDims() {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
     * @return nothing
     */
    public Position getPosition() {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
     * @return nothing
     */
    public boolean isVisible() {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
     * @param newSize is never used
     */
    public void setDims(Size newSize) {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
     * @param newPosition is never used
     */
    public void setPosition(Position newPosition) {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
     * @param newVisibleState is never used
     */
    public void setVisible(boolean newVisibleState) {
        throw new UnsupportedOperationException();
    }

    /**
     * Set the name of the lines labels in the editable zone in the MessageBox
     * @param labels the labels
     */
    public void setLineLabels(String[] labels) {
        ScilabBridge.setLineLabels(this, labels);
    }

    /**
     * Set the name of the columns labels in the editable zone in the MessageBox
     * @param labels the labels
     */
    public void setColumnLabels(String[] labels) {
        ScilabBridge.setColumnLabels(this, labels);
    }

    /**
     * Set the default values of a multi-value editable zone in the MessageBox
     * @param values the values
     */
    public void setDefaultInput(String[] values) {
        ScilabBridge.setDefaultInput(this, values);
    }

    /**
     * Set a MessageBox modal or not
     * @param status true to set the MessageBox modal and false else
     */
    public void setModal(boolean status) {
        ScilabBridge.setModal(this, status);
    }

    /**
     * Set the MessageBox icon
     * @param name the name of the icon
     */
    public void setIcon(String name) {
        ScilabBridge.setIcon(this, name);
    }

    /**
     * Set the component used to set the location of the MessageBox (default is Scilab Console)
     * @param parent the tab to be set as parent
     */
    public void setParentForLocation(SimpleTab parent) {
        ScilabBridge.setParentForLocation(this, parent);
    }

}
