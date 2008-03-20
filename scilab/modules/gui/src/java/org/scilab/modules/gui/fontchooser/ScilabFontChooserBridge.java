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

package org.scilab.modules.gui.fontchooser;

import java.awt.Font;

import org.scilab.modules.gui.bridge.fontchooser.SwingScilabFontChooser;

/**
 * Bridge dedicated to Scilab font chooser GUIs
 * @author Vincent COUVERT
 */
public class ScilabFontChooserBridge {
	
	/**
	 * Constructor
	 */
	protected ScilabFontChooserBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a new FontChooser
	 * @param font the default font
	 * @return the created FontChooser
	 */
	public static SimpleFontChooser createFontChooser(Font font) {
		return new SwingScilabFontChooser(font);
	}
	
	/** 
	 * Retrieve the selected font, or null 
	 * @param fontChooser the FontChooser
	 * @return the selected font
	 */
	public static Font getSelectedFont(FontChooser fontChooser) {
		return fontChooser.getAsSimpleFontChooser().getSelectedFont();
	}

}
