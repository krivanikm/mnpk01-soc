#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <cstdlib>
#include <iomanip>

const std::map<std::string, int> aluOps = {
  {"ADD", 0},  {"SUB", 1},  {"INC", 2},  {"DEC", 3},
  {"AND", 4},  {"OR", 5},   {"XOR", 6},  {"NOT", 7},
  {"SHL", 8},  {"SHR", 9},  {"ROL", 10}, {"CMP", 11},
  {"PASS", 12}, {"CLR", 13}
};


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
int parseImm(const std::string &s, const std::map<std::string, int> &label){
  if(s.front() == '{' && s.back() == '}'){
    std::string meno = s.substr(1, s.size() -2);
    if(label.count(meno) == 0){
      std::cerr <<"unknown label: "<<meno<<'\n';
      exit(1);
    }
    return label.at(meno);

  }
  if(aluOps.count(s)){
    return aluOps.at(s);
  }
  return parseNum(s);

}

const std::map<std::string, int> jumpConds = {
  {"JMP", 0}, {"JZ", 1}, {"JNZ", 2}, {"JC", 3},
  {"JNC", 4}, {"JN", 5}, {"JNN", 6}
};

void needOperands(const std::vector<std::string> &t, size_t n){
  if(t.size() != n + 1){
    std::cerr << t[0] << " needs " << n << " operands\n";
    exit(1);
  }
}

int encode (const std::vector<std::string> &t, const std::map<std::string, int> &label){
  const std::string &op = t[0];
  if(op == "NOP"){
    needOperands(t, 0);
    return 0x0 << 12;
  }
  if(op == "HLT"){
    needOperands(t, 0);
    return 0x8 << 12;
  }
  if(op == "MVI"){
    needOperands(t, 2);
    return (0x1 << 12) | (parseReg(t[1]) << 8) | parseImm(t[2], label);
  }
  if(op == "MVIB"){
    needOperands(t, 2);
    return (0x3 << 12) | (parseReg(t[1]) << 8) | parseImm(t[2], label);
  }
  if(op == "MOV"){
    needOperands(t, 2);
    return (0x2 << 12) | (parseReg(t[1]) << 8) | (parseReg(t[2]) << 4);
  }
  if(op == "LOAD"){
    needOperands(t, 2);
    return (0x4 << 12) | (parseReg(t[1]) << 8) | (parseReg(t[2]) << 4);
  }
  if(op == "STORE"){
    needOperands(t, 2);
    return (0x5 << 12) | (parseReg(t[1]) << 8) | (parseReg(t[2]) << 4);
  }
  if(op == "ALU"){
    needOperands(t, 3);
    return (0x7 << 12) | (parseReg(t[1]) << 8) | (parseReg(t[2]) << 4) | parseReg(t[3]);
  }
  if(jumpConds.count(op)){
    needOperands(t, 1);
    return (0x6 << 12) | (jumpConds.at(op) << 8) | (parseReg(t[1]) << 4);
  }
  std::cerr << "unknown instruction: " << op << '\n';
  exit(1);
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
  for(const auto &riadok: program){
    if(riadok[0].back() == ':') continue;
    std::cout << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << encode(riadok, labels) << '\n';
  }

}
