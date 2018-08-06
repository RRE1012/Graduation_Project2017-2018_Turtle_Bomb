#pragma once
#define WIN32_LEAN_AND_MEAN  
#define INITGUID

#include <WinSock2.h>
#include <windows.h>   // include important windows stuff

#pragma comment(lib, "ws2_32.lib")
#include <thread>
#include <vector>
#include <array>
#include <iostream>
#include <unordered_set>
#include <mutex>

#include <vector>
#include <set>
#include <thread>
#include <mutex>
#include <queue>
#include <stack>
#include <set>
#include <map>

#include <list>
#include <thread>
#include <string>
#include <chrono>
#include <WinSock2.h>
#include <Windows.h>
#include <random>
#include <cmath>

#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include<fstream>
#include<iterator>

#include <math.h>
using namespace std;
#define MAX_BUFF_SIZE   4000 // ������ ������ ��������.
#define MAX_PACKET_SIZE  255


#define BOARD_WIDTH   400
#define BOARD_HEIGHT  400

#define DB_HOST "turtlebombdb.cpcqdyzbke8r.ap-northeast-2.rds.amazonaws.com"
#define DB_USER "goal1012"
#define DB_PASS "ahr618vy"
#define DB_NAME "TurtleBombDB"
#define DB_PORT 3306
#define SHOW_PLAYER_POS_X 10
#define SHOW_PLAYER_POS_Y 10

#define VIEW_RADIUS   3

#define MAX_USER 500

#define NPC_START  1000
#define NUM_OF_NPC  10000

#define MY_SERVER_PORT  9000 // ��Ʈ�� ����/Ŭ�� �����ؾ���.

#define MAX_STR_SIZE  100

#define CS_UP    1
#define CS_DOWN  2
#define CS_LEFT  3
#define CS_RIGHT    4
#define CS_CHAT      5

#define SC_POS           1
#define SC_PUT_PLAYER    2
#define SC_REMOVE_PLAYER 3
#define SC_CHAT      4

//TBServer 
#define MAX_USER_INROOM 4
#define MAX_NPC   50
#define CASE_POS 1
#define CASE_BOMB 2
#define CASE_BOMB_EX 3
#define CASE_MAP 4
#define CASE_ID 5
#define CASE_ITEM_GET 6
#define CASE_DEAD 7
#define CASE_ROOM 8

#define CASE_JOINROOM 9
#define CASE_CREATEROOM 10
#define CASE_READY 11
#define CASE_STARTGAME 12
#define CASE_OUTROOM 13
#define CASE_GAMESET 15

#define CASE_THROWBOMB 17
#define CASE_KICKBOMB 18
#define CASE_THROWCOMPLETE 19
#define CASE_KICKCOMPLETE 20
#define CASE_MAPSET 21
#define CASE_BOXPUSH 22
#define CASE_BOXPUSHCOMPLETE 23
#define CASE_TIME 24
#define CASE_BOMBSET 25
#define CASE_CONNECTSUCCESS 26
#define CASE_DB1 27

#define CASE_GAMEREADY 28
#define CASE_MATCH 14
#define CASE_BOSS 16


#define SIZEOF_TB_GameStartRE_Cop 84
#define SIZEOF_TB_CharPos 22
#define SIZEOF_TB_BossPos 18
#define SIZEOF_TB_GAMEReady 5
#define SIZEOF_TB_BombPos 17
#define SIZEOF_TB_BombExplode 13
#define SIZEOF_TB_BombExplodeRE 15
#define SIZEOF_TB_MAP 227
#define SIZEOF_TB_ID 3
#define SIZEOF_TB_ItemGet 13
#define SIZEOF_TB_GetItem 4
#define SIZEOF_TB_DEAD 3
#define SIZEOF_TB_GAMEEND 4
#define SIZEOF_TB_Room 31
#define SIZEOF_TB_join 12
#define SIZEOF_TB_joinRE 10
#define SIZEOF_TB_create  12
#define SIZEOF_TB_createRE  4
#define SIZEOF_TB_GameStart 4
#define SIZEOF_TB_GameStartRE 3
#define SIZEOF_CASE_READY 5
#define SIZEOF_TB_ReadyRE 5
#define SIZEOF_TB_RoomOut 5
#define SIZEOF_TB_RoomOutRE 3
#define SIZEOF_TB_GetOut  4
#define SIZEOF_TB_GetOutRE  2
#define SIZEOF_TB_RoomSetting 6
#define SIZEOF_TB_TeamSetting 5
#define SIZEOF_TB_ThrowBomb 13
#define SIZEOF_TB_ThrowBombRE 20
#define SIZEOF_TB_ThrowComplete 11
#define SIZEOF_TB_MapSetRE 11
#define SIZEOF_TB_BoxPush 13
#define SIZEOF_TB_BoxPushComplete 11
#define SIZEOF_TB_BoxPushRE 21
#define SIZEOF_TB_Time 6
#define MAX_EVENT_SIZE 64
#define SIZEOF_TB_Connect_Success 2
#define SIZEOF_TB_IDPW 44
#define SIZEOF_TB_DBInfo_1 43

#define MAP_NOTHING 0
#define MAP_BOX 1
#define MAP_ROCK 2

#define MAP_BUSH 3
#define MAP_ITEM 11
#define MAP_BOMB 5
#define MAP_ITEM_F 13
#define MAP_ITEM_S 12
#define MAP_FIREBUSH 10
#define MAP_KICKITEM 14
#define MAP_THROWITEM 15
#define MAP_CHAR 7
#define MAP_ENEMY 8
#define MAP_BOSS 9


