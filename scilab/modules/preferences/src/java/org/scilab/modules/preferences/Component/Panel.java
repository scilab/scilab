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

import java.awt.BorderLayout;
import java.awt.Dimension;

import javax.swing.JPanel;

import org.w3c.dom.Node;

import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XConfigManager;

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

    protected boolean fixedHeight = true;
    protected boolean fixedWidth = false;
    private Dimension maxDim = new Dimension(0, 0);
    private Dimension minDim = new Dimension(0, 0);

    /** Define the set of actuators.
     *
     * @return array of actuator names.
     */
    public String [] actuators() {
        return new String[] {};
    }

    /** Constructor.
     *
     * @param peer : associated view DOM node.
     */
    public Panel(final Node peer) {
        super();
        setLayout(new BorderLayout());
        XConfigManager.setDimension(this, peer);

        String fixed = XConfigManager.getAttribute(peer, "fixed-width");
        if (!(fixed.equals(XConfigManager.NAV))) {
            this.fixedWidth = fixed.equals("true");
        }

        fixed = XConfigManager.getAttribute(peer, "fixed-height");
        if (!(fixed.equals(XConfigManager.NAV))) {
            this.fixedHeight = fixed.equals("true");
        }

        //setBorder(javax.swing.BorderFactory.createLineBorder(java.awt.Color.GREEN));
    }

    /**
     * {@inheritDoc}
     */
    public Dimension getMaximumSize() {
        Dimension max = super.getMaximumSize();
        if (fixedHeight) {
            maxDim.height = getPreferredSize().height;
        } else {
            maxDim.height = max.height;
        }

        if (fixedWidth) {
            maxDim.width = getPreferredSize().width;
        } else {
            maxDim.width = max.width;
        }

        return maxDim;
    }

    /**
     * {@inheritDoc}
     */
    public Dimension getMinimumSize() {
        Dimension min = super.getMinimumSize();
        if (fixedHeight) {
            minDim.height = getPreferredSize().height;
        } else {
            minDim.height = min.height;
        }

        if (fixedWidth) {
            minDim.width = getPreferredSize().width;
        } else {
            minDim.width = min.width;
        }

        return minDim;
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
    public String toString() {
        String signature = "Panel";

        return signature;
    }
}

