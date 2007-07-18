
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.textbox;

import javax.swing.JTextArea;
import javax.swing.border.LineBorder;

import org.scilab.modules.gui.menubar.SimpleMenuBar;
import org.scilab.modules.gui.textbox.SimpleTextBox;
import org.scilab.modules.gui.toolbar.SimpleToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab TextBox in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabTextBox extends JTextArea implements SimpleTextBox {

	/**
	 * Constructor
	 */
	public SwingScilabTextBox() {
		super();
		setEditable(false);
	}

	/**
	 * Sets a MenuBar to an element
	 * @param newMenuBar the MenuBar to set to the element
	 */
	public void addMenuBar(SimpleMenuBar newMenuBar) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException();
	}

	/**
	 * Sets a ToolBar to an element
	 * @param newToolBar the ToolBar to set to the element
	 */
	public void addToolBar(SimpleToolBar newToolBar) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Sets the text of a Text Widget
	 * @param newText the text to set to the Text Widget
	 */
	@Override
	public void setText(String newText) {
		super.setText(newText);
	}

	/**
	 * Gets the text of a Text Widget
	 * @return the text of the Text Widget
	 */
	@Override
	public String getText() {
		return super.getText();
	}
	
	/**
	 * Draws a swing Scilab TextBox
	 * @see org.scilab.modules.gui.uielement.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
	}

	/**
	 * Gets the dimensions (width and height) of a swing Scilab TextBox
	 * @return the dimensions of the TextBox
	 * @see org.scilab.modules.gui.uielement.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(super.getSize().width, super.getSize().height);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab TextBox
	 * @return the position of the TextBox
	 * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
	 */
	public Position getPosition() {
		return new Position(super.getX(), super.getY());
	}
	
	/**
	 * Sets the dimensions (width and height) of a swing Scilab TextBox
	 * @param newSize the dimensions to set to the TextBox
	 * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		super.setSize(newSize.getWidth(), newSize.getHeight());
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab TextBox
	 * @param newPosition the position to set to the TextBox
	 * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		this.setLocation(newPosition.getX(), newPosition.getY());
	}
	
//	 TODO ============= En Developpement ==========/
	/**
	 * To set the Border color and size of the element.
	 * @param lineBorder the LineBorder
	 */
	public void setBorder(LineBorder lineBorder) {
		super.setBorder(lineBorder);
	}

	/**
	 * Add this as member (dockable element) to the Scilab container and returns its index
	 * @param container the container in which we add this
	 * @return index of this in container components
	 * @see org.scilab.modules.gui.container.Container#addMember(org.scilab.modules.gui.dockable.Dockable)
	 * @see org.scilab.modules.gui.dockable.Dockable#addAsMemberTo(org.scilab.modules.gui.container.Container)
	 */
//	public int addAsMemberTo(Container container) {
//		// delegate to the container but also adding info on how to handle me (TextBox)
//		// Interface Container must describe methode: int addMember(TextBox member);
//		return container.addMember((TextBox) this);
//	}
//	 TODO ============= FIN En Developpement ==========/
}
