/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
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

package org.scilab.modules.gui.bridge.tab;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACKTYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACK__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CHILDREN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TAG__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTANGLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTNAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTSIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTWEIGHT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TAB_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TAB_VALUE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TITLE_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TITLE_SCROLL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;

import java.awt.Color;
import java.awt.Component;
import java.awt.Font;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JTabbedPane;
import javax.swing.UIManager;
import javax.swing.border.Border;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.CallBack;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;
import org.scilab.modules.gui.SwingView;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.SwingViewWidget;
import org.scilab.modules.gui.dockable.Dockable;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.widget.Widget;

public class SwingScilabTabGroup extends JTabbedPane implements SwingViewObject, Widget {
    private static final long serialVersionUID = 965704348405077905L;
    private Integer id;
    private Border defaultBorder = null;
    private CommonCallBack callback = null;
    private ChangeListener listener = null;

    public SwingScilabTabGroup() {
        super();

        listener = new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                //update value, string and children visible
                updateModelProperties();

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
            case __GO_VISIBLE__:
                setVisible((Boolean) value);
                break;
            case __GO_UI_TITLE_POSITION__:
                Integer pos = (Integer) value;
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
            case __GO_UI_TITLE_SCROLL__:
                if ((Boolean) value) {
                    setTabLayoutPolicy(JTabbedPane.SCROLL_TAB_LAYOUT);
                } else {
                    setTabLayoutPolicy(JTabbedPane.WRAP_TAB_LAYOUT);
                }
                break;
            case __GO_UI_FONTNAME__:
            case __GO_UI_FONTANGLE__:
            case __GO_UI_FONTSIZE__:
            case __GO_UI_FONTWEIGHT__: {
                for (int i = 0; i < getTabCount(); i++) {
                    setTitleAt(i, null);
                }
                break;
            }
            case __GO_POSITION__: {
                Double[] positions = (Double[]) value;
                setSize(positions[2].intValue(), positions[3].intValue());
                Position javaPosition = PositionConverter.scilabToJava(new Position(positions[0].intValue(), positions[1].intValue()), new Size(getSize().width, getSize().height), getParent());
                setLocation(javaPosition.getX(), javaPosition.getY());
                break;
            }
            case __GO_UI_VALUE__: {
                Double[] doubleValue = ((Double[]) value);
                if (doubleValue.length == 0) {
                    return;
                }

                Integer val = doubleValue[0].intValue();
                //if intValue[0] is out of bounds, do not update view but let "wrong" value in model
                if (val > 0 && val <= getTabCount()) {
                    setSelectedIndex(val - 1);
                }
                break;
            }
            case __GO_UI_STRING__: {
                //set tab by his name
                String name = ((String[]) value)[0];
                for (int i = 0; i < getTabCount(); i++) {
                    JLabel current = (JLabel) getTabComponentAt(i);
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
            default: {
                SwingViewWidget.update(this, property, value);
            }
        }
    }

    public void insertTab(String title, Icon icon, Component component, String tooltip, int index) {
        super.insertTab(title, icon, component, tooltip, index);
        setTitleAt(index, title);
        setSelectedIndex(index);
    }

    public void updateModelProperties() {
        GraphicController controller = GraphicController.getController();

        int index = getSelectedIndex();

        //get current id
        Double[] prev = (Double[]) controller.getProperty(getId(), __GO_UI_VALUE__);
        Integer previous = null;
        if (prev != null && prev[0] != null) {
            if (prev[0] == 0) {
                prev[0] = 1.0;
            }
            previous = ((SwingViewObject) getComponentAt(prev[0].intValue() - 1)).getId();
        }
        //get next id
        Integer next = ((SwingViewObject) getComponentAt(index)).getId();

        if (previous != null && previous != 0) {
            //set visible false en previous selected children
            controller.setProperty(previous, __GO_VISIBLE__, false);
        }

        if (next != null && next != 0) {
            //set visible true on current selected children
            controller.setProperty(next, __GO_VISIBLE__, true);
        }

        //update value with new selected tab index ( 1-indexed )
        controller.setProperty(getId(), __GO_UI_TAB_VALUE__, new Double[] { (double) (index + 1) });
        //update string with new selected tab name
        String tabName = (String) controller.getProperty(next, __GO_TAG__);
        controller.setProperty(getId(), __GO_UI_TAB_STRING__, new String[] {tabName});
    }

    public void setTitleAt(int index, String title) {
        //super.setTitleAt(index, title);
        String fontName = (String) GraphicController.getController().getProperty(getId(), __GO_UI_FONTNAME__);
        Double fontSize = (Double) GraphicController.getController().getProperty(getId(), __GO_UI_FONTSIZE__);
        String fontAngle = (String) GraphicController.getController().getProperty(getId(), __GO_UI_FONTANGLE__);
        String fontWeight = (String) GraphicController.getController().getProperty(getId(), __GO_UI_FONTWEIGHT__);

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
            label = (JLabel) comp;
        } else {
            label = new JLabel();
        }

        Font oldFont = label.getFont();
        Font font = new Font(fontName.equals("") == false ? fontName : oldFont.getFontName(), fontStyle, fontSize != 0.0 ? fontSize.intValue() : oldFont.getSize());

        label.setFont(font);

        if (title != null) {
            label.setText(title);
        }
        setTabComponentAt(index, label);
    }

