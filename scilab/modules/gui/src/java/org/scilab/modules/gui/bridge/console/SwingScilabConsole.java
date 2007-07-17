
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.console;

import java.awt.Dimension;

import javax.swing.JPanel;
import javax.swing.text.BadLocationException;

import org.scilab.modules.console.SciConsole;
import org.scilab.modules.console.SciInputCommandView;
import org.scilab.modules.console.SciPromptView;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.console.SimpleConsole;
import org.scilab.modules.gui.container.Container;
import org.scilab.modules.gui.container.SimpleContainer;
import org.scilab.modules.gui.menubar.SimpleMenuBar;
import org.scilab.modules.gui.toolbar.SimpleToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

import com.artenum.console.util.StringConstants;

/**
 * Swing implementation for Scilab Console in GUIs
 * This implementation uses JyConsole package
 * @author Vincent COUVERT
 */
public class SwingScilabConsole extends SciConsole implements SimpleConsole {

	private static final long serialVersionUID = 1L;
	
	/**
	 * Constructor
	 */
	public SwingScilabConsole() {
		super();
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
	 * Displays data in the console
	 * @param dataToDisplay the data to be displayed
	 * @see fr.scilab.console.Console#display(java.lang.String)
	 */
	public void display(String dataToDisplay) {
		this.getConfiguration().getOutputView().setCaretPositionToEnd();
		this.getConfiguration().getOutputView().append(dataToDisplay);
		updateScrollPosition();
	}

	/**
	 * Reads input data in the console
	 * @return the data entered by the user
	 * @see fr.scilab.console.Console#readLine()
	 */
	public String readLine() {
		String cmd;
		
		// Show the prompt
		this.getConfiguration().getInputCommandView().setEditable(true);
		this.getConfiguration().getPromptView().setVisible(true);
		
		// Modify the size of the input command view (the prompt was not visible when last size modification done
		if (this.getInputCommandViewSizeForced()) {

			JPanel promptView = ((JPanel) this.getConfiguration().getPromptView());
	
			int height = ((JPanel) promptView.getParent()).getPreferredSize().height;
			int width = ((JPanel) promptView.getParent()).getPreferredSize().width;
			int promptViewHeight = ((SciPromptView) promptView).getPromptUI().getHeight();
			
			/* New dimension for the input command view */
			int newHeight = height + promptViewHeight;
			Dimension newDim = null;
			
			if (newHeight > promptViewHeight) {
				/* If the input command view is bigger than the promptUI */
				/* It's height is descreased */
				newDim = new Dimension(width, newHeight);
			} else {
				/* If the input command view is smaller than the promptUI */
				/* It's height adapted to the promptUI height */
				newDim = new Dimension(width, promptViewHeight);
				this.setInputCommandViewSizeForced(false);
			}
			((JPanel) promptView.getParent()).setPreferredSize(newDim);
			((JPanel) promptView.getParent()).invalidate();
			((JPanel) promptView.getParent()).doLayout();
		}

		// Remove last line returned given by Scilab (carriage return)
		try {
			int lastEOL = this.getConfiguration().getOutputViewStyledDocument().getText(0, this.getConfiguration().getOutputViewStyledDocument().getLength()).lastIndexOf(StringConstants.NEW_LINE);
			this.getConfiguration().getOutputViewStyledDocument().remove(lastEOL - 2, this.getConfiguration().getOutputViewStyledDocument().getLength() - lastEOL + 2);
		} catch (BadLocationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		updateScrollPosition();
		
		// Gets the focus to have the caret visible
		this.getConfiguration().getInputCommandView().requestFocus();
		
		// Avoids reading of an empty buffer
// FIXME : commented next line because of compilation error		
//		((SciInputCommandView) this.getConfiguration().getInputCommandView()).setBufferProtected();
		
		// Reads the buffer
// FIXME : commented next line because of compilation error
//		cmd = ((SciInputCommandView) this.getConfiguration().getInputCommandView()).getCmdBuffer();
		
		// Gives the focus to the console to avoid having a blinking caret in the not-editable input command view
		this.requestFocus();

		// Hide the prompt
		this.getConfiguration().getInputCommandView().setEditable(false);
		this.getConfiguration().getPromptView().setVisible(false);
		
// FIXME : commented next line adn added one line because of compilation error
//		return cmd;
		return "";
	}

	/**
	 * Draw a console
	 */
	public void draw() {
		super.setVisible(true);
		super.doLayout();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab console
	 * @return the size of the console
	 */
	public Size getDims() {
		return new Size(super.getWidth(), super.getHeight());
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinates) of a Scilab console
	 * @return the position of the console
	 */
	public Position getPosition() {
		return new Position(this.getX(), this.getY());
	}

	/**
	 * Gets the visibility status of a console
	 * @return the visibility status of the console (true if the console is visible, false if not)
	 */
	public boolean isVisible() {
		return super.isVisible();
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab console
	 * @param newSize the size we want to set to the console
	 */
	public void setDims(Size newSize) {
		this.setPreferredSize(new Dimension(newSize.getWidth(), newSize.getHeight()));
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab console
	 * @param newPosition the position we want to set to the console
	 */
	public void setPosition(Position newPosition) {
		this.setLocation(newPosition.getX(), newPosition.getY());
	}

	/**
	 * Sets the visibility status of a Scilab console
	 * @param newVisibleState the visibility status we want to set to the console
	 */
	public void setVisible(boolean newVisibleState) {
		super.setVisible(newVisibleState);
	}

	/**
	 * Add this as member (dockable element) to the Scilab container and returns its index
	 * @param container the container in which we add this
	 * @return index of this in container components
	 * @see org.scilab.modules.gui.container.Container#addMember(org.scilab.modules.gui.dockable.Dockable)
	 * @see org.scilab.modules.gui.dockable.Dockable#addAsMemberTo(org.scilab.modules.gui.container.Container)
	 */
	public int addAsMemberTo(SimpleContainer container) {
		// delegate to the container but also adding info on how to handle me (Console)
		// Interface Container must describe methode: int addMember(Console member);
		return container.addMember((SimpleConsole) this);
	}
}
