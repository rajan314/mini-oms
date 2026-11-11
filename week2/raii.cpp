#include<iostream>
#include<string>

class Tracer{
  public:
     explicit Tracer(const std:: string& name) : name_(name){
      std::cout<< "[open]" << name_ <<'\n';
     }
     ~Tracer(){
      std::cout<<" [close] "<< name_ <<'\n';
     }
  private:
     std::string name_ ;

};

void process(bool failEarly){
  Tracer session("order-session");

  if(failEarly){
    std::cout<< "Validation failed , returning early\n";
    return ;
  }
  Tracer risk("risk-check");
  std::cout<< "Processing order\n";
}

int main(){
  process(false);
  std::cout<<"---\n";
  process(true);
  return 0;
}