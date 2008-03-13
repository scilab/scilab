/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.waitbar;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.uielement.ScilabUIElement;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.UIElementMapper;

/**
 * WaitBars/ProgressBars used by Scilab
 * @author Vincent COUVERT
 */
public class ScilabWaitBar extends ScilabUIElement implements WaitBar {
	
	private SimpleWaitBar component;
	
	/**
	 * Constructor
	 */
	protected ScilabWaitBar() {
		component = ScilabBridge.createWaitBar();
		component.setElementId(UIElementMapper.add(this));
	}

	/**
	 * Creates a Scilab WaitBar
	 * @return the created WaitBar
	 */
	public static WaitBar createWaitBar() {
		return new ScilabWaitBar();
	}
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	public SimpleWaitBar getAsSimpleWaitBar() {
		return component;
	}
	
	/**
	 * Set the title of the WaitBar
	 * @param title the title to set
	 */
	public void setTitle(String title) {
		ScilabBridge.setTitle(this, title);
	}
	
	/**
	 * Set the message of the WaitBar
	 * @param message the message to set
	 */
	public void setMessage(String[] message) {
		ScilabBridge.setMessage(this, message);
	}
	
	/**
	 * Set the current value of the WaitBar
	 * @param value the value to set
	 */
	public void setValue(int value) {
		ScilabBridge.setValue(this, value);
	};
	
	/**
	 * Close the WaitBar
	 */
	public void close() {
		ScilabBridge.close(this);
	}
	
	/**
	 * Indicates if the total execution time is known
	 * @param status true if the total progress time in unknown
	 */
	public void setIndeterminateMode(boolean status) {
		ScilabBridge.setIndeterminateMode(this, status);
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#getDims()
	 * @return nothing
	 */
	public Size getDims() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
	 * @return nothing
	 */
	public Position getPosition() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
	 * @return nothing
	 */
	public boolean isVisible() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
	 * @param newSize is never used
	 */
	public void setDims(Size newSize) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
	 * @param newPosition is never used
	 */
	public void setPosition(Position newPosition) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
	 * @param newVisibleState is never used
	 */
	public void setVisible(boolean newVisibleState) {
		throw new UnsupportedOperationException();
	}
}
