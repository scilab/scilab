
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.canvas;

import org.scilab.modules.gui.dockable.Dockable;

/**
 * Interface for Scilab canvas in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface Canvas extends Dockable {

	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleCanvas getAsSimpleCanvas();
}
