package org.scilab.modules.graphic_objects;

import java.util.ArrayList;

/**
 * Legend class
 * @author juliachs
 */
public class Legend extends ClippableTextObject {
	/** Legend location */
	private enum LegendLocation { IN_UPPER_RIGHT, IN_UPPER_LEFT, IN_LOWER_RIGHT, IN_LOWER_LEFT,
		OUT_UPPER_RIGHT, OUT_UPPER_LEFT, OUT_LOWER_RIGHT, OUT_LOWER_LEFT,
		UPPER_CAPTION, LOWER_CAPTION, BY_COORDINATES };
	
	/** List of the polylines referred to */
	private ArrayList <Polyline> links;

	/** Legend location */
	private LegendLocation legendLocation;

	/** 2D position relative to the parent axes bounds */
	private double [] position;

	/** Constructor */
	public Legend() {
		super();
		this.links = null;
		this.legendLocation = null;
		position = null;
	}

	/**
	 * @return the legendLocation
	 */
	public LegendLocation getLegendLocation() {
		return legendLocation;
	}

	/**
	 * @param legendLocation the legendLocation to set
	 */
	public void setLegendLocation(LegendLocation legendLocation) {
		this.legendLocation = legendLocation;
	}

	/**
	 * @return the links
	 */
	public ArrayList<Polyline> getLinks() {
		return links;
	}

	/**
	 * @param links the links to set
	 */
	public void setLinks(ArrayList<Polyline> links) {
		this.links = links;
	}

	/**
	 * @return the position
	 */
	public double[] getPosition() {
		return position;
	}

	/**
	 * @param position the position to set
	 */
	public void setPosition(double[] position) {
		this.position = position;
	}
	
	//	 Methods: to be done
}
