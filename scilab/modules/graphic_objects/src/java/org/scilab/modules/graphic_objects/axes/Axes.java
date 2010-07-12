/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.axes;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

import java.util.ArrayList;

import org.scilab.modules.graphic_objects.axes.AxisProperty.AxisLocation;
import org.scilab.modules.graphic_objects.axes.Box.BoxType;
import org.scilab.modules.graphic_objects.axes.Camera.ViewType;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.label.Label;
import org.scilab.modules.graphic_objects.textObject.FormattedText;

/**
 * Axes class
 * @author Manuel JULIACHS
 */
public class Axes extends GraphicObject {
	/** Axes properties names */
	private enum AxesProperty {
		XAXISVISIBLE, XAXISREVERSE, XAXISGRIDCOLOR, XAXISLABEL, XAXISLOCATION, XAXISLOGFLAG,
		XAXISTICKS, XAXISAUTOTICKS, XAXISTICKSLOCATIONS, XAXISTICKSLABELS, XAXISSUBTICKS,
		YAXISVISIBLE, YAXISREVERSE, YAXISGRIDCOLOR, YAXISLABEL, YAXISLOCATION, YAXISLOGFLAG,
		YAXISTICKS, YAXISAUTOTICKS, YAXISTICKSLOCATIONS, YAXISTICKSLABELS, YAXISSUBTICKS,
		ZAXISVISIBLE, ZAXISREVERSE, ZAXISGRIDCOLOR, ZAXISLABEL, ZAXISLOCATION, ZAXISLOGFLAG,
		ZAXISTICKS, ZAXISAUTOTICKS, ZAXISTICKSLOCATIONS, ZAXISTICKSLABELS, ZAXISSUBTICKS,
		GRIDPOSITION, TITLE, AUTOCLEAR, FILLED,
		MARGINS, AXESBOUNDS };

	/** Specifies the grid position relative to the graphics entities */
	public static enum GridPosition { FOREGROUND, BACKGROUND };

	/** 3-element array (properties of the X, Y and Z axes) */
	private AxisProperty[] axes;

	/** Grid position */
	private GridPosition gridPosition;

	/** Title label known by it's UID. */
	private String title;

	/** Specifies whether the Axes subwindow is cleared when a new plot command is performed */ 
	private boolean autoClear;

	/** Specifies whether the Axes background is filled or not  */
	private boolean filled;

	/** Camera */
	private Camera camera;

	/** Box: properties related to the data bounds */
	private Box box;

	/** Margins enclosing the drawing area (left, right, top, bottom) */
	private double[] margins;

	/**
	 * Axes bounds relative to their parent figure's
	 * drawing area (upper-left corner x and y, width, height)
	 */
	private double[] axesBounds;

