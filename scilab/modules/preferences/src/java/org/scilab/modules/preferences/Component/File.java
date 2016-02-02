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

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.JTextField;

import org.scilab.modules.preferences.XCommonManager;
import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XChooser;
import org.scilab.modules.preferences.XConfigManager;
import org.scilab.modules.gui.bridge.colorchooser.SwingScilabColorChooser;
import org.scilab.modules.gui.filechooser.Juigetfile;

import org.w3c.dom.Node;

/** Implementation of Entry compliant with extended management.
 *
 * @author Pierre GRADIT
 *
 */
public class File extends JTextField implements XComponent, XChooser, MouseListener {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -7007541669965737408L;

    /** Define the set of actuators.
     *
     * @return array of actuator names.
     */
    public final String [] actuators() {
        String [] actuators = {"enable", "href", "desc", "mask"};
        return actuators;
    }

    /** Constructor.
     *
     * @param peer : associated view DOM node.
     */
    public File(final Node peer) {
        super();
        refresh(peer);
        addMouseListener(this);
    }

    /** Refresh the component by the use of actuators.
     *
     * @param peer the corresponding view DOM node
     */
    public final void refresh(final Node peer) {
        String href   = XCommonManager.getAttribute(peer , "href");
        String mask   = XCommonManager.getAttribute(peer , "mask");
        String desc   = XCommonManager.getAttribute(peer , "desc");

        if (!href.equals(href())) {
            href(href);
        }
        if (!mask.equals(mask())) {
            mask(mask);
        }
        if (!desc.equals(desc())) {
            desc(desc);
        }
        String enable     = XConfigManager.getAttribute(peer , "enable", "true");
        setEnabled(enable.equals("true"));
    }

    String jMask, jDesc;

    /** Sensor for 'mask' attribute.
     *
     * @return the attribute value.
     */
    public final String mask() {
        return jMask;
    }

    /** Actuator for 'mask' attribute.
     *
     * @param mask : the attribute value.
     */
    public final void mask(final String mask) {
        jMask = mask;
    }

    /** Sensor for 'desc' attribute.
     *
     * @return the attribute value.
     */
    public final String desc() {
        return jDesc;
    }

    /** Actuator for 'desc' attribute.
     *
     * @param text : the attribute value.
     */
    public final void desc(final String desc) {
        jDesc = desc;
    }


    /** Sensor for 'href' attribute.
     *
     * @return the attribute value.
     */
    public final String href() {
        return getText();
    }

    /** Actuator for 'href' attribute.
     *
     * @param text : the attribute value.
     */
    public final void href(final String href) {
        setText(href);
    }

    public void mouseClicked(final MouseEvent e) {
        if (actionListener != null && isEnabled()) {
            ActionEvent transmit  = new ActionEvent(this, e.getID(), "File change", e.getWhen() + 1, e.getModifiers());
            actionListener.actionPerformed(transmit);
        }
    }

    /** Mouse listener unused callback. @param e : event*/
    public void mouseEntered(final MouseEvent e) {
    }
    /** Mouse listener unused callback. @param e : event*/
    public void mouseExited(final MouseEvent e) {
    }
    /** Mouse listener unused callback. @param e : event*/
    public void mousePressed(final MouseEvent e) {
    }
    /** Mouse listener unused callback. @param e : event*/
    public void mouseReleased(final MouseEvent e) {
    }

    /** Actual response read by the listener.
     *
     * @return response read by the listener.
     */
    public final Object choose() {
        String mask[] = {jMask};
        String desc[] = {jDesc};
        Juigetfile.uigetfile(mask, desc);
        String selection[] = Juigetfile.getSelection();
        if (selection.length > 0) {
            return selection[0];
        }
        return href();
    }

    /** Developer serialization method.
     *
     * @return equivalent signature.
     */
    public final String toString() {
        String signature = "File";
        if (!href().equals(XCommonManager.NAV)) {
            signature += " href='" + href() + "'";
        }
        if (!desc().equals(XCommonManager.NAV)) {
            signature += " desc='" + desc() + "'";
        }
        if (!mask().equals(XCommonManager.NAV)) {
            signature += " href='" + mask() + "'";
        }
        return signature;
    }

    /** Row selection management.
     *
     */
    private ActionListener actionListener = null;

    /** Registration of a single listener.
     * @param listener for the unique actionListener.
     */
    public final void addActionListener(final ActionListener listener) {
        actionListener = listener;
    }

}

