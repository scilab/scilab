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

import org.scilab.modules.preferences.XComponent;
import javax.swing.JScrollPane;
import org.w3c.dom.Node;
import org.scilab.modules.preferences.XConfigManager;
import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;

import javax.swing.BorderFactory;

/** Implementation of Scroll compliant with extended management.
*
* @author Pierre GRADIT
*
*/
public class Scroll extends JScrollPane implements XComponent {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -6203963304355019727L;

    /** Constructor.
    *
    * @param peer : associated view DOM node.
    * @param component : parent component.
    */
    public Scroll(final Node peer, final Component component) {
        super(component);
        insideContainer = (Container) component;
        insideContainer.setLayout(new BorderLayout());
        setMinimumSize(new Dimension(200, 200));
        XConfigManager.setDimension(this, peer);
        setBorder(BorderFactory.createEmptyBorder());
        setViewportBorder(BorderFactory.createEmptyBorder());
    }

    /** Define the set of actuators.
      *
      * @return array of actuator names.
      */
    public final String [] actuators() {
        String [] actuators = {};
        return actuators;
    }

    /** Inside panel.
    *
    */
    private Container insideContainer;

    /** Add to container except for system adds.
     *
    * @param component : associated view DOM node.
    * @param constraints : associated view DOM node.
     */
    public final void add(final Component component, final Object constraints) {
        if (component instanceof XComponent) {
            insideContainer.add(component, constraints);
            return;
        }
        super.add(component, constraints);
    }

    /** Add to container except for system adds.
     *
    * @param component : associated view DOM node.
    * @param constraints : associated view DOM node.
    * @param index : order number in layout.
     */
    public final void add(
        final Component component,
        final Object constraints,
        final int index) {
        if (component instanceof XComponent) {
            insideContainer.add(component, constraints, index);
            return;
        }
        super.add(component, constraints, index);
    }

    /** Remove from container.
     *
    * @param component : associated view DOM node.
     */
    public final void remove(final Component component) {
        insideContainer.remove(component);
    }

    /** Children consultation.
     *
     * @return container component count
     */
    public final int getXComponentCount() {
        return insideContainer.getComponentCount();
    }

    /** Children consultation.
     *
     * @param index : order number in layout.
     * @return indexed component
     */
    public final Component getXComponent(final int index) {
        return insideContainer.getComponent(index);
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
        String signature = "Scroll";

        return signature;
    }
}

