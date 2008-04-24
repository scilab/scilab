/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Represent a 3D plane with its equation A.x + B.y + C.z + D = 0
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.utils.geom3D;

/**
 * Represent a 3D plane with its equation A.x + B.y + C.z + D = 0
 * @author Jean-Baptiste Silvy
 */
public class Plane3D {
	
	/** Number of parameter to describe the plane */
	public static final int PLANE3D_EQUATION_LENGTH = 4;
	
	private double a;
	private double b;
	private double c;
	private double d;
	
	/**
	 * Define a plane by giving the equation a.x + b.y + c.z + d = 0
	 * @param a first equation parameter
	 * @param b second equation parameter
	 * @param c third equation parameter
	 * @param d fourth equation parameter
	 */
	public Plane3D(double a, double b, double c, double d) {
		setEquation(a, b, c, d);
	}
	
	/**
	 * Define a plane by giving the equation a.x + b.y + c.z + d = 0
	 * @param equation array of size 4 giving a, b, c and d
	 */
	public Plane3D(double[] equation) {
		setEquation(equation);
	}
	
	/**
	 * Define a plane by giving the equation a.x + b.y + c.z + d = 0
	 * @param copyPlane the plane to copy
	 */
	public Plane3D(Plane3D copyPlane) {
		setEquation(copyPlane.getEquation());
	}
	
	/**
	 * Define a plane by giving three of its non aligned points
	 * @param point1 first point
	 * @param point2 second point
	 * @param point3 third point
	 */
	public Plane3D(Vector3D point1, Vector3D point2, Vector3D point3) {
		setEquation(point1, point2, point3);
	}
	
	/**
	 * Default constructor. Should not be used, it does not create a valid plane.
	 */
	protected Plane3D() {
		setEquation(0.0, 0.0, 0.0, 0.0);
	}
	
	
	/**
	 * Get the equation of the plane
	 * @param equation array of size 4
	 */
	public void getEquation(double[] equation) {
		equation[0] = a;
		equation[1] = b;
		equation[2] = c;
		equation[PLANE3D_EQUATION_LENGTH - 1] = d;
	}
	
	/**
	 * Get the equation of the plane
	 * @return equation which is array of size 4
	 */
	public double[] getEquation() {
		double[] equation = new double[PLANE3D_EQUATION_LENGTH];
		getEquation(equation);
		return equation;
	}
	
	/**
	 * Set the equation of the plane by giving the equation a.x + b.y + c.z + d = 0
	 * @param a first equation parameter
	 * @param b second equation parameter
	 * @param c third equation parameter
	 * @param d fourth equation parameter
	 */
	protected void setEquation(double a, double b, double c, double d) {
		this.a = a;
		this.b = b;
		this.c = c;
		this.d = d;
	}
	
	/**
	 * Set plane equation by giving the equation a.x + b.y + c.z + d = 0
	 * @param equation array of size 4 giving a, b, c and d
	 */
	protected void setEquation(double[] equation) {
		setEquation(equation[0], equation[1], equation[2], equation[PLANE3D_EQUATION_LENGTH - 1]);
	}
	
	/**
	 * Miltuply the equation by a scalar. Does not modify the plane unless scalar is 0.
	 * @param scalar multiplication scalar. Should not be 0.
	 */
	protected void scalarMult(double scalar) {
		a = scalar * a;
		b = scalar * b;
		c = scalar * c;
		d = scalar * d;
	}
	
	/**
	 * Set plane equation by giving three of its non aligned points
	 * @param point1 first point
	 * @param point2 second point
	 * @param point3 third point
	 */
	public void setEquation(Vector3D point1, Vector3D point2, Vector3D point3) {
		// first compute the normal of the plane
		Vector3D p1p2 = point2.substract(point1);
		Vector3D p1p3 = point3.substract(point1);

		Vector3D normal = p1p2.crossProduct(p1p3);
		normal.normalize();
		
		setEquation(point1, normal);
		
	}
	
	/**
	 * Set plane eqution by giving three of its non aligned points and a direction.
	 * W3 points defined correctly a plane. However, there is still two possibles normals for the plane
	 * which are opposite. The normal dir is here to specify the one to choose.
	 * @param point1 first point on the plane
	 * @param point2 second point on the plane
	 * @param point3 third point on the plane
	 * @param normalDir normal choice is made by using the one which has a positive dot product with normalDir
	 */
	public void setEquation(Vector3D point1, Vector3D point2, Vector3D point3, Vector3D normalDir) {
		setEquation(point1, point2, point3);
		if (getNormal().dotProduct(normalDir) < 0.0) {
			setEquation(point1, getNormal().getOpposite());
		}
	}
	