#define BASIC_POSX_CHAR1 0
#define BASIC_POSZ_CHAR1 0
#define BASIC_POSX_CHAR2 0
#define BASIC_POSZ_CHAR2 0
#define BASIC_POSX_CHAR3 0
#define BASIC_POSZ_CHAR3 0
#define BASIC_POSX_CHAR4 0
#define BASIC_POSZ_CHAR4 0

#define TURTLE_ANI_IDLE 0
#define TURTLE_ANI_WALK 1
#define TURTLE_ANI_HIDE 2
#define TURTLE_ANI_DEAD 3
#define TURTLE_ANI_KICK 4
#define TURTLE_ANI_PUSH 5

#pragma pack (push, 1)

struct cs_packet_up {
	unsigned char size;
	unsigned char type;
};

struct cs_packet_down {
	unsigned char size;
	unsigned char type;
};

struct cs_packet_left {
	unsigned char size;
	unsigned char type;
};

struct cs_packet_right {
	unsigned char size;
	unsigned char type;
};

struct cs_packet_chat {
	unsigned char size;
	unsigned char type;
	wchar_t message[MAX_STR_SIZE];
};

struct sc_packet_pos {
	unsigned char size;
	unsigned char type;
	unsigned short id;
	unsigned short x;
	unsigned short y;
};

struct sc_packet_put_player {
	unsigned char size;
	unsigned char type;
	unsigned short id;
	unsigned short x;
	unsigned short y;
};
struct sc_packet_remove_player {
	unsigned char size;
	unsigned char type;
	short id;
};

struct sc_packet_chat {
	unsigned char size;
	unsigned char type;
	short id;
	wchar_t message[MAX_STR_SIZE];
};





struct Socket_Info {
	SOCKET sock;
	bool m_connected;
	bool m_getpacket;
	char buf[MAX_BUFF_SIZE];

	int type;
	int id;
	int recvbytes;

	int sendbytes;
	int remainbytes;
	unsigned char roomID; //����Ʈ�� 0. �� ���ٴ� ��
	unsigned char is_guardian; //�������� �ƴ��� 
	unsigned char is_ready;
	//�߰�
	unsigned char fire;
	unsigned char bomb;
	unsigned char speed;
	unsigned char pos_inRoom;
};

struct TB_MatchingInfo {
	unsigned char size; //7
	unsigned char type; //14
	unsigned char m_id;
	unsigned char difficulty;
	unsigned char boss_type;
	unsigned char map_type;

	
	unsigned char prefer_man; //����� ��ȣ�ϴ��� - 0 �������, �ٸ� ���ڴ� ��ȣ�ϴ� �� �� - �ִ� 4
};
struct TB_MatchingInfo_RE {
	unsigned char size; //3
	unsigned char type;//14
	unsigned char match;
};

struct TB_GAMEReady {
	unsigned char size;  //5
	unsigned char type; //28
	unsigned char imready; //1�̸� ����,0�̸� noready
	unsigned char roomid;
	unsigned char myid;
};
struct TB_DBInfo_1 {
	unsigned char size; //43
	unsigned char type; //27
	unsigned char connect; //2�̸� ���̵� �����Ϸ�, 1�̸� ���̵� ���� ����, 0�� ���̵� ���� ����
	char id_string[20];

	
	int win;
	int lose;
	int tier;
	int exp;
	int exp_max;

};
struct TB_Connect_Success {
	unsigned char size; //2
	unsigned char type; //26

};
struct TB_IDPW {
	unsigned char size; //44
	unsigned char type; //26]
	unsigned char m_id; //26]
	unsigned char m_type; //1�̸� �α���, 2�� ����
	char id[20];
	char pw[20];
};

struct GameCharInfo {
	unsigned char speed;
	unsigned char fire;
	unsigned char bomb;

};

struct TB_CharPos {//type:1
	unsigned char size; //22
	unsigned char type;
	unsigned char ingame_id;
	unsigned char anistate;
	unsigned char is_alive;
	unsigned char room_id;
	unsigned char fire;
	unsigned char bomb;
	unsigned char can_throw;
	unsigned char can_kick;
	float posx;
	float posz;
	float rotY;
};
struct TB_BossPos {//type:1
	unsigned char size; //18
	unsigned char type;//16

	unsigned char anistate;
	unsigned char is_alive;
	unsigned char targetid;
	unsigned char room_id;
	
	float posx;
	float posz;
	float rotY;
};
struct TB_BombPos { //type:2
	unsigned char size;//17
	unsigned char type;
	unsigned char ingame_id;
	unsigned char firepower; //ȭ��
							 //unsigned char throwing; //�������� �ִ���
							 //unsigned char kicking; //�������� �ִ���
	unsigned char room_num;
	int posx;
	int posz;
	float settime;
};
struct TB_BombSetRE {
	unsigned char size;//11
	unsigned char type;//25
	unsigned char f_power;
	int posx;
	int posz;
};
struct TB_MapSetRE {
	unsigned char size;//11
	unsigned char type;//21
	unsigned char m_type;
	int posx;
	int posz;
};
struct TB_BombExplode { //type:3
	unsigned char size;//13
	unsigned char type;
	unsigned char firepower;
	unsigned char room_id;
	unsigned char game_id;
	int posx;
	int posz;

};
struct TB_BombExplodeRE { //type:3
	unsigned char size;//15
	unsigned char type;
	unsigned char upfire;
	unsigned char rightfire;
	unsigned char downfire;
	unsigned char leftfire;
	unsigned char gameID;
	int posx;
	int posz;

};

