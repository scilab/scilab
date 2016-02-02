/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
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
package org.scilab.modules.gui.events;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_ID__;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.utils.SciTranslator;
/*
 * This class is to manage scilab callback through seteventhandler
 * it means call a dedicated scilab function like this :
 * function my_eventhandler(windowsId, mouse X, mouse Y, mouse Button)
 */
public class ScilabEventListener implements KeyListener, MouseListener, MouseMotionListener {

    private String callback;
    private Integer windowsUID;
    private int mouseX = 0;
    private int mouseY = 0;
    private SciTranslator eventTranslator = new SciTranslator();
    private boolean freedom = true;
    private boolean inCanvas = false;
    private boolean useHandle = true;

    public ScilabEventListener(String callback, Integer windowsUID) {
        eventTranslator.setClickAction(SciTranslator.UNMANAGED);
        this.callback = callback;
        this.windowsUID	= windowsUID;
    }

    // Remove this constructor
    // once event_handler call are unified using handle
    public ScilabEventListener(String callback, Integer windowsUID, boolean useHandle) {
        eventTranslator.setClickAction(SciTranslator.UNMANAGED);
        this.callback = callback;
        this.windowsUID = windowsUID;
        this.useHandle = useHandle;
    }

    private void callScilab() {
        // @FIXME : choose to send it to scilab or to display it
        //
        if (useHandle) {
            InterpreterManagement.requestScilabExec(callback + "(getcallbackobject(" + windowsUID + ")," + mouseX + ',' + mouseY + ',' + eventTranslator.getClickAction() + ')');
        } else {
            int windowsId = (Integer) GraphicController.getController().getProperty(windowsUID, __GO_ID__);
            InterpreterManagement.requestScilabExec(callback + '(' + windowsId + ',' + mouseX + ',' + mouseY + ',' + eventTranslator.getClickAction() + ')');
        }
        //
        //System.out.println("call " + callback+'('+windowsId+','+mouseX+','+mouseY+','+eventTranslator.getClickAction()+')');
    }

    private void invokeScilab() {
        // @FIXME : choose to send it to scilab or to display it
        //
        if (useHandle) {
            InterpreterManagement.requestScilabExec(callback + "(getcallbackobject(" + windowsUID + ")," + mouseX + ',' + mouseY + ',' + eventTranslator.javaClick2Scilab() + ')');
        } else {
            int windowsId = (Integer) GraphicController.getController().getProperty(windowsUID, __GO_ID__);
            InterpreterManagement.requestScilabExec(callback + '(' + windowsId + ',' + mouseX + ',' + mouseY + ',' + eventTranslator.javaClick2Scilab() + ')');
        }
        //
        //System.out.println("invoke " + callback+'('+windowsId+','+mouseX+','+mouseY+','+eventTranslator.javaClick2Scilab()+')');
    }

    public void keyPressed(KeyEvent keyEvent) {
        if (inCanvas) {
            if (Character.isJavaIdentifierStart(keyEvent.getKeyChar())) {
                eventTranslator.setClickAction(SciTranslator.javaKey2Scilab(keyEvent.getKeyChar(), keyEvent.isControlDown()));
                callScilab();
            } else {
                int keyChar;
                if (keyEvent.isShiftDown()) {
                    keyChar = keyEvent.getKeyCode();
                } else {
                    keyChar = Character.toLowerCase(keyEvent.getKeyCode());
                    callScilab();
                }
                eventTranslator.setClickAction(SciTranslator.javaKey2Scilab(keyChar,
                                               keyEvent.isControlDown()));
                callScilab();
            }
        }
    }

    public void keyReleased(KeyEvent arg0) {
        if (inCanvas && eventTranslator.getClickAction() != SciTranslator.UNMANAGED) {
            eventTranslator.setClickAction(-eventTranslator.getClickAction());
            callScilab();
            eventTranslator.setClickAction(SciTranslator.UNMANAGED);
        }
    }

    public void keyTyped(KeyEvent arg0) {
        // Do nothing !!!
    }

    public void mouseClicked(MouseEvent arg0) {
        mouseX = arg0.getX();
        mouseY = arg0.getY();
        if (arg0.getClickCount() == 1) {
            eventTranslator.setClickAction(
                SciTranslator.javaButton2Scilab(
                    arg0.getButton(),
                    SciTranslator.CLICKED,
                    arg0.isControlDown()));
        } else {
            /* Means mouseEvent.getClickCount() >= 2 */
            eventTranslator.setClickAction(
                SciTranslator.javaButton2Scilab(
                    arg0.getButton(),
                    SciTranslator.DCLICKED,
                    arg0.isControlDown()));
            // To unlock javaClick2Scilab done in launchfilter
            synchronized (eventTranslator) {
                eventTranslator.notify();
            }
        }
    }

    public void mouseEntered(MouseEvent arg0) {
        inCanvas = true;
    }

    public void mouseExited(MouseEvent arg0) {
        inCanvas = false;
    }

    public void mousePressed(MouseEvent arg0) {
        if (this.freedom) {
            this.freedom = false;
            mouseX = arg0.getX();
            mouseY = arg0.getY();
            eventTranslator.setClickAction(
                SciTranslator.javaButton2Scilab(
                    arg0.getButton(),
                    SciTranslator.PRESSED,
                    arg0.isControlDown()));
            Thread launchMe = new Thread() {
                public void run() {
                    invokeScilab();
                    freedom = true;
                    eventTranslator.setClickAction(SciTranslator.UNMANAGED);
                }
            };
            launchMe.start();
        }
    }

    public void mouseReleased(MouseEvent arg0) {
        if (eventTranslator.getClickAction() == SciTranslator.UNMANAGED ||
                eventTranslator.getClickAction() == SciTranslator.SCIMOVED) {
            eventTranslator.setClickAction(
                SciTranslator.javaButton2Scilab(arg0.getButton(),
                                                SciTranslator.RELEASED,
                                                arg0.isControlDown()));
            mouseX = arg0.getX();
            mouseY = arg0.getY();
            callScilab();
        }
    }

    public void mouseDragged(MouseEvent arg0) {
        if (eventTranslator.getClickAction() == SciTranslator.javaButton2Scilab(MouseEvent.BUTTON1, SciTranslator.PRESSED, false)) { /* If LEFT BUTTON PRESSED */
            this.freedom = false;
            mouseX = arg0.getX();
            mouseY = arg0.getY();
            callScilab();
            freedom = true;
            eventTranslator.setClickAction(SciTranslator.SCIMOVED);
        } else {
            eventTranslator.setClickAction(SciTranslator.SCIMOVED);
            mouseX = arg0.getX();
            mouseY = arg0.getY();
            callScilab();
        }
        eventTranslator.setClickAction(SciTranslator.UNMANAGED);
    }

    public void mouseMoved(MouseEvent arg0) {
        eventTranslator.setClickAction(SciTranslator.SCIMOVED);
        mouseX = arg0.getX();
        mouseY = arg0.getY();
        callScilab();
        eventTranslator.setClickAction(SciTranslator.UNMANAGED);
    }

}
