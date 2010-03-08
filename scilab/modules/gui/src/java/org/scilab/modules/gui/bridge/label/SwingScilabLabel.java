/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.bridge.label;

import java.awt.Color;
import java.awt.Font;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JScrollPane;

import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.label.SimpleLabel;
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
 * Swing implementation for Scilab Labels in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabLabel extends JScrollPane implements SimpleLabel {
	
	private static final long serialVersionUID = 7177323379068859441L;
	
	private JLabel label;

	/**
	 * Constructor
	 */
	public SwingScilabLabel() {
		super();
		getViewport().add(getLabel());
		setBorder(BorderFactory.createEmptyBorder());
		setViewportBorder(BorderFactory.createEmptyBorder());
		setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
	}
	
	/**
	 * Apply a new font for the label.
	 * @param font new font to use.
	 */
	public void setFont(Font font) {
		getLabel().setFont(font);
	}
	
	/**
	 * To get the Font of the element.
	 * @return font the Font
	 */
	public Font getFont() {
		return getLabel().getFont();
	}
	
	/**
	 * To get the Foreground color of the element.
	 * @return color the Color
	 */
	public Color getForeground() {
		return getLabel().getForeground();
	}
	
	/**
	 * To set the Foreground color of the element.
	 * @param color the Color
	 */
	public void setForeground(Color color) {
		getLabel().setForeground(color);
	}
	
	/**
	 * To set the Background color of the element.
	 * @param color the Color
	 */
	public void setBackground(Color color) {
		getLabel().setBackground(color);
	}
	
	/**
	 * To get the Background color of the element.
	 * @return color the Color
	 */
	public Color getBackground() {
		return getLabel().getBackground();
	}
	
	/**
	 * Draws a swing Scilab PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
	}

	/**
	 * Sets the visibility status of an UIElement
	 * @param newVisibleState the visibility status we want to set for the UIElement
	 *                      (true if the UIElement is visible, false if not)
	 */
	public void setVisible(boolean newVisibleState) {
		super.setVisible(newVisibleState);
		getLabel().setVisible(newVisibleState);
	}
	
	/**
	 * Gets the dimensions (width and height) of a swing Scilab element
	 * @return the dimensions of the element
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(getWidth(), getHeight());
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab element
	 * @return the position of the element
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	public Position getPosition() {
		return PositionConverter.javaToScilab(getLocation(), getSize(), getParent());
	}
	
	/**
	 * Sets the dimensions (width and height) of a swing Scilab element
	 * @param newSize the dimensions to set to the element
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		setSize(newSize.getWidth(), newSize.getHeight());
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab element
	 * @param newPosition the position to set to the element
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		Position javaPosition = PositionConverter.scilabToJava(newPosition, getDims(), getParent());
		setLocation(javaPosition.getX(), javaPosition.getY());
	}

	/**
	 * Add a callback to the Label
	 * @param callback the callback to set.
	 */
	public void setCallback(CallBack callback) {
		// Nothing to do...
	}

	/**
	 * Setter for MenuBar
	 * @param menuBarToAdd the MenuBar associated to the Label.
	 */
	public void addMenuBar(MenuBar menuBarToAdd) {
		/* Unimplemented for Labels */
		throw new UnsupportedOperationException();
	}

	/**
	 * Setter for ToolBar
	 * @param toolBarToAdd the ToolBar associated to the Label.
	 */
	public void addToolBar(ToolBar toolBarToAdd) {
		/* Unimplemented for Labels */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for MenuBar
	 * @return MenuBar: the MenuBar associated to the Label.
	 */
	public MenuBar getMenuBar() {
		/* Unimplemented for Labels */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for ToolBar
	 * @return ToolBar: the ToolBar associated to the Label.
	 */
	public ToolBar getToolBar() {
		/* Unimplemented for Labels */
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Set the horizontal alignment for the Label text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setHorizontalAlignment(String alignment) {
		getLabel().setHorizontalAlignment(ScilabAlignment.toSwingAlignment(alignment));
	}

	/**
	 * Set the vertical alignment for the Label text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setVerticalAlignment(String alignment) {
		getLabel().setVerticalAlignment(ScilabAlignment.toSwingAlignment(alignment));
	}

	/**
	 * Set the Relief of the Label
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public void setRelief(String reliefType) {
		setBorder(ScilabRelief.getBorderFromRelief(reliefType));
	}

	/**
	 * Destroy the Label
	 */
	public void destroy() {
		ScilabSwingUtilities.removeFromParent(this);
	}
	
	/**
	 * Setter for InfoBar
	 * @param infoBarToAdd the InfoBar associated to the Label.
	 */
	public void addInfoBar(TextBox infoBarToAdd) {
		/* Unimplemented for Labels */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for InfoBar
	 * @return the InfoBar associated to the Label.
	 */
	public TextBox getInfoBar() {
		/* Unimplemented for Labels */
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Create/Return the label Java object
	 * @return the label
	 */
	private JLabel getLabel() {
		if (label == null) {
			label = new JLabel();
			label.setOpaque(true);
		}
		return label;
		
	}

	/**
	 * get the text displayed in the label
	 * @return the label
	 * @see org.scilab.modules.gui.text.SimpleText#getText()
	 */
	public String getText() {
		return getLabel().getText();
	}

	/**
	 * Set the text displayed in the label
	 * @param newText the text
	 * @see org.scilab.modules.gui.text.SimpleText#setText(java.lang.String)
	 */
	public void setText(String newText) {
		getLabel().setText(newText);
	}

}
