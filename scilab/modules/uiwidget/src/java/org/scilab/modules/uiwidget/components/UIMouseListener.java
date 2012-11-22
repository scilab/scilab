/*
 * Uicontrol2 ( http://forge.scilab.org/index.php/p/uicontrol2/ ) - This file is a part of Uicontrol2
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
import java.util.Map;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;
import org.scilab.modules.uiwidget.UIListener;

import org.scilab.modules.action_binding.InterpreterManagement;

public class UIMouseListener extends UIListener {

    public enum Type {
        CLICKED, MOVED, DRAGGED, EXITED, ENTERED, PRESSED, RELEASED, WHEEL;
    }

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

    public void addListenerToComponent(Component c) {
        if (clicked != null || entered != null || exited != null || pressed != null || released != null) {
            c.addMouseListener(adapter);
        }
        if (dragged != null || moved != null) {
            c.addMouseMotionListener(adapter);
        }
        if (wheelmoved != null) {
            c.addMouseWheelListener(adapter);
        }
    }

    public void setOnmouseclick(String command) {
        clicked = getMouseAction(Type.CLICKED, command);
    }

    public void setOnmouseover(String command) {
        moved = getMouseAction(Type.MOVED, command);
    }

    public void setOnmouseenter(String command) {
        entered = getMouseAction(Type.ENTERED, command);
    }

    public void setOnmouseexit(String command) {
        exited = getMouseAction(Type.EXITED, command);
    }

    public void setOnmousepress(String command) {
        pressed = getMouseAction(Type.PRESSED, command);
    }

    public void setOnmouserelease(String command) {
        released = getMouseAction(Type.RELEASED, command);
    }

    public void setOnmousewheel(String command) {
        wheelmoved = getMouseAction(Type.WHEEL, command);
    }

    public void setOnmousedrag(String command) {
        dragged = getMouseAction(Type.DRAGGED, command);
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

    public static MouseAdapter getMouseAdapter(Type type, String command) {
        final MouseAction action = getMouseAction(type, command);

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

    public static MouseAction getMouseAction(Type type, String command) {
        switch (type) {
            case CLICKED:
                return new MouseAction(command) {
                    public void action(MouseEvent e) {
                        InterpreterManagement.requestScilabExec(this.command + "(" + e.getButton() + "," + e.getClickCount() + "," + e.getX() + "," + e.getY() + ")");
                    }
                };
            case MOVED:
                return new MouseAction(command) {
                    public void action(MouseEvent e) {
                        InterpreterManagement.requestScilabExec(this.command + "(" + e.getX() + "," + e.getY() + ")");
                    }
                };
            case DRAGGED:
                return new MouseAction(command) {
                    public void action(MouseEvent e) {
                        InterpreterManagement.requestScilabExec(this.command + "(" + e.getXOnScreen() + "," + e.getYOnScreen() + "," + e.getX() + "," + e.getY() + ")");
                    }
                };
            case EXITED:
                return new MouseAction(command) {
                    public void action(MouseEvent e) {
                        InterpreterManagement.requestScilabExec(this.command + "()");
                    }
                };
            case ENTERED:
                return new MouseAction(command) {
                    public void action(MouseEvent e) {
                        InterpreterManagement.requestScilabExec(this.command + "()");
                    }
                };
            case PRESSED:
                return new MouseAction(command) {
                    public void action(MouseEvent e) {
                        InterpreterManagement.requestScilabExec(this.command + "(" + e.getButton() + "," + e.getClickCount() + "," + e.getX() + "," + e.getY() + ")");
                    }
                };
            case RELEASED:
                return new MouseAction(command) {
                    public void action(MouseEvent e) {
                        InterpreterManagement.requestScilabExec(this.command + "(" + e.getButton() + "," + e.getClickCount() + "," + e.getX() + "," + e.getY() + ")");
                    }
                };
            case WHEEL:
                return new MouseAction(command) {
                    public void action(MouseEvent e) {
                        InterpreterManagement.requestScilabExec(this.command + "(" + ((MouseWheelEvent) e).getWheelRotation() + ")");
                    }
                };
            default:
                return null;
        }
    }

    private static abstract class MouseAction {

        String command;

        MouseAction(String command) {
            this.command = command;
        }

        public abstract void action(MouseEvent e);
    }
}
