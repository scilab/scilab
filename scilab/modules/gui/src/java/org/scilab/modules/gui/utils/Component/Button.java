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

import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;

import org.scilab.modules.gui.utils.XComponent;
import org.scilab.modules.gui.utils.XConfigManager;
import org.w3c.dom.Node;

/** Implementation of Button compliant with extended management.
 *
 * @author Pierre GRADIT
 *
 */
//public class Button extends JButton implements XComponent {
public class Button extends SwingScilabPushButton implements XComponent {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -3412653691044553310L;

    /** Define the set of actuators.
    *
    * @return array of actuator names.
    */
    public final String [] actuators() {
        String [] actuators = {"text"};
        return actuators;
    }

    /** Constructor.
     *
     * @param peer : associated view DOM node.
     */
    public Button(final Node peer) {
        super();
        String text = XConfigManager.getAttribute(
                peer ,
                "text",
                XConfigManager.NAV);
        setText(text);
    }

    /** Refresh the component by the use of actuators.
    *
    * @param peer the corresponding view DOM node
    */
    public final void refresh(final Node peer) {
        String text = XConfigManager.getAttribute(
               peer ,
               "text",
               XConfigManager.NAV);
        if (!text.equals(text())) {
            text(text);
        }
    }

    /** Sensor for 'text' attribute.
     *
     * @return the attribute value.
     */
    public final String text() {
        return getText();
    }

    /** Actuator for 'text' attribute.
     *
     * @param text : the attribute value.
     */
    public final void text(final String text) {
        setText(text);
    }

    /** Developer serialization method.
     *
     * @return equivalent signature.
     */
    public final String toString() {
        String signature = "Button";

        if (!text().equals(XConfigManager.NAV)) {
            signature += " text='" + text() + "'";
        }

        return signature;
    }


}


