// Phangiai.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdio.h"
#include "conio.h"
#include "stdlib.h"
#include "winsock2.h"
#include "string.h"
#include "ws2tcpip.h"

#pragma comment (lib,"Ws2_32.lib")
#pragma warning (disable:4996)

int CheckNameorIP(char Addr[NI_MAXHOST]);

int main() {
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData)) {
		printf("Version is not supported!!\n");
		return 0;
	}

	while (1) {
		char Addr[NI_MAXHOST];
		printf("\nEnter the address by name or ip:");
		fflush(stdin);
		gets_s(Addr, NI_MAXHOST);
		if (!strcmp(Addr, "")) {
			printf("\nEnter to finish!");
			break;
		}
		addrinfo* result;
		int rc = getaddrinfo(Addr, "http", NULL, &result);
		if (!CheckNameorIP(Addr)) {
			if (rc) printf("\nNot found information");
			else {
				sockaddr_in Address;
				printf("\nLIST OF IPS:");
				int j = 1;
				while (result != NULL) {						// duyet linked-list
					memcpy(&Address, result->ai_addr, result->ai_addrlen);
					printf("\n%d:%s", j, inet_ntoa(Address.sin_addr));		// ham chuyen so 32bit sang xau IP.
					j++;
					result = result->ai_next;
				}
			}

		}
		else {
			char NameIp[NI_MAXHOST];
			if (rc) printf("\nInvalid IP address");
			else {
				int j = 1;
				printf("\nLIST OF NAME:");
				while (result != NULL) {
					// goi ham getnameinfo de lay ten. 
					int dwRetval = getnameinfo(result->ai_addr, sizeof(struct sockaddr), NameIp, NI_MAXHOST, NULL, NI_MAXSERV, NI_NUMERICSERV);
					printf("\n%d:%s", j, NameIp);
					j++;
					result = result->ai_next;
				}
			}
		}
		freeaddrinfo(result); // free linked-list
	}
	_getch();
	WSACleanup();
	return 0;
}
int CheckNameorIP(char Addr[NI_MAXHOST]) {				// kiem tra xau la ip hay name cua address
	int i = 0;
	while (Addr[i] != NULL && Addr[i] != '.') {
		if (Addr[i] > '9' || Addr[i] < '0')
			return 0;
		i++;
	}
	return 1;
}