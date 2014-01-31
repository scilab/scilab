/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.bridge.tab;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTANGLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTNAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTSIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTWEIGHT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TITLE_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TITLE_SCROLL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;

import java.awt.Component;
import java.awt.Font;

import javax.swing.Icon;
import javax.swing.JLabel;
import javax.swing.JTabbedPane;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;
import org.scilab.modules.gui.SwingViewObject;

public class SwingScilabTabGroup extends JTabbedPane implements SwingViewObject {
    private static final long serialVersionUID = 965704348405077905L;

    private Integer id;

    public SwingScilabTabGroup() {
        super();
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public Integer getId() {
        return id;
    }

    public void update(int property, Object value) {

        switch (property) {
            case __GO_VISIBLE__ :
                setVisible((Boolean) value);
                break;
            case __GO_UI_TITLE_POSITION__ :
                Integer pos = (Integer)value;
                switch (Uicontrol.TitlePositionType.intToEnum(pos)) {
                    case BOTTOM:
                        setTabPlacement(JTabbedPane.BOTTOM);
                        break;
                    case LEFT:
                        setTabPlacement(JTabbedPane.LEFT);
                        break;
                    case RIGHT:
                        setTabPlacement(JTabbedPane.RIGHT);
                        break;
                    case TOP:
                    default:
                        setTabPlacement(JTabbedPane.TOP);
                        break;
                }
                break;
            case __GO_UI_TITLE_SCROLL__ :
                if ((Boolean)value) {
                    setTabLayoutPolicy(JTabbedPane.SCROLL_TAB_LAYOUT);
                } else {
                    setTabLayoutPolicy(JTabbedPane.WRAP_TAB_LAYOUT);
                }
                break;
            case __GO_UI_FONTNAME__ :
            case __GO_UI_FONTANGLE__ :
            case __GO_UI_FONTSIZE__ :
            case __GO_UI_FONTWEIGHT__ : {
                for (int i = 0 ; i < getTabCount() ; i++) {
                    setTitleAt(i, null);
                }
            }
        }
    }

    public void insertTab(String title, Icon icon, Component component, String tooltip, int index) {
        super.insertTab(title, icon, component, tooltip, index);
        setTitleAt(index, title);
    }

    public void setTitleAt(int index, String title) {
        //super.setTitleAt(index, title);
        String fontName = (String)GraphicController.getController().getProperty(getId(), __GO_UI_FONTNAME__);
        Double fontSize = (Double)GraphicController.getController().getProperty(getId(), __GO_UI_FONTSIZE__);
        String fontAngle = (String)GraphicController.getController().getProperty(getId(), __GO_UI_FONTANGLE__);
        String fontWeight = (String)GraphicController.getController().getProperty(getId(), __GO_UI_FONTWEIGHT__);

        int fontStyle = Font.PLAIN;
        if (fontAngle.equals("italic")) {
            fontStyle |= Font.ITALIC;
        }

        if (fontWeight.equals("bold")) {
            fontStyle |= Font.BOLD;
        }

        JLabel label = null;
        Component comp = getTabComponentAt(index);
        if (comp instanceof JLabel) {
            label = (JLabel)comp;
        } else {
            label = new JLabel();
        }

        Font oldFont = label.getFont();
        Font font = new Font(
                fontName.equals("") == false ? fontName : oldFont.getFontName(), 
                        fontStyle,
                        fontSize != 0.0 ? fontSize.intValue() : oldFont.getSize());
        
        label.setFont(font);

        if (title != null) {
            label.setText(title);
        }
        setTabComponentAt(index, label);
    }

}
