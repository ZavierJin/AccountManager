#include <iostream>
#include "Computer.h"
#include "Raft.h"


auto& com = computer::Computer::instance();
//computer::Computer com;
void raft_test();


int main()
{
	raft_test();
//	std::string f;
//	std::cin >> f;
//	std::ofstream f_out(f);
//	try {
//		raft_test();
//#ifdef RAFT_DEBUG
//		f_out << "Why go out of the function??" << std::endl;
//		while (true)	continue;
//#endif // RAFT_DEBUG
//	}
//	catch (const boost::filesystem::filesystem_error& e) {
//#ifdef RAFT_DEBUG
//		f_out << e.what() << std::endl;
//		while (true)	continue;
//#endif // RAFT_DEBUG
//	}
//	catch (const std::exception& e){
//		f_out << e.what() << std::endl;
//#ifdef RAFT_DEBUG
//		f_out << " function??" << std::endl;
//		while (true)	continue;
//#endif // RAFT_DEBUG
//	}
//	catch (...) {
//#ifdef RAFT_DEBUG
//		f_out << " else ??" << std::endl;
//		while (true)	continue;
//#endif // RAFT_DEBUG
//	}

	return 0;
}