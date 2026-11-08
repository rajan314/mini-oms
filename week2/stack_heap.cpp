#include<iostream>
struct Order{
  int id;
  int qty;
};

void stackExample(){
  Order o{1,100};          // Lives in stack
  std::cout<<"stack order "<< o.id<<'\n';
}    // o is destroyed here automatically

void heapExample(){
  Order* p = new Order{2,200}; // Lives in heap; p points to it 
  std:: cout<<"heap order "<<p->id<<'\n';
  delete p;                // You must free it
   
}

int main(){
  stackExample();
  heapExample();
  return 0;
}

