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

package org.scilab.modules.preferences.Component;

import javax.swing.JButton;

import org.w3c.dom.Node;

import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.preferences.XCommonManager;
import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XConfigManager;

/** Implementation of Button compliant with extended management.
 *
 * @author Pierre GRADIT
 *
 */
//public class Button extends JButton implements XComponent {
public class Button extends JButton implements XComponent {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -3412653691044553310L;

    
    private String cachedStatus = "enabled";
    
    /** Define the set of actuators.
    *
    * @return array of actuator names.
    */
    public final String [] actuators() {
        String [] actuators = {"enable", "text"};
        return actuators;
    }

    /** Constructor.
     *
     * @param peer : associated view DOM node.
     */
    public Button(final Node peer) {
        super();
	setRequestFocusEnabled(true);
        setFocusable(true);

        refresh(peer);
    }

    /** Refresh the component by the use of actuators.
    *
    * @param peer the corresponding view DOM node
    */
    public final void refresh(final Node peer) {
        String text = XCommonManager.getAttribute(peer, "text", XCommonManager.NAV);
        if (!text.equals(text())) {
            text(text);
        }

        String enable = XConfigManager.getAttribute(peer , "enable", "true");
        setEnabled(enable.equals("true"));
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


