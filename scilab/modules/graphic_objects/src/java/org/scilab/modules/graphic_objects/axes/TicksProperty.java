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

package org.scilab.modules.graphic_objects.axes;

import java.util.ArrayList;

import org.scilab.modules.graphic_objects.textObject.Font;
import org.scilab.modules.graphic_objects.textObject.FormattedText;

/**
 * TicksProperty class
 * @author Manuel JULIACHS
 */
public class TicksProperty {
    /** TicksProperty properties names */
    public enum TicksPropertyProperty { AUTO, LOCATIONS, LABELS,
                                        FONT_SIZE, FONT_STYLE, FONT_COLOR, SUBTICKS
                                      };

    /** Default number of ticks */
    private static final int DEFAULT_NUMBER_OF_TICKS = 11;

    /** Specifies whether ticks are automatically computed or not */
    private boolean auto;

    /** Number of subticks between two main ticks */
    private int subticks;

    /** Default font */
    private Font defaultFont;

    /** TicksArrays class */
    private class TicksArrays {
        /** Ticks locations */
        private double[] locations;

        /** Ticks labels */
        private ArrayList <FormattedText> labels;

        /** Number of ticks */
        private int number;

        /**
         * Constructor
         */
        public TicksArrays(int number) {
            locations = new double[number];
            labels = new ArrayList<FormattedText>(number);

            for (int i = 0; i < number; i++) {
                labels.add(i, new FormattedText());
            }

            this.number = number;
        }

        /**
         * @return the number of ticks
         */
        public Integer getNumber() {
            return number;
        }

        /**
         * @return the labels
         */
        public ArrayList<FormattedText> getLabels() {
            return labels;
        }

        /**
         * @param labels the labels to set
         */
        public void setLabels(ArrayList<FormattedText> labels) {
            if (!this.labels.isEmpty()) {
                this.labels.clear();
            }

            for (int i = 0; i < labels.size(); i++) {
                this.labels.add(i, new FormattedText(labels.get(i)));
            }
        }

        /**
         * @return the labels strings
         */
        public String[] getLabelsStrings() {
            String[] labelsStrings;

            labelsStrings = new String[number];

            for (int i = 0; i < number; i++) {
                labelsStrings[i] = new String(labels.get(i).getText());
            }

            return labelsStrings;
        }

        /**
         * Sets the ticks labels strings
         * Requires the corresponding ticks locations to have previously been set.
         * @param labels the labels to set
         */
        public void setLabelsStrings(String[] labels) {
            if (labels.length != number) {
                return;
            }

            if (this.labels == null || this.labels.size() != labels.length) {
                this.labels = new ArrayList<FormattedText>(0);

                for (int i = 0; i < labels.length; i++) {
                    FormattedText newText = new FormattedText();

                    newText.setFont(new Font(defaultFont));
                    this.labels.add(i, newText);
                }
            }

            for (int i = 0; i < number; i++) {
                this.labels.get(i).setText(labels[i]);
            }
        }

        /**
         * @return the locations
         */
        public Double[] getLocations() {
            Double[] retLocations;

            retLocations = new Double[number];

            for (int i = 0; i < number; i++) {
                retLocations[i] = locations[i];
            }

            return retLocations;
        }

        /**
         * Sets the ticks locations
         * Also sets the current number of ticks to the size of the locations array
         * if the latter is resized.
         * @param locations the locations to set
         */
        public void setLocations(Double[] locations) {
            if (this.locations == null || number != locations.length) {
                this.locations = new double[locations.length];
                number = locations.length;
            }

            for (int i = 0; i < locations.length; i++) {
                this.locations[i] = locations[i];
            }
        }

        /**
         * Supposes all ticks labels have the same font style.
         * To be corrected.
         * @return the ticks labels font style
         */
        public Integer getFontStyle() {
            if (!labels.isEmpty()) {
                return labels.get(0).getFont().getStyle();
            }
            return 0;
        }

        /**
         * Supposes all ticks labels have the same font style.
         * To be corrected.
         * @param fontStyle the ticks labels font style to set
         */
        public void setFontStyle(Integer fontStyle) {
            for (int i = 0; i < labels.size(); i++) {
                labels.get(i).getFont().setStyle(fontStyle);
            }
        }

        /**
         * Supposes all ticks labels have the same font size.
         * To be corrected.
         * @return the ticks labels font size
         */
        public Double getFontSize() {
            if (!labels.isEmpty()) {
                return labels.get(0).getFont().getSize();
            }
            return 0.0;
        }

