/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) Scilab-Enterprises - 2016 - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __SCIMPICOMM_HXX__
#define __SCIMPICOMM_HXX__

// avoid c++ openmpi include
#define OMPI_SKIP_MPICXX

#include <string>
#include <mpi.h>
#include "user.hxx"

extern "C"
{
#include "dynlib_mpi.h"
}

class MPI_IMPEXP SciMpiComm : public types::UserType
{

public :
    SciMpiComm(int* piRanks, int iSize)
    {
        m_iGroupSize = iSize;
        m_piWorldIndex = new int[m_iGroupSize];
        memcpy(m_piWorldIndex, piRanks, m_iGroupSize * sizeof(int));
        // create group from the world communicator
        MPI_Group world_group;
        MPI_Comm_group(MPI_COMM_WORLD, &world_group);

        // create communicator from the group
        MPI_Group_incl(world_group, m_iGroupSize, m_piWorldIndex, &m_group);
        MPI_Comm_create(MPI_COMM_WORLD, m_group, &m_comm);
    }

    SciMpiComm* clone()
    {
        return new SciMpiComm(m_piWorldIndex, m_iGroupSize);
    }

    std::wstring getTypeStr()
    {
        return L"SciMpiComm";
    }

    std::wstring getShortTypeStr()
    {
        return L"smc";
    }

    bool hasToString()
    {
        return true;
    }

    bool toString(std::wostringstream& ostr)
    {
        ostr << L"MPI communicator of size " << m_iGroupSize << std::endl;
        ostr << L"World ranks:" << std::endl;
        for (int i = 0; i < m_iGroupSize; i++)
        {
            ostr << " " << m_piWorldIndex[i];
        }

        return true;
    }

    MPI_Comm getComm()
    {
        return m_comm;
    }

    ~SciMpiComm()
    {
        delete[] m_piWorldIndex;
    }

private :
    MPI_Group m_group;
    MPI_Comm m_comm;
    int m_iGroupSize;
    int* m_piWorldIndex;
};

#endif //__SCIMPICOMM_HXX__
