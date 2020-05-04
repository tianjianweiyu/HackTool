#pragma once

#include <windows.h>
#include <winternl.h>

void OnInlineHook();

void UnInlineHook();

NTSTATUS WINAPI MyZwQuerySystemInformation(
	SYSTEM_INFORMATION_CLASS SystemInformationClass,
	PVOID SystemInformation,
	ULONG SystemInformationLength,
	PULONG ReturnLength
);