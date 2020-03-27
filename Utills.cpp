#include "Utills.h"
#include <iostream>
#include <vector>
#include <string>

std::vector<std::string> Utills::splitBy(std::string str, const std::string& delimiter){
    std::vector<std::string> extracted;
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        extracted.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    extracted.push_back(str);
    return extracted;
}

void Utills::printStringVector(const std::vector<std::string>& vec){
    std::cout<<"["<<std::endl;
    for(int i=0; i<vec.size(); i++){
        std::cout<<"  ["<<vec[i]<<"],\n";
    }
    std::cout<<"]"<<std::endl;
}

void Utills::printPairStringVector( const std::vector<std::pair<std::string, std::string> >& vec){
    std::cout<<"["<<std::endl;
    for(auto item : vec){
        std::cout<<"  [" << item.first << ":" << item.second << "],\n";
    }
    std::cout<<"]"<<std::endl;
}

void Utills::printPairIdStringVector( const std::vector<std::pair<int, std::string> >& vec){
    std::cout<<"["<<std::endl;
    for(auto item : vec){
        std::cout<<"  [" << item.first << ":" << item.second << "],\n";
    }
    std::cout<<"]"<<std::endl;
}

bool Utills::asortpair(const std::pair<int,std::string> &a, const std::pair<int,std::string> &b){
    bool res;
    try{
        res = (std::stof(a.second) < std::stof(b.second));
    }    
    catch(...){
      res = (a.second < b.second);
    }
    return res;
} 

bool Utills::dsortpair(const std::pair<int,std::string> &a, const std::pair<int,std::string> &b){
    bool res;
    try{
        res = (std::stof(a.second) > std::stof(b.second));
    }    
    catch(...){
      res = (a.second > b.second);
    }
    return res;
} 

void Utills::removeSpace(std::string& str){
    size_t i = 0, j;
    if(str[i] == ' '){
        while(str[i] == ' ') i++;
        str.erase(0, i);   
    }
    j = str.size() - 1;
    i = 0;
    if(str[j] == ' '){
        while(str[j] == ' '){j--; i++;}
        str.erase(j+1, i);
    }
}