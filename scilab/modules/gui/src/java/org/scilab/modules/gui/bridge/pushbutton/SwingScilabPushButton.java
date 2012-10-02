/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.pushbutton;

import java.awt.Dimension;
import java.awt.Font;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;

import org.scilab.modules.console.utils.ScilabSpecialTextUtilities;
import org.scilab.modules.gui.SwingViewWidget;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.pushbutton.SimplePushButton;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.ScilabAlignment;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab PushButtons in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabPushButton extends JButton implements SwingViewObject, SimplePushButton {

	private static final long serialVersionUID = 2277539556048935959L;
	
	private String uid;

	private CommonCallBack callback;
	private String text = "";
	private boolean isLaTeX;
	private int fontSize;

	/**
	 * Constructor
	 */
	public SwingScilabPushButton() {
		super();
		setFocusable(false);

		/* Avoid the L&F to erase user background settings */
		setContentAreaFilled(false);
		setOpaque(true);
		addPropertyChangeListener(ICON_CHANGED_PROPERTY, new PropertyChangeListener() {
			public void propertyChange(PropertyChangeEvent evt) {
				final Icon newIcon = (Icon) evt.getNewValue();
				final boolean iconEnable = newIcon != null;

				setContentAreaFilled(iconEnable);
				setOpaque(!iconEnable);
			}
		});
	}

	/**
	 * Get the base text used for the Menu
	 * @return the text
	 */
	public String getBaseText() {
		return this.text;
	}

	/**
	 * @param text to use for the menu, if it's enclosed between '$' then it's interpreted as
	 * a LaTeX string, in this case the setIcon method of this object is used.
	 */
	public void setText(String text) {
		this.text = text;
		isLaTeX = ScilabSpecialTextUtilities.setText(this, text);
		if (isLaTeX) {
			super.setText("");
		} else {
			super.setText(text);
		}
	}

	/**
	 * @param font to use in this textPane
	 */
	public void setFont(Font font) {
		super.setFont(font);
		if (font.getSize() != fontSize && isLaTeX) {
			ScilabSpecialTextUtilities.setText(this, this.text);
			fontSize = font.getSize();
		}
	}

	/**
	 * Draws a swing Scilab PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#draw()
	 */
	public void draw() {
		setVisible(true);
		doLayout();
	}

	/**
	 * Gets the dimensions (width and height) of a swing Scilab PushButton
	 * @return the dimensions of the PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(super.getSize().width, super.getSize().height);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab PushButton
	 * @return the position of the PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	public Position getPosition() {
		return PositionConverter.javaToScilab(getLocation(), getSize(), getParent());
	}

	/**
	 * Sets the dimensions (width and height) of a swing Scilab PushButton
	 * @param newSize the dimensions to set to the PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		setSize(newSize.getWidth(), newSize.getHeight());
		setPreferredSize(new Dimension(newSize.getWidth(), newSize.getHeight()));
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab PushButton
	 * @param newPosition the position to set to the PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		Position javaPosition = PositionConverter.scilabToJava(newPosition, getDims(), getParent());
		setLocation(javaPosition.getX(), javaPosition.getY());
	}

	/**
	 * Sets the icon of a PushButton
	 * @param filename the path to the icon image to set to the PushButton
	 */
	public void setIcon(String filename) {
		super.setIcon(new ImageIcon(filename));
	}

	/**
	 * Add a callback to the pushbutton
	 * @param callback the callback to set.
	 */
	public void setCallback(CommonCallBack callback) {
		if (this.callback != null) {
			removeActionListener(this.callback);
		}
		this.callback = callback;
		addActionListener(this.callback);
	}

	/**
	 * Set if the pushbutton is enabled or not
	 * @param status true if the pushbutton is enabled
	 */
	public void setEnabled(boolean status) {
		super.setEnabled(status);
		/* (Des)Activate the callback */
		if (callback != null) {
			if (status) {
				removeActionListener(callback); /* To be sure the callback is not added two times */
				addActionListener(callback);
			} else {
				removeActionListener(callback);
			}
		}
	}

	/**
	 * Setter for MenuBar
	 * @param menuBarToAdd the MenuBar associated to the Tab.
	 */
	public void addMenuBar(MenuBar menuBarToAdd) {
		/* Unimplemented for pushbuttons */
		throw new UnsupportedOperationException();
	}

	/**
	 * Setter for ToolBar
	 * @param toolBarToAdd the ToolBar associated to the Tab.
	 */
	public void addToolBar(ToolBar toolBarToAdd) {
		/* Unimplemented for pushbuttons */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for MenuBar
	 * @return MenuBar: the MenuBar associated to the Tab.
	 */
	public MenuBar getMenuBar() {
		/* Unimplemented for pushbuttons */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for ToolBar
	 * @return ToolBar: the ToolBar associated to the Tab.
	 */
	public ToolBar getToolBar() {
		/* Unimplemented for pushbuttons */
		throw new UnsupportedOperationException();
	}

	/**
	 * Set the horizontal alignment for the PushButton text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setHorizontalAlignment(String alignment) {
		setHorizontalAlignment(ScilabAlignment.toSwingAlignment(alignment));
	}

	/**
	 * Set the vertical alignment for the PushButton text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setVerticalAlignment(String alignment) {
		setVerticalAlignment(ScilabAlignment.toSwingAlignment(alignment));
	}

	/**
	 * Set the Relief of the PushButton
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public void setRelief(String reliefType) {
		setBorder(ScilabRelief.getBorderFromRelief(reliefType));
	}

	/**
	 * Destroy the PushButton
	 */
	public void destroy() {
		ScilabSwingUtilities.removeFromParent(this);
	}

	/**
	 * Setter for InfoBar
	 * @param infoBarToAdd the InfoBar associated to the PushButton.
	 */
	public void addInfoBar(TextBox infoBarToAdd) {
		/* Unimplemented for PushButtons */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for InfoBar
	 * @return the InfoBar associated to the PushButton.
	 */
	public TextBox getInfoBar() {
		/* Unimplemented for PushButtons */
		throw new UnsupportedOperationException();
	}

	/**
	 * Set the UID
	 * @param id the UID
	 */
    public void setId(String id) {
    	uid = id;
    }
	
	/**
	 * Get the UID
	 * @return the UID
	 */
    public String getId() {
    	return uid;
    }

    /**
	 * Generic update method
	 * @param property property name
	 * @param value property value
	 */
	public void update(int property, Object value) {
		SwingViewWidget.update(this, property, value);
	}
}
