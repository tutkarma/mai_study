#include <iostream>
#include <locale>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <unordered_set>
#include <memory>

class TIndex
{
public:
    void Build(std::string &inputFile);
    void SaveAll(std::string &outputFile);
    void SaveNames(std::string &outputFile);
    void SaveIndex(std::string &outputFile);
    void LoadAll(std::string &inputFile);
    void LoadNames(std::string &inputFile);
    void LoadIndex(std::string &inputFile);

    friend class TQuery;

private:
    bool GetNameArticle(std::wifstream &input);
    void GetIndexArticle(uint32_t num, std::wifstream &input);

    uint32_t cntArticles;
    std::vector<std::wstring> names;
    std::unordered_map<std::wstring, std::vector<uint32_t> > index;
};
