/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.editor;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

import org.scilab.modules.gui.datatip.DatatipCreate;
import org.scilab.modules.gui.datatip.DatatipSelect;
import org.scilab.modules.gui.datatip.DatatipDelete;
import org.scilab.modules.gui.datatip.DatatipMove;
import org.scilab.modules.gui.datatip.DatatipDrag;
import org.scilab.modules.gui.datatip.DatatipOrientation;
import org.scilab.modules.gui.datatip.DatatipManagerMode;

import org.scilab.modules.gui.editor.AxesHandler;
import org.scilab.modules.gui.editor.Editor;
import org.scilab.modules.gui.editor.EditorManager;
import org.scilab.modules.gui.editor.EntityPicker;

import org.scilab.modules.gui.ged.Inspector;
import org.scilab.modules.gui.ged.SwapObject;

import org.scilab.modules.gui.plotbrowser.PlotBrowser;

import org.scilab.modules.renderer.CallRenderer;

/**
* Event listener for the figure editor.
*
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/

public class EditorEventListener implements KeyListener, MouseListener, MouseMotionListener {

    String windowUid;

    String picked;
    EntityPicker ep;

    String selectedDatatip = null;

    Editor editor;
    boolean isInRotation = false;
    boolean isLeftButtonPressed = false;

    public EditorEventListener(String uid) {
        windowUid = uid;
        editor = EditorManager.newEditor(uid);
        ep = new EntityPicker();
        DatatipManagerMode.setFigure(uid);
    }

    public void onExit() {
        EditorManager.deleteEditor(editor.getFigureUid());
    }

    public void keyPressed(KeyEvent arg0) {

        editor.onKeyPressed(arg0);

        if (DatatipManagerMode.getDatatipManagerMode()) {
            if (selectedDatatip != null) {
                if (arg0.getKeyCode() == KeyEvent.VK_RIGHT) {
                    DatatipMove.moveRight (selectedDatatip);
                } else if (arg0.getKeyCode() == KeyEvent.VK_LEFT) {
                    DatatipMove.moveLeft (selectedDatatip);
                }
                keyReleased(arg0);
            }
        }
    }

    public void keyReleased(KeyEvent arg0) {

        if (DatatipManagerMode.getDatatipManagerMode()) {
            if (selectedDatatip != null) {
                //DatatipOrientation.setOrientation (selectedDatatip);
            }
        }
    }

    public void keyTyped(KeyEvent arg0) {
    }

    /**
    * On left mouse click: check if user clicked over
    * a polyline and create a datatip if datatip manager mode is on
    * On right mouse click: check if user clicked over
    * a datatip and deletes it if datatip manager mode is on
    *
    * @param arg0 MouseEvent
    */
    public void mouseClicked(MouseEvent arg0) {
        if (DatatipManagerMode.getDatatipManagerMode()) {
            if (arg0.getButton() == 1) {
                if (arg0.getClickCount() == 1) {
                    selectedDatatip = DatatipSelect.selectDatatip(windowUid, arg0.getX(), arg0.getY());
                    if (selectedDatatip == null) {
                        picked = ep.pick(windowUid, arg0.getX(), arg0.getY());
                        if (picked != null) {
                            String datatipUid = DatatipCreate.createDatatip(windowUid, arg0.getX(), arg0.getY());
                        }
                    }
                } else if (arg0.getClickCount() == 2) {	
                    //open GED with the properties of DataTip
                    //not implemented yet
                    //editor.onClickGED();
                }
            } else if (arg0.getButton() == 3) {
                Integer pos[] = {arg0.getX(), arg0.getY()};
                selectedDatatip = ep.pickDatatip(windowUid, pos);
                if (selectedDatatip != null) {
                    DatatipDelete.deleteDatatip(selectedDatatip);
                    selectedDatatip = null;
                }
            }
        }
    }

    public void mouseEntered(MouseEvent arg0) {
    }

    public void mouseExited(MouseEvent arg0) {
    }

    /**
    * On left mouse press: pass event to editor.
    * @param arg0 MouseEvent
    */
    public void mousePressed(MouseEvent arg0) {
        if (arg0.getButton() == 1) {
            isLeftButtonPressed = true;
            editor.onLeftMouseDown(arg0);

            // Part responsible for the exchange of properties of the GED.
            //If the GED is open, so the code is executed.
            if (Inspector.isInspectorOpened()) {
                editor.onClickGED();
            }
            if (PlotBrowser.isPlotBrowserOpened()) {
                PlotBrowser.getPlotBrowser(windowUid);
            }
        }
    }

    /**
    * On right mouse release: pass event to editor to make
    * the popup menu visible if datatip's manager mode is off.
    * @param arg0 MouseEvent
    */
    public void mouseReleased(MouseEvent arg0) {
        if (arg0.getButton() == 3) {
            if (!isInRotation) {
                if (!DatatipManagerMode.getDatatipManagerMode()) {
                    editor.onRightMouseClick(arg0);
                }
            }
        } else if (arg0.getButton() == 1) {
            editor.onLeftMouseRelease(arg0);
        }
        isInRotation = false;
        isLeftButtonPressed = false;
    }

    /**On left mouse dragged: pass event to editor.*/
    public void mouseDragged(MouseEvent arg0) {
        if (isLeftButtonPressed) {
            editor.onMouseDragged(arg0);
        } else {
            isInRotation = true;
        }
        if (selectedDatatip != null) {
            DatatipDrag.dragDatatip(selectedDatatip, arg0.getX(), arg0.getY());
            //DatatipOrientation.setOrientation (selectedDatatip);
        }
    }

    public void mouseMoved(MouseEvent arg0) {
    }
}

