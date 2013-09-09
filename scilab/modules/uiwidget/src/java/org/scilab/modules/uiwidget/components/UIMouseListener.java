/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.uiwidget.components;

import java.awt.Component;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIWidgetException;
import org.scilab.modules.uiwidget.UIWidgetTools;
import org.scilab.modules.uiwidget.UIListener;
import org.scilab.modules.uiwidget.callback.UICallback;

/**
 * Various mouse Listeners wrappers
 */
public class UIMouseListener extends UIListener {

    public enum Type {
        CLICKED, MOVED, DRAGGED, EXITED, ENTERED, PRESSED, RELEASED, WHEEL;
    }

    private Component component;

    private MouseAdapter adapter;
    private MouseAction clicked;
    private MouseAction moved;
    private MouseAction dragged;
    private MouseAction exited;
    private MouseAction entered;
    private MouseAction pressed;
    private MouseAction released;
    private MouseAction wheelmoved;

    private boolean clickedEnable = true;
    private boolean movedEnable = true;
    private boolean draggedEnable = true;
    private boolean exitedEnable = true;
    private boolean enteredEnable = true;
    private boolean pressedEnable = true;
    private boolean releasedEnable = true;
    private boolean wheelMovedEnable = true;

    private boolean basicListenerAdded;
    private boolean motionListenerAdded;
    private boolean wheelListenerAdded;

    /**
     * {@inheritDoc}
     */
    public UIMouseListener() throws UIWidgetException {
        super(null);
    }

    /**
     * {@inheritDoc}
     */
    public UIMouseListener(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        adapter = new MouseAdapter() {
            public void mouseClicked(MouseEvent e) {
                if (clicked != null && clickedEnable) {
                    clicked.action(e);
                }
            }

            public void mouseMoved(MouseEvent e) {
                if (moved != null && movedEnable) {
                    moved.action(e);
                }
            }

            public void mouseDragged(MouseEvent e) {
                if (dragged != null && draggedEnable) {
                    dragged.action(e);
                }
            }

            public void mouseEntered(MouseEvent e) {
                if (entered != null && enteredEnable) {
                    entered.action(e);
                }
            }

            public void mouseExited(MouseEvent e) {
                if (exited != null && exitedEnable) {
                    exited.action(e);
                }
            }

            public void mousePressed(MouseEvent e) {
                if (pressed != null && pressedEnable) {
                    pressed.action(e);
                }
            }

            public void mouseReleased(MouseEvent e) {
                if (released != null && releasedEnable) {
                    released.action(e);
                }
            }

            public void mouseWheelMoved(MouseWheelEvent e) {
                if (wheelmoved != null && wheelMovedEnable) {
                    wheelmoved.action(e);
                }
            }
        };

        return adapter;
    }

    /**
     * Get the onmouseclick command
     * @return the command
     */
    public UICallback getOnmouseclick() {
        if (clicked != null) {
            return clicked.callback;
        }

        return null;
    }

    /**
     * Get the onmouseover command
     * @return the command
     */
    public UICallback getOnmouseover() {
        if (moved != null) {
            return moved.callback;
        }

        return null;
    }

    /**
     * Get the onmouseenter command
     * @return the command
     */
    public UICallback getOnmouseenter() {
        if (entered != null) {
            return entered.callback;
        }

        return null;
    }

    /**
     * Get the onmouseexit command
     * @return the command
     */
    public UICallback getOnmouseexit() {
        if (exited != null) {
            return exited.callback;
        }

        return null;
    }

    /**
     * Get the onmousepress command
     * @return the command
     */
    public UICallback getOnmousepress() {
        if (pressed != null) {
            return pressed.callback;
        }

        return null;
    }

    /**
     * Get the onmouserelease command
     * @return the command
     */
    public UICallback getOnmouserelease() {
        if (released != null) {
            return released.callback;
        }

        return null;
    }

    /**
     * Get the onmousedrag command
     * @return the command
     */
    public UICallback getOnmousedrag() {
        if (dragged != null) {
            return dragged.callback;
        }

        return null;
    }

    /**
     * Get the onmousewheel command
     * @return the command
     */
    public UICallback getOnmousewheel() {
        if (wheelmoved != null) {
            return wheelmoved.callback;
        }

        return null;
    }

