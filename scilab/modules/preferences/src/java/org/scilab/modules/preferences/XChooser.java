/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Pierre GRADIT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.preferences;

import java.awt.event.ActionListener;

/** XChooser implements an XComponent giving a response.
 *
 * @author Pierre GRADIT
 *
 **/
public interface XChooser {


    /** Subscription for an action listener.
     *
     * @param actionListener : subscribing listener.
     */
    void addActionListener(ActionListener actionListener);

    /** Actual response read by the listener.
     *
     * @return response read by the listener.
     */
    Object choose();

}


