#include <thread>

void initf(int a) {
	int xxx =a ;
}


int main(int ac, char** av) {
	
	std::once_flag of ;
	
	std::call_once(of, initf, 10) ;
}

