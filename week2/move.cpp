#include<iostream>
#include<memory>
#include<string>
#include<utility>

struct Order{
  int id;
  int qty;

  ~Order(){
    std::cout<< "order "<< id <<" destroyed\n";
  }
};

void takeOwnership(std::unique_ptr<Order> o){
  std::cout<< "function owns order " << o->id << '\n';
}

int main(){
  auto a = std::make_unique<Order>(1,100);

  auto b= std::move(a);
  std::cout<< "a is " <<(a ? "full" :"empty")<<'\n';
  std::cout<<"b is " << (b ? "full" : "empty")<<'\n';

  takeOwnership(std:: move(b));
  std::cout<<"back in main , b is "<<(b?"full" : "empty")<<'\n';
  return 0;
}

