#pragma warning( disable : 4996)
#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

int main(int argc, char **argv) {
	//加载套接字库，初始化
	WSADATA	wd;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wd)) {
		printf("加载套接字库失败，错误代码%d\n", GetLastError());
		return 0;
	}

	//判断请求的版本是否一致
	if (LOBYTE(wd.wVersion) != 2 || HIBYTE(wd.wVersion) != 2) {
		printf("请求的套接字版本不一致，错误代码%d\n", GetLastError());
	}

	//1.创建套接字socket tcp传输控制协议 SOCK_STREAM   udp用户数据报协议 SOCK_DGRAM
	SOCKET soktclt = socket(AF_INET, SOCK_STREAM, 0);
	if (FALSE == soktclt) {
		printf("创建套接字失败，错误代码%d\n", GetLastError());
	}
	//2.连接 connect
	SOCKADDR_IN Service;
	Service.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	Service.sin_family = AF_INET;
	Service.sin_port = htons(33066);
	SOCKET soktcnct =  connect(soktclt, (SOCKADDR*)&Service, sizeof(SOCKADDR));
	if (soktcnct == SOCKET_ERROR) {
		printf("连接服务器失败，错误代码%d\n", GetLastError());
	}
	char recvMsg[100] = "";
	char sendMsg[100] = "";
	recv(soktclt, recvMsg, 100, 0);
	printf("服务器：%s\n", recvMsg);
	while (1)
	{
		//接收消息
		recv(soktclt, recvMsg, 100, 0);
		if (strcmp(recvMsg, "q!") == 0) {
			printf("%s已退出\n", inet_ntoa(Service.sin_addr));
			printf("输入q!退出:");
			char Buff[20] = "";
			scanf("%s", &Buff);
			if (strcmp(Buff, "q!") == 0) {
				break;
			}
		}
		printf("服务器：%s\n", recvMsg);
		//发送消息
		printf("请输入待发送的信息(按q!退出):");
		scanf("%s", &sendMsg);
		if (strcmp(sendMsg, "q!") == 0) {
			printf("已经结束聊天");
			send(soktclt, "q!", 3, 0);
			break;
			closesocket(soktclt);
		}
		send(soktclt, sendMsg, sizeof(sendMsg), 0);
	}
	return 0;
}
