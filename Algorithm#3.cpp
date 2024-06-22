#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
#include <map>
#include <filesystem> // for reading files in a folder

using namespace std;

// Define a struct to represent each row of the dataset
struct StockData {
    std::string name;
    double open;
    double high;
    double low;
    double close;
    double adjClose; //this is stock final price
    double volume;
    std::string date; // this is the year
};



void computeStockGrowth(const std::vector<StockData>& data)
{
    std::map<std::string, double> highestPrices;
    std::map<std::string, std::string> firstStatus, lastStatus;

    for (const auto& row : data) {
        std::string rowYear = row.name.substr(0, 4);
        double stockPrice = row.adjClose;

        auto yearIter = highestPrices.find(rowYear);
        if (yearIter == highestPrices.end()) {
            highestPrices[rowYear] = stockPrice;
            firstStatus[rowYear] = "stable";
        }
        else {
            double& highestPrice = yearIter->second;
            if (stockPrice > highestPrice) {
                lastStatus[rowYear] = "growing";
                highestPrice = stockPrice;
            }
            else if (stockPrice == highestPrice) {
                lastStatus[rowYear] = "stable";
            }
            else {
                lastStatus[rowYear] = "decreasing";
            }
        }
    }

    std::cout << std::left << std::setw(10) << "Year"
        << std::left << std::setw(15) << "First Status"
        << std::left << std::setw(15) << "Last Status"
        << std::endl;
    std::cout << std::setfill('-') << std::setw(45) << "-" << std::endl;
    std::cout << std::setfill(' ');

    for (const auto& [year, first] : firstStatus) {
        std::cout << std::left << std::setw(10) << year
            << std::left << std::setw(15) << first
            << std::left << std::setw(15) << lastStatus[year]
            << std::endl;
    }
}

// Define a function to read the data from a single file
std::vector<StockData> readDataFromFile(const std::string& filename) {
    std::ifstream infile;
    infile.open(filename);

    // Check if the file is open and readable
    if (!infile.is_open()) {
        std::cerr << "Error: could not open file: " << filename << std::endl;
        return {};
    }

    // Create a vector to store the data rows
    std::vector<StockData> data;

    // Read the file line by line and store each row in the vector
    std::string line;
    std::getline(infile, line); // skip the header row
    while (std::getline(infile, line)) {
        StockData row;
        std::stringstream ss(line);
        std::getline(ss, row.name, ','); // this is the entire date
        ss >> row.open;
        ss.ignore();
        ss >> row.high;
        ss.ignore();
        ss >> row.low;
        ss.ignore();
        ss >> row.close;
        ss.ignore();
        ss >> row.adjClose;
        ss.ignore();
        ss >> row.volume;
        ss.ignore();
        std::getline(ss, row.date, ','); //important for knowing the year
        data.push_back(row);
    }

    return data;
}

int main()
{
    // Define the folder name
    std::string foldername = "Most Watched Stocks of Past Decade(2013-2023)";

    // Check if the folder exists
    if (!std::filesystem::is_directory(foldername)) {
        std::cerr << "Error: folder does not exist: " << foldername << std::endl;
        return 1;
    }

    // Create a vector to store the data from all files
    std::vector<StockData> allData;

    // Loop over all files in the folder and read the data from each file
    for (const auto& file : std::filesystem::directory_iterator(foldername))
    {
        // Get the filename
        std::string filename = file.path().filename().string();

        // Check if the file is a CSV file
        if (filename.find(".csv") == std::string::npos) {
            continue; // skip non-CSV files
        }

        // Output the filename
        std::cout << "Stock: " << filename << std::endl;

        // Read the data from the file
        std::vector<StockData> data = readDataFromFile(file.path().string());

        // Add the data to the vector of all data
        allData.insert(allData.end(), data.begin(), data.end());

        computeStockGrowth(data);

    }


    return 0;
}