struct TB_ThrowBomb {
	unsigned char size;//13
	unsigned char type;//17
	unsigned char roomid;
	unsigned char ingame_id;
	unsigned char direction;
	int posx;
	int posz;
};
struct TB_ThrowBombRE {
	unsigned char size;//20
	unsigned char type;//17
	unsigned char direction;
	unsigned char ingame_id;
	int posx;
	int posz;
	int posx_re;
	int posz_re;

};

struct TB_ThrowComplete {
	unsigned char size;//11
	unsigned char type;//19
	unsigned char roomid;
	int posx;
	int posz;
};

struct TB_KickBomb {
	unsigned char size;//13
	unsigned char type;//18
	unsigned char roomid;
	unsigned char ingame_id;
	unsigned char direction;
	int posx;
	int posz;
};
struct TB_KickComplete {
	unsigned char size;//11
	unsigned char type;//20
	unsigned char roomid;
	int posx;
	int posz;
};
struct TB_KickBombRE {
	unsigned char size;//21
	unsigned char type;//18
	unsigned char kick;
	unsigned char ingame_id;
	unsigned char direction;
	int posx;
	int posz;
	int posx_re;
	int posz_re;
};
struct TB_BoxPush {
	unsigned char size;//13
	unsigned char type;//22
	unsigned char roomid;
	unsigned char ingame_id;
	unsigned char direction;
	int posx;
	int posz;

};
struct TB_BoxPushRE {
	unsigned char size;//21
	unsigned char type;//22
	unsigned char push; //0�̸� �ȹо�, 1�̸� �о�~!
	unsigned char ingame_id;
	unsigned char direction;
	int posx;
	int posz;
	int posx_d;
	int posz_d;

};

struct TB_BoxPushComplete {
	unsigned char size;//11
	unsigned char type;//19
	unsigned char roomid;
	int posx;
	int posz;
};
struct TB_ReGame {

};
struct TB_Map { //type:4
	unsigned char size;//227
	unsigned char type;
	unsigned char mapInfo[15][15];

};
struct TB_ID {//type:5
	unsigned char size; //3
	unsigned char type;
	unsigned char id;  //0330 ���� int����- BYTE�� ����
};


struct TB_ItemGet { //type:6
	unsigned char size; //13
	unsigned char type;//6
	unsigned char room_id;
	unsigned char ingame_id;
	unsigned char item_type;
	int posx;
	int posz;
};

struct TB_GetItem { //send : type 6 ���� ����-> Ŭ�� ����
	unsigned char size; //4
	unsigned char type;//6

	unsigned char ingame_id;
	unsigned char itemType; //Ÿ�Կ� ���� �ٸ� ������ ��µȴ� + �ɷ��� ������.

};
struct TB_DEAD { //�׾��� �� �˷��ִ� ��Ŷ
	unsigned char size; //3
	unsigned char type;//7
	unsigned char game_id; //���� �׾���!

};

struct TB_GAMEEND {
	unsigned char size; //3
	unsigned char type;//15
	unsigned char winner_id; //���� �̰峪!
	unsigned char loser_id;
};


struct TB_UserInfo { //�������� - type: 7
	unsigned char size; //9
	unsigned char type;
	unsigned char id; //�ΰ��� id�ʹ� �ٸ���.
	unsigned char roomID; //����Ʈ�� 0. �� ���ٴ� ��
	unsigned char is_guardian;//�������� �ƴ��� 
	unsigned char is_ready;
	//�߰�
	unsigned char fire;
	unsigned char bomb;
	unsigned char speed;

};
//���������� �ƴ� ����ü�� �ʵ����͸� ���� �� ����ü �ۼ�

struct TB_Room { //���� �߰�(��)
	unsigned char size; //31
	unsigned char type;//8
	unsigned char roomID;
	unsigned char people_count;
	unsigned char game_start; //���� ���� 1
	unsigned char people_max; //�ִ� �ο� ��
	unsigned char made; //������� ���ΰ�? 0-�� �������, 1- �������(����), 2-�������(�����)
	unsigned char guardian_pos; //�迭�� ���� �� -1�� ��
	unsigned char people_inroom[4];
	unsigned char roomstate;  //�����ΰ� �������ΰ�? 0-������ 1-����
	unsigned char map_thema;
	unsigned char map_mode;
	unsigned char team_inroom[4];
	unsigned char ready[4];
	char password[8];
};

struct TB_RoomInfo {
	unsigned char room_num;
	unsigned char people_count;
	unsigned char people_max;
	unsigned char game_start;
};
//���� ���Դϴ�.
//�������Դϴ�. 
struct TB_Ready {
	unsigned char size; //5
	unsigned char type;//11
	unsigned char room_num;
	unsigned char pos_in_room;
	unsigned char will_ready; // 0�̸� �����ϰڴٰ� ������ ��, 1�̸� ���� �����ϰڴٰ� ������ ��.

};
struct TB_ReadyRE {
	unsigned char size; //5
	unsigned char type;//11
	unsigned char pos_in_room;
	unsigned char ready;//0�̸� ��������, 1�̸� ����
	unsigned char roomid;
};
struct TB_GameStart {
	unsigned char size;//4
	unsigned char type;//12
	unsigned char roomID;
	unsigned char my_pos;
};
struct TB_GameStartRE {
	unsigned char size;
	unsigned char type;
	unsigned char startTB;//1�̸� ����,0�̸� ��
};
struct TB_GameStartRE_Cop {
	unsigned char size;//85
	unsigned char type;//12
	unsigned char startTB; //0�̸� ��Ī ����, 1�̸� ����
	unsigned char howmany;
	unsigned char roomid;
	char id1[20];
	char id2[20];
	char id3[20];
	char id4[20];

};

