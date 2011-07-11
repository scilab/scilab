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
import javax.swing.JPanel;
import org.w3c.dom.Node;
import org.scilab.modules.gui.utils.XConfigManager;
import java.awt.BorderLayout;

/** Implementation of Panel compliant with extended management.
*
* @author Pierre GRADIT
*
*/
public class Panel extends JPanel implements XComponent {

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
    public Panel(final Node peer) {
        super();
        setLayout(new BorderLayout());
        XConfigManager.setDimension(this, peer);
        XConfigManager.drawConstructionBorders(this);
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
        String signature = "Panel";

        return signature;
    }
}
