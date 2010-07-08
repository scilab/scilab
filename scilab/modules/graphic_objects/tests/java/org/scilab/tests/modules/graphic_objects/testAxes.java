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

package org.scilab.tests.modules.graphic_objects;

import java.rmi.server.UID;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axes.Axes.GridPosition;
import org.scilab.modules.graphic_objects.axes.AxisProperty.AxisLocation;
import org.scilab.modules.graphic_objects.axes.Box.BoxType;
import org.scilab.modules.graphic_objects.axes.Camera.ViewType;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.label.Label;
import org.scilab.modules.graphic_objects.textObject.FormattedText;
import org.scilab.modules.graphic_objects.textObject.Font;

import org.testng.annotations.*;

/**
 * Axes test class
 * @author Manuel JULIACHS
 */
public class testAxes {

	/** The default number of ticks */
	public static final int DEFAULT_NUMBER_OF_TICKS = 20;

	/** Figure identifier */
	private String figureID;

	/** The Axes identifier */
	private String axesID;

	/** X-Axis label identifier */
	private String xlabelID;

	/** Y-Axis label identifier */
	private String ylabelID;

	/** Z-Axis label identifier */
	private String zlabelID;

	/** Title label identifier */
	private String titleID;

	/** Graphic controller */
	private GraphicController controller;

	/** X-Axis label */
	private	Label xlabel;

	/** Y-Axis label */
	private Label ylabel;

	/** Z-Axis label */
	private Label zlabel;

	/** title label */
	private Label title;

	/** Axes object used for data initialization */
	private Axes initAxes;

	/**
	 * Creates the Axes graphic object.
	 * Called only once.
	 */
	@BeforeClass
	public void createAxes() {

		controller = GraphicController.getController();
		assert controller != null;

		axesID = controller.askObject(Type.AXES);
		GraphicObject axes = (GraphicObject) controller.getObjectFromId(axesID);

		assert axes != null;
	}

