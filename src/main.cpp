#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <memory>

// IO () -> IO (Maybe String)
std::optional<std::string> fzf() {
    std::array<char, 128> buffer;
    std::string output;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("fzf", "r"), pclose);

    if(!pipe) {
        throw std::runtime_error("popen() failed");
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        output += buffer.data();
    }

    if (output == "") {
        return {};
    }

    return output;
}

int main(void) {
    std::optional<std::string> file = fzf();

    if (!file.has_value()) {
        return 0;
    }

    std::cout << "You choose: " << file.value() << std::endl;
    return 0;
}
