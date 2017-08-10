/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.tests.modules.graphic_objects;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
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

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

import org.junit.*;

/**
 * Axes test class
 * @author Manuel JULIACHS
 */
public class testAxes {

    /** The default number of ticks */
    public static final int DEFAULT_NUMBER_OF_TICKS = 11;

    /** Figure identifier */
    private Integer figureID;

    /** The Axes identifier */
    private Integer axesID;

    /** X-Axis label identifier */
    private Integer xlabelID;

    /** Y-Axis label identifier */
    private Integer ylabelID;

    /** Z-Axis label identifier */
    private Integer zlabelID;

    /** Title label identifier */
    private Integer titleID;

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
    @Before
    public void createAxes() {

        controller = GraphicController.getController();
        assert controller != null;

        axesID = controller.askObject(Type.AXES);
        GraphicObject axes = (GraphicObject) controller.getObjectFromId(axesID);
        assert axes != null;

        /* Allocate an empty figure and link the axes and figure */
        final Integer figureID = controller.askObject(Type.FIGURE);
        axes.setParent(figureID);
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
    @Before
    public void initializeData() {
        this.createAxes();
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

        /* Allocate an empty figure and link the axes and figure */
        final Integer figureInitID = controller.askObject(Type.FIGURE);
        initAxes.setParent(figureInitID);

        /* GraphicObjects properties */
        initAxes.setVisible(randomGen.nextBoolean());

        /* Axes properties */
        Integer axisLocation = randomGen.nextInt(6);
        initAxes.setXAxisVisible(randomGen.nextBoolean());
        initAxes.setXAxisReverse(randomGen.nextBoolean());
        initAxes.setXAxisGridColor(randomGen.nextInt());
        initAxes.setXAxisLocationAsEnum(AxisLocation.intToEnum(axisLocation));
        initAxes.setXAxisLogFlag(randomGen.nextBoolean());
        initAxes.setXAxisAutoTicks(randomGen.nextBoolean());
        initAxes.setXAxisSubticks(randomGen.nextInt());

        axisLocation = randomGen.nextInt(6);
        initAxes.setYAxisVisible(randomGen.nextBoolean());
        initAxes.setYAxisReverse(randomGen.nextBoolean());
        initAxes.setYAxisGridColor(randomGen.nextInt());
        initAxes.setYAxisLocationAsEnum(AxisLocation.intToEnum(axisLocation));
        initAxes.setYAxisLogFlag(randomGen.nextBoolean());
        initAxes.setYAxisAutoTicks(randomGen.nextBoolean());
        initAxes.setYAxisSubticks(randomGen.nextInt());

        axisLocation = randomGen.nextInt(6);
        initAxes.setZAxisVisible(randomGen.nextBoolean());
        initAxes.setZAxisReverse(randomGen.nextBoolean());
        initAxes.setZAxisGridColor(randomGen.nextInt());
        initAxes.setZAxisLocationAsEnum(AxisLocation.intToEnum(axisLocation));
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
                                            1.0 + dataXShift, 1.0 + dataYShift, 1.0 + dataZShift
                                           };
        Double[] realDataBounds = new Double[] {dataXShift, dataYShift, -1.0 + dataZShift,
                                                1.0 + dataXShift, 1.0 + dataYShift, 1.0 + dataZShift
                                               };

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
        initAxes.setXAxisTicksLabelsAsArrayList(xlabels);
        initAxes.setYAxisTicksLocations(ylocs);
        initAxes.setYAxisTicksLabelsAsArrayList(ylabels);
        initAxes.setZAxisTicksLocations(zlocs);
        initAxes.setZAxisTicksLabelsAsArrayList(zlabels);

        initAxes.setAutoSubticks(randomGen.nextBoolean());

        /* Miscellaneous Axes properties */
        Double marginXShift = randomGen.nextDouble() % 0.1;
        Double marginYShift = randomGen.nextDouble() % 0.1;
        Double[] margins = new Double[] {0.15 + marginXShift, 0.15 + marginXShift, 0.15 + marginYShift, 0.15 + marginYShift};
        Double axesBoundsXShift = randomGen.nextDouble() % 0.1;
        Double axesBoundsYShift = randomGen.nextDouble() % 0.1;
        Double[] axesBounds = new Double[] {axesBoundsXShift, axesBoundsYShift, 1.0 - axesBoundsXShift, 1.0 - axesBoundsYShift};

        initAxes.setGridPositionAsEnum(GridPosition.BACKGROUND);
        initAxes.setAutoClear(randomGen.nextBoolean());
        initAxes.setFilled(randomGen.nextBoolean());
        initAxes.setBackground(randomGen.nextInt());
        initAxes.setMargins(margins);
        initAxes.setAxesBounds(axesBounds);

        /* Properties applied to child objects */
        initAxes.setHiddenColor(randomGen.nextInt());
        initAxes.setLineMode(randomGen.nextBoolean());
        initAxes.setLineStyle(randomGen.nextInt(7));
        initAxes.setLineThickness(randomGen.nextDouble());
        initAxes.setLineColor(randomGen.nextInt());

        initAxes.setMarkMode(randomGen.nextBoolean());
        initAxes.setMarkStyle(randomGen.nextInt());
        initAxes.setMarkSize(randomGen.nextInt());
        initAxes.setMarkSizeUnit(randomGen.nextInt(2));
        initAxes.setMarkForeground(randomGen.nextInt());
        initAxes.setMarkBackground(randomGen.nextInt());

        initAxes.setClipState(randomGen.nextInt(3));

        Double xShift = randomGen.nextDouble() % 0.2;
        Double yShift = randomGen.nextDouble() % 0.2;
        Double xDim = 0.8 + randomGen.nextDouble() % 1.0;
        Double yDim = 0.8 + randomGen.nextDouble() % 1.0;
        Double[] clipBox = new Double[] {1.0 - xShift, 1.0 - yShift, xDim, yDim};

        initAxes.setClipBox(clipBox);

        initAxes.setArcDrawingMethod(randomGen.nextInt(2));

        /* Camera properties */
        Double[] rotationAngles = new Double[] {randomGen.nextDouble() * 90.0, randomGen.nextDouble() * 360.0};

        initAxes.setViewAsEnum(ViewType.VIEW_3D);
        initAxes.setCubeScaling(randomGen.nextBoolean());
        initAxes.setIsoview(randomGen.nextBoolean());
        initAxes.setRotationAngles(rotationAngles);

        /* Box properties */
        Double zoomXShift = randomGen.nextDouble() % 0.1;
        Double zoomYShift = randomGen.nextDouble() % 0.1;
        Double zoomZShift = randomGen.nextDouble() % 0.1;
        Double[] zoomBox = new Double[] {zoomXShift, zoomYShift, -1.0 + zoomZShift, 1.0 - zoomXShift, 1.0 - zoomYShift, 1.0 - zoomZShift};

        Integer boxType = randomGen.nextInt(4);
        initAxes.setBoxTypeAsEnum(BoxType.intToEnum(boxType));

        initAxes.setHiddenAxisColor(randomGen.nextInt());
        initAxes.setXTightLimits(randomGen.nextBoolean());
        initAxes.setYTightLimits(randomGen.nextBoolean());
        initAxes.setZTightLimits(randomGen.nextBoolean());
        initAxes.setDataBounds(dataBounds);
        initAxes.setRealDataBounds(realDataBounds);
        initAxes.setZoomEnabled(randomGen.nextBoolean());
        initAxes.setZoomBox(zoomBox);
        initAxes.setAutoScale(randomGen.nextBoolean());
        initAxes.setFirstPlot(randomGen.nextBoolean());
    }

