﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


// MCL에 들어갈 좌표 구조체
public struct Map_Coordinate
{
    public float x, z;
    public Map_Coordinate(float x, float z)
    {
        this.x = x;
        this.z = z;
    }
}


// ========================


// #define 맵 생성 관련
static class MAP
{
    public const int NOT_SET = -1; // 설정이 안됐음

    public const int THEME_FOREST = 1;
    public const int THEME_OCEAN = 2;
    public const int THEME_SNOWLAND = 3;
}

static class OBJECT_TABLE_NUMBER
{
    public const int BOX = 1;
    public const int ROCK = 2;
    public const int GRASS = 3;
    public const int GOBLIN = 4;
    public const int BOSS_GOBLIN = 5;
    public const int START_POINT = 6;
    public const int NEXT_POINT = 7;
    public const int END_POINT = 8;
    public const int CHARACTER_SPAWN = 9;
    public const int BOSS_GIANT_1 = 10;
    public const int ITEM_BOMB = 11;
    public const int ITEM_SPEED = 12;
    public const int ITEM_FIRE = 13;
    public const int ITEM_KICK = 14;
    public const int ITEM_THROW = 15;
    public const int INFO_TRIGGER_BOMB = 16;
    public const int INFO_TRIGGER_SPEED = 17;
    public const int INFO_TRIGGER_FIRE = 18;
    public const int INFO_TRIGGER_KICK = 19;
    public const int INFO_TRIGGER_THROW = 20;
    public const int BOX_NONE_ITEM = 21;
}

public class StageManager : MonoBehaviour
{

    // 맵 사이즈
    int m_Map_Size_X = 17;
    int m_Map_Size_Z = 17;

    // 퍼포먼스 카메라
    public GameObject m_CameraOffset;
    public bool m_is_Intro_Over;

    // 프리팹들
    public GameObject m_Forest_Theme_Terrain;
    public GameObject m_SnowLand_Theme_Terrain;

    // 보스 스테이지 인트로용
    //public GameObject m_Prefab_Intro_Boss;

    // 오브젝트 프리팹들
    public GameObject m_Prefab_Box;
    public GameObject m_Prefab_Rock;
    public GameObject m_Prefab_Grass;
    public GameObject m_Prefab_Goblin;
    public GameObject m_Prefab_Goblin_Boss;
    public GameObject m_Prefab_Start_Point;
    public GameObject m_Prefab_Next_Portal;
    public GameObject m_Prefab_End_Portal;
    //public GameObject m_Prefab_Chractrer_Spawn;
    public GameObject m_Prefab_Ork_Boss;
    public GameObject m_Prefab_Airplane;
    public GameObject m_Prefab_Item_Bomb;
    public GameObject m_Prefab_Item_Speed;
    public GameObject m_Prefab_Item_Fire;
    public GameObject m_Prefab_Item_Kick;
    public GameObject m_Prefab_Item_Throw;
    public GameObject m_Prefab_Info_Trigger_Bomb;
    public GameObject m_Prefab_Info_Trigger_Speed;
    public GameObject m_Prefab_Info_Trigger_Fire;
    public GameObject m_Prefab_Info_Trigger_Kick;
    public GameObject m_Prefab_Info_Trigger_Throw;
    public GameObject m_Prefab_Box_None_Item;


    // ===== 생성한 오브젝트 관리 =====
    // ================================
    List<GameObject> m_Current_Map_Objects = new List<GameObject>();
    int m_Current_Map_Objects_Count; // 생성된 오브젝트 개수

    int m_Current_Stage_index_Count = 0; // 현재 스테이지의 맵 인덱스 카운트

    Vector3 m_Object_Position; // 오브젝트 생성시 위치 변경에 이용
                               // ================================




    public Text m_Text; // 게임 오버 텍스트

    int m_Current_Map_Number = 0; // 현재 맵 번호 (인덱스)

    public static bool m_is_Stage_Clear = false; // 스테이지를 클리어했는가?

    bool m_is_Goal_In = false; // 목표 지점에 들어갔는가?

    public static int m_Stars = 0; // 획득한 별 개수

    public static int m_Total_Monster_Count = 0; // 총 스폰된 일반 몹 수

    public static int m_Left_Monster_Count = 0;// 남은 일반 몹 수

    // MCL
    public List<Map_Coordinate> m_Map_Coordinate_List;

    // MCL_index_is_Blocked_List
    public static List<bool> m_MCL_is_Blocked_List; // MCL의 is_Blocked를 따로 분리했다..