    public Boolean isTabActive(Component tab) {
        if (getSelectedComponent() == tab) {
            return true;
        }

        return false;
    }

    public Integer getIndex(Component component) {
        for (int i = 0 ; i < getTabCount() ; i++) {
            if (getComponentAt(i) == component) {
                return i;
            }
        }

        return -1;
    }

    @Override
    public void setEnabledAt(int index, boolean enabled) {
        super.setEnabledAt(index, enabled);
        //update tab label to show enabled state
        getTabComponentAt(index).setEnabled(enabled);

        //disable current selected tab, select another enable tab if possible
        if (enabled == false && index == getSelectedIndex()) {
            //looking first enable tab and select it.
            for (int i = 0 ; i < getTabCount() ; i++) {
                if (getComponentAt(i).isEnabled()) {
                    setSelectedIndex(i);
                    break;
                }
            }
        }
    }

    public void setEnabled(boolean status) {
        if (status == isEnabled()) {
            return;
        }

        if (status) {
            // Enable the frame
            super.setEnabled(status);
            // Enable its children according to their __GO_UI_ENABLE__ property
            Integer[] children = (Integer[]) GraphicController.getController().getProperty(getId(), __GO_CHILDREN__);
            for (int kChild = 0; kChild < children.length; kChild++) {
                Boolean childStatus = (Boolean) GraphicController.getController().getProperty(children[kChild], __GO_UI_ENABLE__);
                SwingView.getFromId(children[kChild]).update(__GO_UI_ENABLE__, childStatus);
            }
        } else {
            // Disable the frame
            super.setEnabled(status);
            // Disable its children
            Component[] components = getComponents();
            for (int compIndex = 0; compIndex < components.length; compIndex++) {
                components[compIndex].setEnabled(false);
            }
        }
    }

    public int addMember(Dockable member) {
        throw new UnsupportedOperationException();
    }

    public void addToolBar(ToolBar toolBarToAdd) {
        throw new UnsupportedOperationException();
    }

    public void addMenuBar(MenuBar menuBarToAdd) {
        throw new UnsupportedOperationException();
    }

    public void addInfoBar(TextBox infoBarToAdd) {
        throw new UnsupportedOperationException();
    }

    public Size getDims() {
        return new Size(this.getSize().width, this.getSize().height);
    }

    public void setDims(Size newSize) {
        this.setSize(newSize.getWidth(), newSize.getHeight());
    }

    public Position getPosition() {
        return PositionConverter.javaToScilab(getLocation(), getSize(), getParent());
    }

    public void setPosition(Position newPosition) {
        Position javaPosition = PositionConverter.scilabToJava(newPosition, getDims(), getParent());
        setLocation(javaPosition.getX(), javaPosition.getY());
    }

    public void draw() {
        this.setVisible(true);
        this.doLayout();
    }

    public MenuBar getMenuBar() {
        throw new UnsupportedOperationException();
    }

    public ToolBar getToolBar() {
        throw new UnsupportedOperationException();
    }

    public TextBox getInfoBar() {
        throw new UnsupportedOperationException();
    }

    public void resetBackground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("TabbedPane.background");
        if (color != null) {
            setBackground(color);
        }
    }

    public void setHorizontalAlignment(String alignment) {
    }

    public void setVerticalAlignment(String alignment) {
    }

    public void setText(String text) {
    }

    public void setEmptyText() {
        setText(null);
    }

    public String getText() {
        return null;
    }

    public void setCallback(CommonCallBack callback) {
    }

    public void setRelief(String reliefType) {
        if (defaultBorder == null) {
            defaultBorder = getBorder();
        }
        setBorder(ScilabRelief.getBorderFromRelief(reliefType, defaultBorder));
    }

    public void destroy() {
        getParent().remove(this);
        this.setVisible(false);
    }

    public void setIconAt(int index, String iconFile) {
        try {
            File file = new File(iconFile);
            if (file.exists() == false) {
                String filename = FindIconHelper.findImage(iconFile);
                file = new File(filename);
            }

            JLabel label = (JLabel)getTabComponentAt(index);
            if (label != null) {
                label.setIcon(new ImageIcon(ImageIO.read(file)));
            }
        } catch (IOException e) {
            super.setIconAt(index, null);
        }
    }

    public void resetForeground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("TabbedPane.foreground");
        if (color != null) {
            setForeground(color);
        }
    }
}
