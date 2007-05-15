
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.frame;

import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.container.Container;

/**
 * Interface for Scilab frames in GUIs
 * @author Vincent COUVERT
 */
public interface Frame extends Container {

	/**
	 * We want to be able to add directly a Canvas in a Tab.
	 * @param member the member to add
	 * @return the position of the canvas in the member list.
	 */
	int addMember(Canvas member);

}
