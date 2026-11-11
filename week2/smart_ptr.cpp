#include<iostream>
#include<memory>

struct Order {
  int id;
  int qty;

   ~Order(){
    std::cout<<"order"<< id <<" destroyed\n";
   }
};

void showOrder(const Order& o){
  std::cout<< "order "<<o.id<<" qty "<<o.qty<< '\n';
}

int main(){
  auto a =std::make_unique<Order>(1,100);
  showOrder(*a);
  std::cout<<" qty via arrow: "<<a->qty <<'\n';
  return 0;
}


