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

package org.scilab.modules.graphic_objects.figure;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;

/**
 * Figure class
 * @author Manuel JULIACHS
 */
public class Figure extends GraphicObject {
	/** Figure properties names */
	private enum FigureProperty { DIMENSIONS, CANVAS, NAME, INFOMESSAGE, COLORMAP, RENDERINGMODE,
		BACKGROUND, EVENTHANDLER, TAG, ROTATIONTYPE
	};
	
	/** Specifies whether rotation applies to a single subwindow or to all the figure's subwindows */
	private enum RotationType { UNARY, MULTIPLE };
	
	/** Pixel drawing logical operations */
	private enum PixelDrawingMode { CLEAR, AND, ANDREVERSE, COPY, ANDINVERTED, NOOP, XOR, OR,
		EQUIV, INVERT, ORREVERSE, COPYINVERTED, ORINVERTED, NAND, SET };

	/** FigureDimensions properties names */
	public enum FigureDimensionsProperty { POSITION, SIZE };

	/**
	 * FigureDimensions class
	 */
	private class FigureDimensions {
		/** Position (x,y) of the upper-left corner in the top-level window */
		private int[] position;

		/** Dimensions (w, h) of the figure window */
		private int[] size;

		/**
		 * Default constructor
		 */
		public FigureDimensions() {
			position = new int[2];
			size = new int[2];
		}
	}

	/** CanvasProperty properties names */
	public enum CanvasProperty { AUTORESIZE, VIEWPORT, AXESSIZE };

	/**
	 * Canvas class
	 */
	private class Canvas {
		/** Specifies whether automatic resizing is used */
		private boolean autoResize;

		/** Viewport position (x,y) */
		private int[] viewport;

		/** Rendering canvas (w,h) dimensions */
		private int[] axesSize;
		
		/**
		 * Default constructor
		 */
		public Canvas() {
			autoResize = false;
			viewport = new int[2];
			axesSize = new int[2];
		}
	}

	/** FigureName properties names */
	public enum FigureNameProperty { NAME, ID };

	/**
	 * FigureName class
	 */
	private class FigureName {
		/** Name */
		private String name;

		/** Identifier */
		private int id;
	}

	/** RenderingMode properties names */
	public enum RenderingModeProperty { PIXMAP, PIXELDRAWINGMODE, ANTIALIASING, IMMEDIATEDRAWING };

	/**
	 * RenderingMode class 
	 */
	private class RenderingMode {
		/** Specifies rendering into a pixmap */
		private boolean pixmap;

		/** Specifies the pixel drawing mode used */
		private PixelDrawingMode pixelDrawingMode;

		/** Antialising level (0 == off) */
		private int antialiasing;

		/** Specifies whether immediate drawing is used or not */
		private boolean immediateDrawing;

		/**
		 * Default constructor
		 */
		public RenderingMode() {
			pixmap = false;
			pixelDrawingMode = PixelDrawingMode.COPY;
			antialiasing = 0;
			immediateDrawing = true;
		}

	}

	/** EventHandler properties names */
	public enum EventHandlerProperty { EVENTHANDLER };

	/**
	 * EventHandler class
	 */
	private class EventHandler{
		/** Event handler string */
		private String eventHandler;
	}

	/** Figure dimensions */
	private FigureDimensions dimensions;

	/** Canvas property */
	private Canvas canvas;

	/** Figure name */
	private FigureName figureName;

	/** Info message */
	private String infoMessage;

