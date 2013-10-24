package org.scilab.modules.gui.widget;

public interface ViewMethods {
    void setBackgroundcolor(Double[] color);
    void setForegroundcolor(Double[] color);

    //call by update object
    void setString(String[] text);

    void setCallback(String callback);
    void setPosition(Double[] position);

    void setHorizontalalignment(String alignment);

    void setParent(int id);

    //Font
    void setFontweight(String value);
    void setFontname(String value);
    void setFontangle(String value);
    void setFontunits(double value);
    void setFontsize(double value);
    void setMax(double value);
    void setMin(double value);
    void setValue(Double[] value);

    void setRelief(String value);
    void setSliderstep(Double[] value);
    void setListboxtop(Integer[] value);
    void setEnable(boolean value);

    void setCallbacktype(int value);
    void setTooltipstring(String[] value);
}
