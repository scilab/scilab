/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Pierre GRADIT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.preferences.Component;

import java.awt.Dimension;
import java.util.HashMap;
import java.util.Map;

import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XChooser;
import org.scilab.modules.preferences.XCommonManager;
import org.scilab.modules.preferences.XConfigManager;
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
    private Map<String, String> map;

    /** Define the set of actuators.
    *
    * @return array of actuator names.
    */
    public final String [] actuators() {
        String [] actuators = {"enable"};
        return actuators;
    }

    /** Constructor.
    *
    * @param peer : associated view DOM node.
    */
    public Select(final Node peer) {
        super();
        //setMinimumSize(new Dimension(200,30));
        //setPreferredSize(new Dimension(200,30));
        NodeList nodelist = peer.getChildNodes();
        int select = 0;
        int index = 0;
        map = new HashMap<String, String>();
        for (int i = 0; i < nodelist.getLength(); i++) {
            Node node = nodelist.item(i);
            if (node.getNodeName().equals("option")) {
                String value = XConfigManager.getAttribute(node, "value");
                String key = XConfigManager.getAttribute(node, "key");
                String selected = XConfigManager.getAttribute(node, "selected");
                addItem(value);
                if (selected.equals("selected")) {
                    select = index;
                }
                ++index;
                if (key.equals(XCommonManager.NAV)) {
                    map.put(value, value);
                } else {
                    map.put(value, key);
                }
            }
        }

        if (select < getItemCount()) {
            setSelectedIndex(select);
        }
    }

    /** Refresh the component by the use of actuators.
    *
    * @param peer the corresponding view DOM node
    */
    public void refresh(final Node peer) {
        NodeList nodelist = peer.getChildNodes();
        int select = 0;
        int index = 0;
        for (int i = 0; i < nodelist.getLength(); i++) {
            Node node = nodelist.item(i);
            if (node.getNodeName().equals("option")) {
                String selected = XConfigManager.getAttribute(node , "selected");
                if (selected.equals("selected")) {
                    select = index;
                }
                ++index;
            }
        }
        if (select != getSelectedIndex() && select < getItemCount()) {
            setEnabled(false);
            setSelectedIndex(select);
        }
        String enable = XConfigManager.getAttribute(peer, "enable", "true");
        setEnabled(enable.equals("true"));
    }

    /** Actual response read by the listener.
    *
    * @return response read by the listener.
    */
    public final Object choose() {
        if (isEnabled()) {
            return map.get((String) getSelectedItem());
        }
        return null;
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

