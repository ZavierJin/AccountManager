#include <iostream>
#include "Computer.h"
#include "Raft.h"
#include"client.h"
#include"check.h"

void raft_test();
//auto& com = computer::Computer::instance();
void log_test() {
	Logs testlog;
	testlog.addAction("jiangpin 123 100 ", 0);
	testlog.addAction("jiangpin 123 100 ", 1);
	testlog.addAction("jiangpin 123 100 ", 2);
	testlog.addAction("jiangpin 123 100 ", 2);
	std::vector<LogType> templog;
	testlog.getLog(0, 4, templog);
	std::vector<LogType>::iterator i;
	for (i = templog.begin(); i != templog.end(); i++) {
		std::cout << *i << std::endl;
		//client.Write_account(*i);
	}
}

void write_test() {
	using namespace std;
	if (client.Write_account("jiangpin1 123 1000 |2"))
		cout << "successful" << endl;
	else
		cout << "fail" << endl;

	if(client.Write_account("jiangpin2 123 1000 |2"))
		cout << "successful" << endl;
	else
		cout << "fail" << endl;
	
	for (int i = 0; i < 10; i++) {
		if (client.Write_account("jiangpin2 jiangpin1 10 1 |2"))
			cout << "successful" << endl;
		else
			cout << "fail" << endl;
	}

}

int main()
{	
	
	//log_test();
	
	//write_test();

	//client.Write_account("jiangpin2 jiangpin 100 18 |2");

	auto& com = computer::Computer::instance();
	Global_nodeId = com.registerServer();// get nodeId

	std::thread raft_thread(raft_test);
	std::thread check_thread(check_test);
	//std::thread check_thread(check);
	raft_thread.join();
	check_thread.join();


	
	//raft_test();	// thread raft


//
//#ifdef RAFT_DEBUG
//	std::string f;
//	int random_file_name;
//
//	srand((int)time(0));
//	random_file_name = static_cast<IndexType>(rand() % 300);
//	f = "Debug_" + std::to_string(random_file_name);
//	std::ofstream f_out(f);
//
//	try {
//		raft_test();
//		f_out << "Why go out of the function??" << std::endl;
//		while (true)	continue;
//	}
//	catch (const boost::filesystem::filesystem_error& e) {
//		f_out << e.what() << std::endl;
//		while (true)	continue;
//	}
//	catch (const std::exception& e){
//		f_out << e.what() << std::endl;
//		f_out << " function??" << std::endl;
//		while (true)	continue;
//	}
//	catch (...) {
//		f_out << " else ??" << std::endl;
//		while (true)	continue;
//	}
//#endif // RAFT_DEBUG
	return 0;
}