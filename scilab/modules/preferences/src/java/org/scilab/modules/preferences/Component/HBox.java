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
import javax.swing.JPanel;
import org.w3c.dom.Node;
import org.scilab.modules.preferences.XConfigManager;
import java.awt.Component;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;

/** Implementation of Panel compliant with extended management.
*
* @author Pierre GRADIT
*
*/
public class HBox extends JPanel implements XComponent {

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
    public HBox(final Node peer) {
        super();
        setLayout(new GridBagLayout());
        XConfigManager.setDimension(this, peer);
        XConfigManager.drawConstructionBorders(this);
    }

    /** Translation of constraints.
    *
    */
    void setConstraints(GridBagConstraints gbc, Object constraints) {
    }

    /** Overloaded add method to embed grid layout.
    *
    */
    public void add(Component child, Object constraints, int index) {
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridx = index;
        gbc.gridy = 0;
        gbc.weightx = 1;
        gbc.weighty = 1;
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

    /** Refresh the component by the use of actuators.
    *
    * @param peer the corresponding view DOM node
    */
    public void refresh(final Node peer) {
    }

    /** Developer serialization method.
    *
    * @return equivalent signature.
    */
    public final String toString() {
        String signature = "HBox";

        return signature;
    }
}

