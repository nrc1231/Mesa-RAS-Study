#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

static SOCKET g_sock = INVALID_SOCKET;

// --------------------
// initialize client
// --------------------
void initClient(const char* ip, int port)
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    g_sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    int ret = connect(g_sock, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0) {
        printf("Client connect failed, err=%d\n", WSAGetLastError());
    } else {
        printf("Modelica client connected to %s:%d\n", ip, port);
    }
}

// --------------------
// send raw string
// --------------------
void sendString(const char* msg)
{
    if (g_sock == INVALID_SOCKET) return;

    int len = (int)strlen(msg);
    int sent = send(g_sock, msg, len, 0);

    if (sent != len) {
        printf("sendString failed, err=%d\n", WSAGetLastError());
    }
}

// --------------------
// receive double
// (if you still need it)
// --------------------
double readFromServer()
{
    if (g_sock == INVALID_SOCKET) return 0;

    double val = 0;
    char* p = (char*)&val;

    int got = 0;
    while (got < sizeof(double)) {
        int r = recv(g_sock, p + got, sizeof(double) - got, 0);
        if (r <= 0) {
            printf("readFromServer failed, err=%d\n", WSAGetLastError());
            return 0;
        }
        got += r;
    }
    return val;
}

// --------------------
// receive one line as string
// 对应 Modelica 里的 recvData()
// external "C" y = readFromServerString()
// --------------------
const char* readFromServerString()
{
    static char buf[256];   // 静态缓冲区，Modelica 会拷贝内容
    if (g_sock == INVALID_SOCKET) {
        buf[0] = '\0';
        return buf;
    }

    int idx = 0;
    while (idx < (int)sizeof(buf) - 1) {
        char c;
        int r = recv(g_sock, &c, 1, 0);
        if (r <= 0) {
            printf("readFromServerString failed, err=%d\n", WSAGetLastError());
            buf[0] = '\0';
            return buf;
        }
        if (c == '\n') {
            break;
        }
        buf[idx++] = c;
    }
    buf[idx] = '\0';   // 终止

    // 调试用：
    // printf("readFromServerString got: %s\n", buf);

    return buf;
}