    // MCL이 초기화 되었는가?
    public static bool m_is_init_MCL;

    // 스테이지 매니저 객체
    public static StageManager c_Stage_Manager;



    // 맵 이동 시 폭탄을 무효화 하기 위한 변수
    public bool m_is_Map_Changing;

    // 현재 스테이지 번호
    public int m_Stage_ID;

    // 현재 스테이지의 제한시간 설정
    // -1로 설정할 경우 기획 설정(테이블)에 따른다.
    public float m_Stage_Time_Limit;






    // ===== 보스 관련 =====
    // =====================

    // 현재 스테이지가 보스전인지?
    public bool m_is_Boss_Stage;

    // 보스 테이블 번호!
    public int m_Boss_ID;

    // 서든데스 고블맨 객체
    public GameObject m_SuddenDeath_JetGoblin;

    // 서든데스 고블맨을 소환했는가? 
    bool m_is_Summon_SJG;

    // 서든데스 활성화?
    public bool m_is_SuddenDeath;

    // 보스 스테이지의 보스 몬스터가 죽었는가?
    bool m_is_Boss_Dead;
    // =====================



    bool m_is_Tutorial_Stage;


    // ========== 시스템관련 ============
    // ==================================

    // 게임이 일시정지 되었는가?
    public bool m_is_Pause;

    // 게임이 끝났는가?
    public bool m_Game_Over = false;
    // ==================================






    // =========== CSV 관련 ===========
    // ================================

    // 현재 스테이지의 퀘스트 목록
    List<Adventure_Quest_Data> m_QuestList;

    // 오브젝트 테이블
    List<Object_Table_Data> m_Object_Table_List;

    // 현재 스테이지의 오브젝트 배치 목록
    List<Object_Spawn_Position_Data> m_Object_Spawn_Position_List;

    // 스테이지 번호 배열 테이블
    List<int> m_Stage_Number_List = new List<int>();

    // 스크립트(대사) 테이블
    List<Script_Data> m_Script_List = new List<Script_Data>();

    // 보스 스테이터스 데이터
    Adventure_Boss_Data m_Adventure_Boss_Data = new Adventure_Boss_Data();

    // 미션 번호 리스트
    int[] mission_list = new int[3];

    // ================================
    // ================================



    void Awake()
    {
        m_is_Stage_Clear = false;
        m_is_Pause = false;

        m_Stars = 0;

        m_Total_Monster_Count = 0;
        m_Left_Monster_Count = 0;

        m_is_init_MCL = false;
        m_Map_Coordinate_List = new List<Map_Coordinate>();
        m_MCL_is_Blocked_List = new List<bool>();

        c_Stage_Manager = this;
        m_Current_Map_Number = 0;
        m_is_Map_Changing = false;

        m_is_SuddenDeath = false;
        m_is_Summon_SJG = false;

        m_is_Tutorial_Stage = false;

        // 맵 좌표 리스트 초기화
        MCL_init();

        // 카메라
        m_CameraOffset = GameObject.Find("Camera_Offset");

        // 오브젝트 테이블 목록 로드
        m_Object_Table_List = new List<Object_Table_Data>(CSV_Manager.GetInstance().Get_Object_Table_List());


        // 스테이지 ID를 받아온다.
        if (m_Stage_ID == MAP.NOT_SET)
            m_Stage_ID = PlayerPrefs.GetInt("Mode_Adventure_Stage_ID_For_MapLoad");

        // 스테이지 번호 목록 로드
        m_Stage_Number_List = CSV_Manager.GetInstance().Get_Stage_Number_List(m_Stage_ID);

        if (m_Stage_ID == 10) // 튜토리얼 스테이지인가?
            m_is_Tutorial_Stage = true;

        // ===============디버깅 때문에 적어둔 구문임=======================
        //if (m_Stage_ID == 17)
        //    PlayerPrefs.SetInt("Mode_Adventure_Current_Stage_ID", 9);
        // =================================================================

        // 현재 스테이지의 미션 번호들 받아오기
        CSV_Manager.GetInstance().Get_Adv_Mission_Num_List(ref mission_list, PlayerPrefs.GetInt("Mode_Adventure_Current_Stage_ID"));


        // 퀘스트 목록 로딩
        m_QuestList = new List<Adventure_Quest_Data>(CSV_Manager.GetInstance().Get_Adventure_Quest_List(ref mission_list));

        // 퀘스트 목록에 보스 몬스터 처치가 있다는것은
        // 해당 스테이지가 보스 스테이지라는 뜻!
        foreach (Adventure_Quest_Data questdata in m_QuestList)
        {
            if (questdata.Quest_ID == 4)
            {
                m_is_Boss_Stage = true;

                // 보스 테이블의 번호를 설정
                if (PlayerPrefs.GetInt("Mode_Adventure_Current_Stage_ID") == 5)
                    m_Boss_ID = 1;
                else if (PlayerPrefs.GetInt("Mode_Adventure_Current_Stage_ID") == 9)
                    m_Boss_ID = 2;
            }
        }

        // 설정된 번호를 받아서 맵 생성!
        Create_Map(m_Stage_Number_List[m_Current_Stage_index_Count]);

        // 대사 받아오기!
        //m_Script_List = CSV_Manager.GetInstance().Get_Script_List(스크립트ID);



        // 시간 설정
        if (m_Stage_Time_Limit == MAP.NOT_SET)
            m_Stage_Time_Limit = 90.0f;

        // UI에도 시간 적용
        UI.time_Second = m_Stage_Time_Limit;
    }

