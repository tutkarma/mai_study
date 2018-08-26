#include "query.h"

void TQuery::GetIndex(std::string &inputFile)
{
    fullOutput ? index.LoadAll(inputFile) : index.LoadIndex(inputFile);
}

void TQuery::ParseQueries(std::string &inputFile, std::string &outputFile)
{
    std::wstring query;
    std::wifstream input(inputFile.c_str());
    std::wofstream output(outputFile.c_str());
    while (std::getline(input, query)) {
        if (!query.length()) {
            break;
        }
        ProcessingQuery(query);
        std::shared_ptr<std::vector<uint32_t> > res = operands.top();
        operands.pop();
        output << (*res).size() << L"\n";

        if (fullOutput) {
            for (const auto &i : (*res)) {
                output << index.names[i] << L"\n";
            }
        }
    }
    input.close();
    output.close();
}

void TQuery::ProcessingQuery(std::wstring &query)
{
    static const std::vector<uint32_t> empty(0);
    wchar_t c;
    std::wstring word;
    std::wstringstream querySS(query);
    while (querySS >> c) {
        if ((isOperation(c) || c == LEFT_BRACKET || c == RIGHT_BRACKET) && word.length()) {
            std::shared_ptr<std::vector<uint32_t> > vec = std::make_shared<std::vector<uint32_t> >(GetVecOfNumArticles(word, empty));
            operands.push(vec);
            word.clear();
        }

        if (c == LEFT_BRACKET) {
            operations.push(LEFT_BRACKET);
        } else if (c == RIGHT_BRACKET) {
            while (operations.top() != LEFT_BRACKET) {
                ExecuteOperation(operations.top());
                operations.pop();
            }
            operations.pop();
        } else if (isOperation(c)) {
            while (!operations.empty() && (
                   ((c != NOT) && (Priority(operations.top()) >= Priority(c))) || 
                   ((c == NOT) && (Priority(operations.top()) > Priority(c)))))
            {
                ExecuteOperation(operations.top());
                operations.pop();
            }
            operations.push(c);
        } else {
            word += towlower(c);
        }
    }

    if (word.length()) {
        std::shared_ptr<std::vector<uint32_t> > vec = std::make_shared<std::vector<uint32_t> >(GetVecOfNumArticles(word, empty));
        operands.push(vec);
    }


    while (!operations.empty()) {
        wchar_t op = operations.top();
        ExecuteOperation(op);
        operations.pop();
    }
}

const std::vector<uint32_t> &TQuery::GetVecOfNumArticles(std::wstring &word, const std::vector<uint32_t> &empty)
{
    std::unordered_map<std::wstring, std::vector<uint32_t> >::iterator entry = index.index.find(word);
    if (entry != index.index.end()) {
        return entry->second;
    } else {
        return empty;
    }
}

bool TQuery::isOperation(wchar_t op)
{
    return (op == AND) || (op == OR) || (op == NOT);
}

uint32_t TQuery::Priority(wchar_t op)
{
    switch (op) {
        case OR:
            return 1;
        case AND:
            return 2;
        case NOT:
            return 3;
        default:
            return 0;
    }
}

void TQuery::ExecuteOperation(wchar_t op)
{
    switch (op) {
        case OR:
            Union();
            break;
        case AND:
            Intersection();
            break;
        case NOT:
            Negation();
            break;
        default:
            break;
    }
}

void TQuery::Union()
{
    std::vector<uint32_t> res;
    std::shared_ptr<std::vector<uint32_t> > op1 = operands.top();
    operands.pop();
    std::shared_ptr<std::vector<uint32_t> > op2 = operands.top();
    operands.pop();
    std::set_union((*op1).begin(), (*op1).end(), (*op2).begin(), (*op2).end(), std::back_inserter(res));
    std::shared_ptr<std::vector<uint32_t> > tmp = std::make_shared<std::vector<uint32_t> >(res);
    operands.push(tmp);
}

void TQuery::Intersection()
{
    std::vector<uint32_t> res;
    std::shared_ptr<std::vector<uint32_t> > op1 = operands.top();
    operands.pop();
    std::shared_ptr<std::vector<uint32_t> > op2 = operands.top();
    operands.pop();
    std::set_intersection((*op1).begin(), (*op1).end(), (*op2).begin(), (*op2).end(), std::back_inserter(res));
    std::shared_ptr<std::vector<uint32_t> > tmp = std::make_shared<std::vector<uint32_t> >(res);
    operands.push(tmp);
}

void TQuery::Negation()
{
    std::shared_ptr<std::vector<uint32_t> > op = operands.top();
    std::vector<uint32_t> res;
    operands.pop();
    size_t j = 0;
    for (size_t i = 0; i < index.cntArticles; ++i) {
        if (j == (*op).size() || i < (*op)[j]) {
            res.push_back(i);
        } else if (i == (*op)[j]) {
            ++j;
        }
    }
    std::shared_ptr<std::vector<uint32_t> > tmp = std::make_shared<std::vector<uint32_t> >(res);
    operands.push(tmp);
}