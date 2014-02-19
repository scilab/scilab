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

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACKTYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACK__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTANGLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTNAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTSIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTWEIGHT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TITLE_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TITLE_SCROLL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;

import java.awt.Component;
import java.awt.Font;

import javax.swing.Icon;
import javax.swing.JLabel;
import javax.swing.JTabbedPane;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.CallBack;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.Size;

public class SwingScilabTabGroup extends JTabbedPane implements SwingViewObject {
    private static final long serialVersionUID = 965704348405077905L;
    private Integer id;
    private CommonCallBack callback = null;
    private ChangeListener listener = null;

    public SwingScilabTabGroup() {
        super();

        listener = new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                GraphicController controller = GraphicController.getController();
                SwingScilabTabGroup me = (SwingScilabTabGroup)e.getSource();
                int index = me.getSelectedIndex();

                //get current id
                Double[] prev = (Double[])controller.getProperty(me.getId(), __GO_UI_VALUE__);
                Integer previous = null;
                if (prev != null && prev[0] != null) {
                    previous = ((SwingViewObject)getComponentAt(prev[0].intValue() - 1)).getId();
                }
                //get next id
                Integer next = ((SwingViewObject)getComponentAt(index)).getId();
                //update value, string and children visible
                updateModelProperties(previous, next, index);

                //call callback function if exists
                if (callback != null) {
                    callback.actionPerformed(null);
                }
            }
        };

        addChangeListener(listener);
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
                break;
            }
            case __GO_POSITION__ : {
                Double[] positions = (Double[]) value;
                setSize(positions[2].intValue(), positions[3].intValue());
                Position javaPosition = PositionConverter.scilabToJava(new Position(positions[0].intValue(), positions[1].intValue()),
                                        new Size(getSize().width, getSize().height), getParent());
                setLocation(javaPosition.getX(), javaPosition.getY());
                break;
            }
            case __GO_UI_VALUE__: {
                Double[] doubleValue = ((Double[]) value);
                if (doubleValue.length == 0) {
                    return;
                }

                int[] intValue = new int[doubleValue.length];
                for (int k = 0; k < doubleValue.length; k++) {
                    intValue[k] = doubleValue[k].intValue();
                }

                //if intValue[0] is out of bounds, do not update view but let "wrong" value in model
                if (intValue[0] > 0 && intValue[0] <= getTabCount()) {
                    setSelectedIndex(intValue[0] - 1);
                }
                break;
            }
            case __GO_UI_STRING__: {
                //set tab by his name
                String name = ((String[])value)[0];
                for (int i = 0 ; i < getTabCount() ; i++) {
                    JLabel current = (JLabel)getTabComponentAt(i);
                    if (current != null && current.getText() != null && current.getText().equals(name)) {
                        setSelectedIndex(i);
                        break;
                    }
                }
                break;
            }
            case __GO_CALLBACK__: {
                int cbType = (Integer) GraphicController.getController().getProperty(getId(), __GO_CALLBACKTYPE__);
                callback = CommonCallBack.createCallback((String) value, cbType, getId());
                break;
            }
            case __GO_CALLBACKTYPE__: {
                String cbString = (String) GraphicController.getController().getProperty(getId(), __GO_CALLBACK__);
                if ((Integer) value == CallBack.UNTYPED) {
                    //Disable callback
                    callback = null;
                } else {
                    callback = CommonCallBack.createCallback(cbString, (Integer) value, getId());
                }
                break;
            }
            default : {
            }
        }
    }

    public void insertTab(String title, Icon icon, Component component, String tooltip, int index) {
        super.insertTab(title, icon, component, tooltip, index);
        setTitleAt(index, title);
    }

    private void updateModelProperties(Integer previous, Integer next, Integer newIndex) {
        GraphicController controller = GraphicController.getController();
        if (previous != null && previous != 0) {
            //set visible false en previous selected children
            controller.setProperty(previous, __GO_VISIBLE__, false);
        }

        if (next != null && next != 0) {
            //set visible true on current selected children
            controller.setProperty(next, __GO_VISIBLE__, true);
        }

        //update value with new selected tab index ( 1-indexed )
        controller.setProperty(getId(), __GO_UI_VALUE__, new Double[] {(double)(newIndex + 1)});
        //update string with new selected tab name
        String[] tabName = (String[])controller.getProperty(next, __GO_UI_STRING__);
        controller.setProperty(getId(), __GO_UI_STRING__, tabName);

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
