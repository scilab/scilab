/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Han DONG
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.bridge.uitable;

import java.awt.Color;
import java.awt.Font;
import java.awt.Dimension;

import javax.swing.JList;
import javax.swing.JTable;
import javax.swing.JLabel;
import javax.swing.JScrollPane;
import javax.swing.SwingUtilities;

import java.util.StringTokenizer;

import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.uitable.SimpleUiTable;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
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
public class SwingScilabUiTable extends JScrollPane implements SimpleUiTable {

	private static final long serialVersionUID = -5497171010652701217L;

	private JTable uiTable;
	private JList rowHeader;

	private Object[] colNames = {};
	private Object[] rowNames = {};
	private Object[][] data = {};	

	private int nCol = 0;
	private int nRow = 0;

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
	 * @param newVisibleState the visibility status we want to set for the UIElement
	 *                      (true if the UIElement is visible, false if not)
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
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab element
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
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab element
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
	public void setCallback(CallBack callback) {
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
		setBorder(ScilabRelief.getBorderFromRelief(reliefType));
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

	/**
	 * Sets the column names for uitable
	 * @param text the String that contains column names delimited by a '|'. Example: 1|2|3|4
	 */
	public void setColnames(String text) {
		int i = 0;
		
		StringTokenizer stk = new StringTokenizer(text, "|");

		//sets the number of columns from string tokenizer		
		nCol = stk.countTokens();

		//initializes colNames
		colNames = new Object[nCol];
		while(stk.hasMoreTokens()) {
			colNames[i] = stk.nextToken().trim();
			i ++;
		}

		//updates table with new column names
		uiTable = new JTable(data, colNames);
		getViewport().add(uiTable);
	}

	/**
	 * Sets the row names for uitable
	 * @param text the String that contains row names delimited by a '|'. Example: 1|2|3|4
	 */
	public void setRownames(String text) {
		int i = 0;
		
		//sets the number of rows from string tokenizer
		StringTokenizer stk = new StringTokenizer(text, "|");
		nRow = stk.countTokens();
		rowNames = new Object[nRow];

		//initializes rowNames
		while(stk.hasMoreTokens()) {
			rowNames[i] = stk.nextToken().trim();
			i ++;
		}
		
		//updates table with new row names
		rowHeader = new JList(rowNames);
		rowHeader.setFixedCellWidth(50);
		rowHeader.setFixedCellHeight(uiTable.getRowHeight());
		rowHeader.setCellRenderer(new RowHeaderRenderer(uiTable));
		setRowHeaderView(rowHeader);	
	}

	/**
	 * Sets the Data for uitable
	 * @param uiTable the UiTable
	 * @param text the String that contains row data delimited by a '|'
         *        and column data delimited by " ". Example: 1.26 3.47 | a b | d e | a b
	 */
	public void setData(String text) {
		StringTokenizer stk = new StringTokenizer(text, "|");
		StringTokenizer stk2 = null;
		String s1, s2 = null;

		//if no row names were provided, it will set numeric ones according to number of rows. (1, 2, 3, 4, ...)
		if(nRow == 0)
		{
			nRow = stk.countTokens();
			rowNames = new Object[nRow];
			for(int k = 0; k < nRow; k ++)
			{
				rowNames[k] = k;
			}
			rowHeader = new JList(rowNames);
			rowHeader.setFixedCellWidth(50);
			rowHeader.setFixedCellHeight(uiTable.getRowHeight());
			rowHeader.setCellRenderer(new RowHeaderRenderer(uiTable));
			setRowHeaderView(rowHeader);	
		}

		//initializes data structure with number of rows and columns
		data = new Object[nRow][nCol];
		int i = 0;
		int j = 0;

		//gets each row delimited by "|"
		while(stk.hasMoreTokens())
		{
			s1 = stk.nextToken().trim();
			stk2 = new StringTokenizer(s1, " ");
			
			//gets each value in column delimited by " "
			while(stk2.hasMoreTokens())
			{
				s2 = stk2.nextToken().trim();
				
				//if i, j are larger than the specified data size, it will not add them to table
				if(i >= nRow || j >= nCol) 
				{
				}
				else
				{
					data[i][j] = s2;
					j ++;
				}
			}
			i ++;
			j = 0;
		}

		//adds and updates table with new data
		uiTable = new JTable(data, colNames);
		getViewport().add(uiTable);
	}
}
