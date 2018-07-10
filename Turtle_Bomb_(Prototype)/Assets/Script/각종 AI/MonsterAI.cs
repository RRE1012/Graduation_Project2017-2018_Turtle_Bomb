﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

static class Monster_AI_Constants
{
    public const float Walk_Time = 3.0f;
    public const float Attack_Time = 1.0f;
    public const float Boss_Attack_Time = 0.3f;
}

public class MonsterAI : MonoBehaviour
{
    Animator m_Goblman_Animator;

    Monster_Player_Detector m_Attack_Detector;
    Transform m_Attack_Collider;

    IEnumerator m_Current_Behavior;
    IEnumerator m_Behavior_Move;
    IEnumerator m_Behavior_Chase;
    IEnumerator m_Behavior_Attack;

    Vector3 m_V3_To_Find_my_rightObject;
    Vector3 m_V3_To_Find_my_leftObject;

    int m_My_MCL_Index = 0;
    int m_Right_Object_Index = 0;
    int m_Left_Object_Index = 0;
    
    bool m_isFound_Turtleman;

    float m_Monster_Basic_Speed;

    float m_WalkTimer;
    float m_AttackTimer;

    bool m_isDead;
    bool m_isAttacking;

    float m_escape_Time;

    void Start()
    {
        StageManager.c_Stage_Manager.Increase_Normal_Monster_Count(); // 몬스터 개수 증가

        // 객체 자신이 가지고 있는 애니메이터를 찾아온다.
        m_Goblman_Animator = GetComponent<Animator>();
        m_Attack_Detector = GetComponentInChildren<Monster_Player_Detector>();
        m_Attack_Collider = transform.Find("Attack_Range");
        m_Attack_Collider.gameObject.SetActive(false);

        m_WalkTimer = 0.0f;
        m_AttackTimer = 0.0f;
        m_escape_Time = 0.0f;

        m_Monster_Basic_Speed = 2.0f;

        m_isDead = false;
        m_isAttacking = false;
        
        m_isFound_Turtleman = false;
        

        // 몬스터의 행동 코루틴들을 설정
        m_Behavior_Move = MonsterMove();
        //m_Behavior_Chase = ;
        m_Behavior_Attack = MonsterAttack();

        // 처음 실행할 행동 설정
        m_Current_Behavior = MonsterMove();
        StartCoroutine(MonsterBehavior());
    }

    void OnCollisionEnter(Collision collision)
    {
        // 아래 구문을 통해 몬스터가 막힌 길로 들어서지 않도록 한다.
        if (collision.gameObject.CompareTag("Box") 
            || collision.gameObject.CompareTag("Wall") 
            || collision.gameObject.CompareTag("Rock")
            || collision.gameObject.CompareTag("Bomb"))
        {
            // 올바른 위치로 이동
            Set_Right_Position();

            // 방향도 전환시킨다.
            Find_New_Way();
        }

        if (collision.gameObject.CompareTag("Monster"))
        {
            Find_New_Way();
        }
        // ==============================
    }
    

    void OnTriggerEnter(Collider other)
    {
        // 몬스터가 불에 닿으면 사망 판정
        if (!m_isDead && (other.gameObject.tag == "Flame" || other.gameObject.CompareTag("Flame_Bush")))
        {
            if (MusicManager.manage_ESound != null)
                MusicManager.manage_ESound.Goblin_Dead_Sound();

            StageManager.c_Stage_Manager.Decrease_Normal_Monster_Count(); // 불에 닿아 죽는것만 카운팅.
            m_Goblman_Animator.SetBool("Goblman_isDead", true);
            m_isDead = true;
            StopCoroutine(MonsterBehavior());
            Invoke("MonsterDead", 1.5f);
        }
        //===============================
    }

    void Think()
    {
        // 플레이어가 감지범위 안에 들어왔고, 공격모션 중이 아니라면
        // 공격
        if (m_Attack_Detector.m_isInRange && !m_isAttacking)
        {
            if (m_Current_Behavior != m_Behavior_Attack)
                m_Current_Behavior = m_Behavior_Attack;
            //m_isAttacking = true;
            if (m_Attack_Detector.Get_Target())
            {
                Vector3 dir = m_Attack_Detector.Get_Target().transform.position - transform.position;
                Vector3 dirXZ = new Vector3(dir.x, 0f, dir.z);

                if (dirXZ != Vector3.zero)
                {
                    Quaternion targetRot = Quaternion.LookRotation(dirXZ);

                    transform.rotation = targetRot;
                    Debug.Log("Rot Changed");
                }
            }
        }

        // 플레이어가 감지범위 밖에 있고, 공격모션 중이 아니라면
        // 걷기
        else if (!m_Attack_Detector.m_isInRange && !m_isAttacking)
        {
            // 이전 행동이 "공격"이었다면
            // 방향을 수정한다.
            if (m_Current_Behavior == m_Behavior_Attack)
            {
                float AngleY = 0.0f;
                if (transform.localEulerAngles.y >= 315.0f && transform.localEulerAngles.y < 45.0f)
                    AngleY = 0.0f;
                else if (transform.localEulerAngles.y >= 45.0f && transform.localEulerAngles.y < 135.0f)
                    AngleY = 90.0f;
                else if (transform.localEulerAngles.y >= 135.0f && transform.localEulerAngles.y < 225.0f)
                    AngleY = 180.0f;
                else if (transform.localEulerAngles.y >= 225.0f && transform.localEulerAngles.y < 315.0f)
                    AngleY = 270.0f;
                transform.localEulerAngles = new Vector3(0.0f, AngleY, 0.0f);

                Set_Right_Position();
            }

            if (m_Current_Behavior != m_Behavior_Move)
            {
                m_Current_Behavior = m_Behavior_Move;
            }
        }
    }