	/**
	 * Utility method which creates the graphic objects used by the Axes object and
	 * initializes data relevant to the Axes properties.
	 * Must be called prior to any set/get calls, once per group of tests (see below).
	 * Most properties are randomly chosen (for example the axes properties such as visible,
	 * reverse, data bounds values are offset by a random value, etc.). Only a few are not
	 * (such as each axis' location or the box type).
	 * The point of random initialization is to ensure that set and get work correctly, by comparing
	 * the initial random values which are then assigned by set calls to the results obtained by get calls
	 * (the former and the latter should of course be equal).
	 */
	@BeforeGroups(value = { "ReflectSetGetTest", "FastSetGetTest" }, groups = { "InitData" })
	public void initializeData() {

		figureID = controller.askObject(Type.FIGURE);
		GraphicObject parentFigure = (GraphicObject) controller.getObjectFromId(figureID);
		assert parentFigure != null;

		xlabelID = controller.askObject(Type.LABEL);
		xlabel = (Label) controller.getObjectFromId(xlabelID);
		assert xlabel != null;

		ylabelID = controller.askObject(Type.LABEL);
		ylabel = (Label) controller.getObjectFromId(ylabelID);
		assert ylabel != null;

		zlabelID = controller.askObject(Type.LABEL);
		zlabel = (Label) controller.getObjectFromId(zlabelID);
		assert zlabel != null;

		titleID = controller.askObject(Type.LABEL);
		title = (Label) controller.getObjectFromId(titleID);
		assert title != null;
		
		Random randomGen = new Random();

		initAxes = new Axes();
		assert initAxes != null;


		/* GraphicObjects properties */
		initAxes.setVisible(randomGen.nextBoolean());

		/* Axes properties */
		initAxes.setXAxisVisible(randomGen.nextBoolean());
		initAxes.setXAxisReverse(randomGen.nextBoolean());
		initAxes.setXAxisGridColor(randomGen.nextInt());
		initAxes.setXAxisLocation(AxisLocation.BOTTOM);
		initAxes.setXAxisLogFlag(randomGen.nextBoolean());
		initAxes.setXAxisAutoTicks(randomGen.nextBoolean());
		initAxes.setXAxisSubticks(randomGen.nextInt());

		initAxes.setYAxisVisible(randomGen.nextBoolean());
		initAxes.setYAxisReverse(randomGen.nextBoolean());
		initAxes.setYAxisGridColor(randomGen.nextInt());
		initAxes.setYAxisLocation(AxisLocation.RIGHT);
		initAxes.setYAxisLogFlag(randomGen.nextBoolean());
		initAxes.setYAxisAutoTicks(randomGen.nextBoolean());
		initAxes.setYAxisSubticks(randomGen.nextInt());

		initAxes.setZAxisVisible(randomGen.nextBoolean());
		initAxes.setZAxisReverse(randomGen.nextBoolean());
		initAxes.setZAxisGridColor(randomGen.nextInt());
		initAxes.setZAxisLocation(AxisLocation.ORIGIN);
		initAxes.setZAxisLogFlag(randomGen.nextBoolean());
		initAxes.setZAxisAutoTicks(randomGen.nextBoolean());
		initAxes.setZAxisSubticks(randomGen.nextInt());

		/* ticks locations */

		Double dataXShift = randomGen.nextDouble() % 0.1;
		Double dataYShift = randomGen.nextDouble() % 0.1;
		Double dataZShift = randomGen.nextDouble() % 0.1;

		Double xMin = dataXShift;
		Double yMin = dataXShift;
		Double zMin = -1.0 + dataXShift;

		Double[] dataBounds = new Double[] {dataXShift, dataYShift, -1.0 + dataZShift,
				1.0 + dataXShift, 1.0 + dataYShift, 1.0 + dataZShift};
		Double[] realDataBounds = new Double[] {dataXShift, dataYShift, -1.0 + dataZShift,
				1.0 + dataXShift, 1.0 + dataYShift, 1.0 + dataZShift};

		Double[] xlocs = new Double[DEFAULT_NUMBER_OF_TICKS];
		Double[] ylocs = new Double[DEFAULT_NUMBER_OF_TICKS];
		Double[] zlocs = new Double[DEFAULT_NUMBER_OF_TICKS];

		for (int i = 0; i < DEFAULT_NUMBER_OF_TICKS; i++) {
			double posRatio = (double) i / (double) DEFAULT_NUMBER_OF_TICKS;
			xlocs[i] = posRatio * (dataBounds[3] - dataBounds[0]) + dataBounds[0];
			ylocs[i] = posRatio * (dataBounds[4] - dataBounds[1]) + dataBounds[1];
			zlocs[i] = posRatio * (dataBounds[5] - dataBounds[2]) + dataBounds[2];
		}

		ArrayList <FormattedText> xlabels = new ArrayList(DEFAULT_NUMBER_OF_TICKS);
		ArrayList <FormattedText> ylabels = new ArrayList(DEFAULT_NUMBER_OF_TICKS);
		ArrayList <FormattedText> zlabels = new ArrayList(DEFAULT_NUMBER_OF_TICKS);

		Font xFont = new Font();
		xFont.setColor(randomGen.nextInt());
		xFont.setSize(1.0);
		Font yFont = new Font();
		yFont.setColor(randomGen.nextInt());
		yFont.setSize(2.0);
		Font zFont = new Font();
		zFont.setColor(randomGen.nextInt());
		zFont.setSize(3.0);

		for (int i = 0; i < DEFAULT_NUMBER_OF_TICKS; i++) {
			FormattedText newtext = new FormattedText();
			Integer ind = new Integer(i);
			newtext.setText("Xtext" + ind.toString());
			newtext.setFont(xFont);
			xlabels.add(newtext);
			newtext = new FormattedText();
			newtext.setText("Ytext" + ind.toString());
			newtext.setFont(yFont);
			ylabels.add(newtext);
			newtext = new FormattedText();
			newtext.setText("Ztext" + ind.toString());
			newtext.setFont(zFont);
			zlabels.add(newtext);
		}

		initAxes.setXAxisTicksLocations(xlocs);
		initAxes.setXAxisTicksLabels(xlabels);
		initAxes.setYAxisTicksLocations(ylocs);
		initAxes.setYAxisTicksLabels(ylabels);
		initAxes.setZAxisTicksLocations(zlocs);
		initAxes.setZAxisTicksLabels(zlabels);

		/* Miscellaneous Axes properties */
		Double marginXShift = randomGen.nextDouble() % 0.1;
		Double marginYShift = randomGen.nextDouble() % 0.1;
		Double[] margins = new Double[] {0.15 + marginXShift, 0.15 + marginXShift, 0.15 + marginYShift, 0.15 + marginYShift};
		Double axesBoundsXShift = randomGen.nextDouble() % 0.1;
		Double axesBoundsYShift = randomGen.nextDouble() % 0.1;
		Double[] axesBounds = new Double[] {axesBoundsXShift, axesBoundsYShift, 1.0 - axesBoundsXShift, 1.0 - axesBoundsYShift};

		initAxes.setGridPosition(GridPosition.BACKGROUND);
		initAxes.setAutoClear(randomGen.nextBoolean());
		initAxes.setFilled(randomGen.nextBoolean());
		initAxes.setMargins(margins);
		initAxes.setAxesBounds(axesBounds);

		/* Camera properties */
		Double[] rotationAngles = new Double[] {randomGen.nextDouble() * 90.0, randomGen.nextDouble() * 360.0};

		initAxes.setView(ViewType.VIEW_3D);
		initAxes.setCubeScaling(randomGen.nextBoolean());
		initAxes.setIsoview(randomGen.nextBoolean());
		initAxes.setRotationAngles(rotationAngles);

		/* Box properties */
		Double zoomXShift = randomGen.nextDouble() % 0.1;
		Double zoomYShift = randomGen.nextDouble() % 0.1;
		Double zoomZShift = randomGen.nextDouble() % 0.1;
		Double[] zoomBox = new Double[] {zoomXShift, zoomYShift, -1.0 + zoomZShift, 1.0 - zoomXShift, 1.0 - zoomYShift, 1.0 - zoomZShift};

		initAxes.setBoxType(BoxType.ON);
		initAxes.setHiddenAxisColor(randomGen.nextInt());
		initAxes.setTightLimits(randomGen.nextBoolean());
		initAxes.setDataBounds(dataBounds);
		initAxes.setRealDataBounds(realDataBounds);
		initAxes.setZoomBox(zoomBox);
		initAxes.setAutoScale(randomGen.nextBoolean());
	}

