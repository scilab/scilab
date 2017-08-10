/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
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

package org.scilab.modules.gui.editor;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

import org.scilab.modules.gui.datatip.DatatipCreate;
import org.scilab.modules.gui.datatip.DatatipDelete;
import org.scilab.modules.gui.datatip.DatatipMove;
import org.scilab.modules.gui.datatip.DatatipDrag;
import org.scilab.modules.gui.datatip.DatatipOrientation;
import org.scilab.modules.gui.datatip.DatatipManagerMode;
import org.scilab.modules.gui.datatip.DatatipManager;

import org.scilab.modules.gui.editor.Editor;
import org.scilab.modules.gui.editor.EditorManager;
import org.scilab.modules.gui.editor.EntityPicker;
import org.scilab.modules.gui.events.GlobalEventWatcher;

import org.scilab.modules.gui.ged.Inspector;

import org.scilab.modules.gui.plotbrowser.PlotBrowser;


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

    Integer windowUid;

    Integer picked;
    EntityPicker ep;

    DatatipManagerMode datatipManagerMode;
    Integer lastDatatip = null;
    Integer[] lastClick = new Integer[2];
    boolean changeOrientation = false;

    Editor editor;
    boolean isInRotation = false;
    boolean isLeftButtonPressed = false;
    boolean enabled = false;

    public EditorEventListener(Integer uid) {
        windowUid = uid;
        editor = EditorManager.newEditor(uid);
        ep = new EntityPicker();
        datatipManagerMode = DatatipManager.newDatatipManagerMode(uid);
    }

    public void setEnable(boolean enabled) {
        this.enabled = enabled;
    }

    public boolean isEnable() {
        return enabled;
    }

    public boolean isDatatipEnable() {
        return datatipManagerMode.isEnabled();
    }

    public boolean canProcess() {
        return enabled && !GlobalEventWatcher.isActivated() && !datatipManagerMode.isInteractiveZoom();
    }

    public void onExit() {
        EditorManager.deleteEditor(editor.getFigureUid());
        DatatipManager.deleteDatatipManager(windowUid);
    }

    public void keyPressed(KeyEvent arg0) {
        if (enabled && !GlobalEventWatcher.isActivated()) {
            editor.onKeyPressed(arg0);
            if (datatipManagerMode.isEnabled()) {
                if (datatipManagerMode.getSelectedTip() != null) {
                    switch (arg0.getKeyCode()) {
                        case KeyEvent.VK_RIGHT:
                            DatatipMove.moveRight(datatipManagerMode.getSelectedTip());
                            break;
                        case KeyEvent.VK_LEFT:
                            DatatipMove.moveLeft(datatipManagerMode.getSelectedTip());
                            break;
                        case KeyEvent.VK_DELETE:
                        case KeyEvent.VK_BACK_SPACE:
                            DatatipDelete.deleteDatatip(datatipManagerMode.getSelectedTip());
                            break;
                    }
                }
            }
        }
    }

    public void keyReleased(KeyEvent arg0) {
    }

    public void keyTyped(KeyEvent arg0) {
    }

    /**
     * @param arg0 MouseEvent
     */
    public void mouseClicked(MouseEvent arg0) {
    }

    public void mouseEntered(MouseEvent arg0) {
    }

    public void mouseExited(MouseEvent arg0) {
    }

    /**
     * On left mouse press: pass event to editor,
     * if datatip manager mode is enabled try select/ create a datatip
     * @param arg0 MouseEvent
     */
    public void mousePressed(MouseEvent arg0) {
        if (canProcess() && arg0.getButton() == 1) {
            isLeftButtonPressed = true;
            if (!datatipManagerMode.isEnabled()) {
                editor.onLeftMouseDown(arg0);

                // Part responsible for the exchange of properties of the GED.
                //If the GED is open, so the code is executed.
                if (Inspector.isInspectorOpened()) {
                    editor.onClickGED();
                }
                if (PlotBrowser.isPlotBrowserOpened()) {
                    PlotBrowser.getPlotBrowser(windowUid);
                }
            } else {
                if (!datatipManagerMode.pickAndHighlight(arg0.getX(), arg0.getY())) {
                    picked = ep.pick(windowUid, arg0.getX(), arg0.getY());
                    if (picked != null) {
                        lastClick[0] = arg0.getX();
                        lastClick[1] = arg0.getY();
                        lastDatatip = DatatipCreate.createDatatip(windowUid, arg0.getX(), arg0.getY());
                        changeOrientation = true;
                    }
                } else {
                    // we are in datatip mode and we clicked on a datatip
                    datatipManagerMode.enableTranslation(false);
                }
            }
        }
    }

    /**
     * On right mouse release: pass event to editor to make
     * the popup menu visible if datatip's manager mode is off,
     * else try delete a datatip.
     * @param arg0 MouseEvent
     */
    public void mouseReleased(MouseEvent arg0) {
        if (canProcess()) {
            if (!datatipManagerMode.isEnabled()) {
                if (arg0.getButton() == 3) {
                    if (!isInRotation) {
                        editor.onRightMouseClick(arg0);
                    }
                } else if (arg0.getButton() == 1) {
                    editor.onLeftMouseRelease(arg0);
                }
            } else {
                if (arg0.getButton() == 3 && !isInRotation) {
                    if (!datatipManagerMode.pickAndDelete(arg0.getX(), arg0.getY())) {
                        datatipManagerMode.onRightMouseClick(arg0);
                    }
                }
                datatipManagerMode.enableTranslation(true);
            }
            isInRotation = false;
            isLeftButtonPressed = false;
            changeOrientation = false;
            editor.setUpdateDrag(true);
        }
    }

    /**
     * On left mouse dragged: pass event to editor if
     * datatip maneger mode is disabled else
     * drag/ change datatip orientation
     */
    public void mouseDragged(MouseEvent arg0) {
        if (canProcess()) {

            if (!datatipManagerMode.isEnabled()) {

                if (isLeftButtonPressed) {
                    editor.onMouseDragged(arg0);

                } else {
                    isInRotation = true;
                }
            } else {
                if (changeOrientation) {
                    Integer[] curClick = {arg0.getX(), arg0.getY()};
                    DatatipOrientation.setOrientation(lastDatatip, lastClick, curClick);
                } else if (datatipManagerMode.getSelectedTip() != null && isLeftButtonPressed) {
                    DatatipDrag.dragDatatip(datatipManagerMode.getSelectedTip(), arg0.getX(), arg0.getY());
                }
            }
            editor.setUpdateDrag(false);
        }
    }

    public void mouseMoved(MouseEvent arg0) {
    }
}

