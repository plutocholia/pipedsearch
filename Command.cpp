#include "Command.h"
#include "Utills.h"
#include <string>
#include <iostream>

Command::Command(const std::string& user_input){
    this->user_input = user_input;
}

void Command::parseUserInput(){
   std::vector<std::string> vec_tokens = Utills::splitBy(this->user_input, "-");
   for(auto& item : vec_tokens){
       Utills::removeSpace(item);
       std::vector<std::string> vec_pair = Utills::splitBy(item, "=");
       for(auto& item2: vec_pair) Utills::removeSpace(item2);
       this->filters.push_back(std::make_pair(vec_pair[0], vec_pair[1]));
   }

   this->folder_dir = DIR_DATASET + this->filters[this->filters.size() - 1].second;
   this->filters.pop_back();
   this->number_of_2bproc = std::atoi(this->filters[this->filters.size() - 1].second.c_str());
   this->filters.pop_back();

   int start_sort_index = -1, i = 0;
   for(auto item : this->filters){
       if(item.second == DES || item.second == ASC){
           if(start_sort_index == -1)
               start_sort_index = i;
           sorts.push_back(item);
       }
       i += 1;
   }
   if(start_sort_index != -1)
   this->filters.erase(this->filters.begin() + start_sort_index, this->filters.end());

    db Utills::printPairStringVector(this->filters);
    db Utills::printPairStringVector(this->sorts);
    db std::cout<<"Number of Processes:" << this->number_of_2bproc << std::endl;
    db std::cout<<"Folder Direction:" << this->folder_dir << std::endl;
   
}

std::string Command::getFolerDir(){
    return this->folder_dir;
}

size_t Command::getNofProcesses(){
    return this->number_of_2bproc;
}

std::vector<std::pair<std::string, std::string> > Command::getFilters(){
    return this->filters;
}

std::vector<std::pair<std::string, std::string> > Command::getSorts(){
    return this->sorts;
}

Command::~Command(){

}
