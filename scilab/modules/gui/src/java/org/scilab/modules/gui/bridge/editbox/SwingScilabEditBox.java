/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.editbox;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;

import javax.swing.JTextField;

import org.scilab.modules.gui.editbox.SimpleEditBox;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.ScilabAlignment;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab EditBox in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabEditBox extends JTextField implements SimpleEditBox {
	
	private static final long serialVersionUID = 1L;
	
	private CallBack callback;

	private FocusListener focusListener;
	private ActionListener actionListener;
	
	/**
	 * Constructor
	 */
	public SwingScilabEditBox() {
		super();
	}

	/**
	 * Draws a swing Scilab EditBox
	 * @see org.scilab.modules.gui.uielement.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
	}

	/**
	 * Gets the dimensions (width and height) of a swing Scilab EditBox
	 * @return the dimensions of the EditBox
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(this.getSize().width, this.getSize().height);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab EditBox
	 * @return the position of the EditBox
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	public Position getPosition() {
		return PositionConverter.javaToScilab(getLocation(), getSize(), getParent());
	}

	/**
	 * Sets the dimensions (width and height) of a swing Scilab EditBox
	 * @param newSize the dimensions we want to set to the EditBox
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		this.setSize(newSize.getWidth(), newSize.getHeight());
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab EditBox
	 * @param newPosition the position we want to set to the EditBox
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		Position javaPosition = PositionConverter.scilabToJava(newPosition, getDims(), getParent());
		setLocation(javaPosition.getX(), javaPosition.getY());
	}
	
	/**
	 * Add a callback to the EditBox
	 * @param cb the callback to set.
	 */
	public void setCallback(CallBack cb) {
		
		// Remove previous callback
		if (focusListener != null) {
			removeFocusListener(focusListener);
			removeActionListener(actionListener);
		}
		
		this.callback = cb;
		
		// Create a focus listener to call the callback action
		focusListener = new FocusListener() {
			public void focusGained(FocusEvent arg0) {
				// Do nothing
			}
			public void focusLost(FocusEvent arg0) {
				// Validates user input
				callback.actionPerformed(null);
			}
		};
		addFocusListener(focusListener);
		
		// Create n action listener to get ENTER keystrokes
		actionListener = new ActionListener() {

			public void actionPerformed(ActionEvent arg0) {
				// Transfert the focus to generate a FocusEvent
				transferFocus();
			}
			
		};
		addActionListener(actionListener);
	}

	/**
	 * Set if the EditBox is enabled or not
	 * @param status true if the EditBox is enabled
	 */
	public void setEnabled(boolean status) {
		super.setEnabled(status);
		/* (Des)Activate the callback */ 
		if (callback != null) {
			if (status) {
				addFocusListener(focusListener);
				addActionListener(actionListener);
			} else {
				removeFocusListener(focusListener);
				removeActionListener(actionListener);
			}
		}
	}

	/**
	 * Setter for MenuBar
	 * @param menuBarToAdd the MenuBar associated to the Tab.
	 */
	public void addMenuBar(MenuBar menuBarToAdd) {
		/* Unimplemented for EditBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Setter for ToolBar
	 * @param toolBarToAdd the ToolBar associated to the Tab.
	 */
	public void addToolBar(ToolBar toolBarToAdd) {
		/* Unimplemented for EditBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for MenuBar
	 * @return MenuBar: the MenuBar associated to the Tab.
	 */
	public MenuBar getMenuBar() {
		/* Unimplemented for EditBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for ToolBar
	 * @return ToolBar: the ToolBar associated to the Tab.
	 */
	public ToolBar getToolBar() {
		/* Unimplemented for EditBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Set the horizontal alignment for the EditBox text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setHorizontalAlignment(String alignment) {
		setHorizontalAlignment(ScilabAlignment.toSwingAlignment(alignment));
	}

	/**
	 * Set the vertical alignment for the EditBox text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setVerticalAlignment(String alignment) {
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Set the Relief of the EditBox
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public void setRelief(String reliefType) {
		setBorder(ScilabRelief.getBorderFromRelief(reliefType));
	}

	/**
	 * Destroy the EditBox
	 */
	public void destroy() {
		ScilabSwingUtilities.removeFromParent(this);
	}
	
	/**
	 * Setter for InfoBar
	 * @param infoBarToAdd the InfoBar associated to the EditBox.
	 */
	public void addInfoBar(TextBox infoBarToAdd) {
		/* Unimplemented for EditBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for InfoBar
	 * @return the InfoBar associated to the EditBox.
	 */
	public TextBox getInfoBar() {
		/* Unimplemented for EditBoxes */
		throw new UnsupportedOperationException();
	}

}
