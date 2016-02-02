/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
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
package org.scilab.modules.xcos.preferences;

import java.util.Arrays;
import java.util.Map;
import java.util.Map.Entry;
import java.util.TreeMap;

import javax.swing.KeyStroke;

import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.graph.actions.CopyAction;
import org.scilab.modules.graph.actions.CutAction;
import org.scilab.modules.graph.actions.DeleteAction;
import org.scilab.modules.graph.actions.InvertSelectionAction;
import org.scilab.modules.graph.actions.PasteAction;
import org.scilab.modules.graph.actions.RedoAction;
import org.scilab.modules.graph.actions.SelectAllAction;
import org.scilab.modules.graph.actions.UndoAction;
import org.scilab.modules.graph.actions.ZoomInAction;
import org.scilab.modules.graph.actions.ZoomOutAction;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.actions.base.GraphActionManager;
import org.scilab.modules.xcos.actions.AboutXcosAction;
import org.scilab.modules.xcos.actions.CloseAction;
import org.scilab.modules.xcos.actions.CompileAction;
import org.scilab.modules.xcos.actions.DebugLevelAction;
import org.scilab.modules.xcos.actions.DiagramBackgroundAction;
import org.scilab.modules.xcos.actions.ExportAction;
import org.scilab.modules.xcos.actions.FitDiagramToViewAction;
import org.scilab.modules.xcos.actions.NewDiagramAction;
import org.scilab.modules.xcos.actions.NormalViewAction;
import org.scilab.modules.xcos.actions.OpenAction;
import org.scilab.modules.xcos.actions.OpenInSciAction;
import org.scilab.modules.xcos.actions.PrintAction;
import org.scilab.modules.xcos.actions.QuitAction;
import org.scilab.modules.xcos.actions.SaveAction;
import org.scilab.modules.xcos.actions.SaveAsAction;
import org.scilab.modules.xcos.actions.SetContextAction;
import org.scilab.modules.xcos.actions.SetupAction;
import org.scilab.modules.xcos.actions.ShowHideShadowAction;
import org.scilab.modules.xcos.actions.StartAction;
import org.scilab.modules.xcos.actions.StopAction;
import org.scilab.modules.xcos.actions.ViewDiagramBrowserAction;
import org.scilab.modules.xcos.actions.ViewViewportAction;
import org.scilab.modules.xcos.actions.XcosDemonstrationsAction;
import org.scilab.modules.xcos.actions.XcosDocumentationAction;
import org.scilab.modules.xcos.block.actions.BlockDocumentationAction;
import org.scilab.modules.xcos.block.actions.BlockParametersAction;
import org.scilab.modules.xcos.block.actions.BorderColorAction;
import org.scilab.modules.xcos.block.actions.CodeGenerationAction;
import org.scilab.modules.xcos.block.actions.FilledColorAction;
import org.scilab.modules.xcos.block.actions.FlipAction;
import org.scilab.modules.xcos.block.actions.MirrorAction;
import org.scilab.modules.xcos.block.actions.RegionToSuperblockAction;
import org.scilab.modules.xcos.block.actions.RotateAction;
import org.scilab.modules.xcos.block.actions.ViewDetailsAction;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionBottom;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionCenter;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionLeft;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionMiddle;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionRight;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionTop;
import org.scilab.modules.xcos.link.actions.StyleHorizontalAction;
import org.scilab.modules.xcos.link.actions.StyleStraightAction;
import org.scilab.modules.xcos.link.actions.StyleVerticalAction;
import org.scilab.modules.xcos.palette.actions.ViewPaletteBrowserAction;
import org.w3c.dom.Document;

@SuppressWarnings("unchecked")
public final class XcosKeyMap {
    protected static final String KEYMAP_XPATH = "//general/shortcuts/body/actions/action-folder[@xconf-uid=\"xcos\"]/action";
    private static Map<String, KeyStroke> actionKeys;

    private static String[] actionMapKeys;
    private static Class <? extends DefaultAction > [] actionMapValues;

    private XcosKeyMap() {
        // private constructor : static class
    }