    /**
     * {@inheritDoc}
     */
    public void addListenerToComponent(Component c) {
        if (this.component != c) {
            if (this.component != null) {
                this.component.removeMouseListener(adapter);
                this.component.removeMouseMotionListener(adapter);
                this.component.removeMouseWheelListener(adapter);
            }

            if (c != null) {
                if (basicListenerAdded) {
                    c.addMouseListener(adapter);
                }
                if (motionListenerAdded) {
                    c.addMouseMotionListener(adapter);
                }
                if (wheelListenerAdded) {
                    c.addMouseWheelListener(adapter);
                }
            }

            this.component = c;
        }

        if (!basicListenerAdded && component != null && (clicked != null || entered != null || exited != null || pressed != null || released != null)) {
            this.component.addMouseListener(adapter);
            basicListenerAdded = true;
        }
        if (!motionListenerAdded && component != null && (dragged != null || moved != null)) {
            this.component.addMouseMotionListener(adapter);
            motionListenerAdded = true;
        }
        if (!wheelListenerAdded && component != null && wheelmoved != null) {
            this.component.addMouseWheelListener(adapter);
            wheelListenerAdded = true;
        }
    }

    /**
     * Set the onmouseclick command
     * @param command the command
     */
    public void setOnmouseclick(String command) {
        clicked = getMouseAction(Type.CLICKED, getParent(), command);
        addListenerToComponent(component);
    }

    /**
     * Set the onmouseover command
     * @param command the command
     */
    public void setOnmouseover(String command) {
        moved = getMouseAction(Type.MOVED, getParent(), command);
        addListenerToComponent(component);
    }

    /**
     * Set the onmouseenter command
     * @param command the command
     */
    public void setOnmouseenter(String command) {
        entered = getMouseAction(Type.ENTERED, getParent(), command);
        addListenerToComponent(component);
    }

    /**
     * Set the onmouseexit command
     * @param command the command
     */
    public void setOnmouseexit(String command) {
        exited = getMouseAction(Type.EXITED, getParent(), command);
        addListenerToComponent(component);
    }

    /**
     * Set the onmousepress command
     * @param command the command
     */
    public void setOnmousepress(String command) {
        pressed = getMouseAction(Type.PRESSED, getParent(), command);
        addListenerToComponent(component);
    }

    /**
     * Set the onmouserelease command
     * @param command the command
     */
    public void setOnmouserelease(String command) {
        released = getMouseAction(Type.RELEASED, getParent(), command);
        addListenerToComponent(component);
    }

    /**
     * Set the onmousewheel command
     * @param command the command
     */
    public void setOnmousewheel(String command) {
        wheelmoved = getMouseAction(Type.WHEEL, getParent(), command);
        addListenerToComponent(component);
    }

    /**
     * Set the onmousedrag command
     * @param command the command
     */
    public void setOnmousedrag(String command) {
        dragged = getMouseAction(Type.DRAGGED, getParent(), command);
        addListenerToComponent(component);
    }

    /**
     * Enable onmouseclick
     * @param b true to enable
     */
    public void setOnmouseclickEnable(boolean b) {
        clickedEnable = b;
    }

    /**
     * Check if onmouseclick is enabled
     * @return true if enabled
     */
    public boolean getOnmouseclickEnable() {
        return clickedEnable;
    }

    /**
     * Enable onmouseover
     * @param b true to enable
     */
    public void setOnmouseoverEnable(boolean b) {
        movedEnable = b;
    }

    /**
     * Check if onmouseover is enabled
     * @return true if enabled
     */
    public boolean getOnmouseoverEnable() {
        return movedEnable;
    }

    /**
     * Enable onmouseenter
     * @param b true to enable
     */
    public void setOnmouseenterEnable(boolean b) {
        enteredEnable = b;
    }

    /**
     * Check if onmouseenter is enabled
     * @return true if enabled
     */
    public boolean getOnmouseenterEnable() {
        return enteredEnable;
    }

    /**
     * Enable onmouseexit
     * @param b true to enable
     */
    public void setOnmouseexitEnable(boolean b) {
        exitedEnable = b;
    }

    /**
     * Check if onmouseexit is enabled
     * @return true if enabled
     */
    public boolean getOnmouseexitEnable() {
        return exitedEnable;
    }

    /**
     * Enable onmousepress
     * @param b true to enable
     */
    public void setOnmousepressEnable(boolean b) {
        pressedEnable = b;
    }

    /**
     * Check if onmousepress is enabled
     * @return true if enabled
     */
    public boolean getOnmousepressEnable() {
        return pressedEnable;
    }

    /**
     * Enable onmouserelease
     * @param b true to enable
     */
    public void setOnmousereleaseEnable(boolean b) {
        releasedEnable = b;
    }

    /**
     * Check if onmouserelease is enabled
     * @return true if enabled
     */
    public boolean getOnmousereleaseEnable() {
        return releasedEnable;
    }

    /**
     * Enable onmousewheel
     * @param b true to enable
     */
    public void setOnmousewheelEnable(boolean b) {
        wheelMovedEnable = b;
    }

