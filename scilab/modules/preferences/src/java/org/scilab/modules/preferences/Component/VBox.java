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

import org.scilab.modules.preferences.XComponent;

import javax.swing.BoxLayout;
import javax.swing.JComponent;
import javax.swing.JPanel;
import org.w3c.dom.Node;
import org.scilab.modules.preferences.XConfigManager;

import java.awt.Color;
import java.awt.Component;

/** Implementation of Panel compliant with extended management.
*
* @author Pierre GRADIT
*
*/
public class VBox extends Panel implements XComponent {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = 3462302313959678932L;

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
    public VBox(final Node peer) {
        super(peer);
        BoxLayout box = new BoxLayout(this, BoxLayout.PAGE_AXIS);
        setLayout(box);
        refresh(peer);
    }

    /** Overloaded add method to embed grid layout.
    *
    */
    public void add(Component child, Object constraints, int index) {
        if (child instanceof JComponent) {
	    ((JComponent) child).setAlignmentX(LEFT_ALIGNMENT);
            ((JComponent)child).setAlignmentY(TOP_ALIGNMENT);
        }
        super.add(child, constraints, index);
    }

    /** Overloaded add method to embed grid layout.
    *
    */
    public void add(Component child, Object constraints) {
        if (child instanceof JComponent) {
	    ((JComponent) child).setAlignmentX(LEFT_ALIGNMENT);
            ((JComponent) child).setAlignmentY(TOP_ALIGNMENT);
        }
        super.add(child, constraints);
    }

    /** Refresh the component by the use of actuators.
    *
    * @param peer the corresponding view DOM node
    */
    public void refresh(final Node peer) {
        String background = XConfigManager.getAttribute(peer , "background");
        if (background.equals(XConfigManager.NAV)) {
            setOpaque(false);
            setBackground(null);
        } else {
            Color color = XConfigManager.getColor(background);
            setOpaque(true);
            setBackground(color);
        }
    }

    
    /** Developer serialization method.
    *
    * @return equivalent signature.
    */
    public final String toString() {
        String signature = "VBox";

        return signature;
    }
}

