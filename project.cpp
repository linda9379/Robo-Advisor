#include <iostream>
#include "History_Transaction_definitions.hpp"
int main();
unsigned int Transaction::assigned_trans_id = 0;

int main (){
//	Transaction trans1 {"a",1,1,2019,true,10,3};
//	Transaction trans2 {"b", 2,1,2019,false, 10, 8};
//	std::cout << (trans1 < trans2)<<std::endl;
	 History trans_history{};
	 trans_history.read_history();
	 std::cout << "[Starting history]:" << std::endl;
	 trans_history.print();
	trans_history.sort_by_date();
	 std::cout << "[Sorted ]:" << std::endl;
	 trans_history.print();
	 trans_history.update_acb_cgl();
	 trans_history.print();
	 std::cout << "[CGL for 2018 ]: " << trans_history.compute_cgl(2018) << std::endl;
	 std::cout << "[CGL for 2019 ]: " << trans_history.compute_cgl(2019) << std::endl;

	return 0;
}
