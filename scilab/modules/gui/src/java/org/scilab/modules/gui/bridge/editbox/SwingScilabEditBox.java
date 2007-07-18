
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.editbox;

import javax.swing.JTextArea;
import javax.swing.border.LineBorder;

import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab EditBox in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabEditBox extends JTextArea {
	// FIXME : Must have some Interface here...
	//implements EditBoxBridge {

	
	/**
	 * Constructor
	 */
	public SwingScilabEditBox() {
		super();
	}

/*
** DO NOT ADD METHODS THAT HAVE NOTHING TO DO
** WITH WHAT WE ARE DEALING WITH !!!!!
*/

	
//	/**
//	 * Sets a MenuBar to an element
//	 * @param newMenuBar the MenuBar to set to the element
//	 */
//	public void addMenuBar(SimpleMenuBar newMenuBar) {
//		// TODO Auto-generated method stub
//	}

//	/**
//	 * Sets a ToolBar to an element
//	 * @param newToolBar the ToolBar to set to the element
//	 */
//	public void addToolBar(SimpleToolBar newToolBar) {
//		// TODO Auto-generated method stub	
//	}
	
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
		return new Position(this.getX(), this.getY());
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
//		// delegate to the container but also adding info on how to handle me (EditBox)
//		// Interface Container must describe methode: int addMember(EditBox member);
//		return container.addMember((EditBox) this);
//	}
//	 TODO ============= FIN En Developpement ==========/
	
}
