#include <iostream>
#include <thread>
#include "client_sock.h"
#include "login.h"

//#define DOLOGINTEST

int main()
{
	//1 �α��� �ϱ�
	std::string str = Login::DoLogin(L"admin", L"admin");
	std::cout << str << std::endl;


	//2 io_service ����
	boost::asio::io_service io;
	boost::asio::io_service::work work(io);		//io_service�� ������� �ʵ��� ������
	tcp::resolver resolver(io);
	auto endpoint_iterator = resolver.resolve({ "127.0.0.1","1000" });
	std::thread t1(boost::bind(&boost::asio::io_service::run, &io));

	//3 client��ü ����� �ֱ�
	ClientSock cli(io, endpoint_iterator);

	//4 �Ʊ� �α��� �ϰ� ���� ���� �Ѱ��ֱ�
	cli.sendSession(Utils::S2Ws(str));

	//5 �����̿�

		Packet::EnterMapBody lb;
		lb.hp = 100; lb.att = false; lb.name = "fuck";
		lb.nowhp = 10; lb.rotate = 0; lb.def = 0;
		lb.x = 0; lb.y = 0;
		//lb.Make_packet();
		cli.send(lb.Make_packet());
	t1.join();

}