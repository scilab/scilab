/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import java.awt.Container;
import java.lang.reflect.InvocationTargetException;

import javax.swing.JComponent;
import javax.swing.SwingUtilities;

/**
 * Add some utilities related to swing event dispatch thread.
 * @author Jean-Baptiste Silvy
 */
public final class ScilabSwingUtilities {

	/**
	 * Should not be called
	 */
	protected ScilabSwingUtilities() {
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Remove a component from its parent.
	 * @param component component to remove
	 */
	public static void removeFromParent(JComponent component) {
		final JComponent componentF = component;
		try {
			SwingUtilities.invokeAndWait(new Runnable() {
				public void run() {
					componentF.setVisible(false);
					Container parent = componentF.getParent();
					if (parent != null) {
						parent.remove(componentF);
						parent.repaint();
					}
				}
			});
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			e.getCause().printStackTrace();
		}
	}
	
}