    // static constructor to fill the action map key - values
    static {
        final TreeMap < String, Class <? extends DefaultAction >> tree = new TreeMap < String, Class <? extends DefaultAction >> ();

        /*
         * Append
         */
        tree.put("xcos-new", NewDiagramAction.class);
        tree.put("xcos-open", OpenAction.class);
        tree.put("xcos-open-in-sci", OpenInSciAction.class);
        tree.put("xcos-save", SaveAction.class);
        tree.put("xcos-save-as", SaveAsAction.class);
        tree.put("xcos-export", ExportAction.class);
        // recent is not handled
        tree.put("xcos-print", PrintAction.class);
        tree.put("xcos-close", CloseAction.class);
        tree.put("xcos-quit", QuitAction.class);

        tree.put("xcos-undo", UndoAction.class);
        tree.put("xcos-redo", RedoAction.class);
        tree.put("xcos-cut", CutAction.class);
        tree.put("xcos-copy", CopyAction.class);
        tree.put("xcos-paste", PasteAction.class);
        tree.put("xcos-delete", DeleteAction.class);
        tree.put("xcos-select-all", SelectAllAction.class);
        tree.put("xcos-invert-selection", InvertSelectionAction.class);
        tree.put("xcos-block-parameters", BlockParametersAction.class);
        tree.put("xcos-region-to-superblock", RegionToSuperblockAction.class);

        tree.put("xcos-zoom-in", ZoomInAction.class);
        tree.put("xcos-zoom-out", ZoomOutAction.class);
        tree.put("xcos-fit-diagram", FitDiagramToViewAction.class);
        tree.put("xcos-zoom-normal", NormalViewAction.class);
        tree.put("xcos-view-palette", ViewPaletteBrowserAction.class);
        tree.put("xcos-view-diagram", ViewDiagramBrowserAction.class);
        tree.put("xcos-view-viewport", ViewViewportAction.class);
        tree.put("xcos-view-details", ViewDetailsAction.class);

        tree.put("xcos-setup", SetupAction.class);
        tree.put("xcos-debug-level", DebugLevelAction.class);
        tree.put("xcos-set-context", SetContextAction.class);
        tree.put("xcos-compile", CompileAction.class);
        tree.put("xcos-start", StartAction.class);
        tree.put("xcos-stop", StopAction.class);

        tree.put("xcos-rotate", RotateAction.class);
        tree.put("xcos-flip", FlipAction.class);
        tree.put("xcos-mirror", MirrorAction.class);
        tree.put("xcos-shadow", ShowHideShadowAction.class);
        tree.put("xcos-align-left", AlignBlockActionLeft.class);
        tree.put("xcos-align-center", AlignBlockActionCenter.class);
        tree.put("xcos-align-right", AlignBlockActionRight.class);
        tree.put("xcos-align-top", AlignBlockActionTop.class);
        tree.put("xcos-align-middle", AlignBlockActionMiddle.class);
        tree.put("xcos-align-bottom", AlignBlockActionBottom.class);
        tree.put("xcos-border-color", BorderColorAction.class);
        tree.put("xcos-fill-color", FilledColorAction.class);
        tree.put("xcos-link-horizontal", StyleHorizontalAction.class);
        tree.put("xcos-link-straight", StyleStraightAction.class);
        tree.put("xcos-fill-vertical", StyleVerticalAction.class);
        tree.put("xcos-background", DiagramBackgroundAction.class);
        tree.put("xcos-grid", DiagramBackgroundAction.class);

        tree.put("xcos-code-generation", CodeGenerationAction.class);

        tree.put("xcos-help", XcosDocumentationAction.class);
        tree.put("xcos-help-block", BlockDocumentationAction.class);
        tree.put("xcos-demo", XcosDemonstrationsAction.class);
        tree.put("xcos-about", AboutXcosAction.class);

        /*
         * Copy to the local sorted map
         */
        final int len = tree.size();
        actionMapKeys = tree.keySet().toArray(new String[len]);
        actionMapValues = tree.values().toArray(new Class[len]);
    }

    private static Map<String, KeyStroke> getActionKeys() {
        if (actionKeys == null) {
            Document doc = XConfiguration.getXConfigurationDocument();
            actionKeys = XConfiguration.get(doc, "name", String.class, "key", KeyStroke.class, KEYMAP_XPATH);

        }

        return actionKeys;
    }

    public static void invalidate(XcosConfiguration.KeyMap conf) {
        if (conf.keymap) {
            actionKeys = null;
        }
    }

    public static void updateActionKeys() {
        /*
         * Update all the graph actions after an update
         */
        final Map<String, KeyStroke> actions = getActionKeys();

        for (final Entry<String, KeyStroke> it : actions.entrySet()) {
            final int found = Arrays.binarySearch(actionMapKeys, it.getKey());
            if (found >= 0) {
                GraphActionManager.updateActionKeyStroke(actionMapValues[found], it.getValue());
            }
        }
    }
}
