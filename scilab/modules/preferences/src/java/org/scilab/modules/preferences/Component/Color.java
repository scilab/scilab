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

package org.scilab.modules.preferences.Component;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import org.scilab.modules.gui.bridge.colorchooser.SwingScilabColorChooser;
import org.scilab.modules.preferences.XChooser;
import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XCommonManager;
import javax.swing.JLabel;
import org.w3c.dom.Node;

//TODO import org.scilab.modules.gui.bridge.label.SwingScilabLabel;
// - text does not appear.

/** Implementation of Label compliant with extended management.
*
* @author Pierre GRADIT
* TODO http://www.java-tips.org/java-se-tips/javax.swing/how-to-use-popup-menus-in-swing-applications.html
*/
public class Color extends JLabel implements XComponent, XChooser, MouseListener {

  /**
    * 
    */
    private static final long serialVersionUID = 5598263085800128888L;

/** Define the set of actuators.
    *
    * @return array of actuator names.
    */
    public final String [] actuators() {
        String [] actuators = {"color"};
        return actuators;
    }

    /** Constructor.
    *
    * @param peer : associated view DOM node.
    */
    public Color(final Node peer) {
        super();
        setText("\u2588\u2588\u2588\u2588");
        String color = XCommonManager.getAttribute(peer , "color", "000000");
        color(color);
        setOpaque(false);
        addMouseListener((MouseListener) this);
    }

    /** Refresh the component by the use of actuators.
    *
    * @param peer the corresponding view DOM node
    */
    public final void refresh(final Node peer) {
        String color = XCommonManager.getAttribute(peer , "color", "000000");
        if (!color.equals(color())) {
        	color(color);
        }
    }

    /** Sensor for 'color' attribute.
    *
    * @return the attribute value.
    */
    public final String color() {
    	java.awt.Color color = getForeground();
        return XCommonManager.getColor(color);
    }

    /** Actuator for 'color' attribute.
    *
    * @param text : the attribute value.
    */
    public final void color(final String color) {
    	java.awt.Color jColor= XCommonManager.getColor(color);
    	setForeground(jColor);
    }

   /** Developer serialization method.
    *
    * @return equivalent signature.
    */
    public final String toString() {
        String signature = "Color";
        signature += " color='" + color() + "'";
        return signature;
    }

    /** Event management
     * 
     */
    ActionListener actionListener = null;
    /** Color chooser must be invoked on mouse click, 
     *  and post actionEvent
     */
    private SwingScilabColorChooser colorChooser;

    /** Registration of a single listener. 
     * @param listener
     */
    public void addActionListener(ActionListener listener) {
    	actionListener = listener;
    }

    /** External consultation 
     *
     */
    public String choose() {
    	java.awt.Color jColor = colorChooser.getSelectedColor();
	    return XCommonManager.getColor(jColor);
    }
    
    /** Mouse listener used callback. 
     * @param e : event
     */
    public void mouseClicked(final MouseEvent e) {
        java.awt.Color jColor = XCommonManager.getColor(color());
        ActionEvent transmit  = new ActionEvent(
             this, 
             e.getID(), 
             "Color change", 
             e.getWhen() + 1, 
             e.getModifiers());
        colorChooser = new SwingScilabColorChooser(jColor);
        colorChooser.displayAndWait();
        if (actionListener != null) {
            actionListener.actionPerformed(transmit);
        } else {
            System.out.println("----> No registered listener!");
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

}

