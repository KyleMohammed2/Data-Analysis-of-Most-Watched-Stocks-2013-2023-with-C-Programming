#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
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

/*
void displayData(const std::vector<StockData>& data)
{
    // Display the data in a table format
    std::cout << std::left << std::setw(20) << "Date"
        << std::left << std::setw(10) << "Open"
        << std::left << std::setw(10) << "High"
        << std::left << std::setw(15) << "Low"
        << std::left << std::setw(11) << "Close"
        << std::left << std::setw(20) << "Adj Close"
        << std::left << "Volume"
        << std::endl;
    std::cout << std::setfill('-') << std::setw(100) << "-" << std::endl;
    std::cout << std::setfill(' ');

    for (const auto& row : allData) {
        std::cout << std::left << std::setw(20) << row.name
            << std::left << std::setw(10) << row.open
            << std::left << std::setw(10) << row.high
            << std::left << std::setw(15) << row.low
            << std::left << std::setw(11) << row.close
            << std::left << std::setw(20) << row.adjClose
            << std::left << row.volume
            << std::endl;
    }
}
*/


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


    }


    return 0;
}
