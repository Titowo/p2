#include "Utils.h"

namespace Utils {


    std::string toLower(const std::string& str) {
        std::string lowerStr = str; // Crea una copia de la cadena para modificarla
        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                       [](unsigned char c){ return static_cast<unsigned char>(std::tolower(c)); });
        return lowerStr;
    }

    std::string cleanWord(std::string word) {

        word = Utils::toLower(word);


        std::string cleaned_word;
        for (char c : word) {
            if (std::isalnum(c)) {
                cleaned_word += c;
            }
        }

        return cleaned_word;
    }

} 