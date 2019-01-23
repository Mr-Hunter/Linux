#ifndef __EPOLL__H__
#define __EPOLL__H__

#include "Header.h"
#include "TraceLog.h"
const size_t EpollServer::_MAX_EVENT = 1000;

class EpollServer
{
public:
	EpollServer(int port = 8000)	//socks5Ĭ�϶˿�1080�����ױ��飬���Ի���
		:_port(port)
		, _listenfd(-1)
		, _eventfd(-1)
	{}

	//��һ�����󣬾�Ҫ��һ�£�//�¼������ڴ�����Ӧ��ģʽ��
	void OpenEvent(int fd, int eventd, int how)	   //event ��ɾ���¼�����how��ɾ�Ĳ���
	{
		struct epoll_event event;
		event.events = events;
		event.data.fd = fd;		//�û����ݣ�
		if (epoll_ctl(_eventfd, fd,how, &event) <0)	 //Ϊʲô������fd,
		{

			ErrorDebug("epoll_ctl.fd:%d+how:%d",fd,how);
			return;
		}
	}

	void SetNonBlocking(int fd)
	{
		{
			int flags, s;
			flags = fcntl(fd, F_GETFL, 0);
			if (flags == -1)
				ErrorLog("SetNonBlocking:F_GETFL");
			flags |= O_NONBLOCK;
			s = fcntl(fd, PCF_SETFL, flags);
			if (s == -1)
				ErrorLog("SetNonBlocking:F_SETFL");
		}
	}
	

	//�������񣬿�ʼ����
	void Start();

	//�¼�ѭ��
	void EventLoop();

	//�����д�¼�
	virtual void ConnectEventHandle(int fd) = 0;  //���麯�������������д
	virtual void ReadEventHandle(int fd) = 0;
	virtual void WriteEventHandle(int fd) = 0;
	

	enum Socks5State
	{
		AUTH,//�����֤
		ESTABLISHMENT,//��������
		FORWARDING, //ת��
	};

	//����epollˮƽ������û�յ����´λ���֪ͨ��ͨ�������������Ϣ
	struct Channel
	{
		int _fd;//socket ������
		int _event;//�¼�
		string _buffer; //�����������ڲ�����Ϣ�Ĵ洢

		Channel()
			: _fd(-1)
			, _event(0)
		{}

	};


	//����
	struct Connect
	{
		Connect()
		:_state(AUTH)
		, _ref(0)

		{}
		Socks5State _state;  //����״̬
		Channel _ClientChannel;
		Channel _ServerChannel;
		int _ref;
	};

	void Forwordind(Channel* clientChannel, Channel* serverChannel);

	// ��һ������ָ��ָ�����࣬û�ж�̬���ͻ�ֻ���ø��࣬����������
	//�麯����֤�ȵ������ڵ�����	
	virtual ~EpollServer()
	{
		if (_listenfd == -1)
		{
			close(_listenfd);
		}
	}
private:
	//������
	EpollServer(const EpollServer& e);
	EpollServer& operator=(const EpollServer& e);

protected:
	int _listenfd; //�����׽���
	int _port;	//����˿�

	int _eventfd; //�¼�������
	static const size_t _MAX_EVENT;//�¼������

	map<int, Connect*> _connectMap; //key ����������val��ͨ������
};

#endif //__EPOLL__H__