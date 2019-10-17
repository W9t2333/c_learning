#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

int main(int argc, char **argv) {
	//加载套接字库，初始化
	WSADATA	wd;
	int connectNum = 5;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wd)) {
		printf("加载套接字库失败，错误代码%d\n",GetLastError());
		return 0;
	}

	//判断请求的版本是否一致
	if (LOBYTE(wd.wVersion) != 2 || HIBYTE(wd.wVersion) != 2) {
		printf("请求的套接字版本不一致，错误代码%d\n",GetLastError());
	}

	//1.创建套接字 tcp传输控制协议 SOCK_STREAM   udp用户数据报协议 SOCK_DGRAM
	SOCKET soktsv = socket(AF_INET, SOCK_STREAM, 0);
	if (soktsv == INVALID_SOCKET) {
		printf("创建套接字失败");
	}

	//2.绑定socket和端口号
	SOCKADDR_IN service;
	service.sin_family = AF_INET;
	service.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	service.sin_port = htons(33066);
	if (SOCKET_ERROR == bind(soktsv, (SOCKADDR*)&service, sizeof(SOCKADDR))) {
		printf("绑定套接字失败，错误代码%d\n",GetLastError());
	}

	//3.监听listen 其中第二个参数backlog代表能够接收的最多的连接数
	if (SOCKET_ERROR == listen(soktsv, connectNum)) {
		printf("绑定套接字失败，错误代码%d\n", GetLastError());
	}

	//4.接受链接 accept
	SOCKADDR_IN accpt;
	int len = sizeof(SOCKADDR);
	while (1)
	{
		SOCKET soktaccpt = accept(soktsv, (SOCKADDR*)&accpt, &len);
		char sendBuff[100]="";
		char recvBuff[100];
		printf("Client(%s:%d): connect success\n", inet_ntoa(accpt.sin_addr), service.sin_port);
		sprintf(sendBuff, "您的ip为%s", inet_ntoa(accpt.sin_addr));
		send(soktaccpt, sendBuff, strlen(sendBuff), 0);
		while (1)
		{
			printf("请输入待发送的信息(按q退出)");
			scanf("%s", &sendBuff);
			if (strcmp(sendBuff, "q") == 0) {
				printf("已经结束聊天");
				sprintf(sendBuff, "%s已退出", inet_ntoa(accpt.sin_addr));
				break;
				closesocket(soktaccpt);
			}
			send(soktaccpt, sendBuff, sizeof(sendBuff), 0);
			recv(soktaccpt, recvBuff, 100, 0);
			printf("%s：%s\n", inet_ntoa(accpt.sin_addr),recvBuff);
		}
		//5.关闭链接 close
		SOCKET soktcls = closesocket(soktsv);
		if (soktcls != SOCKET_ERROR) { break; }
	}
	return 0;
}