    IEnumerator MonsterBehavior()
    {
        while (true)
        {
            Think();
            if (!StageManager.c_Stage_Manager.Get_is_Pause() && !m_isDead && m_Current_Behavior != null && m_Current_Behavior.MoveNext())
            {
                yield return m_Current_Behavior.Current;
            }

            else
            {
                yield return null;
            }
        }
    }


    IEnumerator MonsterMove()
    {
        while (true)
        {
            m_WalkTimer += Time.deltaTime;
            
            if (m_WalkTimer < Monster_AI_Constants.Walk_Time)
            {
                if (StageManager.c_Stage_Manager.Get_is_Intro_Over() && PlayerMove.C_PM.Get_IsAlive() && !StageManager.c_Stage_Manager.Get_is_Stage_Clear())
                {
                    transform.Translate(new Vector3(0.0f, 0.0f, (m_Monster_Basic_Speed * Time.deltaTime)));
                    m_Goblman_Animator.SetBool("Goblman_isWalk", true);
                    m_Goblman_Animator.SetBool("Goblman_isIdle", false);
                    Find_My_Coord();
                }
            }
            else
            {
                if (MusicManager.manage_ESound != null)
                    MusicManager.manage_ESound.Goblin_Idle_Sound();

                m_Goblman_Animator.SetBool("Goblman_isWalk", false);
                m_Goblman_Animator.SetBool("Goblman_isIdle", true);
                yield return new WaitForSeconds(2.0f);
                m_WalkTimer = 0.0f;
            }

            yield return null;
        }
    }


    
    IEnumerator MonsterAttack()
    {
        while (true)
        {
            m_AttackTimer += Time.deltaTime;

            if (m_AttackTimer < Monster_AI_Constants.Attack_Time)
            {
                if (StageManager.c_Stage_Manager.Get_is_Intro_Over() && !StageManager.c_Stage_Manager.Get_Game_Over() && !m_Goblman_Animator.GetBool("Goblman_isAttack"))
                {
                    if (MusicManager.manage_ESound != null)
                        MusicManager.manage_ESound.Goblin_Attack_Sound();
                    m_Attack_Collider.gameObject.SetActive(true); // 충돌체 활성화
                    m_Goblman_Animator.SetBool("Goblman_isAttack", true);
                    m_Goblman_Animator.SetBool("Goblman_isIdle", false);
                    m_isAttacking = true;
                }
            }

            else
            {
                m_Goblman_Animator.SetBool("Goblman_isAttack", false);
                m_Goblman_Animator.SetBool("Goblman_isIdle", true);
                m_Attack_Collider.gameObject.SetActive(false);
                m_isAttacking = false;
                m_AttackTimer = 0.0f;
                yield return new WaitForSeconds(0.5f);
            }

            yield return null;
        }
    }

    


    void MonsterDead()
    {
        Destroy(gameObject);
    }




    // 몬스터가 새로운 길을 찾도록 하는 함수
    void Find_New_Way()
    {
        m_V3_To_Find_my_leftObject = transform.position - transform.right * 2.0f;
        m_V3_To_Find_my_rightObject = transform.position + transform.right * 2.0f;
        

        m_Right_Object_Index = Find_Objects_Coord(m_V3_To_Find_my_rightObject.x, m_V3_To_Find_my_rightObject.z);
        m_Left_Object_Index = Find_Objects_Coord(m_V3_To_Find_my_leftObject.x, m_V3_To_Find_my_leftObject.z);

        
        if (m_Left_Object_Index != -1 && StageManager.c_Stage_Manager.Get_MCL_index_is_Blocked(m_Left_Object_Index) == false)
        {
            transform.Rotate(-transform.up * 90.0f);
        }

        else if (m_Right_Object_Index != -1 && StageManager.c_Stage_Manager.Get_MCL_index_is_Blocked(m_Right_Object_Index) == false)
        {
            transform.Rotate(transform.up * 90.0f);
        }

        else transform.Rotate(transform.up * 180.0f);
    }
    // ==============================


    

    public void Set_Basic_Speed(float s)
    {
        m_Monster_Basic_Speed = s;
    }



    // 몬스터 자신의 MCL 인덱스를 받아오는 함수
    void Find_My_Coord()
    {
        if (StageManager.c_Stage_Manager.Get_is_init_MCL())
        {
            m_My_MCL_Index = StageManager.c_Stage_Manager.Find_Own_MCL_Index(transform.position.x, transform.position.z);
        }
    }
    // =========================================

    
    // 몬스터 자신의 MCL 인덱스에 해당하는 위치로 옮겨주는 함수
    void Set_Right_Position()
    {
        if (StageManager.c_Stage_Manager.Get_is_init_MCL())
        {
            m_My_MCL_Index = StageManager.c_Stage_Manager.Find_Own_MCL_Index(transform.position.x, transform.position.z);
            
            if (m_My_MCL_Index != -1)
            {
                Vector3 Loc;
                Loc.x = StageManager.c_Stage_Manager.m_Map_Coordinate_List[m_My_MCL_Index].x;
                Loc.y = transform.position.y;
                Loc.z = StageManager.c_Stage_Manager.m_Map_Coordinate_List[m_My_MCL_Index].z;
                transform.position = Loc;
            }
        }
    }
    // =========================================


    // 오브젝트의 좌표를 통하여 해당 MCL 인덱스를 반환한다.
    int Find_Objects_Coord(float x, float z)
    {
        if (StageManager.c_Stage_Manager.Get_is_init_MCL())
        {
            return StageManager.c_Stage_Manager.Find_Own_MCL_Index(x, z);
        }
        return -1;
    }
    // =========================================
}
