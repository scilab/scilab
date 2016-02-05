/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2010-2011 - DIGITEO - Vincent COUVERT
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

package org.scilab.modules.gui.bridge.listbox;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BACKGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LISTBOXTOP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MAX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MIN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING_COLNB__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;

import java.awt.Color;
import java.awt.Component;
import java.awt.Font;
import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;
import java.io.IOException;

import javax.swing.DefaultListModel;
import javax.swing.Icon;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JScrollPane;
import javax.swing.ListCellRenderer;
import javax.swing.ListSelectionModel;
import javax.swing.UIManager;
import javax.swing.border.Border;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

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
 * Swing implementation for Scilab ListBox in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabListBox extends JScrollPane implements SwingViewObject, Widget {

    private static final long serialVersionUID = 3507396207331058895L;

    private static final int COLORS_COEFF = 255;

    private Integer uid;

    private Border defaultBorder = null;

    private CommonCallBack callback;

    private ListSelectionListener listListener;

    private AdjustmentListener adjustmentListener;

    /**
     * the JList we use
     */
    private JList list;

    private ListCellRenderer defaultRenderer = null;
    private ListCellRenderer listRenderer = null;

    /**
     * Constructor
     */
    public SwingScilabListBox() {
        super();
        getViewport().add(getList());
        defaultRenderer = getList().getCellRenderer();

        listRenderer = new ListCellRenderer() {
            public Component getListCellRendererComponent(JList list, Object value, int index, boolean isSelected, boolean cellHasFocus) {
                JLabel label = (JLabel) defaultRenderer.getListCellRendererComponent(list, value, index, isSelected, cellHasFocus);

                if (value instanceof SwingScilabListItem) {
                    SwingScilabListItem item = (SwingScilabListItem) value;
                    label.setText(item.toString());
                    label.setIcon(item.getIcon());

                    if (isSelected == false && item.getBackground() != null) {
                        label.setBackground(item.getBackground());
                    }

                    if (isSelected == false && item.getForeground() != null) {
                        label.setForeground(item.getForeground());
                    }
                } else {
                    label.setText("");
                    label.setIcon(null);
                }
                return label;
            }
        };

        getList().setCellRenderer(listRenderer);

        listListener = new ListSelectionListener() {
            public void valueChanged(ListSelectionEvent e) {

                //value not ready
                if (e.getValueIsAdjusting()) {
                    return;
                }

                // Scilab indices in Value begin at 1 and Java indices begin at 0
                int[] javaIndices = getList().getSelectedIndices().clone();
                Double[] scilabIndices = new Double[javaIndices.length];
                for (int i = 0; i < getList().getSelectedIndices().length; i++) {
                    scilabIndices[i] = (double) javaIndices[i] + 1;
                }

                GraphicController.getController().setProperty(uid, __GO_UI_VALUE__, scilabIndices);
                if (callback != null) {
                    callback.actionPerformed(null);
                }
            }
        };
        getList().addListSelectionListener(listListener);

        adjustmentListener = new AdjustmentListener() {
            public void adjustmentValueChanged(AdjustmentEvent arg0) {
                int listboxtopValue = getList().getUI().locationToIndex(getList(), getViewport().getViewPosition()) + 1;
                GraphicController.getController().setProperty(uid, __GO_UI_LISTBOXTOP__, new Integer[] { listboxtopValue });
            }
        };
        getVerticalScrollBar().addAdjustmentListener(adjustmentListener);
    }

    /**
     * To get the Background color of the element.
     * @return color the Color
     */
    public Color getBackground() {
        return getList().getBackground();
    }

    /**
     * To get the Font of the element.
     * @return font the Font
     */
    public Font getFont() {
        return getList().getFont();
    }

    /**
     * To get the Foreground color of the element.
     * @return color the Color
     */
    public Color getForeground() {
        return getList().getForeground();
    }

    /**
     * To set the Background color of the element.
     * @param color the Color
     */
    public void setListBackground(Color color) {
        getList().setBackground(color);
    }

    /**
     * To set the Font of the element.
     * @param font the Font
     */
    public void setFont(Font font) {
        getList().setFont(font);
    }

    /**
     * To set the Foreground color of the element.
     * @param color the Color
     */
    public void setForeground(Color color) {
        getList().setForeground(color);
    }

    /**
     * Draws a swing Scilab tab
     * @see org.scilab.modules.gui.uielement.UIElement#draw()
     */
    public void draw() {
        this.setVisible(true);
        this.doLayout();
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
     * Sets the visibility status of an UIElement
     * @param newVisibleState the visibility status we want to set for the
     * UIElement (true if the UIElement is visible, false if not)
     */
    public void setVisible(boolean newVisibleState) {
        super.setVisible(newVisibleState);
        list.setVisible(newVisibleState);
    }

    /**
     * Sets the enable status of an UIElement
     * @param newEnableState the enable status we want to set for the UIElement
     * (true if the UIElement is enabled, false if not)
     */
    public void setEnabled(boolean newEnableState) {
        if (newEnableState == isEnabled()) {
            return;
        }

        super.setEnabled(newEnableState);
        getList().setEnabled(newEnableState);
        if (newEnableState) {
            if (listListener != null) {
                getList().addListSelectionListener(listListener);
            }
        } else {
            if (listListener != null) {
                getList().removeListSelectionListener(listListener);
            }
        }
    }

    /**
     * Add a callback to the CheckBox
     * @param cb the callback to set.
     */
    public void setCallback(CommonCallBack cb) {
        this.callback = cb;
    }

    /**
     * Setter for MenuBar
     * @param menuBarToAdd the MenuBar associated to the Tab.
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        /* Unimplemented for ListBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Setter for ToolBar
     * @param toolBarToAdd the ToolBar associated to the Tab.
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        /* Unimplemented for ListBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for MenuBar
     * @return MenuBar: the MenuBar associated to the Tab.
     */
    public MenuBar getMenuBar() {
        /* Unimplemented for ListBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for ToolBar
     * @return ToolBar: the ToolBar associated to the Tab.
     */
    public ToolBar getToolBar() {
        /* Unimplemented for ListBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Get the first item text
     * @return the items
     * @see org.scilab.modules.gui.widget.Widget#getText()
     */
    public String getText() {
        /* Unimplemented for ListBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Get the text of all the list items
     * @return the items
     * @see org.scilab.modules.gui.listbox.ListBox#getAllItemsText()
     */
    public String[] getAllItemsText() {
        String[] retValue = new String[getList().getModel().getSize()];
        for (int i = 0; i < getList().getModel().getSize(); i++) {
            retValue[i] = getList().getModel().getElementAt(i).toString();
        }
        return retValue;
    }

    /**
     * Get the number of items in the list
     * @return the number of items
     * @see org.scilab.modules.gui.listbox.ListBox#getNumberOfItems()
     */
    public int getNumberOfItems() {
        return getList().getModel().getSize();
    }

    /**
     * Set the text of the list items
     * @param text the text of the items
     * @see org.scilab.modules.gui.widget.Widget#setText(java.lang.String)
     */
    public void setText(String text) {
        DefaultListModel model = new DefaultListModel();
        model.addElement(text);
        getList().setModel(model);
        revalidate();
    }

    /**
     * Set the text of the list items
     * @param text the text of the items
     * @see org.scilab.modules.gui.widget.Widget#setText(java.lang.String)
     */
    public void setText(String[] text) {
        DefaultListModel model = new DefaultListModel();

        // check numbers of columns
        GraphicController controller = GraphicController.getController();
        Integer nbCol = (Integer) controller.getProperty(getId(), __GO_UI_STRING_COLNB__);

        /* Remove the listener to avoid the callback to be executed */
        getList().removeListSelectionListener(listListener);

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
                    model.clear();
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
                    model.clear();
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
                    model.clear();
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
        getList().setSelectedIndex(-1);
        getList().setModel(model);
        invalidate();
        //take care to add listener BEFORE set Property to avoid multiple remove and multiple add
        getList().addListSelectionListener(listListener);
    }

    public void setEmptyText() {
        setText(new String[] {});
    }

    /**
     * Set the horizontal alignment for the ListBox text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        // Nothing to do here
    }

    /**
     * Set the vertical alignment for the ListBox text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        // Nothing to do here
    }

    /**
     * Set if more than one item can be selected in a ListBox
     * @param status true if multiple selection is enabled
     */
    public void setMultipleSelectionEnabled(boolean status) {
        if (status) {
            getList().setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
        } else {
            getList().setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        }
    }

    /**
     * Set the selected indices of the ListBox
     * @param indices the indices of the items to be selected
     */
    public void setSelectedIndices(int[] indices) {
        // Scilab indices in Value begin at 1 and Java indices begin at 0
        int[] javaIndices = indices.clone();
        for (int i = 0; i < javaIndices.length; i++) {
            javaIndices[i] = javaIndices[i] - 1;
        }

        /* Remove the listener to avoid the callback to be executed */
        if (listListener != null) {
            getList().removeListSelectionListener(listListener);
        }

        getList().setSelectedIndices(javaIndices);

        /* Put back the listener */
        if (listListener != null) {
            getList().addListSelectionListener(listListener);
        }
    }

    /**
     * Get the selected indices of the ListBox
     * @return the indices of the items selected
     */
    public int[] getSelectedIndices() {
        // Scilab indices in Value begin at 1 and Java indices begin at 0
        int[] javaIndices = getList().getSelectedIndices().clone();
        int[] scilabIndices = javaIndices.clone();
        for (int i = 0; i < getList().getSelectedIndices().length; i++) {
            scilabIndices[i] = scilabIndices[i] + 1;
        }
        return scilabIndices;
    }

    /**
     * Get the number of items selected in the ListBox
     * @return the number of items selected
     */
    public int getSelectionSize() {
        return getList().getSelectedIndices().length;
    }

    /**
     * Get or create the list component
     * @return the list
     */
    private JList getList() {
        if (list == null) {
            list = new JList();
            list.setLayoutOrientation(JList.VERTICAL);
            list.setModel(new DefaultListModel());
        }
        return list;
    }

    /**
     * Set the Relief of the ListBox
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public void setRelief(String reliefType) {
        if (defaultBorder == null) {
            defaultBorder = getBorder();
        }
        setBorder(ScilabRelief.getBorderFromRelief(reliefType, defaultBorder));
    }

    /**
     * Destroy the ListBox
     */
    public void destroy() {
        ScilabSwingUtilities.removeFromParent(this);
    }

    /**
     * Setter for InfoBar
     * @param infoBarToAdd the InfoBar associated to the ListBox.
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        /* Unimplemented for ListBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for InfoBar
     * @return the InfoBar associated to the ListBox.
     */
    public TextBox getInfoBar() {
        /* Unimplemented for ListBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Adjusts the view so that the element given by index is displayed at the
     * top of the ListBox.
     * @param index the index of the element to be displayed at the top of the
     * ListBox.
     */
    public void setListBoxTop(int index) {
        getVerticalScrollBar().removeAdjustmentListener(adjustmentListener);
        if (index > 0 & index != getListBoxTop() && getList().getModel().getSize() != 0) {
            getViewport().setViewPosition(getList().getUI().indexToLocation(getList(), index - 1));
            doLayout();
        }
        getVerticalScrollBar().addAdjustmentListener(adjustmentListener);
    }

    /**
     * Gets the index of the element displayed at the top of the ListBox
     * @return the index of the element displayed at the top of the ListBox
     */
    public int getListBoxTop() {
        return getList().getUI().locationToIndex(getList(), getViewport().getViewPosition()) + 1;
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
        GraphicController controller = GraphicController.getController();
        switch (property) {
            case __GO_UI_VALUE__: {
                Double[] indexes = (Double[]) value;
                int[] index = new int[indexes.length];
                for (int i = 0; i < indexes.length; i++) {
                    index[i] = indexes[i].intValue();
                }
                setSelectedIndices(index);
                break;
            }
            case __GO_UI_BACKGROUNDCOLOR__: {
                Double[] allColors = ((Double[]) value);
                if (allColors[0] != -1) {
                    setListBackground(new Color((int) (allColors[0] * COLORS_COEFF), (int) (allColors[1] * COLORS_COEFF), (int) (allColors[2] * COLORS_COEFF)));
                } else {
                    resetBackground();
                }
                break;
            }
            case __GO_UI_STRING__: {
                // Listboxes manage string vectors
                setText((String[]) value);
                break;
            }
            case __GO_UI_MAX__: {
                Double maxValue = ((Double) value);
                // Enable/Disable multiple selection
                double minValue = (Double) controller.getProperty(uid, __GO_UI_MIN__);
                setMultipleSelectionEnabled(maxValue - minValue > 1);
                break;
            }
            case __GO_UI_MIN__: {
                Double minValue = ((Double) value);
                // Enable/Disable multiple selection
                Double maxValue = (Double) controller.getProperty(uid, __GO_UI_MAX__);
                setMultipleSelectionEnabled(maxValue - minValue > 1);
                break;
            }
            case __GO_UI_LISTBOXTOP__: {
                Integer[] listboxtopValue = ((Integer[]) value);
                if (listboxtopValue.length > 0) {
                    setListBoxTop(listboxtopValue[0]);
                }
                break;
            }
            default: {
                SwingViewWidget.update(this, property, value);
                break;
            }
        }
    }

    public void resetBackground() {
        Color color = (Color) UIManager.getLookAndFeelDefaults().get("List.background");
        if (color != null) {
            getList().setBackground(color);
        }
    }

    public void resetForeground() {
        Color color = (Color) UIManager.getLookAndFeelDefaults().get("List.foreground");
        if (color != null) {
            getList().setForeground(color);
        }
    }
}
