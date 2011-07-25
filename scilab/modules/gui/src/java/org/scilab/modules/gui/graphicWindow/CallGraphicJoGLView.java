/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.gui.graphicWindow;

import org.scilab.modules.gui.SwingView;
import org.scilab.modules.jvm.LoadClassPath;

/**
 * @author Pierre Lando
 */
public final class CallGraphicJoGLView {

	private static boolean notLoaded = true;
    
	/**
	 * Constructor
	 */
	private CallGraphicJoGLView() {
		
	}
	
	/**
	 * Loads Scilab graphics library
	 */
	private static void load() {
        if (notLoaded) {
            LoadClassPath.loadOnUse("graphics");
            notLoaded = false;
        }
    }

    /**
     * Create Scilab SWING view which manages SWING components
     */
    public static void createSwingView() {
        load();
        SwingView.registerSwingView();
    }
}
