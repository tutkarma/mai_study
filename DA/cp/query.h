#include <stack>
#include <algorithm>
#include <sstream>
#include "index.h"

#define AND L'&'
#define OR L'|'
#define NOT L'~'
#define LEFT_BRACKET L'('
#define RIGHT_BRACKET L')'

class TQuery
{
public:
    void FullOutput() { fullOutput = true; };
    void GetIndex(std::string &inputName);
    void ParseQueries(std::string &inputFile, std::string &outputFile);

private:
    const std::vector<uint32_t> &GetVecOfNumArticles(std::wstring &word, const std::vector<uint32_t> &empty);
    uint32_t Priority(wchar_t op);
    bool isOperation(wchar_t op);
    void ProcessingQuery(std::wstring &query);
    void ExecuteOperation(wchar_t op);

    void Negation();
    void Union();
    void Intersection();

    TIndex index;
    std::stack<std::shared_ptr<std::vector<uint32_t>>> operands;
    std::stack<wchar_t> operations;
    bool fullOutput = false;
};

