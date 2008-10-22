/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class of a grid containing cells. Cells are area for String drawing
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.textDrawing;

import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class of a grid containing cells. Cells are area for String drawing.
 * The first cell (0,0) is the upper-left one.
 * By default its lower-left corner id on (0,0,0).
 * @author Jean-Baptiste Silvy
 */
public class TextGrid {

	/** Extend the cells a little bit to give a better fitting */
	public static final float EXTEND_FACTOR_X = 0.1f;
	/** Extend the cells a little bit to give a better fitting */
	public static final float EXTEND_FACTOR_Y = 0.2f;
	
	/** We store every grid intersections. So the matrix size is (nbRow+1) x (nbCol+1) */
	private Vector3D[][] cellsEdges;
	private int nbRow;
	private int nbCol;
	
	/**
	 * Contruct a text grid with nbRow x nbCol cells.
	 * @param nbRow number of row of the grid.
	 * @param nbCol number of columns of the grid.
	 * @param rowHeights array of size nbRow containing the height of each row.
	 * @param colWidths array of size nbCol containing the width of each row.
	 * 
	 */
	public TextGrid(int nbRow, int nbCol, double[] rowHeights, double[] colWidths) {
		this.nbCol = nbCol;
		this.nbRow = nbRow;
		cellsEdges = new Vector3D[nbRow + 1][nbCol + 1];
		
		for (int i = 0; i <= nbRow; i++) {
			for (int j = 0; j <= nbCol; j++) {
				cellsEdges[i][j] = new Vector3D(0.0, 0.0, 0.0);
			}
		}
		// extend a little width and height
		for (int i = 0; i < nbRow; i++) {
			rowHeights[i] *= (1.0 + EXTEND_FACTOR_Y);
		}
		
		for (int j = 0; j < nbCol; j++) {
			colWidths[j] *= (1.0 + EXTEND_FACTOR_X);
		}
		
		setColumnWidth(colWidths);
		setRowHeight(rowHeights);
	}
	
	/**
	 * @return number of cells along Y axis.
	 */
	public int getNbRow() {
		return nbRow;
	}
	
	/**
	 * @return Number of cells along X axis.
	 */
	public int getNbCol() {
		return nbCol;
	}
	
	/**
	 * Specify a new heights of rows.
	 * @param heights array of size nbRow containing the height of each row.
	 */
	protected void setRowHeight(double[] heights) {
		// compute height of the grid
		double gridHeight = 0.0;
		for (int i = 0; i < nbRow; i++) {
			gridHeight += heights[i];
		}
		
		
		double curHeight = gridHeight;
		for (int i = 0; i < nbRow; i++) {
			for (int j = 0; j <= nbCol; j++) {
				cellsEdges[i][j].setY(curHeight);
			}
			curHeight -= heights[i];
		}
		for (int j = 0; j <= nbCol; j++) {
			cellsEdges[nbRow][j].setY(0.0);
		}
	}
	
	/**
	 * Specify a new widths of rows.
	 * @param widths array of size nbCol containing the width of each column.
	 */
	protected void setColumnWidth(double[] widths) {
		
		double curWidth = 0.0;
		for (int j = 0; j < nbCol; j++) {
			for (int i = 0; i <= nbRow; i++) {
				cellsEdges[i][j].setX(curWidth);
			}
			curWidth += widths[j];
		}
		for (int i = 0; i <= nbRow; i++) {
			cellsEdges[i][nbCol].setX(curWidth);
		}
	}
	
	/**
	 * Get the 4 corners of a cell
	 * @param numRow row index of the cell
	 * @param numCol column index of the cell
	 * @return array of size 4 conating the 4 corners of the cell.
	 */
	public Vector3D[] getCellCoordinates(int numRow, int numCol) {
		Vector3D[] res = {cellsEdges[numRow][numCol],
						  cellsEdges[numRow + 1][numCol],
						  cellsEdges[numRow + 1][numCol + 1],
						  cellsEdges[numRow][numCol + 1]};
		return res;
	}
	
	/**
	 * Translate the grid.
	 * @param trans translation vector.
	 */
	public void translate(Vector3D trans) {
		for (int i = 0; i <= nbRow; i++) {
			for (int j = 0; j <= nbCol; j++) {
				cellsEdges[i][j] = cellsEdges[i][j].add(trans);
			}
		}
	}
	
	/**
	 * Use a scale factor to modify the matrix along the two dimensions
	 * @param factor scale factor
	 */
	public void scale(double factor) {
		for (int i = 0; i <= nbRow; i++) {
			for (int j = 0; j <= nbCol; j++) {
				cellsEdges[i][j].scalarMultSelf(factor);
			}
		}
	}
	
	/**
	 * Rotate all the grid around a axis.
	 * @param axisCenter point on the axis
	 * @param axisDir direction of the axis
	 * @param angle rotation angle in radian
	 */
	public void rotate(Vector3D axisCenter, Vector3D axisDir, double angle) {
		for (int i = 0; i <= nbRow; i++) {
			for (int j = 0; j <= nbCol; j++) {
				cellsEdges[i][j] = cellsEdges[i][j].rotate(axisCenter, axisDir, angle);
			}
		}
	}
	
	/**
	 * Get the 4 extrem corners of the TextGrid.
	 * @return array of size 4 with the positions.
	 */
	public Vector3D[] getExtremBounds() {
		Vector3D[] res = new Vector3D[2 + 2];
		res[0] = new Vector3D(cellsEdges[0][0]);
		res[1] = new Vector3D(cellsEdges[nbRow][0]);
		res[2] = new Vector3D(cellsEdges[nbRow][nbCol]);
		res[2 + 1] = new Vector3D(cellsEdges[0][nbCol]);
		return res;
	}
	
	/**
	 * @return String representation of the object.
	 */
	@Override
	public String toString() {
		StringBuffer res = new StringBuffer("");
		for (int i = 0; i <= nbRow; i++) {
			for (int j = 0; j <= nbCol; j++) {
				res.append("cellsEdge[" + i + "," + j + "] = " + cellsEdges[i][j] + "\n");
			}
		}
		return res.toString();
	}
	
}
