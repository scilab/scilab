/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include <jni.h>

extern "C"
{
#include "dap_interface.h"
#include "lsp_interface.h"
}

namespace
{
class JavaString
{
public:
    JavaString(JNIEnv* env, jstring value) : m_env(env), m_value(value)
    {
        if (m_value)
        {
            m_chars = m_env->GetStringUTFChars(m_value, nullptr);
        }
    }

    ~JavaString()
    {
        if (m_chars)
        {
            m_env->ReleaseStringUTFChars(m_value, m_chars);
        }
    }

    const char* c_str() const
    {
        return m_chars ? m_chars : "";
    }

private:
    JNIEnv* m_env;
    jstring m_value;
    const char* m_chars = nullptr;
};
}

extern "C"
{
JNIEXPORT void JNICALL Java_org_scilab_modules_scinotes_SciNotesLspJNI_startLspWebSocket(JNIEnv* env, jclass, jint port, jstring token, jboolean spawned)
{
    JavaString tokenString(env, token);
    lspStartWs(static_cast<int>(port), tokenString.c_str(), spawned == JNI_TRUE);
}

JNIEXPORT void JNICALL Java_org_scilab_modules_scinotes_SciNotesLspJNI_startDapWebSocket(JNIEnv* env, jclass, jint port, jstring token, jboolean spawned)
{
    JavaString tokenString(env, token);
    dapStartWs(static_cast<int>(port), tokenString.c_str(), spawned == JNI_TRUE);
}

JNIEXPORT void JNICALL Java_org_scilab_modules_scinotes_SciNotesLspJNI_indexDocument(JNIEnv* env, jclass, jstring uri, jstring text, jint version)
{
    JavaString uriString(env, uri);
    JavaString textString(env, text);
    lspIndexDocument(uriString.c_str(), textString.c_str(), static_cast<int>(version));
}

JNIEXPORT void JNICALL Java_org_scilab_modules_scinotes_SciNotesLspJNI_removeIndexedDocument(JNIEnv* env, jclass, jstring uri)
{
    JavaString uriString(env, uri);
    lspRemoveIndexedDocument(uriString.c_str());
}
}
