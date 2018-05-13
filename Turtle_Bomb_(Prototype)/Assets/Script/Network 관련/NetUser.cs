﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Threading;
using UnityEngine.UI;
public class NetUser : MonoBehaviour
{
    public static NetUser instance;
    public Text m_TurtleTXT;
    public TextMesh m_TM;
    public GameObject p;
    bool textmesh_On=false;
    byte color = 0;
    byte id = 254;
    bool dead_ani = false;
    bool throw_ani = false;
    bool walk_ani = false;
    bool push_ani = false;
    bool kick_ani = false;
    Animator m_animator;
    public Material[] icon_material;
    public GameObject m_plane;
    public Renderer m_Rplane;
    void Awake()
    {
        instance = this;
    }
    // Use this for initialization
    void Start()
    {
        dead_ani = false;
        //Invoke("IDCheck", 2.0f);
        m_animator = GetComponent<Animator>();
    }
    public void SetMoveMotion()
    {
        walk_ani = true;
    }
    void SetFalse()
    {
        gameObject.SetActive(false);
    }
    public void SetDeadMotion()
    {
        dead_ani = true;
    }
    public void SetKickMotion()
    {
        kick_ani = true;
    }
    public void SetThrowMotion()
    {
        throw_ani = true;
    }
    public void SetPushMotion()
    {
        push_ani = true;
    }
    void Push_Ani_False()
    {
        m_animator.SetBool("TurtleMan_isPush", false);
    }
    void Throw_Ani_False()
    {
        m_animator.SetBool("TurtleMan_isThrow", false);
    }
    void Walk_Ani_False()
    {
        m_animator.SetBool("TurtleMan_isWalk", false);
    }
    void Kick_Ani_False()
    {
        m_animator.SetBool("TurtleMan_isKick", false);
    }
    public void SetPos(float x, float y, float z)
    {
        float tempx = x - transform.position.x;
        float tempz = z - transform.position.z;
        float tempy = y - transform.rotation.y;

        for (int i = 0; i < 4; ++i)
        {
            //transform.position = new Vector3(transform.position.x+(tempx/4), transform.position.y, transform.position.z+(tempz/4));
            transform.position = Vector3.MoveTowards(transform.position, new Vector3(transform.position.x + (tempx / 4), transform.position.y, transform.position.z + (tempz / 4)), 0.5f);
            transform.rotation = new Quaternion(transform.rotation.x, transform.rotation.y + (tempy / 4), transform.rotation.z, transform.rotation.w);
            //Thread.Sleep(125);
            //new WaitForSeconds(0.125f);
            //yield WaitForSeconds(0.125f);
        }

    }

    public void SetText(byte itemtype)
    {
        textmesh_On = true;

        color = itemtype;

    }

    public void SetTextOff()
    {
        m_TM.gameObject.SetActive(false);
    }
    public void SetIconOff()
    {
        m_plane.gameObject.SetActive(false);
    }
    // Update is called once per frame
    void Update()
    {
        if (kick_ani)
        {
            m_animator.SetBool("TurtleMan_isKick", true);
            Invoke("Kick_Ani_False", 1.0f);
            kick_ani = false;
        }
        if (push_ani)
        {
            m_animator.SetBool("TurtleMan_isPush", true);
            Invoke("Push_Ani_False", 1.0f);
            push_ani = false;
        }
        if (walk_ani)
        {
            m_animator.SetBool("TurtleMan_isWalk", true);
            Invoke("Walk_Ani_False", 1.0f);
            walk_ani = false;
        }
        //m_TM.transform.position = new Vector3(gameObject.transform.position.x, m_TM.transform.position.y, gameObject.transform.position.z);
        if (dead_ani)
        {
            m_TM.gameObject.SetActive(true);

            m_TM.text = "1P - Dead!!!";
            m_animator.SetBool("TurtleMan_isDead", true);
            Invoke("SetTextOff", 2.0f);
            
            Invoke("SetFalse", 1.1f);
            dead_ani = false;
        }
        if (throw_ani)
        {
            m_animator.SetBool("TurtleMan_isThrow", true);
            Invoke("Throw_Ani_False", 1.0f);
            throw_ani = false;
        }
        SetPos(NetTest.instance.GetNetPosx(0), NetTest.instance.GetNetRoty(0), NetTest.instance.GetNetPosz(0));
        if (VariableManager.instance.people_inRoom[0] == 0)
            p.SetActive(false);
        else if (VariableManager.instance.pos_inRoom-1 == 0)
        {
            p.SetActive(false);
        }
        else
        {
            p.SetActive(true);
        }
        m_TurtleTXT.text = "ID: " + Turtle_Move.instance.GetId();
        //m_TurtleTXT.text = "X:" + transform.position.x+"\nZ:"+ transform.position.z;
        //m_TM.text = "X:" + transform.position.x;
        if (textmesh_On)
        {
            m_plane.gameObject.SetActive(true);
            switch (color)
            {
                case 0:
                    m_Rplane.sharedMaterial = icon_material[0];
                    //m_TM.color = new Color(0, 0, 1);
                    //m_TM.text = "Bomb Up~";
                    break;
                case 1:
                    m_Rplane.sharedMaterial = icon_material[1];
                    //m_TM.color = new Color(1, 0, 0);
                    //m_TM.text = "Fire Up~";
                    break;
                case 2:
                    m_Rplane.sharedMaterial = icon_material[2];
                    //m_TM.color = new Color(1, 1, 0);
                    //m_TM.text = "Speed Up~";
                    break;
                default:
                    break;
            }
            Invoke("SetIconOff", 2.0f);
            textmesh_On = false;
        }
    }

    void IDCheck()
    {
        if (p != null)
        {
            if (Turtle_Move.instance.GetId() == 0 || Turtle_Move.instance.GetId() > 3)
            {

            }
            else
                StartCoroutine("NetworkCheck");
        }
    }

    IEnumerator NetworkCheck()
    {
        for (; ; )
        {

            SetPos(NetTest.instance.GetNetPosx(0), NetTest.instance.GetNetRoty(0), NetTest.instance.GetNetPosz(0));

            yield return new WaitForSeconds(0.05f);
        }
    }
}
