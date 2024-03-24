////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Глобальные объявления типов и определения.
         \en Global type declarations and defines. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __QT_VISIONDEF_H
#define __QT_VISIONDEF_H

#include <vsn_global.h>

VSN_USE_NAMESPACE
VSN_BEGIN_NAMESPACE


#if defined(_WIN32) // only windows
// если не статическая линковка
#if !defined(QT_VISION_STATIC)
#if defined (_BUILDQTVISIONDLL)
#define  __QTVISION_DECL_SPEC __declspec( dllexport )
#else // _BUILDQTVISIONDLL
#define  __QTVISION_DECL_SPEC __declspec( dllimport )
#endif
#else // QT_VISION_STATIC
#if defined(QT_VISION_EXPORT) 
#define  __QTVISION_DECL_SPEC __declspec( dllexport )
#endif
#endif 
#else  // _WIN32
#define  __QTVISION_DECL_SPEC
#endif

#define QT_CLASS         __QTVISION_DECL_SPEC
#define QT_FUNC(retType) __QTVISION_DECL_SPEC retType CALL_DECLARATION
#define QT_FUNC_EX       __QTVISION_DECL_SPEC 

VSN_END_NAMESPACE

#endif //__QT_VISIONDEF_H

