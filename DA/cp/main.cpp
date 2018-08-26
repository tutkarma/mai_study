#include "query.h"

void Help()
{
    std::cout << "usage:" << std::endl;
    std::cout << "./prog index --input <input file> --output <index file>" << std::endl;
    std::cout << "./prog search --index <index file> --input <input file> --output <output file> [--full-output]" << std::endl;
}


std::string GetFilename(char **begin, char **end, const std::string &flag)
{
    char **it = std::find(begin, end, flag);
    if (it != end && ++it != end) {
        return std::string(*it);
    }
    return 0;
}

bool FlagExists(char **begin, char **end, const std::string &flag)
{
    return std::find(begin, end, flag) != end;
}

int main(int argc, char *argv[])
{
    std::locale::global(std::locale("en_US.UTF-8"));
    std::wcin.imbue(std::locale());
    std::wcout.imbue(std::locale());
    std::string inputFile, outputFile, indexFile;

    if (FlagExists(argv, argv + argc, "--input"))
        inputFile = GetFilename(argv, argv + argc, "--input");
    else {
        Help();
        return 0;
    }
    if (FlagExists(argv, argv + argc, "--output"))
        outputFile = GetFilename(argv, argv + argc, "--output");
    else {
        Help();
        return 0;
    }
    if (FlagExists(argv, argv + argc, "--index"))
        indexFile = GetFilename(argv, argv + argc, "--index");


    if (argc == 6 && std::string(argv[1]) == std::string("index")) {
        TIndex idx;
        idx.Build(inputFile);
        idx.SaveAll(outputFile);
    } else if ((argc == 8 || argc == 9) && std::string(argv[1]) == std::string("search")) {
        TQuery queries;
        if (FlagExists(argv, argv + argc, "--full-output"))
            queries.FullOutput();
        queries.GetIndex(indexFile);
        queries.ParseQueries(inputFile, outputFile);
    } else {
        Help();
    }

    return 0;
}