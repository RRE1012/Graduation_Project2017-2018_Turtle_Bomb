#include "stdafx.h"


int g_TotalSockets = 0;

DWORD io_size, key;

Socket_Info SocketInfoArray[WSA_MAXIMUM_WAIT_EVENTS];//�������� ���� �������� ����ü�� �迭

WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];//WSAEVENT�� �迭


TB_Map g_TurtleMap; //�� ����2

TB_CharPos char_info[4]; //4���� ĳ���������� ��Ƶ� ĳ���� ���� ->�� ������ �߰��� ��� 2�����迭�� Ȱ���� ����
TB_CharPos ingame_Char_Info[20][4]; //4���� ĳ���������� ��Ƶ� ĳ���� ���� ->�� ������ �߰��� ��� 2�����迭�� Ȱ���� ����

TB_Room room[20];
void Refresh_Map(); //���� �������� �����ϱ� ���� ���� �Լ� -> ��굵 �߰��� ����

BYTE g_total_member = 1;//���� �������� ��

list<TB_BombPos> bomb_list;//��ź ������ �ð��� ����ϱ� ���� ���� ����Ʈ - ��������� �տ����� ��Ʈ������, �� �� ������ �ʿ�

void err_quit(char* msg); //���� ���� �� ��� �Լ�

void err_display(char* msg); //���� ��� �Լ�

BOOL AddSOCKETInfo(SOCKET sock); //������ ���� ���� �Է� �Լ�

void RemoveSocketInfo(int nIndex);//������ �� ���� ���� ���� ���� �Լ�

void err_display(int errcode);//�����ڵ忡 ���� ���� ��� �Լ�

DWORD g_prevTime2; //GetTickCount()�� Ȱ���� �ð��� üũ�� �� ����� �Լ�

void ArrayMap(); //�� �ʱ�ȭ �� ���� �Լ�

void CalculateMap(int,int,byte);

