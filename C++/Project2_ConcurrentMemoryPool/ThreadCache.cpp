#include "ThreadCache.h"
#include "CentralCache.h"

//��һ�ַ�����ÿ�ζ��Ǵ�CentralCache��ȡ�̶������Ķ���10
void* ThreadCache::FetchFromCentralCache(size_t index, size_t byte)
{
	FreeList* freelist = &_freelist[index];
	size_t num = 10;

	void* start, *end;
	//����num�����Ĳ�һ���У���Ҫ��ȡʵ������ĸ������������ٱ�֤��һ��
	size_t fetchnum = CentralCache::GetInstance()->FetchRangeObj(start, end, num, byte);
	if (fetchnum == 1)
		return start;
	//����Ļ�������һ�����أ�����Ĺ�������������
	freelist->PushRange(NEXT_OBJ(start), end, fetchnum - 1);
	return start;
}
//��������ÿ�λ�ȡʱ����ȡ�����������������Ĺ���
//void* ThreadCache::FetchFromCentralCache(size_t index, size_t byte)
//{
//	FreeList* freelist = &_freelist[index];
//	size_t num = 10;
//	void* start, *end;
//	size_t fetchnum = CentralCache::GetInstance()->FetchRangeObj(start, end, num, byte);
//	if (fetchnum == 1)
//		return start;
//
//	freelist->PushRange(NEXT_OBJ(start), end, fetchnum - 1);
//	return start;
//}

void* ThreadCache::Allocate(size_t size)
{
	assert(size <= MAXBYTES);

	//�����Զ���Ķ������
	// ���ݶ������size����ȡ��
	size = ClassSize::Roundup(size);
	//��ȡ��������Ӧ���±�
	size_t index = ClassSize::Index(size);
	FreeList* freelist = &_freelist[index];
	if (!freelist->Empty())
	{
		//����ͷɾһ���ڵ㲢����
		return freelist->Pop();
	}
	else
	{
		//����Ϊ�գ���Centralache�л�ȡ����
		//ȡһ��������ÿ�ζ������������ģ����Զ�ȡһ�㣬��Ĺ��������ϣ��Ա��´�ʹ��
		return FetchFromCentralCache(index, size);
	}
}

void ThreadCache::Deallocate(void* ptr, size_t size)
{
	assert(size <= MAXBYTES);
	size_t index = ClassSize::Index(size);
	FreeList* freelist = &_freelist[index];
	freelist->Push(ptr);

	//������������������������������ʱ��ʼ���л���
	if (freelist->Size() >= freelist->MaxSize())
	{
		//�ֱ�����������ȫ������
		//ListTooLong(freelist, freelist->Size());
	}
}
//void ThreadCache::ListTooLong(FreeList* list, size_t size)
//{
//
//
//}