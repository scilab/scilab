/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Han DONG
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.gui.bridge.uitable;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING_COLNB__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;

import java.awt.Color;
import java.awt.Font;

import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.UIManager;
import javax.swing.border.Border;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.SwingViewWidget;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.uitable.SimpleUiTable;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.ScilabAlignment;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab UiTable in GUIs
 * @author Han DONG
 */
public class SwingScilabUiTable extends JScrollPane implements SwingViewObject, SimpleUiTable {

    private static final long serialVersionUID = -5497171010652701217L;

    private Integer uid;

    private Border defaultBorder = null;

    private JTable uiTable;
    private JList rowHeader;

    private Object[] colNames = {};
    private Object[] rowNames = {};
    private Object[][] data = {};

    private int nCol;
    private int nRow;

    private JLabel label;

    /**
     * Constructor
     */
    public SwingScilabUiTable() {
        super();
        getViewport().add(getUiTable());
        setRowHeaderView(getCustomRowHeader());
    }

    /**
     * Apply a new font for the uiTable.
     * @param font new font to use.
     */
    public void setFont(Font font) {
        getLabel().setFont(font);
    }

    /**
     * To get the Font of the element.
     * @return font the Font
     */
    public Font getFont() {
        return getLabel().getFont();
    }

    /**
     * To get the Foreground color of the element.
     * @return color the Color
     */
    public Color getForeground() {
        return getLabel().getForeground();
    }

    /**
     * To set the Foreground color of the element.
     * @param color the Color
     */
    public void setForeground(Color color) {
        getLabel().setForeground(color);
    }

    /**
     * To set the Background color of the element.
     * @param color the Color
     */
    public void setBackground(Color color) {
        getLabel().setBackground(color);
    }

    /**
     * To get the Background color of the element.
     * @return color the Color
     */
    public Color getBackground() {
        return getLabel().getBackground();
    }

    /**
     * Draws a swing Scilab PushButton
     * @see org.scilab.modules.gui.uielement.UIElement#draw()
     */
    public void draw() {
        this.setVisible(true);
        this.doLayout();
    }

    /**
     * Sets the visibility status of an UIElement
     * @param newVisibleState the visibility status we want to set for the
     * UIElement (true if the UIElement is visible, false if not)
     */
    public void setVisible(boolean newVisibleState) {
        super.setVisible(newVisibleState);
        //getLabel().setVisible(newVisibleState);
    }

    /**
     * Gets the dimensions (width and height) of a swing Scilab element
     * @return the dimensions of the element
     * @see org.scilab.modules.gui.uielement.UIElement#getDims()
     */
    public Size getDims() {
        return new Size(getWidth(), getHeight());
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab
     * element
     * @return the position of the element
     * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
     */
    public Position getPosition() {
        return PositionConverter.javaToScilab(getLocation(), getSize(), getParent());
    }

    /**
     * Sets the dimensions (width and height) of a swing Scilab element
     * @param newSize the dimensions to set to the element
     * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public void setDims(Size newSize) {
        setSize(newSize.getWidth(), newSize.getHeight());
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab
     * element
     * @param newPosition the position to set to the element
     * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public void setPosition(Position newPosition) {
        Position javaPosition = PositionConverter.scilabToJava(newPosition, getDims(), getParent());
        setLocation(javaPosition.getX(), javaPosition.getY());
    }

    /**
     * Add a callback to the UiTable
     * @param callback the callback to set.
     */
    public void setCallback(CommonCallBack callback) {
        // Nothing to do...
    }

    /**
     * Setter for MenuBar
     * @param menuBarToAdd the MenuBar associated to the UiTable.
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        /* Unimplemented for UiTables */
        throw new UnsupportedOperationException();
    }