    /* Reflection-based set/get methods tests */
    /* GraphicObject properties */
    @Test
    public void testReflectSetGetAxesVisible() {
        controller.setProperty(axesID, __GO_VISIBLE__, initAxes.getVisible());
        Boolean axesVisible  = (Boolean) controller.getProperty(axesID, __GO_VISIBLE__);
        assert axesVisible.equals(initAxes.getVisible());
    }

    /* X-Axis properties */
    @Test
    public void testReflectSetGetXAxisVisible() {
        controller.setProperty(axesID, __GO_X_AXIS_VISIBLE__, initAxes.getXAxisVisible());
        Boolean retAxisVisible = (Boolean) controller.getProperty(axesID, __GO_X_AXIS_VISIBLE__);
        assert (retAxisVisible.equals(initAxes.getXAxisVisible()));
    }
    @Test
    public void testReflectSetGetXAxisReverse() {
        controller.setProperty(axesID, __GO_X_AXIS_REVERSE__, initAxes.getXAxisReverse());
        Boolean retAxisReverse = (Boolean) controller.getProperty(axesID, __GO_X_AXIS_REVERSE__);
        assert (retAxisReverse.equals(initAxes.getXAxisReverse()));
    }
    @Test
    public void testReflectSetGetXAxisGridColor() {
        controller.setProperty(axesID, __GO_X_AXIS_GRID_COLOR__, initAxes.getXAxisGridColor());
        Integer retGridColor = (Integer) controller.getProperty(axesID, __GO_X_AXIS_GRID_COLOR__);
        assert retGridColor.equals(initAxes.getXAxisGridColor());
    }
    @Test
    public void testReflectSetGetXAxisLabel() {
        controller.setProperty(axesID, __GO_X_AXIS_LABEL__, xlabelID);
        Integer retXlabel = (Integer) controller.getProperty(axesID, __GO_X_AXIS_LABEL__);
        assert retXlabel.equals(xlabelID);
    }
    @Test
    public void testReflectSetGetXAxisLocation() {
        controller.setProperty(axesID, __GO_X_AXIS_LOCATION__, initAxes.getXAxisLocation());
        Integer retAxisLocation = (Integer) controller.getProperty(axesID, __GO_X_AXIS_LOCATION__);
        assert retAxisLocation.equals(initAxes.getXAxisLocation());
    }
    @Test
    public void testReflectSetGetXAxisLogFlag() {
        controller.setProperty(axesID, __GO_X_AXIS_LOG_FLAG__, initAxes.getXAxisLogFlag());
        Boolean retLogFlag = (Boolean) controller.getProperty(axesID, __GO_X_AXIS_LOG_FLAG__);
        assert retLogFlag.equals(initAxes.getXAxisLogFlag());
    }
    @Test
    public void testReflectSetGetXAxisAutoTicks() {
        controller.setProperty(axesID, __GO_X_AXIS_AUTO_TICKS__, initAxes.getXAxisAutoTicks());
        Boolean retAutoTicks = (Boolean) controller.getProperty(axesID, __GO_X_AXIS_AUTO_TICKS__);
        assert retAutoTicks.equals(initAxes.getXAxisAutoTicks());
    }
    @Test
    public void testReflectSetGetXAxisSubTicks() {
        controller.setProperty(axesID, __GO_X_AXIS_SUBTICKS__, initAxes.getXAxisSubticks());
        Integer retSubTicks = (Integer) controller.getProperty(axesID, __GO_X_AXIS_SUBTICKS__);
        assert retSubTicks.equals(initAxes.getXAxisSubticks());
    }
    @Test
    public void testReflectSetGetXAxisTicksLocations() {
        controller.setProperty(axesID, __GO_X_AXIS_TICKS_LOCATIONS__, initAxes.getXAxisTicksLocations());
        Double[] retTicksLocations = (Double[]) controller.getProperty(axesID, __GO_X_AXIS_TICKS_LOCATIONS__);
        assert Arrays.equals(retTicksLocations, initAxes.getXAxisTicksLocations());
    }
    @Test
    public void testReflectSetGetXAxisTicksLabels() {
        controller.setProperty(axesID, __GO_X_AXIS_TICKS_LABELS__, initAxes.getXAxisTicksLabels());
        String[] retTicksLabels = (String[]) controller.getProperty(axesID, __GO_X_AXIS_TICKS_LABELS__);

        assert Arrays.equals(retTicksLabels, initAxes.getXAxisTicksLabels());
    }

    /* Y-Axis properties */
    @Test
    public void testReflectSetGetYAxisVisible() {
        controller.setProperty(axesID, __GO_Y_AXIS_VISIBLE__, initAxes.getYAxisVisible());
        Boolean retAxisVisible = (Boolean) controller.getProperty(axesID, __GO_Y_AXIS_VISIBLE__);
        assert (retAxisVisible.equals(initAxes.getYAxisVisible()));
    }
    @Test
    public void testReflectSetGetYAxisReverse() {
        controller.setProperty(axesID, __GO_Y_AXIS_REVERSE__, initAxes.getYAxisReverse());
        Boolean retAxisReverse = (Boolean) controller.getProperty(axesID, __GO_Y_AXIS_REVERSE__);
        assert (retAxisReverse.equals(initAxes.getYAxisReverse()));
    }
    @Test
    public void testReflectSetGetYAxisGridColor() {
        controller.setProperty(axesID, __GO_Y_AXIS_GRID_COLOR__, initAxes.getYAxisGridColor());
        Integer retGridColor = (Integer) controller.getProperty(axesID, __GO_Y_AXIS_GRID_COLOR__);
        assert retGridColor.equals(initAxes.getYAxisGridColor());
    }
    @Test
    public void testReflectSetGetYAxisLabel() {
        controller.setProperty(axesID, __GO_Y_AXIS_LABEL__, ylabelID);
        Integer retlabel = (Integer) controller.getProperty(axesID, __GO_Y_AXIS_LABEL__);
        assert retlabel.equals(ylabelID);
    }
    @Test
    public void testReflectSetGetYAxisLocation() {
        controller.setProperty(axesID, __GO_Y_AXIS_LOCATION__, initAxes.getYAxisLocation());
        Integer retAxisLocation = (Integer) controller.getProperty(axesID, __GO_Y_AXIS_LOCATION__);
        assert retAxisLocation.equals(initAxes.getYAxisLocation());
    }
    @Test
    public void testReflectSetGetYAxisLogFlag() {
        controller.setProperty(axesID, __GO_Y_AXIS_LOG_FLAG__, initAxes.getYAxisLogFlag());
        Boolean retLogFlag = (Boolean) controller.getProperty(axesID, __GO_Y_AXIS_LOG_FLAG__);
        assert retLogFlag.equals(initAxes.getYAxisLogFlag());
    }
    @Test
    public void testReflectSetGetYAxisAutoTicks() {
        controller.setProperty(axesID, __GO_Y_AXIS_AUTO_TICKS__, initAxes.getYAxisAutoTicks());
        Boolean retAutoTicks = (Boolean) controller.getProperty(axesID, __GO_Y_AXIS_AUTO_TICKS__);
        assert retAutoTicks.equals(initAxes.getYAxisAutoTicks());
    }
    @Test
    public void testReflectSetGetYAxisSubTicks() {
        controller.setProperty(axesID, __GO_Y_AXIS_SUBTICKS__, initAxes.getYAxisSubticks());
        Integer retSubTicks = (Integer) controller.getProperty(axesID, __GO_Y_AXIS_SUBTICKS__);
        assert retSubTicks.equals(initAxes.getYAxisSubticks());
    }
    @Test
    public void testReflectSetGetYAxisTicksLocations() {
        controller.setProperty(axesID, __GO_Y_AXIS_TICKS_LOCATIONS__, initAxes.getYAxisTicksLocations());
        Double[] retTicksLocations = (Double[]) controller.getProperty(axesID, __GO_Y_AXIS_TICKS_LOCATIONS__);
        assert Arrays.equals(retTicksLocations, initAxes.getYAxisTicksLocations());
    }
    @Test
    public void testReflectSetGetYAxisTicksLabels() {
        controller.setProperty(axesID, __GO_Y_AXIS_TICKS_LABELS__, initAxes.getYAxisTicksLabels());
        String[] retTicksLabels = (String[]) controller.getProperty(axesID, __GO_Y_AXIS_TICKS_LABELS__);

        assert Arrays.equals(retTicksLabels, initAxes.getYAxisTicksLabels());
    }

