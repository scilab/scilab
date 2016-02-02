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

import java.awt.Component;
import java.awt.Dimension;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.Insets;
import java.util.HashMap;

import javax.swing.JPanel;

import org.w3c.dom.Node;

import org.scilab.modules.preferences.XCommonManager;
import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XConfigManager;

/** Implementation of Panel compliant with extended management.
 *
 * @author Pierre GRADIT
 *
 */
public class Grid extends Panel implements XComponent {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = 3462302313959678932L;
    private static final HashMap<String, Integer> ANCHORS = new HashMap<String, Integer>();

    static {
        ANCHORS.put("northwest", GridBagConstraints.NORTHWEST);
        ANCHORS.put("north", GridBagConstraints.NORTH);
        ANCHORS.put("northeast", GridBagConstraints.NORTHEAST);
        ANCHORS.put("west", GridBagConstraints.WEST);
        ANCHORS.put("center", GridBagConstraints.CENTER);
        ANCHORS.put("east", GridBagConstraints.EAST);
        ANCHORS.put("southwest", GridBagConstraints.SOUTHWEST);
        ANCHORS.put("south", GridBagConstraints.SOUTH);
        ANCHORS.put("southeast", GridBagConstraints.SOUTHEAST);
        ANCHORS.put("first_line_start", GridBagConstraints.FIRST_LINE_START);
        ANCHORS.put("page_start", GridBagConstraints.PAGE_START);
        ANCHORS.put("first_line_end", GridBagConstraints.FIRST_LINE_END);
        ANCHORS.put("line_start", GridBagConstraints.LINE_START);
        ANCHORS.put("line_end", GridBagConstraints.LINE_END);
        ANCHORS.put("last_line_start", GridBagConstraints.LAST_LINE_START);
        ANCHORS.put("page_end", GridBagConstraints.PAGE_END);
        ANCHORS.put("last_line_end", GridBagConstraints.LAST_LINE_END);
        ANCHORS.put("baseline", GridBagConstraints.BASELINE);
        ANCHORS.put("baseline_leading", GridBagConstraints.BASELINE_LEADING);
        ANCHORS.put("baseline_trailing", GridBagConstraints.BASELINE_TRAILING);
        ANCHORS.put("above_baseline", GridBagConstraints.ABOVE_BASELINE);
        ANCHORS.put("above_baseline_leading", GridBagConstraints.ABOVE_BASELINE_LEADING);
        ANCHORS.put("above_baseline_trailing", GridBagConstraints.ABOVE_BASELINE_TRAILING);
        ANCHORS.put("below_baseline", GridBagConstraints.BELOW_BASELINE);
        ANCHORS.put("below_baseline_leading", GridBagConstraints.BELOW_BASELINE_LEADING);
        ANCHORS.put("below_baseline_trailing", GridBagConstraints.BELOW_BASELINE_TRAILING);
    }

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
        super(peer);
        setLayout(new GridBagLayout());
        //setBorder(javax.swing.BorderFactory.createLineBorder(java.awt.Color.RED));
    }

    /** Translation of constraints.
     *
     */
    void setConstraints(GridBagConstraints gbc, Object constraints) {
        Node component = (Node) constraints;
        gbc.gridx = XCommonManager.getInt(component, "gridx", 1) - 1;
        gbc.gridy = XCommonManager.getInt(component, "gridy", 1) - 1;
        gbc.gridwidth = XCommonManager.getInt(component, "gridwidth",  1);
        gbc.gridheight = XCommonManager.getInt(component, "gridheight", 1);
        gbc.weightx = XCommonManager.getDouble(component, "weightx", 1);
        gbc.weighty = XCommonManager.getDouble(component, "weighty", 1);
        gbc.ipadx = XCommonManager.getInt(component, "ipadx", 0);
        gbc.ipady = XCommonManager.getInt(component, "ipady", 0);

        Integer an = ANCHORS.get(XConfigManager.getAttribute(component, "anchor").toLowerCase());
        if (an != null) {
            gbc.anchor = an.intValue();
        }

        String insets = XConfigManager.getAttribute(component, "insets");
        if (insets.equals("large")) {
            gbc.insets = new Insets(5, 5, 5, 5);
        } else if (insets.equals("huge")) {
            gbc.insets = new Insets(15, 15, 15, 15);
        }

        String fill = XConfigManager.getAttribute(component, "fill");
        if (fill.equals("none")) {
            gbc.fill = GridBagConstraints.NONE;
        } else if (fill.equals("horizontal")) {
            gbc.fill = GridBagConstraints.HORIZONTAL;
        } else if (fill.equals("vertical")) {
            gbc.fill =  GridBagConstraints.VERTICAL;
        } else if (fill.equals("both")) {
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
        System.err.println("- - - - - REMOVE NOT IMPLEMENTABLE IN GRIDS!\n" + child);
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

