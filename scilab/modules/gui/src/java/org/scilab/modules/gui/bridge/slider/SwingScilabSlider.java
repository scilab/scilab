
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.slider;

import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;

import javax.swing.JScrollBar;

import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.events.callback.ScilabCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.slider.SimpleSlider;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab Slider in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabSlider extends JScrollBar implements SimpleSlider {

	private static final long serialVersionUID = -4262320156090829309L;

	private CallBack callback;
	
	private AdjustmentListener adjustmentListener;
	
	/**
	 * Constructor
	 */
	public SwingScilabSlider() {
		super();
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
		return new Position(super.getX(), super.getY());
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
		this.setLocation(newPosition.getX(), newPosition.getY());
	}

	/**
	 * Add a callback to the Slider
	 * @param command the Scilab command to execute when the Slider is validated
	 * @param commandType the type of the command that will be executed.
	 */
	public void setCallback(String command, int commandType) {
		/* Create a callback */
		callback = ScilabCallBack.create(command, commandType);
		
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
		setMinimum(value);	
	}

	/**
	 * Set the maximum value of a Slider
	 * @param value the maximum value
	 */
	public void setMaximumValue(int value) {
		setMaximum(value);	
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
		setBlockIncrement(space);
		/* Set the size of the knob */
		getModel().setExtent(getBlockIncrement());
	}

	/**
	 * Set the minor tick spacing for a Slider
	 * @param space the increment value
	 */
	public void setMinorTickSpacing(int space) {
		setUnitIncrement(space);
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
		getParent().remove(this);
		this.setVisible(false);
	}
}
