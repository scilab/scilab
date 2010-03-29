package org.scilab.modules.graphic_objects;

/**
 * Figure class
 * @author juliachs
 */
public class Figure extends GraphicObject {
	/** Specifies whether rotation applies to a single subwindow or to all the figure's subwindows */
	private enum RotationType { UNARY, MULTIPLE };
	
	/** Pixel drawing logical operations */
	private enum PixelDrawingMode { CLEAR, AND, ANDREVERSE, COPY, ANDINVERTED, NOOP, XOR, OR,
		EQUIV, INVERT, ORREVERSE, COPYINVERTED, ORINVERTED, NAND, SET };
	
	private class FigureDimensions {
		/** Position (x,y) of the upper-left corner in the top-level window */
		private int [] position;

		/** Dimensions (w, h) of the figure window */
		private int [] size;
	}

	private class Canvas {
		/** Specifies whether automatic resizing is used */
		private boolean autoResize;

		/** Viewport position (x,y) */
		private int [] viewPort;

		/** Rendering canvas (w,h) dimensions */
		private int [] axesSize;
	}

	private class FigureName {
		/** Name */
		private String name;

		/** Identifier */
		private int id;
	}

	private class RenderingMode {
		/** Specifies rendering into a pixmap */
		private boolean pixmap;

		/** Antialising level (0 == off) */
		private int antialiasing;

		/** Specifies whether immediate drawing is used or not */
		private boolean immediateDrawing;
	}

	private class EventHandler{
		/** Event handler string */
		private String eventHandler;
	}

	/** Figure dimensions */
	private FigureDimensions dimensions;

	/** Canvas property */
	private Canvas canvas;

	/** Figure name */
	private FigureName name;

	/** Info message */
	private String infoMessage;

	/**
	 * Default colormap: (3 x N) matrix, where N is the
	 * number of colors
	 */
	private int [][] colorMap;

	/** Rendering mode */
	private RenderingMode renderingMode;

	/** Background color */
	private int background;

	/** Event handler */
	private EventHandler eventHandler;

	/** Tag */
	private int tag;

	/** Rotation type */
	private RotationType rotation;

	/** Constructor */
	public Figure() {
		super();
		dimensions = null;
		canvas = null;
		name = null;
		infoMessage = null;
		colorMap = null;
		renderingMode = null;
		background = 0;
		eventHandler = null;
		tag = 0;
		rotation = null;
	}

	/**
	 * @return the background
	 */
	public int getBackground() {
		return background;
	}

	/**
	 * @param background the background to set
	 */
	public void setBackground(int background) {
		this.background = background;
	}

	/**
	 * @return the canvas
	 */
	public Canvas getCanvas() {
		return canvas;
	}

	/**
	 * @param canvas the canvas to set
	 */
	public void setCanvas(Canvas canvas) {
		this.canvas = canvas;
	}

	/**
	 * @return the colorMap
	 */
	public int[][] getColorMap() {
		return colorMap;
	}

	/**
	 * @param colorMap the colorMap to set
	 */
	public void setColorMap(int[][] colorMap) {
		this.colorMap = colorMap;
	}

	/**
	 * @return the dimensions
	 */
	public FigureDimensions getDimensions() {
		return dimensions;
	}

	/**
	 * @param dimensions the dimensions to set
	 */
	public void setDimensions(FigureDimensions dimensions) {
		this.dimensions = dimensions;
	}

	/**
	 * @return the eventHandler
	 */
	public EventHandler getEventHandler() {
		return eventHandler;
	}

	/**
	 * @param eventHandler the eventHandler to set
	 */
	public void setEventHandler(EventHandler eventHandler) {
		this.eventHandler = eventHandler;
	}

	/**
	 * @return the infoMessage
	 */
	public String getInfoMessage() {
		return infoMessage;
	}

	/**
	 * @param infoMessage the infoMessage to set
	 */
	public void setInfoMessage(String infoMessage) {
		this.infoMessage = infoMessage;
	}

	/**
	 * @return the name
	 */
	public FigureName getName() {
		return name;
	}

	/**
	 * @param name the name to set
	 */
	public void setName(FigureName name) {
		this.name = name;
	}

	/**
	 * @return the renderingMode
	 */
	public RenderingMode getRenderingMode() {
		return renderingMode;
	}

	/**
	 * @param renderingMode the renderingMode to set
	 */
	public void setRenderingMode(RenderingMode renderingMode) {
		this.renderingMode = renderingMode;
	}

	/**
	 * @return the rotation
	 */
	public RotationType getRotation() {
		return rotation;
	}

	/**
	 * @param rotation the rotation to set
	 */
	public void setRotation(RotationType rotation) {
		this.rotation = rotation;
	}

	/**
	 * @return the tag
	 */
	public int getTag() {
		return tag;
	}

	/**
	 * @param tag the tag to set
	 */
	public void setTag(int tag) {
		this.tag = tag;
	} 
	
	//	 Methods: to be done
}
