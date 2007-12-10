/*------------------------------------------------------------------------*/
/* file: XTicksDrawerGL.java                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing ticks for the one axis                            */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.subwinDrawing;

import org.scilab.modules.renderer.DrawableObjectGL;

/**
 * Class drawing ticks for the one axis
 * @author Jean-Baptiste Silvy
 */
public abstract class TicksDrawerGL extends DrawableObjectGL {

	private double[] ticksPositions;
	private String[] ticksLabels;
	private double[] subticksPositions;
	
	/**
	 * Default constructor.
	 */
	public TicksDrawerGL() {
		super();
	}
	
	/**
	 * @return Nummber of ticks to draw
	 */
	protected int getNbTicks() {
		return ticksPositions.length;
	}
	
	/**
	 * @param tickIndex index of the tick
	 * @return positions of ticks on its axis.
	 */
	protected double getTickPosition(int tickIndex) {
		return ticksPositions[tickIndex];
	}
	
	/**
	 * @param tickIndex index of the tick
	 * @return label to draw in front of the tick
	 */
	protected String getTickLabel(int tickIndex) {
		return ticksLabels[tickIndex];
	}
	
	/**
	 * @return number of subticks to draw
	 */
	protected int getNbSubticks() {
		return subticksPositions.length;
	}
	
	/**
	 * @param subtickIndex index of the subtick
	 * @return position of the subtick to draw on its axis.
	 */
	protected double getSubtickPosition(int subtickIndex) {
		return subticksPositions[subtickIndex];
	}
	
	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		initializeDrawing(parentFigureIndex);
		drawTicks();
		endDrawing();
	}
	
	/**
	 * Check if the ticks label does not concealed each other at the specified positions.
	 * @param ticksPositions initial position of the ticks
	 * @param ticksLabels labels to display in front of the ticks
	 * @return true if the ticks can be displayed as if or false if the number of ticks needs to be reduced.
	 */
	public boolean checkTicks(double[] ticksPositions, String[] ticksLabels) {
		// TODO actually check
		return true;
	}
	
	/**
	 * Draw a set of ticks
	 * @param ticksPositions positions of each ticks on their axis.
	 * @param ticksLabels labels to draw in front of ticks.
	 *                    must have the same size with ticks positions.
	 * @param subticksPositions positions of sub ticks on their axis.
	 */
	public void drawTicks(double[] ticksPositions, String[] ticksLabels, double[] subticksPositions) {
		this.ticksPositions = ticksPositions;
		this.ticksLabels = ticksLabels;
		this.subticksPositions = subticksPositions;
		
		drawTicks();
	}
	
	/**
	 * Draw ticks from the recorded data.
	 */
	public abstract void drawTicks();

}
