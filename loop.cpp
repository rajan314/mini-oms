#include<iostream>
#include<vector>

int main(){
  const std::vector<int> quantities={10,25,5,40};
  int total=0;

  for(std::size_t i=0;i<quantities.size();i++){
    total+=quantities[i];
  }

  std::cout<<"Total quantity: "<< total <<'\n';
  return 0;

}