struct TB_Refresh { //type:?
	unsigned char size;
	unsigned char type;
	unsigned char id;
	unsigned char roomID;
};


struct TB_join { //�� �� ������ ��Ŷ 9
	unsigned char size;
	unsigned char type;//9
	unsigned char id;
	unsigned char roomID;
	char password[8];
};

struct TB_joinRE { //���� �߰�
	unsigned char size;//10
	unsigned char type;
	unsigned char respond; //0�̸� no, 1�̸� yes
	unsigned char my_room_num;
	unsigned char yourpos; //1,2,3,4 �� �ϳ�
	unsigned char guard_pos; //���� ��ġ
	unsigned char people_inroom[4];


};
struct TB_create { //type:10
	unsigned char size;
	unsigned char type;
	unsigned char id;
	unsigned char roomid;
	char password[8];
};

struct TB_createRE {
	unsigned char size;
	unsigned char type;
	unsigned char can; //�����ϸ� 1, �Ұ����ϸ� 0
	unsigned char roomid;
};

struct TB_GetOut {//Ŭ�� ����->���� ����, ���� ������ ��� ���� Ŭ��� ���� ��� ���
	unsigned char size;
	unsigned char type;//14
	unsigned char roomID;
	unsigned char position;

}; //�޾��� ��� turtle_room.people_count-=1; turtle_waitroom.roodID = 0; 
struct TB_RoomOut {//�濡�� ���� ��
	unsigned char size;//4
	unsigned char type;//13
	unsigned char roomID;
	unsigned char my_pos;
	unsigned char imwinner; //2�ϰ�� lose

};
struct TB_GetOUTRE {
	unsigned char size;
	unsigned char type;//14
};
struct TB_RoomOutRE {
	unsigned char size;
	unsigned char type;
	unsigned char can; //�����ϸ� 1, �Ұ����ϸ� 0

};

struct TB_RoomSetting {
	unsigned char size;//6
	unsigned char type;//15
	unsigned char roomid;
	unsigned char peoplemax; //�ο���
	unsigned char mapthema;
	unsigned char mapnum;

};

struct TB_TeamSetting {
	unsigned char size;//5
	unsigned char type;//16
	unsigned char roomid;
	unsigned char pos_in_room;
	unsigned char team;
};
struct TB_Time {
	unsigned char size;//6
	unsigned char type;//24
	float time;

};
struct TB_Room_Data { //���� �߰�(��)

	unsigned char roomID;
	unsigned char people_count;
	unsigned char game_start;
	unsigned char people_max; //�ִ� �ο� ��
	unsigned char made; //������� ���ΰ�? 0-�� �������, 1- �������(����), 2-�������(�����)
	unsigned char guardian_pos; //�迭�� ���� �� -1�� ��
	unsigned char people_inroom[4];
	TB_Map map;
	char password[8];
};
struct Map_TB {

	unsigned char mapTile[15][15];
};

#pragma pack(pop)
class Bomb_TB {
public:

	pair<int, int> xz;
	float time;
	bool is_throw;
	bool is_kicked;
	unsigned char firepower;
	float explode_time;
	unsigned char room_num;
	unsigned char game_id;

	bool operator ==(const Bomb_TB& other) {
		return xz == other.xz;
	}

	Bomb_TB() {
		time = (float)GetTickCount() / 1000;
		explode_time = 2.0f;
		xz = make_pair(0, 0);
		is_throw = false;
		is_kicked = false;
	}
	Bomb_TB(int a, int b) {
		time = (float)GetTickCount() / 1000; explode_time = 2.0f;
		xz = make_pair(a, b);
		is_throw = false;
		is_kicked = false;
	}
	Bomb_TB(int a, int b, unsigned char r, unsigned char f, unsigned char g) {
		time = (float)GetTickCount() / 1000; explode_time = 2.0f;
		room_num = r;
		firepower = f;
		game_id = g;
		xz = make_pair(a, b);
		is_throw = false;
		is_kicked = false;
	}
	bool GetTime() {
		float temptime = (float)GetTickCount() / 1000;
		return (temptime - time >= explode_time) && !is_throw && !is_kicked;
	}
	void ResetExplodeTime() {
		float temptime = (float)GetTickCount() / 1000;
		temptime = temptime - time;
		explode_time = explode_time - temptime;
	}
	void ResetTime() {
		time = (float)GetTickCount() / 1000;

	}
	pair<int, int> GetXZ() {
		return xz;
	}

};
class InGameCalculator {
	bool id[4];
	float time;

