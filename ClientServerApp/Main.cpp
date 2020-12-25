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
	HANDLE event1;
	HANDLE event2;
	HANDLE event3;
};

DWORD WINAPI ClientThread(LPVOID lparam) {
	MyOperands *myOperands = static_cast<struct MyOperands*>(lparam);

	myOperands->client.EnterRequest(myOperands->a, myOperands->b);
	SetEvent(myOperands->event1);

	DWORD waitRes = WaitForSingleObject(myOperands->event2, INFINITE);
	if (waitRes == WAIT_FAILED) {
		throw (GetLastError());
	}
	myOperands->client.GetResultOnDisplay(myOperands->result);
	SetEvent(myOperands->event3);

	ExitThread(0);
}

DWORD WINAPI ServerThread(LPVOID lparam) {
	MyOperands* myOperands = static_cast<struct MyOperands*>(lparam);

	std::cout << "Please wait, your request is being processed!" << std::endl;
	Sleep(2000); //load simulation

	myOperands->result = myOperands->server.QueryСomputation(myOperands->a, myOperands->b);

	SetEvent(myOperands->event2);

	ExitThread(0);
}

int main() {
	MyOperands myOperands = {};
	myOperands.event1 = CreateEvent(NULL, TRUE, FALSE, 0);
	myOperands.event2 = CreateEvent(NULL, TRUE, FALSE, 0);
	myOperands.event3 = CreateEvent(NULL, TRUE, FALSE, 0);

	HANDLE hClientThread = CreateThread(0, 0, ClientThread, &myOperands, 0, 0);

	DWORD waitRes = WaitForSingleObject(myOperands.event1, INFINITE);//Waiting for the user to enter their data
	
	HANDLE hServerThread = CreateThread(0, 0, ServerThread, &myOperands, 0, 0);

	waitRes = WaitForSingleObject(myOperands.event2, INFINITE);//Waiting for the data to be sent to the server,
															   //the calculation will be made and the answer will be returned 
	waitRes = WaitForSingleObject(myOperands.event3, INFINITE);//Waiting for the answer to be fully displayed to the user
	
	CloseHandle(myOperands.event1);
	CloseHandle(myOperands.event2);
	CloseHandle(myOperands.event3);
	CloseHandle(hClientThread);
	CloseHandle(hServerThread);

	return 0;
}