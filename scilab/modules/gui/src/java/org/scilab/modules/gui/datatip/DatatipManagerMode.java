/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.datatip;

import org.scilab.modules.localization.Messages;
import org.scilab.modules.gui.editor.EntityPicker;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
 * Manage the datatip mode activation
 * @author Gustavo Barbosa Libotte
 */
public class DatatipManagerMode {
    private static DatatipManagerMode instance;
    private Integer selectedTip;
    private Integer markColor;

    private Integer figureUid;
    private boolean datatipManagerModeStatus;

    private static String datatipMessage = Messages.gettext("Left click on the curve creates a datatip and right click on the datatip removes it.");

    public DatatipManagerMode() {
        instance = this;
        selectedTip = null;
        figureUid = null;
        datatipManagerModeStatus = false;
    }

    public void setEnabled(boolean b) {
        datatipManagerModeStatus = b;
        if (!datatipManagerModeStatus) {
            GraphicController.getController().setProperty(figureUid, GraphicObjectProperties.__GO_INFO_MESSAGE__, "");
        } else {
            GraphicController.getController().setProperty(figureUid, GraphicObjectProperties.__GO_INFO_MESSAGE__, datatipMessage);
        }
    }

    /**
    * Get instance
    * @return instance
    */
    public static DatatipManagerMode getInstance() {
        return instance;
    }

    /**
    * Get the actual color of mark
    * @return markColor
    */
    public Integer getMarkColor() {
        return markColor;
    }

    /**
    * Set mark color
    * @param newColor scilab color
    */
    public void setMarkColor(Integer newColor) {
        markColor = newColor;
    }

    /**
    * Get the datatip's actual manager mode
    *
    * @return Datatip manager mode status.
    */
    public boolean isEnabled() {
        return datatipManagerModeStatus;
    }


    public void setSelectedTip(Integer uid) {
        markColor = highlightSelected(uid, selectedTip, markColor);
        selectedTip = uid;
    }

    public Integer getSelectedTip() {
        return selectedTip;
    }



    /**
    * Set figure uid
    *
    * @param uid Figure unique identifier.
    */
    public void setFigure(Integer uid) {
        figureUid = uid;
    }


    /**
    * Highlight the datatip mark when selected
    *
    * @param newTip datatip to highlight.
    * @param odTip datatip to restore the orginal color.
    * @param oldTipColor coor to restore.
    * @return the original datatip mark color.
    */
    private Integer highlightSelected(Integer newTip, Integer oldTip, Integer oldTipColor) {

        Integer color = 0;
        if (oldTip != null) {
            GraphicController.getController().setProperty(oldTip, GraphicObjectProperties.__GO_MARK_BACKGROUND__, oldTipColor);
        }
        if (newTip != null) {
            color = (Integer) GraphicController.getController().getProperty(newTip, GraphicObjectProperties.__GO_MARK_BACKGROUND__);
            GraphicController.getController().setProperty(newTip, GraphicObjectProperties.__GO_MARK_BACKGROUND__, -3);
        }
        return color;
    }


    public boolean pickAndHighlight(Integer x, Integer y) {
        Integer pos[] = {x, y};
        Integer datatip = (new EntityPicker()).pickDatatip(figureUid, pos);
        setSelectedTip(datatip);
        return (datatip != null);
    }

    public boolean pickAndDelete(Integer x, Integer y) {
        Integer pos[] = {x, y};
        Integer datatip = (new EntityPicker()).pickDatatip(figureUid, pos);

        if (datatip != null) {
            DatatipDelete.deleteDatatip(datatip);
            if (datatip.equals(selectedTip)) {
                selectedTip = null;
            }
        }
        return (datatip != null);
    }
}
