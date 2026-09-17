package org.scilab.modules.graphic_objects.uicontrol.edit;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SPINNER__;

import javax.swing.UIDefaults;
import javax.swing.UIManager;

import org.scilab.modules.graphic_objects.root.Root;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;

public class Spinner  extends Uicontrol {

    public Spinner() {
        super();
        setStyle(__GO_UI_SPINNER__);

        if (Root.getRoot().getUseDeprecatedLF()) {
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
