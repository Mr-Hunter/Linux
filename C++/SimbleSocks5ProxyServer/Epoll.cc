#include "Epoll.h"
#include "Header.h"

//�������񣬿�ʼ����
void EpollServer::Start()
{
	_listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_listenfd == -1)
	{
		ErrorLog("create socket");
		return;
	}
	
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htonl(_port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);//һ���������ܶ����������ÿ��������Ϊ�˼���

	if (bind(_listenfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		ErrorLog("bind socket");
		return;
	}
	
	if (listen(_listenfd, 100) < 0)
	{
		ErrorLog("listen socket");
		return;
	}

	//event
	_eventfd = epoll_create(_MAX_EVENT);
	if (_eventfd == -1)
	{
		ErrorLog("create epoll");
		return;
	}
	//���һ�����¼�
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = _eventfd;

	if (epoll_ctl(_eventfd, EPOLL_CTL_ADD, _listenfd, event) < 0) //Ϊʲô_listenffd������
	{
		ErrorLog("EPOLL_CTL_ADD listenfd EPOLLIN");
		return;
	}

	//���listenfd��epoll�����������¼�
	OpenEvent(_listenfd, EPOLLIN, EPOLL_CTL_ADD);

	//�¼�ѭ������epoll������ѭ������
	EventLoop();
}
//�¼�ѭ��
void EpollServer::EventLoop()
{
	struct epoll_event events[_MAX_EVENT];
	while (1)
	{
		int ret = epoll_wait(_eventfd, events, _MAX_EVENT, 0); //�����¼���
		if (ret == -1)
		{
			ErrorLog("epoll wait");
			return;
		}

		for (size_t i = 0; i < ret; ++i)
		{
			if (events[i].data.fd == _listenfd)
			{
				struct sockaddr addr;
				socklen_t len;
				int connectfd = accept(_listenfd, (struct sockaddr*)&addr, &len);
				if (connectfd < 0)
				{
					ErrorLog("accept error ");
					continue;
				}
				//��˭��������
				TraceLog("client connect");
				ConnectEventHandle(connectfd);
			}
			//���ܻ���������׽��ֶ��¼�
			else if (events[i].events&EPOLLIN)
			{
				ReadEventHnadle(events[i].data.fd);
			}
			//���ܻ���������׽���д�¼�
			else if (events[i].events&EPOLLOUT)
			{
				WriteEventHandle(events[i].data.fd)
			}
			else
			{
				ErrorLog("events error");
			}
		}
	}


}
void EpollServer::Forwordind(Channel* clientChannel, Channel* serverChannel)
{
	const size_t BUF = 4096;
	char buffer[BUF];
	int rlen = recv(clientChannel->_fd, buffer, sizeof(buffer),0);
	if (rlen > 0)
	{
		int slen = send(serverChannel->_fd,buffer,rlen,0);
		if (slen < rlen)
		{
			SendInLoop() //����ʱ��ѭ������������
		}
	}
	else if (rlen == 0)
}