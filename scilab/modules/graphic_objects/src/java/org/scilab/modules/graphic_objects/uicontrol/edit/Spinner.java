package org.scilab.modules.graphic_objects.uicontrol.edit;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SPINNER__;

import javax.swing.UIDefaults;
import javax.swing.UIManager;

import org.scilab.modules.graphic_objects.console.Console;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;

public class Spinner  extends Uicontrol {

    public Spinner() {
        super();
        setStyle(__GO_UI_SPINNER__);

        if (Console.getConsole().getUseDeprecatedLF()) {
            setRelief(RELIEF_SUNKEN);
        } else {
            UIDefaults defaults = UIManager.getDefaults();

            //font
            setFont(defaults.getFont("Spinner.font"));

            //h-alignment
            setHorizontalAlignment("left");

            //v-alignement
            setVerticalAlignment("middle");
        }
    }
}
