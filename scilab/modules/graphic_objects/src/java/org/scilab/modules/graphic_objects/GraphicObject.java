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

	/** Constructor */
	public GraphicObject() {
		this.parent = null;
		this.children = null;
		this.visible = false;
		this.userData = null;
	}

	/**
	 * @return the children
	 */
	public ArrayList<GraphicObject> getChildren() {
		return children;
	}

	/**
	 * @param children the children to set
	 */
	public void setChildren(ArrayList<GraphicObject> children) {
		this.children = children;
	}

	/**
	 * @return the parent
	 */
	public GraphicObject getParent() {
		return parent;
	}

	/**
	 * @param parent the parent to set
	 */
	public void setParent(GraphicObject parent) {
		this.parent = parent;
	}

	/**
	 * @return the userData
	 */
	public byte[] getUserData() {
		return userData;
	}

	/**
	 * @param userData the userData to set
	 */
	public void setUserData(byte[] userData) {
		this.userData = userData;
	}

	/**
	 * @return the visible
	 */
	public boolean isVisible() {
		return visible;
	}

	/**
	 * @param visible the visible to set
	 */
	public void setVisible(boolean visible) {
		this.visible = visible;
	}

	// Methods: to be done
}
