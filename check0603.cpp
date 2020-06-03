/*
**20200603
**�������
*/

#include <iostream>
#include "Computer.h"
#include <map>
#include <unordered_map>
#include "client.h"
#include<Windows.h>

using namespace std;

string raft_forward;//�����˷����ַ���
int state = 0;//��raft��string��״̬����,state=1��ʾ��raft�����ַ���

int leaderid;//��ʾleaderid�����ڲ���


std::string Userinfo_to_string(Userinfo userinfo) //��������û���Ϣת��Ϊ�ַ�����
{
	std::string s_userinfo = userinfo.username + " " + userinfo.password + " " + std::to_string(userinfo.money) + " ";
	return s_userinfo;
}

std::string Deal_to_string(Deal deal) //������Ľ�����Ϣת��Ϊ�ַ�����
{
	std::string s_deal = deal.payer + " " + deal.payee + " " + std::to_string(deal.change) + " " + std::to_string(deal.time) + " ";
	return s_deal;
}


void check()
{
	using namespace std;

	auto &com = computer::Computer::instance();
	auto id = com.registerServer();

	Client client;

	auto req2 = com.getRequest();
	while (req2 == nullptr)
		req2 = com.getRequest();

	answer::FeedBack feb_ans(id == leaderid, leaderid);
	answer::FeedBack *r = &feb_ans;
	com.sendAnswer(*r, req2->getAddress);//�����Ƿ���leader��leaderid

	switch (req2->getKind())
	{
	case request::Kind::Register:
	{
		Userinfo userinfo;
		userinfo.username = req2->getUser();
		userinfo.password = req2->getPassword();
		userinfo.money = Initial_money;

		if (!client.Check_id(userinfo.username))  //�����û����Ƿ����
		{
			raft_forward = Userinfo_to_string(userinfo);//���ṹ
		
			state = 1;
			int Ifsuccess = 0;

			clock_t start, finish;
			start = clock();
			
			while (1)
			{
				if (!state)
				{
					Ifsuccess = 1;
					break;
				}
				else
				{
					finish = clock();
					if (finish - start > 100000)//��ʱ
						break;
					Sleep(1000);
				}	
			}
			

			if (Ifsuccess)//��Ҫraft�ķ���ֵ
			{
				answer::RegisterAnswer reg_ans(1);//��ͻ��˷���Answer
				answer::Answer *r = &reg_ans;
				if (r->hasAttribute(answer::Attribute::RegisterState))
					com.sendAnswer(*r, req2->getAddress);
			}

			else
			{
				answer::RegisterAnswer reg_ans(0);
				answer::Answer *r = &reg_ans;
				if (r->hasAttribute(answer::Attribute::RegisterState))
					com.sendAnswer(*r, req2->getAddress);//ע��ʧ��
			}
		}

		else
		{
			answer::RegisterAnswer reg_ans(2);
			answer::Answer *r = &reg_ans;
			if (r->hasAttribute(answer::Attribute::RegisterState))
				com.sendAnswer(*r, req2->getAddress);
		}//�û����Ѵ���
	}

	break;


	case request::Kind::SignIn:
	{
		string userid = req2->getUser();
		string userpassword = req2->getPassword();
		if (client.Check_id(userid))  //�����û����Ƿ����
		{
			if (client.Log_in(userid, userpassword)) //���������Ƿ���ȷ
			{
				answer::LogInAnswer log_ans(1);
				answer::Answer *r = &log_ans;
				if (r->hasAttribute(answer::Attribute::LogInState))
					com.sendAnswer(*r, req2->getAddress);//��½�ɹ�
			}
			else
			{
				answer::LogInAnswer log_ans(0);
				answer::Answer *r = &log_ans;
				if (r->hasAttribute(answer::Attribute::LogInState))
					com.sendAnswer(*r, req2->getAddress);//�������
			}
		}
		else
		{
			answer::LogInAnswer log_ans(2);
			answer::Answer *r = &log_ans;
			if (r->hasAttribute(answer::Attribute::LogInState))
				com.sendAnswer(*r, req2->getAddress);//�û���������
		}
	}
	break;


	case request::Kind::Deal:
	{
		Deal deal;
		deal.payer = req2->getUser();
		deal.payee = req2->getAccountCollector();
		deal.change = req2->getAccount();
		deal.time = req2->getTransactionDate();

		if (client.Check_id(deal.payer) && client.Check_id(deal.payee))  //�����û����Ƿ����
		{
			if (client.Check_fund(deal.payer, deal.change)) //��������Ƿ����
			{
				raft_forward=Deal_to_string(deal);//���׼�¼����raft

				state = 1;
				int Ifsuccess = 0;

				clock_t start, finish;
				start = clock();

				while (1)
				{
					if (!state)
					{
						Ifsuccess = 1;
						break;
					}
					else
					{
						finish = clock();
						if (finish - start > 100000)//��ʱ
							break;
						Sleep(1000);
					}
				}

				if (Ifsuccess)
				{
					answer::DealAnswer dea_ans(1);
					answer::Answer *r = &dea_ans;
					if (r->hasAttribute(answer::Attribute::DealState))
						com.sendAnswer(*r, req2->getAddress);
				}//���׳ɹ�
				else
				{
					answer::DealAnswer dea_ans(0);
					answer::Answer *r = &dea_ans;
					if (r->hasAttribute(answer::Attribute::DealState))
						com.sendAnswer(*r, req2->getAddress);
				}//����ʧ��
			}
			else
			{
				answer::DealAnswer dea_ans(3);
				answer::Answer *r = &dea_ans;
				if (r->hasAttribute(answer::Attribute::DealState))
					com.sendAnswer(*r, req2->getAddress);
			}//����
		}
		else
		{
			answer::DealAnswer dea_ans(2);
			answer::Answer *r = &dea_ans;
			if (r->hasAttribute(answer::Attribute::DealState))
				com.sendAnswer(*r, req2->getAddress);
		}//�û���������
	}

	break;


	case request::Kind::BalanceCheck:
	{
		string userid = req2->getUser();
		if (client.Check_id(userid))  //�����û����Ƿ����
		{
			double balance = client.Get_money(userid);
			answer::BalanceAnswer bal_ans(balance);
			answer::Answer *r = &bal_ans;
			if (r->hasAttribute(answer::Attribute::Amount))
				com.sendAnswer(*r, req2->getAddress);
		}
		else
		{
			answer::BalanceAnswer bal_ans(-1);
			answer::Answer *r = &bal_ans;
			if (r->hasAttribute(answer::Attribute::Amount))
				com.sendAnswer(*r, req2->getAddress);
		}
	}
	break;


	case request::Kind::RecordCheck:
	{
		string userid = req2->getUser();
		if (client.Check_id(userid))  //�����û����Ƿ����
		{
			std::string record = client.Get_record(userid, 0, 10);
			answer::RecordAnswer rec_ans(record);
			answer::Answer *r = &rec_ans;
			if (r->hasAttribute(answer::Attribute::TradeRecord))
				com.sendAnswer(*r, req2->getAddress);
		}
		else
		{
			answer::RecordAnswer rec_ans("");
			answer::Answer *r = &rec_ans;
			if (r->hasAttribute(answer::Attribute::TradeRecord))
				com.sendAnswer(*r, req2->getAddress);
		}
	}
	break;
	}
}

int main()
{
	//check();
    return 0;
}
