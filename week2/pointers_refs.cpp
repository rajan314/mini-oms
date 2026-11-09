#include<iostream>

struct Order{
  int id;
  int qty;
};

void reduceByValue(Order o){    // gets a COPY
  o.qty -= 10;
}

void reduceByRef(Order& o){     // gets the ORIGINAL
  o.qty -= 10;
}

void reduceByPtr(Order* o){   // gets the address of the original
    if(o==nullptr){
      return;
    }
    o->qty -= 10;
}

void printOrder(const Order& o){  //read-only, no copy
   std::cout<<"order "<< o.id <<" qty "<< o.qty << '\n';
}

int main(){
  Order order{1,100};

  reduceByValue(order);
  printOrder(order);

  reduceByRef(order);
  printOrder(order);

  reduceByPtr(&order);
  printOrder(order);

  reduceByPtr(nullptr);
  return 0;
}
