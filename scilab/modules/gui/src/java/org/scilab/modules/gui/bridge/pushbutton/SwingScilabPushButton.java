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

package org.scilab.modules.gui.bridge.pushbutton;

import java.awt.Dimension;
import java.awt.Font;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;

import org.scilab.modules.gui.events.callback.CallBack;
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
import org.scilab.modules.console.utils.ScilabSpecialTextUtilities;

/**
 * Swing implementation for Scilab PushButtons in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabPushButton extends JButton implements SimplePushButton {

	private static final long serialVersionUID = 2277539556048935959L;

	private CallBack callback;
        private String expression;
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
			@Override
			public void propertyChange(PropertyChangeEvent evt) {
				final Icon newIcon = (Icon) evt.getNewValue();
				final boolean iconEnable = newIcon != null;

				setContentAreaFilled(iconEnable);
				setOpaque(!iconEnable);
			}
		});
	}

        /**
         * @param text to use for the menu, if it's enclosed between '$' then it's interpreted as
         * a LaTeX string, in this case the setIcon method of this object is used.
         */
        public void setText(String text) {
            if (ScilabSpecialTextUtilities.setText(this, text)) {
                expression = text;
            } else {
                super.setText(text);
            }
        }

        /**
         * @param font to use in this textPane
         */
        public void setFont(Font font) {
            super.setFont(font);
            if (font.getSize() != fontSize && expression != null) {
                ScilabSpecialTextUtilities.setText(this, expression);
                fontSize = font.getSize();
            }
        }

	/**
	 * Draws a swing Scilab PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#draw()
	 */
	@Override
	public void draw() {
		setVisible(true);
		doLayout();
	}

	/**
	 * Gets the dimensions (width and height) of a swing Scilab PushButton
	 * @return the dimensions of the PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims()
	 */
	@Override
	public Size getDims() {
		return new Size(super.getSize().width, super.getSize().height);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab PushButton
	 * @return the position of the PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	@Override
	public Position getPosition() {
		return PositionConverter.javaToScilab(getLocation(), getSize(), getParent());
	}

	/**
	 * Sets the dimensions (width and height) of a swing Scilab PushButton
	 * @param newSize the dimensions to set to the PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	@Override
	public void setDims(Size newSize) {
		setSize(newSize.getWidth(), newSize.getHeight());
		setPreferredSize(new Dimension(newSize.getWidth(), newSize.getHeight()));
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab PushButton
	 * @param newPosition the position to set to the PushButton
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	@Override
	public void setPosition(Position newPosition) {
		Position javaPosition = PositionConverter.scilabToJava(newPosition, getDims(), getParent());
		setLocation(javaPosition.getX(), javaPosition.getY());
	}

	/**
	 * Sets the icon of a PushButton
	 * @param filename the path to the icon image to set to the PushButton
	 */
	@Override
	public void setIcon(String filename) {
		super.setIcon(new ImageIcon(filename));
	}

	/**
	 * Add a callback to the pushbutton
	 * @param callback the callback to set.
	 */
	@Override
	public void setCallback(CallBack callback) {
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
	@Override
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
	@Override
	public void addMenuBar(MenuBar menuBarToAdd) {
		/* Unimplemented for pushbuttons */
		throw new UnsupportedOperationException();
	}

	/**
	 * Setter for ToolBar
	 * @param toolBarToAdd the ToolBar associated to the Tab.
	 */
	@Override
	public void addToolBar(ToolBar toolBarToAdd) {
		/* Unimplemented for pushbuttons */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for MenuBar
	 * @return MenuBar: the MenuBar associated to the Tab.
	 */
	@Override
	public MenuBar getMenuBar() {
		/* Unimplemented for pushbuttons */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for ToolBar
	 * @return ToolBar: the ToolBar associated to the Tab.
	 */
	@Override
	public ToolBar getToolBar() {
		/* Unimplemented for pushbuttons */
		throw new UnsupportedOperationException();
	}

	/**
	 * Set the horizontal alignment for the PushButton text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	@Override
	public void setHorizontalAlignment(String alignment) {
		setHorizontalAlignment(ScilabAlignment.toSwingAlignment(alignment));
	}

	/**
	 * Set the vertical alignment for the PushButton text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	@Override
	public void setVerticalAlignment(String alignment) {
		setVerticalAlignment(ScilabAlignment.toSwingAlignment(alignment));
	}

	/**
	 * Set the Relief of the PushButton
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	@Override
	public void setRelief(String reliefType) {
		setBorder(ScilabRelief.getBorderFromRelief(reliefType));
	}

	/**
	 * Destroy the PushButton
	 */
	@Override
	public void destroy() {
		ScilabSwingUtilities.removeFromParent(this);
	}

	/**
	 * Setter for InfoBar
	 * @param infoBarToAdd the InfoBar associated to the PushButton.
	 */
	@Override
	public void addInfoBar(TextBox infoBarToAdd) {
		/* Unimplemented for PushButtons */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for InfoBar
	 * @return the InfoBar associated to the PushButton.
	 */
	@Override
	public TextBox getInfoBar() {
		/* Unimplemented for PushButtons */
		throw new UnsupportedOperationException();
	}

}