	bool gameover;


public:
	float start_time;
	float boss_speed;
	map<pair<int, int>, Bomb_TB> bomb_Map;
	vector<TB_BombExplodeRE> explode_List;
	bool ready_player[4];
	bool is_start;
	TB_BossPos ingame_boss_Info;
	int boss_timestamp;
	int idList[4];
	int howmany;
	TB_Map map;
	TB_CharPos ingame_Char_Info[4];
	unsigned char fireMap[15][15];
	int deathcount;
	InGameCalculator() {
		explode_List.clear();
		deathcount = 0;
		boss_timestamp = 0;
		id[0] = true;
		id[1] = true;
		id[2] = true;
		id[3] = true;
		howmany = 2;
		time = 180.0f;
		boss_speed = 1.0f;
		gameover = false;
		map.size = SIZEOF_TB_MAP;
		map.type = CASE_MAP;
		ingame_boss_Info.size = SIZEOF_TB_BossPos;
		ingame_boss_Info.type = CASE_BOSS;
		ingame_boss_Info.posx = 14.0f;
		ingame_boss_Info.posz = 14.0f;
		ingame_boss_Info.rotY = 0.0f;
		ingame_boss_Info.is_alive = true;
		ingame_boss_Info.anistate = 0;
		for (int i = 0; i < 4; ++i) {
			ingame_Char_Info[i].size = SIZEOF_TB_CharPos;
			ingame_Char_Info[i].type = CASE_POS;
			idList[i] =-1;
		}
		for (int i = 0; i < 15; ++i) {
			for (int j = 0; j < 15; ++j) {
				fireMap[i][j] = 0;
			}
		}
		ingame_Char_Info[0].ingame_id = 0;
		ingame_Char_Info[1].ingame_id = 1;
		ingame_Char_Info[2].ingame_id = 2;
		ingame_Char_Info[3].ingame_id = 3;
		ingame_Char_Info[0].posx = 0.0f;
		ingame_Char_Info[0].posz = 0.0f;
		ingame_Char_Info[0].is_alive = true;
		ingame_Char_Info[0].rotY = 0.0f;
		//char_info[1].hp = 10.0f;
		ingame_Char_Info[1].posx = 28.0f;
		ingame_Char_Info[1].posz = 0.0f;
		ingame_Char_Info[1].is_alive = true;
		ingame_Char_Info[1].rotY = 0.0f;
		//char_info[2].hp = 10.0f;
		ingame_Char_Info[2].posx = 0.0f;
		ingame_Char_Info[2].posz = 28.0f;
		ingame_Char_Info[2].is_alive = true;
		ingame_Char_Info[2].rotY = 180.0f;
		//char_info[3].hp = 10.0f;
		ingame_Char_Info[3].posx = 28.0f;
		ingame_Char_Info[3].posz = 28.0f;
		ingame_Char_Info[3].is_alive = true;
		ingame_Char_Info[3].rotY = 180.0f;
	}
	~InGameCalculator() {}
	void InitClass() {
		is_start = false;
		howmany = 2;
		boss_timestamp = 0;
		map.size = SIZEOF_TB_MAP;
		map.type = CASE_MAP;
		deathcount = 0;
		gameover = false;
		explode_List.clear();
		ready_player[0] = false;
		ready_player[1] = false;
		ready_player[2] = false;
		ready_player[3] = false;

		id[0] = true;
		id[1] = true;
		id[2] = true;
		id[3] = true;
		start_time = GetTickCount();
		time = 180.0f;
		ingame_boss_Info.size = SIZEOF_TB_BossPos;
		ingame_boss_Info.type = CASE_BOSS;
		ingame_boss_Info.posx = 14.0f;
		ingame_boss_Info.posz = 14.0f;
		ingame_boss_Info.rotY = 0.0f;
		ingame_boss_Info.is_alive = true;
		ingame_boss_Info.targetid = 1;

		ingame_boss_Info.anistate = 0;
		for (int i = 0; i < 4; ++i) {
			ingame_Char_Info[i].size = SIZEOF_TB_CharPos;
			ingame_Char_Info[i].type = CASE_POS;
			idList[i] = -1;
		}
		for (int i = 0; i < 15; ++i) {
			for (int j = 0; j < 15; ++j) {
				fireMap[i][j] = 0;
			}
		}
		ingame_Char_Info[0].ingame_id = 0;
		ingame_Char_Info[1].ingame_id = 1;
		ingame_Char_Info[2].ingame_id = 2;
		ingame_Char_Info[3].ingame_id = 3;
		ingame_Char_Info[0].posx = 0.0f;
		ingame_Char_Info[0].posz = 0.0f;
		ingame_Char_Info[0].is_alive = true;
		ingame_Char_Info[0].rotY = 0.0f;
		//char_info[1].hp = 10.0f;
		ingame_Char_Info[1].posx = 28.0f;
		ingame_Char_Info[1].posz = 0.0f;
		ingame_Char_Info[1].is_alive = true;
		ingame_Char_Info[1].rotY = 0.0f;
		//char_info[2].hp = 10.0f;
		ingame_Char_Info[2].posx = 0.0f;
		ingame_Char_Info[2].posz = 28.0f;
		ingame_Char_Info[2].is_alive = true;
		ingame_Char_Info[2].rotY = 180.0f;
		//char_info[3].hp = 10.0f;
		ingame_Char_Info[3].posx = 28.0f;
		ingame_Char_Info[3].posz = 28.0f;
		ingame_Char_Info[3].is_alive = true;
		ingame_Char_Info[3].rotY = 180.0f;
	}
	void PlayerDead(unsigned char idd) {
		if (id[idd]) {
			id[idd] = false;
			deathcount++;
		}
	}

	void GameStart() {
		is_start = true;
	}
	void ChangeID(int place, unsigned char id_p) {
		
		idList[place] = id_p;

	}
	void Start_AndSetTime() {

	}
	void PlayerBlank(int id_p) {
		id[id_p] = false;
	}
	void SetGameOver() {
		gameover = true;
	}

	bool IsGameOver() {
		return gameover;
	}
	float GetTime() {
		return time;
	}
	void SetTime(DWORD a) {
		float temp = ((float)a) / 1000.0f;
		time = time - temp;
	}
	bool OneSec() {
		return ((int)(time * 10) % 10)<1;
	}
	unsigned char GetWinnerID() {
		for (unsigned char i = 0; i < 4; ++i) {
			if (id[i])
				return i;
		}
		return 4; //DRAW�� ���Ѵ�.
	}