	/** Constructor */
	public Axes() {
		super();
		axes = new AxisProperty[3];
		axes[0] = new AxisProperty();
		axes[1] = new AxisProperty();
		axes[2] = new AxisProperty();
		gridPosition = GridPosition.FOREGROUND;
		title = null;
		autoClear = false;
		filled = false;
		camera = new Camera();
		box = new Box();
		margins = new double[4];
		axesBounds = new double[4];
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		if (propertyName.equals(__GO_X_AXIS_VISIBLE__)) {
			return AxesProperty.XAXISVISIBLE;
		} else if (propertyName.equals(__GO_X_AXIS_REVERSE__)) {
			return AxesProperty.XAXISREVERSE;
		} else if (propertyName.equals(__GO_X_AXIS_GRID_COLOR__)) {
			return AxesProperty.XAXISGRIDCOLOR;
		} else if (propertyName.equals(__GO_X_AXIS_LABEL__)) {
			return AxesProperty.XAXISLABEL;
		} else if (propertyName.equals(__GO_X_AXIS_LOCATION__)) {
			return AxesProperty.XAXISLOCATION;
		} else if (propertyName.equals(__GO_X_AXIS_LOG_FLAG__)) {
			return AxesProperty.XAXISLOGFLAG;
		} else if (propertyName.equals(__GO_X_AXIS_TICKS__)) {
			return AxesProperty.XAXISTICKS;
		} else if (propertyName.equals(__GO_X_AXIS_AUTO_TICKS__)) {
			return AxesProperty.XAXISAUTOTICKS;
		} else if (propertyName.equals(__GO_X_AXIS_TICKS_LOCATIONS__)) {
			return AxesProperty.XAXISTICKSLOCATIONS;
		} else if (propertyName.equals(__GO_X_AXIS_TICKS_LABELS__)) {
			return AxesProperty.XAXISTICKSLABELS;
		} else if (propertyName.equals(__GO_X_AXIS_SUBTICKS__)) {
			return AxesProperty.XAXISSUBTICKS;
		} else if (propertyName.equals(__GO_Y_AXIS_VISIBLE__)) {
			return AxesProperty.YAXISVISIBLE;
		} else if (propertyName.equals(__GO_Y_AXIS_REVERSE__)) {
			return AxesProperty.YAXISREVERSE;
		} else if (propertyName.equals(__GO_Y_AXIS_GRID_COLOR__)) {
			return AxesProperty.YAXISGRIDCOLOR;
		} else if (propertyName.equals(__GO_Y_AXIS_LABEL__)) {
			return AxesProperty.YAXISLABEL;
		} else if (propertyName.equals(__GO_Y_AXIS_LOCATION__)) {
			return AxesProperty.YAXISLOCATION;
		} else if (propertyName.equals(__GO_Y_AXIS_LOG_FLAG__)) {
			return AxesProperty.YAXISLOGFLAG;
		} else if (propertyName.equals(__GO_Y_AXIS_TICKS__)) {
			return AxesProperty.YAXISTICKS;
		} else if (propertyName.equals(__GO_Y_AXIS_AUTO_TICKS__)) {
			return AxesProperty.YAXISAUTOTICKS;
		} else if (propertyName.equals(__GO_Y_AXIS_TICKS_LOCATIONS__)) {
			return AxesProperty.YAXISTICKSLOCATIONS;
		} else if (propertyName.equals(__GO_Y_AXIS_TICKS_LABELS__)) {
			return AxesProperty.YAXISTICKSLABELS;
		} else if (propertyName.equals(__GO_Y_AXIS_SUBTICKS__)) {
			return AxesProperty.YAXISSUBTICKS;
		} else if (propertyName.equals(__GO_Z_AXIS_VISIBLE__)) {
			return AxesProperty.ZAXISVISIBLE;
		} else if (propertyName.equals(__GO_Z_AXIS_REVERSE__)) {
			return AxesProperty.ZAXISREVERSE;
		} else if (propertyName.equals(__GO_Z_AXIS_GRID_COLOR__)) {
			return AxesProperty.ZAXISGRIDCOLOR;
		} else if (propertyName.equals(__GO_Z_AXIS_LABEL__)) {
			return AxesProperty.ZAXISLABEL;
		} else if (propertyName.equals(__GO_Z_AXIS_LOCATION__)) {
			return AxesProperty.ZAXISLOCATION;
		} else if (propertyName.equals(__GO_Z_AXIS_LOG_FLAG__)) {
			return AxesProperty.ZAXISLOGFLAG;
		} else if (propertyName.equals(__GO_Z_AXIS_TICKS__)) {
			return AxesProperty.ZAXISTICKS;
		} else if (propertyName.equals(__GO_Z_AXIS_AUTO_TICKS__)) {
			return AxesProperty.ZAXISAUTOTICKS;
		} else if (propertyName.equals(__GO_Z_AXIS_TICKS_LOCATIONS__)) {
			return AxesProperty.ZAXISTICKSLOCATIONS;
		} else if (propertyName.equals(__GO_Z_AXIS_TICKS_LABELS__)) {
			return AxesProperty.ZAXISTICKSLABELS;
		} else if (propertyName.equals(__GO_Z_AXIS_SUBTICKS__)) {
			return AxesProperty.ZAXISSUBTICKS;
		} else if (propertyName.equals(__GO_GRID_POSITION__)) {
			return AxesProperty.GRIDPOSITION;
		} else if (propertyName.equals(__GO_TITLE__)) {
			return AxesProperty.TITLE;
		} else if (propertyName.equals(__GO_AUTO_CLEAR__)) {
			return AxesProperty.AUTOCLEAR;
		} else if (propertyName.equals(__GO_FILLED__)) {
			return AxesProperty.FILLED;
		} else if (propertyName.equals(__GO_VIEW__)) {
			return Camera.CameraProperty.VIEW;
		} else if (propertyName.equals(__GO_ISOVIEW__)) {
			return Camera.CameraProperty.ISOVIEW;
		} else if (propertyName.equals(__GO_CUBE_SCALING__)) {
			return Camera.CameraProperty.CUBESCALING;
		} else if (propertyName.equals(__GO_ROTATION_ANGLES__)) {
			return Camera.CameraProperty.ROTATIONANGLES;
		} else if (propertyName.equals(__GO_BOX_TYPE__)) {
			return Box.BoxProperty.BOX;
		} else if (propertyName.equals(__GO_HIDDEN_AXIS_COLOR__)) {
			return Box.BoxProperty.HIDDENAXISCOLOR;
		} else if (propertyName.equals(__GO_TIGHT_LIMITS__)) {
			return Box.BoxProperty.TIGHTLIMITS;
		} else if (propertyName.equals(__GO_DATA_BOUNDS__)) {
			return Box.BoxProperty.DATABOUNDS;
		} else if (propertyName.equals(__GO_REAL_DATA_BOUNDS__)) {
			return Box.BoxProperty.REALDATABOUNDS;
		} else if (propertyName.equals(__GO_ZOOM_BOX__)) {
			return Box.BoxProperty.ZOOMBOX;
		} else if (propertyName.equals(__GO_AUTO_SCALE__)) {
			return Box.BoxProperty.AUTOSCALE;
		} else if (propertyName.equals(__GO_MARGINS__)) {
			return AxesProperty.MARGINS;
		} else if (propertyName.equals(__GO_AXES_BOUNDS__)) {
			return AxesProperty.AXESBOUNDS;
		} else {
			return super.getPropertyFromName(propertyName);
		}
	}

