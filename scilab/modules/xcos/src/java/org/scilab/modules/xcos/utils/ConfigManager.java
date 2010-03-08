/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clément DAVID
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.utils;

import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Specify the requirements for a configuration manager.
 * 
 * NOTE: this interface is not xcos specific but linked to
 * org.scilab.modules.jvm.Scilab calls
 * 
 * @see org.scilab.modules.jvm.Scilab
 */
public interface ConfigManager {
	/**
	 * @return The component main window default position.
	 */
	Position getMainWindowPosition();
	
	/**
	 * @param position The component main window default position.
	 */
	void saveMainWindowPosition(Position position);
	
	/**
	 * @return The component main window size
	 */
	Size getMainWindowSize();
	
	/**
	 * @param size The component main window size
	 */
	void saveMainWindowSize(Size size);
}
