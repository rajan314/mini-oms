#include<iostream>

struct Order{
  int id;
  int qty;
};

class OrderOwner{
  public:
     OrderOwner(int id, int qty): order_(new Order{id,qty}){
      std::cout<< "created order "<< order_->id<<'\n'; 
     }

     ~OrderOwner(){
      std::cout<< "freezing order memory\n";
      delete order_;
     }

     int qty() const {
      return order_->qty;
     }
  private:
      Order* order_;   

};

int main(){
  OrderOwner a(1,100);
  std::cout<< "qty = " << a.qty()<< '\n';
  return 0;
}