	/**
	 * Fast property get method
	 * @param property the property to get
	 * @return the property value
	 */
	public Object getProperty(Object property) {
		if (property == AxesProperty.XAXISVISIBLE) {
			return getXAxisVisible();
		} else if (property == AxesProperty.XAXISREVERSE) {
			return getXAxisReverse();
		} else if (property == AxesProperty.XAXISGRIDCOLOR) {
			return getXAxisGridColor();
		} else if (property == AxesProperty.XAXISLABEL) {
			return getXAxisLabel();
		} else if (property == AxesProperty.XAXISLOCATION) {
			return getXAxisLocation();
		} else if (property == AxesProperty.XAXISLOGFLAG) {
			return getXAxisLogFlag();
		} else if (property == AxesProperty.XAXISTICKS) {
			return getXAxisTicks();
		} else if (property == AxesProperty.XAXISAUTOTICKS) {
			return getXAxisAutoTicks();
		} else if (property == AxesProperty.XAXISTICKSLOCATIONS) {
			return getXAxisTicksLocations();
		} else if (property == AxesProperty.XAXISTICKSLABELS) {
			return getXAxisTicksLabels();
		} else if (property == AxesProperty.XAXISSUBTICKS) {
			return getXAxisSubticks();
		} else if (property == AxesProperty.YAXISVISIBLE) {
			return getYAxisVisible();
		} else if (property == AxesProperty.YAXISREVERSE) {
			return getYAxisReverse();
		} else if (property == AxesProperty.YAXISGRIDCOLOR) {
			return getYAxisGridColor();
		} else if (property == AxesProperty.YAXISLABEL) {
			return getYAxisLabel();
		} else if (property == AxesProperty.YAXISLOCATION) {
			return getYAxisLocation();
		} else if (property == AxesProperty.YAXISLOGFLAG) {
			return getYAxisLogFlag();
		} else if (property == AxesProperty.YAXISTICKS) {
			return getYAxisTicks();
		} else if (property == AxesProperty.YAXISAUTOTICKS) {
			return getYAxisAutoTicks();
		} else if (property == AxesProperty.YAXISTICKSLOCATIONS) {
			return getYAxisTicksLocations();
		} else if (property == AxesProperty.YAXISTICKSLABELS) {
			return getYAxisTicksLabels();
		} else if (property == AxesProperty.YAXISSUBTICKS) {
			return getYAxisSubticks();
		} else if (property == AxesProperty.ZAXISVISIBLE) {
			return getZAxisVisible();
		} else if (property == AxesProperty.ZAXISREVERSE) {
			return getZAxisReverse();
		} else if (property == AxesProperty.ZAXISGRIDCOLOR) {
			return getZAxisGridColor();
		} else if (property == AxesProperty.ZAXISLABEL) {
			return getZAxisLabel();
		} else if (property == AxesProperty.ZAXISLOCATION) {
			return getZAxisLocation();
		} else if (property == AxesProperty.ZAXISLOGFLAG) {
			return getZAxisLogFlag();
		} else if (property == AxesProperty.ZAXISTICKS) {
			return getZAxisTicks();
		} else if (property == AxesProperty.ZAXISAUTOTICKS) {
			return getZAxisAutoTicks();
		} else if (property == AxesProperty.ZAXISTICKSLOCATIONS) {
			return getZAxisTicksLocations();
		} else if (property == AxesProperty.ZAXISTICKSLABELS) {
			return getZAxisTicksLabels();
		} else if (property == AxesProperty.ZAXISSUBTICKS) {
			return getZAxisSubticks();
		} else if (property == AxesProperty.GRIDPOSITION) {
			return getGridPosition();
		} else if (property == AxesProperty.TITLE) {
			return getTitle();
		} else if (property == AxesProperty.AUTOCLEAR) {
			return getAutoClear();
		} else if (property == AxesProperty.FILLED) {
			return getFilled();
		} else if (property == Camera.CameraProperty.VIEW) {
			return getView();
		} else if (property == Camera.CameraProperty.ISOVIEW) {
			return getIsoview();
		} else if (property == Camera.CameraProperty.CUBESCALING) {
			return getCubeScaling();
		} else if (property == Camera.CameraProperty.ROTATIONANGLES) {
			return getRotationAngles();
		} else if (property == Box.BoxProperty.BOX) {
			return getBoxType();
		} else if (property == Box.BoxProperty.HIDDENAXISCOLOR) {
			return getHiddenAxisColor();
		} else if (property == Box.BoxProperty.TIGHTLIMITS) {
			return getTightLimits();
		} else if (property == Box.BoxProperty.DATABOUNDS) {
			return getDataBounds();
		} else if (property == Box.BoxProperty.REALDATABOUNDS) {
			return getRealDataBounds();
		} else if (property == Box.BoxProperty.ZOOMBOX) {
			return getZoomBox();
		} else if (property == Box.BoxProperty.AUTOSCALE) {
			return getAutoScale();
		} else if (property == AxesProperty.MARGINS) {
			return getMargins();
		} else if (property == AxesProperty.AXESBOUNDS) {
			return getAxesBounds();
		} else {
			return super.getProperty(property);
		}
	}

