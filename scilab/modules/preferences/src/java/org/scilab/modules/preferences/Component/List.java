/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JList;
import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.AbstractListModel;

import org.scilab.modules.preferences.XChooser;
import org.scilab.modules.preferences.XCommonManager;
import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XConfigManager;

import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/** Implementation of Select compliant with extended management.
 *
 * @author Calixte DENIZET
 *
 */
public class List extends Panel implements XComponent, XChooser, ListSelectionListener {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -6127289363733321915L;

    private ActionListener actionListener;
    private Model model;
    private JList list;
    private boolean dontChange;

    /** Constructor.
     *
     * @param peer : associated view DOM node.
     */
    public List(final Node peer) {
        super(peer);
        model = new Model(peer);
        list = new JList(model);
        list.getSelectionModel().setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        list.getSelectionModel().addListSelectionListener(this);
        JScrollPane scrollPane = new JScrollPane(list);
        add(scrollPane);
        list.setRequestFocusEnabled(true);
        list.setFocusable(true);

        refresh(peer);
    }

    /** Define the set of actuators.
    *
    * @return array of actuator names.
    */
    public final String [] actuators() {
        String[] actuators = {"item", "enable", "nb-visible-rows"};
        return actuators;
    }

    public void valueChanged(ListSelectionEvent e) {
        if (actionListener != null && !dontChange && !e.getValueIsAdjusting()) {
            actionListener.actionPerformed(new ActionEvent(this, 0, "List selected Value changed", System.currentTimeMillis(), 0));
        }
    }

    /** Refresh the component by the use of actuators.
     *
     * @param peer the corresponding view DOM node
     */
    public final void refresh(final Node peer) {
        model.setNodeList(peer.getChildNodes());
        String item = XCommonManager.getAttribute(peer , "item");
        if (!item.equals(item())) {
            item(item);
        }

        String nbvisible = XCommonManager.getAttribute(peer , "nb-visible-rows", "5");
        if (!nbvisible.equals(nbvisible())) {
            nbvisible(nbvisible);
        }

        String enable = XConfigManager.getAttribute(peer , "enable", "true");
        setEnabled(enable.equals("true"));
    }

    /** Actual response read by the listener.
     *
     * @return the chosen value.
     */
    public final Object choose() {
        return item();
    }

    /** Registration of a single listener.
     * @param listener for the unique actionListener.
     */
    public final void addActionListener(final ActionListener listener) {
        actionListener = listener;
    }

    /** Sensor for 'item' attribute.
     *
     * @return the attribute value.
     */
    public final String item() {
        Object value = list.getSelectedValue();
        if (value == null) {
            value = model.getElementAt(0);
            list.setSelectedValue(value, true);
        }

        return value.toString();
    }

    /** Actuator for 'item' attribute.
     *
     * @param text : the attribute value.
     */
    public final void item(final String item) {
        if (!item.equals(XCommonManager.NAV) && !item.equals("")) {
            dontChange = true;
            list.setSelectedValue(item, true);
            dontChange = false;
        }
    }

    /** Sensor for 'nb-visible-rows' attribute.
     *
     * @return the attribute value.
     */
    public final String nbvisible() {
        return Integer.toString(list.getVisibleRowCount());
    }

    /** Actuator for 'item' attribute.
     *
     * @param text : the attribute value.
     */
    public final void nbvisible(final String nbvisible) {
        if (!nbvisible.equals(XCommonManager.NAV) && !nbvisible.equals("")) {
            try {
                int nb = Integer.parseInt(nbvisible);
                list.setVisibleRowCount(nb);
            } catch (NumberFormatException e) { }
        }
    }

    /** Developer serialization method.
     *
     * @return equivalent signature.
     */
    public final String toString() {
        String signature = "List ...";
        return signature;
    }

    /**
     * Scans DOM nodes and translate it into list model.
     */
    class Model extends AbstractListModel {

        /**
         * Serialization ID.
         */
        private static final long serialVersionUID = -4786321481195930071L;

        /** Data source.
         *
         */
        private NodeList nodelist;

        public Model(Node peer) {
            setNodeList(peer.getChildNodes());
        }

        public void setNodeList(NodeList list) {
            nodelist = list;
        }

        /**
         * {@inheritDoc}
         */
        public Object getElementAt(int index) {
            int count = index;
            for (int i = 0; i < nodelist.getLength(); i++) {
                Node node = nodelist.item(i);
                if (node.getNodeName().equals("listElement")) {
                    if (count == 0) {
                        return XCommonManager.getAttribute(node, "name");
                    }
                    count--;
                }
            }
            return null;
        }

        /**
         * {@inheritDoc}
         */
        public int getSize() {
            int count = 0;
            for (int i = 0; i < nodelist.getLength(); i++) {
                Node node = nodelist.item(i);
                if (node.getNodeName().equals("listElement")) {
                    count++;
                }
            }
            return count;
        }
    }
}
