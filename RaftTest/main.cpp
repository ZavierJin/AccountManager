#include <iostream>
#include "Computer.h"
#include "Raft.h"


auto& com = computer::Computer::instance();
//computer::Computer com;
void raft_test();


int main()
{
	//raft_test();
#ifdef RAFT_DEBUG
	std::string f;
	int random_file_name;

	srand((int)time(0));
	random_file_name = static_cast<IndexType>(rand() % 300);
	f = "Debug_" + std::to_string(random_file_name);
	std::ofstream f_out(f);

	try {
		raft_test();
		f_out << "Why go out of the function??" << std::endl;
		while (true)	continue;
	}
	catch (const boost::filesystem::filesystem_error& e) {
		f_out << e.what() << std::endl;
		while (true)	continue;
	}
	catch (const std::exception& e){
		f_out << e.what() << std::endl;
		f_out << " function??" << std::endl;
		while (true)	continue;
	}
	catch (...) {
		f_out << " else ??" << std::endl;
		while (true)	continue;
	}
#endif // RAFT_DEBUG
	return 0;
}