	/**
	 * Fast property set method
	 * @param property the property to set
	 * @param value the property value
	 * @return true if the property has been set, false otherwise
	 */
	public boolean setProperty(Object property, Object value) {
		if (property == AxesProperty.XAXISVISIBLE) {
			setXAxisVisible((Boolean) value);
		} else if (property == AxesProperty.XAXISREVERSE) {
			setXAxisReverse((Boolean) value);	
		} else if (property == AxesProperty.XAXISGRIDCOLOR) {
			setXAxisGridColor((Integer) value);
		} else if (property == AxesProperty.XAXISLABEL) {
			setXAxisLabel((Label) value);
		} else if (property == AxesProperty.XAXISLOCATION) {
			setXAxisLocation((AxisLocation) value);
		} else if (property == AxesProperty.XAXISLOGFLAG) {
			setXAxisLogFlag((Boolean) value);
		} else if (property == AxesProperty.XAXISTICKS) {
			setXAxisTicks((TicksProperty) value);
		} else if (property == AxesProperty.XAXISAUTOTICKS) {
			setXAxisAutoTicks((Boolean) value);
		} else if (property == AxesProperty.XAXISTICKSLOCATIONS) {
			setXAxisTicksLocations((Double[]) value);
		} else if (property == AxesProperty.XAXISTICKSLABELS) {
			setXAxisTicksLabels((ArrayList<FormattedText>) value);
		} else if (property == AxesProperty.XAXISSUBTICKS) {
			setXAxisSubticks((Integer) value);
		} else if (property == AxesProperty.YAXISVISIBLE) {
			setYAxisVisible((Boolean) value);
		} else if (property == AxesProperty.YAXISREVERSE) {
			setYAxisReverse((Boolean) value);	
		} else if (property == AxesProperty.YAXISGRIDCOLOR) {
			setYAxisGridColor((Integer) value);
		} else if (property == AxesProperty.YAXISLABEL) {
			setYAxisLabel((Label) value);
		} else if (property == AxesProperty.YAXISLOCATION) {
			setYAxisLocation((AxisLocation) value);
		} else if (property == AxesProperty.YAXISLOGFLAG) {
			setYAxisLogFlag((Boolean) value);
		} else if (property == AxesProperty.YAXISTICKS) {
			setYAxisTicks((TicksProperty) value);
		} else if (property == AxesProperty.YAXISAUTOTICKS) {
			setYAxisAutoTicks((Boolean) value);
		} else if (property == AxesProperty.YAXISTICKSLOCATIONS) {
			setYAxisTicksLocations((Double[]) value);
		} else if (property == AxesProperty.YAXISTICKSLABELS) {
			setYAxisTicksLabels((ArrayList<FormattedText>) value);
		} else if (property == AxesProperty.YAXISSUBTICKS) {
			setYAxisSubticks((Integer) value);
		} else if (property == AxesProperty.ZAXISVISIBLE) {
			setZAxisVisible((Boolean) value);
		} else if (property == AxesProperty.ZAXISREVERSE) {
			setZAxisReverse((Boolean) value);	
		} else if (property == AxesProperty.ZAXISGRIDCOLOR) {
			setZAxisGridColor((Integer) value);
		} else if (property == AxesProperty.ZAXISLABEL) {
			setZAxisLabel((Label) value);
		} else if (property == AxesProperty.ZAXISLOCATION) {
			setZAxisLocation((AxisLocation) value);
		} else if (property == AxesProperty.ZAXISLOGFLAG) {
			setZAxisLogFlag((Boolean) value);
		} else if (property == AxesProperty.ZAXISTICKS) {
			setZAxisTicks((TicksProperty) value);
		} else if (property == AxesProperty.ZAXISAUTOTICKS) {
			setZAxisAutoTicks((Boolean) value);
		} else if (property == AxesProperty.ZAXISTICKSLOCATIONS) {
			setZAxisTicksLocations((Double[]) value);
		} else if (property == AxesProperty.ZAXISTICKSLABELS) {
			setZAxisTicksLabels((ArrayList<FormattedText>) value);
		} else if (property == AxesProperty.ZAXISSUBTICKS) {
			setZAxisSubticks((Integer) value);
		} else if (property == AxesProperty.GRIDPOSITION) {
			setGridPosition((GridPosition) value);
		} else if (property == AxesProperty.TITLE) {
			setTitle((String) value);
		} else if (property == AxesProperty.AUTOCLEAR) {
			setAutoClear((Boolean) value);
		} else if (property == AxesProperty.FILLED) {
			setFilled((Boolean) value);
		} else if (property == Camera.CameraProperty.VIEW) {
			setView((ViewType) value);
		} else if (property == Camera.CameraProperty.ISOVIEW) {
			setIsoview((Boolean) value);
		} else if (property == Camera.CameraProperty.CUBESCALING) {
			setCubeScaling((Boolean) value);
		} else if (property == Camera.CameraProperty.ROTATIONANGLES) {
			setRotationAngles((Double[]) value);
		} else if (property == Box.BoxProperty.HIDDENAXISCOLOR) {
			setHiddenAxisColor((Integer) value);
		} else if (property == Box.BoxProperty.TIGHTLIMITS) {
			setTightLimits((Boolean) value);
		} else if (property == Box.BoxProperty.DATABOUNDS) {
			setDataBounds((Double[]) value);
		} else if (property == Box.BoxProperty.REALDATABOUNDS) {
			setRealDataBounds((Double[]) value);
		} else if (property == Box.BoxProperty.ZOOMBOX) {
			setZoomBox((Double[]) value);
		} else if (property == Box.BoxProperty.AUTOSCALE) {
			setAutoScale((Boolean) value);
		} else if (property == AxesProperty.MARGINS) {
			setMargins((Double[]) value);
		} else if (property == AxesProperty.AXESBOUNDS) {
			setAxesBounds((Double[]) value);
		} else {
			return super.setProperty(property, value);
		}

		return true;
	}