    /**
     * Setter for ToolBar
     * @param toolBarToAdd the ToolBar associated to the UiTable.
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        /* Unimplemented for UiTables */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for MenuBar
     * @return MenuBar: the MenuBar associated to the UiTable.
     */
    public MenuBar getMenuBar() {
        /* Unimplemented for UiTables */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for ToolBar
     * @return ToolBar: the ToolBar associated to the UiTable.
     */
    public ToolBar getToolBar() {
        /* Unimplemented for UiTables */
        throw new UnsupportedOperationException();
    }

    /**
     * Set the horizontal alignment for the UiTable text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        getLabel().setHorizontalAlignment(ScilabAlignment.toSwingAlignment(alignment));
    }

    /**
     * Set the vertical alignment for the UiTable text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        getLabel().setVerticalAlignment(ScilabAlignment.toSwingAlignment(alignment));
    }

    /**
     * Set the Relief of the UiTable
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public void setRelief(String reliefType) {
        if (defaultBorder == null) {
            defaultBorder = getBorder();
        }
        setBorder(ScilabRelief.getBorderFromRelief(reliefType, defaultBorder));
    }

    /**
     * Destroy the UiTable
     */
    public void destroy() {
        ScilabSwingUtilities.removeFromParent(this);
    }

    /**
     * Setter for InfoBar
     * @param infoBarToAdd the InfoBar associated to the UiTable.
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        /* Unimplemented for UiTables */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for InfoBar
     * @return the InfoBar associated to the UiTable.
     */
    public TextBox getInfoBar() {
        /* Unimplemented for UiTables */
        throw new UnsupportedOperationException();
    }

    /**
     * Create/Return the uiTable Java object
     * @return the uiTable
     */
    private JTable getUiTable() {
        if (uiTable == null) {
            uiTable = new JTable(data, colNames);
            uiTable.setFillsViewportHeight(true);
            if (uiTable.getGridColor().equals(Color.WHITE)) {
                uiTable.setGridColor(new Color(128, 128, 128));
            }
            uiTable.setShowHorizontalLines(true);
            uiTable.setShowVerticalLines(true);
        }
        return uiTable;
    }

    /**
     * Create/Return the rowHeader Java Object
     * @return the rowHeader
     */
    private JList getCustomRowHeader() {
        if (rowHeader == null) {
            rowHeader = new JList(rowNames);
            rowHeader.setFixedCellWidth(50);
            rowHeader.setFixedCellHeight(uiTable.getRowHeight());
            rowHeader.setCellRenderer(new RowHeaderRenderer(uiTable));
        }
        return rowHeader;
    }

    /**
     * Create/Return the uiTable Java object
     * @return the uiTable
     */
    private JLabel getLabel() {
        if (label == null) {
            label = new JLabel();
        }
        return label;
    }

    /**
     * gets directory of image in image render
     * @return the directory string
     * @see org.scilab.modules.gui.text.SimpleText#getText()
     */
    public String getText() {
        return getLabel().getText();
    }

    /**
     * Sets the directory for image to render
     * @param newText the new directory to image
     */
    public void setText(String newText) {
        getLabel().setText(newText);
    }

    public void setEmptyText() {
        setColumnNames(new String[] {""});
        setRowNames(new String[] {""});
        setData(new String[] {""});
    }

    /**
     * Sets the column names for uitable
     * @param names the String[] that contains column names
     */
    public void setColumnNames(String[] names) {
        //updates table with new column names
        nCol = names.length;
        colNames = names;
        uiTable = new JTable(data, names);
        getViewport().add(uiTable);
        uiTable.doLayout();
    }

    /**
     * Sets the row names for uitable
     * @param names the String[] that contains row names
     */
    public void setRowNames(String[] names) {
        //updates table with new row names
        nRow = names.length;
        rowNames = names;
        rowHeader = new JList(names);
        rowHeader.setFixedCellWidth(50);
        rowHeader.setFixedCellHeight(uiTable.getRowHeight());
        rowHeader.setCellRenderer(new RowHeaderRenderer(uiTable));
        setRowHeaderView(rowHeader);
        uiTable.doLayout();
    }

    /**
     * Sets the Data for uitable
     * @param text the String that contains row data delimited by a '|' and
     * column data delimited by " ". Example: 1.26 3.47 | a b | d e | a b
     */
    public void setData(String[] text) {
        //initializes data structure with number of rows and columns
        if (nRow == 0) {
            nRow = text.length / nCol;
        }
        data = new Object[nRow][nCol];
        int i = 0;
        int j = 0;
        int nbElements = 0;

        while (nbElements < text.length && i < nRow && j < nCol) {
            data[i][j] = text[nbElements];
            i++;
            nbElements++;
            if (i == nRow) {
                i = 0;
                j++;
            }
        }

        //if no row names were provided, it will set numeric ones according to number of rows. (1, 2, 3, 4, ...)
        if (nRow == 0) {
            nRow = j;
            rowNames = new Object[nRow];
            for (int k = 0; k < nRow; k++) {
                rowNames[k] = k;
            }
            rowHeader = new JList(rowNames);
            rowHeader.setFixedCellWidth(50);
            rowHeader.setFixedCellHeight(uiTable.getRowHeight());
            rowHeader.setCellRenderer(new RowHeaderRenderer(uiTable));
            setRowHeaderView(rowHeader);
        }

        //adds and updates table with new data
        uiTable = new JTable(data, colNames);
        getViewport().add(uiTable);
        uiTable.doLayout();
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
            case __GO_UI_STRING__: {
                // Update column names
                String[] stringValue = (String[]) value;
                if (stringValue.length == 0) {
                    setColumnNames(new String[] {""});
                    setRowNames(new String[] {""});
                    setData(new String[] {""});
                    return;
                }
                int colNb = ((Integer) controller.getProperty(uid, __GO_UI_STRING_COLNB__));
                String[] colNames = new String[colNb - 1];
                for (int k = 1; k < colNb; k++) {
                    colNames[k - 1] = stringValue[k * (stringValue.length / colNb)];
                }

                setColumnNames(colNames);

                // Update row names
                String[] rowNames = new String[stringValue.length / colNb - 1];
                for (int k = 1; k < stringValue.length / colNb; k++) {
                    rowNames[k - 1] = stringValue[k];
                }

                setRowNames(rowNames);

                // Update data
                String[] tableData = new String[rowNames.length * colNames.length];
                int kData = 0;
                for (int kCol = 1; kCol <= colNames.length; kCol++) {
                    for (int kRow = 1; kRow <= rowNames.length; kRow++) {
                        tableData[kData++] = stringValue[kCol * (stringValue.length / colNb) + kRow];
                    }
                }

                if (tableData.length != 0) {
                    setData(tableData);
                }
                break;
            }
            default: {
                SwingViewWidget.update(this, property, value);
            }
        }
    }

    public void resetBackground() {
        Color color = (Color) UIManager.getLookAndFeelDefaults().get("ScrollPane.background");
        if (color != null) {
            setBackground(color);
        }
    }

    public void resetForeground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("ScrollPane.foreground");
        if (color != null) {
            setForeground(color);
        }
    }
}
