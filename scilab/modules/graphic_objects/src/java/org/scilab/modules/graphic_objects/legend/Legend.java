/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2012 - DIGITEO - Manuel JULIACHS
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

package org.scilab.modules.graphic_objects.legend;

import java.util.ArrayList;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;
import org.scilab.modules.graphic_objects.textObject.ClippableTextObject;
import org.scilab.modules.graphic_objects.textObject.FormattedText;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Legend class
 * @author Manuel JULIACHS
 */
public class Legend extends ClippableTextObject {
    /** Legend properties names */
    private enum LegendProperty { LINKS, LINKSCOUNT, LEGENDLOCATION, POSITION , SIZE, MARKSCOUNT, LINEWIDTH};

    /** Legend location */
    public enum LegendLocation { IN_UPPER_RIGHT, IN_UPPER_LEFT, IN_LOWER_RIGHT, IN_LOWER_LEFT,
                                 OUT_UPPER_RIGHT, OUT_UPPER_LEFT, OUT_LOWER_RIGHT, OUT_LOWER_LEFT,
                                 UPPER_CAPTION, LOWER_CAPTION, BY_COORDINATES;

                                 /**
                                  * Converts an integer to the corresponding enum
                                  * @param intValue the integer value
                                  * @return the legend location enum
                                  */
    public static LegendLocation intToEnum(Integer intValue) {
        switch (intValue) {
            case 0:
                return LegendLocation.IN_UPPER_RIGHT;
            case 1:
                return LegendLocation.IN_UPPER_LEFT;
            case 2:
                return LegendLocation.IN_LOWER_RIGHT;
            case 3:
                return LegendLocation.IN_LOWER_LEFT;
            case 4:
                return LegendLocation.OUT_UPPER_RIGHT;
            case 5:
                return LegendLocation.OUT_UPPER_LEFT;
            case 6:
                return LegendLocation.OUT_LOWER_RIGHT;
            case 7:
                return LegendLocation.OUT_LOWER_LEFT;
            case 8:
                return LegendLocation.UPPER_CAPTION;
            case 9:
                return LegendLocation.LOWER_CAPTION;
            case 10:
                return LegendLocation.BY_COORDINATES;
            default:
                return null;
        }
    }
                               };

    /** List of the polylines referred to */
    private ArrayList <Integer> links;

    /** Legend location */
    private LegendLocation legendLocation;

    /** 2D position relative to the parent axes bounds */
    private double[] position;

    /** 2D size relative to the parent axes bounds */
    private double[] size;

    /** Number of marks to draw */
    private Integer marksCount;

    /** Line width in axes coordinates (percentage) */
    private Double lineWidth;

    /** Constructor */
    public Legend() {
        super();
        this.links = new ArrayList<Integer>(0);
        this.legendLocation = LegendLocation.LOWER_CAPTION;
        position = new double[2];
        size = new double[2];
        marksCount = 3;
        lineWidth = 0.1;
    }

    @Override
    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(int propertyName) {
        switch (propertyName) {
            case __GO_LINKS__ :
                return LegendProperty.LINKS;
            case __GO_LINKS_COUNT__ :
                return LegendProperty.LINKSCOUNT;
            case __GO_LEGEND_LOCATION__ :
                return LegendProperty.LEGENDLOCATION;
            case __GO_POSITION__ :
                return LegendProperty.POSITION;
            case __GO_SIZE__ :
                return LegendProperty.SIZE;
            case __GO_MARKS_COUNT__ :
                return LegendProperty.MARKSCOUNT;
            case __GO_LINE_WIDTH__ :
                return LegendProperty.LINEWIDTH;
            default :
                return super.getPropertyFromName(propertyName);
        }
    }

