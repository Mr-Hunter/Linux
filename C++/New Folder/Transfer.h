#ifndef __TRANSFER__H__
#define __TRANSFER__H__

#include "Epoll.h"
//����epoll�Ļ�����򼸸���ҳ�Ͳ�����
//��Ҫtransfer��ԭ���ڱ��أ�����ֱ��ת����û�м��ܣ��ᱻץ�������Լ�һ��ת���������ż��ܣ�ת������
class TransferServer:public EpollServer
{
public:
	TransferServer(const char* Socks5ServerIp,int Socks5ServerPort,int SelfPort = 8000)//����Ҫ����ip���Զ�ʶ���ַ��ȡ
		:EpollServer(SelfPort)
	{
		bzero(&_socks5addr, sizeof(_socks5addr);
		_socks5addr.sin_family = AF_INET;
		_socks5addr.sin_port = htonl(Socks5ServerPort);
		_socks5addr.sin_addr.s_addr = inet_addr(Socks5ServerIp);
	}
	virtual void ConnectEventHandle(int fd);
	virtual void ReadEventHandle(int fd);
	virtual void WriteEventHandle(int fd);

	//���಻дҲ���ԣ�����Ϊ��̳и���������������������������������������
	virtual ~TransferServer()
	{}
protected:
	struct sockaddr_in _socks5addr; //Sock5Server�ĵ�ַ
};

#endif //__TRANSFER__H__