// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <tchar.h>
#include <windows.h>

// 直接转发DLL函数
#pragma comment(linker, "/EXPORT:__CreateFrameInfo=vcruntime140dOrg._CreateFrameInfo")
#pragma comment(linker, "/EXPORT:__CxxThrowException=vcruntime140dOrg._CxxThrowException")
#pragma comment(linker, "/EXPORT:__EH_prolog=vcruntime140dOrg._EH_prolog")
#pragma comment(linker, "/EXPORT:__FindAndUnlinkFrame=vcruntime140dOrg._FindAndUnlinkFrame")
#pragma comment(linker, "/EXPORT:__IsExceptionObjectToBeDestroyed=vcruntime140dOrg._IsExceptionObjectToBeDestroyed")
#pragma comment(linker, "/EXPORT:__NLG_Dispatch2=vcruntime140dOrg._NLG_Dispatch2")
#pragma comment(linker, "/EXPORT:__NLG_Return=vcruntime140dOrg._NLG_Return")
#pragma comment(linker, "/EXPORT:__NLG_Return2=vcruntime140dOrg._NLG_Return2")
#pragma comment(linker, "/EXPORT:__SetWinRTOutOfMemoryExceptionCallback=vcruntime140dOrg._SetWinRTOutOfMemoryExceptionCallback")
#pragma comment(linker, "/EXPORT:___AdjustPointer=vcruntime140dOrg.__AdjustPointer")
#pragma comment(linker, "/EXPORT:___BuildCatchObject=vcruntime140dOrg.__BuildCatchObject")
#pragma comment(linker, "/EXPORT:___BuildCatchObjectHelper=vcruntime140dOrg.__BuildCatchObjectHelper")
#pragma comment(linker, "/EXPORT:___CxxDetectRethrow=vcruntime140dOrg.__CxxDetectRethrow")
#pragma comment(linker, "/EXPORT:___CxxExceptionFilter=vcruntime140dOrg.__CxxExceptionFilter")
#pragma comment(linker, "/EXPORT:___CxxFrameHandler=vcruntime140dOrg.__CxxFrameHandler")
#pragma comment(linker, "/EXPORT:___CxxFrameHandler2=vcruntime140dOrg.__CxxFrameHandler2")
#pragma comment(linker, "/EXPORT:___CxxFrameHandler3=vcruntime140dOrg.__CxxFrameHandler3")
#pragma comment(linker, "/EXPORT:___CxxLongjmpUnwind=vcruntime140dOrg.__CxxLongjmpUnwind")
#pragma comment(linker, "/EXPORT:___CxxQueryExceptionSize=vcruntime140dOrg.__CxxQueryExceptionSize")
#pragma comment(linker, "/EXPORT:___CxxRegisterExceptionObject=vcruntime140dOrg.__CxxRegisterExceptionObject")
#pragma comment(linker, "/EXPORT:___CxxUnregisterExceptionObject=vcruntime140dOrg.__CxxUnregisterExceptionObject")
#pragma comment(linker, "/EXPORT:___DestructExceptionObject=vcruntime140dOrg.__DestructExceptionObject")
#pragma comment(linker, "/EXPORT:___FrameUnwindFilter=vcruntime140dOrg.__FrameUnwindFilter")
#pragma comment(linker, "/EXPORT:___GetPlatformExceptionInfo=vcruntime140dOrg.__GetPlatformExceptionInfo")
#pragma comment(linker, "/EXPORT:___RTCastToVoid=vcruntime140dOrg.__RTCastToVoid")
#pragma comment(linker, "/EXPORT:___RTDynamicCast=vcruntime140dOrg.__RTDynamicCast")
#pragma comment(linker, "/EXPORT:___RTtypeid=vcruntime140dOrg.__RTtypeid")
#pragma comment(linker, "/EXPORT:___TypeMatch=vcruntime140dOrg.__TypeMatch")
#pragma comment(linker, "/EXPORT:___current_exception=vcruntime140dOrg.__current_exception")
#pragma comment(linker, "/EXPORT:___current_exception_context=vcruntime140dOrg.__current_exception_context")
#pragma comment(linker, "/EXPORT:___intrinsic_setjmp=vcruntime140dOrg.__intrinsic_setjmp")
#pragma comment(linker, "/EXPORT:___processing_throw=vcruntime140dOrg.__processing_throw")
#pragma comment(linker, "/EXPORT:___report_gsfailure=vcruntime140dOrg.__report_gsfailure")
#pragma comment(linker, "/EXPORT:___std_exception_copy=vcruntime140dOrg.__std_exception_copy")
#pragma comment(linker, "/EXPORT:___std_exception_destroy=vcruntime140dOrg.__std_exception_destroy")
#pragma comment(linker, "/EXPORT:___std_terminate=vcruntime140dOrg.__std_terminate")
#pragma comment(linker, "/EXPORT:___std_type_info_compare=vcruntime140dOrg.__std_type_info_compare")
#pragma comment(linker, "/EXPORT:___std_type_info_destroy_list=vcruntime140dOrg.__std_type_info_destroy_list")
#pragma comment(linker, "/EXPORT:___std_type_info_hash=vcruntime140dOrg.__std_type_info_hash")
#pragma comment(linker, "/EXPORT:___std_type_info_name=vcruntime140dOrg.__std_type_info_name")
#pragma comment(linker, "/EXPORT:___telemetry_main_invoke_trigger=vcruntime140dOrg.__telemetry_main_invoke_trigger")
#pragma comment(linker, "/EXPORT:___telemetry_main_return_trigger=vcruntime140dOrg.__telemetry_main_return_trigger")
#pragma comment(linker, "/EXPORT:___unDName=vcruntime140dOrg.__unDName")
#pragma comment(linker, "/EXPORT:___unDNameEx=vcruntime140dOrg.__unDNameEx")
#pragma comment(linker, "/EXPORT:___uncaught_exception=vcruntime140dOrg.__uncaught_exception")
#pragma comment(linker, "/EXPORT:___uncaught_exceptions=vcruntime140dOrg.__uncaught_exceptions")
#pragma comment(linker, "/EXPORT:___vcrt_GetModuleFileNameW=vcruntime140dOrg.__vcrt_GetModuleFileNameW")
#pragma comment(linker, "/EXPORT:___vcrt_GetModuleHandleW=vcruntime140dOrg.__vcrt_GetModuleHandleW")
#pragma comment(linker, "/EXPORT:___vcrt_InitializeCriticalSectionEx=vcruntime140dOrg.__vcrt_InitializeCriticalSectionEx")
#pragma comment(linker, "/EXPORT:___vcrt_LoadLibraryExW=vcruntime140dOrg.__vcrt_LoadLibraryExW")
#pragma comment(linker, "/EXPORT:__chkesp=vcruntime140dOrg._chkesp")
#pragma comment(linker, "/EXPORT:__except_handler2=vcruntime140dOrg._except_handler2")
#pragma comment(linker, "/EXPORT:__except_handler3=vcruntime140dOrg._except_handler3")
#pragma comment(linker, "/EXPORT:__except_handler4_common=vcruntime140dOrg._except_handler4_common")
#pragma comment(linker, "/EXPORT:__get_purecall_handler=vcruntime140dOrg._get_purecall_handler")
#pragma comment(linker, "/EXPORT:__get_unexpected=vcruntime140dOrg._get_unexpected")
#pragma comment(linker, "/EXPORT:__global_unwind2=vcruntime140dOrg._global_unwind2")
#pragma comment(linker, "/EXPORT:__is_exception_typeof=vcruntime140dOrg._is_exception_typeof")
#pragma comment(linker, "/EXPORT:__local_unwind2=vcruntime140dOrg._local_unwind2")
#pragma comment(linker, "/EXPORT:__local_unwind4=vcruntime140dOrg._local_unwind4")
#pragma comment(linker, "/EXPORT:__longjmpex=vcruntime140dOrg._longjmpex")
#pragma comment(linker, "/EXPORT:__purecall=vcruntime140dOrg._purecall")
#pragma comment(linker, "/EXPORT:__seh_longjmp_unwind4=vcruntime140dOrg._seh_longjmp_unwind4")
#pragma comment(linker, "/EXPORT:__seh_longjmp_unwind=vcruntime140dOrg._seh_longjmp_unwind")
#pragma comment(linker, "/EXPORT:__set_purecall_handler=vcruntime140dOrg._set_purecall_handler")
#pragma comment(linker, "/EXPORT:__set_se_translator=vcruntime140dOrg._set_se_translator")
#pragma comment(linker, "/EXPORT:__setjmp3=vcruntime140dOrg._setjmp3")
#pragma comment(linker, "/EXPORT:longjmp=vcruntime140dOrg.longjmp")
#pragma comment(linker, "/EXPORT:memchr=vcruntime140dOrg.memchr")
#pragma comment(linker, "/EXPORT:memcmp=vcruntime140dOrg.memcmp")
#pragma comment(linker, "/EXPORT:memcpy=vcruntime140dOrg.memcpy")
#pragma comment(linker, "/EXPORT:memmove=vcruntime140dOrg.memmove")
#pragma comment(linker, "/EXPORT:memset=vcruntime140dOrg.memset")
#pragma comment(linker, "/EXPORT:set_unexpected=vcruntime140dOrg.set_unexpected")
#pragma comment(linker, "/EXPORT:strchr=vcruntime140dOrg.strchr")
#pragma comment(linker, "/EXPORT:strrchr=vcruntime140dOrg.strrchr")
#pragma comment(linker, "/EXPORT:strstr=vcruntime140dOrg.strstr")
#pragma comment(linker, "/EXPORT:unexpected=vcruntime140dOrg.unexpected")
#pragma comment(linker, "/EXPORT:wcschr=vcruntime140dOrg.wcschr")
#pragma comment(linker, "/EXPORT:wcsrchr=vcruntime140dOrg.wcsrchr")
#pragma comment(linker, "/EXPORT:wcsstr=vcruntime140dOrg.wcsstr")

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hModule);
		MessageBox(NULL,_T("劫持成功！"),_T("提示"),NULL);
		break;
	}
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

