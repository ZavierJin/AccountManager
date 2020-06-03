#include "client.h"

Client::Client() {  //构造函数，全部初始化为0或空
	clnt.money = 0;
	clnt.username = "";
	clnt.password = "";
	transfer_head = NULL;
	transfer_tail = NULL;
}

void  Client::Create_node(std::string payer, std::string payee, double change, int time) // 有转账操作后再调用，初始直接赋值即可 
{
	// 创建新的 transfer_tool 的地址 
	Transfer_record* transfer_tool = new Transfer_record;
	transfer_tool->payer = payer;
	transfer_tool->payee = payee;
	transfer_tool->change = change;
	transfer_tool->time = time;

	if (transfer_head == NULL) {
		transfer_tool->last = NULL; // 上一个 
		transfer_tool->next = NULL; // 下一个 

		transfer_head = transfer_tool;
		transfer_tail = transfer_head;
	}
	else
	{
		transfer_tool->last = transfer_tail; // 上一个 
		transfer_tool->next = NULL; // 下一个 
			
		transfer_tail->next = transfer_tool;
		transfer_tail = transfer_tail->next;
    }
}

bool Client::Load_user(std::string username) //载入当前用户数据
{
	using namespace std;
	if (username != clnt.username) //当输入的用户名与当前用户不同时，重新载入当前用户数据，否则无需重新载入
	{
		clnt.money = 0;
		clnt.username = "";
		clnt.password = "";
		while (transfer_head != NULL) //清空当前链表
		{
			Transfer_record* delet;
			delet = transfer_head;
			transfer_head = transfer_head->next;
			delete delet;
		}
		transfer_tail = NULL;

		string filename = username;
		filename = "Users\\" + filename + ".txt";

		ifstream fin;
		fin.open(filename.c_str(), ios_base::in);
		if (fin.is_open()) {
			fin >> clnt.username >> clnt.password >> clnt.money;
			Transfer_record temp;
			while (fin >> temp.payer >> temp.payee >> temp.change >> temp.time)
			{
				Create_node(temp.payer, temp.payee, temp.change, temp.time);
			}
			fin.close();
		}
		else
		{
			return false; //若失败则终止
		}
		return true;
	}
}

bool Client::Write_user(std::string username) //写入当前用户数据
{
	using namespace std;
	string filename = username;
	filename = "Users\\" + filename + ".txt";

	ofstream fout;
	fout.open(filename.c_str(), ios_base::out);
	if (!fout.is_open()) {
		return false; //若失败则终止
	}

	fout << clnt.username << endl << clnt.password << endl << clnt.money << endl;
	Transfer_record* temp = transfer_head;
	while (temp != NULL)
	{
		fout << temp->payer << ' ' << temp->payee << ' ' << temp->change << ' ' << temp->time << endl;
		temp = temp->next;
	}
	fout.close();
	return true;
}

bool Client::Check_id(std::string username) //检验用户名是否存在,主程序用此函数判断后，方可调用下列函数
{
	using namespace std;
	string filename = "users.txt";
	string temp;

	ifstream fin;
	fin.open(filename.c_str(), ios_base::in);
	if (fin.is_open()) {
		while (fin >> temp) //反复读取判断
		{
			if (temp == username)
			{
				fin.close();
				return true; //用户名存在
			}	
		}
		fin.clear(); //恢复输入
		fin.close();
		return false; //用户名不存在
	}
	else
	{
		return false; //若文件打开失败，则视为用户名不存在
	}
}

bool Client::Check_fund(std::string username, double change) //检验余额是否充足
{
	if (!Load_user(username)) //若文件打开失败，则视为余额不充足
		return false;
	if (clnt.money >= change)
		return true;
	else
		return false;
}

double Client::Get_money(std::string username) //返回账户余额
{
	if (!Load_user(username)) //若文件打开失败，返回-1
		return -1;
	return clnt.money;
}

std::string Client::Get_record(std::string username, int start, int end) //返回交易记录
{
	if (!Load_user(username)) //若文件打开失败，返回-1
		return "-1";
	std::string record = "";
	Transfer_record* temp = transfer_head;
	while (temp != NULL)
	{
		if ((temp->time >= start) && (temp->time <= end)) //将指定时间段内的交易记录加入字符串
		{
			record = record + temp->payer + " " + temp->payee + " " + std::to_string(temp->change) + " " + std::to_string(temp->time) + "\t";
			temp = temp->next;
		}
		else if (temp->time > end) //超出截止时间后，结束循环
			break;
	}
	return record;
}

bool Client::Log_in(std::string username, std::string password) //用户登录,判断密码是否正确
{
	if (!Load_user(username)) //若文件打开失败，则视为登录失败
		return false;
	if (password == clnt.password)
		return true;
	else
		return false;
}

bool Client::Write_account(std::string s_string) //判断字符串种类，然后调用转账或注册函数
{
	int count = 0;
	for (int i = 0; i < s_string.length(); i++)
	{
		if (s_string[i] == ' ')
		{
			count++;
		}
	}
	if (count == 3) //若字符串中有三个空格，注册
	{
		if (!Register(s_string))
			return false; //若失败则终止
	}	
	else if (count == 4) //若字符串中有四个空格，转账
	{
		if (!Write_money(s_string))
			return false; //若失败则终止
	}
	else //若字符串不符合要求，失败
	{
		return false; //若失败则终止
	}
	return true;
}

bool Client::Write_money(std::string s_deal) //用户转账
{
	using namespace std;
	std::string payer;
	std::string payee;
	double change;
	int time;
	istringstream is(s_deal); //字符串流输入
	is >> payer >> payee >> change >> time;

	if (!Load_user(payer)) //载入转出方
		return false; //若失败则终止
	clnt.money -= change;
	Create_node(payer, payee, change, time); //链表部分储存转账记录
	if (!Write_user(payer)) //写入转出方
		return false; //若失败则终止
		
	if (!Load_user(payee)) //载入转入方
		return false; //若失败则终止
	clnt.money += change;
	Create_node(payer, payee, change, time); //链表部分储存转账记录
	if (!Write_user(payee)) //写入转入方
		return false; //若失败则终止

	return true;
}

bool Client::Register(std::string s_userinfo) //用户注册，即新建用户文件
{
	using namespace std;
	std::string username;
	std::string password;
	double money;
	istringstream is(s_userinfo); //字符串流输入
	is >> username >> password >> money;
	
	ofstream fout;
	string filename = username;
	filename = "Users\\" + filename + ".txt"; //打开用户的文件，写入信息
	fout.open(filename.c_str(), ios_base::out);
	if (!fout.is_open()) {
		return false; //若失败则终止
	}
	fout << username << endl << password << endl << money << endl;
	fout.close();

	filename = "users.txt";
	fout.open(filename.c_str(), ios_base::app); //打开存储所有用户名的文件，只追加的方式写入
	if (!fout.is_open()) {
		return false; //若失败则终止
	}
    fout << username << endl;
	fout.close();
	return true;
}

Client::~Client() //析构函数，清空链表
{
    while(transfer_head != NULL)
	{
		Transfer_record* delet;   // 临时声明一个变量，清除整个链表就行了 
		delet = transfer_head;
		transfer_head = transfer_head->next;
		delete delet;
	}
}