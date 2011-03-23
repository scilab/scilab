/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.action_binding.utils;

import org.scilab.modules.localization.Messages;



public final class ActionBindingMessages {

	/* Interpreter errors */
	public static final String SCILAB_SAMECOMMAND = Messages.gettext("Same command executed again");
	public static final String SCILAB_UNABLE = Messages.gettext("Unable to communicate with the interpreter");


	/** This class is a static singleton, thus it must not be instantiated */
	private ActionBindingMessages() {
	}
}