	void Boss_Target_Change(int id) {
		ingame_boss_Info.targetid = id;
	}
	void ChaseAI() {
		ingame_boss_Info.posx = ingame_boss_Info.posx + (boss_speed*cos(ingame_boss_Info.rotY));
		ingame_boss_Info.posz = ingame_boss_Info.posz + (boss_speed*sin(ingame_boss_Info.rotY));


	}
	int Boss_AI_Search() { //���� AI_Ž��
		//�ü�
		float distance_Char[4] = { 500000,500000,500000,500000 };
		int ret_val=0;
		float rotation_angle = 0;
		for (int i = 0; i < howmany; ++i) {
			float angle_sight = cos(45.0f * 3.1416 / 180);
			float innerx = (ingame_Char_Info[i].posx - ingame_boss_Info.posx)* (20 * cos((ingame_boss_Info.rotY )));
			float innerz = (ingame_Char_Info[i].posz - ingame_boss_Info.posz) *(20 * sin((ingame_boss_Info.rotY)));
			//* 3.1416 / 180
			float inner_pro = abs(innerx) + abs(innerz);
			float innerx2 = (20 * cos(ingame_boss_Info.rotY))*(20 * cos(ingame_boss_Info.rotY));
			float innerz2 = (20 * sin(ingame_boss_Info.rotY))*(20 * sin(ingame_boss_Info.rotY));
			
			float innerx3 = (ingame_Char_Info[i].posx - ingame_boss_Info.posx)*(ingame_Char_Info[i].posx - ingame_boss_Info.posx);
			float innerz3 = (ingame_Char_Info[i].posz - ingame_boss_Info.posz)*(ingame_Char_Info[i].posz - ingame_boss_Info.posz);
			float par_getcos = sqrtf(abs(innerx2 + innerz2)) *sqrtf(abs(innerx3 + innerz3));
			float target_angle = inner_pro / par_getcos;
			//cout << cos(0.5) << endl;
			if (angle_sight <= target_angle) {
				
				distance_Char[i] = abs(innerx3)+ abs(innerz3);
				if (distance_Char[ret_val] > distance_Char[i]) {
					ret_val = i;
					rotation_angle = acos(target_angle);
					cout << "Get" <<rotation_angle << endl;
				}

			}
			else {
				//cout << "No Get" << endl;
			}

		}
		if (distance_Char[ret_val] >= 500000) {
			//cout << "nothing better than you" << endl;
			return 0;
		}
		else {
			
			//ingame_boss_Info.rotY = rotation_angle;
			cout << "find you and rotate " << ingame_boss_Info.rotY << endl;
			return ret_val + 1;
		}

	}
	

