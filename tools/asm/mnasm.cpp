#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]){
  if(argc<2){
    std::cout << "pouzitie: mnasm subor.asm\n";
    return 1;
  }

  std::ifstream in(argv[1]);
  if(!in){
    std::cout<<"neviem otvorit "<< argv[1] << "\n";
    return 1;
  }
  std::string line;
  while (getline(in, line)){
    std::cout << " [" << line << "]\n"; 
  }

}
