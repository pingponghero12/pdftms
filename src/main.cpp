#include <iostream>
#include <string>
#include <unistd.h>
#include <functional>
#include <unordered_map>

#include "commands.hpp"

using CommandFunction = std::function<int(const std::vector<std::string>&)>;

int main(int argc, char* argv[]) {
    std::unordered_map<std::string, CommandFunction> commands = {
        {"create", create},
        {"mv", mv},
        {"add", add},
        {"mkdir", mkdir},
        {"rename", rename_cmd}
    };

    if (argc == 1) {
        return view_default();
    }

    std::vector<std::string> args(argv+1, argv + argc);
    std::string command;

    if (!args.empty()) {
        command = args.at(0);
    }

    if (commands.find(command) != commands.end()) {
        return commands[command](args);
    }
    else {
        std::cerr << "Command not found" << std::endl;
        return EXIT_FAILURE;
    }
}
