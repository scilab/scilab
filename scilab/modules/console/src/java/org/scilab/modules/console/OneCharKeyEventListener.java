/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.console;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

/**
 * This key listener is used to wait for one input char when displayed data are bigger than console
 * @author Vincent COUVERT
 */
public class OneCharKeyEventListener implements KeyListener {

	private SciConsole sciConsole;

	/**
	 * Constructor
	 * @param console the console associated to this key listener
	 */
	public OneCharKeyEventListener(SciConsole console) {
		super();
		sciConsole = console;
	}

	/**
	 * What do we have to do when a key is pressed ?
	 *  - did the user press n ?
	 * @param e the event to threat
	 * @see java.awt.event.KeyListener#keyPressed(java.awt.event.KeyEvent)
	 */
	public void keyPressed(KeyEvent e) {
		/* Answer to more y or n ? */
		if (Character.toLowerCase(e.getKeyChar()) == 'n') {
			sciConsole.setUserInputValue(Integer.parseInt(Integer.toString(Character.toLowerCase(e.getKeyChar()))));
		} else {
			sciConsole.setUserInputValue(1);
		}

	}

	/**
	 * What do we have to do when a key is released ?
	 * @param e the event to threat
	 * @see java.awt.event.KeyListener#keyPressed(java.awt.event.KeyEvent)
	 */
	public void keyReleased(KeyEvent e) {
		// TODO Auto-generated method stub

	}

	/**
	 * What do we have to do when a key is typed ?
	 * @param e the event to threat
	 * @see java.awt.event.KeyListener#keyPressed(java.awt.event.KeyEvent)
	 */
	public void keyTyped(KeyEvent e) {
		// TODO Auto-generated method stub

	}

}
