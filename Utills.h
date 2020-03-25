#ifndef UTILLS_H
#define UTILLS_H

#define DES "descending"
#define ASC "ascending"
#define DEBUG true
#define db if(DEBUG)

#include <vector>
#include <string>


class Utills
{
public:
    static void removeSpace(std::string&);
    static std::vector<std::string> splitBy(
        std::string, const std::string&
    );
    static void printStringVector(
        const std::vector<std::string>& vec
    );
    static void printPairStringVector(
        const std::vector<std::pair<std::string, std::string> >&
    );
};


#endif // !UTILLS_H