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

package org.scilab.modules.gui.bridge.slider;

import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;

import javax.swing.JScrollBar;

import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.slider.SimpleSlider;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab Slider in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabSlider extends JScrollBar implements SimpleSlider {

	private static final long serialVersionUID = -4262320156090829309L;
	
	private static final int MIN_KNOB_SIZE = 40;

	private CallBack callback;
	
	private AdjustmentListener adjustmentListener;
	
	/**
	 * Constructor
	 */
	public SwingScilabSlider() {
		super();
		// needed to have slider working with GLCanvas
		setOpaque(true);
	}

	/**
	 * Draws a swing Scilab Slider
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
	}
	
	/**
	 * Gets the dimensions (width and height) of a swing Scilab Slider
	 * @return the dimensions of the Slider
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(super.getSize().width, super.getSize().height);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab Slider
	 * @return the position of the Slider
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	public Position getPosition() {
		return PositionConverter.javaToScilab(getLocation(), getSize(), getParent());
	}
	
	/**
	 * Sets the dimensions (width and height) of a swing Scilab Slider
	 * @param newSize the dimensions to set to the Slider
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		super.setSize(newSize.getWidth(), newSize.getHeight());
		super.doLayout(); // Usefull in case of resize
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab Slider
	 * @param newPosition the position to set to the Slider
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		Position javaPosition = PositionConverter.scilabToJava(newPosition, getDims(), getParent());
		setLocation(javaPosition.getX(), javaPosition.getY());
	}

	/**
	 * Add a callback to the Slider
	 * @param cb the callback to set.
	 */
	public void setCallback(CallBack cb) {
		/* Create a callback */
		this.callback = cb;
		
		/* Remove previous listener if exists */
		if (adjustmentListener != null) {
			removeAdjustmentListener(adjustmentListener);
		}
		
		/* Create a listener for Adjustment events */
		adjustmentListener = new AdjustmentListener() {
			public void adjustmentValueChanged(AdjustmentEvent arg0) {
				callback.actionPerformed(null);
			}
		};
		
		/* Add this listener */
		addAdjustmentListener(adjustmentListener);
	}

	/**
	 * Setter for MenuBar
	 * @param menuBarToAdd the MenuBar associated to the Tab.
	 */
	public void addMenuBar(MenuBar menuBarToAdd) {
		/* Unimplemented for CheckBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Setter for ToolBar
	 * @param toolBarToAdd the ToolBar associated to the Tab.
	 */
	public void addToolBar(ToolBar toolBarToAdd) {
		/* Unimplemented for CheckBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for MenuBar
	 * @return MenuBar: the MenuBar associated to the Tab.
	 */
	public MenuBar getMenuBar() {
		/* Unimplemented for CheckBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for ToolBar
	 * @return ToolBar: the ToolBar associated to the Tab.
	 */
	public ToolBar getToolBar() {
		/* Unimplemented for CheckBoxes */
		throw new UnsupportedOperationException();
	}

	/**
	 * Get the text of the Slider
	 * @return the text
	 * @see org.scilab.modules.gui.widget.Widget#getText()
	 */
	public String getText() {
		return this.getName();
	}

	/**
	 * Set the text of the Slider
	 * @param text the text to set to the Slider
	 * @see org.scilab.modules.gui.widget.Widget#setText(java.lang.String)
	 */
	public void setText(String text) {
		this.setName(text);
	}

	/**
	 * Set the horizontal alignment for the Slider text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setHorizontalAlignment(String alignment) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Set the vertical alignment for the Slider text
	 * @param alignment the value for the alignment (See ScilabAlignment.java)
	 */
	public void setVerticalAlignment(String alignment) {
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Set the minimum value of a Slider
	 * @param value the minimum value
	 */
	public void setMinimumValue(int value) {
		/* Remove the listener to avoid the callback to be executed */
		if (adjustmentListener != null) {
			removeAdjustmentListener(adjustmentListener);
		}
		
		setMinimum(value);	
		
		/* Put back the listener */
		if (adjustmentListener != null) {
			addAdjustmentListener(adjustmentListener);
		}
	}

	/**
	 * Set the maximum value of a Slider
	 * @param value the maximum value
	 */
	public void setMaximumValue(int value) {
		/* Remove the listener to avoid the callback to be executed */
		if (adjustmentListener != null) {
			removeAdjustmentListener(adjustmentListener);
		}

		setMaximum(value + getVisibleAmount());
		
		/* Put back the listener */
		if (adjustmentListener != null) {
			addAdjustmentListener(adjustmentListener);
		}
	}

	/**
	 * Set the Relief of the Slider
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public void setRelief(String reliefType) {
		setBorder(ScilabRelief.getBorderFromRelief(reliefType));
	}

	/**
	 * Set the major tick spacing for a Slider
	 * @param space the increment value
	 */
	public void setMajorTickSpacing(int space) {
		/* Remove the listener to avoid the callback to be executed */
		if (adjustmentListener != null) {
			removeAdjustmentListener(adjustmentListener);
		}

		setBlockIncrement(space);
		int oldMax = getMaximum() - getVisibleAmount();
		setVisibleAmount(Math.max(space, MIN_KNOB_SIZE));
		setMaximum(oldMax + getVisibleAmount());
		
		/* Put back the listener */
		if (adjustmentListener != null) {
			addAdjustmentListener(adjustmentListener);
		}
	}

	/**
	 * Set the minor tick spacing for a Slider
	 * @param space the increment value
	 */
	public void setMinorTickSpacing(int space) {
		/* Remove the listener to avoid the callback to be executed */
		if (adjustmentListener != null) {
			removeAdjustmentListener(adjustmentListener);
		}

		setUnitIncrement(space);

		/* Put back the listener */
		if (adjustmentListener != null) {
			addAdjustmentListener(adjustmentListener);
		}
	}
	
	/**
	 * Set the slider orientation to vertical
	 */
	public void setVertical() {
		setOrientation(JScrollBar.VERTICAL);
	}

	/**
	 * Set the slider orientation to horizontal
	 */
	public void setHorizontal() {
		setOrientation(JScrollBar.HORIZONTAL);
	}

	/**
	 * Destroy the Slider
	 */
	public void destroy() {
		ScilabSwingUtilities.removeFromParent(this);
	}
	
	/**
	 * Setter for InfoBar
	 * @param infoBarToAdd the InfoBar associated to the Slider.
	 */
	public void addInfoBar(TextBox infoBarToAdd) {
		/* Unimplemented for Sliders */
		throw new UnsupportedOperationException();
	}

	/**
	 * Getter for InfoBar
	 * @return the InfoBar associated to the Slider.
	 */
	public TextBox getInfoBar() {
		/* Unimplemented for Sliders */
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Set the current value of the Slider
	 * @param value the new value
	 */
	public void setUserValue(int value) {
		/* Remove the listener to avoid the callback to be executed */
		if (adjustmentListener != null) {
			removeAdjustmentListener(adjustmentListener);
		}

		super.setValue(value);
		
		/* Put back the listener */
		if (adjustmentListener != null) {
			addAdjustmentListener(adjustmentListener);
		}
	}

}