	/**
	 * Set plane equation by knowing its normal and a point on the plane
	 * @param point point on the plane
	 * @param normal normal of the plane
	 */
	public void setEquation(Vector3D point, Vector3D normal) {
		// equation of the plane is Nx.x + Ny.y + Nz.z + d = 0
		this.a = normal.getX();
		this.b = normal.getY();
		this.c = normal.getZ();
		
		// point is on the plane so a.p1x + b.p1y + c.p1z + d = 0;
		this.d = -this.a * point.getX() - this.b * point.getY() - this.c * point.getZ();
	}
	
	/**
	 * get a normal of the plane
	 * @return normal of the plane
	 */
	public Vector3D getNormal() {
		Vector3D res = new Vector3D(a, b, c);
		res.normalize();
		return res;
	}
	
	
	/**
	 * Get the a point on the plane and its normal
	 * @param point a point on the plane
	 * @param normal normal of the plane, normalized
	 */
	public void getPointAndNormal(Vector3D point, Vector3D normal) {
		getThreePointsOnPlane(point, point, point);
		normal.setValues(getNormal());
	}
	
	/**
	 * Compute the coordinates of three points included on the plane.
	 * The tripet (P1, P1P2, P1P3) is defining an orthonormal frame.
	 * @param point1 first point
	 * @param point2 second point
	 * @param point3 third point
	 */
	public void getThreePointsOnPlane(Vector3D point1, Vector3D point2, Vector3D point3) {
		// do not modify directly p2 to allows to use the same vector 3 times
		Vector3D p1 = new Vector3D();
		Vector3D p2 = new Vector3D();
		if (Math.abs(a) >= Math.abs(b) && Math.abs(a) >= Math.abs(c)) {
			
			// a is the greatest so we can use it as divider.
		    // we assume that at least one of the first three parameters is not 0.
		    // now we fix some value for y and z and find the corresponding x
		    // to avoid arithmetical issues we use the values values (0,0) and (1,1)
			p1.setValues(-d / a, 0.0, 0.0); // A.x + 0.B + 0.C + D = 0
		    p2.setValues(-(b + c + d) / a, 1.0, 1.0); // A.x + B + C + D = 0
		    
		} else if (Math.abs(b) >= Math.abs(c)) {
			// same with b instead of a
			p1.setValues(0.0, -d / b, 0.0); // 0.A + B.y + 0.C + D = 0
			p2.setValues(1.0, -(a + c + d) / b, 1.0); // A + B.y + C + D = 0
			
		} else {
			// same with c
			p1.setValues(0.0, 0.0, -d / c); // 0.A + 0.B + C.z + D = 0
			p2.setValues(1.0, 1.0, -(a + b + d) / c); // A + B + C.z + D = 0
		}
		Vector3D p1p2 = p2.substract(p1);
		p1p2.normalize(); // normalize distance between P1 and P2
		point2.setValues(p1.add(p1p2)); // retrieve P2 coordinates
		
		// find the last vector, cross product of the normal (Z) and the first vector (X).
		Vector3D p1p3 = getNormal().crossProduct(p1p2);
		point3.setValues(p1.add(p1p3));
		point1.setValues(p1);
		
	}
	
	/**
	 * Project a point on the plane orthogonaly.
	 * @param point point to project
	 * @return projection of the point on the plane.
	 */
	public Vector3D projectOrtho(Vector3D point) {
		
		// resut is P' = P + k.n
		// with k = PO. n /||n||^2
		// where P is the point, P' its projection, n the normal and O a point on the plane.
		
		Vector3D pointOnPlane = new Vector3D();
		Vector3D normal = new Vector3D();
		getPointAndNormal(pointOnPlane, normal);
		
		Vector3D po = pointOnPlane.substract(point);
		double k = po.dotProduct(normal) / normal.getSquareNorm();
		
		return point.add(normal.scalarMult(k));
	}

	/**
	 * @return text representation of the plane
	 */
	public String toString() {
		return a + "x + " + b + "y + " + c + "z + " + d + " = 0";
	}

}