    /**
     * Fast property get method
     * @param property the property to get
     * @return the property value
     */
    public Object getProperty(Object property) {
        if (property == LegendProperty.LINKS) {
            return getValidLinks();
        } else if (property == LegendProperty.LINKSCOUNT) {
            return getValidLinksCount();
        } else if (property == LegendProperty.LEGENDLOCATION) {
            return getLegendLocation();
        } else if (property == LegendProperty.POSITION) {
            return getPosition();
        } else if (property == TextObjectProperty.TEXT_ARRAY_DIMENSIONS) {
            return getValidTextArrayDimensions();
        } else if (property == FormattedText.FormattedTextProperty.TEXT) {
            return getValidTextStrings();
        } else if (property == LegendProperty.SIZE) {
            return getSize();
        } else if (property == LegendProperty.MARKSCOUNT) {
            return getMarksCount();
        } else if (property == LegendProperty.LINEWIDTH) {
            return getLineWidth();
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
    public UpdateStatus setProperty(Object property, Object value) {
        if (property == LegendProperty.LINKS) {
            setLinks((Integer[]) value);
        } else if (property == LegendProperty.LEGENDLOCATION) {
            setLegendLocation((Integer) value);
        } else if (property == LegendProperty.POSITION) {
            setPosition((Double[]) value);
        } else if (property == LegendProperty.MARKSCOUNT) {
            setMarksCount((Integer) value);
        } else if (property == LegendProperty.LINEWIDTH) {
            setLineWidth((Double) value);
        } else {
            return super.setProperty(property, value);
        }

        return UpdateStatus.Success;
    }

    /**
     * @return the legendLocation
     */
    public Integer getLegendLocation() {
        return getLegendLocationAsEnum().ordinal();
    }

    /**
     * @return the legendLocation
     */
    public LegendLocation getLegendLocationAsEnum() {
        return legendLocation;
    }

    /**
     * @param legendLocation the legendLocation to set
     */
    public UpdateStatus setLegendLocation(Integer legendLocation) {
        setLegendLocationAsEnum(LegendLocation.intToEnum(legendLocation));
        return UpdateStatus.Success;
    }

    /**
     * @param legendLocation the legendLocation to set
     */
    public UpdateStatus setLegendLocationAsEnum(LegendLocation legendLocation) {
        this.legendLocation = legendLocation;
        return UpdateStatus.Success;
    }

    /**
     * @return the valid links
     */
    public Integer[] getValidLinks() {
        ArrayList <Integer> validLinks = new ArrayList<Integer>(0);

        for (int i = 0; i < links.size(); i++) {
            GraphicObject object = GraphicController.getController().getObjectFromId(links.get(i));

            if (object != null) {
                validLinks.add(links.get(i));
            }
        }

        return validLinks.toArray(new Integer[validLinks.size()]);
    }

    /**
     * @return the valid links count
     */
    public Integer getValidLinksCount() {
        int numValidLinks = 0;

        for (int i = 0; i < links.size(); i++) {
            GraphicObject object = GraphicController.getController().getObjectFromId(links.get(i));

            if (object != null) {
                numValidLinks++;
            }
        }

        return numValidLinks;
    }

    /**
     * @return the links
     */
    public Integer[] getLinks() {
        Integer[] retLinks = new Integer[links.size()];

        for (int i = 0; i < links.size(); i++) {
            retLinks[i] = links.get(i);
        }

        return retLinks;
    }

    /**
     * @return the links count
     */
    public Integer getLinksCount() {
        return links.size();
    }

    /**
     * @param links the links to set
     */
    public UpdateStatus setLinks(Integer[] links) {
        if (!this.links.isEmpty()) {
            this.links.clear();
        }

        for (int i = 0; i < links.length; i++) {
            this.links.add(links[i]);
        }
        return UpdateStatus.Success;
    }

    /**
     * @return the marks count
     */
    public Integer getMarksCount() {
        return marksCount;
    }

    /**
     * @param marksCount the marks count
     */
    public UpdateStatus setMarksCount(Integer marksCount) {
        if (!this.marksCount.equals(marksCount)) {
            this.marksCount = marksCount;

            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the line width
     */
    public Double getLineWidth() {
        return lineWidth;
    }

    /**
     * @param lineWidth the line width
     */
    public UpdateStatus setLineWidth(Double lineWidth) {
        if (!this.lineWidth.equals(lineWidth)) {
            this.lineWidth = lineWidth;

            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * Returns the dimensions of the text array, taking into account only valid links.
     * @return the dimensions of the text array
     */
    public Integer[] getValidTextArrayDimensions() {
        return new Integer[] {getValidLinksCount(), 1};
    }

    /**
     * Returns the array of valid text strings.
     * A string if considered valid if its corresponding link is also valid, otherwise it is ignored.
     * This is used when getting the Legend's TEXT_STRINGS property in order to return only the strings
     * corresponding to valid links (see also getValidLinks).
     * @return the valid text strings
     */
    public String[] getValidTextStrings() {
        ArrayList <String> validStrings = new ArrayList<String>(0);

        /* Text strings are stored in reverse order relative to links. */
        for (int i = 0; i < links.size(); i++) {
            GraphicObject object = GraphicController.getController().getObjectFromId(links.get(links.size() - i - 1));

            if (object != null) {
                validStrings.add(text[i].getText());
            }
        }

        return validStrings.toArray(new String[validStrings.size()]);
    }

    /**
     * @param links the links to set
     */
    public UpdateStatus setLinks(ArrayList<Integer> links) {
        this.links = links;
        return UpdateStatus.Success;
    }

    /**
     * @return the position
     */
    public Double[] getPosition() {
        Double[] retPosition = new Double[2];
        retPosition[0] = position[0];
        retPosition[1] = position[1];

        return retPosition;
    }

    /**
     * @param position the position to set
     */
    public UpdateStatus setPosition(Double[] position) {
        this.position[0] = position[0];
        this.position[1] = position[1];
        return UpdateStatus.Success;
    }

    /**
     * @return the size
     */
    public Double[] getSize() {
        Double[] retSize = new Double[2];
        retSize[0] = size[0];
        retSize[1] = size[1];

        return retSize;
    }

    /**
     * @param size the size to set
     */
    public UpdateStatus setSize(Double[] size) {
        this.size[0] = size[0];
        this.size[1] = size[1];
        return UpdateStatus.Success;
    }

    /**
     * @return Type as String
     */
    public Integer getType() {
        return GraphicObjectProperties.__GO_LEGEND__;
    }
}
