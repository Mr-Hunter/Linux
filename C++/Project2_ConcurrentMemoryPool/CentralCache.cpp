#include "CentralCache.h"
#include "PageCache.h"

CentralCache CentralCache::_inst;

//// ��׮	(α���ݲ���)
//// �����Ļ����ȡһ�������Ķ����thread cache
//size_t CentralCache::FetchRangeObj(void*& start, void*& end, size_t n, size_t bytes)
//{	 //��֤threadcache�����Ļ�ȡ�ڴ�
//	// ͨ��α���ݲ��Թ��ܣ���Ԫ�����н�����׮
//	start = malloc(n*bytes);
//	end = (char*)start + (n - 1)*bytes;
//	//���ӳ�����
//	void* cur = start;
//	while (cur != end)
//	{
//		void* next = (char*)cur + bytes;
//		NEXT_OBJ(cur) = next;
//		cur = next;
//	}
//	NEXT_OBJ(end) = nullptr;
//	return n;
//}

//��ȡ��Ϊ�յ�span��������һ��
Span* CentralCache::GetOneSpan(SpanList* spanlist, size_t bytes)
{
	Span* span = spanlist->begin();
	while (span != spanlist->end())
	{
		if (span->_objlist != nullptr)
			return span;

		span = span->_next;
	}

	//ͨ��bytes�����npage
	size_t npage = ClassSize::BytesMovePage(bytes);
	//ȫ��span�� ��pagecache����һ���µĺ��ʴ�С��span
	Span* newspan = PageCache::GetInstance()->NewSpan(npage);

	// ��span���ڴ��и��һ����bytes��С�Ķ��������
	char* start = (char*)(newspan->_pageid << PAGE_SHIFT);
	char* end = start + (newspan->_npage << PAGE_SHIFT);
	char* cur = start;
	char* next = cur + bytes;
	while (next < end)
	{
		NEXT_OBJ(cur) = next;
		cur = next;
		next = cur + bytes;
	}
	NEXT_OBJ(cur) = nullptr;
	newspan->_objlist = start;
	newspan->_objsize = bytes;
	newspan->_usecount = 0;

	// ��newspan���뵽spanlist
	spanlist->PushFront(newspan);
	return newspan;
}

// �����Ļ����ȡһ�������Ķ����thread cache
size_t CentralCache::FetchRangeObj(void*& start, void*& end, size_t num, size_t bytes)
{
	size_t index = ClassSize::Index(bytes);
	SpanList* spanlist = &_spanlist[index];
	Span* span = GetOneSpan(spanlist, bytes);  

	void* cur = span->_objlist;
	void* prev = cur;
	size_t fetchnum = 0;
	while (cur != nullptr && fetchnum < num)
	{
		prev = cur;
		cur = NEXT_OBJ(cur);
		++fetchnum;
	}

	start = span->_objlist;
	end = prev;
	NEXT_OBJ(end) = nullptr;

	span->_objlist = cur;
	span->_usecount += fetchnum;

	return fetchnum;
}