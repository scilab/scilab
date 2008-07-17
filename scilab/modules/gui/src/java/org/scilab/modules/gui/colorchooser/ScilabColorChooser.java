/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.colorchooser;

import java.awt.Color;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.uielement.ScilabUIElement;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.UIElementMapper;

/**
 * Interface for ColorChooser for Scilab 
 * @author Vincent COUVERT
 */
public class ScilabColorChooser extends ScilabUIElement implements ColorChooser {

	private SimpleColorChooser component;
	
	/**
	 * Constructor
	 * @param color the default Color 
	 */
	protected ScilabColorChooser(Color color) {
		component = ScilabBridge.createColorChooser(color);
		component.setElementId(UIElementMapper.add(this));
	}

	/**
	 * Creates a Scilab ColorChooser
	 * @return the created ColorChooser
	 */
	public static ColorChooser createColorChooser() {
		return createColorChooser(null);
	}

	/**
	 * Creates a Scilab ColorChooser
	 * @param color the default Color 
	 * @return the created ColorChooser
	 */
	public static ColorChooser createColorChooser(Color color) {
		return new ScilabColorChooser(color);
	}

	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	public SimpleColorChooser getAsSimpleColorChooser() {
		return component;
	}

	/** 
	 * Set the title of the ColorChooser 
	 * @param title the title to set
	 */
	public void setTitle(String title) {
		ScilabBridge.setTitle(this, title);
	}

	/** 
	 * Retrieve the selected Color, or null 
	 * @return the selected Color
	 */
	public Color getSelectedColor() {
		return ScilabBridge.getSelectedColor(this);
	}

	/** 
	 * Set the default color 
	 * @param color the default color
	 */
	public void setDefaultColor(Color color) {
		ScilabBridge.setDefaultColor(this, color);
	}
	
	/**
	 * Display the font chooser and wait for a user input
	 */
	public void displayAndWait() {
		ScilabBridge.displayAndWait(this);
	}

	/**
	 * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implement org.scilab.modules.gui.UIElement#getDims()
	 * @return nothing
	 */
	public Size getDims() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
	 * @return nothing
	 */
	public Position getPosition() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
	 * @return nothing
	 */
	public boolean isVisible() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
	 * @param newSize is never used
	 */
	public void setDims(Size newSize) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
	 * @param newPosition is never used
	 */
	public void setPosition(Position newPosition) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
	 * @param newVisibleState is never used
	 */
	public void setVisible(boolean newVisibleState) {
		throw new UnsupportedOperationException();
	}
	
}