    /* Z-Axis properties */
    @Test
    public void testReflectSetGetZAxisVisible() {
        controller.setProperty(axesID, __GO_Z_AXIS_VISIBLE__, initAxes.getZAxisVisible());
        Boolean retAxisVisible = (Boolean) controller.getProperty(axesID, __GO_Z_AXIS_VISIBLE__);
        assert (retAxisVisible.equals(initAxes.getZAxisVisible()));
    }
    @Test
    public void testReflectSetGetZAxisReverse() {
        controller.setProperty(axesID, __GO_Z_AXIS_REVERSE__, initAxes.getZAxisReverse());
        Boolean retAxisReverse = (Boolean) controller.getProperty(axesID, __GO_Z_AXIS_REVERSE__);
        assert (retAxisReverse.equals(initAxes.getZAxisReverse()));
    }
    @Test
    public void testReflectSetGetZAxisGridColor() {
        controller.setProperty(axesID, __GO_Z_AXIS_GRID_COLOR__, initAxes.getZAxisGridColor());
        Integer retGridColor = (Integer) controller.getProperty(axesID, __GO_Z_AXIS_GRID_COLOR__);
        assert retGridColor.equals(initAxes.getZAxisGridColor());
    }
    @Test
    public void testReflectSetGetZAxisLabel() {
        controller.setProperty(axesID, __GO_Z_AXIS_LABEL__, zlabelID);
        Integer retlabel = (Integer) controller.getProperty(axesID, __GO_Z_AXIS_LABEL__);
        assert retlabel.equals(zlabelID);
    }
    @Test
    public void testReflectSetGetZAxisLocation() {
        controller.setProperty(axesID, __GO_Z_AXIS_LOCATION__, initAxes.getZAxisLocation());
        Integer retAxisLocation = (Integer) controller.getProperty(axesID, __GO_Z_AXIS_LOCATION__);
        assert retAxisLocation.equals(initAxes.getZAxisLocation());
    }
    @Test
    public void testReflectSetGetZAxisLogFlag() {
        controller.setProperty(axesID, __GO_Z_AXIS_LOG_FLAG__, initAxes.getZAxisLogFlag());
        Boolean retLogFlag = (Boolean) controller.getProperty(axesID, __GO_Z_AXIS_LOG_FLAG__);
        assert retLogFlag.equals(initAxes.getZAxisLogFlag());
    }
    @Test
    public void testReflectSetGetZAxisAutoTicks() {
        controller.setProperty(axesID, __GO_Z_AXIS_AUTO_TICKS__, initAxes.getZAxisAutoTicks());
        Boolean retAutoTicks = (Boolean) controller.getProperty(axesID, __GO_Z_AXIS_AUTO_TICKS__);
        assert retAutoTicks.equals(initAxes.getZAxisAutoTicks());
    }
    @Test
    public void testReflectSetGetZAxisSubTicks() {
        controller.setProperty(axesID, __GO_Z_AXIS_SUBTICKS__, initAxes.getZAxisSubticks());
        Integer retSubTicks = (Integer) controller.getProperty(axesID, __GO_Z_AXIS_SUBTICKS__);
        assert retSubTicks.equals(initAxes.getZAxisSubticks());
    }
    @Test
    public void testReflectSetGetZAxisTicksLocations() {
        controller.setProperty(axesID, __GO_Z_AXIS_TICKS_LOCATIONS__, initAxes.getZAxisTicksLocations());
        Double[] retTicksLocations = (Double[]) controller.getProperty(axesID, __GO_Z_AXIS_TICKS_LOCATIONS__);
        assert Arrays.equals(retTicksLocations, initAxes.getZAxisTicksLocations());
    }
    @Test
    public void testReflectSetGetZAxisTicksLabels() {
        controller.setProperty(axesID, __GO_Z_AXIS_TICKS_LABELS__, initAxes.getZAxisTicksLabels());
        String[] retTicksLabels = (String[]) controller.getProperty(axesID, __GO_Z_AXIS_TICKS_LABELS__);

        assert Arrays.equals(retTicksLabels, initAxes.getZAxisTicksLabels());
    }

    /* Ticks property shared by the three axes */
    @Test
    public void testReflectSetGetAutoSubticks() {
        controller.setProperty(axesID, __GO_AUTO_SUBTICKS__, initAxes.getAutoSubticks());
        Boolean retAutoSubticks = (Boolean) controller.getProperty(axesID, __GO_AUTO_SUBTICKS__);
        assert retAutoSubticks.equals(initAxes.getAutoSubticks());
    }

    /* Miscellaneous Axes properties */
    @Test
    public void testReflectSetGetGridPosition() {
        controller.setProperty(axesID, __GO_GRID_POSITION__, initAxes.getGridPosition());
        Integer retGridPos = (Integer) controller.getProperty(axesID, __GO_GRID_POSITION__);
        assert retGridPos.equals(initAxes.getGridPosition());
    }
    @Test
    public void testReflectSetGetTitle() {
        controller.setProperty(axesID, __GO_TITLE__, titleID);
        Integer retTitle = (Integer) controller.getProperty(axesID, __GO_TITLE__);
        assert retTitle.equals(titleID);
    }
    @Test
    public void testReflectSetGetAutoClear() {
        controller.setProperty(axesID, __GO_AUTO_CLEAR__, initAxes.getAutoClear());
        Boolean retAutoClear = (Boolean) controller.getProperty(axesID, __GO_AUTO_CLEAR__);
        assert retAutoClear.equals(initAxes.getAutoClear());
    }
    @Test
    public void testReflectSetGetFilled() {
        controller.setProperty(axesID, __GO_FILLED__, initAxes.getFilled());
        Boolean retFilled = (Boolean) controller.getProperty(axesID, __GO_FILLED__);
        assert retFilled.equals(initAxes.getFilled());
    }
    @Test
    public void testReflectSetGetBackground() {
        controller.setProperty(axesID, __GO_BACKGROUND__, initAxes.getBackground());
        Integer retBackground = (Integer) controller.getProperty(axesID, __GO_BACKGROUND__);
        assert retBackground.equals(initAxes.getBackground());
    }
    @Test
    public void testReflectSetGetMargins() {
        controller.setProperty(axesID, __GO_MARGINS__, initAxes.getMargins());
        Double[] retMargins = (Double[]) controller.getProperty(axesID, __GO_MARGINS__);
        assert Arrays.equals(retMargins, initAxes.getMargins());
    }
    @Test
    public void testReflectSetGetAxesBounds() {
        controller.setProperty(axesID, __GO_AXES_BOUNDS__, initAxes.getAxesBounds());
        Double[] retAxesBounds = (Double[]) controller.getProperty(axesID, __GO_AXES_BOUNDS__);
        assert Arrays.equals(retAxesBounds, initAxes.getAxesBounds());
    }

