#ifndef MACRO_H__
#define MACRO_H__

#pragma once

#include <QDebug>
#include <QtCore/qglobal.h>

#ifdef __cplusplus
#  define EXTERN_C_BEGIN extern "C" {
#  define EXTERN_C_END    }
#else
#  define EXTERN_C_BEGIN
#  define EXTERN_C_END
#endif

#if 1
#  if defined(KFRAME_LIB)		// 框架使用
#    define KFRAME_API Q_DECL_EXPORT
#  else
#    define KFRAME_API Q_DECL_IMPORT
#  endif
#  if defined(KWIDGET_LIB)		// 自定义组件工程使用
#    define KWIDGET_API Q_DECL_EXPORT
#  else
#    define KWIDGET_API Q_DECL_IMPORT
#  endif
#  if defined(KMODULE_LIB)		// 业务模块使用
#    define KMODULE_API Q_DECL_EXPORT
#  else
#    define KMODULE_API Q_DECL_IMPORT
#  endif
#  if defined(KQUOT_LIB)		// 行情管理模块
#    define KQUOT_API Q_DECL_EXPORT
#  else
#    define KQUOT_API Q_DECL_IMPORT
#  endif
#  if defined(KMSG_LIB)			// 消息管理模块
#    define KMSG_API Q_DECL_EXPORT
#  else
#    define KMSG_API Q_DECL_IMPORT
#  endif
#  if defined(KRES_LIB)			// 资源管理模块
#    define KRES_API Q_DECL_EXPORT
#  else
#    define KRES_API Q_DECL_IMPORT
#  endif
#  ifdef KLOG_LIB				// 日志模块
#    define KLOG_API Q_DECL_EXPORT
#  else
#    define KLOG_API Q_DECL_IMPORT
#  endif
#  ifdef KDATA_CENTER_LIB		// 数据中心模块
#    define KDATA_CENTER_API Q_DECL_EXPORT
#  else
#    define KDATA_CENTER_API Q_DECL_IMPORT
#  endif
#  ifdef KASSIST_LIB		// 交易辅助类
#    define KASSIST_API Q_DECL_EXPORT
#  else
#    define KASSIST_API Q_DECL_IMPORT
#  endif
#endif

/* 断言K_ASSERT */
#define K_ASSERT Q_ASSERT

/* 字符串KSL*/
#define KSL(S) QStringLiteral(S)
#define KSL_STD(S) QStringLiteral(S).toStdString()

//////////////////////////////////////////////////////////////////////////
// 转换相对路径到绝对路径
//      转换路径基于使用此宏的文件
// eg:
//  E:\dir\a.h 中使用此宏
//      _CONVERT_2_ABSPATH(lib/test.lib)
//  宏展开后为：
//      E:\dir\a.h/../lib/test.lib  == E:/dir/lib/test.lib
#ifndef _CONVERT_2_ABSPATH
    #define _CONVERT_2_ABSPATH(f) __FILE__##"/../"#f
#endif

// 链接到lib文件（lib文件路径为使用此宏的文件的相对路径）
//  eg:
//      #pragma _LINK_LIBFILE(lib/test.lib)
#ifndef _LINK_LIBFILE
    #define _LINK_LIBFILE(f) comment(lib, _CONVERT_2_ABSPATH(f))
#endif 

//////////////////////////////////////////////////////////////////////////
// 单实例模式宏
#define SINGLETON_ON_DESTRUCTOR(clsname) \
    public: static clsname* GetInstance(){ static clsname _instance;return &_instance; }\
    private:~clsname()

// 释放管理器中的所有元素
#define DELETE_ALL_ELEMENT(container_) \
    for each(auto *__ in container_) { delete __; }

//////////////////////////////////////////////////////////////////////////
// 内存泄漏检测
// 
// #ifdef _DEBUG
//     #define _CRTDBG_MAP_ALLOC 
//     #define _CRTDBG_MAP_ALLOC_NEW
//     #include<stdlib.h> 
//     #include<crtdbg.h>
// 
//     #define _QUIMemCheckPoint(s) \
//         _CrtMemState __ui_##s; \
//         _CrtMemCheckpoint(&(__ui_##s));
// 
//     #define _QUIIsMemLeak(s1,s2) \
//          { \
//              _CrtMemState __ui_s3;\
//             if ( _CrtMemDifference(&__ui_s3, &(__ui_##s1),&(__ui_##s2)) )\
//             {\
//                 TRACE(L"------------内存泄漏---------------\n");\
//                 _CrtMemDumpStatistics(&__ui_s3);\
//                 TRACE(L"-----------------------------------\n");\
//                 _ASSERT_EXPR(FALSE,L"发生内存泄漏！请检查!");\
//              }\
//          }
// #else
//     #define _QUIMemCheckPoint(s) 
//     #define _QUIIsMemLeak(s1,s2)
// #endif
// 
#endif // MACRO_H__
