/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Pierre GRADIT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.preferences.Component;

import javax.swing.JLabel;

import org.scilab.modules.preferences.XComponent;

import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class XCosPreview extends JLabel implements XComponent {

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
    public XCosPreview(final Node peer) {
        super();
    }

    /** Refresh the component by the use of actuators.
    *
    * @param peer the corresponding view DOM node
    */
    public void refresh(final Node peer) {
        Node root = peer.getFirstChild().getFirstChild();
        NodeList nodeList = root.getChildNodes();
        String portSize = "[";

        for (int i = 0 ; i < nodeList.getLength() ; ++i) {
            Node cur = nodeList.item(i);
            portSize = portSize + cur.getNodeName();
            portSize = portSize + "=";
            portSize = portSize + cur.getNodeValue();
            portSize = portSize + ";";

        }
        portSize = portSize + "]";
        setText(portSize);
    }

    /** Developer serialization method.
    *
    * @return equivalent signature.
    */
    public final String toString() {
        String signature = "BlockPreview";

        return signature;
    }
}