    void Start()
    {
        // 페이드 인
        Fade_Slider.c_Fade_Slider.Start_Fade_Slider(2);
    }

    void Update()
    {
        Check_GameOver();

        if (!m_is_Boss_Stage && m_is_SuddenDeath && !m_is_Summon_SJG && !m_is_Tutorial_Stage)
        {
            Instantiate(m_SuddenDeath_JetGoblin);
            m_is_Summon_SJG = true;
        }
    }

    void Check_GameOver()
    {
        if (!PlayerMove.C_PM.Get_IsAlive()) // 죽어서 끝났거나,
        {
            m_Text.text = "Game Over";
            m_Game_Over = true;
        }

        else if (m_is_Stage_Clear) // 클리어해서 끝났거나!
        {
            m_Game_Over = true;
        }
    }

    public bool Get_Game_Over()
    {
        return m_Game_Over;
    }

    // 터레인 생성
    void Create_Terrain()
    {
        Instantiate(m_Forest_Theme_Terrain);
    }


    // 맵 생성 및 설정
    void Create_Map(int stage_id)
    {
        // 터레인 생성
        Create_Terrain();

        // 오브젝트 스폰 위치 목록을 받아온다.
        m_Object_Spawn_Position_List = new List<Object_Spawn_Position_Data>(CSV_Manager.GetInstance().Get_Object_Spawn_Position_List(stage_id));

        m_Current_Map_Objects_Count = 0;



        // 위치에 맞게 오브젝트를 생성한다.
        for (int z = 0; z < 15; ++z)
        {
            for (int x = 0; x < 15; ++x)
            {
                if (m_Object_Spawn_Position_List[z].Spawn_Node[x] != 0)
                {
                    // x, z 좌표 설정
                    m_Object_Position.x = x * 2.0f;
                    m_Object_Position.z = 78.0f - (z * 2.0f);

                    switch (m_Object_Spawn_Position_List[z].Spawn_Node[x])
                    {
                        case OBJECT_TABLE_NUMBER.BOX: // 박스
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_Box));
                            m_Object_Position.y = m_Prefab_Box.transform.position.y;
                            break;

                        case OBJECT_TABLE_NUMBER.ROCK: // 바위
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_Rock));
                            m_Object_Position.y = m_Prefab_Rock.transform.position.y;
                            break;

                        case OBJECT_TABLE_NUMBER.GRASS: // 부쉬
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_Grass));
                            m_Object_Position.y = m_Prefab_Grass.transform.position.y;
                            break;

                        case OBJECT_TABLE_NUMBER.GOBLIN: // 일반 고블린
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_Goblin));
                            m_Object_Position.y = m_Prefab_Goblin.transform.position.y;
                            break;

                        case OBJECT_TABLE_NUMBER.BOSS_GOBLIN: // 보스 고블린
                                                              //Instantiate(m_Prefab_Intro_Boss);

                            // 보스 데이터 읽어오기
                            m_Adventure_Boss_Data = CSV_Manager.GetInstance().Get_Adventure_Boss_Data(m_Boss_ID);

                            // 생성
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_Goblin_Boss));
                            m_Object_Position.y = m_Prefab_Goblin_Boss.transform.position.y;

                            // 생성한 객체 좌표 이동
                            m_Current_Map_Objects[m_Current_Map_Objects_Count].transform.position = m_Object_Position;

                            ++m_Current_Map_Objects_Count;

                            // 보스전이므로 비행기도 부른다.
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_Airplane));
                            m_Object_Position = m_Prefab_Airplane.transform.position;
                            break;

                        case OBJECT_TABLE_NUMBER.START_POINT:
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_Start_Point));
                            m_Object_Position.y = m_Prefab_Start_Point.transform.position.y;
                            break;

                        case OBJECT_TABLE_NUMBER.NEXT_POINT:
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_Next_Portal));
                            m_Object_Position.y = m_Prefab_Next_Portal.transform.position.y;
                            break;

                        case OBJECT_TABLE_NUMBER.END_POINT:
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_End_Portal));
                            m_Object_Position.y = m_Prefab_End_Portal.transform.position.y;
                            break;

                        case OBJECT_TABLE_NUMBER.CHARACTER_SPAWN:
                            //m_Current_Map_Objects.Add(Instantiate(m_Prefab_Character_Spawn));
                            //m_Object_Position.y = m_Prefab_Character_Spawn.transform.position.y;
                            break;

                        case OBJECT_TABLE_NUMBER.BOSS_GIANT_1:
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_Ork_Boss));
                            m_Object_Position.y = m_Prefab_Ork_Boss.transform.position.y;

                            // 생성한 객체 좌표 이동
                            m_Current_Map_Objects[m_Current_Map_Objects_Count].transform.position = m_Object_Position;

                            ++m_Current_Map_Objects_Count;

                            // 보스전이므로 비행기도 부른다.
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_Airplane));
                            m_Object_Position = m_Prefab_Airplane.transform.position;
                            break;

                        case OBJECT_TABLE_NUMBER.ITEM_BOMB:
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_Item_Bomb));
                            m_Object_Position.y = m_Prefab_Item_Bomb.transform.position.y;
                            break;

                        case OBJECT_TABLE_NUMBER.ITEM_SPEED:
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_Item_Speed));
                            m_Object_Position.y = m_Prefab_Item_Speed.transform.position.y;
                            break;

                        case OBJECT_TABLE_NUMBER.ITEM_FIRE:
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_Item_Fire));
                            m_Object_Position.y = m_Prefab_Item_Fire.transform.position.y;
                            break;

                        case OBJECT_TABLE_NUMBER.ITEM_KICK:
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_Item_Kick));
                            m_Object_Position.y = m_Prefab_Item_Kick.transform.position.y;
                            break;

                        case OBJECT_TABLE_NUMBER.ITEM_THROW:
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_Item_Throw));
                            m_Object_Position.y = m_Prefab_Item_Throw.transform.position.y;
                            break;

                        case OBJECT_TABLE_NUMBER.INFO_TRIGGER_BOMB:
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_Info_Trigger_Bomb));
                            m_Object_Position.y = m_Prefab_Info_Trigger_Bomb.transform.position.y;
                            break;

                        case OBJECT_TABLE_NUMBER.INFO_TRIGGER_SPEED:
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_Info_Trigger_Speed));
                            m_Object_Position.y = m_Prefab_Info_Trigger_Speed.transform.position.y;
                            break;

                        case OBJECT_TABLE_NUMBER.INFO_TRIGGER_FIRE:
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_Info_Trigger_Fire));
                            m_Object_Position.y = m_Prefab_Info_Trigger_Fire.transform.position.y;
                            break;

                        case OBJECT_TABLE_NUMBER.INFO_TRIGGER_KICK:
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_Info_Trigger_Kick));
                            m_Object_Position.y = m_Prefab_Info_Trigger_Kick.transform.position.y;
                            break;

                        case OBJECT_TABLE_NUMBER.INFO_TRIGGER_THROW:
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_Info_Trigger_Throw));
                            m_Object_Position.y = m_Prefab_Info_Trigger_Throw.transform.position.y;
                            break;

                        case OBJECT_TABLE_NUMBER.BOX_NONE_ITEM:
                            m_Current_Map_Objects.Add(Instantiate(m_Prefab_Box_None_Item));
                            m_Object_Position.y = m_Prefab_Box_None_Item.transform.position.y;
                            break;
                    }
                    // 생성한 객체 좌표 이동
                    m_Current_Map_Objects[m_Current_Map_Objects_Count].transform.position = m_Object_Position;

                    // 카운트 증가
                    ++m_Current_Map_Objects_Count;
                }
            }
        }
    }



    // ==================== MCL 관련 =======================
    // =====================================================

    // MCL 좌표 및 is_Blocked 초기화
    void MCL_init()
    {
        m_Map_Coordinate_List.Clear();

        Map_Coordinate m_tmpCoordinate;

        for (int i = -1; i < m_Map_Size_X - 1; ++i)
        {
            for (int j = -1; j < m_Map_Size_Z - 1; ++j)
            {
                m_tmpCoordinate.x = i * 2;
                m_tmpCoordinate.z = j * 2 + 50;
                m_Map_Coordinate_List.Add(m_tmpCoordinate);
                m_MCL_is_Blocked_List.Add(false);
            }
        }

        for (int i = 0; i < 17; ++i)
        {
            // z라인 울타리
            Update_MCL_isBlocked(i, true);
            Update_MCL_isBlocked(i + 272, true);

            // x라인 울타리
            Update_MCL_isBlocked(i * 17, true);
            Update_MCL_isBlocked(i * 17 + 16, true);
        }

        m_is_init_MCL = true;
    }




    // 오브젝트들이 MCL의 isBlocked를 갱신할 수 있도록 해주는 메소드
    public void Update_MCL_isBlocked(int index, bool isBlocked)
    {
        if (index != -1)
        {
            m_MCL_is_Blocked_List[index] = isBlocked;
        }
    }





    // MCL 안의 해당 index가 막혀있는지를 반환해준다.
    public bool Get_MCL_index_is_Blocked(int index)
    {
        return m_MCL_is_Blocked_List[index];
    }



    public void Get_MCL_Coordinate(int index, ref float x, ref float z)
    {
        x = m_Map_Coordinate_List[index].x;
        z = m_Map_Coordinate_List[index].z;
    }




    // 받아온 좌표에 대한 MCL 인덱스 반환
    public int Find_Own_MCL_Index(float x, float z)
    {
        Map_Coordinate m_tmpCoordinate;

        // 받아온 좌표에서 가장 가까운 좌표로 설정한다.
        int index_X = (int)x;
        int index_Z = (int)z;

        if (index_X % 2 == 1)
        {
            m_tmpCoordinate.x = index_X + 1;
        }
        else if (index_X % 2 == -1)
        {
            m_tmpCoordinate.x = index_X - 1;
        }
        else
            m_tmpCoordinate.x = index_X;


        if (index_Z % 2 == 1)
        {
            m_tmpCoordinate.z = index_Z + 1;
        }
        else if (index_Z % 2 == -1)
        {
            m_tmpCoordinate.z = index_Z - 1;
        }
        else
            m_tmpCoordinate.z = index_Z;


        // 속한 좌표의 인덱스를 뽑아서 반환한다.
        return m_Map_Coordinate_List.IndexOf(m_tmpCoordinate);
    }

    // =====================================================
    // =====================================================


    public void Next_Map_Load()
    {
        // 다음 맵 번호를 지정하고
        ++m_Current_Stage_index_Count;

        if (m_Stage_Number_List[m_Current_Stage_index_Count] != 0)
        {
            m_is_Map_Changing = true;

            // MCL을 재설정한다.
            MCL_init();

            // 남아있는 오브젝트들을 제거한다.
            Destroy_Objects();

            // 새 맵을 생성!
            Create_Map(m_Stage_Number_List[m_Current_Stage_index_Count]);



            Invoke("Map_Changing_Over", 1.0f);
        }
    }




    // 맵 전환이 끝났음을 알리는
    // Invoke를 위한 메소드
    void Map_Changing_Over()
    {
        m_is_Map_Changing = false;
    }




    // 스테이지 클리어!
    public void Stage_Clear()
    {
        m_is_Stage_Clear = true;
        m_is_Pause = true;

        // 별 획득 조건 체크 및 저장
        Condition_For_Getting_Stars(ref mission_list);

        // 현재 스테이지가 플레이 가능 최대 스테이지라면
        int tempMaxStage = PlayerPrefs.GetInt("Mode_Adventure_Playable_Max_Stage");

        if (PlayerPrefs.GetInt("Mode_Adventure_Current_Stage_ID") == tempMaxStage)
        {
            tempMaxStage += 1;

            // 최대 스테이지 범위 내에서 플레이가능 최대 스테이지 증가!
            if (tempMaxStage <= PlayerPrefs_Manager_Constants.MAX_STAGE_NUM)
            {
                PlayerPrefs.SetInt("Mode_Adventure_Playable_Max_Stage", tempMaxStage);
            }
        }

        PlayerPrefs.Save();

        // 클리어 화면 출력
        UI.Draw_StageClearPage();
    }


    // 별 획득 조건 관리
    void Condition_For_Getting_Stars(ref int[] list)
    {
        foreach (Adventure_Quest_Data QuestData in m_QuestList)
        {
            string tempString;
            for (int i = 0; i < 3; ++i)
            {
                if (QuestData.ID == list[i])
                {
                    Debug.Log(QuestData.ID);
                    Debug.Log(list[i]);

                    if (QuestData.Quest_ID == 1) // 시간
                    {
                        if (UI.time_Second >= QuestData.Quest_Goal)
                        {
                            m_Stars += 1;
                            tempString = "Adventure_Stars_ID_" + list[i];
                            PlayerPrefs.SetInt(tempString, 1);
                        }
                    }
                    else if (QuestData.Quest_ID == 2) // 일반몹처치
                    {
                        if (m_Total_Monster_Count - m_Left_Monster_Count >= QuestData.Quest_Goal)
                        {
                            m_Stars += 1;
                            tempString = "Adventure_Stars_ID_" + list[i];
                            PlayerPrefs.SetInt(tempString, 1);
                        }
                    }
                    else if (QuestData.Quest_ID == 3) // 목표지점
                    {
                        if (m_is_Goal_In)
                        {
                            m_Stars += 1;
                            tempString = "Adventure_Stars_ID_" + list[i];
                            PlayerPrefs.SetInt(tempString, 1);
                        }
                    }
                    else if (QuestData.Quest_ID == 4) // 보스처치
                    {
                        if (m_is_Boss_Dead)
                        {
                            m_Stars += 1;
                            tempString = "Adventure_Stars_ID_" + list[i];
                            PlayerPrefs.SetInt(tempString, 1);
                        }
                    }
                }
            }
        }
        Debug.Log(m_Stars);
        PlayerPrefs.Save();
    }

    public void SetBossDead(bool b)
    {
        m_is_Boss_Dead = b;
    }

    public bool GetBossDead()
    {
        return m_is_Boss_Dead;
    }

    public void SetGoalIn(bool b)
    {
        m_is_Goal_In = b;
    }

    // 퀘스트 내용 리스트를 리턴해준다. (UI에 띄우기 위해)
    public void GetQuestList(ref List<Adventure_Quest_Data> list)
    {
        list = m_QuestList;
    }

    // 보스 데이터를 리턴해준다.
    public Adventure_Boss_Data Get_Adventure_Boss_Data()
    {
        return m_Adventure_Boss_Data;
    }

    public void Destroy_Objects()
    {
        // 이하는 인스턴스 객체들을 제거한다.
        // =========================================

        // 테이블로 생성했던 오브젝트들을 제거
        foreach (GameObject g in m_Current_Map_Objects)
        {
            if (g != null)
            {
                Destroy(g);
            }
        }

        // 동적생성 폭탄들 제거
        GameObject[] bombs = GameObject.FindGameObjectsWithTag("Bomb");
        foreach (GameObject b in bombs)
            Destroy(b);

        // 동적생성 화염들 제거
        GameObject[] flame = GameObject.FindGameObjectsWithTag("Flame");
        foreach (GameObject f in flame)
            Destroy(f);

        // 동적생성 화염잔해들 제거
        GameObject[] flame_remains = GameObject.FindGameObjectsWithTag("Flame_Remains");
        foreach (GameObject fr in flame_remains)
            Destroy(fr);

        // 불 붙은 부쉬의 파티클도 제거
        GameObject[] fBushs = GameObject.FindGameObjectsWithTag("Flame_Bush_Particle");
        foreach (GameObject fb in fBushs)
            Destroy(fb);

        // 동적생성 아이템들도 제거
        GameObject[] items = GameObject.FindGameObjectsWithTag("Bomb_Item");
        foreach (GameObject i in items)
            Destroy(i);
        items = GameObject.FindGameObjectsWithTag("Fire_Item");
        foreach (GameObject i in items)
            Destroy(i);
        items = GameObject.FindGameObjectsWithTag("Speed_Item");
        foreach (GameObject i in items)
            Destroy(i);
        items = GameObject.FindGameObjectsWithTag("Kick_Item");
        foreach (GameObject i in items)
            Destroy(i);
        items = GameObject.FindGameObjectsWithTag("Throw_Item");
        foreach (GameObject i in items)
            Destroy(i);
        items = GameObject.FindGameObjectsWithTag("Airdrop_Item");
        foreach (GameObject i in items)
            Destroy(i);
        // ========================================
    }
}
