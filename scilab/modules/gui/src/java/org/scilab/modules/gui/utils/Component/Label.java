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
import org.scilab.modules.gui.utils.XConfigManager;
import javax.swing.JLabel;
import java.awt.Color;
import org.w3c.dom.Node;

/** Implementation of Label compliant with extended management.
*
* @author Pierre GRADIT
*
*/
public class Label extends JLabel implements XComponent {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -4842434795956015957L;

    /** Define the set of actuators.
    *
    * @return array of actuator names.
    */
    public final String [] actuators() {
        String [] actuators = {"text", "foreground", "background"};
        return actuators;
    }

    /** Constructor.
    *
    * @param peer : associated view DOM node.
    */
    public Label(final Node peer) {
        super();

        String text = XConfigManager.getAttribute(peer , "text");
        text(text);

        String background = XConfigManager.getAttribute(peer , "background");
        background(background);

        String foreground = XConfigManager.getAttribute(peer , "foreground");
        foreground(foreground);
    }

    /** Refresh the component by the use of actuators.
    *
    * @param peer the corresponding view DOM node
    */
    public final void refresh(final Node peer) {

        String text = XConfigManager.getAttribute(peer , "text");
        if (!text.equals(text())) {
            text(text);
        }

        String background = XConfigManager.getAttribute(peer , "background");
        if (!background.equals(foreground())) {
            background(background);
        }

        String foreground = XConfigManager.getAttribute(peer , "foreground");
        if (!foreground.equals(foreground())) {
            foreground(foreground);
        }
    }

    /** Sensor for 'text' attribute.
    *
    * @return the attribute value.
    */
    public final String text() {
        return getText();
    }

    /** Sensor for 'foreground' attribute.
    *
    * @return the attribute value.
    */
    public final String foreground() {
        if (getForeground().equals(getParent().getForeground())) {
            return XConfigManager.NAV;
        } else {
            return XConfigManager.getColor(getForeground());
        }
    }

    /** Sensor for 'background' attribute.
    *
    * @return the attribute value.
    */
    public final String background() {
        if (getForeground().equals(getParent().getBackground())) {
            return XConfigManager.NAV;
        } else {
            return XConfigManager.getColor(getBackground());
        }
    }

    /** Actuator for 'text' attribute.
    *
    * @param text : the attribute value.
    */
    public final void text(final String text) {
        setText(text);
    }

    /** Actuator for 'foreground' attribute.
    *
    * @param foreground : the attribute value.
    */
    public final void foreground(final String foreground) {
        if (foreground.equals(XConfigManager.NAV)) {
            setForeground(null);
        } else {
            Color color = XConfigManager.getColor(foreground);
            //System.err.println("foreground:" + actuator + " = " + color);
            setForeground(color);
        }
    }

    /** Actuator for 'background' attribute.
    *
    * @param background : the attribute value.
    */
    public final void background(final String background) {
        if (background.equals(XConfigManager.NAV)) {
            setOpaque(false);
            setBackground(null);
        } else {
            Color color = XConfigManager.getColor(background);
            //System.err.println("background:" + actuator + " = " + color);
            setOpaque(true);
            setBackground(color);
        }
    }

   /** Developer serialization method.
    *
    * @return equivalent signature.
    */
    public final String toString() {
        String signature = "Label";

        if (!text().equals(XConfigManager.NAV)) {
            signature += " text='" + text() + "'";
        }
        if (!background().equals(XConfigManager.NAV)) {
            signature += " background='" + background() + "'";
        }
        if (!foreground().equals(XConfigManager.NAV)) {
            signature += " foreground='" + foreground() + "'";
        }
        return signature;
    }
}
