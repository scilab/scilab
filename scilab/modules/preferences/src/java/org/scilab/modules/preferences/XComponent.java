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

import org.w3c.dom.Node;

/** XComponent implements scilab components compliant with XManagement.
 *
 * @author Pierre GRADIT
 */
public interface XComponent {

    // CAUTION : Layout information have to be set in constructor!

    /** Define the set of actuators.
     *
     * @return array of actuator names.
     */
    String [] actuators();

    /** Refresh the component by the use of actuators.
     *
     * @param peer the corresponding view DOM node
     */
    void refresh(Node peer);
}