        /**
         * Supposes all ticks labels have the same font size.
         * To be corrected.
         * @param fontSize the ticks labels font size to set
         */
        public void setFontSize(Double fontSize) {
            for (int i = 0; i < labels.size(); i++) {
                labels.get(i).getFont().setSize(fontSize);
            }
        }

        /**
         * Supposes all ticks labels have the same font color.
         * To be corrected.
         * @return the ticks labels font color
         */
        public Integer getFontColor() {
            if (!labels.isEmpty()) {
                return labels.get(0).getFont().getColor();
            }
            return 0;
        }

        /**
         * Supposes all ticks labels have the same font color.
         * To be corrected.
         * @param fontColor the ticks labels font color to set
         */
        public void setFontColor(Integer fontColor) {
            for (int i = 0; i < labels.size(); i++) {
                labels.get(i).getFont().setColor(fontColor);
            }
        }

        /**
         * Supposes all ticks labels have the same font fractional.
         * To be corrected.
         * @return the ticks labels font fractional
         */
        public Boolean getFontFractional() {
            if (!labels.isEmpty()) {
                return labels.get(0).getFont().getFractional();
            }
            return false;
        }

        /**
         * Supposes all ticks labels have the same font fractional.
         * To be corrected.
         * @param fontFractional the ticks labels font fractional to set
         */
        public void setFontFractional(Boolean fontFractional) {
            for (int i = 0; i < labels.size(); i++) {
                labels.get(i).getFont().setFractional(fontFractional);
            }
        }


    }

    /** Automatic ticks */
    TicksArrays automaticTicks;

    /** User ticks */
    TicksArrays userTicks;

    /** Constructor */
    public TicksProperty() {
        auto = false;

        subticks = 0;

        defaultFont = new Font();

        automaticTicks = new TicksArrays(DEFAULT_NUMBER_OF_TICKS);
        userTicks = new TicksArrays(0);
    }

    /**
     * Copy constructor
     * @param ticksProperty the TicksProperty to copy
     */
    public TicksProperty(TicksProperty ticksProperty) {
        auto = ticksProperty.auto;

        subticks = ticksProperty.subticks;

        defaultFont = new Font(ticksProperty.defaultFont);

        automaticTicks = new TicksArrays(0);
        userTicks = new TicksArrays(0);

        automaticTicks.setLocations(ticksProperty.automaticTicks.getLocations());
        userTicks.setLocations(ticksProperty.userTicks.getLocations());

        automaticTicks.setLabels(ticksProperty.automaticTicks.getLabels());
        userTicks.setLabels(ticksProperty.userTicks.getLabels());
    }

    /**
     * @return the auto
     */
    public Boolean getAuto() {
        return auto;
    }

    /**
     * @param auto the auto to set
     */
    public void setAuto(Boolean auto) {
        this.auto = auto;
    }

    /**
     * @return the labels
     */
    public ArrayList<FormattedText> getLabels() {
        if (auto) {
            return automaticTicks.getLabels();
        } else {
            return userTicks.getLabels();
        }
    }

    /**
     * @param labels the labels to set
     */
    public void setLabels(ArrayList<FormattedText> labels) {
        if (auto) {
            automaticTicks.setLabels(labels);
        } else {
            userTicks.setLabels(labels);
        }
    }

    /**
     * @return the labels strings
     */
    public String[] getLabelsStrings() {
        if (auto) {
            return automaticTicks.getLabelsStrings();
        } else  {
            return userTicks.getLabelsStrings();
        }
    }

    /**
     * Sets the ticks labels strings
     * Requires the corresponding ticks locations to have previously been set.
     * @param labels the labels to set
     */
    public void setLabelsStrings(String[] labels) {
        if (auto) {
            automaticTicks.setLabelsStrings(labels);
        } else {
            userTicks.setLabelsStrings(labels);
        }
    }

    /**
     * @return the number of ticks
     */
    public Integer getNumber() {
        if (auto) {
            return automaticTicks.getNumber();
        } else {
            return userTicks.getNumber();
        }
    }

    /**
     * @return the locations
     */
    public Double[] getLocations() {
        if (auto) {
            return automaticTicks.getLocations();
        } else {
            return userTicks.getLocations();
        }
    }