	/**
	 * @return the autoClear
	 */
	public Boolean getAutoClear() {
		return autoClear;
	}

	/**
	 * @param autoClear the autoClear to set
	 */
	public void setAutoClear(Boolean autoClear) {
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
	 * @return the x axis
	 */
	public AxisProperty getXAxis() {
		return axes[0];
	}

	/**
	 * @param xAxis the x axis to set
	 */
	public void setXAxis(AxisProperty xAxis) {
		axes[0] = xAxis;
	}

	/**
	 * @return the x axis visible
	 */
	public Boolean getXAxisVisible() {
		return axes[0].getVisible();
	}
	
	/**
	 * @param visible the x axis visible to set
	 */
	public void setXAxisVisible(Boolean visible) {
		axes[0].setVisible(visible);
	}

	/**
	 * @return the x axis reverse
	 */
	public Boolean getXAxisReverse() {
		return axes[0].getReverse();
	}
	
	/**
	 * @param reverse the x axis reverse to set
	 */
	public void setXAxisReverse(Boolean reverse) {
		axes[0].setReverse(reverse);
	}
	
	/**
	 * @return the x axis grid color
	 */
	public Integer getXAxisGridColor() {
		return axes[0].getGridColor();
	}
	
	/**
	 * @param gridColor the x axis grid color to set
	 */
	public void setXAxisGridColor(Integer gridColor) {
		axes[0].setGridColor(gridColor);
	}

	/**
	 * @return the x axis label
	 */
	public Label getXAxisLabel() {
		return axes[0].getLabel();
	}
	
	/**
	 * @param label the x axis label to set
	 */
	public void setXAxisLabel(Label label) {
		axes[0].setLabel(label);
	}

	/**
	 * @return the x axis location
	 */
	public AxisLocation getXAxisLocation() {
		return axes[0].getAxisLocation();
	}

	/**
	 * @param axisLocation the x axis location to set
	 */
	public void setXAxisLocation(AxisLocation axisLocation) {
		axes[0].setAxisLocation(axisLocation);
	}

	/**
	 * @return the x axis log flag
	 */
	public Boolean getXAxisLogFlag() {
		return axes[0].getLogFlag();
	}

	/**
	 * @param logFlag the x axis log flag to set
	 */
	public void setXAxisLogFlag(Boolean logFlag) {
		axes[0].setLogFlag(logFlag);
	}

	/**
	 * @return the x axis ticks
	 */
	public TicksProperty getXAxisTicks() {
		return axes[0].getTicks();
	}

	/**
	 * @param ticks the x axis ticks to set 
	 */
	public void setXAxisTicks(TicksProperty ticks) {
		axes[0].setTicks(ticks);
	}

	/**
	 * @return the x axis autoticks
	 */
	public Boolean getXAxisAutoTicks() {
		return axes[0].getAutoTicks();
	}
	
	/**
	 * @param autoTicks the x axis autoticks to set
	 */
	public void setXAxisAutoTicks(Boolean autoTicks) {
		axes[0].setAutoTicks(autoTicks);
	}

	/**
	 * @return the x axis ticks locations
	 */
	public Double[] getXAxisTicksLocations() {
		return axes[0].getTicksLocations();
	}
	
	/**
	 * @param ticksLocations the x axis ticks locations to set
	 */
	public void setXAxisTicksLocations(Double[] ticksLocations) {
			axes[0].setTicksLocations(ticksLocations);
	}

	/**
	 * @return the x axis ticks labels
	 */
	public ArrayList<FormattedText> getXAxisTicksLabels() {
		return axes[0].getTicksLabels();
	}

	/**
	 * @param labels the x axis ticks labels to set
	 */
	public void setXAxisTicksLabels(ArrayList<FormattedText> labels) {
		axes[0].setTicksLabels(labels);
	}

	/**
	 * @return the x axis number of subticks
	 */
	public Integer getXAxisSubticks() {
		return axes[0].getSubticks();
	}

	/**
	 * @param subticks the x axis number of subticks to set
	 */
	public void setXAxisSubticks(Integer subticks) {
		axes[0].setSubticks(subticks);
	}
	
	/**
	 * @return the y axis
	 */
	public AxisProperty getYAxis() {
		return axes[1];
	}

	/**
	 * @param yAxis the y axis to set
	 */
	public void setYAxis(AxisProperty yAxis) {
		axes[1] = yAxis;
	}

	/**
	 * @return the y axis visible
	 */
	public Boolean getYAxisVisible() {
		return axes[1].getVisible();
	}
	
	/**
	 * @param visible the y axis visible to set
	 */
	public void setYAxisVisible(Boolean visible) {
		axes[1].setVisible(visible);
	}

	/**
	 * @return the y axis reverse
	 */
	public Boolean getYAxisReverse() {
		return axes[1].getReverse();
	}
	
	/**
	 * @param reverse the y axis reverse to set
	 */
	public void setYAxisReverse(Boolean reverse) {
		axes[1].setReverse(reverse);
	}
	
	/**
	 * @return the y axis grid color
	 */
	public Integer getYAxisGridColor() {
		return axes[1].getGridColor();
	}
	
	/**
	 * @param gridColor the y axis grid color to set
	 */
	public void setYAxisGridColor(Integer gridColor) {
		axes[1].setGridColor(gridColor);
	}

	/**
	 * @return the y axis label
	 */
	public Label getYAxisLabel() {
		return axes[1].getLabel();
	}
	
	/**
	 * @param label the y axis label to set
	 */
	public void setYAxisLabel(Label label) {
		axes[1].setLabel(label);
	}

	/**
	 * @return the y axis location
	 */
	public AxisLocation getYAxisLocation() {
		return axes[1].getAxisLocation();
	}

	/**
	 * @param axisLocation the y axis location to set
	 */
	public void setYAxisLocation(AxisLocation axisLocation) {
		axes[1].setAxisLocation(axisLocation);
	}

	/**
	 * @return the y axis log flag
	 */
	public Boolean getYAxisLogFlag() {
		return axes[1].getLogFlag();
	}

	/**
	 * @param logFlag the y axis log flag to set
	 */
	public void setYAxisLogFlag(Boolean logFlag) {
		axes[1].setLogFlag(logFlag);
	}

	/**
	 * @return the y axis ticks
	 */
	public TicksProperty getYAxisTicks() {
		return axes[1].getTicks();
	}

	/**
	 * @param ticks the y axis ticks to set 
	 */
	public void setYAxisTicks(TicksProperty ticks) {
		axes[1].setTicks(ticks);
	}

	/**
	 * @return the y axis autoticks
	 */
	public Boolean getYAxisAutoTicks() {
		return axes[1].getAutoTicks();
	}

	/**
	 * @param autoTicks the y axis autoticks to set
	 */
	public void setYAxisAutoTicks(Boolean autoTicks) {
		axes[1].setAutoTicks(autoTicks);
	}

	/**
	 * @return the y axis ticks locations
	 */
	public Double[] getYAxisTicksLocations() {
		return axes[1].getTicksLocations();
	}

	/**
	 * @param ticksLocations the y axis ticks locations to set
	 */
	public void setYAxisTicksLocations(Double[] ticksLocations) {
			axes[1].setTicksLocations(ticksLocations);
	}

	/**
	 * @return the y axis ticks labels
	 */
	public ArrayList<FormattedText> getYAxisTicksLabels() {
		return axes[1].getTicksLabels();
	}

	/**
	 * @param labels the y axis ticks labels to set
	 */
	public void setYAxisTicksLabels(ArrayList<FormattedText> labels) {
		axes[1].setTicksLabels(labels);
	}

	/**
	 * @return the y axis number of subticks
	 */
	public Integer getYAxisSubticks() {
		return axes[1].getSubticks();
	}

	/**
	 * @param subticks the y axis number of subticks to set
	 */
	public void setYAxisSubticks(Integer subticks) {
		axes[1].setSubticks(subticks);
	}

	/**
	 * @return the z axis
	 */
	public AxisProperty getZAxis() {
		return axes[2];
	}

	/**
	 * @param zAxis the z axis to set
	 */
	public void setZAxis(AxisProperty zAxis) {
		axes[2] = zAxis;
	}

	/**
	 * @return the z axis visible
	 */
	public Boolean getZAxisVisible() {
		return axes[2].getVisible();
	}
	
	/**
	 * @param visible the z axis visible to set
	 */
	public void setZAxisVisible(Boolean visible) {
		axes[2].setVisible(visible);
	}

	/**
	 * @return the z axis reverse
	 */
	public Boolean getZAxisReverse() {
		return axes[2].getReverse();
	}
	
	/**
	 * @param reverse the z axis reverse to set
	 */
	public void setZAxisReverse(Boolean reverse) {
		axes[2].setReverse(reverse);
	}
	
	/**
	 * @return the z axis grid color
	 */
	public Integer getZAxisGridColor() {
		return axes[2].getGridColor();
	}
	
	/**
	 * @param gridColor the z axis grid color to set
	 */
	public void setZAxisGridColor(Integer gridColor) {
		axes[2].setGridColor(gridColor);
	}

	/**
	 * @return the z axis label
	 */
	public Label getZAxisLabel() {
		return axes[2].getLabel();
	}

	/**
	 * @param label the z axis label to set
	 */
	public void setZAxisLabel(Label label) {
		axes[2].setLabel(label);
	}

	/**
	 * @return the z axis location
	 */
	public AxisLocation getZAxisLocation() {
		return axes[2].getAxisLocation();
	}

	/**
	 * @param axisLocation the z axis location to set
	 */
	public void setZAxisLocation(AxisLocation axisLocation) {
		axes[2].setAxisLocation(axisLocation);
	}

	/**
	 * @return the z axis log flag
	 */
	public Boolean getZAxisLogFlag() {
		return axes[2].getLogFlag();
	}

	/**
	 * @param logFlag the z axis log flag to set
	 */
	public void setZAxisLogFlag(Boolean logFlag) {
		axes[2].setLogFlag(logFlag);
	}
	
	/**
	 * @return the z axis ticks
	 */
	public TicksProperty getZAxisTicks() {
		return axes[2].getTicks();
	}

	/**
	 * @param ticks the z axis ticks to set 
	 */
	public void setZAxisTicks(TicksProperty ticks) {
		axes[2].setTicks(ticks);
	}

	/**
	 * @return the z axis autoticks
	 */
	public Boolean getZAxisAutoTicks() {
		return axes[2].getAutoTicks();
	}
	
	/**
	 * @param autoTicks the z axis autoticks to set
	 */
	public void setZAxisAutoTicks(Boolean autoTicks) {
		axes[2].setAutoTicks(autoTicks);
	}

	/**
	 * @return the z axis ticks locations
	 */
	public Double[] getZAxisTicksLocations() {
		return axes[2].getTicksLocations();
	}
	
	/**
	 * @param ticksLocations the z axis ticks locations to set
	 */
	public void setZAxisTicksLocations(Double[] ticksLocations) {
		axes[2].setTicksLocations(ticksLocations);
	}

	/**
	 * @return the z axis ticks labels
	 */
	public ArrayList<FormattedText> getZAxisTicksLabels() {
		return axes[2].getTicksLabels();
	}

	/**
	 * @param labels the z axis ticks labels to set
	 */
	public void setZAxisTicksLabels(ArrayList<FormattedText> labels) {
		axes[2].setTicksLabels(labels);
	}

	/**
	 * @return the z axis number of subticks
	 */
	public Integer getZAxisSubticks() {
		return axes[2].getSubticks();
	}

	/**
	 * @param subticks the z axis number of subticks to set
	 */
	public void setZAxisSubticks(Integer subticks) {
		axes[2].setSubticks(subticks);
	}

	/**
	 * @return the axesBounds
	 */
	public Double[] getAxesBounds() {
		Double[] retAxesBounds = new Double[4];

		retAxesBounds[0] = axesBounds[0];
		retAxesBounds[1] = axesBounds[1];
		retAxesBounds[2] = axesBounds[2];
		retAxesBounds[3] = axesBounds[3];

		return retAxesBounds;
	}

	/**
	 * @param axesBounds the axesBounds to set
	 */
	public void setAxesBounds(Double[] axesBounds) {
		this.axesBounds[0] = axesBounds[0];
		this.axesBounds[1] = axesBounds[1];
		this.axesBounds[2] = axesBounds[2];
		this.axesBounds[3] = axesBounds[3];
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
	 * @return the box type
	 */
	public BoxType getBoxType() {
		return box.getBox();
	}

	/**
	 * @param box the BoxType to set
	 */
	public void setBoxType(BoxType box) {
		this.box.setBox(box);
	}

	/**
	 * @return the hidden axis color
	 */
	public Integer getHiddenAxisColor() {
		return box.getHiddenAxisColor();
	}

	/**
	 * @param color the hidden axis color to set
	 */
	public void setHiddenAxisColor(Integer color) {
		box.setHiddenAxisColor(color);
	}

	/**
	 * @return the tight limits
	 */
	public Boolean getTightLimits() {
		return box.getTightLimits();
	}

	/**
	 * @param tightLimits the tightLimits to set 
	 */
	public void setTightLimits(Boolean tightLimits) {
		box.setTightLimits(tightLimits);
	}

	/**
	 * @return the data bounds
	 */
	public Double[] getDataBounds() {
		return box.getDataBounds();
	}

	/**
	 * @param dataBounds the data bounds to set
	 */
	public void setDataBounds(Double[] dataBounds) {
		box.setDataBounds(dataBounds);
	}

	/**
	 * @return the real data bounds
	 */
	public Double[] getRealDataBounds() {
		return box.getRealDataBounds();
	}

	/**
	 * @param realDataBounds the real data bounds to set
	 */
	public void setRealDataBounds(Double[] realDataBounds) {
		box.setRealDataBounds(realDataBounds);
	}

	/**
	 * @return the zoom box
	 */
	public Double[] getZoomBox() {
		return box.getZoomBox();
	}

	/**
	 * @param zoomBox the zoom box to set
	 */
	public void setZoomBox(Double[] zoomBox) {
		box.setZoomBox(zoomBox);
	}

	/**
	 * @return the autoscale
	 */
	public Boolean getAutoScale() {
		return box.getAutoScale();
	}

	/**
	 * @param autoScale the autoscale to set 
	 */
	public void setAutoScale(Boolean autoScale) {
		box.setAutoScale(autoScale);
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
	 * @return the view type
	 */
	public ViewType getView() {
		return camera.getView();
	}

	/**
	 * @param view the view type to set
	 */
	public void setView(ViewType view) {
		camera.setView(view);
	}

	/**
	 * @return the isoview
	 */
	public Boolean getIsoview() {
		return camera.getIsoview();
	}

	/**
	 * @param isoview the isoview to set
	 */
	public void setIsoview(Boolean isoview) {
		camera.setIsoview(isoview);
	}

	/**
	 * @return the cubescaling
	 */
	public Boolean getCubeScaling() {
		return camera.getCubeScaling();
	}

	/**
	 * @param cubeScaling the cubescaling to set
	 */
	public void setCubeScaling(Boolean cubeScaling) {
		camera.setCubeScaling(cubeScaling);
	}

	/**
	 * @return the rotation angles
	 */
	public Double[] getRotationAngles() {
		return camera.getRotationAngles();
	}

	/**
	 * @param rotationAngles the rotation angles to set
	 */
	public void setRotationAngles(Double[] rotationAngles) {
		camera.setRotationAngles(rotationAngles);
	}

	/**
	 * @return the filled
	 */
	public Boolean getFilled() {
		return filled;
	}

	/**
	 * @param filled the filled to set
	 */
	public void setFilled(Boolean filled) {
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
	public Double[] getMargins() {
		Double[] retMargins = new Double[4];

		retMargins[0] = margins[0];
		retMargins[1] = margins[1];
		retMargins[2] = margins[2];
		retMargins[3] = margins[3];

		return retMargins;
	}

	/**
	 * @param margins the margins to set
	 */
	public void setMargins(Double[] margins) {
		this.margins[0] = margins[0];
		this.margins[1] = margins[1];
		this.margins[2] = margins[2];
		this.margins[3] = margins[3];
	}

	/**
	 * @return the title UID
	 */
	public String getTitle() {
		return title;
	}

	/**
	 * @param title the title to set
	 */
	public void setTitle(String title) {
		this.title = title;
	}

}
