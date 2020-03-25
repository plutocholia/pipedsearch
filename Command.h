#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>
#include <map>

class Command
{
private:
    std::string user_input;
    std::vector<std::pair<std::string, std::string> > filters;
    std::vector<std::pair<std::string, std::string> > sorts;
    size_t number_of_2bproc;
    std::string folder_dir;
public:
    Command(const std::string&);
    void parseUserInput();
    std::string getFolerDir();
    size_t getNofProcesses();
    std::vector<std::pair<std::string, std::string> > getFilters();
    std::vector<std::pair<std::string, std::string> > getSorts();
    ~Command();
};



#endif // !COMMAND_H