    /* Camera properties */
    @Test
    public void testReflectSetGetView() {
        controller.setProperty(axesID, __GO_VIEW__, initAxes.getView());
        Integer retView = (Integer) controller.getProperty(axesID, __GO_VIEW__);
        assert retView.equals(initAxes.getView());
    }
    @Test
    public void testReflectSetGetIsoview() {
        controller.setProperty(axesID, __GO_ISOVIEW__, initAxes.getIsoview());
        Boolean retIsoview = (Boolean) controller.getProperty(axesID, __GO_ISOVIEW__);
        assert retIsoview.equals(initAxes.getIsoview());
    }
    @Test
    public void testReflectSetGetCubeScaling() {
        controller.setProperty(axesID, __GO_CUBE_SCALING__, initAxes.getCubeScaling());
        Boolean retCubeScaling = (Boolean) controller.getProperty(axesID, __GO_CUBE_SCALING__);
        assert retCubeScaling.equals(initAxes.getCubeScaling());
    }
    @Test
    public void testReflectSetGetRotationAngles() {
        controller.setProperty(axesID, __GO_ROTATION_ANGLES__, initAxes.getRotationAngles());
        Double[] retRotationAngles = (Double[]) controller.getProperty(axesID, __GO_ROTATION_ANGLES__);
        assert (Arrays.equals(retRotationAngles, initAxes.getRotationAngles()) && retRotationAngles.length == 2);
    }

    /* Box properties */
    @Test
    public void testReflectSetGetBoxType() {
        controller.setProperty(axesID, __GO_BOX_TYPE__, initAxes.getBoxType());
        Integer retBoxType = (Integer) controller.getProperty(axesID, __GO_BOX_TYPE__);
        assert retBoxType.equals(initAxes.getBoxType());
    }
    @Test
    public void testReflectSetGetHiddenAxisColor() {
        controller.setProperty(axesID, __GO_HIDDEN_AXIS_COLOR__, initAxes.getHiddenAxisColor());
        Integer retHiddenAxisColor = (Integer) controller.getProperty(axesID, __GO_HIDDEN_AXIS_COLOR__);
        assert retHiddenAxisColor.equals(initAxes.getHiddenAxisColor());
    }
    @Test
    public void testReflectSetGetXTightLimits() {
        controller.setProperty(axesID, __GO_X_TIGHT_LIMITS__, initAxes.getXTightLimits());
        Boolean retTightLimits = (Boolean) controller.getProperty(axesID, __GO_X_TIGHT_LIMITS__);
        assert retTightLimits.equals(initAxes.getXTightLimits());
    }
    @Test
    public void testReflectSetGetYTightLimits() {
        controller.setProperty(axesID, __GO_Y_TIGHT_LIMITS__, initAxes.getYTightLimits());
        Boolean retTightLimits = (Boolean) controller.getProperty(axesID, __GO_Y_TIGHT_LIMITS__);
        assert retTightLimits.equals(initAxes.getYTightLimits());
    }
    @Test
    public void testReflectSetGetZTightLimits() {
        controller.setProperty(axesID, __GO_Z_TIGHT_LIMITS__, initAxes.getZTightLimits());
        Boolean retTightLimits = (Boolean) controller.getProperty(axesID, __GO_Z_TIGHT_LIMITS__);
        assert retTightLimits.equals(initAxes.getZTightLimits());
    }
    @Test
    public void testReflectSetGetDataBounds() {
        controller.setProperty(axesID, __GO_DATA_BOUNDS__, initAxes.getDataBounds());
        Double[] retDataBounds = (Double[]) controller.getProperty(axesID, __GO_DATA_BOUNDS__);
        assert Arrays.equals(retDataBounds, initAxes.getDataBounds());
    }
    @Test
    public void testReflectSetGetRealDataBounds() {
        controller.setProperty(axesID, __GO_REAL_DATA_BOUNDS__, initAxes.getRealDataBounds());
        Double[] retRealDataBounds = (Double[]) controller.getProperty(axesID, __GO_REAL_DATA_BOUNDS__);
        assert Arrays.equals(retRealDataBounds, initAxes.getRealDataBounds());
    }
    @Test
    public void testReflectSetGetZoomEnabled() {
        controller.setProperty(axesID, __GO_ZOOM_ENABLED__, initAxes.getZoomEnabled());
        Boolean retZoomEnabled = (Boolean) controller.getProperty(axesID, __GO_ZOOM_ENABLED__);
        assert retZoomEnabled.equals(initAxes.getZoomEnabled());
    }
    @Test
    public void testReflectSetGetZoomBox() {
        controller.setProperty(axesID, __GO_ZOOM_BOX__, initAxes.getZoomBox());
        Double[] retZoomBox = (Double[]) controller.getProperty(axesID, __GO_ZOOM_BOX__);
        assert Arrays.equals(retZoomBox, initAxes.getZoomBox());
    }
    @Test
    public void testReflectSetGetAutoScale() {
        controller.setProperty(axesID, __GO_AUTO_SCALE__, initAxes.getAutoScale());
        Boolean retAutoScale = (Boolean) controller.getProperty(axesID, __GO_AUTO_SCALE__);
        assert retAutoScale.equals(initAxes.getAutoScale());
    }
    @Test
    public void testReflectSetGetFirstPlot() {
        controller.setProperty(axesID, __GO_FIRST_PLOT__, initAxes.getFirstPlot());
        Boolean retFirstPlot = (Boolean) controller.getProperty(axesID, __GO_FIRST_PLOT__);
        assert retFirstPlot.equals(initAxes.getFirstPlot());
    }

