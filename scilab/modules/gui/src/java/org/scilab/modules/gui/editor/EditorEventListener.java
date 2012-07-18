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
import java.util.ArrayList;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.graphicObject.*;

import org.scilab.modules.gui.datatip.DatatipCreate;
import org.scilab.modules.gui.datatip.MarkerCreate;
import org.scilab.modules.gui.datatip.DatatipSelect;
import org.scilab.modules.gui.datatip.DatatipDelete;
import org.scilab.modules.gui.datatip.DatatipMove;
import org.scilab.modules.gui.datatip.DatatipManagerMode;

import org.scilab.modules.gui.editor.AxesHandler;
import org.scilab.modules.gui.editor.Editor;
import org.scilab.modules.gui.editor.PolylineHandler;
import org.scilab.modules.gui.editor.ObjectSearcher;

import org.scilab.modules.gui.ged.Inspector;
import org.scilab.modules.gui.ged.SwapObject;


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

    public static String windowUid;
    boolean isInRotation = false;
    String picked;
    Editor editor;
    EntityPicker ep;
    ArrayList<Double> getDatatipsCoord = new ArrayList<Double>();
    ArrayList<String> getDatatipsUid = new ArrayList<String>();
    ArrayList<String> getMarkersUid = new ArrayList<String>();
    ArrayList<String> getPolylinesUid = new ArrayList<String>();
    Integer indexToMove;

    public EditorEventListener(String uid) {
        windowUid = uid;
        editor = new Editor();
        ep = new EntityPicker();
        editor.setFigure(uid);
        DatatipManagerMode.setFigure(uid);
    }

    public void keyPressed(KeyEvent arg0) {
        if (DatatipManagerMode.getDatatipManagerMode()) {
            if (indexToMove != null) {
                if (arg0.getKeyCode() == KeyEvent.VK_RIGHT) {
                    getDatatipsCoord = DatatipMove.moveRight (windowUid, getPolylinesUid, indexToMove, getDatatipsCoord, getDatatipsUid, getMarkersUid);
                } else if (arg0.getKeyCode() == KeyEvent.VK_LEFT) {
                    getDatatipsCoord = DatatipMove.moveLeft (windowUid, getPolylinesUid, indexToMove, getDatatipsCoord, getDatatipsUid, getMarkersUid);
                }
            }
        }
    }

    public void keyReleased(KeyEvent arg0) {
        /* TODO add copy/cut/paste by keyboard shortcut*/
        if (arg0.isControlDown()) {
            if (arg0.getKeyCode() == KeyEvent.VK_C) {
                if (editor.getSelected() != null) {

                }
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
                    picked = ep.pick( windowUid, arg0.getX(), arg0.getY() );
                    if (picked != null) {
                        String datatipUid = DatatipCreate.createDatatip ( windowUid, arg0.getX(), arg0.getY() );
                        String markerUid = MarkerCreate.createMarker ( windowUid, arg0.getX(), arg0.getY() );
                        getDatatipsCoord = DatatipCreate.getAllDatatipsCoord (getDatatipsCoord, datatipUid);
                        getDatatipsUid = DatatipCreate.getAllDatatipsUid (getDatatipsUid, datatipUid);
                        getMarkersUid = MarkerCreate.getAllMarkersUid (getMarkersUid, markerUid);
                        getPolylinesUid = DatatipCreate.getAllPolylinesUid (getPolylinesUid, picked);
                    } else {
                        indexToMove = DatatipSelect.selectDatatip(windowUid, arg0.getX(), arg0.getY(), getDatatipsCoord, getDatatipsUid, getMarkersUid);
                    }
                }
            } else if (arg0.getButton() == 3) {
                if (getDatatipsCoord.size() > 0) {
                    Integer indexToDelete = DatatipSelect.selectDatatip(windowUid, arg0.getX(), arg0.getY(), getDatatipsCoord, getDatatipsUid, getMarkersUid);
                    if (indexToDelete != null) {
                        DatatipDelete.deleteDatatip (indexToDelete, getDatatipsUid, getMarkersUid);
                        getDatatipsCoord = DatatipDelete.romoveDatatipCoords (indexToDelete, getDatatipsCoord);
                        getDatatipsUid = DatatipDelete.romoveDatatipUid (indexToDelete, getDatatipsUid);
                        getMarkersUid = DatatipDelete.romoveMarkerUid (indexToDelete, getMarkersUid);
                        getPolylinesUid = DatatipDelete.romovePolylineUid (indexToDelete, getPolylinesUid);
                        indexToMove = null;
                    }
                }
            }
        }
    }

    public void mouseEntered(MouseEvent arg0) {
    }

    public void mouseExited(MouseEvent arg0) {
    }

    /**
    * On left mouse press: check if the user clicked over
    * a polyline and set it selected.
    * @param arg0 MouseEvent
    */
    public void mousePressed(MouseEvent arg0) {
        if (arg0.getButton() == 1) {
            picked = ep.pick(windowUid, arg0.getX(), arg0.getY());
            editor.setSelected(picked);
            // Part responsible for the exchange of properties of the GED.
            //If the GED is open, so the code is executed.
            if (Inspector.window != null) {
                if (picked == null) {
                    new SwapObject("axes or figure", windowUid, (Integer) arg0.getX(), (Integer) arg0.getY());
            } else {
                    new SwapObject("curve", picked, 0, 0);
		}
            }
        } else if (arg0.getButton() == 3) { }
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
                    editor.onMouseClick(arg0);
                }
            }
        }
        isInRotation = false;
    }

    public void mouseDragged(MouseEvent arg0) {
        isInRotation = true;
    }

    public void mouseMoved(MouseEvent arg0) {
    }
}

