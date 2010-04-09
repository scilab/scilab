/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.graphicObject;


/**
 * ClippableProperty class
 * @author Manuel JULIACHS
 */
public class ClippableProperty {
	/** ClippableProperty properties */
	public enum ClippablePropertyType { CLIPSTATE, CLIPBOX };

	/** Indicates how clipping is performed */
	public static enum ClipStateType { OFF, CLIPGRF, ON };

	/** Clipping state */
	private ClipStateType clipState;

	/** Clip box (4- or 6-element array) */
	private double[] clipBox;

	/** Constructor */
	public ClippableProperty() {
		clipState = ClipStateType.OFF;
		clipBox = new double[4];
	}

	/**
	 * @return the clipBox
	 */
	public Double[] getClipBox() {
		Double[] retClipBox = new Double[clipBox.length];

		for (int i = 0; i < clipBox.length; i++) {
			retClipBox[i] = clipBox[i];
		}

		return retClipBox;
	}

	/**
	 * @param clipBox the clipBox to set
	 */
	public void setClipBox(Double[] clipBox) {
		for (int i = 0; i < clipBox.length; i++) {
			this.clipBox[i] = clipBox[i];
		}
	}

	/**
	 * @return the clipState
	 */
	public ClipStateType getClipState() {
		return clipState;
	}

	/**
	 * @param clipState the clipState to set
	 */
	public void setClipState(ClipStateType clipState) {
		this.clipState = clipState;
	}

}