    /* Default properties */
    @Test
    public void testReflectSetGetHiddenColor() {
        controller.setProperty(axesID, __GO_HIDDEN_COLOR__, initAxes.getHiddenColor());
        Integer retHiddenColor = (Integer) controller.getProperty(axesID, __GO_HIDDEN_COLOR__);
        assert retHiddenColor.equals(initAxes.getHiddenColor());
    }
    @Test
    public void testReflectSetGetLineMode() {
        controller.setProperty(axesID, __GO_LINE_MODE__, initAxes.getLineMode());
        Boolean retLineMode = (Boolean) controller.getProperty(axesID, __GO_LINE_MODE__);
        assert retLineMode.equals(initAxes.getLineMode());
    }
    @Test
    public void testReflectSetGetLineStyle() {
        controller.setProperty(axesID, __GO_LINE_STYLE__, initAxes.getLineStyle());
        Integer retLineStyle = (Integer) controller.getProperty(axesID, __GO_LINE_STYLE__);
        assert retLineStyle.equals(initAxes.getLineStyle());
    }
    @Test
    public void testReflectSetGetLineThickness() {
        controller.setProperty(axesID, __GO_LINE_THICKNESS__, initAxes.getLineThickness());
        Double retLineThickness = (Double) controller.getProperty(axesID, __GO_LINE_THICKNESS__);
        assert retLineThickness.equals(initAxes.getLineThickness());
    }
    @Test
    public void testReflectSetGetLineColor() {
        controller.setProperty(axesID, __GO_LINE_COLOR__, initAxes.getLineColor());
        Integer retLineColor = (Integer) controller.getProperty(axesID, __GO_LINE_COLOR__);
        assert retLineColor.equals(initAxes.getLineColor());
    }
    @Test
    public void testReflectSetGetMarkMode() {
        controller.setProperty(axesID, __GO_MARK_MODE__, initAxes.getMarkMode());
        Boolean retMarkMode = (Boolean) controller.getProperty(axesID, __GO_MARK_MODE__);
        assert retMarkMode.equals(initAxes.getMarkMode());
    }
    @Test
    public void testReflectSetGetMarkStyle() {
        controller.setProperty(axesID, __GO_MARK_STYLE__, initAxes.getMarkStyle());
        Integer retMarkStyle = (Integer) controller.getProperty(axesID, __GO_MARK_STYLE__);
        assert retMarkStyle.equals(initAxes.getMarkStyle());
    }
    @Test
    public void testReflectSetGetMarkSize() {
        controller.setProperty(axesID, __GO_MARK_SIZE__, initAxes.getMarkSize());
        Integer retMarkSize = (Integer) controller.getProperty(axesID, __GO_MARK_SIZE__);
        assert retMarkSize.equals(initAxes.getMarkSize());
    }
    @Test
    public void testReflectSetGetMarkSizeUnit() {
        controller.setProperty(axesID, __GO_MARK_SIZE_UNIT__, initAxes.getMarkSizeUnit());
        Integer retMarkSizeUnit = (Integer) controller.getProperty(axesID, __GO_MARK_SIZE_UNIT__);
        assert retMarkSizeUnit.equals(initAxes.getMarkSizeUnit());
    }
    @Test
    public void testReflectSetGetMarkForeground() {
        controller.setProperty(axesID, __GO_MARK_FOREGROUND__, initAxes.getMarkForeground());
        Integer retMarkForeground = (Integer) controller.getProperty(axesID, __GO_MARK_FOREGROUND__);
        assert retMarkForeground.equals(initAxes.getMarkForeground());
    }
    @Test
    public void testReflectSetGetMarkBackground() {
        controller.setProperty(axesID, __GO_MARK_BACKGROUND__, initAxes.getMarkBackground());
        Integer retMarkBackground = (Integer) controller.getProperty(axesID, __GO_MARK_BACKGROUND__);
        assert retMarkBackground.equals(initAxes.getMarkBackground());
    }
    @Test
    public void testReflectSetGetClipState() {
        controller.setProperty(axesID, __GO_CLIP_STATE__, initAxes.getClipState());
        Integer retClipState = (Integer) controller.getProperty(axesID, __GO_CLIP_STATE__);
        assert retClipState.equals(initAxes.getClipState());
    }
    @Test
    public void testReflectSetGetClipBox() {
        controller.setProperty(axesID, __GO_CLIP_BOX__, initAxes.getClipBox());
        Double[] retClipBox = (Double[]) controller.getProperty(axesID, __GO_CLIP_BOX__);
        assert Arrays.equals(retClipBox, initAxes.getClipBox());
    }
    @Test
    public void testReflectSetGetArcDrawingMethod() {
        controller.setProperty(axesID, __GO_ARC_DRAWING_METHOD__, initAxes.getArcDrawingMethod());
        Integer retArcDrawingMethod = (Integer) controller.getProperty(axesID, __GO_ARC_DRAWING_METHOD__);
        assert retArcDrawingMethod.equals(initAxes.getArcDrawingMethod());
    }

    /* Fast set/get methods tests */
    /* GraphicObject properties */

    @Test
    public void testFastSetGetParent() {
        controller.setProperty(axesID, __GO_PARENT__, figureID);
        Integer retParent = (Integer) controller.getProperty(axesID, __GO_PARENT__);
        assert retParent.equals(figureID);
    }
    @Test
    public void testFastSetGetAxesVisible() {
        controller.setProperty(axesID, __GO_VISIBLE__, initAxes.getVisible());
        Boolean axesVisible  = (Boolean) controller.getProperty(axesID, __GO_VISIBLE__);
        assert axesVisible.equals(initAxes.getVisible());
    }


    /* X-Axis properties */
    @Test
    public void testFastSetGetXAxisVisible() {
        controller.setProperty(axesID, __GO_X_AXIS_VISIBLE__, initAxes.getXAxisVisible());
        Boolean retAxisVisible = (Boolean) controller.getProperty(axesID, __GO_X_AXIS_VISIBLE__);
        assert (retAxisVisible.equals(initAxes.getXAxisVisible()));
    }
    @Test
    public void testFastSetGetXAxisReverse() {
        controller.setProperty(axesID, __GO_X_AXIS_REVERSE__, initAxes.getXAxisReverse());
        Boolean retAxisReverse = (Boolean) controller.getProperty(axesID, __GO_X_AXIS_REVERSE__);
        assert (retAxisReverse.equals(initAxes.getXAxisReverse()));
    }
    @Test
    public void testFastSetGetXAxisGridColor() {
        controller.setProperty(axesID, __GO_X_AXIS_GRID_COLOR__, initAxes.getXAxisGridColor());
        Integer retGridColor = (Integer) controller.getProperty(axesID, __GO_X_AXIS_GRID_COLOR__);
        assert retGridColor.equals(initAxes.getXAxisGridColor());
    }
    @Test
    public void testFastSetGetXAxisLabel() {
        controller.setProperty(axesID, __GO_X_AXIS_LABEL__, xlabelID);
        Integer retXlabel = (Integer) controller.getProperty(axesID, __GO_X_AXIS_LABEL__);
        assert retXlabel.equals(xlabelID);
    }
    @Test
    public void testFastSetGetXAxisLocation() {
        controller.setProperty(axesID, __GO_X_AXIS_LOCATION__, initAxes.getXAxisLocation());
        Integer retAxisLocation = (Integer) controller.getProperty(axesID, __GO_X_AXIS_LOCATION__);
        assert retAxisLocation.equals(initAxes.getXAxisLocation());
    }
    @Test
    public void testFastSetGetXAxisLogFlag() {
        controller.setProperty(axesID, __GO_X_AXIS_LOG_FLAG__, initAxes.getXAxisLogFlag());
        Boolean retLogFlag = (Boolean) controller.getProperty(axesID, __GO_X_AXIS_LOG_FLAG__);
        assert retLogFlag.equals(initAxes.getXAxisLogFlag());
    }
    @Test
    public void testFastSetGetXAxisAutoTicks() {
        controller.setProperty(axesID, __GO_X_AXIS_AUTO_TICKS__, initAxes.getXAxisAutoTicks());
        Boolean retAutoTicks = (Boolean) controller.getProperty(axesID, __GO_X_AXIS_AUTO_TICKS__);
        assert retAutoTicks.equals(initAxes.getXAxisAutoTicks());
    }
    @Test
    public void testFastSetGetXAxisSubTicks() {
        controller.setProperty(axesID, __GO_X_AXIS_SUBTICKS__, initAxes.getXAxisSubticks());
        Integer retSubTicks = (Integer) controller.getProperty(axesID, __GO_X_AXIS_SUBTICKS__);
        assert retSubTicks.equals(initAxes.getXAxisSubticks());
    }
    @Test
    public void testFastSetGetXAxisTicksLocations() {
        controller.setProperty(axesID, __GO_X_AXIS_TICKS_LOCATIONS__, initAxes.getXAxisTicksLocations());
        Double[] retTicksLocations = (Double[]) controller.getProperty(axesID, __GO_X_AXIS_TICKS_LOCATIONS__);
        assert Arrays.equals(retTicksLocations, initAxes.getXAxisTicksLocations());
    }
    @Test
    public void testFastSetGetXAxisTicksLabels() {
        controller.setProperty(axesID, __GO_X_AXIS_TICKS_LABELS__, initAxes.getXAxisTicksLabels());
        String[] retTicksLabels = (String[]) controller.getProperty(axesID, __GO_X_AXIS_TICKS_LABELS__);
        assert Arrays.equals(retTicksLabels, initAxes.getXAxisTicksLabels());
    }

