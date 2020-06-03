#include "client.h"

Client::Client() {  //���캯����ȫ����ʼ��Ϊ0���
	clnt.money = 0;
	clnt.username = "";
	clnt.password = "";
	transfer_head = NULL;
	transfer_tail = NULL;
}

void  Client::Create_node(std::string payer, std::string payee, double change, int time) // ��ת�˲������ٵ��ã���ʼֱ�Ӹ�ֵ���� 
{
	// �����µ� transfer_tool �ĵ�ַ 
	Transfer_record* transfer_tool = new Transfer_record;
	transfer_tool->payer = payer;
	transfer_tool->payee = payee;
	transfer_tool->change = change;
	transfer_tool->time = time;

	if (transfer_head == NULL) {
		transfer_tool->last = NULL; // ��һ�� 
		transfer_tool->next = NULL; // ��һ�� 

		transfer_head = transfer_tool;
		transfer_tail = transfer_head;
	}
	else
	{
		transfer_tool->last = transfer_tail; // ��һ�� 
		transfer_tool->next = NULL; // ��һ�� 
			
		transfer_tail->next = transfer_tool;
		transfer_tail = transfer_tail->next;
    }
}

bool Client::Load_user(std::string username) //���뵱ǰ�û�����
{
	using namespace std;
	if (username != clnt.username) //��������û����뵱ǰ�û���ͬʱ���������뵱ǰ�û����ݣ�����������������
	{
		clnt.money = 0;
		clnt.username = "";
		clnt.password = "";
		while (transfer_head != NULL) //��յ�ǰ����
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
			return false; //��ʧ������ֹ
		}
		return true;
	}
}

bool Client::Write_user(std::string username) //д�뵱ǰ�û�����
{
	using namespace std;
	string filename = username;
	filename = "Users\\" + filename + ".txt";

	ofstream fout;
	fout.open(filename.c_str(), ios_base::out);
	if (!fout.is_open()) {
		return false; //��ʧ������ֹ
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

bool Client::Check_id(std::string username) //�����û����Ƿ����,�������ô˺����жϺ󣬷��ɵ������к���
{
	using namespace std;
	string filename = "users.txt";
	string temp;

	ifstream fin;
	fin.open(filename.c_str(), ios_base::in);
	if (fin.is_open()) {
		while (fin >> temp) //������ȡ�ж�
		{
			if (temp == username)
			{
				fin.close();
				return true; //�û�������
			}	
		}
		fin.clear(); //�ָ�����
		fin.close();
		return false; //�û���������
	}
	else
	{
		return false; //���ļ���ʧ�ܣ�����Ϊ�û���������
	}
}

bool Client::Check_fund(std::string username, double change) //��������Ƿ����
{
	if (!Load_user(username)) //���ļ���ʧ�ܣ�����Ϊ������
		return false;
	if (clnt.money >= change)
		return true;
	else
		return false;
}

double Client::Get_money(std::string username) //�����˻����
{
	if (!Load_user(username)) //���ļ���ʧ�ܣ�����-1
		return -1;
	return clnt.money;
}

std::string Client::Get_record(std::string username, int start, int end) //���ؽ��׼�¼
{
	if (!Load_user(username)) //���ļ���ʧ�ܣ�����-1
		return "-1";
	std::string record = "";
	Transfer_record* temp = transfer_head;
	while (temp != NULL)
	{
		if ((temp->time >= start) && (temp->time <= end)) //��ָ��ʱ����ڵĽ��׼�¼�����ַ���
		{
			record = record + temp->payer + " " + temp->payee + " " + std::to_string(temp->change) + " " + std::to_string(temp->time) + "\t";
			temp = temp->next;
		}
		else if (temp->time > end) //������ֹʱ��󣬽���ѭ��
			break;
	}
	return record;
}

bool Client::Log_in(std::string username, std::string password) //�û���¼,�ж������Ƿ���ȷ
{
	if (!Load_user(username)) //���ļ���ʧ�ܣ�����Ϊ��¼ʧ��
		return false;
	if (password == clnt.password)
		return true;
	else
		return false;
}

bool Client::Write_account(std::string s_string) //�ж��ַ������࣬Ȼ�����ת�˻�ע�ắ��
{
	int count = 0;
	for (int i = 0; i < s_string.length(); i++)
	{
		if (s_string[i] == ' ')
		{
			count++;
		}
	}
	if (count == 3) //���ַ������������ո�ע��
	{
		if (!Register(s_string))
			return false; //��ʧ������ֹ
	}	
	else if (count == 4) //���ַ��������ĸ��ո�ת��
	{
		if (!Write_money(s_string))
			return false; //��ʧ������ֹ
	}
	else //���ַ���������Ҫ��ʧ��
	{
		return false; //��ʧ������ֹ
	}
	return true;
}

bool Client::Write_money(std::string s_deal) //�û�ת��
{
	using namespace std;
	std::string payer;
	std::string payee;
	double change;
	int time;
	istringstream is(s_deal); //�ַ���������
	is >> payer >> payee >> change >> time;

	if (!Load_user(payer)) //����ת����
		return false; //��ʧ������ֹ
	clnt.money -= change;
	Create_node(payer, payee, change, time); //�����ִ���ת�˼�¼
	if (!Write_user(payer)) //д��ת����
		return false; //��ʧ������ֹ
		
	if (!Load_user(payee)) //����ת�뷽
		return false; //��ʧ������ֹ
	clnt.money += change;
	Create_node(payer, payee, change, time); //�����ִ���ת�˼�¼
	if (!Write_user(payee)) //д��ת�뷽
		return false; //��ʧ������ֹ

	return true;
}

bool Client::Register(std::string s_userinfo) //�û�ע�ᣬ���½��û��ļ�
{
	using namespace std;
	std::string username;
	std::string password;
	double money;
	istringstream is(s_userinfo); //�ַ���������
	is >> username >> password >> money;
	
	ofstream fout;
	string filename = username;
	filename = "Users\\" + filename + ".txt"; //���û����ļ���д����Ϣ
	fout.open(filename.c_str(), ios_base::out);
	if (!fout.is_open()) {
		return false; //��ʧ������ֹ
	}
	fout << username << endl << password << endl << money << endl;
	fout.close();

	filename = "users.txt";
	fout.open(filename.c_str(), ios_base::app); //�򿪴洢�����û������ļ���ֻ׷�ӵķ�ʽд��
	if (!fout.is_open()) {
		return false; //��ʧ������ֹ
	}
    fout << username << endl;
	fout.close();
	return true;
}

Client::~Client() //�����������������
{
    while(transfer_head != NULL)
	{
		Transfer_record* delet;   // ��ʱ����һ������������������������ 
		delet = transfer_head;
		transfer_head = transfer_head->next;
		delete delet;
	}
}