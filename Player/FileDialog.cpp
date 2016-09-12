#include <Windows.h>
#include <commdlg.h>
#include <tchar.h>

#include "FileDialog.h"

//ѡ���ļ�
int DialogGetFileName(HWND hParent,LPCTSTR strFilter,TCHAR *szFile,int nFileSize)  
{  
	//TCHAR szFile[MAX_PATH]; // �����ȡ�ļ����ƵĻ�����
	ZeroMemory(szFile,nFileSize);

	OPENFILENAME ofn;      // �����Ի���ṹ��    
	// ��ʼ��ѡ���ļ��Ի���   
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);  
	ofn.hwndOwner = hParent;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = nFileSize;
	ofn.lpstrFilter = strFilter/*_T("All(*.*)\0*.*\0Text(*.txt)\0*.TXT\0\0")*/;
	ofn.nFilterIndex = 1;  
	ofn.lpstrFileTitle = NULL;  
	ofn.nMaxFileTitle = 0;  
	ofn.lpstrInitialDir = NULL;  
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;  
	//ofn.lpTemplateName =  MAKEINTRESOURCE(ID_TEMP_DIALOG);  
	// ��ʾ��ѡ���ļ��Ի���   

	if ( GetOpenFileName(&ofn) )  
	{  
		//��ʾѡ����ļ���   
		//OutputDebugString(szFile);
		return 1;
	} 
	return -1;
}
//��ѡ�ļ�
int GetFileNames()
{
	TCHAR szOpenFileNames[80*MAX_PATH];
	ZeroMemory(szOpenFileNames,80*MAX_PATH);

	OPENFILENAME ofn;
	TCHAR szPath[MAX_PATH];
	TCHAR szFileName[80*MAX_PATH];
	TCHAR* p;
	int nLen = 0;
	ZeroMemory( &ofn, sizeof(ofn) );
	ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szOpenFileNames;
	ofn.nMaxFile = sizeof(szOpenFileNames);
	ofn.lpstrFilter = TEXT("All Files(*.*)/0*.*/0");

	if( GetOpenFileName( &ofn ) )
	{  
		//�ѵ�һ���ļ���ǰ�ĸ��Ƶ�szPath,��:
		//���ֻѡ��һ���ļ�,�͸��Ƶ����һ��'/'
		//���ѡ�˶���ļ�,�͸��Ƶ���һ��NULL�ַ�
		lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset );
		//��ֻѡ��һ���ļ�ʱ,�������NULL�ַ��Ǳ����.
		//���ﲻ����Դ�ѡ��һ���Ͷ���ļ������
		szPath[ ofn.nFileOffset ] = '/0';
		nLen = lstrlen(szPath);
  
		if( szPath[nLen-1] != '//' )   //���ѡ�˶���ļ�,��������'//'
		{
			lstrcat(szPath, TEXT("//"));
		}
  
		p = szOpenFileNames + ofn.nFileOffset; //��ָ���Ƶ���һ���ļ�
  
		ZeroMemory(szFileName, sizeof(szFileName));
		while( *p )
		{   
			lstrcat(szFileName, szPath);  //���ļ�������·��  
			lstrcat(szFileName, p);    //�����ļ���  
			lstrcat(szFileName, TEXT("/n")); //����   
			p += lstrlen(p) +1;     //������һ���ļ�
		}
		MessageBox(NULL, szFileName, TEXT("MultiSelect"), MB_OK);
		return 1;
	}
	return -1;
}
#include <ShlObj.h>

//ѡ���ļ���
int GetDirecty()
{
	TCHAR szBuffer[MAX_PATH] = {0}; 
	BROWSEINFO bi; 
	ZeroMemory(&bi,sizeof(BROWSEINFO)); 
	bi.hwndOwner = NULL; 
	bi.pszDisplayName = szBuffer; 
	bi.lpszTitle = _T("������ѡ�ļ���Ŀ¼:"); 
	bi.ulFlags = BIF_RETURNFSANCESTORS; 
	LPITEMIDLIST idl = SHBrowseForFolder(&bi); 
	if (NULL == idl) 
	{ 
		return -1; 
	} 
	SHGetPathFromIDList(idl,szBuffer); 
	return 1;
}
int GetFileOrDirecty()
{
	TCHAR szBuffer[MAX_PATH] = {0};   
	BROWSEINFO bi;   
	ZeroMemory(&bi,sizeof(BROWSEINFO));   
	bi.hwndOwner = NULL;   
	bi.pszDisplayName = szBuffer;   
	bi.lpszTitle = _T("������ѡ���ļ����ļ���:");   
	bi.ulFlags = BIF_BROWSEINCLUDEFILES;   
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);   
	if (NULL == idl)   
	{   
		return -1;   
	}   
	SHGetPathFromIDList(idl,szBuffer);
	return 1;
}