    /* Y-Axis properties */
    @Test
    public void testFastSetGetYAxisVisible() {
        controller.setProperty(axesID, __GO_Y_AXIS_VISIBLE__, initAxes.getYAxisVisible());
        Boolean retAxisVisible = (Boolean) controller.getProperty(axesID, __GO_Y_AXIS_VISIBLE__);
        assert (retAxisVisible.equals(initAxes.getYAxisVisible()));
    }
    @Test
    public void testFastSetGetYAxisReverse() {
        controller.setProperty(axesID, __GO_Y_AXIS_REVERSE__, initAxes.getYAxisReverse());
        Boolean retAxisReverse = (Boolean) controller.getProperty(axesID, __GO_Y_AXIS_REVERSE__);
        assert (retAxisReverse.equals(initAxes.getYAxisReverse()));
    }
    @Test
    public void testFastSetGetYAxisGridColor() {
        controller.setProperty(axesID, __GO_Y_AXIS_GRID_COLOR__, initAxes.getYAxisGridColor());
        Integer retGridColor = (Integer) controller.getProperty(axesID, __GO_Y_AXIS_GRID_COLOR__);
        assert retGridColor.equals(initAxes.getYAxisGridColor());
    }
    @Test
    public void testFastSetGetYAxisLabel() {
        controller.setProperty(axesID, __GO_Y_AXIS_LABEL__, ylabelID);
        Integer retlabel = (Integer) controller.getProperty(axesID, __GO_Y_AXIS_LABEL__);
        assert retlabel.equals(ylabelID);
    }
    @Test
    public void testFastSetGetYAxisLocation() {
        controller.setProperty(axesID, __GO_Y_AXIS_LOCATION__, initAxes.getYAxisLocation());
        Integer retAxisLocation = (Integer) controller.getProperty(axesID, __GO_Y_AXIS_LOCATION__);
        assert retAxisLocation.equals(initAxes.getYAxisLocation());
    }
    @Test
    public void testFastSetGetYAxisLogFlag() {
        controller.setProperty(axesID, __GO_Y_AXIS_LOG_FLAG__, initAxes.getYAxisLogFlag());
        Boolean retLogFlag = (Boolean) controller.getProperty(axesID, __GO_Y_AXIS_LOG_FLAG__);
        assert retLogFlag.equals(initAxes.getYAxisLogFlag());
    }
    @Test
    public void testFastSetGetYAxisAutoTicks() {
        controller.setProperty(axesID, __GO_Y_AXIS_AUTO_TICKS__, initAxes.getYAxisAutoTicks());
        Boolean retAutoTicks = (Boolean) controller.getProperty(axesID, __GO_Y_AXIS_AUTO_TICKS__);
        assert retAutoTicks.equals(initAxes.getYAxisAutoTicks());
    }
    @Test
    public void testFastSetGetYAxisSubTicks() {
        controller.setProperty(axesID, __GO_Y_AXIS_SUBTICKS__, initAxes.getYAxisSubticks());
        Integer retSubTicks = (Integer) controller.getProperty(axesID, __GO_Y_AXIS_SUBTICKS__);
        assert retSubTicks.equals(initAxes.getYAxisSubticks());
    }
    @Test
    public void testFastSetGetYAxisTicksLocations() {
        controller.setProperty(axesID, __GO_Y_AXIS_TICKS_LOCATIONS__, initAxes.getYAxisTicksLocations());
        Double[] retTicksLocations = (Double[]) controller.getProperty(axesID, __GO_Y_AXIS_TICKS_LOCATIONS__);
        assert Arrays.equals(retTicksLocations, initAxes.getYAxisTicksLocations());
    }
    @Test
    public void testFastSetGetYAxisTicksLabels() {
        controller.setProperty(axesID, __GO_Y_AXIS_TICKS_LABELS__, initAxes.getYAxisTicksLabels());
        String[] retTicksLabels = (String[]) controller.getProperty(axesID, __GO_Y_AXIS_TICKS_LABELS__);
        assert Arrays.equals(retTicksLabels, initAxes.getYAxisTicksLabels());
    }

    /* Z-Axis properties */
    @Test
    public void testFastSetGetZAxisVisible() {
        controller.setProperty(axesID, __GO_Z_AXIS_VISIBLE__, initAxes.getZAxisVisible());
        Boolean retAxisVisible = (Boolean) controller.getProperty(axesID, __GO_Z_AXIS_VISIBLE__);
        assert (retAxisVisible.equals(initAxes.getZAxisVisible()));
    }
    @Test
    public void testFastSetGetZAxisReverse() {
        controller.setProperty(axesID, __GO_Z_AXIS_REVERSE__, initAxes.getZAxisReverse());
        Boolean retAxisReverse = (Boolean) controller.getProperty(axesID, __GO_Z_AXIS_REVERSE__);
        assert (retAxisReverse.equals(initAxes.getZAxisReverse()));
    }
    @Test
    public void testFastSetGetZAxisGridColor() {
        controller.setProperty(axesID, __GO_Z_AXIS_GRID_COLOR__, initAxes.getZAxisGridColor());
        Integer retGridColor = (Integer) controller.getProperty(axesID, __GO_Z_AXIS_GRID_COLOR__);
        assert retGridColor.equals(initAxes.getZAxisGridColor());
    }
    @Test
    public void testFastSetGetZAxisLabel() {
        controller.setProperty(axesID, __GO_Z_AXIS_LABEL__, zlabelID);
        Integer retlabel = (Integer) controller.getProperty(axesID, __GO_Z_AXIS_LABEL__);
        assert retlabel.equals(zlabelID);
    }
    @Test
    public void testFastSetGetZAxisLocation() {
        controller.setProperty(axesID, __GO_Z_AXIS_LOCATION__, initAxes.getZAxisLocation());
        Integer retAxisLocation = (Integer) controller.getProperty(axesID, __GO_Z_AXIS_LOCATION__);
        assert retAxisLocation.equals(initAxes.getZAxisLocation());
    }
    @Test
    public void testFastSetGetZAxisLogFlag() {
        controller.setProperty(axesID, __GO_Z_AXIS_LOG_FLAG__, initAxes.getZAxisLogFlag());
        Boolean retLogFlag = (Boolean) controller.getProperty(axesID, __GO_Z_AXIS_LOG_FLAG__);
        assert retLogFlag.equals(initAxes.getZAxisLogFlag());
    }
    @Test
    public void testFastSetGetZAxisAutoTicks() {
        controller.setProperty(axesID, __GO_Z_AXIS_AUTO_TICKS__, initAxes.getZAxisAutoTicks());
        Boolean retAutoTicks = (Boolean) controller.getProperty(axesID, __GO_Z_AXIS_AUTO_TICKS__);
        assert retAutoTicks.equals(initAxes.getZAxisAutoTicks());
    }
    @Test
    public void testFastSetGetZAxisSubTicks() {
        controller.setProperty(axesID, __GO_Z_AXIS_SUBTICKS__, initAxes.getZAxisSubticks());
        Integer retSubTicks = (Integer) controller.getProperty(axesID, __GO_Z_AXIS_SUBTICKS__);
        assert retSubTicks.equals(initAxes.getZAxisSubticks());
    }
    @Test
    public void testFastSetGetZAxisTicksLocations() {
        controller.setProperty(axesID, __GO_Z_AXIS_TICKS_LOCATIONS__, initAxes.getZAxisTicksLocations());
        Double[] retTicksLocations = (Double[]) controller.getProperty(axesID, __GO_Z_AXIS_TICKS_LOCATIONS__);
        assert Arrays.equals(retTicksLocations, initAxes.getZAxisTicksLocations());
    }
    @Test
    public void testFastSetGetZAxisTicksLabels() {
        controller.setProperty(axesID, __GO_Z_AXIS_TICKS_LABELS__, initAxes.getZAxisTicksLabels());
        String[] retTicksLabels = (String[]) controller.getProperty(axesID, __GO_Z_AXIS_TICKS_LABELS__);
        assert Arrays.equals(retTicksLabels, initAxes.getZAxisTicksLabels());
    }

