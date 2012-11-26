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

public class UIMouseListener extends UIListener {

    public enum Type {
        CLICKED, MOVED, DRAGGED, EXITED, ENTERED, PRESSED, RELEASED, WHEEL;
    }

    Component component;

    MouseAdapter adapter;
    MouseAction clicked;
    MouseAction moved;
    MouseAction dragged;
    MouseAction exited;
    MouseAction entered;
    MouseAction pressed;
    MouseAction released;
    MouseAction wheelmoved;

    boolean clickedEnable = true;
    boolean movedEnable = true;
    boolean draggedEnable = true;
    boolean exitedEnable = true;
    boolean enteredEnable = true;
    boolean pressedEnable = true;
    boolean releasedEnable = true;
    boolean wheelMovedEnable = true;

    boolean basicListenerAdded;
    boolean motionListenerAdded;
    boolean wheelListenerAdded;

    public UIMouseListener() throws UIWidgetException {
        super(null);
    }

    public UIMouseListener(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

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

    public String getOnmouseclick() {
        if (clicked != null) {
            return clicked.command;
        }

        return null;
    }

    public String getOnmouseover() {
        if (moved != null) {
            return moved.command;
        }

        return null;
    }

    public String getOnmouseenter() {
        if (entered != null) {
            return entered.command;
        }

        return null;
    }

    public String getOnmouseexit() {
        if (exited != null) {
            return exited.command;
        }

        return null;
    }

    public String getOnmousepress() {
        if (pressed != null) {
            return pressed.command;
        }

        return null;
    }

    public String getOnmouserelease() {
        if (released != null) {
            return released.command;
        }

        return null;
    }

    public String getOnmousedrag() {
        if (dragged != null) {
            return dragged.command;
        }

        return null;
    }

    public String getOnmousewheel() {
        if (wheelmoved != null) {
            return wheelmoved.command;
        }

        return null;
    }

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

    public void setOnmouseclick(String command) {
        clicked = getMouseAction(Type.CLICKED, getParent(), command);
        addListenerToComponent(component);
    }

    public void setOnmouseover(String command) {
        moved = getMouseAction(Type.MOVED, getParent(), command);
        addListenerToComponent(component);
    }

    public void setOnmouseenter(String command) {
        entered = getMouseAction(Type.ENTERED, getParent(), command);
        addListenerToComponent(component);
    }

    public void setOnmouseexit(String command) {
        exited = getMouseAction(Type.EXITED, getParent(), command);
        addListenerToComponent(component);
    }

    public void setOnmousepress(String command) {
        pressed = getMouseAction(Type.PRESSED, getParent(), command);
        addListenerToComponent(component);
    }

    public void setOnmouserelease(String command) {
        released = getMouseAction(Type.RELEASED, getParent(), command);
        addListenerToComponent(component);
    }

    public void setOnmousewheel(String command) {
        wheelmoved = getMouseAction(Type.WHEEL, getParent(), command);
        addListenerToComponent(component);
    }

    public void setOnmousedrag(String command) {
        dragged = getMouseAction(Type.DRAGGED, getParent(), command);
        addListenerToComponent(component);
    }

    public void setOnmouseclickEnable(boolean b) {
        clickedEnable = b;
    }

    public void setOnmouseoverEnable(boolean b) {
        movedEnable = b;
    }

    public void setOnmouseenterEnable(boolean b) {
        enteredEnable = b;
    }

    public void setOnmouseexitEnable(boolean b) {
        exitedEnable = b;
    }

    public void setOnmousepressEnable(boolean b) {
        pressedEnable = b;
    }

    public void setOnmousereleaseEnable(boolean b) {
        releasedEnable = b;
    }

    public void setOnmousewheelEnable(boolean b) {
        wheelMovedEnable = b;
    }

    public void setOnmousedragEnable(boolean b) {
        draggedEnable = b;
    }

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

    public static MouseAction getMouseAction(Type type, UIComponent uicomp, String command) {
        switch (type) {
            case CLICKED:
                return new MouseAction(uicomp, command) {
                    public void action(MouseEvent e) {
                        UIWidgetTools.execAction(this.uicomp, this.command, e.getButton(), e.getClickCount(), e.getX(), e.getY());
                    }
                };
            case MOVED:
                return new MouseAction(uicomp, command) {
                    public void action(MouseEvent e) {
                        UIWidgetTools.execAction(this.uicomp, this.command, e.getX(), e.getY());
                    }
                };
            case DRAGGED:
                return new MouseAction(uicomp, command) {
                    public void action(MouseEvent e) {
                        UIWidgetTools.execAction(this.uicomp, this.command, e.getXOnScreen(), e.getYOnScreen(), e.getX(), e.getY());
                    }
                };
            case EXITED:
                return new MouseAction(uicomp, command) {
                    public void action(MouseEvent e) {
                        UIWidgetTools.execAction(this.uicomp, this.command);
                    }
                };
            case ENTERED:
                return new MouseAction(uicomp, command) {
                    public void action(MouseEvent e) {
                        UIWidgetTools.execAction(this.uicomp, this.command);
                    }
                };
            case PRESSED:
                return new MouseAction(uicomp, command) {
                    public void action(MouseEvent e) {
                        UIWidgetTools.execAction(this.uicomp, this.command, e.getButton(), e.getClickCount(), e.getX(), e.getY());
                    }
                };
            case RELEASED:
                return new MouseAction(uicomp, command) {
                    public void action(MouseEvent e) {
                        UIWidgetTools.execAction(this.uicomp, this.command, e.getButton(), e.getClickCount(), e.getX(), e.getY());
                    }
                };
            case WHEEL:
                return new MouseAction(uicomp, command) {
                    public void action(MouseEvent e) {
                        UIWidgetTools.execAction(this.uicomp, this.command, ((MouseWheelEvent) e).getWheelRotation());
                    }
                };
            default:
                return null;
        }
    }

    private static abstract class MouseAction {

        String command;
        UIComponent uicomp;

        MouseAction(UIComponent uicomp, String command) {
            this.uicomp = uicomp;
            this.command = command;
        }

        public abstract void action(MouseEvent e);
    }
}
