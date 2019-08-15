#include<iostream>
#include <emscripten/bind.h>
#include <stdio.h>
#include <stdlib.h>

using namespace emscripten;

int main(){
   std::cout << "Hello Word" << std::endl;
   return 0;
}

std::string hello(){
   std::string message = "Hello, welcome to Sad Calculator 0.1!";
   return message;
}

int doSub(int acc, std::string x);

int doAdd(int acc, std::string x){
    int left;
    bool m = false;
    int result;
    size_t loc;
    size_t plus = x.find("+");
    size_t minus = x.find("-");
    
    if(plus<minus){
      loc = plus;
    }else{
      loc = minus;
      m=true;
    }

     if(loc!=std::string::npos){
      std::string first(x.substr(0, loc));
      std::string right(x.substr(loc + 1, x.length()));
       left = acc + std::stoi(first);
      if(!m){
       result = doAdd(left, right);
      }else{
        result = doSub(left, right);
      }
       return result;
    }else{      
       return acc + std::stoi(x);;
    }
}

int doSub(int acc, std::string x){
  int left;
    bool m = false;
    int result;
    size_t loc;
    size_t plus = x.find("+");
    size_t minus = x.find("-");
    
    if(plus<minus){
      loc = plus;
    }else{
      loc = minus;
      m=true;
    }

        if(loc!=std::string::npos){
      std::string first(x.substr(0, loc));
      std::string right(x.substr(loc + 1, x.length()));
       left = acc - std::stoi(first);
      if(!m){
        result = doAdd(left, right);
      }else{
        result = doSub(left, right);
      }
       
       return result;
    }else{
       
       return acc - std::stoi(x);;
    }

}

std::string evaluate(std::string x){
  try{
     int result = doAdd(0, x);    
     return std::to_string(result);
  }catch(std::exception &err)
  {
    std::string errorMsg = "-ERROR-";
    return errorMsg;
  }
    
}

EMSCRIPTEN_BINDINGS(my_module)
{
  function("hello", &hello);
  function("evaluate", &evaluate);
}

