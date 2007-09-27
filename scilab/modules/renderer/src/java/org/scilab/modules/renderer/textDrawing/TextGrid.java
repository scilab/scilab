/*------------------------------------------------------------------------*/
/* file: TextGrid.java                                                    */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class of a grid containing cells. Cells are area for String     */
/*        drawing.                                                       */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.textDrawing;

import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class of a grid containing cells. Cells are area for String drawing.
 * The first cell (0,0) is the upper-left one.
 * By default its lower-left corner id on (0,0,0).
 * @author Jean-Baptiste Silvy
 */
public class TextGrid {

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
		
		for (int i = 0; i < nbRow; i++) {
			for (int j = 0; j < nbCol; j++) {
				cellsEdges[i][j] = new Vector3D(0.0, 0.0, 0.0);
			}
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
				cellsEdges[i][j].setX(curHeight);
			}
			curHeight -= heights[i];
		}
		for (int j = 0; j <= nbCol; j++) {
			cellsEdges[nbRow][j].setX(0.0);
		}
	}
	
	/**
	 * Specify a new widths of rows.
	 * @param widths array of size nbCol containing the width of each row.
	 */
	protected void setColumnWidth(double[] widths) {
		
		double curWidth = 0.0;
		for (int i = 0; i < nbRow; i++) {
			for (int j = 0; j <= nbCol; j++) {
				cellsEdges[i][j].setY(curWidth);
			}
			curWidth += widths[i];
		}
		for (int j = 0; j <= nbCol; j++) {
			cellsEdges[nbRow][j].setY(curWidth);
		}
	}
	
	/**
	 * Get the 4 corners of a cell
	 * @param numRow row index of the cell
	 * @param numCol column index of the cell
	 * @return array of size 4 conating the 4 corners of the cell.
	 */
	public Vector3D[] getCellCoordinates(int numRow, int numCol) {
		Vector3D[] res = {cellsEdges[numRow][numCol].getCopy(),
						  cellsEdges[numRow + 1][numCol].getCopy(),
						  cellsEdges[numRow + 1][numCol + 1].getCopy(),
						  cellsEdges[numRow][numCol + 1].getCopy()};
		return res;
	}
	
	/**
	 * Translate the grid.
	 * @param trans translation vector.
	 */
	public void translate(Vector3D trans) {
		for (int i = 0; i < nbRow; i++) {
			for (int j = 0; j <= nbCol; j++) {
				cellsEdges[i][j] = cellsEdges[i][j].add(trans);
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
		for (int i = 0; i < nbRow; i++) {
			for (int j = 0; j <= nbCol; j++) {
				cellsEdges[i][j] = cellsEdges[i][j].rotate(axisCenter, axisDir, angle);
			}
		}
	}
	
}
