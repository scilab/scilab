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
import java.net.URL;

import javax.swing.ImageIcon;
import javax.swing.JLabel;

import org.w3c.dom.Node;

import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XConfigManager;

/**
 * Implementation of Image compliant with extended management.
 *
 * @author Calixte DENIZET
 *
 */
public class Image extends Panel implements XComponent {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = 6183280976436648612L;

    private JLabel label;
    private String url;

    /** Define the set of actuators.
    *
    * @return array of actuator names.
    */
    public String[] actuators() {
        return new String[] {"url"};
    }

    /** Constructor.
    *
    * @param peer : associated view DOM node.
    */
    public Image(final Node peer) {
        super(peer);
        label = new JLabel();
        label.setHorizontalAlignment(JLabel.CENTER);
        setLayout(new BorderLayout());
        add(label, BorderLayout.CENTER);
        setVisible(true);
        refresh(peer);
    }

    /** Refresh the component by the use of actuators.
    *
    * @param peer the corresponding view DOM node
    */
    public void refresh(final Node peer) {
        String url = XConfigManager.getAttribute(peer, "url");

        if (!url.equals(url())) {
            url(url);
        }
    }

    public String url() {
        return this.url;
    }

    public void url(String url) {
        if (!url.equals(this.url)) {
            this.url = url.replace("$SCI", System.getenv("SCI"));
            try {
                URL myurl = new URL(this.url);
                label.setIcon(new ImageIcon(myurl));
            } catch (Exception e) {
                System.err.println(e);
            }
        }
    }

    /** Developer serialization method.
    *
    * @return equivalent signature.
    */
    public final String toString() {
        return "Image url: " + url();
    }
}
