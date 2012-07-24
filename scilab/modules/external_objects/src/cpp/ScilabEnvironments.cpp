/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ScilabEnvironments.hxx"

namespace org_modules_external_objects
{

std::vector<ScilabAbstractEnvironment *> ScilabEnvironments::environments = std::vector<ScilabAbstractEnvironment *>();

int ScilabEnvironments::registerScilabEnvironment(ScilabAbstractEnvironment * env)
{
    int i = 0;
    for (; i < environments.size(); i++)
    {
        if (environments[i] == env)
        {
            return i;
        }
        else if (!environments[i])
        {
            environments[i] = env;
            return i;
        }
    }

    environments.push_back(env);

    return i;
}

void ScilabEnvironments::unregisterScilabEnvironment(const int id)
{
    if (id >= 0 && id < environments.size())
    {
        environments[id] = 0;
    }
}

ScilabAbstractEnvironment & ScilabEnvironments::getEnvironment(const int id)
{
    if (id >= 0 && id < environments.size() && environments[id])
    {
        return *(environments[id]);
    }

    throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid environment: identifier %d is invalid"), id);
}
}
