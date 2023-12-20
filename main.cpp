#include "DoubleLinkedList.hpp"

int main(void) {

DoubleLinkedList<int> list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);

  DoubleLinkedList<int> list2(list1);
  std::cout << list2.front() << std::endl;

   list1.pop_front();
  list1.push_front(0);

  std::cout << list1.front() << std::endl;
 std::cout << list2.front() << std::endl;

return 0;
}
