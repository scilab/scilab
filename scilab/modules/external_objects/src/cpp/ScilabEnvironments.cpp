/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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
