/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
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

package org.scilab.modules.gui.bridge.popupmenu;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MAX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING_COLNB__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;

import java.awt.Color;
import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.Arrays;

import javax.swing.DefaultComboBoxModel;
import javax.swing.Icon;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.ListCellRenderer;
import javax.swing.UIManager;
import javax.swing.border.Border;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.SwingViewWidget;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.ColorBox;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.SwingScilabListItem;
import org.scilab.modules.gui.widget.Widget;

/**
 * Swing implementation for Scilab PopupMenu in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabPopupMenu extends JComboBox implements SwingViewObject, Widget {

    private static final long serialVersionUID = -4366581303317502544L;

    private Integer uid;

    private CommonCallBack callback;

    private ActionListener defaultActionListener;

    private Border defaultBorder = null;


    private ListCellRenderer defaultRenderer = null;
    private ListCellRenderer listRenderer = null;

    /**
     * Constructor
     */
    public SwingScilabPopupMenu() {
        super();

        defaultRenderer = getRenderer();
        listRenderer = new ListCellRenderer() {
            public Component getListCellRendererComponent(JList list, Object value, int index, boolean isSelected, boolean cellHasFocus) {
                JLabel label = (JLabel) defaultRenderer.getListCellRendererComponent(list, value, index, isSelected, cellHasFocus);

                if (value instanceof SwingScilabListItem) {
                    SwingScilabListItem item = (SwingScilabListItem) value;

                    label.setText(item.toString());
                    label.setIcon(item.getIcon());

                    //index == -1 is for selected item after click
                    //so let standard FG and BG
                    if (index != - 1 && isSelected == false && item.getBackground() != null) {
                        label.setBackground(item.getBackground());
                    }

                    if (index != - 1 && isSelected == false && item.getForeground() != null) {
                        label.setForeground(item.getForeground());
                    }
                } else {
                    label.setText("");
                    label.setIcon(null);
                }
                return label;
            }
        };

        setRenderer(listRenderer);
        /* Bug 3635 fixed: allow arrow keys to browse items */
        putClientProperty("JComboBox.isTableCellEditor", Boolean.TRUE);
        defaultActionListener = new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                Double scilabIndices = (double) getUserSelectedIndex();
                if (scilabIndices == -1) {
                    GraphicController.getController().setProperty(uid, __GO_UI_VALUE__, new Double[] {});
                } else {
                    GraphicController.getController().setProperty(uid, __GO_UI_VALUE__, new Double[] {scilabIndices});
                }

                if (callback != null) {
                    callback.actionPerformed(null);
                }
            }
        };
        addActionListener(defaultActionListener);
    }

    /**
     * Draws a swing Scilab tab
     * @see org.scilab.modules.gui.uielement.UIElement#draw()
     */
    public void draw() {
        this.setVisible(true);
    }

    /**
     * Gets the dimensions (width and height) of a swing Scilab tab
     * @return the dimensions of the tab
     * @see org.scilab.modules.gui.uielement.UIElement#getDims()
     */
    public Size getDims() {
        return new Size(getWidth(), getHeight());
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab tab
     * @return the position of the tab
     * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
     */
    public Position getPosition() {
        return PositionConverter.javaToScilab(getLocation(), getSize(), getParent());
    }

    /**
     * Sets the dimensions (width and height) of a swing Scilab tab
     * @param newSize the dimensions we want to set to the tab
     * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public void setDims(Size newSize) {
        setSize(newSize.getWidth(), newSize.getHeight());
        doLayout(); /* Needed !! because PopupMenu is badly drawn else */
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab tab
     * @param newPosition the position we want to set to the tab
     * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public void setPosition(Position newPosition) {
        Position javaPosition = PositionConverter.scilabToJava(newPosition, getDims(), getParent());
        setLocation(javaPosition.getX(), javaPosition.getY());
    }

    /**
     * Add a callback to the PopupMenu
     * @param callback the callback to set.
     */
    public void setCallback(CommonCallBack callback) {
        this.callback = callback;
    }

    /**
     * Setter for MenuBar
     * @param menuBarToAdd the MenuBar associated to the Tab.
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        /* Unimplemented for PopupMenus */
        throw new UnsupportedOperationException();
    }

    /**
     * Setter for ToolBar
     * @param toolBarToAdd the ToolBar associated to the Tab.
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        /* Unimplemented for PopupMenus */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for MenuBar
     * @return MenuBar: the MenuBar associated to the Tab.
     */
    public MenuBar getMenuBar() {
        /* Unimplemented for PopupMenus */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for ToolBar
     * @return ToolBar: the ToolBar associated to the Tab.
     */
    public ToolBar getToolBar() {
        /* Unimplemented for PopupMenus */
        throw new UnsupportedOperationException();
    }

    /**
     * Get the text if the PopupMenu items
     * @return the items
     * @see org.scilab.modules.gui.widget.Widget#getText()
     */
    public String getText() {
        /* Unimplemented for PopupMenus */
        throw new UnsupportedOperationException();
    }

    /**
     * Set the text of the PopupMenu items
     * @param text the text of the items
     * @see org.scilab.modules.gui.widget.Widget#setText(java.lang.String)
     */
    public void setText(String text) {
        /* Unimplemented for PopupMenus */
        throw new UnsupportedOperationException();
    }

    /**
     * Set the horizontal alignment for the PopupMenu text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        // Nothing to do here
    }

    /**
     * Set the vertical alignment for the PopupMenu text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        // Nothing to do here
    }

    /**
     * Set the selected index of the PopupMenu
     * @param index the index of the item to be selected
     */
    public void setUserSelectedIndex(int index) {
        /* Remove the listener to avoid the callback to be executed */
        removeActionListener(defaultActionListener);

        // Scilab indices in Value begin at 1 and Java indices begin at 0
        if (index >= 0 && index <= getItemCount()) {
            setSelectedIndex(index - 1);
        }

        /* Put back the listener */
        addActionListener(defaultActionListener);
    }

    /**
     * Get the selected index of the PopupMenu
     * @return the index of the item selected
     */
    public int getUserSelectedIndex() {
        int index = getSelectedIndex();
        if (index == - 1) {
            return -1;
        }

        return index + 1;
    }

    /**
     * Get the text of all the PopupMenu items
     * @return the text items
     */
    public String[] getAllItemsText() {
        String[] retValue = new String[getItemCount()];
        for (int i = 0; i < getItemCount(); i++) {
            retValue[i] = getItemAt(i).toString();
        }
        return retValue;

    }

    /**
     * Get the number of items in the PopupMenu
     * @return the number of items
     */
    public int getNumberOfItems() {
        return getItemCount();
    }

    /**
     * Set the text of the PopupMenu items
     * @param text the text of the items
     */
    public void setText(String[] text) {
        DefaultComboBoxModel model = new DefaultComboBoxModel();

        //get numbers of columns
        GraphicController controller = GraphicController.getController();
        Integer nbCol = (Integer) controller.getProperty(getId(), __GO_UI_STRING_COLNB__);

        /* Remove the listener to avoid the callback to be executed */
        removeActionListener(defaultActionListener);

        boolean tryColorBox = true;
        boolean tryColor = true;
        boolean tryIcon = true;
        int nbRow = text.length / nbCol;

        for (int i = 0; i < nbRow; i++) {
            Icon icon = null;
            String str = null;
            Color background = null;
            Color foreground = null;

            //4 cols :
            // - 1st icon or colorBox
            // - 2nd text
            // - 3rd BG
            // - 4th FG

            //3 cols :  2 cases
            // - 1st icon or colorBox
            // - 2nd text
            // - 3rd BG
            //or
            // - 1st text
            // - 2nd BG
            // - 3rd FG

            //2 cols : 2 cases
            // - 1st icon or colorBox
            // - 2nd text
            //or
            // - 1st text
            // - 2nd BG

            if (tryColorBox) { //color
                try {
                    //format #FFFFFF
                    if (text[i].startsWith("#") == false) {
                        throw new NumberFormatException();
                    }

                    Color color = Color.decode(text[i]);
                    icon = ColorBox.createColorBox(16, 16, color);
                } catch (NumberFormatException e) {
                    tryColorBox = false;
                    model.removeAllElements();
                    //restart loop with icon
                    i = -1;
                    continue;
                }
            }

            if (tryIcon) {
                try {
                    icon = FindIconHelper.loadIcon(text[i]);
                } catch (IOException e) {
                    tryIcon = false;
                    model.removeAllElements();
                    //restart loop with text only
                    i = -1;
                    continue;
                }
            }

            if (tryColor) {
                try {
                    int colIndex = 0;
                    if (tryColorBox || tryIcon) {
                        colIndex = 1;
                    }

                    str = text[(nbRow * colIndex) + i];
                    if (nbCol > (1 + colIndex)) {
                        if (text[nbRow * (1 + colIndex) + i].startsWith("#") == false) {
                            throw new NumberFormatException();
                        }

                        background = Color.decode(text[nbRow * (1 + colIndex) + i]);
                        if (nbCol > (2 + colIndex)) {
                            if (text[nbRow * (2 + colIndex) + i].startsWith("#") == false) {
                                throw new NumberFormatException();
                            }

                            foreground = Color.decode(text[nbRow * (2 + colIndex) + i]);
                        }
                    }

                    //add item in list box
                    model.addElement(new SwingScilabListItem(str, icon, background, foreground));
                } catch (NumberFormatException e) {
                    tryColor = false;
                    model.removeAllElements();
                    //restart loop with text only
                    i = -1;
                    continue;
                }
            } else { //text only
                for (int j = 0; j < nbCol; j++) {
                    model.addElement(new SwingScilabListItem(text[nbRow * j + i], icon, background, foreground));
                }
            }
        }

        //reset selected index
        setSelectedIndex(-1);
        setModel(model);
        invalidate();
        //take care to add listener BEFORE set Property to avoid multiple remove and multiple add
        addActionListener(defaultActionListener);
    }

    /**
     * Set the Relief of the PopupMenu
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public void setRelief(String reliefType) {
        if (defaultBorder == null) {
            defaultBorder = getBorder();
        }
        setBorder(ScilabRelief.getBorderFromRelief(reliefType, defaultBorder));
    }

    /**
     * Destroy the PopupMenu
     */
    public void destroy() {
        ScilabSwingUtilities.removeFromParent(this);
    }

    /**
     * Setter for InfoBar
     * @param infoBarToAdd the InfoBar associated to the PopupMenu.
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        /* Unimplemented for PopupMenus */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for InfoBar
     * @return the InfoBar associated to the PopupMenu.
     */
    public TextBox getInfoBar() {
        /* Unimplemented for PopupMenus */
        throw new UnsupportedOperationException();
    }


    /**
     * Set the UID
     * @param id the UID
     */
    public void setId(Integer id) {
        uid = id;
    }

    /**
     * Get the UID
     * @return the UID
     */
    public Integer getId() {
        return uid;
    }

    /**
     * Generic update method
     * @param property property name
     * @param value property value
     */
    public void update(int property, Object value) {
        switch (property) {
            case __GO_UI_STRING__: {
                setText((String[]) value);
                break;
            }
            case __GO_UI_MAX__: {
                Integer val = ((Double)value).intValue();
                if (val > 1) {
                    char[] chars = new char[val];
                    Arrays.fill(chars, '*');
                    String proto = new String(chars);
                    setPrototypeDisplayValue(proto);
                }
                break;
            }
            case __GO_UI_VALUE__: {
                Double[] doubleValue = ((Double[]) value);

                //[] or 0 -> no selection
                if (doubleValue.length == 0 || doubleValue[0] == 0) {
                    setUserSelectedIndex(0);
                    return;
                }

                int[] intValue = new int[doubleValue.length];
                for (int k = 0; k < doubleValue.length; k++) {
                    intValue[k] = doubleValue[k].intValue();
                }

                // Update selected items in the popupmenu
                setUserSelectedIndex(intValue[0]);
                break;
            }
            default: {
                SwingViewWidget.update(this, property, value);
                break;
            }
        }
    }

    public void resetBackground() {
        Color color = (Color) UIManager.getLookAndFeelDefaults().get("ComboBox.background");
        if (color != null) {
            setBackground(color);
        }
    }

    public void resetForeground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("ComboBox.foreground");
        if (color != null) {
            setForeground(color);
        }
    }

    public void setEmptyText() {
        setText(new String[] {});
    }
}