    /**
     * Sets the ticks locations
     * Also sets the current number of ticks to the size of the locations array
     * if the latter is resized.
     * @param locations the locations to set
     */
    public void setLocations(Double[] locations) {
        if (auto) {
            automaticTicks.setLocations(locations);
        } else {
            userTicks.setLocations(locations);
        }
    }

    /**
     * @return the subticks
     */
    public Integer getSubticks() {
        return subticks;
    }

    /**
     * @param subticks the subticks to set
     */
    public void setSubticks(Integer subticks) {
        this.subticks = subticks;
    }

    /**
     * Supposes that all automatic and user ticks labels have the same font style.
     * To be corrected (commented out block) when the associated C get function is completed.
     * @return the ticks labels font style
     */
    public Integer getFontStyle() {
        return automaticTicks.getFontStyle();

        /*
        if (auto) {
        	return automaticTicks.getFontStyle();
        } else {
        	return userTicks.getFontStyle();
        }
        */
    }

    /**
     * Supposes that all automatic and user ticks labels have the same font style.
     * To be corrected (commented out block) when the associated C set function is completed.
     * @param fontStyle the ticks labels font style to set
     */
    public void setFontStyle(Integer fontStyle) {
        defaultFont.setStyle(fontStyle);

        automaticTicks.setFontStyle(fontStyle);
        userTicks.setFontStyle(fontStyle);

        /*
        if (auto) {
        	automaticTicks.setFontStyle(fontStyle);
        } else {
        	userTicks.setFontStyle(fontStyle);
        }
        */
    }

    /**
     * Supposes that all automatic and user ticks labels have the same font size.
     * To be corrected (commented out block) when the associated C get function is completed.
     * @return the ticks labels font size
     */
    public Double getFontSize() {
        return automaticTicks.getFontSize();

        /*
        if (auto) {
        	return automaticTicks.getFontSize();
        } else {
        	return userTicks.getFontSize();
        }
        */
    }

    /**
     * Supposes that all automatic and user ticks labels have the same font size.
     * To be corrected (commented out block) when the associated C set function is completed.
     * @param fontSize the ticks labels font size to set
     */
    public void setFontSize(Double fontSize) {
        defaultFont.setSize(fontSize);

        automaticTicks.setFontSize(fontSize);
        userTicks.setFontSize(fontSize);

        /*
        if (auto) {
        	automaticTicks.setFontSize(fontSize);
        } else {
        	userTicks.setFontSize(fontSize);
        }
        */
    }

    /**
     * Supposes that all automatic and user ticks labels have the same font color.
     * To be corrected (commented out block) when the associated C get function is completed.
     * @return the ticks labels font color
     */
    public Integer getFontColor() {
        return automaticTicks.getFontColor();

        /*
        if (auto) {
        	return automaticTicks.getFontColor();
        } else {
        	return userTicks.getFontColor();
        }
        */
    }

    /**
     * Supposes that all automatic and user ticks labels have the same font color.
     * To be corrected (commented out block) when the associated C set function is completed.
     * @param fontColor the ticks labels font color to set
     */
    public void setFontColor(Integer fontColor) {
        defaultFont.setColor(fontColor);

        automaticTicks.setFontColor(fontColor);
        userTicks.setFontColor(fontColor);

        /*
        if (auto) {
        	automaticTicks.setFontColor(fontColor);
        } else {
        	userTicks.setFontColor(fontColor);
        }
        */
    }

    /**
     * Supposes all automatic and user ticks labels have the same font fractional.
     * To be corrected (commented out block) when the associated C get function is completed.
     * @return the ticks labels font fractional
     */
    public Boolean getFontFractional() {
        return automaticTicks.getFontFractional();

        /*
        if (auto) {
        	return automaticTicks.getFontFractional();
        } else {
        	return userTicks.getFontFractional();
        }
        */
    }

    /**
     * Supposes all automatic and user ticks labels have the same font fractional.
     * To be corrected (commented out block) when the associated C set function is completed.
     * @param fontFractional the ticks labels font fractional to set
     */
    public void setFontFractional(Boolean fontFractional) {
        defaultFont.setFractional(fontFractional);

        automaticTicks.setFontFractional(fontFractional);
        userTicks.setFontFractional(fontFractional);

        /*
        if (auto) {
        	automaticTicks.setFontFractional(fontFractional);
        } else {
        	userTicks.setFontFractional(fontFractional);
        }
        */
    }

}
