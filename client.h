#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

const double Initial_money = 10; //用户注册时，账户初始金额

struct Userinfo // 个人信息结构体
{
	std::string username;
	std::string password;
	double money;
};

struct Deal //交易信息结构体
{
	std::string payer;
	std::string payee;
	double change;
	int time;
};

struct Transfer_record//使用链表存储交易记录
{
	std::string payer;//付款人
	std::string payee;//收款人
	double change;//金额变动量，除了转账外还可能有转入和转出
	int time;//交易时间

	struct Transfer_record* last;
	struct Transfer_record* next;
};

class Client
{
private:
	Userinfo clnt;
	Transfer_record* transfer_head; //使用链表存储交易记录
	Transfer_record* transfer_tail; 

	void Create_node(std::string payer, std::string payee, double change, int time); //创建交易记录链表的节点
	bool Load_user(std::string username); //载入当前用户数据
	bool Write_user(std::string username); //写入当前用户数据
	bool Write_money(std::string s_deal); //用户转账
	bool Register(std::string s_userinfo); //用户注册，即新建用户文件，默认初始化余额
	
public:
	Client(); //构造函数，全部初始化为0或空

	//审核用这两个函数判断后，方可调用下列函数，或向RAFT发送信息
	bool Check_id(std::string username); //检验用户名是否存在
	bool Check_fund(std::string username, double change); //检验余额是否充足
	
	double Get_money(std::string username); //返回账户余额
	std::string Get_record(std::string username, int start, int end); //返回交易记录
	bool Log_in(std::string username, std::string password); //用户登录,判断密码是否正确

	//RAFT写入账本
	bool Write_account(std::string s_string); //判断字符串种类，然后调用转账或注册函数

	~Client(); //析构函数，清空链表
};