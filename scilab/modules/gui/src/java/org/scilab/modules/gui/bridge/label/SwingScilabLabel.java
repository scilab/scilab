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

import java.awt.Font;
import java.lang.reflect.InvocationTargetException;

import javax.swing.JLabel;
import javax.swing.SwingUtilities;

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
public class SwingScilabLabel extends JLabel implements SimpleLabel {
	
	private static final long serialVersionUID = 7177323379068859441L;

	/**
	 * Constructor
	 */
	public SwingScilabLabel() {
		super();
		setOpaque(true);
	}
	
	/**
	 * Apply a new font for the label.
	 * @param font new font to use.
	 */
	public void setFont(Font font) {
		final Font fontF = font;
		try {
			SwingUtilities.invokeAndWait(new Runnable() {
				public void run() {
					superSetFont(fontF);
				}
			});
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * Just call super.setFont
	 * @param font cool font
	 */
	private void superSetFont(Font font) {
		super.setFont(font);
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
	 * Gets the dimensions (width and height) of a swing Scilab element
	 * @return the dimensions of the element
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(super.getSize().width, super.getSize().height);
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
		super.setSize(newSize.getWidth(), newSize.getHeight());
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
		setHorizontalAlignment(ScilabAlignment.toSwingAlignment(alignment));
	}

	/**
	 * Set the vertical alignment for the Label text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setVerticalAlignment(String alignment) {
		setVerticalAlignment(ScilabAlignment.toSwingAlignment(alignment));
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

}
