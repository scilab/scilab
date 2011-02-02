/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#ifndef __INTERNAL_HXX__
#define __INTERNAL_HXX__

#include <vector>
#include <iostream>
#include <string.h>
#include "anytype.hxx"

namespace types
{
  class InternalType
  {
  public :
      enum RealType {
          /* Internal Type */
          RealInternal,
          /* Generic Types */
          RealGeneric,
          RealInt8,
          RealUInt8,
          RealInt16,
          RealUInt16,
          RealInt32,
          RealUInt32,
          RealInt64,
          RealUInt64,
          RealString,
          RealDouble,
          RealBool,
          RealFloat,
          RealPoly,
          RealSinglePoly,
          /* Callable */
          RealFunction,
          RealMacro,
          RealMacroFile,
          /* Implicit List */
          RealImplicitList,
          /* Container */
          RealContainer,
          RealList,
          RealTList,
          RealMList,
          RealStruct,
          RealCell,
          /* User */
          RealUserType,
          /*For list operation*/
          RealListOperation, //parent type
          RealListInsertOperation,
          RealListDeleteOperation,
          RealListUndefinedOperation,
          RealFile,
          RealColon,
          RealDollar,
          RealThreadId
      };

  protected :
                                        InternalType() : m_iRef(0), m_bAllowDelete(true) {}

  public :
      virtual                           ~InternalType(){};
      virtual void                      whoAmI(void) { std::cout << "types::Inernal"; }

      virtual bool                      isAssignable(void) { return false; }
      virtual RealType                  getType(void) = 0 ; //{ return RealInternal; }


      virtual std::wstring	            toString(int _iPrecison, int _iLineLen) = 0;
      virtual InternalType*             clone(void) = 0;


      void IncreaseRef()
      {
          m_iRef++;
      }

      void DecreaseRef()
      {
          if(m_iRef > 0)
          {
              m_iRef--;
          }
      }

      bool	                            isDeletable() { return m_iRef == 0; }
      bool	                            isRef(int _iRef = 0) { return m_iRef > _iRef; }
      int                               getRef() { return m_iRef; }

      /* return type as string ( double, int, cell, list, ... )*/
      virtual std::wstring              getTypeStr() = 0;
      /* return type as short string ( s, i, ce, l, ... )*/
      virtual std::wstring              getShortTypeStr() = 0;

      virtual bool                      operator==(const InternalType& it) { return (getType() == (const_cast<InternalType *>(&it))->getType()); }
      virtual bool                      operator!=(const InternalType& it) { return !(*this == it); }

      /**
      ** GenericType
      ** \{
      */

      template <class T>
      T*                                getAs(void) { return dynamic_cast<T*>(this); }

      /* GenericType */
      virtual bool                      isGenericType(void);
      virtual GenericType*              getAsGenericType(void) { return NULL; }

      virtual bool                      isString(void) { return false; }
      virtual bool                      isDouble(void) { return false; }

      /* Float */
      virtual bool                      isFloat(void) { return false; }
      virtual Float*                    getAsFloat(void) { return NULL; }

      /* Int */
      virtual bool                      isInt(void) { return false; }
      virtual bool                      isInt8(void) { return false; }
      virtual bool                      isUInt8(void) { return false; }
      virtual bool                      isInt16(void) { return false; }
      virtual bool                      isUInt16(void) { return false; }
      virtual bool                      isInt32(void) { return false; }
      virtual bool                      isUInt32(void) { return false; }
      virtual bool                      isInt64(void) { return false; }
      virtual bool                      isUInt64(void) { return false; }

      /* Bool */
      virtual bool                      isBool(void) { return false; }

      /* SinglePoly Matrix */
      virtual bool                      isPoly(void) { return false; }

      /* Single SinglePoly */
      virtual bool                      isSinglePoly(void) { return false; }
      virtual SinglePoly*               getAsSinglePoly(void) { return NULL; }

      /**
      ** \}
      */

      /**
      ** Callable
      ** \{
      */
      virtual bool                      isCallable(void) { return false; }
      virtual Callable*                 getAsCallable(void) { return NULL; }

      /* Function */
      virtual bool                      isFunction(void) { return false; }
      virtual Function*                 getAsFunction(void) { return NULL; }

      /* Macro */
      virtual bool                      isMacro(void) { return false; }
      virtual Macro*                    getAsMacro(void) { return NULL; }

      /* MacroFile */
      virtual bool                      isMacroFile(void) { return false; }
      virtual MacroFile*                getAsMacroFile(void) { return NULL; }

      /**
      ** \}
      */

      /**
      ** Container
      ** \{
      */
      virtual bool                      isContainer(void) { return false; }
      virtual Container*                getAsContainer(void) { return NULL; }

      /* List */
      virtual bool                      isList(void) { return false; }
      virtual List*                     getAsList(void) { return NULL; }

      /* Struct */
      virtual bool                      isStruct(void) { return false; }
      virtual Struct*                   getAsStruct(void) { return NULL; }

      /* Cell */
      virtual bool                      isCell(void) { return false; }

      /* TList */
      virtual bool                      isTList(void) { return false; }
      virtual TList*                    getAsTList(void) { return NULL; }

      /* MList */
      virtual bool                      isMList(void) { return false; }
      virtual MList*                    getAsMList(void) { return NULL; }

      /**
      ** \}
      */

      /* ImplicitList */
      virtual bool                      isImplicitList(void) { return false; }
      virtual ImplicitList*             getAsImplicitList(void) { return NULL; }

      /* Colon & Dollar */
      virtual bool                      isColon(void) { return false; }
      virtual bool                      isDollar(void) { return false; }

      /* File */
      virtual bool                      isFile(void) { return false; }
      virtual File*                     getAsFile(void) { return NULL; }

      /* ThreadId */
      virtual bool                      isThreadId(void) { return false; }
      virtual ThreadId*                 getAsThreadId(void) { return NULL; }

      /**
      ** List Operations
      ** \{
      */

      /* ListOperation */
      virtual bool                      isListOperation(void) { return false; }
      virtual ListOperation*            getAsListOperation(void) { return NULL; }

      /* ListDelete */
      virtual bool                      isListDelete(void) { return false; }
      virtual ListDelete*               getAsListDelete(void) { return NULL; }

      /* ListAdd */
      virtual bool                      isListInsert(void) { return false; }
      virtual ListInsert*               getAsListInsert(void) { return NULL; }

      /* ListUndefined */
      virtual bool                      isListUndefined(void) { return false; }
      virtual ListUndefined*            getAsListUndefined(void) { return NULL; }

      /**
      ** \}
      */

  private :
      int                               m_iRef;
      //use to know if we can delete this variables or if it's link to a scilab variable.
      bool                              m_bAllowDelete;
  };

  /*
  ** List of types
  */
  typedef std::vector<InternalType *>   typed_list;

}

#endif /* !__INTERNAL_HXX__ */
