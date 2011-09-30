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

import org.scilab.modules.preferences.XCommonManager;
import org.scilab.modules.preferences.XComponent;
import javax.swing.JPanel;
import org.w3c.dom.Node;
import org.scilab.modules.preferences.XConfigManager;
import java.awt.Component;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.Insets;

/** Implementation of Panel compliant with extended management.
*
* @author Pierre GRADIT
*
*/
public class Grid extends JPanel implements XComponent {

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
    public Grid(final Node peer) {
        super();
        setLayout(new GridBagLayout());
        XCommonManager.setDimension(this, peer);
        XCommonManager.drawConstructionBorders(this);
    }

    /** Translation of constraints.
    *
    */
    void setConstraints(GridBagConstraints gbc, Object constraints) {
        Node component = (Node) constraints;
        gbc.gridx      = XCommonManager.getInt(component, "gridx",      1) - 1;
        gbc.gridy      = XCommonManager.getInt(component, "gridy",      1) - 1;
        gbc.gridwidth  = XCommonManager.getInt(component, "gridwidth",  1);
        gbc.gridheight = XCommonManager.getInt(component, "gridheight", 1);
        gbc.weightx    = XCommonManager.getInt(component, "weightx",    1);
        gbc.weighty    = XCommonManager.getInt(component, "weighty"   , 1);
        gbc.ipadx      = XCommonManager.getInt(component, "ipadx",      0);
        gbc.ipady      = XCommonManager.getInt(component, "ipady"   ,   0);

        if (XConfigManager.getAttribute(component, "anchor").equals("north")) {
            gbc.anchor = GridBagConstraints.NORTH;
        }
        if (XConfigManager.getAttribute(component, "anchor").equals("west")) {
            gbc.anchor = GridBagConstraints.WEST;
        }
        if (XConfigManager.getAttribute(component, "insets").equals("large")) {
            gbc.insets = new Insets(5,5,5,5);
        }
        if (XConfigManager.getAttribute(component, "insets").equals("huge")) {
            gbc.insets = new Insets(15,15,15,15);
        }
        if (XConfigManager.getAttribute(component, "fill").equals("none")) {
            gbc.fill = GridBagConstraints.NONE;
        }
        if (XConfigManager.getAttribute(component, "fill").equals("horizontal")) {
            gbc.fill = GridBagConstraints.HORIZONTAL;
        }
        if (XConfigManager.getAttribute(component, "fill").equals("vertical")) {
            gbc.fill =  GridBagConstraints.VERTICAL;
        }
        if (XConfigManager.getAttribute(component, "fill").equals("both")) {
            gbc.fill =  GridBagConstraints.BOTH;
        }
    }

    /** Overloaded add method to embed grid layout.
    *
    */
    public void add(Component child, Object constraints, int index) {
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.fill = GridBagConstraints.HORIZONTAL;

        setConstraints(gbc, constraints);
        super.add(child, gbc);
    }

    /** Overloaded add method to embed grid layout.
    *
    */
    public void add(Component child, Object constraints) {
        int index = getComponentCount();
        add(child, constraints, index);
    }

    //TODO:
    public void remove(Component child) {
        getLayout().removeLayoutComponent(child);
        super.remove(child);
        revalidate();
        System.err.println("- - - - - REMOVE NOT IMPLEMENTABLE IN GRIDS!");
    }

    /** Refresh the component by the use of actuators.
    *
    * @param peer the corresponding view DOM node
    */
    public void refresh(final Node peer) {
        revalidate();
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

