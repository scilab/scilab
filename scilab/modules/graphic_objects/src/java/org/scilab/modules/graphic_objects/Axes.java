package org.scilab.modules.graphic_objects;

/**
 * Axes class
 * @author juliachs
 */
public class Axes extends GraphicObject {
	/** Specifies the grid position relative to the graphics entities */
	private static enum GridPosition { FOREGROUND, BACKGROUND };

	/** 3-element array (properties of the X, Y and Z axes) */
	private AxisProperty [] axes;

	/** Grid position */
	private GridPosition gridPosition;
	
	/** Title label */
	private Label title;

	/** Specifies whether the Axes subwindow is cleared when a new plot command is performed */ 
	private boolean autoClear;

	/** Specifies whether the Axes background is filled or not  */
	private boolean filled;

	/** Camera */
	private Camera camera;

	/** Box: properties related to the data bounds */
	private Box box;
	
	/** Margins enclosing the drawing area (left, right, top, bottom) */
	private double [] margins;
	
	/**
	 * Axes bounds relative to their parent figure's
	 * drawing area (x, y, width, height)
	 */
	private double [] axesBounds;

	/** Constructor */
	public Axes() {
		super();
		axes = null;
		gridPosition = GridPosition.FOREGROUND;
		title = null;
		autoClear = false;
		filled = false;
		camera = null;
		box = null;
		margins = null;
		axesBounds = null;
	}
	
	/**
	 * @return the autoClear
	 */
	public boolean isAutoClear() {
		return autoClear;
	}

	/**
	 * @param autoClear the autoClear to set
	 */
	public void setAutoClear(boolean autoClear) {
		this.autoClear = autoClear;
	}

	/**
	 * @return the axes
	 */
	public AxisProperty[] getAxes() {
		return axes;
	}

	/**
	 * @param axes the axes to set
	 */
	public void setAxes(AxisProperty[] axes) {
		this.axes = axes;
	}

	/**
	 * @return the axesBounds
	 */
	public double[] getAxesBounds() {
		return axesBounds;
	}

	/**
	 * @param axesBounds the axesBounds to set
	 */
	public void setAxesBounds(double[] axesBounds) {
		this.axesBounds = axesBounds;
	}

	/**
	 * @return the box
	 */
	public Box getBox() {
		return box;
	}

	/**
	 * @param box the box to set
	 */
	public void setBox(Box box) {
		this.box = box;
	}

	/**
	 * @return the camera
	 */
	public Camera getCamera() {
		return camera;
	}

	/**
	 * @param camera the camera to set
	 */
	public void setCamera(Camera camera) {
		this.camera = camera;
	}

	/**
	 * @return the filled
	 */
	public boolean isFilled() {
		return filled;
	}

	/**
	 * @param filled the filled to set
	 */
	public void setFilled(boolean filled) {
		this.filled = filled;
	}

	/**
	 * @return the gridPosition
	 */
	public GridPosition getGridPosition() {
		return gridPosition;
	}

	/**
	 * @param gridPosition the gridPosition to set
	 */
	public void setGridPosition(GridPosition gridPosition) {
		this.gridPosition = gridPosition;
	}

	/**
	 * @return the margins
	 */
	public double[] getMargins() {
		return margins;
	}

	/**
	 * @param margins the margins to set
	 */
	public void setMargins(double[] margins) {
		this.margins = margins;
	}

	/**
	 * @return the title
	 */
	public Label getTitle() {
		return title;
	}

	/**
	 * @param title the title to set
	 */
	public void setTitle(Label title) {
		this.title = title;
	}

	//	 Methods: to be done
}
