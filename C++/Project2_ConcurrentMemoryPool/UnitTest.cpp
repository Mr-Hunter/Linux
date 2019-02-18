#include "Common.h"
#include "ConcurrentMePool.h"

void TestThreadCache()
{
	//��֤ÿ�������ڴ��̶߳���һ��Cache��Ҳ����ÿ������ʱ�ʹ���������һ��cache
	//�߳���λ�ȡCache��
	//1��ȫ�־�̬�����̹߳���������Threadcache��������������������Ȼ��ÿ����һ���߳�id��
	//		Ҳ���ǿ�ʼ����Ϊ�գ���һ���̣߳�����һ��cache�������ӵ���������¼tid(ÿ���߳�tid����ͨ��һ���ӿڻ�ȡ�����磺thread_self()��)
	//�����߳������Ȳ��ң�����ͬtidʹ�ã�û�д���������.....�����ã��߳�һ������²�̫��
	//���Ǵ˴���ÿ���̻߳�ȡcache����Ҫ��������Ϊ�������ڣ�����ͬʱ�е��ڶ����ݣ��е���д����
	//�������ԣ�����������̫��
	/*
	int _tid;
	ThreadCache* _next;
	//2.���̵߳�ʱ�򣬼ǵ�tls��thread local storage�����̱߳��ش洢��ÿ���̶߳���һ��ȫ�ֱ����������⺯����������������Ǹ��̡߳�
	//�������ĵط����ڣ������ݣ���ͬƽ̨������Ҫ��ִ���
	//��̬�붯̬���𣺾�̬Ч�ʸ��ߣ������		
	*/

	//��֤ÿ���߳�һ��cache����������ȡ��
	//�������ӿ���ӿڵĲ������̺߳����ǿɱ������
	/*std::thread t1(ConcurrentAlloc,10);
	std::thread t2(ConcurrentAlloc, 10);
	
	t1.join();
	t2.join();*/
	
	//��֤��δ�����cache��ȡ����,�ҵ�һ������Ϊ�մ������������ڴ�������ڶ��������ڴ�����ʱ������Ϊ�գ�ֱ��ȡ��
	/*void* p1 = ConcurrentAlloc(10);
	void* p2 = ConcurrentAlloc(10);*/

	//��֤�ڴ��ظ����ã������룬���ͷţ������룬���ֿռ�����ظ�ʹ��
	/*std::vector<void*> v;
	for (size_t i = 0; i < 10; ++i)
	{
		v.push_back(ConcurrentAlloc(10));
		cout << v.back() << endl;
	}
	cout << endl << endl;

	for (size_t i = 0; i < 10; ++i)
	{
		ConcurrentFree(v[i],10);
	}
	v.clear();

	for (size_t i = 0; i < 10; ++i)
	{
		v.push_back(ConcurrentAlloc(10));
		cout << v.back() << endl;
	}

	for (size_t i = 0; i < 10; ++i)
	{
		ConcurrentFree(v[i], 10);
	}
	v.clear();*/
}

//���Բ鿴ҳ��
void TestPageCache()
{
	//VirtualAlloc��ҳ������Եģ�����malloc���ǰ�ҳ����Ͳ����ԣ����ܴ��ɿ��ԣ�
	void* ptr = VirtualAlloc(NULL, (NPAGES - 1) << PAGE_SHIFT, MEM_RESERVE, PAGE_READWRITE);
	cout << ptr << endl;
	if (ptr == nullptr)
	{
		throw std::bad_alloc();
	}

	PageID pageid = (PageID)ptr >> PAGE_SHIFT;
	cout << pageid << endl;

	void* shiftptr = (void*)(pageid << PAGE_SHIFT);
	cout << shiftptr << endl;
}

int main()
{
	//TestThreadCache();
	TestPageCache();

	return 0;
}