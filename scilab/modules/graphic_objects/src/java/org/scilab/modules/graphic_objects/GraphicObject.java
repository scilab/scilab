package org.scilab.modules.graphic_objects;

import java.util.ArrayList;

/**
 * GraphicObject class
 * @author juliachs
 */
public abstract class GraphicObject {
	/** Parent object */
	private GraphicObject parent;

	/** Child objects list */
	private ArrayList <GraphicObject> children;

	/** Specifies whether the object is visible or not */
	private boolean visible;

	/** User data */
	private byte [] userData;

	/** Default constructor */
	public GraphicObject() {
		this.parent = null;
		this.children = null;
		this.visible = false;
		this.userData = null;
	}

	// Methods: to be done
}
