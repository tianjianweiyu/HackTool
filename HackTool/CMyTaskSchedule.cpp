#include "pch.h"
#include "CMyTaskSchedule.h"

CMyTaskSchedule::CMyTaskSchedule()
{
	m_lpITS = NULL;
	m_lpRootFolder = NULL;
	//��ʼ��COM
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"��ʼ��COM�ӿڻ���ʧ��!", L"Error", MB_OK);
		return;
	}

	//��������������
	hr = CoCreateInstance(CLSID_TaskScheduler, NULL,
		CLSCTX_INPROC_SERVER, IID_ITaskService,
		(LPVOID*)(&m_lpITS));
	if (FAILED(hr))
	{
		MessageBox(NULL, L"��������������ʧ��!", L"Error", MB_OK);
		return;
	}

	//���ӵ��������
	hr = m_lpITS->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
	if (FAILED(hr))
	{
		MessageBox(NULL, L"���ӵ��������ʧ��!", L"Error", MB_OK);
		return;
	}

	//��ȡ�������ָ��
	//��ȡRoot Task Folder ��ָ�� �����ָ��ָ�������ע�������
	hr = m_lpITS->GetFolder(_bstr_t("\\"), &m_lpRootFolder);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"��ȡ�������ָ��ʧ��!", L"Error", MB_OK);
		return;
	}
}

CMyTaskSchedule::~CMyTaskSchedule()
{

	if (m_lpITS)
	{
		m_lpITS->Release();
	}
	if (m_lpRootFolder)
	{
		m_lpRootFolder->Release();
	}
	// ж��COM
	CoUninitialize();
}

//��������ƻ�
BOOL CMyTaskSchedule::NewTask(char* lpszTaskName, char* lpszProgramPath, char* lpszParameters, char* lpszAuthor)
{
	if (NULL == m_lpRootFolder)
	{
		return FALSE;
	}

	//���������ͬ�ļƻ�������ɾ��
	Delete(lpszTaskName);

	//�����������������������
	ITaskDefinition *pTaskDefinition = NULL;
	HRESULT hr = m_lpITS->NewTask(0, &pTaskDefinition);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"��������ʧ��!", L"Error", MB_OK);
		return FALSE;
	}
	//����ע����Ϣ
	IRegistrationInfo *pRegInfo = NULL;
	CComVariant variantAuthor(NULL);
	variantAuthor = lpszAuthor;
	hr = pTaskDefinition->get_RegistrationInfo(&pRegInfo);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"����ע����Ϣʧ��!", L"Error", MB_OK);
		return FALSE;
	}
	// ����������Ϣ
	hr = pRegInfo->put_Author(variantAuthor.bstrVal);
	pRegInfo->Release();
	// ���õ�¼���ͺ�����Ȩ��
	IPrincipal *pPrincipal = NULL;
	hr = pTaskDefinition->get_Principal(&pPrincipal);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"���õ�¼���ͺ�����Ȩ��ʧ��!", L"Error", MB_OK);
		return FALSE;
	}
	// ���õ�¼����
	hr = pPrincipal->put_LogonType(TASK_LOGON_INTERACTIVE_TOKEN);
	// ��������Ȩ��
	// ���Ȩ��
	hr = pPrincipal->put_RunLevel(TASK_RUNLEVEL_HIGHEST);
	pPrincipal->Release();
	// ����������Ϣ
	ITaskSettings *pSettting = NULL;
	hr = pTaskDefinition->get_Settings(&pSettting);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"����������Ϣʧ��!", L"Error", MB_OK);
		return FALSE;
	}
	// ����������Ϣ
	hr = pSettting->put_StopIfGoingOnBatteries(VARIANT_FALSE);
	hr = pSettting->put_DisallowStartIfOnBatteries(VARIANT_FALSE);
	hr = pSettting->put_AllowDemandStart(VARIANT_TRUE);
	hr = pSettting->put_StartWhenAvailable(VARIANT_FALSE);
	hr = pSettting->put_MultipleInstances(TASK_INSTANCES_PARALLEL);
	pSettting->Release();
	// ����ִ�ж���
	IActionCollection *pActionCollect = NULL;
	hr = pTaskDefinition->get_Actions(&pActionCollect);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"����ִ�ж���ʧ��!", L"Error", MB_OK);
		return FALSE;
	}
	IAction *pAction = NULL;
	// ����ִ�в���
	hr = pActionCollect->Create(TASK_ACTION_EXEC, &pAction);
	pActionCollect->Release();
	// ����ִ�г���·���Ͳ���
	CComVariant variantProgramPath(NULL);
	CComVariant variantParameters(NULL);
	IExecAction *pExecAction = NULL;
	hr = pAction->QueryInterface(IID_IExecAction, (PVOID *)(&pExecAction));
	if (FAILED(hr))
	{
		pAction->Release();
		MessageBox(NULL, L"����ִ�ж���ʧ��!", L"Error", MB_OK);
		return FALSE;
	}
	pAction->Release();
	// ���ó���·���Ͳ���
	variantProgramPath = lpszProgramPath;
	variantParameters = lpszParameters;
	pExecAction->put_Path(variantProgramPath.bstrVal);
	pExecAction->put_Arguments(variantParameters.bstrVal);
	pExecAction->Release();
	// ���ô�������Ϣ�������û���¼ʱ����
	ITriggerCollection *pTriggers = NULL;
	hr = pTaskDefinition->get_Triggers(&pTriggers);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"���ô�������Ϣʧ��!", L"Error", MB_OK);
		return FALSE;
	}
	// ����������
	ITrigger *pTrigger = NULL;
	hr = pTriggers->Create(TASK_TRIGGER_LOGON, &pTrigger);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"����������ʧ��!", L"Error", MB_OK);
		return FALSE;
	}
	// ע������ƻ�
	IRegisteredTask *pRegisteredTask = NULL;
	CComVariant variantTaskName(NULL);
	variantTaskName = lpszTaskName;
	hr = m_lpRootFolder->RegisterTaskDefinition(variantTaskName.bstrVal,
		pTaskDefinition,
		TASK_CREATE_OR_UPDATE,
		_variant_t(),
		_variant_t(),
		TASK_LOGON_INTERACTIVE_TOKEN,
		_variant_t(""),
		&pRegisteredTask);
	if (FAILED(hr))
	{
		pTaskDefinition->Release();
		MessageBox(NULL, L"ע������ƻ�ʧ��!", L"Error", MB_OK);
		return FALSE;
	}
	pTaskDefinition->Release();
	pRegisteredTask->Release();
	return TRUE;
}

//ɾ���ƻ�����
BOOL CMyTaskSchedule::Delete(char* lpszTaskName)
{
	if(NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	CComVariant variantTaskName(NULL);
	variantTaskName = lpszTaskName;
	HRESULT hr = m_lpRootFolder->DeleteTask(variantTaskName.bstrVal, 0);
	if (FAILED(hr))
	{
		return FALSE;
	}
	return TRUE;
}
