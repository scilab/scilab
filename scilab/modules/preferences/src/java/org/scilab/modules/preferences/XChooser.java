/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Pierre GRADIT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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


