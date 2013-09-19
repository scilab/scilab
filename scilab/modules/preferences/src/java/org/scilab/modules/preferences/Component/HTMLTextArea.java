/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.preferences.Component;

import java.awt.Dimension;
import java.io.StringWriter;

import javax.swing.BorderFactory;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import org.scilab.modules.preferences.XCommonManager;
import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XConfigManager;

/**
 * Implementation of Label compliant with extended management.
 *
 * @author Calixte DENIZET
 *
 */
public class HTMLTextArea extends JTextPane implements XComponent {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -4842434795956015958L;

    /** Define the set of actuators.
     *
     * @return array of actuator names.
     */
    public final String [] actuators() {
        return new String[] {};
    }

    /** Constructor.
     *
     * @param peer : associated view DOM node.
     */
    public HTMLTextArea(final Node peer) {
        super();
        setContentType("text/html");
        setOpaque(false);
        setBorder(BorderFactory.createEmptyBorder());
        setEditable(false);
        setBackground(new java.awt.Color(0, 0, 0, 0));

        setText(dumpNode(peer));
    }

    private static String dumpNode(final Node node) {
        NodeList list = node.getChildNodes();
        Node nodeToDump = null;
        for (int i = 0; i < list.getLength(); i++) {
            Node n = list.item(i);
            if (n.getLocalName().equalsIgnoreCase("html")) {
                nodeToDump = n;
                break;
            }
        }

        if (nodeToDump != null) {
            try {
                Transformer serializer = TransformerFactory.newInstance().newTransformer();
                StringWriter writer = new StringWriter();
                serializer.transform(new DOMSource(nodeToDump), new StreamResult(writer));
                return writer.toString();
            } catch (Exception e) {
                return nodeToDump.getTextContent();
            }
        }

        return node.getTextContent();
    }

    /** Refresh the component by the use of actuators.
     *
     * @param peer the corresponding view DOM node
     */
    public void refresh(final Node peer) {
        String s = dumpNode(peer);
        if (!s.equals(getText())) {
            setText(s);
        }
    }

    /** Developer serialization method.
     *
     * @return equivalent signature.
     */
    public final String toString() {
        StringBuilder signature = new StringBuilder("TextArea");

        return signature.toString();
    }
}
