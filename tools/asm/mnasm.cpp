#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <cstdlib>
int parseReg(const std::string &s){
  int r = 0;
  if(s[0] == 'R'){
    r= std::stoi(s.substr(1));
  }else {
    std::cerr <<"R is missing for address of register"<<'\n';
    exit(1);
  }
  if(r>=0 && r <=15){
    return r;
  }else {
    std::cerr<<"Register R"<<r<< " is not supported"<<'\n';
    exit(1);
  }
  

}
int parseNum(const std::string &s){
  int v;

  v = std::stoi(s, nullptr, 0);
  if(v>=0 && v <=255){
    return v;
  }
  else{
    std::cerr <<"value is not supported "<<s<<'\n';
    exit(1);
    }
}

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
  std::vector<std::vector<std::string>> program;
  while (getline(in, line)){
    size_t p = line.find(';');
    if(p != std::string::npos){
      line.erase(p);
    }
    for (char &c : line){
      if(c == ',' || c == '[' || c == ']'){
        c = ' ';
      }
    }
    std::stringstream ss(line);
    std::vector<std::string> tokens;

    std::string slovo;
    
    while(ss>>slovo){
      tokens.push_back(slovo);
    }
    if(tokens.empty()) continue;

    program.push_back(tokens);
  }
  std::map<std::string, int> labels;
  int address = 0;
  for(const auto &riadok: program){
    if(riadok[0].back() == ':'){
      std::string meno = riadok[0].substr(0, riadok[0].size()-1);
      labels[meno] = address;
      
    }
    else{
      address++;
    }
    

  }
  for(const auto &[meno, a]: labels){
    std::cout << meno << " " <<  a <<'\n';
  }

  std::cout << program.size()<<'\n';

}
