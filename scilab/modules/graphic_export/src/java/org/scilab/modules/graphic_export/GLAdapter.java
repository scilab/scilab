package org.scilab.modules.graphic_export;

import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.DoubleBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.nio.LongBuffer;
import java.nio.ShortBuffer;
import javax.media.opengl.GL;

/**
 * GLAdapter
 * @author Sylvestre Koumar
 *
 */
public class GLAdapter implements GL {


	public Object getExtension(String arg0) {
		throw new UnsupportedOperationException();
	}

	
	public Object getPlatformGLExtensions() {
		throw new UnsupportedOperationException();
	}

	
	public void glAccum(int arg0, float arg1) {
		throw new UnsupportedOperationException();		
	}

	
	public void glActiveStencilFaceEXT(int arg0) {
		throw new UnsupportedOperationException();		
	}

	
	public void glActiveTexture(int arg0) {
		throw new UnsupportedOperationException();		
	}

	
	public void glActiveVaryingNV(int arg0, ByteBuffer arg1) {
		throw new UnsupportedOperationException();		
	}

	
	public void glActiveVaryingNV(int arg0, byte[] arg1, int arg2) {
		throw new UnsupportedOperationException();
	}

	
	public ByteBuffer glAllocateMemoryNV(int arg0, float arg1, float arg2,
			float arg3) {
		throw new UnsupportedOperationException();
	}

	
	public void glAlphaFragmentOp1ATI(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5) {
		throw new UnsupportedOperationException();
	}

	
	public void glAlphaFragmentOp2ATI(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, int arg7, int arg8) {
		throw new UnsupportedOperationException();
	}

	
	public void glAlphaFragmentOp3ATI(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, int arg7, int arg8, int arg9,
			int arg10, int arg11) {
		throw new UnsupportedOperationException();
	}

	
	public void glAlphaFunc(int arg0, float arg1) {
		throw new UnsupportedOperationException();
	}

	
	public void glApplyTextureEXT(int arg0) {
		throw new UnsupportedOperationException();
	}

	
	public boolean glAreProgramsResidentNV(int arg0, IntBuffer arg1,
			ByteBuffer arg2) {
		throw new UnsupportedOperationException();
	}

	
	public boolean glAreProgramsResidentNV(int arg0, int[] arg1, int arg2,
			byte[] arg3, int arg4) {
		throw new UnsupportedOperationException();
	}

	
	public boolean glAreTexturesResident(int arg0, IntBuffer arg1,
			ByteBuffer arg2) {
		throw new UnsupportedOperationException();
	}

	
	public boolean glAreTexturesResident(int arg0, int[] arg1, int arg2,
			byte[] arg3, int arg4) {
		throw new UnsupportedOperationException();
	}

	
	public void glArrayElement(int arg0) {
		throw new UnsupportedOperationException();
	}

	
	public void glArrayObjectATI(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5) {
		throw new UnsupportedOperationException();
	}

	
	public void glAsyncMarkerSGIX(int arg0) {
		throw new UnsupportedOperationException();
	}

	
	public void glAttachObjectARB(int arg0, int arg1) {
		throw new UnsupportedOperationException();
	}

	
	public void glAttachShader(int arg0, int arg1) {
		throw new UnsupportedOperationException();
	}

	
	public void glBegin(int arg0) {
		throw new UnsupportedOperationException();
	}

	
	public void glBeginFragmentShaderATI() {
		throw new UnsupportedOperationException();
	}

	
	public void glBeginOcclusionQueryNV(int arg0) {
		throw new UnsupportedOperationException();
	}

	
	public void glBeginQuery(int arg0, int arg1) {
		throw new UnsupportedOperationException();
	}

	
	public void glBeginQueryARB(int arg0, int arg1) {
		throw new UnsupportedOperationException();
	}

	
	public void glBeginTransformFeedbackNV(int arg0) {
		throw new UnsupportedOperationException();
	}

	
	public void glBeginVertexShaderEXT() {
		throw new UnsupportedOperationException();
	}

	
	public void glBindAttribLocation(int arg0, int arg1, String arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBindAttribLocationARB(int arg0, int arg1, String arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBindBuffer(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBindBufferARB(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBindBufferBaseNV(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBindBufferOffsetNV(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBindBufferRangeNV(int arg0, int arg1, int arg2, int arg3,
			int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBindFragDataLocationEXT(int arg0, int arg1, ByteBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBindFragDataLocationEXT(int arg0, int arg1, byte[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBindFragmentShaderATI(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBindFramebufferEXT(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glBindLightParameterEXT(int arg0, int arg1) {
		throw new UnsupportedOperationException();
	}

	
	public int glBindMaterialParameterEXT(int arg0, int arg1) {
		throw new UnsupportedOperationException();
	}

	
	public int glBindParameterEXT(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBindProgramARB(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBindProgramNV(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBindRenderbufferEXT(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glBindTexGenParameterEXT(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBindTexture(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glBindTextureUnitParameterEXT(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBindVertexArrayAPPLE(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBindVertexShaderEXT(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBitmap(int arg0, int arg1, float arg2, float arg3,
			float arg4, float arg5, ByteBuffer arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBitmap(int arg0, int arg1, float arg2, float arg3,
			float arg4, float arg5, long arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBitmap(int arg0, int arg1, float arg2, float arg3,
			float arg4, float arg5, byte[] arg6, int arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBlendColor(float arg0, float arg1, float arg2, float arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBlendEquation(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBlendEquationSeparate(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBlendEquationSeparateEXT(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBlendFunc(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBlendFuncSeparate(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBlendFuncSeparateEXT(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBlendFuncSeparateINGR(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBlitFramebufferEXT(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, int arg7, int arg8, int arg9) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBufferData(int arg0, int arg1, Buffer arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBufferDataARB(int arg0, int arg1, Buffer arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBufferParameteriAPPLE(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glBufferRegionEnabled() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBufferSubData(int arg0, int arg1, int arg2, Buffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glBufferSubDataARB(int arg0, int arg1, int arg2, Buffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCallList(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCallLists(int arg0, int arg1, Buffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glCheckFramebufferStatusEXT(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glClampColorARB(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glClear(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glClearAccum(float arg0, float arg1, float arg2, float arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glClearColor(float arg0, float arg1, float arg2, float arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glClearColorIiEXT(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glClearColorIuiEXT(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glClearDepth(double arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glClearDepthdNV(double arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glClearIndex(float arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glClearStencil(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glClientActiveTexture(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glClientActiveVertexStreamATI(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glClipPlane(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glClipPlane(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3b(byte arg0, byte arg1, byte arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3bv(ByteBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3bv(byte[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3d(double arg0, double arg1, double arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3dv(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3dv(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3f(float arg0, float arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3fVertex3fSUN(float arg0, float arg1, float arg2,
			float arg3, float arg4, float arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3fVertex3fvSUN(FloatBuffer arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3fVertex3fvSUN(float[] arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3fv(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3fv(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3hNV(short arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3hvNV(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3hvNV(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3i(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3iv(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3iv(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3s(short arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3sv(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3sv(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3ub(byte arg0, byte arg1, byte arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3ubv(ByteBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3ubv(byte[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3ui(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3uiv(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3uiv(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3us(short arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3usv(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor3usv(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4b(byte arg0, byte arg1, byte arg2, byte arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4bv(ByteBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4bv(byte[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4d(double arg0, double arg1, double arg2, double arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4dv(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4dv(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4f(float arg0, float arg1, float arg2, float arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4fNormal3fVertex3fSUN(float arg0, float arg1,
			float arg2, float arg3, float arg4, float arg5, float arg6,
			float arg7, float arg8, float arg9) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4fNormal3fVertex3fvSUN(FloatBuffer arg0,
			FloatBuffer arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4fNormal3fVertex3fvSUN(float[] arg0, int arg1,
			float[] arg2, int arg3, float[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4fv(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4fv(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4hNV(short arg0, short arg1, short arg2, short arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4hvNV(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4hvNV(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4i(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4iv(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4iv(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4s(short arg0, short arg1, short arg2, short arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4sv(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4sv(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4ub(byte arg0, byte arg1, byte arg2, byte arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4ubVertex2fSUN(byte arg0, byte arg1, byte arg2,
			byte arg3, float arg4, float arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4ubVertex2fvSUN(ByteBuffer arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4ubVertex2fvSUN(byte[] arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4ubVertex3fSUN(byte arg0, byte arg1, byte arg2,
			byte arg3, float arg4, float arg5, float arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4ubVertex3fvSUN(ByteBuffer arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4ubVertex3fvSUN(byte[] arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4ubv(ByteBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4ubv(byte[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4ui(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4uiv(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4uiv(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4us(short arg0, short arg1, short arg2, short arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4usv(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColor4usv(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColorFragmentOp1ATI(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColorFragmentOp2ATI(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, int arg7, int arg8, int arg9) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColorFragmentOp3ATI(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, int arg7, int arg8, int arg9,
			int arg10, int arg11, int arg12) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColorMask(boolean arg0, boolean arg1, boolean arg2,
			boolean arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColorMaskIndexedEXT(int arg0, boolean arg1, boolean arg2,
			boolean arg3, boolean arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColorMaterial(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColorPointer(int arg0, int arg1, int arg2, Buffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColorPointer(int arg0, int arg1, int arg2, long arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColorSubTable(int arg0, int arg1, int arg2, int arg3,
			int arg4, Buffer arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColorSubTable(int arg0, int arg1, int arg2, int arg3,
			int arg4, long arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColorTable(int arg0, int arg1, int arg2, int arg3, int arg4,
			Buffer arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColorTable(int arg0, int arg1, int arg2, int arg3, int arg4,
			long arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColorTableEXT(int arg0, int arg1, int arg2, int arg3,
			int arg4, Buffer arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColorTableParameterfv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColorTableParameterfv(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColorTableParameteriv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glColorTableParameteriv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCombinerInputNV(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCombinerOutputNV(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, boolean arg7, boolean arg8,
			boolean arg9) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCombinerParameterfNV(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCombinerParameterfvNV(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCombinerParameterfvNV(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCombinerParameteriNV(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCombinerParameterivNV(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCombinerParameterivNV(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCombinerStageParameterfvNV(int arg0, int arg1,
			FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCombinerStageParameterfvNV(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCompileShader(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCompileShaderARB(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCompressedTexImage1D(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, Buffer arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCompressedTexImage1D(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, long arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCompressedTexImage2D(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, Buffer arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCompressedTexImage2D(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, long arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCompressedTexImage3D(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, int arg7, Buffer arg8) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCompressedTexImage3D(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, int arg7, long arg8) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCompressedTexSubImage1D(int arg0, int arg1, int arg2,
			int arg3, int arg4, int arg5, Buffer arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCompressedTexSubImage1D(int arg0, int arg1, int arg2,
			int arg3, int arg4, int arg5, long arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCompressedTexSubImage2D(int arg0, int arg1, int arg2,
			int arg3, int arg4, int arg5, int arg6, int arg7, Buffer arg8) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCompressedTexSubImage2D(int arg0, int arg1, int arg2,
			int arg3, int arg4, int arg5, int arg6, int arg7, long arg8) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCompressedTexSubImage3D(int arg0, int arg1, int arg2,
			int arg3, int arg4, int arg5, int arg6, int arg7, int arg8,
			int arg9, Buffer arg10) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCompressedTexSubImage3D(int arg0, int arg1, int arg2,
			int arg3, int arg4, int arg5, int arg6, int arg7, int arg8,
			int arg9, long arg10) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glConvolutionFilter1D(int arg0, int arg1, int arg2, int arg3,
			int arg4, Buffer arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glConvolutionFilter1D(int arg0, int arg1, int arg2, int arg3,
			int arg4, long arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glConvolutionFilter2D(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, Buffer arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glConvolutionFilter2D(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, long arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glConvolutionParameterf(int arg0, int arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glConvolutionParameterfv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glConvolutionParameterfv(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glConvolutionParameteri(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glConvolutionParameteriv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glConvolutionParameteriv(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCopyColorSubTable(int arg0, int arg1, int arg2, int arg3,
			int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCopyColorTable(int arg0, int arg1, int arg2, int arg3,
			int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCopyConvolutionFilter1D(int arg0, int arg1, int arg2,
			int arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCopyConvolutionFilter2D(int arg0, int arg1, int arg2,
			int arg3, int arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCopyPixels(int arg0, int arg1, int arg2, int arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCopyTexImage1D(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCopyTexImage2D(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, int arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCopyTexSubImage1D(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCopyTexSubImage2D(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, int arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCopyTexSubImage3D(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, int arg7, int arg8) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glCreateProgram() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glCreateProgramObjectARB() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glCreateShader(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glCreateShaderObjectARB(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCullFace(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCullParameterdvEXT(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCullParameterdvEXT(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCullParameterfvEXT(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCullParameterfvEXT(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glCurrentPaletteMatrixARB(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeformSGIX(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeformationMap3dSGIX(int arg0, double arg1, double arg2,
			int arg3, int arg4, double arg5, double arg6, int arg7, int arg8,
			double arg9, double arg10, int arg11, int arg12, DoubleBuffer arg13) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeformationMap3dSGIX(int arg0, double arg1, double arg2,
			int arg3, int arg4, double arg5, double arg6, int arg7, int arg8,
			double arg9, double arg10, int arg11, int arg12, double[] arg13,
			int arg14) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeformationMap3fSGIX(int arg0, float arg1, float arg2,
			int arg3, int arg4, float arg5, float arg6, int arg7, int arg8,
			float arg9, float arg10, int arg11, int arg12, FloatBuffer arg13) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeformationMap3fSGIX(int arg0, float arg1, float arg2,
			int arg3, int arg4, float arg5, float arg6, int arg7, int arg8,
			float arg9, float arg10, int arg11, int arg12, float[] arg13,
			int arg14) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteAsyncMarkersSGIX(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteBufferRegion(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteBuffers(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteBuffers(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteBuffersARB(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteBuffersARB(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteFencesAPPLE(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteFencesAPPLE(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteFencesNV(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteFencesNV(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteFragmentShaderATI(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteFramebuffersEXT(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteFramebuffersEXT(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteLists(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteObjectARB(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteOcclusionQueriesNV(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteOcclusionQueriesNV(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteProgram(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteProgramsARB(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteProgramsARB(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteProgramsNV(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteProgramsNV(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteQueries(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteQueries(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteQueriesARB(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteQueriesARB(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteRenderbuffersEXT(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteRenderbuffersEXT(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteShader(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteTextures(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteTextures(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteVertexArraysAPPLE(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteVertexArraysAPPLE(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDeleteVertexShaderEXT(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDepthBoundsEXT(double arg0, double arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDepthBoundsdNV(double arg0, double arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDepthFunc(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDepthMask(boolean arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDepthRange(double arg0, double arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDepthRangedNV(double arg0, double arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDetachObjectARB(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDetachShader(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDetailTexFuncSGIS(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDetailTexFuncSGIS(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDisable(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDisableClientState(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDisableIndexedEXT(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDisableVariantClientStateEXT(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDisableVertexAttribAPPLE(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDisableVertexAttribArray(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDisableVertexAttribArrayARB(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawArrays(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawArraysInstancedEXT(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawBuffer(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawBufferRegion(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawBuffers(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawBuffers(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawBuffersARB(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawBuffersARB(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawBuffersATI(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawBuffersATI(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawElementArrayAPPLE(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawElementArrayATI(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawElements(int arg0, int arg1, int arg2, Buffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawElements(int arg0, int arg1, int arg2, long arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawElementsInstancedEXT(int arg0, int arg1, int arg2,
			Buffer arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawMeshArraysSUN(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawPixels(int arg0, int arg1, int arg2, int arg3, Buffer arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawPixels(int arg0, int arg1, int arg2, int arg3, long arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawRangeElementArrayAPPLE(int arg0, int arg1, int arg2,
			int arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawRangeElementArrayATI(int arg0, int arg1, int arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawRangeElements(int arg0, int arg1, int arg2, int arg3,
			int arg4, Buffer arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glDrawRangeElements(int arg0, int arg1, int arg2, int arg3,
			int arg4, long arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEdgeFlag(boolean arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEdgeFlagPointer(int arg0, Buffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEdgeFlagPointer(int arg0, long arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEdgeFlagv(ByteBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEdgeFlagv(byte[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glElementPointerAPPLE(int arg0, Buffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glElementPointerATI(int arg0, Buffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glElementPointerATI(int arg0, long arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEnable(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEnableClientState(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEnableIndexedEXT(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEnableVariantClientStateEXT(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEnableVertexAttribAPPLE(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEnableVertexAttribArray(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEnableVertexAttribArrayARB(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEnd() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEndFragmentShaderATI() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEndList() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEndOcclusionQueryNV() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEndQuery(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEndQueryARB(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEndTransformFeedbackNV() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEndVertexShaderEXT() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEvalCoord1d(double arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEvalCoord1dv(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEvalCoord1dv(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEvalCoord1f(float arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEvalCoord1fv(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEvalCoord1fv(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEvalCoord2d(double arg0, double arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEvalCoord2dv(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEvalCoord2dv(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEvalCoord2f(float arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEvalCoord2fv(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEvalCoord2fv(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEvalMapsNV(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEvalMesh1(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEvalMesh2(int arg0, int arg1, int arg2, int arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEvalPoint1(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glEvalPoint2(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glExecuteProgramNV(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glExecuteProgramNV(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glExtractComponentEXT(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFeedbackBuffer(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFinalCombinerInputNV(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFinish() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glFinishAsyncSGIX(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glFinishAsyncSGIX(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFinishFenceAPPLE(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFinishFenceNV(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFinishObjectAPPLE(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFinishRenderAPPLE() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFinishTextureSUNX() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFlush() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFlushMappedBufferRangeAPPLE(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFlushPixelDataRangeNV(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFlushRasterSGIX() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFlushRenderAPPLE() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFlushVertexArrayRangeAPPLE(int arg0, Buffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFlushVertexArrayRangeNV() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogCoordPointer(int arg0, int arg1, Buffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogCoordPointer(int arg0, int arg1, long arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogCoordPointerEXT(int arg0, int arg1, Buffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogCoordPointerEXT(int arg0, int arg1, long arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogCoordd(double arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogCoorddEXT(double arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogCoorddv(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogCoorddv(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogCoorddvEXT(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogCoorddvEXT(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogCoordf(float arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogCoordfEXT(float arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogCoordfv(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogCoordfv(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogCoordfvEXT(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogCoordfvEXT(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogCoordhNV(short arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogCoordhvNV(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogCoordhvNV(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogFuncSGIS(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogFuncSGIS(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogf(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogfv(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogfv(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogi(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogiv(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFogiv(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFragmentColorMaterialSGIX(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFragmentLightModelfSGIX(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFragmentLightModelfvSGIX(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFragmentLightModelfvSGIX(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFragmentLightModeliSGIX(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFragmentLightModelivSGIX(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFragmentLightModelivSGIX(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFragmentLightfSGIX(int arg0, int arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFragmentLightfvSGIX(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFragmentLightfvSGIX(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFragmentLightiSGIX(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFragmentLightivSGIX(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFragmentLightivSGIX(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFragmentMaterialfSGIX(int arg0, int arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFragmentMaterialfvSGIX(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFragmentMaterialfvSGIX(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFragmentMaterialiSGIX(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFragmentMaterialivSGIX(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFragmentMaterialivSGIX(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFrameZoomSGIX(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFramebufferRenderbufferEXT(int arg0, int arg1, int arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFramebufferTexture1DEXT(int arg0, int arg1, int arg2,
			int arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFramebufferTexture2DEXT(int arg0, int arg1, int arg2,
			int arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFramebufferTexture3DEXT(int arg0, int arg1, int arg2,
			int arg3, int arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFramebufferTextureEXT(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFramebufferTextureFaceEXT(int arg0, int arg1, int arg2,
			int arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFramebufferTextureLayerEXT(int arg0, int arg1, int arg2,
			int arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFreeObjectBufferATI(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFrontFace(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glFrustum(double arg0, double arg1, double arg2, double arg3,
			double arg4, double arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glGenAsyncMarkersSGIX(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenBuffers(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenBuffers(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenBuffersARB(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenBuffersARB(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenFencesAPPLE(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenFencesAPPLE(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenFencesNV(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenFencesNV(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glGenFragmentShadersATI(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenFramebuffersEXT(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenFramebuffersEXT(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glGenLists(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenOcclusionQueriesNV(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenOcclusionQueriesNV(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenProgramsARB(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenProgramsARB(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenProgramsNV(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenProgramsNV(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenQueries(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenQueries(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenQueriesARB(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenQueriesARB(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenRenderbuffersEXT(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenRenderbuffersEXT(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glGenSymbolsEXT(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenTextures(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenTextures(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenVertexArraysAPPLE(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenVertexArraysAPPLE(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glGenVertexShadersEXT(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGenerateMipmapEXT(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetActiveAttrib(int arg0, int arg1, int arg2, IntBuffer arg3,
			IntBuffer arg4, IntBuffer arg5, ByteBuffer arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetActiveAttrib(int arg0, int arg1, int arg2, int[] arg3,
			int arg4, int[] arg5, int arg6, int[] arg7, int arg8, byte[] arg9,
			int arg10) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetActiveAttribARB(int arg0, int arg1, int arg2,
			IntBuffer arg3, IntBuffer arg4, IntBuffer arg5, ByteBuffer arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetActiveAttribARB(int arg0, int arg1, int arg2, int[] arg3,
			int arg4, int[] arg5, int arg6, int[] arg7, int arg8, byte[] arg9,
			int arg10) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetActiveUniform(int arg0, int arg1, int arg2,
			IntBuffer arg3, IntBuffer arg4, IntBuffer arg5, ByteBuffer arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetActiveUniform(int arg0, int arg1, int arg2, int[] arg3,
			int arg4, int[] arg5, int arg6, int[] arg7, int arg8, byte[] arg9,
			int arg10) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetActiveUniformARB(int arg0, int arg1, int arg2,
			IntBuffer arg3, IntBuffer arg4, IntBuffer arg5, ByteBuffer arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetActiveUniformARB(int arg0, int arg1, int arg2, int[] arg3,
			int arg4, int[] arg5, int arg6, int[] arg7, int arg8, byte[] arg9,
			int arg10) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetActiveVaryingNV(int arg0, int arg1, int arg2,
			IntBuffer arg3, IntBuffer arg4, IntBuffer arg5, ByteBuffer arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetActiveVaryingNV(int arg0, int arg1, int arg2, int[] arg3,
			int arg4, int[] arg5, int arg6, int[] arg7, int arg8, byte[] arg9,
			int arg10) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetArrayObjectfvATI(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetArrayObjectfvATI(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetArrayObjectivATI(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetArrayObjectivATI(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetAttachedObjectsARB(int arg0, int arg1, IntBuffer arg2,
			IntBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetAttachedObjectsARB(int arg0, int arg1, int[] arg2,
			int arg3, int[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetAttachedShaders(int arg0, int arg1, IntBuffer arg2,
			IntBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetAttachedShaders(int arg0, int arg1, int[] arg2, int arg3,
			int[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glGetAttribLocation(int arg0, String arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glGetAttribLocationARB(int arg0, String arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetBooleanIndexedvEXT(int arg0, int arg1, ByteBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetBooleanIndexedvEXT(int arg0, int arg1, byte[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetBooleanv(int arg0, ByteBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetBooleanv(int arg0, byte[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetBufferParameteriv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetBufferParameteriv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetBufferParameterivARB(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetBufferParameterivARB(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetBufferSubData(int arg0, int arg1, int arg2, Buffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetBufferSubDataARB(int arg0, int arg1, int arg2, Buffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetClipPlane(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetClipPlane(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetColorTable(int arg0, int arg1, int arg2, Buffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetColorTable(int arg0, int arg1, int arg2, long arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetColorTableEXT(int arg0, int arg1, int arg2, Buffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetColorTableParameterfv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetColorTableParameterfv(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetColorTableParameterfvEXT(int arg0, int arg1,
			FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetColorTableParameterfvEXT(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetColorTableParameteriv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetColorTableParameteriv(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetColorTableParameterivEXT(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetColorTableParameterivEXT(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetCombinerInputParameterfvNV(int arg0, int arg1, int arg2,
			int arg3, FloatBuffer arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetCombinerInputParameterfvNV(int arg0, int arg1, int arg2,
			int arg3, float[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetCombinerInputParameterivNV(int arg0, int arg1, int arg2,
			int arg3, IntBuffer arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetCombinerInputParameterivNV(int arg0, int arg1, int arg2,
			int arg3, int[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetCombinerOutputParameterfvNV(int arg0, int arg1, int arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetCombinerOutputParameterfvNV(int arg0, int arg1, int arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetCombinerOutputParameterivNV(int arg0, int arg1, int arg2,
			IntBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetCombinerOutputParameterivNV(int arg0, int arg1, int arg2,
			int[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetCombinerStageParameterfvNV(int arg0, int arg1,
			FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetCombinerStageParameterfvNV(int arg0, int arg1,
			float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetCompressedTexImage(int arg0, int arg1, Buffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetCompressedTexImage(int arg0, int arg1, long arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetConvolutionFilter(int arg0, int arg1, int arg2, Buffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetConvolutionFilter(int arg0, int arg1, int arg2, long arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetConvolutionParameterfv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetConvolutionParameterfv(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetConvolutionParameteriv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetConvolutionParameteriv(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetDetailTexFuncSGIS(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetDetailTexFuncSGIS(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetDoublev(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetDoublev(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glGetError() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetFenceivNV(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetFenceivNV(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetFinalCombinerInputParameterfvNV(int arg0, int arg1,
			FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetFinalCombinerInputParameterfvNV(int arg0, int arg1,
			float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetFinalCombinerInputParameterivNV(int arg0, int arg1,
			IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetFinalCombinerInputParameterivNV(int arg0, int arg1,
			int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetFloatv(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetFloatv(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetFogFuncSGIS(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetFogFuncSGIS(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glGetFragDataLocationEXT(int arg0, ByteBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glGetFragDataLocationEXT(int arg0, byte[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetFragmentLightfvSGIX(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetFragmentLightfvSGIX(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetFragmentLightivSGIX(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetFragmentLightivSGIX(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetFragmentMaterialfvSGIX(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetFragmentMaterialfvSGIX(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetFragmentMaterialivSGIX(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetFragmentMaterialivSGIX(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetFramebufferAttachmentParameterivEXT(int arg0, int arg1,
			int arg2, IntBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetFramebufferAttachmentParameterivEXT(int arg0, int arg1,
			int arg2, int[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glGetHandleARB(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetHistogram(int arg0, boolean arg1, int arg2, int arg3,
			Buffer arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetHistogram(int arg0, boolean arg1, int arg2, int arg3,
			long arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetHistogramParameterfv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetHistogramParameterfv(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetHistogramParameteriv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetHistogramParameteriv(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetImageTransformParameterfvHP(int arg0, int arg1,
			FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetImageTransformParameterfvHP(int arg0, int arg1,
			float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetImageTransformParameterivHP(int arg0, int arg1,
			IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetImageTransformParameterivHP(int arg0, int arg1,
			int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetInfoLogARB(int arg0, int arg1, IntBuffer arg2,
			ByteBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetInfoLogARB(int arg0, int arg1, int[] arg2, int arg3,
			byte[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glGetInstrumentsSGIX() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetIntegerIndexedvEXT(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetIntegerIndexedvEXT(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetIntegerv(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetIntegerv(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetInvariantBooleanvEXT(int arg0, int arg1, ByteBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetInvariantBooleanvEXT(int arg0, int arg1, byte[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetInvariantFloatvEXT(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetInvariantFloatvEXT(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetInvariantIntegervEXT(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetInvariantIntegervEXT(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetLightfv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetLightfv(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetLightiv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetLightiv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetListParameterfvSGIX(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetListParameterfvSGIX(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetListParameterivSGIX(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetListParameterivSGIX(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetLocalConstantBooleanvEXT(int arg0, int arg1,
			ByteBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetLocalConstantBooleanvEXT(int arg0, int arg1, byte[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetLocalConstantFloatvEXT(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetLocalConstantFloatvEXT(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetLocalConstantIntegervEXT(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetLocalConstantIntegervEXT(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMapAttribParameterfvNV(int arg0, int arg1, int arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMapAttribParameterfvNV(int arg0, int arg1, int arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMapAttribParameterivNV(int arg0, int arg1, int arg2,
			IntBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMapAttribParameterivNV(int arg0, int arg1, int arg2,
			int[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMapControlPointsNV(int arg0, int arg1, int arg2, int arg3,
			int arg4, boolean arg5, Buffer arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMapParameterfvNV(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMapParameterfvNV(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMapParameterivNV(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMapParameterivNV(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMapdv(int arg0, int arg1, DoubleBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMapdv(int arg0, int arg1, double[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMapfv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMapfv(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMapiv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMapiv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMaterialfv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMaterialfv(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMaterialiv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMaterialiv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMinmax(int arg0, boolean arg1, int arg2, int arg3,
			Buffer arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMinmax(int arg0, boolean arg1, int arg2, int arg3,
			long arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMinmaxParameterfv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMinmaxParameterfv(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMinmaxParameteriv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetMinmaxParameteriv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetObjectBufferfvATI(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetObjectBufferfvATI(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetObjectBufferivATI(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetObjectBufferivATI(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetObjectParameterfvARB(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetObjectParameterfvARB(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetObjectParameterivARB(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetObjectParameterivARB(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetOcclusionQueryivNV(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetOcclusionQueryivNV(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetOcclusionQueryuivNV(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetOcclusionQueryuivNV(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetPixelMapfv(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetPixelMapfv(int arg0, long arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetPixelMapfv(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetPixelMapuiv(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetPixelMapuiv(int arg0, long arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetPixelMapuiv(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetPixelMapusv(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetPixelMapusv(int arg0, long arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetPixelMapusv(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetPixelTexGenParameterfvSGIS(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetPixelTexGenParameterfvSGIS(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetPixelTexGenParameterivSGIS(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetPixelTexGenParameterivSGIS(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetPolygonStipple(ByteBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetPolygonStipple(long arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetPolygonStipple(byte[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramEnvParameterIivNV(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramEnvParameterIivNV(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramEnvParameterIuivNV(int arg0, int arg1,
			IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramEnvParameterIuivNV(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramEnvParameterdvARB(int arg0, int arg1,
			DoubleBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramEnvParameterdvARB(int arg0, int arg1,
			double[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramEnvParameterfvARB(int arg0, int arg1,
			FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramEnvParameterfvARB(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramInfoLog(int arg0, int arg1, IntBuffer arg2,
			ByteBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramInfoLog(int arg0, int arg1, int[] arg2, int arg3,
			byte[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramLocalParameterIivNV(int arg0, int arg1,
			IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramLocalParameterIivNV(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramLocalParameterIuivNV(int arg0, int arg1,
			IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramLocalParameterIuivNV(int arg0, int arg1,
			int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramLocalParameterdvARB(int arg0, int arg1,
			DoubleBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramLocalParameterdvARB(int arg0, int arg1,
			double[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramLocalParameterfvARB(int arg0, int arg1,
			FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramLocalParameterfvARB(int arg0, int arg1,
			float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramNamedParameterdvNV(int arg0, int arg1, String arg2,
			DoubleBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramNamedParameterdvNV(int arg0, int arg1, String arg2,
			double[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramNamedParameterfvNV(int arg0, int arg1, String arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramNamedParameterfvNV(int arg0, int arg1, String arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramParameterdvNV(int arg0, int arg1, int arg2,
			DoubleBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramParameterdvNV(int arg0, int arg1, int arg2,
			double[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramParameterfvNV(int arg0, int arg1, int arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramParameterfvNV(int arg0, int arg1, int arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramStringARB(int arg0, int arg1, Buffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramStringNV(int arg0, int arg1, ByteBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramStringNV(int arg0, int arg1, byte[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramiv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramiv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramivARB(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramivARB(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramivNV(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetProgramivNV(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetQueryObjecti64vEXT(int arg0, int arg1, LongBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetQueryObjecti64vEXT(int arg0, int arg1, long[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetQueryObjectiv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetQueryObjectiv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetQueryObjectivARB(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetQueryObjectivARB(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetQueryObjectui64vEXT(int arg0, int arg1, LongBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetQueryObjectui64vEXT(int arg0, int arg1, long[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetQueryObjectuiv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetQueryObjectuiv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetQueryObjectuivARB(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetQueryObjectuivARB(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetQueryiv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetQueryiv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetQueryivARB(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetQueryivARB(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetRenderbufferParameterivEXT(int arg0, int arg1,
			IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetRenderbufferParameterivEXT(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetSeparableFilter(int arg0, int arg1, int arg2, Buffer arg3,
			Buffer arg4, Buffer arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetSeparableFilter(int arg0, int arg1, int arg2, long arg3,
			long arg4, long arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetShaderInfoLog(int arg0, int arg1, IntBuffer arg2,
			ByteBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetShaderInfoLog(int arg0, int arg1, int[] arg2, int arg3,
			byte[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetShaderSource(int arg0, int arg1, IntBuffer arg2,
			ByteBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetShaderSource(int arg0, int arg1, int[] arg2, int arg3,
			byte[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetShaderSourceARB(int arg0, int arg1, IntBuffer arg2,
			ByteBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetShaderSourceARB(int arg0, int arg1, int[] arg2, int arg3,
			byte[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetShaderiv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetShaderiv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetSharpenTexFuncSGIS(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetSharpenTexFuncSGIS(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public String glGetString(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexBumpParameterfvATI(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexBumpParameterfvATI(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexBumpParameterivATI(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexBumpParameterivATI(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexEnvfv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexEnvfv(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexEnviv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexEnviv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexFilterFuncSGIS(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexFilterFuncSGIS(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexGendv(int arg0, int arg1, DoubleBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexGendv(int arg0, int arg1, double[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexGenfv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexGenfv(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexGeniv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexGeniv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexImage(int arg0, int arg1, int arg2, int arg3,
			Buffer arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexImage(int arg0, int arg1, int arg2, int arg3, long arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexLevelParameterfv(int arg0, int arg1, int arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexLevelParameterfv(int arg0, int arg1, int arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexLevelParameteriv(int arg0, int arg1, int arg2,
			IntBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexLevelParameteriv(int arg0, int arg1, int arg2,
			int[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexParameterIivEXT(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexParameterIivEXT(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexParameterIuivEXT(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexParameterIuivEXT(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexParameterfv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexParameterfv(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexParameteriv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTexParameteriv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTrackMatrixivNV(int arg0, int arg1, int arg2,
			IntBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTrackMatrixivNV(int arg0, int arg1, int arg2, int[] arg3,
			int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTransformFeedbackVaryingNV(int arg0, int arg1,
			IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetTransformFeedbackVaryingNV(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glGetUniformBufferSizeEXT(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glGetUniformLocation(int arg0, String arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glGetUniformLocationARB(int arg0, String arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glGetUniformOffsetEXT(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetUniformfv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetUniformfv(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetUniformfvARB(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetUniformfvARB(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetUniformiv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetUniformiv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetUniformivARB(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetUniformivARB(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetUniformuivEXT(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetUniformuivEXT(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVariantArrayObjectfvATI(int arg0, int arg1,
			FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVariantArrayObjectfvATI(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVariantArrayObjectivATI(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVariantArrayObjectivATI(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVariantBooleanvEXT(int arg0, int arg1, ByteBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVariantBooleanvEXT(int arg0, int arg1, byte[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVariantFloatvEXT(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVariantFloatvEXT(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVariantIntegervEXT(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVariantIntegervEXT(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glGetVaryingLocationNV(int arg0, ByteBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glGetVaryingLocationNV(int arg0, byte[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribArrayObjectfvATI(int arg0, int arg1,
			FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribArrayObjectfvATI(int arg0, int arg1,
			float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribArrayObjectivATI(int arg0, int arg1,
			IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribArrayObjectivATI(int arg0, int arg1,
			int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribIivEXT(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribIivEXT(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribIuivEXT(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribIuivEXT(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribdv(int arg0, int arg1, DoubleBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribdv(int arg0, int arg1, double[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribdvARB(int arg0, int arg1, DoubleBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribdvARB(int arg0, int arg1, double[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribdvNV(int arg0, int arg1, DoubleBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribdvNV(int arg0, int arg1, double[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribfv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribfv(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribfvARB(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribfvARB(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribfvNV(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribfvNV(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribiv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribiv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribivARB(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribivARB(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribivNV(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGetVertexAttribivNV(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGlobalAlphaFactorbSUN(byte arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGlobalAlphaFactordSUN(double arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGlobalAlphaFactorfSUN(float arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGlobalAlphaFactoriSUN(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGlobalAlphaFactorsSUN(short arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGlobalAlphaFactorubSUN(byte arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGlobalAlphaFactoruiSUN(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glGlobalAlphaFactorusSUN(short arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glHint(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glHintPGI(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glHistogram(int arg0, int arg1, int arg2, boolean arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glIglooInterfaceSGIX(int arg0, Buffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glImageTransformParameterfHP(int arg0, int arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glImageTransformParameterfvHP(int arg0, int arg1,
			FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glImageTransformParameterfvHP(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glImageTransformParameteriHP(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glImageTransformParameterivHP(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glImageTransformParameterivHP(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glIndexFuncEXT(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glIndexMask(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glIndexMaterialEXT(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glIndexPointer(int arg0, int arg1, Buffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glIndexd(double arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glIndexdv(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glIndexdv(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glIndexf(float arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glIndexfv(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glIndexfv(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glIndexi(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glIndexiv(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glIndexiv(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glIndexs(short arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glIndexsv(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glIndexsv(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glIndexub(byte arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glIndexubv(ByteBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glIndexubv(byte[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glInitNames() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glInsertComponentEXT(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glInstrumentsBufferSGIX(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glInstrumentsBufferSGIX(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glInterleavedArrays(int arg0, int arg1, Buffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glInterleavedArrays(int arg0, int arg1, long arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsAsyncMarkerSGIX(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsBuffer(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsBufferARB(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsEnabled(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsEnabledIndexedEXT(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsFenceAPPLE(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsFenceNV(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsFramebufferEXT(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsList(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsObjectBufferATI(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsOcclusionQueryNV(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsProgram(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsProgramARB(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsProgramNV(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsQuery(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsQueryARB(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsRenderbufferEXT(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsShader(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsTexture(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsVariantEnabledEXT(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsVertexArrayAPPLE(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glIsVertexAttribEnabledAPPLE(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLightEnviSGIX(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLightModelf(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLightModelfv(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLightModelfv(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLightModeli(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLightModeliv(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLightModeliv(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLightf(int arg0, int arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLightfv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLightfv(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLighti(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLightiv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLightiv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLineStipple(int arg0, short arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLineWidth(float arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLinkProgram(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLinkProgramARB(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glListBase(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glListParameterfSGIX(int arg0, int arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glListParameterfvSGIX(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glListParameterfvSGIX(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glListParameteriSGIX(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glListParameterivSGIX(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glListParameterivSGIX(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLoadIdentity() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLoadIdentityDeformationMapSGIX(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLoadMatrixd(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLoadMatrixd(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLoadMatrixf(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLoadMatrixf(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLoadName(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLoadProgramNV(int arg0, int arg1, int arg2, String arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLoadTransposeMatrixd(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLoadTransposeMatrixd(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLoadTransposeMatrixf(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLoadTransposeMatrixf(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLockArraysEXT(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glLogicOp(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMap1d(int arg0, double arg1, double arg2, int arg3, int arg4,
			DoubleBuffer arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMap1d(int arg0, double arg1, double arg2, int arg3, int arg4,
			double[] arg5, int arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMap1f(int arg0, float arg1, float arg2, int arg3, int arg4,
			FloatBuffer arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMap1f(int arg0, float arg1, float arg2, int arg3, int arg4,
			float[] arg5, int arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMap2d(int arg0, double arg1, double arg2, int arg3, int arg4,
			double arg5, double arg6, int arg7, int arg8, DoubleBuffer arg9) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMap2d(int arg0, double arg1, double arg2, int arg3, int arg4,
			double arg5, double arg6, int arg7, int arg8, double[] arg9,
			int arg10) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMap2f(int arg0, float arg1, float arg2, int arg3, int arg4,
			float arg5, float arg6, int arg7, int arg8, FloatBuffer arg9) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMap2f(int arg0, float arg1, float arg2, int arg3, int arg4,
			float arg5, float arg6, int arg7, int arg8, float[] arg9, int arg10) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public ByteBuffer glMapBuffer(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public ByteBuffer glMapBufferARB(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMapControlPointsNV(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, boolean arg7, Buffer arg8) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMapGrid1d(int arg0, double arg1, double arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMapGrid1f(int arg0, float arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMapGrid2d(int arg0, double arg1, double arg2, int arg3,
			double arg4, double arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMapGrid2f(int arg0, float arg1, float arg2, int arg3,
			float arg4, float arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMapParameterfvNV(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMapParameterfvNV(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMapParameterivNV(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMapParameterivNV(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMapVertexAttrib1dAPPLE(int arg0, int arg1, double arg2,
			double arg3, int arg4, int arg5, DoubleBuffer arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMapVertexAttrib1dAPPLE(int arg0, int arg1, double arg2,
			double arg3, int arg4, int arg5, double[] arg6, int arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMapVertexAttrib1fAPPLE(int arg0, int arg1, float arg2,
			float arg3, int arg4, int arg5, FloatBuffer arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMapVertexAttrib1fAPPLE(int arg0, int arg1, float arg2,
			float arg3, int arg4, int arg5, float[] arg6, int arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMapVertexAttrib2dAPPLE(int arg0, int arg1, double arg2,
			double arg3, int arg4, int arg5, double arg6, double arg7,
			int arg8, int arg9, DoubleBuffer arg10) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMapVertexAttrib2dAPPLE(int arg0, int arg1, double arg2,
			double arg3, int arg4, int arg5, double arg6, double arg7,
			int arg8, int arg9, double[] arg10, int arg11) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMapVertexAttrib2fAPPLE(int arg0, int arg1, float arg2,
			float arg3, int arg4, int arg5, float arg6, float arg7, int arg8,
			int arg9, FloatBuffer arg10) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMapVertexAttrib2fAPPLE(int arg0, int arg1, float arg2,
			float arg3, int arg4, int arg5, float arg6, float arg7, int arg8,
			int arg9, float[] arg10, int arg11) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMaterialf(int arg0, int arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMaterialfv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMaterialfv(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMateriali(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMaterialiv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMaterialiv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMatrixIndexPointerARB(int arg0, int arg1, int arg2,
			Buffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMatrixIndexPointerARB(int arg0, int arg1, int arg2, long arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMatrixIndexubvARB(int arg0, ByteBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMatrixIndexubvARB(int arg0, byte[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMatrixIndexuivARB(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMatrixIndexuivARB(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMatrixIndexusvARB(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMatrixIndexusvARB(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMatrixMode(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMinmax(int arg0, int arg1, boolean arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultMatrixd(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultMatrixd(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultMatrixf(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultMatrixf(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultTransposeMatrixd(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultTransposeMatrixd(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultTransposeMatrixf(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultTransposeMatrixf(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiDrawArrays(int arg0, IntBuffer arg1, IntBuffer arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiDrawArrays(int arg0, int[] arg1, int arg2, int[] arg3,
			int arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiDrawArraysEXT(int arg0, IntBuffer arg1, IntBuffer arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiDrawArraysEXT(int arg0, int[] arg1, int arg2,
			int[] arg3, int arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiDrawElementArrayAPPLE(int arg0, IntBuffer arg1,
			IntBuffer arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiDrawElementArrayAPPLE(int arg0, int[] arg1, int arg2,
			int[] arg3, int arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiDrawElements(int arg0, IntBuffer arg1, int arg2,
			Buffer[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiDrawElements(int arg0, int[] arg1, int arg2, int arg3,
			Buffer[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiDrawElementsEXT(int arg0, IntBuffer arg1, int arg2,
			Buffer[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiDrawElementsEXT(int arg0, int[] arg1, int arg2,
			int arg3, Buffer[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiDrawRangeElementArrayAPPLE(int arg0, int arg1, int arg2,
			IntBuffer arg3, IntBuffer arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiDrawRangeElementArrayAPPLE(int arg0, int arg1, int arg2,
			int[] arg3, int arg4, int[] arg5, int arg6, int arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiModeDrawArraysIBM(IntBuffer arg0, IntBuffer arg1,
			IntBuffer arg2, int arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiModeDrawArraysIBM(int[] arg0, int arg1, int[] arg2,
			int arg3, int[] arg4, int arg5, int arg6, int arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiModeDrawElementsIBM(IntBuffer arg0, IntBuffer arg1,
			int arg2, Buffer[] arg3, int arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiModeDrawElementsIBM(int[] arg0, int arg1, int[] arg2,
			int arg3, int arg4, Buffer[] arg5, int arg6, int arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord1d(int arg0, double arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord1dv(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord1dv(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord1f(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord1fv(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord1fv(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord1hNV(int arg0, short arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord1hvNV(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord1hvNV(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord1i(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord1iv(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord1iv(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord1s(int arg0, short arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord1sv(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord1sv(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord2d(int arg0, double arg1, double arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord2dv(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord2dv(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord2f(int arg0, float arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord2fv(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord2fv(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord2hNV(int arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord2hvNV(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord2hvNV(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord2i(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord2iv(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord2iv(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord2s(int arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord2sv(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord2sv(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord3d(int arg0, double arg1, double arg2,
			double arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord3dv(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord3dv(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord3f(int arg0, float arg1, float arg2, float arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord3fv(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord3fv(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord3hNV(int arg0, short arg1, short arg2, short arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord3hvNV(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord3hvNV(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord3i(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord3iv(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord3iv(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord3s(int arg0, short arg1, short arg2, short arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord3sv(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord3sv(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord4d(int arg0, double arg1, double arg2,
			double arg3, double arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord4dv(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord4dv(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord4f(int arg0, float arg1, float arg2, float arg3,
			float arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord4fv(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord4fv(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord4hNV(int arg0, short arg1, short arg2,
			short arg3, short arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord4hvNV(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord4hvNV(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord4i(int arg0, int arg1, int arg2, int arg3,
			int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord4iv(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord4iv(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord4s(int arg0, short arg1, short arg2, short arg3,
			short arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord4sv(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glMultiTexCoord4sv(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glNewBufferRegion(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNewList(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glNewObjectBufferATI(int arg0, Buffer arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormal3b(byte arg0, byte arg1, byte arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormal3bv(ByteBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormal3bv(byte[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormal3d(double arg0, double arg1, double arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormal3dv(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormal3dv(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormal3f(float arg0, float arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormal3fVertex3fSUN(float arg0, float arg1, float arg2,
			float arg3, float arg4, float arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormal3fVertex3fvSUN(FloatBuffer arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormal3fVertex3fvSUN(float[] arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormal3fv(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormal3fv(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormal3hNV(short arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormal3hvNV(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormal3hvNV(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormal3i(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormal3iv(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormal3iv(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormal3s(short arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormal3sv(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormal3sv(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormalPointer(int arg0, int arg1, Buffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormalPointer(int arg0, int arg1, long arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormalStream3bATI(int arg0, byte arg1, byte arg2, byte arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormalStream3bvATI(int arg0, ByteBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormalStream3bvATI(int arg0, byte[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormalStream3dATI(int arg0, double arg1, double arg2,
			double arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormalStream3dvATI(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormalStream3dvATI(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormalStream3fATI(int arg0, float arg1, float arg2, float arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormalStream3fvATI(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormalStream3fvATI(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormalStream3iATI(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormalStream3ivATI(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormalStream3ivATI(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormalStream3sATI(int arg0, short arg1, short arg2, short arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormalStream3svATI(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glNormalStream3svATI(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glOrtho(double arg0, double arg1, double arg2, double arg3,
			double arg4, double arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPNTrianglesfATI(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPNTrianglesiATI(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPassTexCoordATI(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPassThrough(float arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelDataRangeNV(int arg0, int arg1, Buffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelMapfv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelMapfv(int arg0, int arg1, long arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelMapfv(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelMapuiv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelMapuiv(int arg0, int arg1, long arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelMapuiv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelMapusv(int arg0, int arg1, ShortBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelMapusv(int arg0, int arg1, long arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelMapusv(int arg0, int arg1, short[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelStoref(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelStorei(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelTexGenParameterfSGIS(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelTexGenParameterfvSGIS(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelTexGenParameterfvSGIS(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelTexGenParameteriSGIS(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelTexGenParameterivSGIS(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelTexGenParameterivSGIS(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelTexGenSGIX(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelTransferf(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelTransferi(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelTransformParameterfEXT(int arg0, int arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelTransformParameterfvEXT(int arg0, int arg1,
			FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelTransformParameterfvEXT(int arg0, int arg1,
			float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelTransformParameteriEXT(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelTransformParameterivEXT(int arg0, int arg1,
			IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelTransformParameterivEXT(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPixelZoom(float arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPointParameterf(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPointParameterfARB(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPointParameterfEXT(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPointParameterfSGIS(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPointParameterfv(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPointParameterfv(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPointParameterfvARB(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPointParameterfvARB(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPointParameterfvEXT(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPointParameterfvEXT(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPointParameterfvSGIS(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPointParameterfvSGIS(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPointParameteri(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPointParameteriNV(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPointParameteriv(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPointParameteriv(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPointParameterivNV(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPointParameterivNV(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPointSize(float arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glPollAsyncSGIX(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glPollAsyncSGIX(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glPollInstrumentsSGIX(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glPollInstrumentsSGIX(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPolygonMode(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPolygonOffset(float arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPolygonStipple(ByteBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPolygonStipple(long arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPolygonStipple(byte[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPopAttrib() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPopClientAttrib() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPopMatrix() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPopName() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPrimitiveRestartIndexNV(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPrimitiveRestartNV() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPrioritizeTextures(int arg0, IntBuffer arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPrioritizeTextures(int arg0, int[] arg1, int arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramBufferParametersIivNV(int arg0, int arg1, int arg2,
			int arg3, IntBuffer arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramBufferParametersIivNV(int arg0, int arg1, int arg2,
			int arg3, int[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramBufferParametersIuivNV(int arg0, int arg1, int arg2,
			int arg3, IntBuffer arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramBufferParametersIuivNV(int arg0, int arg1, int arg2,
			int arg3, int[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramBufferParametersfvNV(int arg0, int arg1, int arg2,
			int arg3, FloatBuffer arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramBufferParametersfvNV(int arg0, int arg1, int arg2,
			int arg3, float[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramEnvParameter4dARB(int arg0, int arg1, double arg2,
			double arg3, double arg4, double arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramEnvParameter4dvARB(int arg0, int arg1,
			DoubleBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramEnvParameter4dvARB(int arg0, int arg1, double[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramEnvParameter4fARB(int arg0, int arg1, float arg2,
			float arg3, float arg4, float arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramEnvParameter4fvARB(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramEnvParameter4fvARB(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramEnvParameterI4iNV(int arg0, int arg1, int arg2,
			int arg3, int arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramEnvParameterI4ivNV(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramEnvParameterI4ivNV(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramEnvParameterI4uiNV(int arg0, int arg1, int arg2,
			int arg3, int arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramEnvParameterI4uivNV(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramEnvParameterI4uivNV(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramEnvParameters4fvEXT(int arg0, int arg1, int arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramEnvParameters4fvEXT(int arg0, int arg1, int arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramEnvParametersI4ivNV(int arg0, int arg1, int arg2,
			IntBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramEnvParametersI4ivNV(int arg0, int arg1, int arg2,
			int[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramEnvParametersI4uivNV(int arg0, int arg1, int arg2,
			IntBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramEnvParametersI4uivNV(int arg0, int arg1, int arg2,
			int[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramLocalParameter4dARB(int arg0, int arg1, double arg2,
			double arg3, double arg4, double arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramLocalParameter4dvARB(int arg0, int arg1,
			DoubleBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramLocalParameter4dvARB(int arg0, int arg1,
			double[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramLocalParameter4fARB(int arg0, int arg1, float arg2,
			float arg3, float arg4, float arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramLocalParameter4fvARB(int arg0, int arg1,
			FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramLocalParameter4fvARB(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramLocalParameterI4iNV(int arg0, int arg1, int arg2,
			int arg3, int arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramLocalParameterI4ivNV(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramLocalParameterI4ivNV(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramLocalParameterI4uiNV(int arg0, int arg1, int arg2,
			int arg3, int arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramLocalParameterI4uivNV(int arg0, int arg1,
			IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramLocalParameterI4uivNV(int arg0, int arg1, int[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramLocalParameters4fvEXT(int arg0, int arg1, int arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramLocalParameters4fvEXT(int arg0, int arg1, int arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramLocalParametersI4ivNV(int arg0, int arg1, int arg2,
			IntBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramLocalParametersI4ivNV(int arg0, int arg1, int arg2,
			int[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramLocalParametersI4uivNV(int arg0, int arg1, int arg2,
			IntBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramLocalParametersI4uivNV(int arg0, int arg1, int arg2,
			int[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramNamedParameter4dNV(int arg0, int arg1, String arg2,
			double arg3, double arg4, double arg5, double arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramNamedParameter4dvNV(int arg0, int arg1, String arg2,
			DoubleBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramNamedParameter4dvNV(int arg0, int arg1, String arg2,
			double[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramNamedParameter4fNV(int arg0, int arg1, String arg2,
			float arg3, float arg4, float arg5, float arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramNamedParameter4fvNV(int arg0, int arg1, String arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramNamedParameter4fvNV(int arg0, int arg1, String arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramParameter4dNV(int arg0, int arg1, double arg2,
			double arg3, double arg4, double arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramParameter4dvNV(int arg0, int arg1, DoubleBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramParameter4dvNV(int arg0, int arg1, double[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramParameter4fNV(int arg0, int arg1, float arg2,
			float arg3, float arg4, float arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramParameter4fvNV(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramParameter4fvNV(int arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramParameteriEXT(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramParameters4dvNV(int arg0, int arg1, int arg2,
			DoubleBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramParameters4dvNV(int arg0, int arg1, int arg2,
			double[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramParameters4fvNV(int arg0, int arg1, int arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramParameters4fvNV(int arg0, int arg1, int arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramStringARB(int arg0, int arg1, int arg2, String arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glProgramVertexLimitNV(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPushAttrib(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPushClientAttrib(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPushMatrix() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glPushName(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos2d(double arg0, double arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos2dv(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos2dv(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos2f(float arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos2fv(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos2fv(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos2i(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos2iv(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos2iv(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos2s(short arg0, short arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos2sv(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos2sv(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos3d(double arg0, double arg1, double arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos3dv(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos3dv(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos3f(float arg0, float arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos3fv(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos3fv(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos3i(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos3iv(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos3iv(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos3s(short arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos3sv(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos3sv(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos4d(double arg0, double arg1, double arg2, double arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos4dv(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos4dv(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos4f(float arg0, float arg1, float arg2, float arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos4fv(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos4fv(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos4i(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos4iv(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos4iv(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos4s(short arg0, short arg1, short arg2, short arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos4sv(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRasterPos4sv(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReadBuffer(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReadBufferRegion(int arg0, int arg1, int arg2, int arg3,
			int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReadInstrumentsSGIX(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReadPixels(int arg0, int arg1, int arg2, int arg3, int arg4,
			int arg5, Buffer arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReadPixels(int arg0, int arg1, int arg2, int arg3, int arg4,
			int arg5, long arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRectd(double arg0, double arg1, double arg2, double arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRectdv(DoubleBuffer arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRectdv(double[] arg0, int arg1, double[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRectf(float arg0, float arg1, float arg2, float arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRectfv(FloatBuffer arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRectfv(float[] arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRecti(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRectiv(IntBuffer arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRectiv(int[] arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRects(short arg0, short arg1, short arg2, short arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRectsv(ShortBuffer arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRectsv(short[] arg0, int arg1, short[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReferencePlaneSGIX(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReferencePlaneSGIX(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public int glRenderMode(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRenderbufferStorageEXT(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRenderbufferStorageMultisampleCoverageNV(int arg0, int arg1,
			int arg2, int arg3, int arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRenderbufferStorageMultisampleEXT(int arg0, int arg1,
			int arg2, int arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiColor3fVertex3fSUN(int arg0, float arg1,
			float arg2, float arg3, float arg4, float arg5, float arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiColor3fVertex3fvSUN(IntBuffer arg0,
			FloatBuffer arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiColor3fVertex3fvSUN(int[] arg0, int arg1,
			float[] arg2, int arg3, float[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiColor4fNormal3fVertex3fSUN(int arg0,
			float arg1, float arg2, float arg3, float arg4, float arg5,
			float arg6, float arg7, float arg8, float arg9, float arg10) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiColor4fNormal3fVertex3fvSUN(IntBuffer arg0,
			FloatBuffer arg1, FloatBuffer arg2, FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiColor4fNormal3fVertex3fvSUN(int[] arg0,
			int arg1, float[] arg2, int arg3, float[] arg4, int arg5,
			float[] arg6, int arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiColor4ubVertex3fSUN(int arg0, byte arg1,
			byte arg2, byte arg3, byte arg4, float arg5, float arg6, float arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiColor4ubVertex3fvSUN(IntBuffer arg0,
			ByteBuffer arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiColor4ubVertex3fvSUN(int[] arg0, int arg1,
			byte[] arg2, int arg3, float[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiNormal3fVertex3fSUN(int arg0, float arg1,
			float arg2, float arg3, float arg4, float arg5, float arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiNormal3fVertex3fvSUN(IntBuffer arg0,
			FloatBuffer arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiNormal3fVertex3fvSUN(int[] arg0, int arg1,
			float[] arg2, int arg3, float[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN(
			int arg0, float arg1, float arg2, float arg3, float arg4,
			float arg5, float arg6, float arg7, float arg8, float arg9,
			float arg10, float arg11, float arg12) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN(
			IntBuffer arg0, FloatBuffer arg1, FloatBuffer arg2,
			FloatBuffer arg3, FloatBuffer arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN(
			int[] arg0, int arg1, float[] arg2, int arg3, float[] arg4,
			int arg5, float[] arg6, int arg7, float[] arg8, int arg9) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN(int arg0,
			float arg1, float arg2, float arg3, float arg4, float arg5,
			float arg6, float arg7, float arg8) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN(
			IntBuffer arg0, FloatBuffer arg1, FloatBuffer arg2, FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN(int[] arg0,
			int arg1, float[] arg2, int arg3, float[] arg4, int arg5,
			float[] arg6, int arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiTexCoord2fVertex3fSUN(int arg0, float arg1,
			float arg2, float arg3, float arg4, float arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiTexCoord2fVertex3fvSUN(IntBuffer arg0,
			FloatBuffer arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiTexCoord2fVertex3fvSUN(int[] arg0, int arg1,
			float[] arg2, int arg3, float[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiVertex3fSUN(int arg0, float arg1,
			float arg2, float arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiVertex3fvSUN(IntBuffer arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glReplacementCodeuiVertex3fvSUN(int[] arg0, int arg1,
			float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRequestResidentProgramsNV(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRequestResidentProgramsNV(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glResetHistogram(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glResetMinmax(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glResizeBuffersMESA() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRotated(double arg0, double arg1, double arg2, double arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glRotatef(float arg0, float arg1, float arg2, float arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSampleCoverage(float arg0, boolean arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSampleMapATI(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSampleMaskEXT(float arg0, boolean arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSampleMaskSGIS(float arg0, boolean arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSamplePatternEXT(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSamplePatternSGIS(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glScaled(double arg0, double arg1, double arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glScalef(float arg0, float arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glScissor(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3b(byte arg0, byte arg1, byte arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3bEXT(byte arg0, byte arg1, byte arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3bv(ByteBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3bv(byte[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3bvEXT(ByteBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3bvEXT(byte[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3d(double arg0, double arg1, double arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3dEXT(double arg0, double arg1, double arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3dv(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3dv(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3dvEXT(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3dvEXT(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3f(float arg0, float arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3fEXT(float arg0, float arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3fv(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3fv(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3fvEXT(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3fvEXT(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3hNV(short arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3hvNV(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3hvNV(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3i(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3iEXT(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3iv(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3iv(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3ivEXT(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3ivEXT(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3s(short arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3sEXT(short arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3sv(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3sv(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3svEXT(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3svEXT(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3ub(byte arg0, byte arg1, byte arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3ubEXT(byte arg0, byte arg1, byte arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3ubv(ByteBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3ubv(byte[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3ubvEXT(ByteBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3ubvEXT(byte[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3ui(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3uiEXT(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3uiv(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3uiv(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3uivEXT(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3uivEXT(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3us(short arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3usEXT(short arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3usv(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3usv(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3usvEXT(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColor3usvEXT(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColorPointer(int arg0, int arg1, int arg2,
			Buffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColorPointer(int arg0, int arg1, int arg2, long arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColorPointerEXT(int arg0, int arg1, int arg2,
			Buffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSecondaryColorPointerEXT(int arg0, int arg1, int arg2,
			long arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSelectBuffer(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSeparableFilter2D(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, Buffer arg6, Buffer arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSeparableFilter2D(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, long arg6, long arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSetFenceAPPLE(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSetFenceNV(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSetFragmentShaderConstantATI(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSetFragmentShaderConstantATI(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSetInvariantEXT(int arg0, int arg1, Buffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSetLocalConstantEXT(int arg0, int arg1, Buffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glShadeModel(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glShaderOp1EXT(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glShaderOp2EXT(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glShaderOp3EXT(int arg0, int arg1, int arg2, int arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glShaderSource(int arg0, int arg1, String[] arg2, IntBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glShaderSource(int arg0, int arg1, String[] arg2, int[] arg3,
			int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glShaderSourceARB(int arg0, int arg1, String[] arg2,
			IntBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glShaderSourceARB(int arg0, int arg1, String[] arg2,
			int[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSharpenTexFuncSGIS(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSharpenTexFuncSGIS(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSpriteParameterfSGIX(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSpriteParameterfvSGIX(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSpriteParameterfvSGIX(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSpriteParameteriSGIX(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSpriteParameterivSGIX(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSpriteParameterivSGIX(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glStartInstrumentsSGIX() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glStencilClearTagEXT(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glStencilFunc(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glStencilFuncSeparate(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glStencilFuncSeparateATI(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glStencilMask(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glStencilMaskSeparate(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glStencilOp(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glStencilOpSeparate(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glStencilOpSeparateATI(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glStopInstrumentsSGIX(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glStringMarkerGREMEDY(int arg0, Buffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSwapAPPLE() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glSwizzleEXT(int arg0, int arg1, int arg2, int arg3, int arg4,
			int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTagSampleBufferSGIX() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTbufferMask3DFX(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glTestFenceAPPLE(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glTestFenceNV(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glTestObjectAPPLE(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexBufferEXT(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexBumpParameterfvATI(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexBumpParameterfvATI(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexBumpParameterivATI(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexBumpParameterivATI(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord1d(double arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord1dv(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord1dv(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord1f(float arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord1fv(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord1fv(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord1hNV(short arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord1hvNV(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord1hvNV(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord1i(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord1iv(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord1iv(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord1s(short arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord1sv(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord1sv(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2d(double arg0, double arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2dv(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2dv(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2f(float arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2fColor3fVertex3fSUN(float arg0, float arg1,
			float arg2, float arg3, float arg4, float arg5, float arg6,
			float arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2fColor3fVertex3fvSUN(FloatBuffer arg0,
			FloatBuffer arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2fColor3fVertex3fvSUN(float[] arg0, int arg1,
			float[] arg2, int arg3, float[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2fColor4fNormal3fVertex3fSUN(float arg0, float arg1,
			float arg2, float arg3, float arg4, float arg5, float arg6,
			float arg7, float arg8, float arg9, float arg10, float arg11) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2fColor4fNormal3fVertex3fvSUN(FloatBuffer arg0,
			FloatBuffer arg1, FloatBuffer arg2, FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2fColor4fNormal3fVertex3fvSUN(float[] arg0, int arg1,
			float[] arg2, int arg3, float[] arg4, int arg5, float[] arg6,
			int arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2fColor4ubVertex3fSUN(float arg0, float arg1,
			byte arg2, byte arg3, byte arg4, byte arg5, float arg6, float arg7,
			float arg8) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2fColor4ubVertex3fvSUN(FloatBuffer arg0,
			ByteBuffer arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2fColor4ubVertex3fvSUN(float[] arg0, int arg1,
			byte[] arg2, int arg3, float[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2fNormal3fVertex3fSUN(float arg0, float arg1,
			float arg2, float arg3, float arg4, float arg5, float arg6,
			float arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2fNormal3fVertex3fvSUN(FloatBuffer arg0,
			FloatBuffer arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2fNormal3fVertex3fvSUN(float[] arg0, int arg1,
			float[] arg2, int arg3, float[] arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2fVertex3fSUN(float arg0, float arg1, float arg2,
			float arg3, float arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2fVertex3fvSUN(FloatBuffer arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2fVertex3fvSUN(float[] arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2fv(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2fv(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2hNV(short arg0, short arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2hvNV(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2hvNV(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2i(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2iv(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2iv(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2s(short arg0, short arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2sv(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord2sv(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord3d(double arg0, double arg1, double arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord3dv(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord3dv(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord3f(float arg0, float arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord3fv(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord3fv(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord3hNV(short arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord3hvNV(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord3hvNV(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord3i(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord3iv(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord3iv(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord3s(short arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord3sv(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord3sv(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord4d(double arg0, double arg1, double arg2, double arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord4dv(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord4dv(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord4f(float arg0, float arg1, float arg2, float arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord4fColor4fNormal3fVertex4fSUN(float arg0, float arg1,
			float arg2, float arg3, float arg4, float arg5, float arg6,
			float arg7, float arg8, float arg9, float arg10, float arg11,
			float arg12, float arg13, float arg14) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord4fColor4fNormal3fVertex4fvSUN(FloatBuffer arg0,
			FloatBuffer arg1, FloatBuffer arg2, FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord4fColor4fNormal3fVertex4fvSUN(float[] arg0, int arg1,
			float[] arg2, int arg3, float[] arg4, int arg5, float[] arg6,
			int arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord4fVertex4fSUN(float arg0, float arg1, float arg2,
			float arg3, float arg4, float arg5, float arg6, float arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord4fVertex4fvSUN(FloatBuffer arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord4fVertex4fvSUN(float[] arg0, int arg1, float[] arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord4fv(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord4fv(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord4hNV(short arg0, short arg1, short arg2, short arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord4hvNV(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord4hvNV(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord4i(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord4iv(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord4iv(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord4s(short arg0, short arg1, short arg2, short arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord4sv(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoord4sv(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoordPointer(int arg0, int arg1, int arg2, Buffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexCoordPointer(int arg0, int arg1, int arg2, long arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexEnvf(int arg0, int arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexEnvfv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexEnvfv(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexEnvi(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexEnviv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexEnviv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexFilterFuncSGIS(int arg0, int arg1, int arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexFilterFuncSGIS(int arg0, int arg1, int arg2, float[] arg3,
			int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexGend(int arg0, int arg1, double arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexGendv(int arg0, int arg1, DoubleBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexGendv(int arg0, int arg1, double[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexGenf(int arg0, int arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexGenfv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexGenfv(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexGeni(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexGeniv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexGeniv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexImage1D(int arg0, int arg1, int arg2, int arg3, int arg4,
			int arg5, int arg6, Buffer arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexImage1D(int arg0, int arg1, int arg2, int arg3, int arg4,
			int arg5, int arg6, long arg7) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexImage2D(int arg0, int arg1, int arg2, int arg3, int arg4,
			int arg5, int arg6, int arg7, Buffer arg8) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexImage2D(int arg0, int arg1, int arg2, int arg3, int arg4,
			int arg5, int arg6, int arg7, long arg8) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexImage3D(int arg0, int arg1, int arg2, int arg3, int arg4,
			int arg5, int arg6, int arg7, int arg8, Buffer arg9) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexImage3D(int arg0, int arg1, int arg2, int arg3, int arg4,
			int arg5, int arg6, int arg7, int arg8, long arg9) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexImage4DSGIS(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, int arg7, int arg8, int arg9,
			Buffer arg10) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexParameterIivEXT(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexParameterIivEXT(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexParameterIuivEXT(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexParameterIuivEXT(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexParameterf(int arg0, int arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexParameterfv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexParameterfv(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexParameteri(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexParameteriv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexParameteriv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexSubImage1D(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, Buffer arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexSubImage1D(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, long arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexSubImage2D(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, int arg7, Buffer arg8) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexSubImage2D(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, int arg7, long arg8) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexSubImage3D(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, int arg7, int arg8, int arg9,
			Buffer arg10) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexSubImage3D(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, int arg7, int arg8, int arg9,
			long arg10) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTexSubImage4DSGIS(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5, int arg6, int arg7, int arg8, int arg9,
			int arg10, int arg11, Buffer arg12) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTextureColorMaskSGIS(boolean arg0, boolean arg1,
			boolean arg2, boolean arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTextureLightEXT(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTextureMaterialEXT(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTextureNormalEXT(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTextureRangeAPPLE(int arg0, int arg1, Buffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTrackMatrixNV(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTransformFeedbackAttribsNV(int arg0, IntBuffer arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTransformFeedbackAttribsNV(int arg0, int[] arg1, int arg2,
			int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTransformFeedbackVaryingsNV(int arg0, int arg1,
			IntBuffer arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTransformFeedbackVaryingsNV(int arg0, int arg1, int[] arg2,
			int arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTranslated(double arg0, double arg1, double arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glTranslatef(float arg0, float arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform1f(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform1fARB(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform1fv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform1fv(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform1fvARB(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform1fvARB(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform1i(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform1iARB(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform1iv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform1iv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform1ivARB(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform1ivARB(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform1uiEXT(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform1uivEXT(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform1uivEXT(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform2f(int arg0, float arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform2fARB(int arg0, float arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform2fv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform2fv(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform2fvARB(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform2fvARB(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform2i(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform2iARB(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform2iv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform2iv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform2ivARB(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform2ivARB(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform2uiEXT(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform2uivEXT(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform2uivEXT(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform3f(int arg0, float arg1, float arg2, float arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform3fARB(int arg0, float arg1, float arg2, float arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform3fv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform3fv(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform3fvARB(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform3fvARB(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform3i(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform3iARB(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform3iv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform3iv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform3ivARB(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform3ivARB(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform3uiEXT(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform3uivEXT(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform3uivEXT(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform4f(int arg0, float arg1, float arg2, float arg3,
			float arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform4fARB(int arg0, float arg1, float arg2, float arg3,
			float arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform4fv(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform4fv(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform4fvARB(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform4fvARB(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform4i(int arg0, int arg1, int arg2, int arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform4iARB(int arg0, int arg1, int arg2, int arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform4iv(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform4iv(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform4ivARB(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform4ivARB(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform4uiEXT(int arg0, int arg1, int arg2, int arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform4uivEXT(int arg0, int arg1, IntBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniform4uivEXT(int arg0, int arg1, int[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformBufferEXT(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix2fv(int arg0, int arg1, boolean arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix2fv(int arg0, int arg1, boolean arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix2fvARB(int arg0, int arg1, boolean arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix2fvARB(int arg0, int arg1, boolean arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix2x3fv(int arg0, int arg1, boolean arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix2x3fv(int arg0, int arg1, boolean arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix2x4fv(int arg0, int arg1, boolean arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix2x4fv(int arg0, int arg1, boolean arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix3fv(int arg0, int arg1, boolean arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix3fv(int arg0, int arg1, boolean arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix3fvARB(int arg0, int arg1, boolean arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix3fvARB(int arg0, int arg1, boolean arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix3x2fv(int arg0, int arg1, boolean arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix3x2fv(int arg0, int arg1, boolean arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix3x4fv(int arg0, int arg1, boolean arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix3x4fv(int arg0, int arg1, boolean arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix4fv(int arg0, int arg1, boolean arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix4fv(int arg0, int arg1, boolean arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix4fvARB(int arg0, int arg1, boolean arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix4fvARB(int arg0, int arg1, boolean arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix4x2fv(int arg0, int arg1, boolean arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix4x2fv(int arg0, int arg1, boolean arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix4x3fv(int arg0, int arg1, boolean arg2,
			FloatBuffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUniformMatrix4x3fv(int arg0, int arg1, boolean arg2,
			float[] arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUnlockArraysEXT() {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glUnmapBuffer(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean glUnmapBufferARB(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUpdateObjectBufferATI(int arg0, int arg1, int arg2,
			Buffer arg3, int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUseProgram(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glUseProgramObjectARB(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glValidateProgram(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glValidateProgramARB(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVariantArrayObjectATI(int arg0, int arg1, int arg2, int arg3,
			int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVariantPointerEXT(int arg0, int arg1, int arg2, Buffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVariantPointerEXT(int arg0, int arg1, int arg2, long arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVariantbvEXT(int arg0, ByteBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVariantbvEXT(int arg0, byte[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVariantdvEXT(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVariantdvEXT(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVariantfvEXT(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVariantfvEXT(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVariantivEXT(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVariantivEXT(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVariantsvEXT(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVariantsvEXT(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVariantubvEXT(int arg0, ByteBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVariantubvEXT(int arg0, byte[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVariantuivEXT(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVariantuivEXT(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVariantusvEXT(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVariantusvEXT(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex2d(double arg0, double arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex2dv(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex2dv(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex2f(float arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex2fv(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex2fv(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex2hNV(short arg0, short arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex2hvNV(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex2hvNV(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex2i(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex2iv(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex2iv(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex2s(short arg0, short arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex2sv(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex2sv(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex3d(double arg0, double arg1, double arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex3dv(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex3dv(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex3f(float arg0, float arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex3fv(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex3fv(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex3hNV(short arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex3hvNV(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex3hvNV(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex3i(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex3iv(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex3iv(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex3s(short arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex3sv(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex3sv(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex4d(double arg0, double arg1, double arg2, double arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex4dv(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex4dv(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex4f(float arg0, float arg1, float arg2, float arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex4fv(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex4fv(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex4hNV(short arg0, short arg1, short arg2, short arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex4hvNV(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex4hvNV(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex4i(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex4iv(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex4iv(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex4s(short arg0, short arg1, short arg2, short arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex4sv(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertex4sv(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexArrayParameteriAPPLE(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexArrayRangeAPPLE(int arg0, Buffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexArrayRangeNV(int arg0, Buffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1d(int arg0, double arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1dARB(int arg0, double arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1dNV(int arg0, double arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1dv(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1dv(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1dvARB(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1dvARB(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1dvNV(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1dvNV(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1f(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1fARB(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1fNV(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1fv(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1fv(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1fvARB(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1fvARB(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1fvNV(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1fvNV(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1hNV(int arg0, short arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1hvNV(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1hvNV(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1s(int arg0, short arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1sARB(int arg0, short arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1sNV(int arg0, short arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1sv(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1sv(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1svARB(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1svARB(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1svNV(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib1svNV(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2d(int arg0, double arg1, double arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2dARB(int arg0, double arg1, double arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2dNV(int arg0, double arg1, double arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2dv(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2dv(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2dvARB(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2dvARB(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2dvNV(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2dvNV(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2f(int arg0, float arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2fARB(int arg0, float arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2fNV(int arg0, float arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2fv(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2fv(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2fvARB(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2fvARB(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2fvNV(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2fvNV(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2hNV(int arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2hvNV(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2hvNV(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2s(int arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2sARB(int arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2sNV(int arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2sv(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2sv(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2svARB(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2svARB(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2svNV(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib2svNV(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3d(int arg0, double arg1, double arg2, double arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3dARB(int arg0, double arg1, double arg2,
			double arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3dNV(int arg0, double arg1, double arg2,
			double arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3dv(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3dv(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3dvARB(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3dvARB(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3dvNV(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3dvNV(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3f(int arg0, float arg1, float arg2, float arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3fARB(int arg0, float arg1, float arg2, float arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3fNV(int arg0, float arg1, float arg2, float arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3fv(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3fv(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3fvARB(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3fvARB(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3fvNV(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3fvNV(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3hNV(int arg0, short arg1, short arg2, short arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3hvNV(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3hvNV(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3s(int arg0, short arg1, short arg2, short arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3sARB(int arg0, short arg1, short arg2, short arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3sNV(int arg0, short arg1, short arg2, short arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3sv(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3sv(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3svARB(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3svARB(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3svNV(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib3svNV(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4Nbv(int arg0, ByteBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4Nbv(int arg0, byte[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4NbvARB(int arg0, ByteBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4NbvARB(int arg0, byte[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4Niv(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4Niv(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4NivARB(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4NivARB(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4Nsv(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4Nsv(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4NsvARB(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4NsvARB(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4Nub(int arg0, byte arg1, byte arg2, byte arg3,
			byte arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4NubARB(int arg0, byte arg1, byte arg2,
			byte arg3, byte arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4Nubv(int arg0, ByteBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4Nubv(int arg0, byte[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4NubvARB(int arg0, ByteBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4NubvARB(int arg0, byte[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4Nuiv(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4Nuiv(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4NuivARB(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4NuivARB(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4Nusv(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4Nusv(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4NusvARB(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4NusvARB(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4bv(int arg0, ByteBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4bv(int arg0, byte[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4bvARB(int arg0, ByteBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4bvARB(int arg0, byte[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4d(int arg0, double arg1, double arg2,
			double arg3, double arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4dARB(int arg0, double arg1, double arg2,
			double arg3, double arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4dNV(int arg0, double arg1, double arg2,
			double arg3, double arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4dv(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4dv(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4dvARB(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4dvARB(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4dvNV(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4dvNV(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4f(int arg0, float arg1, float arg2, float arg3,
			float arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4fARB(int arg0, float arg1, float arg2,
			float arg3, float arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4fNV(int arg0, float arg1, float arg2,
			float arg3, float arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4fv(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4fv(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4fvARB(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4fvARB(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4fvNV(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4fvNV(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4hNV(int arg0, short arg1, short arg2,
			short arg3, short arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4hvNV(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4hvNV(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4iv(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4iv(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4ivARB(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4ivARB(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4s(int arg0, short arg1, short arg2, short arg3,
			short arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4sARB(int arg0, short arg1, short arg2,
			short arg3, short arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4sNV(int arg0, short arg1, short arg2,
			short arg3, short arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4sv(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4sv(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4svARB(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4svARB(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4svNV(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4svNV(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4ubNV(int arg0, byte arg1, byte arg2, byte arg3,
			byte arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4ubv(int arg0, ByteBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4ubv(int arg0, byte[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4ubvARB(int arg0, ByteBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4ubvARB(int arg0, byte[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4ubvNV(int arg0, ByteBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4ubvNV(int arg0, byte[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4uiv(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4uiv(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4uivARB(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4uivARB(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4usv(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4usv(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4usvARB(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttrib4usvARB(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribArrayObjectATI(int arg0, int arg1, int arg2,
			boolean arg3, int arg4, int arg5, int arg6) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI1iEXT(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI1ivEXT(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI1ivEXT(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI1uiEXT(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI1uivEXT(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI1uivEXT(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI2iEXT(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI2ivEXT(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI2ivEXT(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI2uiEXT(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI2uivEXT(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI2uivEXT(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI3iEXT(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI3ivEXT(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI3ivEXT(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI3uiEXT(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI3uivEXT(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI3uivEXT(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI4bvEXT(int arg0, ByteBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI4bvEXT(int arg0, byte[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI4iEXT(int arg0, int arg1, int arg2, int arg3,
			int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI4ivEXT(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI4ivEXT(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI4svEXT(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI4svEXT(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI4ubvEXT(int arg0, ByteBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI4ubvEXT(int arg0, byte[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI4uiEXT(int arg0, int arg1, int arg2, int arg3,
			int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI4uivEXT(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI4uivEXT(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI4usvEXT(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribI4usvEXT(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribIPointerEXT(int arg0, int arg1, int arg2,
			int arg3, Buffer arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribPointer(int arg0, int arg1, int arg2,
			boolean arg3, int arg4, Buffer arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribPointer(int arg0, int arg1, int arg2,
			boolean arg3, int arg4, long arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribPointerARB(int arg0, int arg1, int arg2,
			boolean arg3, int arg4, Buffer arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribPointerARB(int arg0, int arg1, int arg2,
			boolean arg3, int arg4, long arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribPointerNV(int arg0, int arg1, int arg2, int arg3,
			Buffer arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribPointerNV(int arg0, int arg1, int arg2, int arg3,
			long arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs1dvNV(int arg0, int arg1, DoubleBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs1dvNV(int arg0, int arg1, double[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs1fvNV(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs1fvNV(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs1hvNV(int arg0, int arg1, ShortBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs1hvNV(int arg0, int arg1, short[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs1svNV(int arg0, int arg1, ShortBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs1svNV(int arg0, int arg1, short[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs2dvNV(int arg0, int arg1, DoubleBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs2dvNV(int arg0, int arg1, double[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs2fvNV(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs2fvNV(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs2hvNV(int arg0, int arg1, ShortBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs2hvNV(int arg0, int arg1, short[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs2svNV(int arg0, int arg1, ShortBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs2svNV(int arg0, int arg1, short[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs3dvNV(int arg0, int arg1, DoubleBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs3dvNV(int arg0, int arg1, double[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs3fvNV(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs3fvNV(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs3hvNV(int arg0, int arg1, ShortBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs3hvNV(int arg0, int arg1, short[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs3svNV(int arg0, int arg1, ShortBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs3svNV(int arg0, int arg1, short[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs4dvNV(int arg0, int arg1, DoubleBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs4dvNV(int arg0, int arg1, double[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs4fvNV(int arg0, int arg1, FloatBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs4fvNV(int arg0, int arg1, float[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs4hvNV(int arg0, int arg1, ShortBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs4hvNV(int arg0, int arg1, short[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs4svNV(int arg0, int arg1, ShortBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs4svNV(int arg0, int arg1, short[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs4ubvNV(int arg0, int arg1, ByteBuffer arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexAttribs4ubvNV(int arg0, int arg1, byte[] arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexBlendARB(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexBlendEnvfATI(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexBlendEnviATI(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexPointer(int arg0, int arg1, int arg2, Buffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexPointer(int arg0, int arg1, int arg2, long arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream1dATI(int arg0, double arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream1dvATI(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream1dvATI(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream1fATI(int arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream1fvATI(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream1fvATI(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream1iATI(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream1ivATI(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream1ivATI(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream1sATI(int arg0, short arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream1svATI(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream1svATI(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream2dATI(int arg0, double arg1, double arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream2dvATI(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream2dvATI(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream2fATI(int arg0, float arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream2fvATI(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream2fvATI(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream2iATI(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream2ivATI(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream2ivATI(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream2sATI(int arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream2svATI(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream2svATI(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream3dATI(int arg0, double arg1, double arg2,
			double arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream3dvATI(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream3dvATI(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream3fATI(int arg0, float arg1, float arg2, float arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream3fvATI(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream3fvATI(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream3iATI(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream3ivATI(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream3ivATI(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream3sATI(int arg0, short arg1, short arg2, short arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream3svATI(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream3svATI(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream4dATI(int arg0, double arg1, double arg2,
			double arg3, double arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream4dvATI(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream4dvATI(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream4fATI(int arg0, float arg1, float arg2,
			float arg3, float arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream4fvATI(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream4fvATI(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream4iATI(int arg0, int arg1, int arg2, int arg3,
			int arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream4ivATI(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream4ivATI(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream4sATI(int arg0, short arg1, short arg2,
			short arg3, short arg4) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream4svATI(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexStream4svATI(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexWeightPointerEXT(int arg0, int arg1, int arg2,
			Buffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexWeightPointerEXT(int arg0, int arg1, int arg2, long arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexWeightfEXT(float arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexWeightfvEXT(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexWeightfvEXT(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexWeighthNV(short arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexWeighthvNV(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glVertexWeighthvNV(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glViewport(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWeightPointerARB(int arg0, int arg1, int arg2, Buffer arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWeightPointerARB(int arg0, int arg1, int arg2, long arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWeightbvARB(int arg0, ByteBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWeightbvARB(int arg0, byte[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWeightdvARB(int arg0, DoubleBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWeightdvARB(int arg0, double[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWeightfvARB(int arg0, FloatBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWeightfvARB(int arg0, float[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWeightivARB(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWeightivARB(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWeightsvARB(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWeightsvARB(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWeightubvARB(int arg0, ByteBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWeightubvARB(int arg0, byte[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWeightuivARB(int arg0, IntBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWeightuivARB(int arg0, int[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWeightusvARB(int arg0, ShortBuffer arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWeightusvARB(int arg0, short[] arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2d(double arg0, double arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2dARB(double arg0, double arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2dMESA(double arg0, double arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2dv(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2dv(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2dvARB(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2dvARB(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2dvMESA(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2dvMESA(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2f(float arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2fARB(float arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2fMESA(float arg0, float arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2fv(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2fv(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2fvARB(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2fvARB(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2fvMESA(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2fvMESA(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2i(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2iARB(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2iMESA(int arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2iv(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2iv(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2ivARB(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2ivARB(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2ivMESA(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2ivMESA(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2s(short arg0, short arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2sARB(short arg0, short arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2sMESA(short arg0, short arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2sv(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2sv(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2svARB(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2svARB(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2svMESA(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos2svMESA(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3d(double arg0, double arg1, double arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3dARB(double arg0, double arg1, double arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3dMESA(double arg0, double arg1, double arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3dv(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3dv(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3dvARB(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3dvARB(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3dvMESA(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3dvMESA(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3f(float arg0, float arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3fARB(float arg0, float arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3fMESA(float arg0, float arg1, float arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3fv(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3fv(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3fvARB(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3fvARB(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3fvMESA(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3fvMESA(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3i(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3iARB(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3iMESA(int arg0, int arg1, int arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3iv(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3iv(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3ivARB(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3ivARB(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3ivMESA(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3ivMESA(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3s(short arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3sARB(short arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3sMESA(short arg0, short arg1, short arg2) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3sv(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3sv(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3svARB(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3svARB(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3svMESA(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos3svMESA(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos4dMESA(double arg0, double arg1, double arg2,
			double arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos4dvMESA(DoubleBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos4dvMESA(double[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos4fMESA(float arg0, float arg1, float arg2, float arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos4fvMESA(FloatBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos4fvMESA(float[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos4iMESA(int arg0, int arg1, int arg2, int arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos4ivMESA(IntBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos4ivMESA(int[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos4sMESA(short arg0, short arg1, short arg2, short arg3) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos4svMESA(ShortBuffer arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWindowPos4svMESA(short[] arg0, int arg1) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void glWriteMaskEXT(int arg0, int arg1, int arg2, int arg3,
			int arg4, int arg5) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean isExtensionAvailable(String arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public boolean isFunctionAvailable(String arg0) {
		throw new UnsupportedOperationException();
		
		
	}

	
	public void setSwapInterval(int arg0) {
		throw new UnsupportedOperationException();
		
		
	}

}