    /* Ticks property shared by the three axes */
    @Test
    public void testFastSetGetAutoSubticks() {
        controller.setProperty(axesID, __GO_AUTO_SUBTICKS__, initAxes.getAutoSubticks());
        Boolean retAutoSubticks = (Boolean) controller.getProperty(axesID, __GO_AUTO_SUBTICKS__);
        assert retAutoSubticks.equals(initAxes.getAutoSubticks());
    }

    /* Miscellaneous Axes properties */
    @Test
    public void testFastSetGetGridPosition() {
        controller.setProperty(axesID, __GO_GRID_POSITION__, initAxes.getGridPosition());
        Integer retGridPos = (Integer) controller.getProperty(axesID, __GO_GRID_POSITION__);
        assert retGridPos.equals(initAxes.getGridPosition());
    }
    @Test
    public void testFastSetGetTitle() {
        controller.setProperty(axesID, __GO_TITLE__, titleID);
        Integer retTitle = (Integer) controller.getProperty(axesID, __GO_TITLE__);
        assert retTitle.equals(titleID);
    }
    @Test
    public void testFastSetGetAutoClear() {
        controller.setProperty(axesID, __GO_AUTO_CLEAR__, initAxes.getAutoClear());
        Boolean retAutoClear = (Boolean) controller.getProperty(axesID, __GO_AUTO_CLEAR__);
        assert retAutoClear.equals(initAxes.getAutoClear());
    }
    @Test
    public void testFastSetGetFilled() {
        controller.setProperty(axesID, __GO_FILLED__, initAxes.getFilled());
        Boolean retFilled = (Boolean) controller.getProperty(axesID, __GO_FILLED__);
        assert retFilled.equals(initAxes.getFilled());
    }
    @Test
    public void testFastSetGetBackground() {
        controller.setProperty(axesID, __GO_BACKGROUND__, initAxes.getBackground());
        Integer retBackground = (Integer) controller.getProperty(axesID, __GO_BACKGROUND__);
        assert retBackground.equals(initAxes.getBackground());
    }
    @Test
    public void testFastSetGetMargins() {
        controller.setProperty(axesID, __GO_MARGINS__, initAxes.getMargins());
        Double[] retMargins = (Double[]) controller.getProperty(axesID, __GO_MARGINS__);
        assert Arrays.equals(retMargins, initAxes.getMargins());
    }
    @Test
    public void testFastSetGetAxesBounds() {
        controller.setProperty(axesID, __GO_AXES_BOUNDS__, initAxes.getAxesBounds());
        Double[] retAxesBounds = (Double[]) controller.getProperty(axesID, __GO_AXES_BOUNDS__);
        assert Arrays.equals(retAxesBounds, initAxes.getAxesBounds());
    }

    /* Camera properties */
    @Test
    public void testFastSetGetView() {
        controller.setProperty(axesID, __GO_VIEW__, initAxes.getView());
        Integer retView = (Integer) controller.getProperty(axesID, __GO_VIEW__);
        assert retView.equals(initAxes.getView());
    }
    @Test
    public void testFastSetGetIsoview() {
        controller.setProperty(axesID, __GO_ISOVIEW__, initAxes.getIsoview());
        Boolean retIsoview = (Boolean) controller.getProperty(axesID, __GO_ISOVIEW__);
        assert retIsoview.equals(initAxes.getIsoview());
    }
    @Test
    public void testFastSetGetCubeScaling() {
        controller.setProperty(axesID, __GO_CUBE_SCALING__, initAxes.getCubeScaling());
        Boolean retCubeScaling = (Boolean) controller.getProperty(axesID, __GO_CUBE_SCALING__);
        assert retCubeScaling.equals(initAxes.getCubeScaling());
    }
    @Test
    public void testFastSetGetRotationAngles() {
        controller.setProperty(axesID, __GO_ROTATION_ANGLES__, initAxes.getRotationAngles());
        Double[] retRotationAngles = (Double[]) controller.getProperty(axesID, __GO_ROTATION_ANGLES__);
        assert (Arrays.equals(retRotationAngles, initAxes.getRotationAngles()) && retRotationAngles.length == 2);
    }

