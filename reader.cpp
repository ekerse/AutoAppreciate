#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

struct employeeData {
    std::string name;
    std::string employeeType;
    std::string department = "";
};

std::unordered_map<std::string, int> map;
std::unordered_map<std::string, employeeData> cross;

void crosscheck(char* list) {

    std::ifstream infile(list);

    std::string uniqname, first, last, employeeType, department, temp, fullName;

    while(infile >> uniqname) {

        infile >> first >> last >> employeeType;

        fullName = first + " " + last;

        if (employeeType == "External") {

            infile >> temp;

            employeeType = employeeType +  " " + temp;

        }

        infile >> department;

        if (department not_eq "N/A") {

            getline(infile, temp);

            department = department + temp;

        } else {

            department = "";

        }

        cross[uniqname + "@umich.edu"] = {fullName, employeeType, department.substr(0, department.length() - 1)};

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

    std::cout << "Name\tPoints\tStatus\tDepartment\n";

    for (auto const &pair: map) {

        if (cross.count(pair.first)) {

            employeeData dat = cross[pair.first];

            std::cout << dat.name << "\t" << pair.second << "\t" << dat.employeeType << "\t" << dat.department << "\n";

        } else {

            std::cout << pair.first << "\t" << pair.second << "\t \t \n";

        }

    }

}

int main(int argc, char** argv) {

    //std::cout << argv[0] << " " << argv[1] << '\n';

    if (argc != 3) exit(1);

    fileHandler(argv);

}