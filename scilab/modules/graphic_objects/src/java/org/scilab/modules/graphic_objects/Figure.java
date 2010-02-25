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
	
	//	 Methods: to be done
}