    /**
     * Check if onmousewheel is enabled
     * @return true if enabled
     */
    public boolean getOnmousewheelEnable() {
        return wheelMovedEnable;
    }

    /**
     * Enable onmousedrag
     * @param b true to enable
     */
    public void setOnmousedragEnable(boolean b) {
        draggedEnable = b;
    }

    /**
     * Check if onmousedrag is enabled
     * @return true if enabled
     */
    public boolean getOnmousedragEnable() {
        return draggedEnable;
    }

    /**
     * Get a FocusAdapter corresponding to the event type
     * @param type the event type
     * @param uicomp the component where to put the listener
     * @param command the command
     * @return the listener
     */
    public static MouseAdapter getMouseAdapter(Type type, UIComponent uicomp, String command) {
        final MouseAction action = getMouseAction(type, uicomp, command);

        switch (type) {
            case CLICKED:
                return new MouseAdapter() {
                    public void mouseClicked(MouseEvent e) {
                        action.action(e);
                    }
                };
            case MOVED:
                return new MouseAdapter() {
                    public void mouseMoved(MouseEvent e) {
                        action.action(e);
                    }
                };
            case DRAGGED:
                return new MouseAdapter() {
                    public void mouseDragged(MouseEvent e) {
                        action.action(e);
                    }
                };
            case EXITED:
                return new MouseAdapter() {
                    public void mouseExited(MouseEvent e) {
                        action.action(e);
                    }
                };
            case ENTERED:
                return new MouseAdapter() {
                    public void mouseEntered(MouseEvent e) {
                        action.action(e);
                    }
                };
            case PRESSED:
                return new MouseAdapter() {
                    public void mousePressed(MouseEvent e) {
                        action.action(e);
                    }
                };
            case RELEASED:
                return new MouseAdapter() {
                    public void mouseReleased(MouseEvent e) {
                        action.action(e);
                    }
                };
            case WHEEL:
                return new MouseAdapter() {
                    public void mouseWheelMoved(MouseEvent e) {
                        action.action(e);
                    }
                };
            default:
                return null;
        }
    }

    /**
     * Get a mouse action according to the type
     * @param type the event type
     * @param uicomp the component where to put the listener
     * @param command the command
     * @return the action
     */
    public static MouseAction getMouseAction(Type type, UIComponent uicomp, String command) {
        switch (type) {
            case CLICKED:
                return new MouseAction(uicomp, command) {
                    public void action(MouseEvent e) {
                        UIWidgetTools.execAction(this.callback, e.getButton(), e.getClickCount(), e.getX(), e.getY());
                    }
                };
            case MOVED:
                return new MouseAction(uicomp, command) {
                    public void action(MouseEvent e) {
                        UIWidgetTools.execAction(this.callback, e.getX(), e.getY());
                    }
                };
            case DRAGGED:
                return new MouseAction(uicomp, command) {
                    public void action(MouseEvent e) {
                        UIWidgetTools.execAction(this.callback, e.getXOnScreen(), e.getYOnScreen(), e.getX(), e.getY());
                    }
                };
            case EXITED:
                return new MouseAction(uicomp, command) {
                    public void action(MouseEvent e) {
                        UIWidgetTools.execAction(this.callback);
                    }
                };
            case ENTERED:
                return new MouseAction(uicomp, command) {
                    public void action(MouseEvent e) {
                        UIWidgetTools.execAction(this.callback);
                    }
                };
            case PRESSED:
                return new MouseAction(uicomp, command) {
                    public void action(MouseEvent e) {
                        UIWidgetTools.execAction(this.callback, e.getButton(), e.getClickCount(), e.getX(), e.getY());
                    }
                };
            case RELEASED:
                return new MouseAction(uicomp, command) {
                    public void action(MouseEvent e) {
                        UIWidgetTools.execAction(this.callback, e.getButton(), e.getClickCount(), e.getX(), e.getY());
                    }
                };
            case WHEEL:
                return new MouseAction(uicomp, command) {
                    public void action(MouseEvent e) {
                        UIWidgetTools.execAction(this.callback, ((MouseWheelEvent) e).getWheelRotation());
                    }
                };
            default:
                return null;
        }
    }

    /**
     * Inner class to have a Focus action
     */
    protected static abstract class MouseAction {

        UICallback callback;
        UIComponent uicomp;

        /**
         * Default constructor
         * @param uicomp the component which will have this action
         * @param command the command
         */
        MouseAction(UIComponent uicomp, String command) {
            this.uicomp = uicomp;
            this.callback = UICallback.newInstance(uicomp, command);
        }

        /**
         * The action
         * @param e event
         */
        public abstract void action(MouseEvent e);
    }
}
