#include <Windows.h>
#include <iostream>
#include "ClientClass.h"
#include "ServerClass.h"

struct MyOperands {
	int a;
	int b;
	int result;
	ServerClass server;
	ClientClass client;
	HANDLE event;
};

DWORD WINAPI ClientThread(LPVOID lparam) {
	MyOperands *myOperands = static_cast<struct MyOperands*>(lparam);

	DWORD waitRes = WaitForSingleObject(myOperands->event, INFINITE);
	myOperands->client.EnterRequest(myOperands->a, myOperands->b);
	SetEvent(myOperands->event);

	waitRes = WaitForSingleObject(myOperands->event, INFINITE);
	myOperands->client.GetResultOnDisplay(myOperands->result);
	SetEvent(myOperands->event);

	ExitThread(0);
}

DWORD WINAPI ServerThread(LPVOID lparam) {
	MyOperands* myOperands = static_cast<struct MyOperands*>(lparam);

	DWORD waitRes = WaitForSingleObject(myOperands->event, INFINITE);

	std::cout << "Please wait, your request is being processed!";
	Sleep(2000); //load simulation

	myOperands->result = myOperands->server.QueryСomputation(myOperands->a, myOperands->b);

	SetEvent(myOperands->event);

	ExitThread(0);
}

int main() {
	MyOperands myOperands = {};
	myOperands.event = CreateEvent(NULL, TRUE, FALSE, 0);

	HANDLE hClientThread = CreateThread(0, 0, ClientThread, &myOperands, 0, 0);
	HANDLE hServerThread = CreateThread(0, 0, ServerThread, &myOperands, 0, 0);

	CloseHandle(myOperands.event);
	CloseHandle(hClientThread);
	CloseHandle(hServerThread);

	return 0;
}