	/**
	 * Default colormap: (3 x N) matrix, where N is the
	 * number of colors
	 */
	private int[][] colorMap;

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
		dimensions = new FigureDimensions();
		canvas = new Canvas();
		figureName = new FigureName();
		infoMessage = null;
		colorMap = null;
		renderingMode = new RenderingMode();
		background = 0;
		eventHandler = null;
		tag = 0;
		rotation = RotationType.UNARY;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		if (propertyName.equals("Dimensions")) {
			return FigureProperty.DIMENSIONS;
		} else if (propertyName.equals("Position")) {
			return FigureDimensionsProperty.POSITION;
		} else if (propertyName.equals("Size")) {
			return FigureDimensionsProperty.SIZE;
		} else if (propertyName.equals("Canvas")) {
			return FigureProperty.CANVAS;
		} else if (propertyName.equals("AutoResize")) {
			return CanvasProperty.AUTORESIZE;
		} else if (propertyName.equals("Viewport")) {
			return CanvasProperty.VIEWPORT;
		} else if (propertyName.equals("AxesSize")) {
			return CanvasProperty.AXESSIZE;
		} else if (propertyName.equals("FigureName")) {
			return FigureProperty.NAME;
		} else if (propertyName.equals("Name")) {
			return FigureNameProperty.NAME;
		} else if (propertyName.equals("Id")) {
			return FigureNameProperty.ID;
		} else if (propertyName.equals("InfoMessage")) {
			return FigureProperty.INFOMESSAGE;
		} else if (propertyName.equals("ColorMap")) {
			return FigureProperty.COLORMAP;
		} else if (propertyName.equals("RenderingMode")) {
			return FigureProperty.RENDERINGMODE;
		} else if (propertyName.equals("Pixmap")) {
			return RenderingModeProperty.PIXMAP;
		} else if (propertyName.equals("PixelDrawingMode")) {
			return RenderingModeProperty.PIXELDRAWINGMODE;
		} else if (propertyName.equals("Antialiasing")) {
			return RenderingModeProperty.ANTIALIASING;
		} else if (propertyName.equals("ImmediateDrawing")) {
			return RenderingModeProperty.IMMEDIATEDRAWING;
		} else if (propertyName.equals("Background")) {
			return FigureProperty.BACKGROUND;
		} else if (propertyName.equals("EventHandler")) {
			return FigureProperty.EVENTHANDLER;
		} else if (propertyName.equals("EventHandlerName")) {
			return EventHandlerProperty.EVENTHANDLER;
		} else if (propertyName.equals("Tag")) {
			return FigureProperty.TAG;
		} else if (propertyName.equals("RotationType")) {
			return FigureProperty.ROTATIONTYPE;
		} else {
			return super.getPropertyFromName(propertyName);
		}
	}

	/**
	 * Fast property get method
	 * @param property the property to get
	 * @return the property value
	 */
	public Object getPropertyFast(Object property) {
		if (property == FigureProperty.DIMENSIONS) {
			return getDimensions();
		} else if (property == FigureDimensionsProperty.POSITION) {
			return getPosition();
		} else if (property == FigureDimensionsProperty.SIZE) {
			return getSize();
		} else if (property == FigureProperty.CANVAS) {
			return getCanvas();
		} else if (property == CanvasProperty.AUTORESIZE) {
			return getAutoResize();
		} else if (property == CanvasProperty.VIEWPORT) {
			return getViewport();
		} else if (property == CanvasProperty.AXESSIZE) {
			return getAxesSize();
		} else if (property == FigureProperty.NAME) {
			return getFigureName();
		} else if (property == FigureNameProperty.NAME) {
			return getName();
		} else if (property == FigureNameProperty.ID) {
			return getId();
		} else if (property == FigureProperty.INFOMESSAGE) {
			return getInfoMessage();
		} else if (property == FigureProperty.COLORMAP) {
			return getColorMap();
		} else if (property == FigureProperty.RENDERINGMODE) {
			return getRenderingMode();
		} else if (property == RenderingModeProperty.PIXMAP) {
			return getPixmap();
		} else if (property == RenderingModeProperty.PIXELDRAWINGMODE) {
			return getPixelDrawingMode();
		} else if (property == RenderingModeProperty.ANTIALIASING) {
			return getAntialiasing();
		} else if (property == RenderingModeProperty.IMMEDIATEDRAWING) {
			return getImmediateDrawing();
		} else if (property == FigureProperty.BACKGROUND) {
			return getBackground();
		} else if (property == FigureProperty.EVENTHANDLER) {
			return getEventHandler();
		} else if (property == EventHandlerProperty.EVENTHANDLER) {
			return getEventHandlerString();
		} else if (property == FigureProperty.TAG) {
			return getTag();
		} else if (property == FigureProperty.ROTATIONTYPE) {
			return getRotation();
		} else {
			return super.getPropertyFast(property);
		}
	}

	/**
	 * Fast property set method
	 * @param property the property to set
	 * @param value the property value
	 * @return true if the property has been set, false otherwise
	 */
	public boolean setPropertyFast(Object property, Object value) {
		if (property == FigureProperty.DIMENSIONS) {
			setDimensions((FigureDimensions) value);
		} else if (property == FigureDimensionsProperty.POSITION) {
			setPosition((Integer[]) value);
		} else if (property == FigureDimensionsProperty.SIZE) {
			setSize((Integer[]) value);
		} else if (property == FigureProperty.CANVAS) {
			setCanvas((Canvas) value);
		} else if (property == CanvasProperty.AUTORESIZE) {
			setAutoResize((Boolean) value);
		} else if (property == CanvasProperty.VIEWPORT) {
			setViewport((Integer[]) value);
		} else if (property == CanvasProperty.AXESSIZE) {
			setAxesSize((Integer[]) value);
		} else if (property == FigureProperty.NAME) {
			setFigureName((FigureName) value);
		} else if (property == FigureNameProperty.NAME) {
			setName((String) value);
		} else if (property == FigureNameProperty.ID) {
			setId((Integer) value);
		} else if (property == FigureProperty.INFOMESSAGE) {
			setInfoMessage((String) value);
		} else if (property == FigureProperty.COLORMAP) {
			setColorMap((int[][]) value);
		} else if (property == FigureProperty.RENDERINGMODE) {
			setRenderingMode((RenderingMode) value);
		} else if (property == RenderingModeProperty.PIXMAP) {
			setPixmap((Boolean) value);
		} else if (property == RenderingModeProperty.PIXELDRAWINGMODE) {
			setPixelDrawingMode((PixelDrawingMode) value);
		} else if (property == RenderingModeProperty.ANTIALIASING) {
			setAntialiasing((Integer) value);
		} else if (property == RenderingModeProperty.IMMEDIATEDRAWING) {
			setImmediateDrawing((Boolean) value);
		} else if (property == FigureProperty.BACKGROUND) {
			setBackground((Integer) value);
		} else if (property == FigureProperty.EVENTHANDLER) {
			setEventHandler((EventHandler) value);
		} else if (property == EventHandlerProperty.EVENTHANDLER) {
			setEventHandlerString((String) value);
		} else if (property == FigureProperty.TAG) {
			setTag((Integer) value);
		} else if (property == FigureProperty.ROTATIONTYPE) {
			setRotation((RotationType) value);
		} else {
			return super.setPropertyFast(property, value);
		}

		return true;
	}

	/**
	 * @return the background
	 */
	public Integer getBackground() {
		return background;
	}

	/**
	 * @param background the background to set
	 */
	public void setBackground(Integer background) {
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
	 * @return the autoresize
	 */
	public Boolean getAutoResize() {
		return canvas.autoResize;
	}

	/**
	 * @param autoResize the autoresize to set
	 */
	public void setAutoResize(Boolean autoResize) {
		canvas.autoResize = autoResize;
	}

	/**
	 * @return the viewport
	 */
	public Integer[] getViewport() {
		Integer[] retViewport = new Integer[2];

		retViewport[0] = canvas.viewport[0];
		retViewport[1] = canvas.viewport[1];

		return retViewport;
	}

	/**
	 * @param viewport the viewport to set
	 */
	public void setViewport(Integer[] viewport) {
		canvas.viewport[0] = viewport[0];
		canvas.viewport[1] = viewport[1];
	}

	/**
	 * @return the axes size
	 */
	public Integer[] getAxesSize() {
		Integer[] retAxesSize = new Integer[2];

		retAxesSize[0] = canvas.axesSize[0];
		retAxesSize[1] = canvas.axesSize[1];

		return retAxesSize;
	}

	/**
	 * @param axesSize the axes size to set
	 */
	public void setAxesSize(Integer[] axesSize) {
		canvas.axesSize[0] = axesSize[0];
		canvas.axesSize[1] = axesSize[1];
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
	 * @return the figure position
	 */
	public Integer[] getPosition() {
		Integer[] retPosition = new Integer[2];

		retPosition[0] = dimensions.position[0];
		retPosition[1] = dimensions.position[1];

		return retPosition;
	}

	/**
	 * @param position the position to set
	 */
	public void setPosition(Integer[] position) {
		dimensions.position[0] = position[0];
		dimensions.position[1] = position[1];
	}

	/**
	 * @return the figure size
	 */
	public Integer[] getSize() {
		Integer[] retSize = new Integer[2];

		retSize[0] = dimensions.size[0];
		retSize[1] = dimensions.size[1];

		return retSize;
	}

	/**
	 * @param size the size to set
	 */
	public void setSize(Integer[] size) {
		dimensions.size[0] = size[0];
		dimensions.size[1] = size[1];
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
	 * @return the eventHandler string
	 */
	public String getEventHandlerString() {
		return eventHandler.eventHandler;
	}

	/**
	 * @param eventHandlerString the eventHandler string to set
	 */
	public void setEventHandlerString(String eventHandlerString) {
		eventHandler.eventHandler = eventHandlerString;
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
	public FigureName getFigureName() {
		return figureName;
	}

	/**
	 * @param figureName the figure name to set
	 */
	public void setFigureName(FigureName figureName) {
		this.figureName = figureName;
	}

	/**
	 * @return the figure name
	 */
	public String getName() {
		return figureName.name;
	}

	/**
	 * @param name the name to set
	 */
	public void setName(String name) {
		figureName.name = name;	
	}

	/**
	 * @return the id
	 */
	public Integer getId() {
		return figureName.id;
	}

	/**
	 * @param id the id to set
	 */
	public void setId(Integer id) {
		figureName.id = id;
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
	 * @return the pixmap
	 */
	public Boolean getPixmap() {
		return renderingMode.pixmap;
	}

	/**
	 * @param pixmap the pixmap to set
	 */
	public void setPixmap(Boolean pixmap) {
		renderingMode.pixmap = pixmap;
	}

	/**
	 * @return the pixel drawing mode
	 */
	public PixelDrawingMode getPixelDrawingMode() {
		return renderingMode.pixelDrawingMode;
	}

	/**
	 * @param pixelDrawingMode the pixel drawing mode to set
	 */
	public void setPixelDrawingMode(PixelDrawingMode pixelDrawingMode) {
		renderingMode.pixelDrawingMode = pixelDrawingMode;
	}

	/**
	 * @return the pixmap
	 */
	public Integer getAntialiasing() {
		return renderingMode.antialiasing;
	}

	/**
	 * @param antialiasing the antialiasing to set
	 */
	public void setAntialiasing(Integer antialiasing) {
		renderingMode.antialiasing = antialiasing;
	}

	/**
	 * @return the immediateDrawing
	 */
	public Boolean getImmediateDrawing() {
		return renderingMode.immediateDrawing;
	}

	/**
	 * @param immediateDrawing the immediateDrawing to set
	 */
	public void setImmediateDrawing(Boolean immediateDrawing) {
		renderingMode.immediateDrawing = immediateDrawing;
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
	public Integer getTag() {
		return tag;
	}

	/**
	 * @param tag the tag to set
	 */
	public void setTag(Integer tag) {
		this.tag = tag;
	}

}
