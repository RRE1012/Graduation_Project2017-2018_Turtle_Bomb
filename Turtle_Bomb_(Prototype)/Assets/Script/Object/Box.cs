﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Box : MonoBehaviour {
    
    public GameObject m_Object_FireItem;
    public GameObject m_Object_BombItem;
    public GameObject m_Object_SpeedItem;
    public GameObject m_Object_KickItem;
    public GameObject m_Object_ThrowItem;
    
    public GameObject m_Particle;

    GameObject m_PlayerCollider = null; // 플레이어의 밀기용 감지기
    GameObject m_IcicleCollider = null; // 고드름 바닥 충돌체

    protected bool m_is_Destroyed = false;
    protected int m_MCL_index; public int Get_MCL_index() { return m_MCL_index; }

    void Start()
    {
        m_MCL_index = StageManager.GetInstance().Find_Own_MCL_Index(transform.position.x, transform.position.z);
    }

    public void Reset_MCL_index(int index)
    {
        StageManager.GetInstance().Update_MCL_isBlocked(m_MCL_index, false);
        m_MCL_index = index;
        StageManager.GetInstance().Update_MCL_isBlocked(m_MCL_index, true);
    }

    void OnDestroy()
    {
        if (m_PlayerCollider) // 충돌중인 감지기가 있으면
        {
            m_PlayerCollider.GetComponent<Front_Collider>().TriggerExit_Ver2(); // 트리거아웃을 직접 수행해준다.
        }
        if (m_IcicleCollider) // 충돌중인 고드름바닥이 있으면
        {
            m_IcicleCollider.GetComponent<Icicle_Bottom>().TriggerExit_Ver2(); // 트리거아웃을 직접 수행해준다.
        }
    }


    void OnTriggerEnter(Collider other)
    {
        if (!m_is_Destroyed && (other.gameObject.CompareTag("Flame_Remains") || other.gameObject.CompareTag("Flame_Crash") || other.gameObject.CompareTag("Flame")))
        {
            m_is_Destroyed = true;

            // MCL 갱신
            StageManager.GetInstance().Update_MCL_isBlocked(m_MCL_index, false);

            SetItem(); // 아이템 생성

            Instantiate(m_Particle).transform.position = transform.position; // 파티클 발생

            Destroy(gameObject); // 박스 파괴
        }
    }

    protected void SetItem()
    {
        if (Random.Range(0, 100) > 51)
        {
            GameObject Instance_Item;
            int dropRate = Random.Range(0, 100);
            if (dropRate < 25)
            {
                m_Object_FireItem.transform.position = new Vector3(transform.position.x, 0.6f, transform.position.z);
                Instance_Item = Instantiate(m_Object_FireItem);
            }
            else if (dropRate >= 25 && dropRate < 55)
            {
                m_Object_BombItem.transform.position = new Vector3(transform.position.x, 0.6f, transform.position.z);
                Instance_Item = Instantiate(m_Object_BombItem);
            }
            else if (dropRate >= 55 && dropRate < 80)
            {
                m_Object_SpeedItem.transform.position = new Vector3(transform.position.x, 0.6f, transform.position.z);
                Instance_Item = Instantiate(m_Object_SpeedItem);
            }
            else if (dropRate >= 80 && dropRate < 90)
            {
                m_Object_KickItem.transform.position = new Vector3(transform.position.x, 0.6f, transform.position.z);
                Instance_Item = Instantiate(m_Object_KickItem);
            }
            else
            {
                m_Object_ThrowItem.transform.position = new Vector3(transform.position.x, 0.6f, transform.position.z);
                Instance_Item = Instantiate(m_Object_ThrowItem);
            }
        }
    }

    public void Save_Player_Front_Collider(GameObject fc)
    {
        m_PlayerCollider = fc;
    }
    public void Clear_Player_Front_Collider()
    {
        m_PlayerCollider = null;
    }

    public void Save_Icicle_Bottom_Collider(GameObject ic)
    {
        m_IcicleCollider = ic;
    }
    public void Clear_Icicle_Bottom_Collider()
    {
        m_IcicleCollider = null;
    }
}