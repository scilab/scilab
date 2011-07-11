/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Pierre GRADIT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils.Component;

import org.scilab.modules.gui.utils.XComponent;
import org.scilab.modules.gui.utils.XChooser;
import org.scilab.modules.gui.utils.XConfigManager;
import javax.swing.JCheckBox;
import org.w3c.dom.Node;

/** Implementation of Checkbox compliant with extended management.
*
* @author Pierre GRADIT
*
*/
public class Checkbox extends JCheckBox implements XComponent, XChooser {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -7007541669965737408L;

    /** Define the set of actuators.
    *
    * @return array of actuator names.
    */
    public final String [] actuators() {
        String [] actuators = {};
        return actuators;
    }

    /** Constructor.
    *
    * @param peer : associated view DOM node.
    */
    public Checkbox(final Node peer) {
        super();
        refresh(peer);
    }

    /** Refresh the component by the use of actuators.
    *
    * @param peer the corresponding view DOM node
    */
    public final void refresh(final Node peer) {
        String checked  = XConfigManager.getAttribute(peer , "checked");
        boolean state =  checked.equals("checked");
        setSelected(state);
    }

    /** Actual response read by the listener.
    *
    * @return response read by the listener.
    */
    public final String choose() {
        if (isSelected()) {
            return "checked";
        }
        return "unchecked";
    }

    /** Developer serialization method.
    *
    * @return equivalent signature.
    */
    public final String toString() {
        String signature = "CHECKBOX";
        return signature;
    }
}
