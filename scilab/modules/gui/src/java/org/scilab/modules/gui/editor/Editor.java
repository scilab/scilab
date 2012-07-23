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


import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.event.MouseEvent;
import java.awt.event.KeyEvent;
import java.awt.Component;
import javax.swing.JOptionPane;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;

import org.scilab.modules.gui.editor.ScilabClipboard;
import org.scilab.modules.gui.editor.SystemClipboard;
import org.scilab.modules.gui.editor.PolylineHandler;
import org.scilab.modules.gui.editor.LabelHandler;
import org.scilab.modules.gui.editor.LegendHandler;
import org.scilab.modules.gui.ged.Inspector;
import org.scilab.modules.localization.Messages;



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
    JMenuItem copy, cut, paste, delete, clear, hide, unhide, clipboardCopy, labelX, labelY, labelZ, insert, remove, ged, editdata;
    JMenu labels, legends;

    EntityPicker.LegendInfo selectedLegend = null;
    String selected = null;
    String figureUid = null;
    Integer oriColor = 0;
    Integer[] lastClick = { 0, 0 };
    EntityPicker entityPicker;
    DataEditor dataEditor;
    boolean isLegend = false;
    boolean dataModifyEnabled = false;
    boolean dataEditEnabled = false;

    Component dialogComponent;

    public Editor() {
        init();
        setSelected(null);
        setFigure(null);

        entityPicker = new EntityPicker();
        dataEditor = new DataEditor();
        dataEditor.setLeaveAction(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                leaveDataEditor();
            }
        });
    }

    /**
     * Enable / disable data modify.
     * @param b true to enable, false to disable.
     */
    public void setDataModifyEnabled(boolean b) {
        dataModifyEnabled = b;
        if (!dataModifyEnabled) {
            leaveDataEditor();
        } else if (getSelected() != null) {
            enterDataEditor();
        }
    }

    /**
     * Returns if the data modify is enabled or not.
     * @return True if enabled, false otherwise.
     */
    public boolean isDataModifyEnabled() {
        return dataModifyEnabled;
    }

    /**
     * On right mouse click, set popup menu visible.
     * @param event MouseEvent to retrieve click positon in figure.
     */
    public void onRightMouseClick(MouseEvent event) {
        if (!dataEditEnabled) {
            boolean b = ScilabClipboard.getInstance().canPaste();
            paste.setEnabled(b);

            boolean notBlank = AxesHandler.isAxesNotBlank(figureUid);
            clipboardCopy.setEnabled(notBlank);

            menu.show(event.getComponent(), event.getX(), event.getY());
            lastClick[0] = event.getX();
            lastClick[1] = event.getY();
            dialogComponent = (Component)event.getComponent();
        } else {
            dataEditor.onRightClick(event);
        }
    }

    /**
     * Check if the user clicked over a polyline.
     *
     * @param event the mouse event.
     */
    public void onLeftMouseDown(MouseEvent event) {

        lastClick[0] = event.getX();
        lastClick[1] = event.getY();

        if (!dataEditEnabled) {
            switch(event.getClickCount()) {
                case 1:
                    /*try pick a legend*/
                    selectedLegend = entityPicker.pickLegend(figureUid, lastClick);
                    if (selectedLegend != null) {
                        isLegend = true;
                        setSelected(selectedLegend.legend);
                    } else {
                        /*try pick a polyline*/
                        isLegend = false;
                        setSelected(entityPicker.pick(figureUid, lastClick[0], lastClick[1]));
                    }
                    break;
                case 2:
                    /*there is a polyline selected? if yes start dataEditor*/
                    if (selected != null && !isLegend && dataModifyEnabled) {
                        enterDataEditor();
                    }
                    /*on double click over a legend or label open dialog*/
                    else if (selectedLegend != null) {
                        onClickInsert(selectedLegend.polyline);
                    } else {
                        /*try pick a label and open the dialog*/
                        onClickLabel(entityPicker.pickLabel(figureUid, lastClick));
                    }
                    break;
                default:
                    break;
            }
        } else { /*data editor is enabled, pass event to it*/
            switch(event.getClickCount()) {
                case 1:
                    dataEditor.onLeftMouseDown(event);
                    break;
                case 2:
                    dataEditor.onLeftDoubleClick(event);
                    break;
                default:
                    break;
            }
        }
    }

    /**
     * On drag move the selected object, if dataEditor 
     * is enabled pass event to it.
     * @param event The mouse event.
     */
    public void onMouseDragged(MouseEvent event) {
        Integer[] newClick = { event.getX(), event.getY() };
        if (dataModifyEnabled) {
            if (!dataEditEnabled) {
                String objUID = getSelected();
                if (objUID != null) {
                    if (isLegend) {
                        LegendHandler.dragLegend(objUID, lastClick, newClick);
                    } else {
                        PolylineHandler.getInstance().dragPolyline(objUID, lastClick, newClick);
                    }
                }
            } else {
                dataEditor.onDrag(lastClick, newClick);
            }
        }
        lastClick[0] = newClick[0];
        lastClick[1] = newClick[1];
    }

    /**
     * On ESC typed if dataEditor is enabled,
     * back to normal editor else leave editor mode.
     * is enabled pass event to it.
     * @param event The Key event.
     */
    void onKeyTyped(KeyEvent event) {
        if (event.getKeyCode() == KeyEvent.VK_ESCAPE) {
            leaveDataEditor();
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
        labels = new JMenu(Messages.gettext("Label"));
        legends = new JMenu(Messages.gettext("Legend"));

        copy = new JMenuItem(Messages.gettext("Copy"));
        copy.setToolTipText(Messages.gettext("Copy selected curve."));
        cut = new JMenuItem(Messages.gettext("Cut"));
        cut.setToolTipText(Messages.gettext("Cut selected curve."));
        paste = new JMenuItem(Messages.gettext("Paste"));
        paste.setToolTipText(Messages.gettext("Paste copied curve to this figure."));
        delete = new JMenuItem(Messages.gettext("Delete"));
        delete.setToolTipText(Messages.gettext("Delete selected curve."));
        clear = new JMenuItem(Messages.gettext("Clear"));
        clear.setToolTipText(Messages.gettext("Clear figure."));
        hide = new JMenuItem(Messages.gettext("Hide"));
        hide.setToolTipText(Messages.gettext("Hide selected curve."));
        unhide = new JMenuItem(Messages.gettext("Unhide all"));
        unhide.setToolTipText(Messages.gettext("Unhide all curves."));
        clipboardCopy = new JMenuItem(Messages.gettext("Copy to Clipboard"));
        clipboardCopy.setToolTipText(Messages.gettext("Copy figure to system clipboard."));
        labelX = new JMenuItem(Messages.gettext("Label X"));
        labelX.setToolTipText(Messages.gettext("Insert a label in X axis"));
        labelY = new JMenuItem(Messages.gettext("Label Y"));
        labelY.setToolTipText(Messages.gettext("Insert a label in Y axis"));
        labelZ = new JMenuItem(Messages.gettext("Label Z"));
        labelZ.setToolTipText(Messages.gettext("Insert a label in Z axis"));
        insert = new JMenuItem(Messages.gettext("Insert"));
        insert.setToolTipText(Messages.gettext("Insert a legend to current selected item"));
        remove = new JMenuItem(Messages.gettext("Remove"));
        remove.setToolTipText(Messages.gettext("Remove a legend of current selected item"));
        ged = new JMenuItem(Messages.gettext("Open Quick Editor"));
        ged.setToolTipText(Messages.gettext("Initialize the graphics editor"));
        editdata = new JMenuItem(Messages.gettext("Edit curve data"));
        editdata.setToolTipText(Messages.gettext("Enables curve data modify"));


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

        labelX.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickLabel(AxesHandler.axisTo.__X__);
            }
        });

        labelY.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickLabel(AxesHandler.axisTo.__Y__);
            }
        });

        labelZ.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickLabel(AxesHandler.axisTo.__Z__);
            }
        });

        insert.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickInsert(getSelected());
            }
        });

        remove.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickRemove();
            }
        });

        ged.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickGED();
            }
        });

        editdata.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                setDataModifyEnabled(true);
            }
        });

        labels.add(labelX);
        labels.add(labelY);
        labels.add(labelZ);
        legends.add(insert);
        legends.add(remove);
        menu.add(copy);
        menu.add(cut);
        menu.add(paste);
        menu.addSeparator();
        menu.add(delete);
        menu.add(clear);
        menu.addSeparator();
        menu.add(hide);
        menu.add(unhide);
        menu.addSeparator();
        menu.add(clipboardCopy);
        menu.addSeparator();
        menu.add(labels);
        menu.add(legends);
        menu.add(editdata);
        menu.addSeparator();
        menu.add(ged);
    }


    /**
     * Set the current selected object, change its color,
     * if there is a previous selected object restores its color,
     * enable/disable popup menu items if there is/isn't
     * a object selected.
     *
     * @param uid polyline/legend unique identifier. Null uid unselect previous selection.
     */
    public void setSelected(String uid) {

        if (PolylineHandler.getInstance().polylineExists(selected)) {
            PolylineHandler.getInstance().setColor(selected, oriColor);
        }

        selected = uid;

        if (selected != null) {
            oriColor = PolylineHandler.getInstance().setColor(selected, -3);
            if(!isLegend) {
                copy.setEnabled(true);
                cut.setEnabled(true);
                delete.setEnabled(true);
                hide.setEnabled(true);
                legends.setEnabled(true);
                editdata.setEnabled(true);
            }
            delete.setEnabled(true);
        } else {
            copy.setEnabled(false);
            cut.setEnabled(false);
            delete.setEnabled(false);
            hide.setEnabled(false);
            legends.setEnabled(false);
            editdata.setEnabled(false);
        }
    }

    /**
    * Get current color of the polyline line/mark.
    *
    * @return Returns the current color of the polyline.
    */
    public Integer getOriColor() {
        return oriColor;
    }

    /**
    * Set current color of the polyline line/mark.
    *
    * @param newScilabColor Color selected by user.
    */
    public void setOriColor(Integer newScilabColor) {
        oriColor = newScilabColor;
    }


    /**
     * Returns selected object unique identifier.
     * @return selected object uid or null if there isn't any selected.
     */
    public String getSelected() {
        if (PolylineHandler.getInstance().polylineExists(selected)) {
            return selected;
        } else {
            setSelected(null);
            return null;
        }
    }

    /**
     * Set the figure uid wich the editor belongs.
     * @param uid Figure unique identifier.
     */
    public void setFigure(String uid) {
        figureUid = uid;
    }

    /**
     * Get the figure uid wich the editor belongs.
     * @return figure uid.
     */
     public String getFigureUid() {
         return figureUid;
     }

    /**
    * Implements copy menu item action(Callback).
    */
    public void onClickCopy() {
        if (!isLegend) {
            ScilabClipboard.getInstance().copy(getSelected());
            ScilabClipboard.getInstance().setCopiedColor(oriColor);
        }
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
        String s = getSelected();
        if (s != null && !isLegend) {
            setSelected(null);
            ScilabClipboard.getInstance().cut(s);
            ScilabClipboard.getInstance().setCopiedColor(oriColor);
        }
    }

    /**
     * Implements delete menu item action(Callback).
     */
    public void onClickDelete() {
        String toDelete = getSelected();
        if (toDelete != null) {
            setSelected(null);
            PolylineHandler.getInstance().delete(toDelete);
        }
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
        if (getSelected() != null) {
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

    /**
     * Implements label insert action(Callback).
     * @param axis axis number.
     */
    public void onClickLabel(AxesHandler.axisTo axis) {

        String axes = AxesHandler.clickedAxes(figureUid, lastClick);
        if (axes != null && axis != null) {
            String text = LabelHandler.getLabelText(axes, axis);
            String s = (String)JOptionPane.showInputDialog(
                        dialogComponent,
                        Messages.gettext("Enter the text"),
                        Messages.gettext("Set label text"),
                        JOptionPane.PLAIN_MESSAGE,
                        null,
                        null,
                        text);
            if (s != null) {
                String tmp[] = {s};
                LabelHandler.setLabel(axes, tmp, axis);
            }
        }
    }

    /**
     * Implements legend insert action(Callback).
     * @param polyline Polyline to be inserted in the legend.
     */
    public void onClickInsert(String polyline) {

        String axes = AxesHandler.clickedAxes(figureUid, lastClick);
        if (axes != null) {
            String text = LegendHandler.getLegendText(axes, polyline);
            String s = (String)JOptionPane.showInputDialog(
                        dialogComponent,
                        Messages.gettext("Enter the text"),
                        Messages.gettext("Set legend text"),
                        JOptionPane.PLAIN_MESSAGE,
                        null,
                        null,
                        text);
            if (s != null) {
                LegendHandler.setLegend(axes, polyline, s);
            }
        }
    }

    /**
     * Implements legend remove action(Callback).
     */
    public void onClickRemove() {

        String axesTo = AxesHandler.clickedAxes(figureUid, lastClick);
        LegendHandler.removeLegend(axesTo, selected);
    }
    
    /**
     * Enter data editor mode.
     */
    public void enterDataEditor() {
        if (!dataEditEnabled && !isLegend) {
            dataEditor.beginEdit(selected);
            dataEditEnabled = true;
        }
    }

    /**
     * Leave data editor mode.
     */
    public void leaveDataEditor() {
        if (dataEditEnabled) {
            dataEditor.endEdit();
            dataEditEnabled = false;
        }
    }

    /**
    * Starts the GED with the property of the Figure.
    */
    public void onClickGED() {
        Inspector.getInspector("figure" , figureUid);
    }
}
