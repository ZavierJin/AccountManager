#include <iostream>
#include "Computer.h"
#include "Raft.h"


auto& com = computer::Computer::instance();
//computer::Computer com;
void raft_test();


int main()
{
	raft_test();
//	try {
//		raft_test();
//#ifdef DEBUG
//		f_out << "Why go out of the function??" << std::endl;
//		while (true)	continue;
//#endif // DEBUG
//	}
//	catch (const boost::filesystem::filesystem_error& e) {
//#ifdef DEBUG
//		f_out << e.what() << std::endl;
//		while (true)	continue;
//#endif // DEBUG
//	}
//	catch (const std::exception& e){
//		f_out << e.what() << std::endl;
//#ifdef DEBUG
//		f_out << " function??" << std::endl;
//		while (true)	continue;
//#endif // DEBUG
//	}
//	catch (...) {
//#ifdef DEBUG
//		f_out << " else ??" << std::endl;
//		while (true)	continue;
//#endif // DEBUG
//	}

	return 0;
}