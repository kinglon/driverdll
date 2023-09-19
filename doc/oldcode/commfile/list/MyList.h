// MyList.h: interface for the CMyList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYLIST_H__76FBA52F_C089_4523_9DBB_091A0FA8E8DC__INCLUDED_)
#define AFX_MYLIST_H__76FBA52F_C089_4523_9DBB_091A0FA8E8DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMyList  
{
public:
	CMyList();
	virtual ~CMyList();

public:
	DWORD *m_arrPtr;
	int	  m_iPtrCnt;

public:
	void* GetAt(int i);
	int GetCount();
	void RemoveHead();
	void* GetHead();
	int AddTail(void *ptr);
};

#endif // !defined(AFX_MYLIST_H__76FBA52F_C089_4523_9DBB_091A0FA8E8DC__INCLUDED_)
