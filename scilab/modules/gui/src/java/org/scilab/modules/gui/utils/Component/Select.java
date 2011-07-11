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
import javax.swing.JComboBox;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/** Implementation of Select compliant with extended management.
*
* @author Pierre GRADIT
*
*/
public class Select extends JComboBox implements XComponent, XChooser {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -6127289363733321914L;

    /** Define the set of actuators.
    *
    * @return array of actuator names.
    */
    public final String [] actuators() {
        String [] actuators = {""};
        return actuators;
    }

    /** Constructor.
    *
    * @param peer : associated view DOM node.
    */
    public Select(final Node peer) {
        super();
        NodeList nodelist = peer.getChildNodes();
        int select = 0;
        int index = 0;
        for (int i = 0; i < nodelist.getLength(); i++) {
            Node node = nodelist.item(i);
            if (node.getNodeName().equals("option")) {
                String value = XConfigManager.getAttribute(node , "value");
                String selected = XConfigManager.getAttribute(
                        node ,
                        "selected");
                addItem(value);
                if (selected.equals("selected")) {
                    select = index;
                }
                index += 1;
            }
        }
        setSelectedIndex(select);
    }
    /** Refresh the component by the use of actuators.
    *
    * @param peer the corresponding view DOM node
    */
    public void refresh(final Node peer) {
    }

    /** Actual response read by the listener.
    *
    * @return response read by the listener.
    */
    public final String choose() {
        return (String) getSelectedItem();
    }

    /** Developer serialization method.
    *
    * @return equivalent signature.
    */
    public final String toString() {
        String signature = "Select";
        return signature;
    }
}