int main(int argc, char* argv[]) {

	printf("-------------------------------------------------------------------------\n\n%d Server Start\n\n-------------------------------------------------------------------------\n\n",sizeof(char));
	
	ArrayMap(); // �� �ʱ�ȭ

	int retval; //recv, �� send �� �����Ʈ�� �޾Ҵ°� ��Ÿ���� ��������

	WSADATA wsa; //���ӵ����� ����

	//�⺻���� wsastartup���� ~ 
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 0;
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		err_quit("socket()");
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(TB_SERVER_PORT);
	retval = ::bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		err_quit("bind()");
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		err_quit("listen()");
	//~Listen ����

	//���� ���� �߰�(WSAEventSelect�� ���� ���� ���� �߰�)&WSAEventSelect
	AddSOCKETInfo(listen_sock);
	retval = WSAEventSelect(listen_sock, EventArray[g_TotalSockets - 1], FD_ACCEPT | FD_CLOSE);
	if (retval == SOCKET_ERROR)
		err_quit("WSAEventSelect()");

	WSANETWORKEVENTS m_NetworkEvents;
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int i, addrlen;

	
	while (1) { //Loop��
		//�̺�Ʈ ��ü �����ϱ�
		DWORD currTime = GetTickCount();
		DWORD elapsedTime = currTime - g_prevTime2;
		g_prevTime2 = currTime;
		//i = WSAWaitForMultipleEvents(g_TotalSockets, EventArray, FALSE, WSA_INFINITE, FALSE);
		i = WSAWaitForMultipleEvents(g_TotalSockets, EventArray, FALSE, 0, FALSE);
		
		//Ÿ�Ӿƿ��� �ɸ� ��� - ��ź �ð��� ��� üũ�ϰ� �־�� �ϹǷ�, ���⼭�� �ð�üũ�� �����Ų��.
		if (i == WSA_WAIT_FAILED || i==258 ||i==WAIT_TIMEOUT) {
			if (bomb_list.size() > 0) {
				list<TB_BombPos>::iterator bomb = bomb_list.begin();
				for (; bomb != bomb_list.end(); ++bomb) {
					bomb->settime = bomb->settime + ((float)elapsedTime / 1000);
					//cout << "Timeout "<<bomb->settime << "  ";
					if (bomb->settime >= 2.0f) {
						int tempx = bomb->posx;
						int tempz = bomb->posz;
						g_TurtleMap.mapInfo[bomb->posx][bomb->posz] = MAP_NOTHING;
						CalculateMap(bomb->posx, bomb->posz, bomb->firepower);
						
						TB_BombExplode temp_bomb = { 11,3, bomb->firepower,tempx ,tempz };
						g_TurtleMap.size = 227;
						for (int j = 0; j < g_TotalSockets; ++j) {
							
							//printf("������ ��ź ����!!!\n");
							
							if (SocketInfoArray[j].m_connected) {
								printf("Send size : %d\n",temp_bomb.size);

								retval = send(SocketInfoArray[j].sock, (char*)&temp_bomb, sizeof(TB_BombExplode), 0);
								printf("Retval size : %d\n", retval);
								printf("Send size : %d\n", g_TurtleMap.size);

								retval = send(SocketInfoArray[j].sock, (char*)&g_TurtleMap, sizeof(TB_Map), 0);
								printf("Retval size : %d\n", retval);
							}

						}
						
						
						bomb_list.pop_front();
						//Refresh_Map();
						if (bomb_list.size() <= 0) {
							break;
						}
						break;
						
					}
				}
			}
			continue;
		}
		else {
			if (bomb_list.size() > 0) {
				list<TB_BombPos>::iterator bomb = bomb_list.begin();
				for (; bomb != bomb_list.end(); ++bomb)
				{
					bomb->settime = bomb->settime + ((float)elapsedTime / 1000);
					//cout << "EventGet " << bomb->settime << "  ";
					if (bomb->settime >= 2.0f)
					{
						g_TurtleMap.mapInfo[bomb->posx][bomb->posz] = MAP_NOTHING;
						CalculateMap(bomb->posx, bomb->posz, bomb->firepower);
						int tempx = bomb->posx;
						int tempz = bomb->posz;
						TB_BombExplode temp_bomb = { 11,3, bomb->firepower,tempx ,tempz };
						g_TurtleMap.size = 227;
						for (int j = 0; j < g_TotalSockets; ++j)
						{

							printf("������ ��ź ����!!!\n");
							if (SocketInfoArray[j].m_connected) {
								printf("Send size : %d\n", temp_bomb.size);
								retval = send(SocketInfoArray[j].sock, (char*)&temp_bomb, sizeof(TB_BombExplode), 0);
								printf("Retval size : %d\n", retval);
								printf("Send size : %d\n", g_TurtleMap.size);

								retval = send(SocketInfoArray[j].sock, (char*)&g_TurtleMap, sizeof(TB_Map), 0);
								printf("Retval size : %d\n", retval);
							}
						}


						bomb_list.pop_front();
						//Refresh_Map();
						if (bomb_list.size() <= 0)
						{
							break;
						}
						break;
					}
				}
			}
			i -= WSA_WAIT_EVENT_0;
			//��ü���� ��Ʈ��ũ �̺�Ʈ �˾Ƴ���
			retval = WSAEnumNetworkEvents(SocketInfoArray[i].sock, EventArray[i], &m_NetworkEvents);
			if (retval == SOCKET_ERROR)
				continue;
			//FD_ACCEPT �̺�Ʈ ó��
			if (m_NetworkEvents.lNetworkEvents&FD_ACCEPT) 
			{
				if (m_NetworkEvents.iErrorCode[FD_ACCEPT_BIT] != 0) 
				{
					err_display(m_NetworkEvents.iErrorCode[FD_ACCEPT_BIT]);
					continue;
				}
				addrlen = sizeof(clientaddr);
				client_sock = accept(SocketInfoArray[i].sock, (SOCKADDR*)&clientaddr, &addrlen);
				if (client_sock == INVALID_SOCKET) 
				{
					err_display("accept()");
					continue;
				}
				printf("[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� =%s, ��Ʈ��ȣ=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
				
				TB_ID tempid = {3,5,g_total_member};
				retval = send(client_sock, (char*)&tempid, sizeof(TB_ID), 0); //ID ����.
				printf("����-%d��° -ID : %d\n", i, g_total_member);

				g_total_member++; // ���� ������ �� üũ
				
				retval = send(client_sock, (char*)&room, sizeof(room), 0); //�ʱ�ȭ�� ������ Ŭ���̾�Ʈ���� ����
				printf("������ ���� :%d����Ʈ\n", retval);
				//retval = send(client_sock, (char*)&g_TurtleMap, sizeof(TB_Map), 0); //�ʱ�ȭ�� ������ Ŭ���̾�Ʈ���� ����
				//printf("������ ���� :%d����Ʈ\n", retval);
				
				if (g_TotalSockets >= WSA_MAXIMUM_WAIT_EVENTS)  //�����ڰ� ������ �ִ��� ���
				{
					printf("[����] �� �̻� ������ �޾Ƶ��� �� �����ϴ�!!!!!\n");
					closesocket(client_sock);
					continue;
				}

				if (retval == SOCKET_ERROR) //send ���� ��
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK) 
					{
						err_display("send()");
						RemoveSocketInfo(i);
					}
					continue;
				}
				
				//���� ���� �߰�
				AddSOCKETInfo(client_sock);

				retval = WSAEventSelect(client_sock, EventArray[g_TotalSockets - 1], FD_READ | FD_WRITE | FD_CLOSE);
				if (retval == SOCKET_ERROR)
					err_quit("WSAEventSelect()-client");

			}
			//Ŭ���̾�Ʈ ��û�� �б⳪ ������ ���
			if (m_NetworkEvents.lNetworkEvents&FD_READ || m_NetworkEvents.lNetworkEvents&FD_WRITE) 
			{
				if (m_NetworkEvents.lNetworkEvents&FD_READ &&m_NetworkEvents.iErrorCode[FD_READ_BIT] != 0) 
				{
					err_display(m_NetworkEvents.iErrorCode[FD_READ_BIT]);
					continue;
				}
				if (m_NetworkEvents.lNetworkEvents&FD_WRITE &&m_NetworkEvents.iErrorCode[FD_WRITE_BIT] != 0) 
				{
					err_display(m_NetworkEvents.iErrorCode[FD_WRITE_BIT]);
					continue;
				}

				Socket_Info* ptr = &SocketInfoArray[i];

				int m_temp_id = 0;

				if (ptr->recvbytes == 0) 
				{
					//������ �ޱ�
					char recv_buf[MAX_BUFF_SIZE];
					retval = recv(ptr->sock, (char*)recv_buf, sizeof(recv_buf), 0);
					char* c_buf = recv_buf;

					if (retval == SOCKET_ERROR) 
					{
						err_display("recv()");
						printf("���� ���� !!\n");
						continue;
					}
					else 
					{
						memcpy(ptr->buf + ptr->remainbytes, c_buf, retval);
						printf("%d����Ʈ ���� !!\n", retval);
						//c_buf[retval] = '\0';
						//ptr->buf[retval + ptr->remainbytes] = '\0';
						//ptr->recvbytes = ptr->recvbytes+retval;
						ptr->remainbytes = ptr->remainbytes + retval;
						//c_buf[ptr->remainbytes] = '\0';
					}




					if (ptr->remainbytes >= 9) 
					{
						switch (c_buf[1]) {
						case CASE_POS: //CharPos
							if (ptr->remainbytes >= 22) {
								TB_CharPos* pos = reinterpret_cast<TB_CharPos*>(c_buf);
								//�ʼ� -
								BYTE tempid = pos->ingame_id-1;
								char_info[tempid].anistate = pos->anistate;
								char_info[tempid].is_alive = pos->is_alive;
								char_info[tempid].posx = pos->posx;
								char_info[tempid].rotY = pos->rotY;
								char_info[tempid].posz = pos->posz;
								//printf("1p�����ǰ�  :x :%f, z:%f , roty:%f \n", char_info[0].posx, char_info[0].posz, char_info[0].rotY);
								//printf("2p�����ǰ�  :x :%f, z:%f , roty:%f \n", char_info[1].posx, char_info[1].posz, char_info[1].rotY);
								//printf("3p�����ǰ�  :x :%f, z:%f , roty:%f \n", char_info[2].posx, char_info[2].posz, char_info[2].rotY);
								//printf("4p�����ǰ�  :x :%f, z:%f , roty:%f \n", char_info[3].posx, char_info[3].posz, char_info[3].rotY);
								ptr->remainbytes -= 22;

								memcpy(c_buf, ptr->buf + 22, ptr->remainbytes);
								memcpy(ptr->buf, c_buf, ptr->remainbytes);

								for (int j = 0; j < g_TotalSockets; ++j) {

									if (SocketInfoArray[j].m_connected) {
										printf("Send size : %d\n", char_info[tempid].size);
										char_info[tempid].size = 22;
										char_info[tempid].type = 1;
										retval = send(SocketInfoArray[j].sock, (char*)&char_info[tempid], sizeof(TB_CharPos), 0);
										printf("Retval size : %d\n", retval);
									}
								}
								

								break;
							}
							break;
						case CASE_BOMB:
							if (ptr->remainbytes >= 11) {
								TB_BombExplode* b_pos = reinterpret_cast<TB_BombExplode*>(c_buf);
								int tempx = b_pos->posx;
								int tempz = b_pos->posz;
								g_TurtleMap.mapInfo[tempz][tempx] = MAP_BOMB;
								//printf("��ź�����ǰ�  :x :%d, z:%d ,  \n", b_pos->posx, b_pos->posz);
								
								BYTE tempfire = b_pos->firepower;
								
								TB_BombPos tempbomb = { sizeof(TB_BombPos),CASE_BOMB,0,tempfire,tempx,tempz,0.0f };
								bomb_list.emplace_back(tempbomb);
								
								ptr->remainbytes -= 11;
								memcpy(ptr->buf, c_buf +11, ptr->remainbytes);
								memset(c_buf, 0, sizeof(c_buf));
 								memcpy(c_buf, ptr->buf, sizeof(ptr->buf));
								//Refresh_Map();
								
								
								for (int j = 0; j < g_TotalSockets; ++j) {
									//��ź�� �޾����Ƿ� ���ŵ� �������� �������ִ� �������� ����
									if (SocketInfoArray[j].m_connected) {
										printf("Send size : %d\n", g_TurtleMap.size);

										retval = send(SocketInfoArray[j].sock, (char*)&g_TurtleMap, sizeof(TB_Map), 0);
										printf("Retval size : %d\n", retval);
										//printf("Bomb�� �߰��� �������� ����!\n");
									}
								}
								break;
							}
							break;
						case CASE_JOINROOM:
							if (ptr->remainbytes >= 12) {
								TB_join* joininfo = reinterpret_cast<TB_join*>(c_buf);
								byte temproomid = joininfo->roomID;
								printf("ID:%d\n", temproomid);
								if (temproomid != 0) {
									bool bool_a = room[temproomid - 1].people_count < room[temproomid - 1].people_max;
									bool bool_b = room[temproomid - 1].game_start != 1;
									bool bool_c = room[temproomid - 1].made == 1;
									if (bool_a&&bool_b&&bool_c) {
										byte tempcount = room[temproomid - 1].people_count;
										byte tempguard = room[temproomid - 1].guardian_pos;
										room[temproomid - 1].people_count += 1;
										for (int j = 0; j < 4; ++j) {
											if (room[temproomid - 1].people_inroom[j] == 0) {
												room[temproomid - 1].people_inroom[j] = joininfo->id;
												tempcount = j + 1;
												break;
											}
										}
										ptr->roomID = temproomid;
										TB_joinRE tempjoin = { 9,9,1,tempcount,tempguard };
										for (int j = 0; j < 4; ++j)
											tempjoin.people_inroom[j] = room[temproomid - 1].people_inroom[j];
										/*for (int j = 0; j < g_TotalSockets; ++j) {
											if (SocketInfoArray[j].m_connected) {
												printf("Send size : %d\n", g_TurtleMap.size);

												retval = send(SocketInfoArray[j].sock, (char*)&tempjoin, sizeof(tempjoin), 0);
												//printf("Retval size : %d\n", retval);
												//printf("Bomb�� �߰��� �������� ����!\n");
											}

										}*/
										retval = send(ptr->sock, (char*)&tempjoin, sizeof(tempjoin), 0);
										printf("send yes!! %d\n",retval);
									}
									else {
										TB_joinRE tempjoin = { 9,9,0 };
										retval = send(ptr->sock, (char*)&tempjoin, sizeof(tempjoin), 0);
										printf("send no! %d\n", retval);
									}
									ptr->remainbytes -= 12;
									memcpy(ptr->buf, c_buf + 12, ptr->remainbytes);
									memset(c_buf, 0, sizeof(c_buf));
									memcpy(c_buf, ptr->buf, sizeof(ptr->buf));
								}
							}
							break;
						case CASE_CREATEROOM:
							if (ptr->remainbytes >= 11) {
								TB_create* createinfo = reinterpret_cast<TB_create*>(c_buf);
								TB_createRE tempa = { 3,10,0 };
								for (auto roominfo : room) {
									if (roominfo.made == 0) {
										tempa.can = 1;
										tempa.roomid = roominfo.roomID;
										break;
									}
								}
								retval = send(ptr->sock, (char*)&tempa, sizeof(TB_createRE), 0);

							}
							break;
						default:
							printf("���� ���� ù ����Ʈ�� : %d\n", c_buf[0]);
							break;
						}
					}

					addrlen = sizeof(clientaddr);

 					getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);

				}
				
				
			}
			//FD_CLOSE �̺�Ʈ ó��
			if (m_NetworkEvents.lNetworkEvents&FD_CLOSE) {
				if (m_NetworkEvents.iErrorCode[FD_CLOSE_BIT] != 0) {
					err_display(m_NetworkEvents.iErrorCode[FD_CLOSE_BIT]);
				}
				RemoveSocketInfo(i);
			}
		}
	}
	

	WSACleanup();
	return 0;

}



