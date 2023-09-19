// MyList.cpp: implementation of the CMyList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "TestList.h"

#include "MyList.h"

/*
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyList::CMyList()
{
	m_arrPtr = new DWORD[1000];
	m_iPtrCnt = 0;
}

CMyList::~CMyList()
{

}

int CMyList::AddTail(void *ptr)
{
	if(1000 <= m_iPtrCnt)
	{
		return m_iPtrCnt;
	}

	m_arrPtr[m_iPtrCnt++] = (DWORD)ptr;

	return m_iPtrCnt;
}

void* CMyList::GetHead()
{
	if(0 == m_iPtrCnt)
	{
		return NULL;
	}

	return (void*)m_arrPtr[0];
}

void CMyList::RemoveHead()
{
	if(m_iPtrCnt <= 0)
	{
		return;
	}

	for(int i=0; i<m_iPtrCnt-1; i++)
	{
		m_arrPtr[i] = m_arrPtr[i+1];
	}

	m_iPtrCnt--;
}

int CMyList::GetCount()
{
	return m_iPtrCnt;
}

void* CMyList::GetAt(int i)
{
	return (void*)m_arrPtr[i];
}
