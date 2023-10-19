#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

std::unordered_map<std::string, int> map;
std::unordered_map<std::string, std::string> cross;

void crosscheck(char* list) {

    std::ifstream infile(list);

    std::string uniqname, fullName, remains;

    while(infile >> uniqname) {

        infile >> fullName;

        getline(infile, remains);

        fullName = fullName + remains;

        cross[uniqname + "@umich.edu"] = fullName.substr(0, fullName.length() - 1);

    }

}

void tallyMap(std::string key) {

    if (map.count(key)) {

        map[key]++;

    } else {

        map[key] = 1;

    }

}

void fileHandler(char** file) {

    std::ifstream infile(file[1]);

    std::string dump, date, sender, raw, potential;

    getline(infile, dump);

    infile >> date;

    getline(infile, dump);

    // Set starting point

    while (date.find("9/") != 0 or date.find("/2023") == std::string::npos) {

        if (not (infile >> date)) {

            std::cerr << "Error: Date range not found.\n";
            exit(1);

        }

        getline(infile, dump);

    }

    // Begin tallying points

    while (infile >> date) {

        infile >> dump >> sender >> raw;

        tallyMap(sender);

        // If a comma is in the string then there are multiple recepients

        std::size_t index = raw.find(",");

        while ((raw.find("@umich.edu") != std::string::npos) and (index != std::string::npos)) {

            index = raw.find(",");

            if (index == (raw.length() - 1)) {

                tallyMap(raw.substr(0, index));

                infile >> raw;

            } else {

                while (index != std::string::npos) {

                    tallyMap(raw.substr(0, index));

                    raw = raw.substr(index + 1);

                    index = raw.find(",");

                }

            }

        }

        tallyMap(raw);

        getline(infile, dump);

    }

    crosscheck(file[2]);

    for (auto const &pair: map) {

        if (cross.count(pair.first)) {

            std::cout << cross[pair.first] << ": " << pair.second << "\n";

        } else {

            std::cout << pair.first << ": " << pair.second << "\n";

        }

    }

}

int main(int argc, char** argv) {

    //std::cout << argv[0] << " " << argv[1] << '\n';

    if (argc != 3) exit(1);

    fileHandler(argv);

}