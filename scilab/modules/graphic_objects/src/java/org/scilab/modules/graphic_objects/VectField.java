package org.scilab.modules.graphic_objects;

import java.util.ArrayList;

/**
 * VectField class
 * @author juliachs
 */
public abstract class VectField extends GraphicClippableObject {
	/** List of arrows composing the field */
	private ArrayList <Arrow> arrows;

	/** Constructor */
	public VectField() {
		super();
		arrows = null;
	}

	/**
	 * @return the arrows
	 */
	public ArrayList<Arrow> getArrows() {
		return arrows;
	}

	/**
	 * @param arrows the arrows to set
	 */
	public void setArrows(ArrayList<Arrow> arrows) {
		this.arrows = arrows;
	}

	// Methods: to be done
}
