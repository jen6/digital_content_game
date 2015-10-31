#include <iostream>
#include <thread>
#include "client.h"
#include "login.h"

//#define DOLOGINTEST

int main()
{
	//1 �α��� �ϱ�
	std::string str = Login::DoLogin(L"googler", L"googleisbest123");
	std::cout << str << std::endl;


	//2 io_service ����
	boost::asio::io_service io;
	boost::asio::io_service::work work(io);		//io_service�� ������� �ʵ��� ������
	tcp::resolver resolver(io);
	auto endpoint_iterator = resolver.resolve({ "127.0.0.1","1000" });
	std::thread t1(boost::bind(&boost::asio::io_service::run, &io));

	//3 client��ü ����� �ֱ�
	client cli(io, endpoint_iterator);

	//4 �Ʊ� �α��� �ϰ� ���� ���� �Ѱ��ֱ�
	cli.sendSession(Utils::S2Ws(str));

	//5 �����̿�
	while (1)
	{
		float x, y;
		std::cin >> x >> y;
		cli.move(x, y);
	}
	t1.join();

}