	/* Reflection-based set/get methods tests */
	/* GraphicObject properties */
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetAxesVisible() {
		controller.setProperty(axesID, "Visible", initAxes.getVisible());
		Boolean axesVisible  = (Boolean) controller.getProperty(axesID, "Visible");
		assert axesVisible.equals(initAxes.getVisible());
	}

	/* X-Axis properties */
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetXAxisVisible() {
		controller.setProperty(axesID, "XAxisVisible", initAxes.getXAxisVisible());
		Boolean retAxisVisible = (Boolean) controller.getProperty(axesID, "XAxisVisible");
		assert (retAxisVisible.equals(initAxes.getXAxisVisible()));
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetXAxisReverse() {
		controller.setProperty(axesID, "XAxisReverse", initAxes.getXAxisReverse());
		Boolean retAxisReverse = (Boolean) controller.getProperty(axesID, "XAxisReverse");
		assert (retAxisReverse.equals(initAxes.getXAxisReverse()));
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetXAxisGridColor() {
		controller.setProperty(axesID, "XAxisGridColor", initAxes.getXAxisGridColor());
		Integer retGridColor = (Integer) controller.getProperty(axesID, "XAxisGridColor");
		assert retGridColor.equals(initAxes.getXAxisGridColor());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetXAxisLabel() {
		controller.setProperty(axesID, "XAxisLabel", xlabel);
		Label retXlabel = (Label) controller.getProperty(axesID, "XAxisLabel");
		assert retXlabel.equals(xlabel);
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetXAxisLocation() {
		controller.setProperty(axesID, "XAxisLocation", initAxes.getXAxisLocation());
		AxisLocation retAxisLocation = (AxisLocation) controller.getProperty(axesID, "XAxisLocation");
		assert retAxisLocation.equals(initAxes.getXAxisLocation());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetXAxisLogFlag() {
		controller.setProperty(axesID, "XAxisLogFlag", initAxes.getXAxisLogFlag());
		Boolean retLogFlag = (Boolean) controller.getProperty(axesID, "XAxisLogFlag");
		assert retLogFlag.equals(initAxes.getXAxisLogFlag());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetXAxisAutoTicks() {
		controller.setProperty(axesID, "XAxisAutoTicks", initAxes.getXAxisAutoTicks());
		Boolean retAutoTicks = (Boolean) controller.getProperty(axesID, "XAxisAutoTicks");
		assert retAutoTicks.equals(initAxes.getXAxisAutoTicks());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetXAxisSubTicks() {
		controller.setProperty(axesID, "XAxisSubticks", initAxes.getXAxisSubticks());
		Integer retSubTicks = (Integer) controller.getProperty(axesID, "XAxisSubticks");
		assert retSubTicks.equals(initAxes.getXAxisSubticks());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetXAxisTicksLocations() {
		controller.setProperty(axesID, "XAxisTicksLocations", initAxes.getXAxisTicksLocations());
		Double[] retTicksLocations = (Double[]) controller.getProperty(axesID, "XAxisTicksLocations");
		assert Arrays.equals(retTicksLocations, initAxes.getXAxisTicksLocations());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetXAxisTicksLabels() {
		controller.setProperty(axesID, "XAxisTicksLabels", initAxes.getXAxisTicksLabels());
		ArrayList retTicksLabels = (ArrayList) controller.getProperty(axesID, "XAxisTicksLabels");
		assert compareFormattedTextLists(retTicksLabels, initAxes.getXAxisTicksLabels());
	}

	/* Y-Axis properties */
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetYAxisVisible() {
		controller.setProperty(axesID, "YAxisVisible", initAxes.getYAxisVisible());
		Boolean retAxisVisible = (Boolean) controller.getProperty(axesID, "YAxisVisible");
		assert (retAxisVisible.equals(initAxes.getYAxisVisible()));
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetYAxisReverse() {
		controller.setProperty(axesID, "YAxisReverse", initAxes.getYAxisReverse());
		Boolean retAxisReverse = (Boolean) controller.getProperty(axesID, "YAxisReverse");
		assert (retAxisReverse.equals(initAxes.getYAxisReverse()));
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetYAxisGridColor() {
		controller.setProperty(axesID, "YAxisGridColor", initAxes.getYAxisGridColor());
		Integer retGridColor = (Integer) controller.getProperty(axesID, "YAxisGridColor");
		assert retGridColor.equals(initAxes.getYAxisGridColor());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetYAxisLabel() {
		controller.setProperty(axesID, "YAxisLabel", ylabel);
		Label retlabel = (Label) controller.getProperty(axesID, "YAxisLabel");
		assert retlabel.equals(ylabel);
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetYAxisLocation() {
		controller.setProperty(axesID, "YAxisLocation", initAxes.getYAxisLocation());
		AxisLocation retAxisLocation = (AxisLocation) controller.getProperty(axesID, "YAxisLocation");
		assert retAxisLocation.equals(initAxes.getYAxisLocation());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetYAxisLogFlag() {
		controller.setProperty(axesID, "YAxisLogFlag", initAxes.getYAxisLogFlag());
		Boolean retLogFlag = (Boolean) controller.getProperty(axesID, "YAxisLogFlag");
		assert retLogFlag.equals(initAxes.getYAxisLogFlag());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetYAxisAutoTicks() {
		controller.setProperty(axesID, "YAxisAutoTicks", initAxes.getYAxisAutoTicks());
		Boolean retAutoTicks = (Boolean) controller.getProperty(axesID, "YAxisAutoTicks");
		assert retAutoTicks.equals(initAxes.getYAxisAutoTicks());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetYAxisSubTicks() {
		controller.setProperty(axesID, "YAxisSubticks", initAxes.getYAxisSubticks());
		Integer retSubTicks = (Integer) controller.getProperty(axesID, "YAxisSubticks");
		assert retSubTicks.equals(initAxes.getYAxisSubticks());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetYAxisTicksLocations() {
		controller.setProperty(axesID, "YAxisTicksLocations", initAxes.getYAxisTicksLocations());
		Double[] retTicksLocations = (Double[]) controller.getProperty(axesID, "YAxisTicksLocations");
		assert Arrays.equals(retTicksLocations, initAxes.getYAxisTicksLocations());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetYAxisTicksLabels() {
		controller.setProperty(axesID, "YAxisTicksLabels", initAxes.getYAxisTicksLabels());
		ArrayList retTicksLabels = (ArrayList) controller.getProperty(axesID, "YAxisTicksLabels");
		assert compareFormattedTextLists(retTicksLabels, initAxes.getYAxisTicksLabels());
	}

	/* Z-Axis properties */
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetZAxisVisible() {
		controller.setProperty(axesID, "ZAxisVisible", initAxes.getZAxisVisible());
		Boolean retAxisVisible = (Boolean) controller.getProperty(axesID, "ZAxisVisible");
		assert (retAxisVisible.equals(initAxes.getZAxisVisible()));
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetZAxisReverse() {
		controller.setProperty(axesID, "ZAxisReverse", initAxes.getZAxisReverse());
		Boolean retAxisReverse = (Boolean) controller.getProperty(axesID, "ZAxisReverse");
		assert (retAxisReverse.equals(initAxes.getZAxisReverse()));
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetZAxisGridColor() {
		controller.setProperty(axesID, "ZAxisGridColor", initAxes.getZAxisGridColor());
		Integer retGridColor = (Integer) controller.getProperty(axesID, "ZAxisGridColor");
		assert retGridColor.equals(initAxes.getZAxisGridColor());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetZAxisLabel() {
		controller.setProperty(axesID, "ZAxisLabel", zlabel);
		Label retlabel = (Label) controller.getProperty(axesID, "ZAxisLabel");
		assert retlabel.equals(zlabel);
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetZAxisLocation() {
		controller.setProperty(axesID, "ZAxisLocation", initAxes.getZAxisLocation());
		AxisLocation retAxisLocation = (AxisLocation) controller.getProperty(axesID, "ZAxisLocation");
		assert retAxisLocation.equals(initAxes.getZAxisLocation());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetZAxisLogFlag() {
		controller.setProperty(axesID, "ZAxisLogFlag", initAxes.getZAxisLogFlag());
		Boolean retLogFlag = (Boolean) controller.getProperty(axesID, "ZAxisLogFlag");
		assert retLogFlag.equals(initAxes.getZAxisLogFlag());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetZAxisAutoTicks() {
		controller.setProperty(axesID, "ZAxisAutoTicks", initAxes.getZAxisAutoTicks());
		Boolean retAutoTicks = (Boolean) controller.getProperty(axesID, "ZAxisAutoTicks");
		assert retAutoTicks.equals(initAxes.getZAxisAutoTicks());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetZAxisSubTicks() {
		controller.setProperty(axesID, "ZAxisSubticks", initAxes.getZAxisSubticks());
		Integer retSubTicks = (Integer) controller.getProperty(axesID, "ZAxisSubticks");
		assert retSubTicks.equals(initAxes.getZAxisSubticks());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetZAxisTicksLocations() {
		controller.setProperty(axesID, "ZAxisTicksLocations", initAxes.getZAxisTicksLocations());
		Double[] retTicksLocations = (Double[]) controller.getProperty(axesID, "ZAxisTicksLocations");
		assert Arrays.equals(retTicksLocations, initAxes.getZAxisTicksLocations());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetZAxisTicksLabels() {
		controller.setProperty(axesID, "ZAxisTicksLabels", initAxes.getZAxisTicksLabels());
		ArrayList retTicksLabels = (ArrayList) controller.getProperty(axesID, "ZAxisTicksLabels");
		assert compareFormattedTextLists(retTicksLabels, initAxes.getZAxisTicksLabels());
	}

	/* Miscellaneous Axes properties */
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetGridPosition() {
		controller.setProperty(axesID, "GridPosition", initAxes.getGridPosition());
		GridPosition retGridPos = (GridPosition) controller.getProperty(axesID, "GridPosition");
		assert retGridPos.equals(initAxes.getGridPosition());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetTitle() {
		controller.setProperty(axesID, "Title", title);
		Label retTitle = (Label) controller.getProperty(axesID, "Title");
		assert retTitle.equals(title);
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetAutoClear() {
		controller.setProperty(axesID, "AutoClear", initAxes.getAutoClear());
		Boolean retAutoClear = (Boolean) controller.getProperty(axesID, "AutoClear");
		assert retAutoClear.equals(initAxes.getAutoClear());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetFilled() {
		controller.setProperty(axesID, "Filled", initAxes.getFilled());
		Boolean retFilled = (Boolean) controller.getProperty(axesID, "Filled");
		assert retFilled.equals(initAxes.getFilled());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetMargins() {
		controller.setProperty(axesID, "Margins", initAxes.getMargins());
		Double[] retMargins = (Double[]) controller.getProperty(axesID, "Margins");
		assert Arrays.equals(retMargins, initAxes.getMargins());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetAxesBounds() {
		controller.setProperty(axesID, "AxesBounds", initAxes.getAxesBounds());
		Double[] retAxesBounds = (Double[]) controller.getProperty(axesID, "AxesBounds");
		assert Arrays.equals(retAxesBounds, initAxes.getAxesBounds());
	}

	/* Camera properties */
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetView() {
		controller.setProperty(axesID, "View", initAxes.getView());
		ViewType retView = (ViewType) controller.getProperty(axesID, "View");
		assert retView.equals(initAxes.getView());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetIsoview() {
		controller.setProperty(axesID, "Isoview", initAxes.getIsoview());
		Boolean retIsoview = (Boolean) controller.getProperty(axesID, "Isoview");
		assert retIsoview.equals(initAxes.getIsoview());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetCubeScaling() {
		controller.setProperty(axesID, "CubeScaling", initAxes.getCubeScaling());
		Boolean retCubeScaling = (Boolean) controller.getProperty(axesID, "CubeScaling");
		assert retCubeScaling.equals(initAxes.getCubeScaling());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetRotationAngles() {
		controller.setProperty(axesID, "RotationAngles", initAxes.getRotationAngles());
		Double[] retRotationAngles = (Double[]) controller.getProperty(axesID, "RotationAngles");
		assert (Arrays.equals(retRotationAngles, initAxes.getRotationAngles()) && retRotationAngles.length == 2);
	}

	/* Box properties */
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetBoxType() {
		controller.setProperty(axesID, "BoxType", initAxes.getBoxType());
		BoxType retBoxType = (BoxType) controller.getProperty(axesID, "BoxType");
		assert retBoxType.equals(initAxes.getBoxType());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetHiddenAxisColor() {
		controller.setProperty(axesID, "HiddenAxisColor", initAxes.getHiddenAxisColor());
		Integer retHiddenAxisColor = (Integer) controller.getProperty(axesID, "HiddenAxisColor");
		assert retHiddenAxisColor.equals(initAxes.getHiddenAxisColor());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetTightLimits() {
		controller.setProperty(axesID, "TightLimits", initAxes.getTightLimits());
		Boolean retTightLimits = (Boolean) controller.getProperty(axesID, "TightLimits");
		assert retTightLimits.equals(initAxes.getTightLimits());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetDataBounds() {
		controller.setProperty(axesID, "DataBounds", initAxes.getDataBounds());
		Double[] retDataBounds = (Double[]) controller.getProperty(axesID, "DataBounds");
		assert Arrays.equals(retDataBounds, initAxes.getDataBounds());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetRealDataBounds() {
		controller.setProperty(axesID, "RealDataBounds", initAxes.getRealDataBounds());
		Double[] retRealDataBounds = (Double[]) controller.getProperty(axesID, "RealDataBounds");
		assert Arrays.equals(retRealDataBounds, initAxes.getRealDataBounds());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetZoomBox() {
		controller.setProperty(axesID, "ZoomBox", initAxes.getZoomBox());
		Double[] retZoomBox = (Double[]) controller.getProperty(axesID, "ZoomBox");
		assert Arrays.equals(retZoomBox, initAxes.getZoomBox());
	}
	@Test(groups = { "ReflectSetGetTest" })
	public void testReflectSetGetAutoScale() {
		controller.setProperty(axesID, "AutoScale", initAxes.getAutoScale());
		Boolean retAutoScale = (Boolean) controller.getProperty(axesID, "AutoScale");
		assert retAutoScale.equals(initAxes.getAutoScale());
	}

	/* Fast set/get methods tests */
	/* GraphicObject properties */

	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetParent() {
		GraphicObject parentFigure = (GraphicObject) controller.getObjectFromId(figureID);
		controller.setProperty(axesID, "Parent", parentFigure);
		GraphicObject retParent = (GraphicObject) controller.getProperty(axesID, "Parent");
		assert retParent.equals(parentFigure);
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetAxesVisible() {
		controller.setProperty(axesID, "Visible", initAxes.getVisible());
		Boolean axesVisible  = (Boolean) controller.getProperty(axesID, "Visible");
		assert axesVisible.equals(initAxes.getVisible());
	}


	/* X-Axis properties */
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetXAxisVisible() {
		controller.setProperty(axesID, "XAxisVisible", initAxes.getXAxisVisible());
		Boolean retAxisVisible = (Boolean) controller.getProperty(axesID, "XAxisVisible");
		assert (retAxisVisible.equals(initAxes.getXAxisVisible()));
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetXAxisReverse() {
		controller.setProperty(axesID, "XAxisReverse", initAxes.getXAxisReverse());
		Boolean retAxisReverse = (Boolean) controller.getProperty(axesID, "XAxisReverse");
		assert (retAxisReverse.equals(initAxes.getXAxisReverse()));
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetXAxisGridColor() {
		controller.setProperty(axesID, "XAxisGridColor", initAxes.getXAxisGridColor());
		Integer retGridColor = (Integer) controller.getProperty(axesID, "XAxisGridColor");
		assert retGridColor.equals(initAxes.getXAxisGridColor());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetXAxisLabel() {
		controller.setProperty(axesID, "XAxisLabel", xlabel);
		Label retXlabel = (Label) controller.getProperty(axesID, "XAxisLabel");
		assert retXlabel.equals(xlabel);
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetXAxisLocation() {
		controller.setProperty(axesID, "XAxisLocation", initAxes.getXAxisLocation());
		AxisLocation retAxisLocation = (AxisLocation) controller.getProperty(axesID, "XAxisLocation");
		assert retAxisLocation.equals(initAxes.getXAxisLocation());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetXAxisLogFlag() {
		controller.setProperty(axesID, "XAxisLogFlag", initAxes.getXAxisLogFlag());
		Boolean retLogFlag = (Boolean) controller.getProperty(axesID, "XAxisLogFlag");
		assert retLogFlag.equals(initAxes.getXAxisLogFlag());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetXAxisAutoTicks() {
		controller.setProperty(axesID, "XAxisAutoTicks", initAxes.getXAxisAutoTicks());
		Boolean retAutoTicks = (Boolean) controller.getProperty(axesID, "XAxisAutoTicks");
		assert retAutoTicks.equals(initAxes.getXAxisAutoTicks());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetXAxisSubTicks() {
		controller.setProperty(axesID, "XAxisSubticks", initAxes.getXAxisSubticks());
		Integer retSubTicks = (Integer) controller.getProperty(axesID, "XAxisSubticks");
		assert retSubTicks.equals(initAxes.getXAxisSubticks());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetXAxisTicksLocations() {
		controller.setProperty(axesID, "XAxisTicksLocations", initAxes.getXAxisTicksLocations());
		Double[] retTicksLocations = (Double[]) controller.getProperty(axesID, "XAxisTicksLocations");
		assert Arrays.equals(retTicksLocations, initAxes.getXAxisTicksLocations());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetXAxisTicksLabels() {
		controller.setProperty(axesID, "XAxisTicksLabels", initAxes.getXAxisTicksLabels());
		ArrayList retTicksLabels = (ArrayList) controller.getProperty(axesID, "XAxisTicksLabels");
		assert compareFormattedTextLists(retTicksLabels, initAxes.getXAxisTicksLabels());
	}

	/* Y-Axis properties */
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetYAxisVisible() {
		controller.setProperty(axesID, "YAxisVisible", initAxes.getYAxisVisible());
		Boolean retAxisVisible = (Boolean) controller.getProperty(axesID, "YAxisVisible");
		assert (retAxisVisible.equals(initAxes.getYAxisVisible()));
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetYAxisReverse() {
		controller.setProperty(axesID, "YAxisReverse", initAxes.getYAxisReverse());
		Boolean retAxisReverse = (Boolean) controller.getProperty(axesID, "YAxisReverse");
		assert (retAxisReverse.equals(initAxes.getYAxisReverse()));
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetYAxisGridColor() {
		controller.setProperty(axesID, "YAxisGridColor", initAxes.getYAxisGridColor());
		Integer retGridColor = (Integer) controller.getProperty(axesID, "YAxisGridColor");
		assert retGridColor.equals(initAxes.getYAxisGridColor());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetYAxisLabel() {
		controller.setProperty(axesID, "YAxisLabel", ylabel);
		Label retlabel = (Label) controller.getProperty(axesID, "YAxisLabel");
		assert retlabel.equals(ylabel);
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetYAxisLocation() {
		controller.setProperty(axesID, "YAxisLocation", initAxes.getYAxisLocation());
		AxisLocation retAxisLocation = (AxisLocation) controller.getProperty(axesID, "YAxisLocation");
		assert retAxisLocation.equals(initAxes.getYAxisLocation());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetYAxisLogFlag() {
		controller.setProperty(axesID, "YAxisLogFlag", initAxes.getYAxisLogFlag());
		Boolean retLogFlag = (Boolean) controller.getProperty(axesID, "YAxisLogFlag");
		assert retLogFlag.equals(initAxes.getYAxisLogFlag());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetYAxisAutoTicks() {
		controller.setProperty(axesID, "YAxisAutoTicks", initAxes.getYAxisAutoTicks());
		Boolean retAutoTicks = (Boolean) controller.getProperty(axesID, "YAxisAutoTicks");
		assert retAutoTicks.equals(initAxes.getYAxisAutoTicks());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetYAxisSubTicks() {
		controller.setProperty(axesID, "YAxisSubticks", initAxes.getYAxisSubticks());
		Integer retSubTicks = (Integer) controller.getProperty(axesID, "YAxisSubticks");
		assert retSubTicks.equals(initAxes.getYAxisSubticks());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetYAxisTicksLocations() {
		controller.setProperty(axesID, "YAxisTicksLocations", initAxes.getYAxisTicksLocations());
		Double[] retTicksLocations = (Double[]) controller.getProperty(axesID, "YAxisTicksLocations");
		assert Arrays.equals(retTicksLocations, initAxes.getYAxisTicksLocations());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetYAxisTicksLabels() {
		controller.setProperty(axesID, "YAxisTicksLabels", initAxes.getYAxisTicksLabels());
		ArrayList retTicksLabels = (ArrayList) controller.getProperty(axesID, "YAxisTicksLabels");
		assert compareFormattedTextLists(retTicksLabels, initAxes.getYAxisTicksLabels());
	}

	/* Z-Axis properties */
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetZAxisVisible() {
		controller.setProperty(axesID, "ZAxisVisible", initAxes.getZAxisVisible());
		Boolean retAxisVisible = (Boolean) controller.getProperty(axesID, "ZAxisVisible");
		assert (retAxisVisible.equals(initAxes.getZAxisVisible()));
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetZAxisReverse() {
		controller.setProperty(axesID, "ZAxisReverse", initAxes.getZAxisReverse());
		Boolean retAxisReverse = (Boolean) controller.getProperty(axesID, "ZAxisReverse");
		assert (retAxisReverse.equals(initAxes.getZAxisReverse()));
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetZAxisGridColor() {
		controller.setProperty(axesID, "ZAxisGridColor", initAxes.getZAxisGridColor());
		Integer retGridColor = (Integer) controller.getProperty(axesID, "ZAxisGridColor");
		assert retGridColor.equals(initAxes.getZAxisGridColor());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetZAxisLabel() {
		controller.setProperty(axesID, "ZAxisLabel", zlabel);
		Label retlabel = (Label) controller.getProperty(axesID, "ZAxisLabel");
		assert retlabel.equals(zlabel);
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetZAxisLocation() {
		controller.setProperty(axesID, "ZAxisLocation", initAxes.getZAxisLocation());
		AxisLocation retAxisLocation = (AxisLocation) controller.getProperty(axesID, "ZAxisLocation");
		assert retAxisLocation.equals(initAxes.getZAxisLocation());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetZAxisLogFlag() {
		controller.setProperty(axesID, "ZAxisLogFlag", initAxes.getZAxisLogFlag());
		Boolean retLogFlag = (Boolean) controller.getProperty(axesID, "ZAxisLogFlag");
		assert retLogFlag.equals(initAxes.getZAxisLogFlag());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetZAxisAutoTicks() {
		controller.setProperty(axesID, "ZAxisAutoTicks", initAxes.getZAxisAutoTicks());
		Boolean retAutoTicks = (Boolean) controller.getProperty(axesID, "ZAxisAutoTicks");
		assert retAutoTicks.equals(initAxes.getZAxisAutoTicks());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetZAxisSubTicks() {
		controller.setProperty(axesID, "ZAxisSubticks", initAxes.getZAxisSubticks());
		Integer retSubTicks = (Integer) controller.getProperty(axesID, "ZAxisSubticks");
		assert retSubTicks.equals(initAxes.getZAxisSubticks());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetZAxisTicksLocations() {
		controller.setProperty(axesID, "ZAxisTicksLocations", initAxes.getZAxisTicksLocations());
		Double[] retTicksLocations = (Double[]) controller.getProperty(axesID, "ZAxisTicksLocations");
		assert Arrays.equals(retTicksLocations, initAxes.getZAxisTicksLocations());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetZAxisTicksLabels() {
		controller.setProperty(axesID, "ZAxisTicksLabels", initAxes.getZAxisTicksLabels());
		ArrayList retTicksLabels = (ArrayList) controller.getProperty(axesID, "ZAxisTicksLabels");
		assert compareFormattedTextLists(retTicksLabels, initAxes.getZAxisTicksLabels());
	}

	/* Miscellaneous Axes properties */
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetGridPosition() {
		controller.setProperty(axesID, "GridPosition", initAxes.getGridPosition());
		GridPosition retGridPos = (GridPosition) controller.getProperty(axesID, "GridPosition");
		assert retGridPos.equals(initAxes.getGridPosition());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetTitle() {
		controller.setProperty(axesID, "Title", title);
		Label retTitle = (Label) controller.getProperty(axesID, "Title");
		assert retTitle.equals(title);
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetAutoClear() {
		controller.setProperty(axesID, "AutoClear", initAxes.getAutoClear());
		Boolean retAutoClear = (Boolean) controller.getProperty(axesID, "AutoClear");
		assert retAutoClear.equals(initAxes.getAutoClear());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetFilled() {
		controller.setProperty(axesID, "Filled", initAxes.getFilled());
		Boolean retFilled = (Boolean) controller.getProperty(axesID, "Filled");
		assert retFilled.equals(initAxes.getFilled());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetMargins() {
		controller.setProperty(axesID, "Margins", initAxes.getMargins());
		Double[] retMargins = (Double[]) controller.getProperty(axesID, "Margins");
		assert Arrays.equals(retMargins, initAxes.getMargins());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetAxesBounds() {
		controller.setProperty(axesID, "AxesBounds", initAxes.getAxesBounds());
		Double[] retAxesBounds = (Double[]) controller.getProperty(axesID, "AxesBounds");
		assert Arrays.equals(retAxesBounds, initAxes.getAxesBounds());
	}

	/* Camera properties */
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetView() {
		controller.setProperty(axesID, "View", initAxes.getView());
		ViewType retView = (ViewType) controller.getProperty(axesID, "View");
		assert retView.equals(initAxes.getView());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetIsoview() {
		controller.setProperty(axesID, "Isoview", initAxes.getIsoview());
		Boolean retIsoview = (Boolean) controller.getProperty(axesID, "Isoview");
		assert retIsoview.equals(initAxes.getIsoview());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetCubeScaling() {
		controller.setProperty(axesID, "CubeScaling", initAxes.getCubeScaling());
		Boolean retCubeScaling = (Boolean) controller.getProperty(axesID, "CubeScaling");
		assert retCubeScaling.equals(initAxes.getCubeScaling());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetRotationAngles() {
		controller.setProperty(axesID, "RotationAngles", initAxes.getRotationAngles());
		Double[] retRotationAngles = (Double[]) controller.getProperty(axesID, "RotationAngles");
		assert (Arrays.equals(retRotationAngles, initAxes.getRotationAngles()) && retRotationAngles.length == 2);
	}

	/* Box properties */
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetBoxType() {
		controller.setProperty(axesID, "BoxType", initAxes.getBoxType());
		BoxType retBoxType = (BoxType) controller.getProperty(axesID, "BoxType");
		assert retBoxType.equals(initAxes.getBoxType());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetHiddenAxisColor() {
		controller.setProperty(axesID, "HiddenAxisColor", initAxes.getHiddenAxisColor());
		Integer retHiddenAxisColor = (Integer) controller.getProperty(axesID, "HiddenAxisColor");
		assert retHiddenAxisColor.equals(initAxes.getHiddenAxisColor());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetTightLimits() {
		controller.setProperty(axesID, "TightLimits", initAxes.getTightLimits());
		Boolean retTightLimits = (Boolean) controller.getProperty(axesID, "TightLimits");
		assert retTightLimits.equals(initAxes.getTightLimits());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetDataBounds() {
		controller.setProperty(axesID, "DataBounds", initAxes.getDataBounds());
		Double[] retDataBounds = (Double[]) controller.getProperty(axesID, "DataBounds");
		assert Arrays.equals(retDataBounds, initAxes.getDataBounds());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetRealDataBounds() {
		controller.setProperty(axesID, "RealDataBounds", initAxes.getRealDataBounds());
		Double[] retRealDataBounds = (Double[]) controller.getProperty(axesID, "RealDataBounds");
		assert Arrays.equals(retRealDataBounds, initAxes.getRealDataBounds());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetZoomBox() {
		controller.setProperty(axesID, "ZoomBox", initAxes.getZoomBox());
		Double[] retZoomBox = (Double[]) controller.getProperty(axesID, "ZoomBox");
		assert Arrays.equals(retZoomBox, initAxes.getZoomBox());
	}
	@Test(groups = { "FastSetGetTest" }, dependsOnGroups = { "ReflectSetGetTest" })
	public void testFastSetGetAutoScale() {
		controller.setProperty(axesID, "AutoScale", initAxes.getAutoScale());
		Boolean retAutoScale = (Boolean) controller.getProperty(axesID, "AutoScale");
		assert retAutoScale.equals(initAxes.getAutoScale());
	}

	/**
	 * A simple utility method which compares two FormattedText ArrayLists
	 * @param list1 the first list
	 * @param list2 the second list
	 * @return true if the two lists are equal, else false
	 */
	public boolean compareFormattedTextLists(ArrayList <FormattedText> list1, ArrayList <FormattedText> list2) {

		if (list1.size() != list2.size()) {
			return false;
		}

		for (int i = 0; i < list1.size(); i++) {
			String s1 = new String(((FormattedText) list1.get(i)).getText());
			String s2 = new String(((FormattedText) list2.get(i)).getText());

			if (!s1.equals(s2)) {
				return false;
			}

			Font f1 = list1.get(i).getFont();
			Font f2 = list2.get(i).getFont();

			if (!f1.equals(f2)) {
				return false;
			}
		}

		return true;
	}
}
