#include "index.h"

void TIndex::Build(std::string& inputFile)
{
    std::wifstream input((inputFile).c_str());
    for (size_t num = 1; input; ++num) {
        if (GetNameArticle(input)) {
            GetIndexArticle(num, input);
        }
    }
    cntArticles = names.size();
    input.close();
}

bool TIndex::GetNameArticle(std::wifstream& input)
{
    std::wstring line;
    if (std::getline(input, line)) {
        uint32_t start = line.find(L"title=");
        if (start == std::string::npos) {
            return false;
        }
        start += 7; // 7 -- len "title="
        uint32_t lenName = line.length() - start - 2; // 2 -- two last symbols in str
        names.push_back(line.substr(start, lenName));
        return true;
    } else {
        return false;
    }
}

void TIndex::GetIndexArticle(uint32_t num, std::wifstream &input)
{
    std::wstring line, word;
    std::unordered_set<std::wstring> words;
    while (std::getline(input, line)) {
        if (line == L"</doc>") {
            break;
        }

        for (const auto &c : line) {
            if (isalnum(c, std::locale())) {
                word += towlower(c);
            } else if (word.length()) {
                words.insert(word);
                word.clear();
            }
        }

        if (word.length()) {
            words.insert(word);
            word.clear();
        }
    }

    for (const auto &it : words) {
        std::pair<std::unordered_map<std::wstring, std::vector<uint32_t> >::iterator, bool> rec = index.insert(std::make_pair(it, std::vector<uint32_t>()));
        rec.first->second.push_back(num);
    }
}


void TIndex::SaveAll(std::string &outputFile)
{
    SaveNames(outputFile);
    SaveIndex(outputFile);
}

void TIndex::SaveNames(std::string &outputFile)
{
    std::wofstream namesOut((outputFile + ".name").c_str());
    if (namesOut) {
        namesOut << names.size() << L"\n";
        for (const auto &name : names) {
            namesOut << name << L"\n";
        }
    }
    namesOut.close();
}

void TIndex::SaveIndex(std::string &outputFile)
{
    std::wofstream indexOut((outputFile).c_str());
    if (indexOut) {
        indexOut << cntArticles << L"\n";
        for (const auto &it: index) {
            indexOut << it.first << L" ";
            indexOut << it.second.size() << L" ";
            for (uint32_t i = 0; i < it.second.size(); ++i) {
                indexOut << it.second[i] - 1 << L" ";
            }
            indexOut << L"\n";
        }
    }
    indexOut.close();
}

void TIndex::LoadAll(std::string &inputFile)
{
    LoadNames(inputFile);
    LoadIndex(inputFile);
}

void TIndex::LoadNames(std::string &inputFile)
{
    std::wifstream namesIn((inputFile + ".name").c_str());
    std::wstring name, size;
    std::getline(namesIn, size);
    names.reserve(std::stoul(size));
    while (std::getline(namesIn, name)) {
        names.push_back(name);
        name.clear();
    }
    namesIn.close();
}

void TIndex::LoadIndex(std::string &inputFile)
{
    std::wifstream indexIn((inputFile).c_str());
    uint32_t size;
    std::wstring word;
    indexIn >> cntArticles;
    while (!indexIn.eof()) {
        std::vector<uint32_t> tmp;
        indexIn >> word;
        indexIn >> size;

        uint32_t num;
        tmp.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            indexIn >> num;
            tmp.push_back(num);
        }

        index.insert(std::make_pair(word, tmp));
        word.clear();
    }
    indexIn.close();
}
