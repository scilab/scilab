/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.toolbar;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.uielement.ScilabUIElement;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.UIElementMapper;

/**
 * Class for Scilab ToolBars in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabToolBar extends ScilabUIElement implements ToolBar {

	private SimpleToolBar component;
	
	/**
	 * Constructor
	 */
	protected ScilabToolBar() {
		component = ScilabBridge.createToolBar();
		component.setElementId(UIElementMapper.add(this));
		
		//setMenuBarId(UIElementMapper.getDefaultId());
	}

	/**
	 * Creates a Scilab MenuBar object
	 * @return the created MenuBar
	 */
	public static ToolBar createToolBar() {
		return new ScilabToolBar();
	}

	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	public SimpleToolBar getAsSimpleToolBar() {
		return component;
	}
	
	/**
	 * Add a PushButton to the ToolBar.
	 * @param pushButton the PushButton to add .
	 */
	public void add(PushButton pushButton) {
		ScilabBridge.add(this, pushButton);
	}

	/**
	 * Add a separator in the toolbar
	 * @see org.scilab.modules.gui.toolbar.ToolBar#addSeparator()
	 */
	public void addSeparator() {
		ScilabBridge.addSeparator(this);
	}
	
	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
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
	 * @param newSize is not used
	 */
	public void setDims(Size newSize) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
	 * @param newPosition is not used
	 */
	public void setPosition(Position newPosition) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
	 * @param newVisibleState is not used
	 */
	public void setVisible(boolean newVisibleState) {
		throw new UnsupportedOperationException();
	}
}
