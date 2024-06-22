#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
#include <map>
#include <filesystem> 

struct StockData
{
    std::string name;
    double open;
    double high;
    double low;
    double close;
    double adjClose;
    double volume;
    std::string date;
};

void computeHighestStockPrice(const std::vector<StockData>& data)
{
    // Define a hash table to store the highest stock price for each year
    std::unordered_map<std::string, double> highestPrices;

    // Iterate over each row in the data structure
    for (const auto& row : data) {
        // Extract the year and stock price from the current row
        std::string rowYear = row.name.substr(0, 4);
        double stockPrice = row.adjClose;

        // Check if the current year already exists in the hash table
        auto yearIter = highestPrices.find(rowYear);
        if (yearIter == highestPrices.end()) {
            // If it doesn't, add it with the current stock price as the highest price
            highestPrices[rowYear] = stockPrice;
        }
        else {
            // If the current year already exists in the hash table, compare the current stock price
            // with the highest price stored in the hash table
            double& highestPrice = yearIter->second;
            if (stockPrice > highestPrice) {
                // If the current stock price is higher than the highest price stored in the hash table,
                // update the hash table with the new highest price for that year
                highestPrice = stockPrice;
            }
        }
    }

    // Iterate over the hash table to output each year and its corresponding highest stock price
    std::cout << std::left << std::setw(10) << "Year"
        << std::left << std::setw(20) << "Highest Price"
        << std::endl;
    std::cout << std::setfill('-') << std::setw(30) << "-" << std::endl;
    std::cout << std::setfill(' ');
    for (const auto& [year, price] : highestPrices) {
        std::cout << std::left << std::setw(10) << year
            << std::left << std::setw(20) << price
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
        std::getline(ss, row.name, ',');
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
        std::getline(ss, row.date, ',');
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

        computeHighestStockPrice(data);


    }


    return 0;
}