	void CalculateMap(int x, int z, unsigned char f,unsigned char id_player) {
		bool l_UpBlock = false;
		bool l_DownBlock = false;
		bool l_LeftBlock = false;
		bool l_RightBlock = false;
		unsigned char uf = f;
		unsigned char df = f;
		unsigned char lf = f;
		unsigned char rf = f;
		unsigned char tempMap[15][15];
		memcpy(tempMap, map.mapInfo, sizeof(tempMap));
		tempMap[z][x] = MAP_NOTHING;
		for (unsigned char b = 1; b <= f; ++b) {
			if (!l_DownBlock) {
				if (z - b < 0) {
					l_DownBlock = true;
					df = b - 1;
				}
				else {
					if (tempMap[z - b][x] == MAP_BOMB) {
						tempMap[z - b][x] = MAP_NOTHING;
						memcpy(map.mapInfo, tempMap, sizeof(tempMap));

						//*tempB = true;
						//CalculateMap_Simple(x, z - b, fireMap[room_num - 1][z - b][x], room_num);
						if (bomb_Map[make_pair(x, z - b)].firepower != 0) {

							CalculateMap(x, z- b, fireMap[z - b][x], bomb_Map[make_pair(x, z - b)].game_id);
							auto a = bomb_Map.find(pair<int, int>(x, z - b));
							bomb_Map.erase(a);
						}
						memcpy(tempMap, map.mapInfo, sizeof(tempMap));
						l_DownBlock = true;
						df = b;
					}
					else if (tempMap[z - b][x] == MAP_BOX) {
						cout << "Box!!!" << endl;
						int temp_rand = (rand() % 14);
						if (temp_rand < 4)
							tempMap[z - b][x] = MAP_NOTHING;
						else if (temp_rand >= 4 && temp_rand <= 5)
							tempMap[z - b][x] = MAP_ITEM;
						else if (temp_rand >= 6 && temp_rand <= 7)
							tempMap[z - b][x] = MAP_ITEM_F;
						else if (temp_rand >= 8 && temp_rand <= 9)
							tempMap[z - b][x] = MAP_ITEM_S;
						else if (temp_rand >= 10 && temp_rand <= 11)
							tempMap[z - b][x] = MAP_KICKITEM;
						else if (temp_rand >= 12 && temp_rand <= 13)
							tempMap[z - b][x] = MAP_THROWITEM;

						l_DownBlock = true;
						df = b - 1;
					}
					else if (tempMap[z - b][x] == MAP_ITEM || tempMap[z - b][x] == MAP_ITEM_F || tempMap[z - b][x] == MAP_ITEM_S || tempMap[z - b][x] == MAP_KICKITEM || tempMap[z - b][x] == MAP_THROWITEM) {
						tempMap[z - b][x] = MAP_NOTHING;
					}
					else if (tempMap[z - b][x] == MAP_BUSH || tempMap[z - b][x] == MAP_FIREBUSH) {

					}
					else if (tempMap[z - b][x] == MAP_ROCK) {
						l_DownBlock = true;
						df = b - 1;
					}
				}
			}
			if (!l_UpBlock) {
				if (z + b > 14) {
					l_UpBlock = true;
					uf = b - 1;
				}
				else {
					if (tempMap[z + b][x] == MAP_BOMB) {
						tempMap[z + b][x] = MAP_NOTHING;

						l_UpBlock = true;
						memcpy(map.mapInfo, tempMap, sizeof(tempMap));
						if (bomb_Map[make_pair(x, z + b)].firepower != 0) {
							
							CalculateMap(x, z + b, fireMap[z + b][x], bomb_Map[make_pair(x, z + b)].game_id);
							auto a = bomb_Map.find(pair<int, int>(x, z+b));
							bomb_Map.erase(a);
						}
						

						//*tempB = true;
						//CalculateMap_Simple(x, z - b, fireMap[room_num - 1][z - b][x], room_num);
						memcpy(tempMap, map.mapInfo, sizeof(tempMap));
						uf = b;
					}
					else if (tempMap[z + b][x] == MAP_BOX) {
						cout << "Box!!!" << endl;
						int temp_rand = (rand() % 14);
						if (temp_rand<4)
							tempMap[z + b][x] = MAP_NOTHING;
						else if (temp_rand >= 4 && temp_rand <= 5)
							tempMap[z + b][x] = MAP_ITEM;
						else if (temp_rand >= 6 && temp_rand <= 7)
							tempMap[z + b][x] = MAP_ITEM_F;
						else if (temp_rand >= 8 && temp_rand <= 9)
							tempMap[z + b][x] = MAP_ITEM_S;
						else if (temp_rand >= 10 && temp_rand <= 11)
							tempMap[z + b][x] = MAP_KICKITEM;
						else if (temp_rand >= 12 && temp_rand <= 13)
							tempMap[z + b][x] = MAP_THROWITEM;
						l_UpBlock = true;
						uf = b - 1;
					}
					else if (tempMap[z + b][x] == MAP_ITEM || tempMap[z + b][x] == MAP_ITEM_F || tempMap[z + b][x] == MAP_ITEM_S || tempMap[z + b][x] == MAP_KICKITEM || tempMap[z + b][x] == MAP_THROWITEM) {
						tempMap[z + b][x] = MAP_NOTHING;
					}
					else if (tempMap[z + b][x] == MAP_ROCK) {
						l_UpBlock = true;
						uf = b - 1;
					}
				}
			}
			if (!l_LeftBlock) {
				if (x - b < 0) {
					l_LeftBlock = true;
					lf = b - 1;
				}
				else {
					if (tempMap[z][x - b] == MAP_BOMB) {
						tempMap[z][x - b] = MAP_NOTHING;
						l_LeftBlock = true;
						memcpy(map.mapInfo, tempMap, sizeof(tempMap));
						//*tempB = true;
						//CalculateMap_Simple(x, z - b, fireMap[room_num - 1][z - b][x], room_num);
						if (bomb_Map[make_pair(x-b, z)].firepower != 0) {

							CalculateMap(x-b, z, fireMap[z][x-b], bomb_Map[make_pair(x-b, z)].game_id);
							auto a = bomb_Map.find(pair<int,int>(x - b, z));
							bomb_Map.erase(a);
						}
						memcpy(tempMap, map.mapInfo, sizeof(tempMap));
						lf = b;
					}
					else if (tempMap[z][x - b] == MAP_BOX) {
						cout << "Box!!!" << endl;
						int temp_rand = (rand() % 14);
						if (temp_rand<4)
							tempMap[z][x - b] = MAP_NOTHING;
						else if (temp_rand >= 4 && temp_rand <= 5)
							tempMap[z][x - b] = MAP_ITEM;
						else if (temp_rand >= 6 && temp_rand <= 7)
							tempMap[z][x - b] = MAP_ITEM_F;
						else if (temp_rand >= 8 && temp_rand <= 9)
							tempMap[z][x - b] = MAP_ITEM_S;
						else if (temp_rand >= 10 && temp_rand <= 11)
							tempMap[z][x - b] = MAP_KICKITEM;
						else if (temp_rand >= 12 && temp_rand <= 13)
							tempMap[z][x - b] = MAP_THROWITEM;
						l_LeftBlock = true;
						lf = b - 1;
					}
					else if (tempMap[z][x - b] == MAP_ITEM || tempMap[z][x - b] == MAP_KICKITEM || tempMap[z][x - b] == MAP_THROWITEM || tempMap[z][x - b] == MAP_ITEM_F || tempMap[z][x - b] == MAP_ITEM_S) {
						tempMap[z][x - b] = MAP_NOTHING;
					}
					else if (tempMap[z][x - b] == MAP_ROCK) {
						l_LeftBlock = true;
						lf = b - 1;
					}
				}
			}
			if (!l_RightBlock) {
				if (x + b > 14) {
					l_RightBlock = true;
					rf = b - 1;
				}
				else {
					if (tempMap[z][x + b] == MAP_BOMB) {
						tempMap[z][x + b] = MAP_NOTHING;
						l_RightBlock = true;
						memcpy(map.mapInfo, tempMap, sizeof(tempMap));
						//*tempB = true;
						//CalculateMap_Simple(x, z - b, fireMap[room_num - 1][z - b][x], room_num);
						if (bomb_Map[make_pair(x + b, z)].firepower != 0) {

							CalculateMap(x + b, z, fireMap[z][x + b], bomb_Map[make_pair(x + b, z)].game_id);
							auto a = bomb_Map.find(pair<int, int>(x+b, z ));
							bomb_Map.erase(a);
						}
						memcpy(tempMap, map.mapInfo, sizeof(tempMap));
						rf = b;
					}
					else if (tempMap[z][x + b] == MAP_BOX) {
						cout << "Box!!!" << endl;
						int temp_rand = (rand() % 14);
						if (temp_rand<4)
							tempMap[z][x + b] = MAP_NOTHING;
						else if (temp_rand >= 4 && temp_rand <= 5)
							tempMap[z][x + b] = MAP_ITEM;
						else if (temp_rand >= 6 && temp_rand <= 7)
							tempMap[z][x + b] = MAP_ITEM_F;
						else if (temp_rand >= 8 && temp_rand <= 9)
							tempMap[z][x + b] = MAP_ITEM_S;
						else if (temp_rand >= 10 && temp_rand <= 11)
							tempMap[z][x + b] = MAP_KICKITEM;
						else if (temp_rand >= 12 && temp_rand <= 13)
							tempMap[z][x + b] = MAP_THROWITEM;
						l_RightBlock = true;
						rf = b - 1;
					}
					else if (tempMap[z][x + b] == MAP_ITEM || tempMap[z][x + b] == MAP_KICKITEM || tempMap[z][x + b] == MAP_THROWITEM || tempMap[z][x + b] == MAP_ITEM_F || tempMap[z][x + b] == MAP_ITEM_S) {
						tempMap[z][x + b] = MAP_NOTHING;
					}
					else if (tempMap[z][x + b] == MAP_ROCK) {
						l_RightBlock = true;
						rf = b - 1;
					}
				}
			}


		}
		TB_BombExplodeRE bomb = { SIZEOF_TB_BombExplodeRE,CASE_BOMB_EX,uf,rf,df,lf,id_player,x,z };
		explode_List.emplace_back(bomb);
		//explode_List
		fireMap[z][x] = 0;
		tempMap[z][x] = MAP_NOTHING;
		memcpy(map.mapInfo, tempMap, sizeof(tempMap));
	}
};

