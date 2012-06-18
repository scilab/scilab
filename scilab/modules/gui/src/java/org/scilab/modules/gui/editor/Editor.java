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

import java.awt.*;
import java.awt.event.*;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;

import org.scilab.modules.gui.editor.ScilabClipboard;
import org.scilab.modules.gui.editor.SystemClipboard;
import org.scilab.modules.gui.editor.PolylineHandler;


/**
* Point and click figure editor.
*
* Provides polyline selection by mouse click,
* copy, cut, paste, delete, hide, unhide polylines
* by popup menus and keyboard shortcuts.
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/

public class Editor {

    JPopupMenu menu;
    JMenuItem copy, cut, paste, delete, clear, hide, unhide, clipboardCopy;
    String[] names = {"Copy", "Cut", "Paste", "Delete", "Clear", "Hide", "Unhide All", "Copy to Clipboard"};
    String[] hint = {"Copy selected polyline.",
                     "Cut selected polyline.",
                     "Paste copied polyline to this figure.",
                     "Delete selected polyline.",
                     "Clear figure.",
                     "Hide selected polyline.",
                     "Unhide all polylines.",
                     "Copy figure to system clipboard."
                    };

    String selected = null;
    String figureUid = null;
    Integer oriColor = 0;
    Integer[] lastClick = { 0, 0 };

    public Editor() {
        init();
        setSelected(null);
        setFigure(null);
    }

    /**
    * On right mouse click set popup menu visible.
    * @param event MouseEvent to retrieve click positon in figure.
    */
    public void mouseDown(MouseEvent event) {
        if (event.isPopupTrigger()) {
            boolean b = ScilabClipboard.getInstance().canPaste();
            paste.setEnabled(b);
            menu.show(event.getComponent(), event.getX(), event.getY());
            lastClick[0] = event.getX();
            lastClick[1] = event.getY();
        }
    }

    /**
    * On right mouse click set popup menu visible.
    * @param event MouseEvent to retrieve click positon in figure.
    */
    public void mouseUp(MouseEvent event) {
        if (event.isPopupTrigger()) {
            boolean b = ScilabClipboard.getInstance().canPaste();
            paste.setEnabled(b);
            menu.show(event.getComponent(), event.getX(), event.getY());
            lastClick[0] = event.getX();
            lastClick[1] = event.getY();
        }
    }

    /**
    * Initializes the popup menu.
    *
    * Create the popup menu and all popup menu items
    * and set the properly action for each menu item.
    */
    public void init() {

        menu = new JPopupMenu();

        copy = new JMenuItem(names[0]);
        copy.setToolTipText(hint[0]);
        cut = new JMenuItem(names[1]);
        cut.setToolTipText(hint[1]);
        paste = new JMenuItem(names[2]);
        paste.setToolTipText(hint[2]);
        delete = new JMenuItem(names[3]);
        delete.setToolTipText(hint[3]);
        clear = new JMenuItem(names[4]);
        clear.setToolTipText(hint[4]);
        hide = new JMenuItem(names[5]);
        hide.setToolTipText(hint[5]);
        unhide = new JMenuItem(names[6]);
        unhide.setToolTipText(hint[6]);
        clipboardCopy = new JMenuItem(names[7]);
        clipboardCopy.setToolTipText(hint[7]);



        copy.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickCopy();
            }
        });

        cut.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickCut();
            }
        });

        paste.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickPaste();
            }
        });

        delete.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickDelete();
            }
        });

        clear.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickClear();
            }
        });

        hide.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickHide();
            }
        });

        unhide.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickUnhide();
            }
        });

        clipboardCopy.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickCCopy();
            }
        });


        menu.add(copy);
        menu.add(cut);
        menu.add(paste);
        menu.add(delete);
        menu.add(clear);
        menu.add(hide);
        menu.add(unhide);
        menu.add(clipboardCopy);
    }

    /**
    * Set the current selected polyline, change its color,
    * if there is a previous selected polyline restores its color,
    * enable/disable popup menu items if there is/isn't
    * a polyline selected.
    *
    * @param uid polyline unique identifier. Null uid unselect previous selection.
    */

    public void setSelected(String uid) {

        if (selected != null) {
            PolylineHandler.getInstance().setColor(selected, oriColor);
        }

        selected = uid;

        if (selected != null) {
            copy.setEnabled(true);
            cut.setEnabled(true);
            delete.setEnabled(true);
            hide.setEnabled(true);

            oriColor = PolylineHandler.getInstance().setColor(selected, 12);
        } else {
            copy.setEnabled(false);
            cut.setEnabled(false);
            delete.setEnabled(false);
            hide.setEnabled(false);
        }
    }

    /**
    * Returns selected polyline unique identifier.
    * @return selected polyline uid or null if there isn't any selected.
    */
    public String getSelected() {
        return selected;
    }

    /**
    * Set the figure uid wich the editor belongs.
    * @param uid Figure unique identifier.
    */
    public void setFigure(String uid) {
        figureUid = uid;
    }

    /**
    * Implements copy menu item action(Callback).
    */
    public void onClickCopy() {
        ScilabClipboard.getInstance().copy(selected);
        ScilabClipboard.getInstance().setCopiedColor(oriColor);
    }

    /**
    * Implements paste menu item action(Callback).
    */
    public void onClickPaste() {
        ScilabClipboard.getInstance().paste(figureUid, lastClick);
    }

    /**
    * Implements cut menu item action
    */
    public void onClickCut() {
        String s = selected;
        setSelected(null);
        ScilabClipboard.getInstance().cut(s);
        ScilabClipboard.getInstance().setCopiedColor(oriColor);
    }

    /**
    * Implements delete menu item action(Callback).
    */
    public void onClickDelete() {
        String toDelete = selected;
        setSelected(null);
        PolylineHandler.getInstance().delete(toDelete);
    }

    /**
    * Implements clear menu item action(Callback).
    */
    public void onClickClear() {
        setSelected(null);
        String axesTo = AxesHandler.clickedAxes(figureUid, lastClick);
        if (axesTo != null) {
            PolylineHandler.getInstance().deleteAll(axesTo);
        }
    }

    /**
    * Implements hide menu item action(Callback).
    */
    public void onClickHide() {
        if (selected != null) {
            PolylineHandler.getInstance().visible(selected, false);
            setSelected(null);
        }
    }

    /**
    * Implements unhide menu item action(Callback).
    */
    public void onClickUnhide() {
        PolylineHandler.getInstance().visible(figureUid, true);
    }

    /**
    * Implements clipboard copy menu item action(Callback).
    */
    public void onClickCCopy() {
        SystemClipboard.copyToSysClipboard(figureUid);
    }

}

