#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

const double Initial_money = 10; //�û�ע��ʱ���˻���ʼ���

struct Userinfo // ������Ϣ�ṹ��
{
	std::string username;
	std::string password;
	double money;
};

struct Deal //������Ϣ�ṹ��
{
	std::string payer;
	std::string payee;
	double change;
	int time;
};

struct Transfer_record//ʹ������洢���׼�¼
{
	std::string payer;//������
	std::string payee;//�տ���
	double change;//���䶯��������ת���⻹������ת���ת��
	int time;//����ʱ��

	struct Transfer_record* last;
	struct Transfer_record* next;
};

class Client
{
private:
	Userinfo clnt;
	Transfer_record* transfer_head; //ʹ������洢���׼�¼
	Transfer_record* transfer_tail; 

	void Create_node(std::string payer, std::string payee, double change, int time); //�������׼�¼����Ľڵ�
	bool Load_user(std::string username); //���뵱ǰ�û�����
	bool Write_user(std::string username); //д�뵱ǰ�û�����
	bool Write_money(std::string s_deal); //�û�ת��
	bool Register(std::string s_userinfo); //�û�ע�ᣬ���½��û��ļ���Ĭ�ϳ�ʼ�����
	
public:
	Client(); //���캯����ȫ����ʼ��Ϊ0���

	//����������������жϺ󣬷��ɵ������к���������RAFT������Ϣ
	bool Check_id(std::string username); //�����û����Ƿ����
	bool Check_fund(std::string username, double change); //��������Ƿ����
	
	double Get_money(std::string username); //�����˻����
	std::string Get_record(std::string username, int start, int end); //���ؽ��׼�¼
	bool Log_in(std::string username, std::string password); //�û���¼,�ж������Ƿ���ȷ

	//RAFTд���˱�
	bool Write_account(std::string s_string); //�ж��ַ������࣬Ȼ�����ת�˻�ע�ắ��

	~Client(); //�����������������
};