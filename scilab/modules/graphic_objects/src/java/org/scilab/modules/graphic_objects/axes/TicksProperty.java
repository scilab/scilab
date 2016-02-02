/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.modules.graphic_objects.axes;

import java.util.ArrayList;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;
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

    private String format = "";
    private Double[] st_factors = new Double[] {1., 0.};

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

        @Override
        public boolean equals(Object o) {
            if (o instanceof TicksArrays) {
                TicksArrays ta = (TicksArrays) o;
                if (ta.number == number) {
                    for (int i = 0; i < number; i++) {
                        if (ta.locations[i] != locations[i] || !ta.labels.get(i).equals(labels.get(i))) {
                            return false;
                        }
                    }

                    return true;
                }
            }

            return false;
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
        public UpdateStatus setLabels(ArrayList<FormattedText> labels) {
            UpdateStatus status = this.labels.equals(labels) ? UpdateStatus.NoChange : UpdateStatus.Success;
            if (status == UpdateStatus.Success) {
                if (!this.labels.isEmpty()) {
                    this.labels.clear();
                }

                for (int i = 0; i < labels.size(); i++) {
                    this.labels.add(i, new FormattedText(labels.get(i)));
                }
            }

            return status;
        }

        /**
         * @return the labels strings
         */
        public String[] getLabelsStrings() {
            String[] labelsStrings;

            labelsStrings = new String[number];

            for (int i = 0; i < number; i++) {
                labelsStrings[i] = new String(labels.get(i).getText()).replaceAll("\u00A0", " ");
            }

            return labelsStrings;
        }

        /**
         * Sets the ticks labels strings
         * Requires the corresponding ticks locations to have previously been set.
         * @param labels the labels to set
         */
        public UpdateStatus setLabelsStrings(String[] labels) {
            if (labels.length != number) {
                return UpdateStatus.NoChange;
            }

            if (this.labels == null || this.labels.size() != labels.length) {
                this.labels = new ArrayList<FormattedText>(0);

                Font font = new Font(defaultFont);
                for (int i = 0; i < labels.length; i++) {
                    FormattedText newText = new FormattedText(labels[i], font);
                    this.labels.add(newText);
                }

                return UpdateStatus.Success;
            }

            UpdateStatus status = UpdateStatus.NoChange;
            for (int i = 0; i < number; i++) {
                FormattedText ft = this.labels.get(i);
                if (!ft.getText().equals(labels[i])) {
                    this.labels.get(i).setText(labels[i]);
                    status = UpdateStatus.Success;
                }
            }

            return status;
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
        public UpdateStatus setLocations(Double[] locations) {
            UpdateStatus status = UpdateStatus.Success;
            if (this.locations == null || number != locations.length) {
                this.locations = new double[locations.length];
                number = locations.length;
            } else {
                status = UpdateStatus.NoChange;
            }

            for (int i = 0; i < locations.length; i++) {
                if (status == UpdateStatus.NoChange) {
                    if (this.locations[i] != locations[i]) {
                        status = UpdateStatus.Success;
                        this.locations[i] = locations[i];
                    }
                } else {
                    this.locations[i] = locations[i];
                }
            }

            return status;
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
        public UpdateStatus setFontStyle(Integer fontStyle) {
            UpdateStatus status = UpdateStatus.NoChange;
            for (int i = 0; i < labels.size(); i++) {
                Font f = labels.get(i).getFont();
                if (f.getStyle() != fontStyle) {
                    f.setStyle(fontStyle);
                    status = UpdateStatus.Success;
                }
            }

            return status;
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
        public UpdateStatus setFontSize(Double fontSize) {
            UpdateStatus status = UpdateStatus.NoChange;
            for (int i = 0; i < labels.size(); i++) {
                Font f = labels.get(i).getFont();
                if (f.getSize() != fontSize) {
                    f.setSize(fontSize);
                    status = UpdateStatus.Success;
                }
            }

            return status;
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
        public UpdateStatus setFontColor(Integer fontColor) {
            UpdateStatus status = UpdateStatus.NoChange;
            for (int i = 0; i < labels.size(); i++) {
                Font f = labels.get(i).getFont();
                if (!f.getColor().equals(fontColor)) {
                    f.setColor(fontColor);
                    status = UpdateStatus.Success;
                }
            }

            return status;
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
        public UpdateStatus setFontFractional(Boolean fontFractional) {
            UpdateStatus status = UpdateStatus.NoChange;
            for (int i = 0; i < labels.size(); i++) {
                Font f = labels.get(i).getFont();
                if (f.getFractional() != fontFractional) {
                    f.setFractional(fontFractional);
                    status = UpdateStatus.Success;
                }
            }

            return status;
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
        format = ticksProperty.format;
        st_factors = ticksProperty.st_factors;

        defaultFont = new Font(ticksProperty.defaultFont);

        automaticTicks = new TicksArrays(0);
        userTicks = new TicksArrays(0);

        automaticTicks.setLocations(ticksProperty.automaticTicks.getLocations());
        userTicks.setLocations(ticksProperty.userTicks.getLocations());

        automaticTicks.setLabels(ticksProperty.automaticTicks.getLabels());
        userTicks.setLabels(ticksProperty.userTicks.getLabels());
    }

    @Override
    public boolean equals(Object o) {
        if (o instanceof TicksProperty) {
            TicksProperty tp = (TicksProperty) o;
            if (tp.auto == auto && tp.subticks == subticks && tp.defaultFont.equals(defaultFont)) {
                if (auto) {
                    return automaticTicks.equals(tp.automaticTicks);
                } else {
                    return userTicks.equals(tp.userTicks);
                }
            }
        }

        return false;
    }

    /**
     * @return the format
     */
    public String getFormat() {
        return format;
    }

    /**
     * @param format the format to set
     */
    public UpdateStatus setFormat(String format) {
        if (!this.format.equals(format)) {
            this.format = format;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the format
     */
    public Double[] getSTFactors() {
        return st_factors;
    }

    /**
     * @param format the format to set
     */
    public UpdateStatus setSTFactors(Double[] factors) {
        if (!this.st_factors[0].equals(factors[0]) || !this.st_factors[1].equals(factors[1])) {
            this.st_factors = factors;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
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
    public UpdateStatus setAuto(Boolean auto) {
        if (this.auto != auto) {
            this.auto = auto;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
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
    public UpdateStatus setLabels(ArrayList<FormattedText> labels) {
        if (auto) {
            return automaticTicks.setLabels(labels);
        } else {
            return userTicks.setLabels(labels);
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
    public UpdateStatus setLabelsStrings(String[] labels) {
        if (auto) {
            return automaticTicks.setLabelsStrings(labels);
        } else {
            return userTicks.setLabelsStrings(labels);
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
    public UpdateStatus setLocations(Double[] locations) {
        if (auto) {
            return automaticTicks.setLocations(locations);
        } else {
            return userTicks.setLocations(locations);
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
    public UpdateStatus setSubticks(Integer subticks) {
        if (this.subticks != subticks) {
            this.subticks = subticks;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
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
    public UpdateStatus setFontStyle(Integer fontStyle) {
        UpdateStatus status = UpdateStatus.NoChange;
        if (fontStyle != defaultFont.getStyle()) {
            defaultFont.setStyle(fontStyle);
            status = UpdateStatus.Success;
        }

        UpdateStatus s1 = automaticTicks.setFontStyle(fontStyle);
        UpdateStatus s2 = userTicks.setFontStyle(fontStyle);

        if (status == UpdateStatus.Success || s1 == UpdateStatus.Success || s2 == UpdateStatus.Success) {
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;

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
    public UpdateStatus setFontSize(Double fontSize) {
        UpdateStatus status = UpdateStatus.NoChange;
        if (fontSize != defaultFont.getSize()) {
            defaultFont.setSize(fontSize);
            status = UpdateStatus.Success;
        }

        UpdateStatus s1 = automaticTicks.setFontSize(fontSize);
        UpdateStatus s2 = userTicks.setFontSize(fontSize);

        if (status == UpdateStatus.Success || s1 == UpdateStatus.Success || s2 == UpdateStatus.Success) {
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;

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
    public UpdateStatus setFontColor(Integer fontColor) {
        UpdateStatus status = UpdateStatus.NoChange;
        if (fontColor != defaultFont.getColor()) {
            defaultFont.setColor(fontColor);
            status = UpdateStatus.Success;
        }

        UpdateStatus s1 = automaticTicks.setFontColor(fontColor);
        UpdateStatus s2 = userTicks.setFontColor(fontColor);

        if (status == UpdateStatus.Success || s1 == UpdateStatus.Success || s2 == UpdateStatus.Success) {
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;

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
    public UpdateStatus setFontFractional(Boolean fontFractional) {
        UpdateStatus status = UpdateStatus.NoChange;
        if (fontFractional != defaultFont.getFractional()) {
            defaultFont.setFractional(fontFractional);
            status = UpdateStatus.Success;
        }

        UpdateStatus s1 = automaticTicks.setFontFractional(fontFractional);
        UpdateStatus s2 = userTicks.setFontFractional(fontFractional);

        if (status == UpdateStatus.Success || s1 == UpdateStatus.Success || s2 == UpdateStatus.Success) {
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;

        /*
          if (auto) {
          automaticTicks.setFontFractional(fontFractional);
          } else {
          userTicks.setFontFractional(fontFractional);
          }
        */
    }

    public Font getDefaultFont() {
        return defaultFont;
    }
}
