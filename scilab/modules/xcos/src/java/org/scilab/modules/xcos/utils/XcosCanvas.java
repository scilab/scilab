package org.scilab.modules.xcos.utils;

import java.awt.AlphaComposite;
import java.awt.Composite;
import java.awt.Stroke;
import java.awt.geom.AffineTransform;
import java.util.Hashtable;

import com.mxgraph.swing.view.mxInteractiveCanvas;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxUtils;

public class XcosCanvas extends mxInteractiveCanvas{

	public Object drawVertex(int x, int y, int w, int h,
			Hashtable<String, Object> style) {

		//		System.err.println("drawVertex");
		//		if (g != null)
		//		{
		//			x += translate.x;
		//			y += translate.y;
		//
		//			// Applies the rotation on the graphics object and stores
		//			// the previous transform so that it can be restored
		//			AffineTransform transform = null;
		//			transform = g.getTransform();
		//			
		//			String flip = mxUtils.getString(style, XcosConstants.STYLE_FLIP, "false");
		//			String mirror = mxUtils.getString(style, XcosConstants.STYLE_MIRROR, "false");
		//			System.err.println("flip : " + flip);
		//			System.err.println("mirror : " + mirror);
		//
		//			if (flip.compareTo("true") == 0) {
		//				if (mirror.compareTo("true") == 0) {
		//					g.scale(-1, -1);
		//				} else {
		//					g.scale(1, -1);
		//				}
		//			} else {
		//				if (mirror.compareTo("true") == 0) {
		//					g.scale(-1, 1);
		//				} else {
		//					g.scale(1, 1);
		//				}
		//			}
		//		
		//
		//			super.drawVertex(x, y, w, h, style);
		//
		//			// Restores the affine transformation
		//			if (transform != null)
		//			{
		//				g.setTransform(transform);
		//			}
		//
		//		}
		//		
		//		
		//		
		//		return null;
		
		if (g != null)
		{
			x += translate.x;
			y += translate.y;

			// Applies the rotation on the graphics object and stores
			// the previous transform so that it can be restored
			AffineTransform saveTransform = g.getTransform();
			g.translate( w / 2.0, h / 2.0 );

			double rotation = mxUtils.getDouble(style, mxConstants.STYLE_ROTATION, 0);

			if (rotation != 0) {
				g.rotate(Math.toRadians(rotation));
			}

			String flip = mxUtils.getString(style, XcosConstants.STYLE_FLIP, "false");
			String mirror = mxUtils.getString(style, XcosConstants.STYLE_MIRROR, "false");

			if (flip.compareTo("true") == 0) {
				if (mirror.compareTo("true") == 0) {
					g.scale(-1.0, -1.0);
				} else {
					g.scale(1.0, -1.0);
				}
			} else {
				if (mirror.compareTo("true") == 0) {
					g.scale(-1.0, 1.0);
				} else {
					g.scale(1.0, 1.0);
				}
			}
			g.translate( -w / 2.0, -h / 2.0 );
			
			Composite composite = null;
			float opacity = mxUtils.getFloat(style, mxConstants.STYLE_OPACITY,
					100);

			// Applies the opacity to the graphics object
			if (opacity != 100)
			{
				composite = g.getComposite();
				g.setComposite(AlphaComposite.getInstance(
						AlphaComposite.SRC_OVER, opacity / 100));
			}

			// Saves the stroke
			Stroke stroke = g.getStroke();

			// Draws a swimlane if start is > 0
			int start = mxUtils.getInt(style, mxConstants.STYLE_STARTSIZE);

			if (start == 0)
			{
				drawShape(x, y, w, h, style);
			}
			else
			{
				start = (int) Math.round(start * scale);

				// Removes some styles to draw the content area
				Hashtable<String, Object> cloned = new Hashtable<String, Object>(
						style);
				cloned.remove(mxConstants.STYLE_FILLCOLOR);
				cloned.remove(mxConstants.STYLE_ROUNDED);

				if (mxUtils.isTrue(style, mxConstants.STYLE_HORIZONTAL, true))
				{
					drawShape(x, y, w, start, style);
					drawShape(x, y + start, w, h - start, cloned);
				}
				else
				{
					drawShape(x, y, start, h, style);
					drawShape(x + start, y, w - start, h, cloned);
				}
			}

			// Restores the stroke
			g.setStroke(stroke);

			// Restores the composite rule on the graphics object
			if (composite != null)
			{
				g.setComposite(composite);
			}

			// Restores the affine transformation
			if (saveTransform != null)
			{
				g.setTransform(saveTransform);
			}
		}

		return null;
	}
}