void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void err_quit(char* msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}
void err_display(int errcode) {
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[����]%s", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
BOOL AddSOCKETInfo(SOCKET sock) {
	Socket_Info* ptr = &SocketInfoArray[g_TotalSockets];
	if (ptr == NULL) {
		printf("Not enough Memory!!!\n");
		return FALSE;
	}
	WSAEVENT hEvent = WSACreateEvent();
	if (hEvent == WSA_INVALID_EVENT) {
		err_display("WSACreateEvent()");
		return FALSE;
	}
	

	ZeroMemory(ptr->buf, sizeof(ptr->buf));
	//ZeroMemory(ptr->c_buf, sizeof(ptr->c_buf));
	ptr->id = g_total_member;
	ptr->m_getpacket = false;
	ptr->sock = sock;
	ptr->recvbytes = 0;
	ptr->remainbytes = 0;
	ptr->sendbytes = 0;
	
	if(g_TotalSockets==0)
		ptr->m_connected = false;
	else
		ptr->m_connected = true;
	ptr->roomID = 0;
	ptr->bomb = 2;
	ptr->fire = 2;
	ptr->speed = 2;
	ptr->is_guardian = 0;
	ptr->is_ready = 0;
	EventArray[g_TotalSockets] = hEvent;
	++g_TotalSockets;
	printf("��ϿϷ�\n");
	return TRUE;
}
void RemoveSocketInfo(int nIndex) {
	Socket_Info* ptr = &SocketInfoArray[nIndex];
	ptr->m_connected = false;
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);
	printf("TCP���� Ŭ���̾�Ʈ ����:IP �ּ�=%s,��Ʈ��ȣ = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	closesocket(ptr->sock);
	
	WSACloseEvent(EventArray[nIndex]);
	if (nIndex != (g_TotalSockets - 1)) {
		SocketInfoArray[nIndex] = SocketInfoArray[g_TotalSockets - 1];
		EventArray[nIndex] = EventArray[g_TotalSockets - 1];

	}
	--g_TotalSockets;

}










void ArrayMap() {

	for (int j = 0; j < 20; ++j) {
		room[j].game_start = 0;
		room[j].size = 20;
		room[j].type = 8;
		room[j].made = 0;
		room[j].people_count = 0;
		room[j].people_max = 4;
		room[j].roomID = j+1;
		room[j].guardian_pos = 0;
		for (int i = 0; i < 4; ++i)
		{
			room[j].people_inroom[i] = 0;
			ingame_Char_Info[j][i].size = 22;
			ingame_Char_Info[j][i].type = 1;
			ingame_Char_Info[j][i].anistate = 0;
			ingame_Char_Info[j][i].is_alive = 0;
			ingame_Char_Info[j][i].can_kick = 0;
			ingame_Char_Info[j][i].can_throw = 0;
			ingame_Char_Info[j][i].bomb = 2;
			ingame_Char_Info[j][i].fire = 2;
			ingame_Char_Info[j][i].speed = 2;
			char_info[i].size = 22;
			char_info[i].type = 1;
			char_info[i].anistate = 0;
			char_info[i].is_alive = 0;
			char_info[i].can_kick = 0;
			char_info[i].can_throw = 0;
			char_info[i].bomb = 2;
			char_info[i].fire = 2;
			char_info[i].speed = 2;

		}

		ingame_Char_Info[j][0].ingame_id = 1;
		ingame_Char_Info[j][1].ingame_id = 2;
		ingame_Char_Info[j][2].ingame_id = 3;
		ingame_Char_Info[j][3].ingame_id = 4;
		//char_info[0].hp = 10.0f;

		ingame_Char_Info[j][0].posx = 0.0f;
		ingame_Char_Info[j][0].posz = 0.0f;
		ingame_Char_Info[j][0].is_alive = true;
		ingame_Char_Info[j][0].rotY = 0.0f;
		//char_info[1].hp = 10.0f;
		ingame_Char_Info[j][1].posx = 28.0f;
		ingame_Char_Info[j][1].posz = 0.0f;
		ingame_Char_Info[j][1].is_alive = true;
		ingame_Char_Info[j][1].rotY = 0.0f;
		//char_info[2].hp = 10.0f;
		ingame_Char_Info[j][2].posx = 0.0f;
		ingame_Char_Info[j][2].posz = 28.0f;
		ingame_Char_Info[j][2].is_alive = true;
		ingame_Char_Info[j][2].rotY = 180.0f;
		//char_info[3].hp = 10.0f;
		ingame_Char_Info[j][3].posx = 28.0f;
		ingame_Char_Info[j][3].posz = 28.0f;
		ingame_Char_Info[j][3].is_alive = true;
		ingame_Char_Info[j][3].rotY = 180.0f;
	}
	room[0].made = 1;
	room[2].made = 1;
	room[10].made = 1;
	
	room[3].people_inroom[2] = 25;
	room[3].guardian_pos = 3;
	char_info[0].ingame_id = 0;
	char_info[1].ingame_id = 1;
	char_info[2].ingame_id = 2;
	char_info[3].ingame_id = 3;
	//char_info[0].hp = 10.0f;

	char_info[0].posx = 0.0f;
	char_info[0].posz = 0.0f;
	char_info[0].is_alive = true;
	char_info[0].rotY = 0.0f;
	//char_info[1].hp = 10.0f;
	char_info[1].posx = 28.0f;
	char_info[1].posz = 0.0f;
	char_info[1].is_alive = true;
	char_info[1].rotY = 0.0f;
	//char_info[2].hp = 10.0f;
	char_info[2].posx = 0.0f;
	char_info[2].posz = 28.0f;
	char_info[2].is_alive = true;
	char_info[2].rotY = 180.0f;
	//char_info[3].hp = 10.0f;
	char_info[3].posx = 28.0f;
	char_info[3].posz = 28.0f;
	char_info[3].is_alive = true;
	char_info[3].rotY = 180.0f;
	g_TurtleMap.size = 227;
	g_TurtleMap.type = 4;


	for (int x = 0; x < 15; ++x) {
		for (int y = 0; y < 15; ++y) {
			g_TurtleMap.mapInfo[x][y] = 2;

		}

	}

	for (int i = 2; i < 13; ++i) {
		if (rand() % 15 < 1) {
			g_TurtleMap.mapInfo[i][0] =MAP_BUSH;
		}
		else if (rand() % 15 < 5) {
			g_TurtleMap.mapInfo[i][0] = 2;
		}
		else if (rand() % 15 < 9) {
			g_TurtleMap.mapInfo[i][0] = 3;
		}
		else if (rand() % 15 < 13) {
			g_TurtleMap.mapInfo[i][0] = 4;
		}
		else
		{
			g_TurtleMap.mapInfo[i][0] = 5;
		}
		if (rand() % 15 < 1) {
			g_TurtleMap.mapInfo[i][1] =MAP_BUSH;
		}
		else if (rand() % 15 < 5) {
			g_TurtleMap.mapInfo[i][1] = 2;
		}
		else if (rand() % 15 < 9) {
			g_TurtleMap.mapInfo[i][1] = 3;
		}
		else if (rand() % 15 < 13) {
			g_TurtleMap.mapInfo[i][1] = 4;
		}
		else {
			g_TurtleMap.mapInfo[i][1] = 5;
		}
		
		if (rand() % 15 < 1) {
			g_TurtleMap.mapInfo[i][14] =MAP_BUSH;
		}
		else if (rand() % 15 < 5) {
			g_TurtleMap.mapInfo[i][14] = 2;
		}
		else if (rand() % 15 < 9) {
			g_TurtleMap.mapInfo[i][14] = 3;
		}
		else if (rand() % 15 < 13) {
			g_TurtleMap.mapInfo[i][14] = 4;
		}
		else {
			g_TurtleMap.mapInfo[i][14] = 5;
		}
		if (rand() % 15 < 1) {
			g_TurtleMap.mapInfo[i][13] =MAP_BUSH;
		}
		else if (rand() % 15 < 5) {
			g_TurtleMap.mapInfo[i][13] = 2;
		}
		else if (rand() % 15 < 9) {
			g_TurtleMap.mapInfo[i][13] = 3;
		}
		else if (rand() % 15 < 13) {
			g_TurtleMap.mapInfo[i][13] = 4;
		}
		else {
			g_TurtleMap.mapInfo[i][13] = 5;
		}
		if (rand() % 15 < 1) {
			g_TurtleMap.mapInfo[0][i] =MAP_BUSH;
		}
		else if (rand() % 15 < 5) {
			g_TurtleMap.mapInfo[0][i] = 2;
		}
		else if (rand() % 15 < 9) {
			g_TurtleMap.mapInfo[0][i] = 3;
		}
		else if (rand() % 15 < 13) {
			g_TurtleMap.mapInfo[0][i] = 4;
		}
		else {
			g_TurtleMap.mapInfo[0][i] = 5;
		}
		if (rand() % 15 < 1) {
			g_TurtleMap.mapInfo[1][i] =MAP_BUSH;
		}
		else if (rand() % 15 < 5) {
			g_TurtleMap.mapInfo[1][i] = 2;
		}
		else if (rand() % 15 < 9) {
			g_TurtleMap.mapInfo[1][i] = 3;
		}
		else if (rand() % 15 < 13) {
			g_TurtleMap.mapInfo[1][i] = 4;
		}
		else {
			g_TurtleMap.mapInfo[1][i] = 5;
		}
		if (rand() % 15 < 1) {
			g_TurtleMap.mapInfo[13][i] =MAP_BUSH;
		}
		else if (rand() % 15 < 5) {
			g_TurtleMap.mapInfo[13][i] = 2;
		}
		else if (rand() % 15 < 9) {
			g_TurtleMap.mapInfo[13][i] = 3;
		}
		else if (rand() % 15 < 13) {
			g_TurtleMap.mapInfo[13][i] = 4;
		}
		else {
			g_TurtleMap.mapInfo[13][i] = 5;
		}
		if (rand() % 15 < 1) {
			g_TurtleMap.mapInfo[14][i] =MAP_BUSH;
		}
		else if (rand() % 15 < 5) {
			g_TurtleMap.mapInfo[14][i] = 2;
		}
		else if (rand() % 15 < 9) {
			g_TurtleMap.mapInfo[14][i] = 3;
		}
		else if (rand() % 15 < 13) {
			g_TurtleMap.mapInfo[14][i] = 4;
			
		}
		else {
			g_TurtleMap.mapInfo[14][i] = 5;
			
		}

		for (int j = 2; j < 13; ++j) {
			if (rand() % 15 < 1) {
				g_TurtleMap.mapInfo[i][j] =MAP_BUSH;
			}
			else if (rand() % 15 < 5) {
				g_TurtleMap.mapInfo[i][j] = 2;
			}
			else if (rand() % 15 < 9) {
				g_TurtleMap.mapInfo[i][j] = 3;
			}
			else if (rand() % 15 < 13) {
				g_TurtleMap.mapInfo[i][j] = 4;
			}

			else
			{
				g_TurtleMap.mapInfo[i][j] = 5;
			}
		}


	}

	for (int y = 0; y < 15; ++y) {
		for (int x = 0; x < 15; ++x) {
			if (g_TurtleMap.mapInfo[14 - y][x] == 1) {
				printf("��  ");
			}
			else if (g_TurtleMap.mapInfo[14 - y][x] == 2) {
				printf("N  ");
			}
			else if (g_TurtleMap.mapInfo[14 - y][x] == 3) {
				printf("C  ");
			}
			else if (g_TurtleMap.mapInfo[14 - y][x] == 4) {
				printf("R  ");
			}
			else if (g_TurtleMap.mapInfo[14 - y][x] == 5) {
				printf("I  ");
			}
			else if (g_TurtleMap.mapInfo[14 - y][x] == 6) {
				printf("B  ");
			}

		}
		printf("\n");
	}

}

void Refresh_Map() {
	system("cls");
	for (int y = 0; y < 15; ++y) {
		for (int x = 0; x < 15; ++x) {
			if (g_TurtleMap.mapInfo[14-y][x] == 1) {
				printf("��  ");
			}
			else if (g_TurtleMap.mapInfo[14 - y][x] == 2) {
				printf("N  ");
			}
			else if (g_TurtleMap.mapInfo[14 - y][x] == 3) {
				printf("C  ");
			}
			else if (g_TurtleMap.mapInfo[14 - y][x] == 4) {
				printf("R  ");
			}
			else if (g_TurtleMap.mapInfo[14 - y][x] == 5) {
				printf("I  ");
			}
			else if (g_TurtleMap.mapInfo[14 - y][x] == 6) {
				printf("B  ");
			}
			else if (g_TurtleMap.mapInfo[14 - y][x] == 7) {
				printf("F  ");
			}
			else if (g_TurtleMap.mapInfo[14 - y][x] == 8) {
				printf("S  ");
			}

		}
		printf("\n");
	}

}

void CalculateMap(int x, int z, byte f) {
	bool l_UpBlock = false;
	bool l_DownBlock = false;
	bool l_LeftBlock = false;
	bool l_RightBlock = false;

	BYTE tempMap[15][15];
	memcpy(tempMap, g_TurtleMap.mapInfo, sizeof(tempMap));
	tempMap[z][x] = MAP_NOTHING;
	for (byte b = 1; b <= f; ++b) {
		if (!l_DownBlock) {
			if (z - b < 0) {
				l_DownBlock = true;
			}
			else {
				if (tempMap[z - b][x] == MAP_BOMB ) {
					tempMap[z - b][x] = MAP_NOTHING;
					l_DownBlock = true;
				}
				else if (tempMap[z - b][x] == MAP_BOX) {
					int temp_rand = (rand() % 10);
					if(temp_rand<4)
						tempMap[z - b][x] = MAP_NOTHING;
					else if(temp_rand>=4&&temp_rand<=5)
						tempMap[z - b][x] = MAP_ITEM;
					else if (temp_rand >= 6 && temp_rand <= 7)
						tempMap[z - b][x] = MAP_ITEM_F;
					else if (temp_rand >= 8 && temp_rand <= 9)
						tempMap[z - b][x] = MAP_ITEM_S;

					l_DownBlock = true;
				}
				else if (tempMap[z - b][x] == MAP_ITEM || tempMap[z - b][x] == MAP_ITEM_F || tempMap[z - b][x] == MAP_ITEM_S || tempMap[z - b][x] == MAP_BUSH) {
					tempMap[z - b][x] = MAP_NOTHING;
				}
				else if (tempMap[z - b][x] == MAP_ROCK ) {
					l_DownBlock = true;
				}
			}
		}
		if (!l_UpBlock) {
			if (z + b > 14) {
				l_UpBlock = true;
			}
			else {
				if (tempMap[z + b][x] == MAP_BOMB ) {
					tempMap[z + b][x] = MAP_NOTHING;
					l_UpBlock = true;
				}
				else if (tempMap[z + b][x] == MAP_BOX) {
					int temp_rand = (rand() % 10);
					if (temp_rand<4)
						tempMap[z + b][x] = MAP_NOTHING;
					else if (temp_rand >= 4 && temp_rand <= 5)
						tempMap[z + b][x] = MAP_ITEM;
					else if (temp_rand >= 6 && temp_rand <= 7)
						tempMap[z + b][x] = MAP_ITEM_F;
					else if (temp_rand >= 8 && temp_rand <= 9)
						tempMap[z + b][x] = MAP_ITEM_S;

					l_UpBlock = true;
				}
				else if (tempMap[z + b][x] == MAP_ITEM || tempMap[z + b][x] == MAP_ITEM_F || tempMap[z + b][x] == MAP_ITEM_S || tempMap[z + b][x] == MAP_BUSH) {
					tempMap[z + b][x] = MAP_NOTHING;
				}
				else if (tempMap[z + b][x] == MAP_ROCK) {
					l_UpBlock = true;
				}
			}
		}
		if (!l_LeftBlock) {
			if (x - b < 0) {
				l_LeftBlock = true;
			}
			else {
				if (tempMap[z][x-b] == MAP_BOMB) {
					tempMap[z][x-b] = MAP_NOTHING;
					l_LeftBlock = true;
				}
				else if (tempMap[z][x - b] == MAP_BOX) {
					int temp_rand = (rand() % 10);
					if (temp_rand<4)
						tempMap[z][x - b] = MAP_NOTHING;
					else if (temp_rand >= 4 && temp_rand <= 5)
						tempMap[z][x - b] = MAP_ITEM;
					else if (temp_rand >= 6 && temp_rand <= 7)
						tempMap[z][x - b] = MAP_ITEM_F;
					else if (temp_rand >= 8 && temp_rand <= 9)
						tempMap[z][x - b] = MAP_ITEM_S;

					l_LeftBlock = true;
				}
				else if (tempMap[z][x-b] == MAP_ITEM || tempMap[z][x-b] == MAP_BUSH|| tempMap[z][x - b] == MAP_ITEM_F || tempMap[z][x - b] == MAP_ITEM_S) {
					tempMap[z][x-b] = MAP_NOTHING;
				}
				else if (tempMap[z][x-b] == MAP_ROCK) {
					l_LeftBlock = true;
				}
			}
		}
		if (!l_RightBlock) {
			if (x + b > 14) {
				l_RightBlock = true;
			}
			else {
				if (tempMap[z][x+b] == MAP_BOMB ) {
					tempMap[z][x+b] = MAP_NOTHING;
					l_RightBlock = true;
				}
				else if (tempMap[z][x + b] == MAP_BOX) {
					int temp_rand = (rand() % 10);
					if (temp_rand<4)
						tempMap[z][x + b] = MAP_NOTHING;
					else if (temp_rand >= 4 && temp_rand <= 5)
						tempMap[z][x + b] = MAP_ITEM;
					else if (temp_rand >= 6 && temp_rand <= 7)
						tempMap[z][x + b] = MAP_ITEM_F;
					else if (temp_rand >= 8 && temp_rand <= 9)
						tempMap[z][x + b] = MAP_ITEM_S;

					l_RightBlock = true;
				}
				else if (tempMap[z][x + b] == MAP_ITEM || tempMap[z][x + b] == MAP_BUSH || tempMap[z][x + b] == MAP_ITEM_F || tempMap[z][x + b] == MAP_ITEM_S) {
					tempMap[z][x+b] = MAP_NOTHING;
				}
				else if (tempMap[z][x+b] == MAP_ROCK) {
					l_RightBlock = true;
				}
			}
		}
		

	}
	g_TurtleMap.type = 4;
	memcpy( g_TurtleMap.mapInfo, tempMap, sizeof(tempMap));

}