template <class Iter, class Value>
Iter myFind(Iter a, Iter b, Value val) {
	Iter p = a;
	while (a != b) {
		if (*a == val)
			return a;
		else
			++a;
	}
	return b;

}


struct EXOVER {
	WSAOVERLAPPED m_over;
	char m_iobuf[MAX_BUFF_SIZE];
	WSABUF m_wsabuf;
	bool is_recv;
};


class Client {
public:
	SOCKET m_s;
	bool m_isconnected;
	unsigned char is_guardian;
	int m_x;
	int m_y;
	int m_scene; // 0 - �κ�, 1- ��, 2- ������
	EXOVER m_rxover;
	int m_packet_size;  // ���� �����ϰ� �ִ� ��Ŷ�� ũ��
	int   m_prev_packet_size; // ������ recv���� �ϼ����� �ʾƼ� ������ ���� ��Ŷ�� �պκ��� ũ��
	char m_packet[MAX_PACKET_SIZE];
	int id;
	int roomNum;
	char stringID[20];
	int win_vs;
	int lose_vs;
	int tier;
	int exp;
	int exp_max;
	int success_coop;
	int fail_coop;
	bool playing_game;
	InGameCalculator* link;
	// set<int> view_list; // ����/������ �����ο��� �Ѵ�. + �ð����⵵ �� ���� ȿ�����ΰ��� set�̴�. (list�� ����/������ �������� �˻� ������ ������.)
	// multiset �� �ߺ����� �̱⶧���� ������� �ʾƾ��Ѵ�.
	unordered_set<int> m_view_list; // ������ ���� �ʿ����� �ʱ� ������ �����ļ����� ���� ���.
	mutex m_mVl; // �丮��Ʈ�� ��ȣ�ϱ�����

	Client()
	{
		m_isconnected = false;
		m_x = 4;
		m_y = 4;
		playing_game = false;
		ZeroMemory(&m_rxover.m_over, sizeof(WSAOVERLAPPED));
		m_rxover.m_wsabuf.buf = m_rxover.m_iobuf;
		m_rxover.m_wsabuf.len = sizeof(m_rxover.m_wsabuf.buf);
		m_rxover.is_recv = true;
		m_prev_packet_size = 0;
	}
};

struct MatchingSt {
	unsigned char boss_type;
	unsigned char map_type;

	unsigned char difficulty;
	
	unsigned char prefer_man;
	int id[4];
};

class Matching {
	unsigned char boss_type;
	unsigned char map_type;

	unsigned char difficulty;
	
	InGameCalculator game_cal;
	
public:
	mutex mtx;
	int id[4];
	unsigned char prefer_man;
	Matching() {
		for (int i = 0; i < 4; ++i)
			id[i] = -1;
		prefer_man = 0;
		boss_type = 0;
		map_type = 0;
		difficulty = 0;
		
	}
	Matching(unsigned char boss_t, unsigned char map_t, unsigned char diff_t, unsigned char how_many) {
		for (int i = 0; i < 4; ++i)
			id[i] = -1;
		prefer_man = how_many;
		boss_type = boss_t;
		map_type = map_t;
		difficulty = diff_t;
		
	}
	Matching(int id_t,unsigned char boss_t, unsigned char map_t, unsigned char diff_t, unsigned char how_many) {
		for (int i = 0; i < 4; ++i)
			id[i] = -1;
		id[0] = id_t;
		prefer_man = how_many;
		boss_type = boss_t;
		map_type = map_t;
		difficulty = diff_t;

	}
	~Matching() {}

	bool IsAbleToJoin(int id_m) {
		
		for (int i = 0; i <= prefer_man; ++i) {
			mtx.lock();
			if (id[i] < 0) {
				id[i] = id_m;
				mtx.unlock();
				return true;
			}
			mtx.unlock();
		}
		return false;
	}
	bool JoinMatch() {
		for (int i = 0; i <= prefer_man; ++i) {
			if (id[i] < 0) {
				
				return false;
			}

		}
		return true;
	}
};