    /* Box properties */
    @Test
    public void testFastSetGetBoxType() {
        controller.setProperty(axesID, __GO_BOX_TYPE__, initAxes.getBoxType());
        Integer retBoxType = (Integer) controller.getProperty(axesID, __GO_BOX_TYPE__);
        assert retBoxType.equals(initAxes.getBoxType());
    }
    @Test
    public void testFastSetGetHiddenAxisColor() {
        controller.setProperty(axesID, __GO_HIDDEN_AXIS_COLOR__, initAxes.getHiddenAxisColor());
        Integer retHiddenAxisColor = (Integer) controller.getProperty(axesID, __GO_HIDDEN_AXIS_COLOR__);
        assert retHiddenAxisColor.equals(initAxes.getHiddenAxisColor());
    }
    @Test
    public void testFastSetGetXTightLimits() {
        controller.setProperty(axesID, __GO_X_TIGHT_LIMITS__, initAxes.getXTightLimits());
        Boolean retTightLimits = (Boolean) controller.getProperty(axesID, __GO_X_TIGHT_LIMITS__);
        assert retTightLimits.equals(initAxes.getXTightLimits());
    }
    @Test
    public void testFastSetGetYTightLimits() {
        controller.setProperty(axesID, __GO_Y_TIGHT_LIMITS__, initAxes.getYTightLimits());
        Boolean retTightLimits = (Boolean) controller.getProperty(axesID, __GO_Y_TIGHT_LIMITS__);
        assert retTightLimits.equals(initAxes.getYTightLimits());
    }
    @Test
    public void testFastSetGetZTightLimits() {
        controller.setProperty(axesID, __GO_Z_TIGHT_LIMITS__, initAxes.getZTightLimits());
        Boolean retTightLimits = (Boolean) controller.getProperty(axesID, __GO_Z_TIGHT_LIMITS__);
        assert retTightLimits.equals(initAxes.getZTightLimits());
    }
    @Test
    public void testFastSetGetDataBounds() {
        controller.setProperty(axesID, __GO_DATA_BOUNDS__, initAxes.getDataBounds());
        Double[] retDataBounds = (Double[]) controller.getProperty(axesID, __GO_DATA_BOUNDS__);
        assert Arrays.equals(retDataBounds, initAxes.getDataBounds());
    }
    @Test
    public void testFastSetGetRealDataBounds() {
        controller.setProperty(axesID, __GO_REAL_DATA_BOUNDS__, initAxes.getRealDataBounds());
        Double[] retRealDataBounds = (Double[]) controller.getProperty(axesID, __GO_REAL_DATA_BOUNDS__);
        assert Arrays.equals(retRealDataBounds, initAxes.getRealDataBounds());
    }
    @Test
    public void testFastSetGetZoomEnabled() {
        controller.setProperty(axesID, __GO_ZOOM_ENABLED__, initAxes.getZoomEnabled());
        Boolean retZoomEnabled = (Boolean) controller.getProperty(axesID, __GO_ZOOM_ENABLED__);
        assert retZoomEnabled.equals(initAxes.getZoomEnabled());
    }
    @Test
    public void testFastSetGetZoomBox() {
        controller.setProperty(axesID, __GO_ZOOM_BOX__, initAxes.getZoomBox());
        Double[] retZoomBox = (Double[]) controller.getProperty(axesID, __GO_ZOOM_BOX__);
        assert Arrays.equals(retZoomBox, initAxes.getZoomBox());
    }
    @Test
    public void testFastSetGetAutoScale() {
        controller.setProperty(axesID, __GO_AUTO_SCALE__, initAxes.getAutoScale());
        Boolean retAutoScale = (Boolean) controller.getProperty(axesID, __GO_AUTO_SCALE__);
        assert retAutoScale.equals(initAxes.getAutoScale());
    }
    @Test
    public void testFastSetGetFirstPlot() {
        controller.setProperty(axesID, __GO_FIRST_PLOT__, initAxes.getFirstPlot());
        Boolean retFirstPlot = (Boolean) controller.getProperty(axesID, __GO_FIRST_PLOT__);
        assert retFirstPlot.equals(initAxes.getFirstPlot());
    }

    /* Default properties */
    @Test
    public void testFastSetGetHiddenColor() {
        controller.setProperty(axesID, __GO_HIDDEN_COLOR__, initAxes.getHiddenColor());
        Integer retHiddenColor = (Integer) controller.getProperty(axesID, __GO_HIDDEN_COLOR__);
        assert retHiddenColor.equals(initAxes.getHiddenColor());
    }
    @Test
    public void testFastSetGetLineMode() {
        controller.setProperty(axesID, __GO_LINE_MODE__, initAxes.getLineMode());
        Boolean retLineMode = (Boolean) controller.getProperty(axesID, __GO_LINE_MODE__);
        assert retLineMode.equals(initAxes.getLineMode());
    }
    @Test
    public void testFastSetGetLineStyle() {
        controller.setProperty(axesID, __GO_LINE_STYLE__, initAxes.getLineStyle());
        Integer retLineStyle = (Integer) controller.getProperty(axesID, __GO_LINE_STYLE__);
        assert retLineStyle.equals(initAxes.getLineStyle());
    }
    @Test
    public void testFastSetGetLineThickness() {
        controller.setProperty(axesID, __GO_LINE_THICKNESS__, initAxes.getLineThickness());
        Double retLineThickness = (Double) controller.getProperty(axesID, __GO_LINE_THICKNESS__);
        assert retLineThickness.equals(initAxes.getLineThickness());
    }
    @Test
    public void testFastSetGetLineColor() {
        controller.setProperty(axesID, __GO_LINE_COLOR__, initAxes.getLineColor());
        Integer retLineColor = (Integer) controller.getProperty(axesID, __GO_LINE_COLOR__);
        assert retLineColor.equals(initAxes.getLineColor());
    }
    @Test
    public void testFastSetGetMarkMode() {
        controller.setProperty(axesID, __GO_MARK_MODE__, initAxes.getMarkMode());
        Boolean retMarkMode = (Boolean) controller.getProperty(axesID, __GO_MARK_MODE__);
        assert retMarkMode.equals(initAxes.getMarkMode());
    }
    @Test
    public void testFastSetGetMarkStyle() {
        controller.setProperty(axesID, __GO_MARK_STYLE__, initAxes.getMarkStyle());
        Integer retMarkStyle = (Integer) controller.getProperty(axesID, __GO_MARK_STYLE__);
        assert retMarkStyle.equals(initAxes.getMarkStyle());
    }
    @Test
    public void testFastSetGetMarkSize() {
        controller.setProperty(axesID, __GO_MARK_SIZE__, initAxes.getMarkSize());
        Integer retMarkSize = (Integer) controller.getProperty(axesID, __GO_MARK_SIZE__);
        assert retMarkSize.equals(initAxes.getMarkSize());
    }
    @Test
    public void testFastSetGetMarkSizeUnit() {
        controller.setProperty(axesID, __GO_MARK_SIZE_UNIT__, initAxes.getMarkSizeUnit());
        Integer retMarkSizeUnit = (Integer) controller.getProperty(axesID, __GO_MARK_SIZE_UNIT__);
        assert retMarkSizeUnit.equals(initAxes.getMarkSizeUnit());
    }
    @Test
    public void testFastSetGetMarkForeground() {
        controller.setProperty(axesID, __GO_MARK_FOREGROUND__, initAxes.getMarkForeground());
        Integer retMarkForeground = (Integer) controller.getProperty(axesID, __GO_MARK_FOREGROUND__);
        assert retMarkForeground.equals(initAxes.getMarkForeground());
    }
    @Test
    public void testFastSetGetMarkBackground() {
        controller.setProperty(axesID, __GO_MARK_BACKGROUND__, initAxes.getMarkBackground());
        Integer retMarkBackground = (Integer) controller.getProperty(axesID, __GO_MARK_BACKGROUND__);
        assert retMarkBackground.equals(initAxes.getMarkBackground());
    }
    @Test
    public void testFastSetGetClipState() {
        controller.setProperty(axesID, __GO_CLIP_STATE__, initAxes.getClipState());
        Integer retClipState = (Integer) controller.getProperty(axesID, __GO_CLIP_STATE__);
        assert retClipState.equals(initAxes.getClipState());
    }
    @Test
    public void testFastSetGetClipBox() {
        controller.setProperty(axesID, __GO_CLIP_BOX__, initAxes.getClipBox());
        Double[] retClipBox = (Double[]) controller.getProperty(axesID, __GO_CLIP_BOX__);
        assert Arrays.equals(retClipBox, initAxes.getClipBox());
    }
    @Test
    public void testFastSetGetArcDrawingMethod() {
        controller.setProperty(axesID, __GO_ARC_DRAWING_METHOD__, initAxes.getArcDrawingMethod());
        Integer retArcDrawingMethod = (Integer) controller.getProperty(axesID, __GO_ARC_DRAWING_METHOD__);
        assert retArcDrawingMethod.equals(initAxes.getArcDrawingMethod());
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
