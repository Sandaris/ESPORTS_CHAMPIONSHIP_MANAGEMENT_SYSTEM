#ifndef CSV_TOOLKIT_HPP
#define CSV_TOOLKIT_HPP

#include <iostream>      // For std::cout, std::cerr, std::cin
#include <fstream>       // For std::ifstream, std::ofstream, std::fstream
#include <string>        // For std::string
#include <cstring>       // For strcpy, strlen, strcmp, etc. (C-style string functions)
#include <cstdlib>       // For std::atoi, std::atof
#include <cmath>         // For std::floor, other math functions
#include <cctype>        // For std::isdigit, std::tolower
#include <filesystem>    // For path manipulation (C++17)
#include <limits>        // For std::numeric_limits
#include <chrono>        // For std::chrono (sleep)
#include <thread>        // For std::this_thread (sleep)
#include <sstream>       // For std::ostringstream

// Define a namespace alias for std::filesystem
namespace fs = std::filesystem;

namespace CsvToolkit {

    // --- Data Structures ---
    struct dataContainer2D {
        int error;
        char** fields;
        char*** data;
        int y; // Number of data rows
        int x; // Number of columns (fields)

        dataContainer2D() : error(0), fields(nullptr), data(nullptr), y(0), x(0) {}
    };

    struct dataContainer1D {
        int error;
        char** fields;
        int fields_count; // Number of elements in the 'fields' array
        char** data;
        int x; // Number of elements in the 'data' array

        dataContainer1D() : error(0), fields(nullptr), fields_count(0), data(nullptr), x(0) {}
    };

    // --- Forward Declarations ---

    // Memory Management
    void deleteDataContainer2D(dataContainer2D& container);
    void deleteDataContainer1D(dataContainer1D& container);

    // CSV Parsing
    char** parseCsvRow(const std::string& line, char delimiter, int expectedFields, int& actualTokenCount);
    dataContainer2D getData(const std::string& filenameWithExtension);

    // Utilities
    void clearTerminal();
    char* getString(const char* prompt);
    int getInt(const char* prompt);
    float getFloat(const char* prompt);

    dataContainer2D concatDataContainer(const dataContainer2D& data1, const dataContainer2D& data2, const char* keyField1, const char* keyField2);
    dataContainer2D filterDataContainer(const dataContainer2D& data, const char* field, const char* key);
    dataContainer1D getFieldValues(const dataContainer2D& data, const char* field);
    dataContainer2D shortenDataContainer(const dataContainer2D& data, const char* wantedFields[], int numFields);

    // Display Functions
    void displaySystemMessage(const char* message, int waitTimeInSeconds);
    void displayUnorderedOptions(const char* header, const char* options[], int noOptions);
    void displayOptions(const char* header, const char* options[], int noOptions, bool freeModified = true);
    int displayMenu(const char* header, const char* options[], int noOptions);
    void displayTabulatedData(const dataContainer2D& data);

    // Data Read Functions (Query Functions)
    dataContainer1D queryKey(const std::string& filenameWithExtension, const char* key);
    dataContainer1D queryField(const std::string& filenameWithExtension, const char* field);
    dataContainer2D queryFieldStrict(const std::string& filenameWithExtension, const char* field, const char* key);

    // Data Write Functions
    bool openCsvFileForWrite(const std::string& filenameWithExtension, std::ofstream& outFileStream, std::ios_base::openmode mode);
    int writeData(const std::string& filenameWithExtension, const dataContainer2D& containerToWrite);
    int writeNewDataRow(const std::string& filenameWithExtension, int numInputs, const char* inputs[]);
    int updateDataByUniqueKey(const std::string& filenameWithExtension, const char* uniqueKeyToUpdate, int uniqueKeyColumnIndex, int numColsInNewData, const char* newDataRow[]);
    int updateDataBySecondKey(const std::string& filenameWithExtension, const char* primaryKey, int primaryKeyIndex, const char* secondaryKey, int secondaryKeyIndex, int numColsInNewData, const char* newDataRow[]);
    int deleteByKey(const std::string& filenameWithExtension, const char* uniqueKeyToDelete, int uniqueKeyColumnIndex);
    int deleteBySecondKey(const std::string& filenameWithExtension, const char* primaryKey, int primaryKeyIndex, const char* secondaryKey, int secondaryKeyIndex);


// --- Implementations ---

// --- Memory Management ---
void deleteDataContainer2D(dataContainer2D& container) {
    if (container.fields) {
        for (int i = 0; i < container.x; ++i) {
            delete[] container.fields[i];
        }
        delete[] container.fields;
        container.fields = nullptr;
    }

    if (container.data) {
        for (int i = 0; i < container.y; ++i) {
            if (container.data[i]) {
                for (int j = 0; j < container.x; ++j) {
                    delete[] container.data[i][j];
                }
                delete[] container.data[i];
            }
        }
        delete[] container.data;
        container.data = nullptr;
    }
    container.y = 0;
    container.x = 0;
    container.error = 0;
}

void deleteDataContainer1D(dataContainer1D& container) {
    if (container.fields) {
        for (int i = 0; i < container.fields_count; ++i) {
            delete[] container.fields[i];
        }
        delete[] container.fields;
        container.fields = nullptr;
    }

    if (container.data) {
        for (int i = 0; i < container.x; ++i) {
            delete[] container.data[i];
        }
        delete[] container.data;
        container.data = nullptr;
    }
    container.fields_count = 0;
    container.x = 0;
    container.error = 0;
}

// --- CSV Parsing ---
char** parseCsvRow(const std::string& line, char delimiter, int expectedFields, int& actualTokenCount) {
    if (expectedFields < 1) {
        actualTokenCount = 0;
        return nullptr;
    }

    char** tokens = new char*[expectedFields];
    for (int i = 0; i < expectedFields; ++i) {
        tokens[i] = nullptr;
    }

    std::string currentTokenBuffer;
    bool insideQuotes = false;
    int tokenIndex = 0;

    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];

        if (c == '"') {
            insideQuotes = !insideQuotes;
        } else if (c == delimiter && !insideQuotes) {
            if (tokenIndex < expectedFields) {
                tokens[tokenIndex] = new char[currentTokenBuffer.length() + 1];
                strcpy(tokens[tokenIndex], currentTokenBuffer.c_str());
                tokenIndex++;
            }
            currentTokenBuffer.clear();

            if (tokenIndex == expectedFields - 1 && tokenIndex < expectedFields ) {
                std::string restOfLine = line.substr(i + 1);
                tokens[tokenIndex] = new char[restOfLine.length() + 1];
                strcpy(tokens[tokenIndex], restOfLine.c_str());
                tokenIndex++;
                actualTokenCount = tokenIndex;
                return tokens;
            }
        } else {
            currentTokenBuffer += c;
        }
    }

    if (tokenIndex < expectedFields && !currentTokenBuffer.empty()) {
        tokens[tokenIndex] = new char[currentTokenBuffer.length() + 1];
        strcpy(tokens[tokenIndex], currentTokenBuffer.c_str());
        tokenIndex++;
    } else if (tokenIndex < expectedFields && currentTokenBuffer.empty() && line.length() > 0 && line.back() == delimiter && !insideQuotes) {
        tokens[tokenIndex] = new char[1];
        tokens[tokenIndex][0] = '\0';
        tokenIndex++;
    } else if (tokenIndex < expectedFields && currentTokenBuffer.empty() && line.empty()){
         tokens[tokenIndex] = new char[1];
         tokens[tokenIndex][0] = '\0';
         tokenIndex++;
    }

    actualTokenCount = tokenIndex;
    return tokens;
}

dataContainer2D getData(const std::string& filenameWithExtension) {
    dataContainer2D container;
    fs::path dataDir;

    try {
        fs::path currentPath = fs::current_path();
        if (currentPath.has_parent_path()) {
            dataDir = currentPath.parent_path() / "data";
             if (!fs::exists(dataDir) && currentPath.parent_path().has_parent_path()){ // project_root/src/build -> project_root/data
                dataDir = currentPath.parent_path().parent_path() / "data";
            }
        } else { // Running from root of a drive or similar
            dataDir = currentPath / "data";
        }

        // Final check for 'data' directory existence
        if (!fs::exists(dataDir) || !fs::is_directory(dataDir)) {
            fs::path alternativeDataDir = fs::current_path() / "data"; // e.g. running from project_root
            if (fs::exists(alternativeDataDir) && fs::is_directory(alternativeDataDir)) {
                dataDir = alternativeDataDir;
            } else { // Try to create data dir if it's in a reasonable place (relative to parent or current)
                 fs::path primaryAttemptPath = currentPath.has_parent_path() ? currentPath.parent_path() / "data" : currentPath / "data";
                 std::cout << "Info: 'data' directory not found at " << primaryAttemptPath.string() 
                           << " or " << alternativeDataDir.string() << ". Attemping to use/create: " << primaryAttemptPath.string() << std::endl;
                dataDir = primaryAttemptPath; // Choose one to try to create if necessary
                // This directory creation logic might be too aggressive or platform-dependent for a library.
                // Better to assume the 'data' directory exists or handle its absence more gracefully.
                // For now, will proceed as if it exists or error out if file can't be opened.
                // The file opening will fail if dataDir isn't found/creatable.
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error when determining path: " << e.what() << std::endl;
        container.error = 1;
        return container;
    }

    fs::path filePath = dataDir / filenameWithExtension;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filePath.string() << std::endl;
        container.error = 1;
        return container;
    }

    std::string lineBuffer;
    int M_expectedFields = 0;

    if (std::getline(file, lineBuffer)) {
        if (lineBuffer.empty() && !file.eof()) {
             if (!std::getline(file, lineBuffer)){
                 std::cerr << "Error: CSV file is empty or contains only an empty header line after first." << std::endl;
                 container.error = 1; file.close(); return container;
             }
        } else if (lineBuffer.empty() && file.eof()){
            std::cerr << "Error: CSV file is empty or header is empty." << std::endl;
            container.error = 1; file.close(); return container;
        }

        if (lineBuffer.length() >= 3 && static_cast<unsigned char>(lineBuffer[0]) == 0xEF &&
            static_cast<unsigned char>(lineBuffer[1]) == 0xBB && static_cast<unsigned char>(lineBuffer[2]) == 0xBF) {
            lineBuffer = lineBuffer.substr(3);
        }
        if (!lineBuffer.empty() && lineBuffer.back() == '\r') lineBuffer.pop_back();

        int actualFieldCountInHeader = 0;
        char** tempFields = parseCsvRow(lineBuffer, ',', 256, actualFieldCountInHeader);

        if (!tempFields || actualFieldCountInHeader == 0) {
             std::cerr << "Error: Could not parse header or header is empty in file: " << filePath.string() << std::endl;
             container.error = 1;
             if(tempFields) {
                 for(int i=0; i < 256; ++i) delete[] tempFields[i]; // Clean up if array was made
                 delete[] tempFields;
             }
             file.close(); return container;
        }
        M_expectedFields = actualFieldCountInHeader;
        container.x = M_expectedFields;
        container.fields = new char*[M_expectedFields];
        for(int i=0; i < M_expectedFields; ++i) {
            if (tempFields[i]) container.fields[i] = tempFields[i]; // Transfer ownership
            else { container.fields[i] = new char[1]; container.fields[i][0] = '\0';} // Should not happen
        }
        // Clean up remaining pointers in tempFields if they were somehow allocated by parseCsvRow beyond actualFieldCountInHeader
        // (current parseCsvRow doesn't do this for its main output, but good for safety with the large expectedFields value)
        for (int i = M_expectedFields; i < 256; ++i) delete[] tempFields[i]; // delete[] nullptr is safe
        delete[] tempFields; // Delete the temporary array of pointers.
    } else {
        std::cerr << "Error: Could not read header line (file might be empty): " << filePath.string() << std::endl;
        container.error = 1; file.close(); return container;
    }

    int rowCount = 0; int rowCapacity = 10;
    if (rowCapacity == 0 && container.y > 0) rowCapacity = 10; // Ensure capacity if y > 0 but capacity is 0.
    container.data = new char**[rowCapacity > 0 ? rowCapacity : 1]; // Min alloc 1 to avoid new char**[0]
    if (rowCapacity > 0) container.data[0] = nullptr; 

    while (std::getline(file, lineBuffer)) {
        if (lineBuffer.empty()) continue;
        if (!lineBuffer.empty() && lineBuffer.back() == '\r') lineBuffer.pop_back();

        if (rowCount >= rowCapacity) {
            int newCapacity = rowCapacity == 0 ? 10 : rowCapacity * 2;
            char*** newData = new char**[newCapacity];
            for (int i = 0; i < rowCount; ++i) newData[i] = container.data[i];
            for (int i = rowCount; i < newCapacity; ++i) newData[i] = nullptr; // Init new part
            delete[] container.data;
            container.data = newData;
            rowCapacity = newCapacity;
        }

        int actualDataTokensInRow = 0;
        container.data[rowCount] = parseCsvRow(lineBuffer, ',', M_expectedFields, actualDataTokensInRow);
        
        if (!container.data[rowCount]) {
            std::cerr << "Warning: Failed to parse data row " << (rowCount + 1) << ". Malformed CSV or memory issue. Skipping." << std::endl;
            continue; 
        }

        if (actualDataTokensInRow != M_expectedFields) {
            // std::cerr << "Warning: Row " << (rowCount + 1) << " in " << filenameWithExtension << " has " << actualDataTokensInRow
            //           << " fields, expected " << M_expectedFields << ". Padding with empty strings." << std::endl;
            for(int k=actualDataTokensInRow; k < M_expectedFields; ++k) {
                if (container.data[rowCount][k] == nullptr) { // If parseCsvRow didn't fill it
                    container.data[rowCount][k] = new char[1];
                    container.data[rowCount][k][0] = '\0';
                }
            }
        }
        rowCount++;
    }
    container.y = rowCount;
    file.close();

    if (rowCount > 0 && rowCount < rowCapacity) {
        char*** finalData = new char**[rowCount]; 
        for (int i = 0; i < rowCount; ++i) finalData[i] = container.data[i];
        delete[] container.data;
        container.data = finalData;
    } else if (rowCount == 0 && container.data != nullptr) { 
        delete[] container.data; container.data = nullptr;
    }
    return container;
}


// --- Utilities ---
void clearTerminal() {
    std::cout << "\033[2J\033[1;1H"; // Clears screen, moves cursor to top-left (ANSI)
    std::cout.flush();
}

char* getString(const char* prompt) {
    std::string inputString;
    if(prompt) std::cout << prompt;
    std::getline(std::cin, inputString);

    if (std::cin.fail() && !std::cin.eof()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear rest of bad input line
        std::cerr << "Input error during getString." << std::endl;
        char* errorRes = new char[1]; errorRes[0] = '\0'; return errorRes;
    }

    char* result = new char[inputString.length() + 1];
    strcpy(result, inputString.c_str());
    return result;
}

int getInt(const char* prompt) {
    std::string line;
    if(prompt) std::cout << prompt;
    std::getline(std::cin, line);

    if (std::cin.fail() && !std::cin.eof()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Input error during getInt." << std::endl; return -1;
    }
    if (line.empty()) { std::cerr << "Input error: Empty line for getInt." << std::endl; return -1; }

    bool isNumeric = true; size_t start = 0;
    if (line[0] == '-' || line[0] == '+') {
        if (line.length() == 1) isNumeric = false; start = 1;
    }
    if(isNumeric) { // only check digits if not already flagged as non-numeric
        for (size_t i = start; i < line.length(); ++i) {
            if (!std::isdigit(static_cast<unsigned char>(line[i]))) {
                isNumeric = false; break;
            }
        }
    }
    if (isNumeric && line.length() == start && start > 0) isNumeric = false; // only sign case "-"

    if (isNumeric) {
        try { return std::stoi(line); }
        catch (const std::out_of_range&) { std::cerr << "Error: Number '" << line << "' out of range for integer." << std::endl; }
        catch (const std::invalid_argument&) { std::cerr << "Error: Invalid argument '" << line << "' for integer conversion." << std::endl; }
    } else {
        std::cerr << "Error: Input '" << line << "' is not a valid integer." << std::endl;
    }
    return -1;
}

float getFloat(const char* prompt) {
    std::string line;
    if(prompt) std::cout << prompt;
    std::getline(std::cin, line);

    if (std::cin.fail() && !std::cin.eof()) {
        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Input error during getFloat." << std::endl; return -1.0f;
    }
    if (line.empty()) { std::cerr << "Input error: Empty line for getFloat." << std::endl; return -1.0f; }

    bool isNumericFloat = true; int decimalPointCount = 0; size_t start = 0;
    if (line[0] == '-' || line[0] == '+') {
        if (line.length() == 1) isNumericFloat = false; start = 1;
    }

    if(isNumericFloat) {
        for (size_t i = start; i < line.length(); ++i) {
            if (line[i] == '.') {
                decimalPointCount++;
                if (decimalPointCount > 1) { isNumericFloat = false; break; }
            } else if (!std::isdigit(static_cast<unsigned char>(line[i]))) {
                isNumericFloat = false; break;
            }
        }
    }
    if (isNumericFloat && line.length() == start + decimalPointCount && decimalPointCount == 1 && start == line.length()) isNumericFloat = false; // e.g. "."
    if (isNumericFloat && line.length() == start + decimalPointCount && decimalPointCount == 1 && start > 0 && line.length() == start + 1) isNumericFloat = false; // e.g. "-."
    if (isNumericFloat && line.length() == start && start > 0) isNumericFloat = false; // only sign case "-"

    if (isNumericFloat) {
        try { return std::stof(line); }
        catch (const std::out_of_range&) { std::cerr << "Error: Number '" << line << "' out of range for float." << std::endl;}
        catch (const std::invalid_argument&) { std::cerr << "Error: Invalid argument '" << line << "' for float conversion." << std::endl; }
    } else {
        std::cerr << "Error: Input '" << line << "' is not a valid float." << std::endl;
    }
    return -1.0f;
}

// Helper to duplicate a C-string using new char[]
char* duplicateString(const char* cstr) {
    if (!cstr) { // Handle null input gracefully
        char* empty_str = new char[1];
        empty_str[0] = '\0';
        return empty_str; // Return empty string for null input
    }
    char* new_str = new char[strlen(cstr) + 1];
    strcpy(new_str, cstr);
    return new_str;
}

    // --- Helper to ensure CSV headers exist ---
    void ensureCsvHeader(const std::string& filename, const char* headers[], int numFields) {
        fs::path dataDir;
        try { // Simplified path logic for brevity, ensure your openCsvFileForWrite handles this robustly
            fs::path currentPath = fs::current_path();
            if (currentPath.has_parent_path()) dataDir = currentPath.parent_path() / "data"; else dataDir = currentPath / "data";
             if (!fs::exists(dataDir) && currentPath.has_parent_path() && currentPath.parent_path().has_parent_path()){
                 dataDir = currentPath.parent_path().parent_path() / "data";
            }
             if (!fs::exists(dataDir)) dataDir = fs::current_path() / "data"; // Final fallback
        } catch (...) { dataDir = "data"; }


        fs::path filePath = dataDir / filename;
        std::ifstream fileCheck(filePath.string());
        bool fileExistsAndNotEmpty = fileCheck.good() && fileCheck.peek() != EOF;
        fileCheck.close();

        if (!fileExistsAndNotEmpty) {
            std::cout << "Info: CSV file '" << filename << "' not found or empty. Writing headers." << std::endl;
            // Use writeNewDataRow to write the header
            // This function itself will use openCsvFileForWrite which handles directory creation
            writeNewDataRow(filename, numFields, headers);
        }
    }


dataContainer2D concatDataContainer(const dataContainer2D& data1, const dataContainer2D& data2, const char* keyField1, const char* keyField2) {
    dataContainer2D returnedValue;
    if (data1.error || data2.error) {
        returnedValue.error = 1; std::cerr << "Error: Input data for concat has error." << std::endl; return returnedValue;
    }
    if (!keyField1 || !keyField2) {
        returnedValue.error = 1; std::cerr << "Error: Null keyField provided for concat." << std::endl; return returnedValue;
    }

    int wantedFieldIndex1 = -1;
    for (int i = 0; i < data1.x; ++i) {
        if (data1.fields[i] && strcmp(data1.fields[i], keyField1) == 0) { wantedFieldIndex1 = i; break; }
    }
    if (wantedFieldIndex1 == -1) {
        returnedValue.error = 1; std::cerr << "Error: keyField1 '" << keyField1 << "' not in data1." << std::endl; return returnedValue;
    }

    int wantedFieldIndex2 = -1;
    for (int i = 0; i < data2.x; ++i) {
        if (data2.fields[i] && strcmp(data2.fields[i], keyField2) == 0) { wantedFieldIndex2 = i; break; }
    }
    if (wantedFieldIndex2 == -1) {
        returnedValue.error = 1; std::cerr << "Error: keyField2 '" << keyField2 << "' not in data2." << std::endl; return returnedValue;
    }

    returnedValue.y = data1.y;
    returnedValue.x = data1.x + data2.x - 1;
    if (returnedValue.x <= 0) { // y can be 0 for empty data1
        returnedValue.error = 1; std::cerr << "Error: Invalid dimensions for concat result." << std::endl; return returnedValue;
    }

    returnedValue.fields = new char*[returnedValue.x];
    for (int i = 0; i < data1.x; ++i) returnedValue.fields[i] = duplicateString(data1.fields[i]);
    int currentFieldIdx = data1.x;
    for (int k = 0; k < data2.x; ++k) {
        if (k == wantedFieldIndex2) continue;
        if (currentFieldIdx < returnedValue.x) returnedValue.fields[currentFieldIdx++] = duplicateString(data2.fields[k]);
    }
    
    if (returnedValue.y > 0) {
        returnedValue.data = new char**[returnedValue.y];
        for (int i = 0; i < returnedValue.y; ++i) {
            returnedValue.data[i] = new char*[returnedValue.x];
            for (int k = 0; k < data1.x; ++k) returnedValue.data[i][k] = duplicateString(data1.data[i] ? data1.data[i][k] : "");
            for (int k = data1.x; k < returnedValue.x; ++k) returnedValue.data[i][k] = duplicateString("-");

            const char* keyToMatch = (data1.data[i] && data1.data[i][wantedFieldIndex1]) ? data1.data[i][wantedFieldIndex1] : nullptr;
            if (!keyToMatch) continue;

            for (int j = 0; j < data2.y; ++j) {
                if (data2.data[j] && data2.data[j][wantedFieldIndex2] && strcmp(keyToMatch, data2.data[j][wantedFieldIndex2]) == 0) {
                    int currentDataColIdx = data1.x;
                    for (int k = 0; k < data2.x; ++k) {
                        if (k == wantedFieldIndex2) continue;
                        if (currentDataColIdx < returnedValue.x) { // Check boundary before access
                            delete[] returnedValue.data[i][currentDataColIdx]; // Delete placeholder
                            returnedValue.data[i][currentDataColIdx] = duplicateString(data2.data[j][k]);
                            currentDataColIdx++;
                        }
                    }
                    break; 
                }
            }
        }
    } else returnedValue.data = nullptr;
    return returnedValue;
}


dataContainer2D filterDataContainer(const dataContainer2D& dataIn, const char* field, const char* key) {
    dataContainer2D returnedValue;
    if (dataIn.error) { returnedValue.error = 1; std::cerr << "Error: Input for filter has error." << std::endl; return returnedValue; }
    if (!field || !key) { returnedValue.error = 1; std::cerr << "Error: Null field/key for filter." << std::endl; return returnedValue; }

    returnedValue.x = dataIn.x;
    int wantedFieldIndex = -1;
    for (int i = 0; i < dataIn.x; ++i) {
        if (dataIn.fields[i] && strcmp(dataIn.fields[i], field) == 0) { wantedFieldIndex = i; break; }
    }
    if (wantedFieldIndex == -1) {
        returnedValue.error = 1; std::cerr << "Error: Field '" << field << "' not found for filter." << std::endl; return returnedValue;
    }

    int matchingRowCount = 0;
    for (int i = 0; i < dataIn.y; ++i) {
        if (dataIn.data[i] && dataIn.data[i][wantedFieldIndex] && strcmp(dataIn.data[i][wantedFieldIndex], key) == 0) matchingRowCount++;
    }

    returnedValue.fields = new char*[returnedValue.x];
    for(int i=0; i < returnedValue.x; ++i) returnedValue.fields[i] = duplicateString(dataIn.fields[i]);
    
    if (matchingRowCount == 0) {
        returnedValue.y = 0; returnedValue.data = nullptr; return returnedValue;
    }

    returnedValue.data = new char**[matchingRowCount];
    returnedValue.y = matchingRowCount;
    int currentRowInReturned = 0;
    for (int i = 0; i < dataIn.y; ++i) {
        if (dataIn.data[i] && dataIn.data[i][wantedFieldIndex] && strcmp(dataIn.data[i][wantedFieldIndex], key) == 0) {
            if (currentRowInReturned < matchingRowCount) {
                returnedValue.data[currentRowInReturned] = new char*[returnedValue.x];
                for (int j = 0; j < returnedValue.x; ++j) returnedValue.data[currentRowInReturned][j] = duplicateString(dataIn.data[i][j]);
                currentRowInReturned++;
            }
        }
    }
    return returnedValue;
}

dataContainer1D getFieldValues(const dataContainer2D& dataIn, const char* field) {
    dataContainer1D returnedValue;
    if (dataIn.error) { returnedValue.error = 1; std::cerr << "Error: Input for getFieldValues has error." << std::endl; return returnedValue; }
    if (!field) { returnedValue.error = 1; std::cerr << "Error: Null field for getFieldValues." << std::endl; return returnedValue; }

    int fieldColumn = -1;
    for (int i = 0; i < dataIn.x; ++i) {
        if (dataIn.fields[i] && strcmp(dataIn.fields[i], field) == 0) { fieldColumn = i; break; }
    }
    if (fieldColumn == -1) {
        returnedValue.error = 1; std::cerr << "Error: Field '" << field << "' not found." << std::endl; return returnedValue;
    }

    returnedValue.fields = new char*[1];
    returnedValue.fields[0] = duplicateString(field);
    returnedValue.fields_count = 1;

    if (dataIn.y > 0) {
        returnedValue.data = new char*[dataIn.y];
        returnedValue.x = dataIn.y;
        for (int i = 0; i < dataIn.y; ++i) {
            returnedValue.data[i] = (dataIn.data[i] && dataIn.data[i][fieldColumn]) ? duplicateString(dataIn.data[i][fieldColumn]) : duplicateString("");
        }
    } else { returnedValue.data = nullptr; returnedValue.x = 0; }
    return returnedValue;
}

dataContainer2D shortenDataContainer(const dataContainer2D& dataIn, const char* wantedFields[], int numWantedFields) {
    dataContainer2D returnedValue;
    if (dataIn.error) { returnedValue.error = 1; std::cerr << "Error: Input for shorten has error." << std::endl; return returnedValue; }
    if (numWantedFields <= 0 || !wantedFields) {
        returnedValue.error = 1; std::cerr << "Error: No/invalid fields for shorten." << std::endl; return returnedValue;
    }

    returnedValue.x = numWantedFields; returnedValue.y = dataIn.y;
    int* wantedFieldIndexes = new int[numWantedFields];

    for (int i = 0; i < numWantedFields; ++i) {
        wantedFieldIndexes[i] = -1;
        if (!wantedFields[i]) {
            returnedValue.error = 1; std::cerr << "Error: Null field name in wantedFields at index " << i << std::endl;
            delete[] wantedFieldIndexes; return returnedValue;
        }
        for (int j = 0; j < dataIn.x; ++j) {
            if (dataIn.fields[j] && strcmp(dataIn.fields[j], wantedFields[i]) == 0) {
                wantedFieldIndexes[i] = j; break;
            }
        }
        if (wantedFieldIndexes[i] == -1) {
            returnedValue.error = 1; std::cerr << "Error: Wanted field '" << wantedFields[i] << "' not found." << std::endl;
            delete[] wantedFieldIndexes; return returnedValue;
        }
    }

    returnedValue.fields = new char*[returnedValue.x];
    for (int i = 0; i < numWantedFields; ++i) returnedValue.fields[i] = duplicateString(wantedFields[i]);

    if (returnedValue.y > 0) {
        returnedValue.data = new char**[returnedValue.y];
        for (int i = 0; i < returnedValue.y; ++i) {
            returnedValue.data[i] = new char*[returnedValue.x];
            for (int j = 0; j < returnedValue.x; ++j) {
                returnedValue.data[i][j] = (dataIn.data[i] && dataIn.data[i][wantedFieldIndexes[j]]) ? duplicateString(dataIn.data[i][wantedFieldIndexes[j]]) : duplicateString("");
            }
        }
    } else returnedValue.data = nullptr;
    
    delete[] wantedFieldIndexes;
    return returnedValue;
}


// --- Display Functions ---
void displaySystemMessage(const char* message, int waitTimeInSeconds) {
    clearTerminal();
    if(message) std::cout << message << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(waitTimeInSeconds));
    clearTerminal();
}

void displayUnorderedOptions(const char* header, const char* options[], int noOptions) {
    if (!options && noOptions > 0) { std::cerr << "Warning: Null options array with noOptions > 0." << std::endl; return; }
    if (noOptions <= 0 && !header) { std::cout << "No options or header to display." << std::endl; return; }


    size_t maxOptLength = 0;
    if (header) maxOptLength = strlen(header);

    for (int i = 0; i < noOptions; ++i) {
        if (options[i]) {
            size_t currentLength = strlen(options[i]);
            if (currentLength > maxOptLength) maxOptLength = currentLength;
        }
    }

    const int horizontalPadding = 5; // Reduced padding
    size_t borderLength = maxOptLength + 2 * horizontalPadding + 2; // +2 for '|' borders

    auto printLine = [&](){ for (size_t i = 0; i < borderLength; ++i) std::cout << "-"; std::cout << std::endl; };
    
    printLine();
    if (header) {
        size_t headerLen = strlen(header);
        size_t totalPaddingForHeader = (borderLength > 2 + headerLen) ? borderLength - 2 - headerLen : 0;
        size_t leftHeaderPadding = totalPaddingForHeader / 2;
        size_t rightHeaderPadding = totalPaddingForHeader - leftHeaderPadding;
        std::cout << "|";
        for (size_t i = 0; i < leftHeaderPadding; ++i) std::cout << " ";
        std::cout << header;
        for (size_t i = 0; i < rightHeaderPadding; ++i) std::cout << " ";
        std::cout << "|" << std::endl;
        printLine();
    }

    const int leftItemPadding = 2;
    for (int i = 0; i < noOptions; ++i) {
        if (options[i]) {
            const char* text = options[i];
            size_t textLen = strlen(text);
            std::cout << "|";
            for (int j = 0; j < leftItemPadding; ++j) std::cout << " ";
            std::cout << text;
            size_t currentContentLength = leftItemPadding + textLen;
            size_t rightItemPadding = (borderLength > 2 + currentContentLength) ? (borderLength - 2 - currentContentLength) : 0;
            for (size_t j = 0; j < rightItemPadding; ++j) std::cout << " ";
            std::cout << "|" << std::endl;
        }
    }
    printLine();
}


void displayOptions(const char* header, const char* options[], int noOptions, bool freeTempModifiedOptions /*=true*/) {
    if (!options && noOptions > 0) { std::cerr << "Warning: Null options array with noOptions > 0 for displayOptions." << std::endl; return;}
    if (noOptions <= 0 && !header) { std::cout << "No options or header to display for displayOptions." << std::endl; return;}


    char** tempModifiedOptions = (noOptions > 0) ? new char*[noOptions] : nullptr;
    if(noOptions > 0) { // Only create tempModifiedOptions if there are options
        for (int i = 0; i < noOptions; ++i) {
            std::ostringstream oss;
            oss << (i + 1) << ". " << (options[i] ? options[i] : "");
            tempModifiedOptions[i] = duplicateString(oss.str().c_str());
        }
    }

    displayUnorderedOptions(header, const_cast<const char**>(tempModifiedOptions), noOptions);

    if (freeTempModifiedOptions && tempModifiedOptions) {
        for (int i = 0; i < noOptions; ++i) delete[] tempModifiedOptions[i];
        delete[] tempModifiedOptions;
    }
}

int displayMenu(const char* header, const char* options[], int noOptions) {
    if (!options || noOptions <= 0) { std::cerr << "Error: No options provided for menu." << std::endl; return -1;}

    displayOptions(header, options, noOptions, true);

    std::string inputLine = getString("Enter your Input: "); // getString now returns dynamically allocated char*

    bool isNumeric = !inputLine.empty();
    size_t start = 0;
    if (!inputLine.empty() && (inputLine[0] == '-' || inputLine[0] == '+')) {
        if (inputLine.length() == 1) isNumeric = false; start = 1;
    }
    if(isNumeric) {
        for (size_t i = start; i < inputLine.length(); ++i) {
            if (!std::isdigit(static_cast<unsigned char>(inputLine[i]))) { isNumeric = false; break; }
        }
    }
    if (isNumeric && inputLine.length() == start && start > 0) isNumeric = false;
    
    if (isNumeric) {
        try {
            int choice = std::stoi(inputLine);
            if (choice > 0 && choice <= noOptions) {
                // delete[] inputLine; // No longer needed as getString changed to std::string internally
                return choice;
            }
        } catch (...) { /* ignore, try string match */ }
    }

    std::string lowerInput = inputLine;
    for (char &c : lowerInput) c = std::tolower(static_cast<unsigned char>(c));
    // delete[] inputLine; // No, lowerInput is a copy on stack, inputLine from getString is heap.
                         // Wait, getString was changed to return char*, so need to delete the result of getString.
                         // Actually, previous displayMenu was using std::string inputLine; directly.
                         // Let's revert `inputLine` to `std::string` for simplicity if `getString` returns `std::string`
                         // OR ensure `getString` result is deleted.
                         // The `getString` I wrote earlier returns `char*`. This needs careful handling.

    // For now, assuming inputLine from getString needs delete.
    // But it's safer if getString returns std::string, or displayMenu uses std::string directly.
    // Let's assume getString is kept as returning char* for now.
    // char* rawInput = getString("Enter your Input: "); // Get raw input
    // std::string inputLineStr(rawInput ? rawInput : ""); // Convert to std::string
    // delete[] rawInput; // Free memory from getString
    // ... then use inputLineStr
    // This is getting complex. Simpler: use std::string for inputLine and std::getline directly in displayMenu.

    // Re-simplifying input for displayMenu:
    std::string menuInputStr;
    std::cout << "Enter your Input: ";
    std::getline(std::cin, menuInputStr);
    // ... rest of the logic using menuInputStr ...
    // The numeric check becomes:
    isNumeric = !menuInputStr.empty();
    start = 0;
    // ... same numeric check logic with menuInputStr ...
    if (isNumeric) {
        try {
            int choice = std::stoi(menuInputStr);
            if (choice > 0 && choice <= noOptions) return choice;
        } catch (...) { /* ignore */ }
    }
    // String comparison logic using menuInputStr
    std::string lowerMenuInput = menuInputStr;
    for (char &c : lowerMenuInput) c = std::tolower(static_cast<unsigned char>(c));

    for (int i = 0; i < noOptions; ++i) {
        if (options[i]) {
            std::string lowerOption = options[i];
            for (char &c : lowerOption) c = std::tolower(static_cast<unsigned char>(c));
            if (lowerMenuInput == lowerOption) return i + 1;
        }
    }

    displaySystemMessage("Invalid input. Please try again.", 2);
    return displayMenu(header, options, noOptions);
}

void displayTabulatedData(const dataContainer2D& data) {
    if (data.error || data.x == 0 || !data.fields) {
        std::cout << "No data/fields to display or data container has an error." << std::endl; return;
    }

    const int minPaddingPerSide = 2;
    int* columnWidths = new int[data.x];
    for (int i = 0; i < data.x; ++i) columnWidths[i] = data.fields[i] ? strlen(data.fields[i]) : 0;

    if (data.y > 0 && data.data) {
        for (int j = 0; j < data.y; ++j) {
            if (data.data[j]) {
                for (int i = 0; i < data.x; ++i) {
                    if (data.data[j][i]) {
                        int currentLength = strlen(data.data[j][i]);
                        if (currentLength > columnWidths[i]) columnWidths[i] = currentLength;
                    }
                }
            }
        }
    }

    int totalTableWidth = 1;
    for (int i = 0; i < data.x; ++i) totalTableWidth += columnWidths[i] + (2 * minPaddingPerSide) + 1;

    auto printHorizontalLine = [&](){ for (int i = 0; i < totalTableWidth; ++i) std::cout << "-"; std::cout << std::endl; };
    auto printRowContent = [&](char* rowData[]) {
        std::cout << "|";
        for (int i = 0; i < data.x; ++i) {
            std::string cellContent = rowData && rowData[i] ? rowData[i] : "";
            int currentCellContentLength = cellContent.length();
            int totalCellWidth = columnWidths[i] + (2 * minPaddingPerSide);
            int paddingNeeded = totalCellWidth - currentCellContentLength;
            int leftPadding = paddingNeeded / 2;
            int rightPadding = paddingNeeded - leftPadding;
            for(int p=0; p<leftPadding; ++p) std::cout << " ";
            std::cout << cellContent;
            for(int p=0; p<rightPadding; ++p) std::cout << " ";
            std::cout << "|";
        }
        std::cout << std::endl;
    };

    printHorizontalLine(); printRowContent(data.fields); printHorizontalLine();
    if (data.y > 0 && data.data) {
        for (int i = 0; i < data.y; ++i) {
            if (data.data[i]) { printRowContent(data.data[i]); printHorizontalLine(); }
        }
    }
    delete[] columnWidths;
}


// --- Data Read Functions (Query Functions) ---
dataContainer1D queryKey(const std::string& filenameWithExtension, const char* key) {
    dataContainer1D returnedValue;
    if (!key) { returnedValue.error = 1; std::cerr << "Error: Null key for queryKey." << std::endl; return returnedValue; }

    dataContainer2D fullData = getData(filenameWithExtension);
    if (fullData.error) {
        returnedValue.error = 1; deleteDataContainer2D(fullData); return returnedValue;
    }
    if (fullData.y == 0 || fullData.x == 0 || !fullData.data || !fullData.fields) {
        returnedValue.error = 1; std::cerr << "Warning: No data/fields in file for queryKey." << std::endl;
        deleteDataContainer2D(fullData); return returnedValue;
    }

    const int keyColumnIndex = 0;
    for (int i = 0; i < fullData.y; ++i) {
        if (fullData.data[i] && fullData.data[i][keyColumnIndex] && strcmp(fullData.data[i][keyColumnIndex], key) == 0) {
            returnedValue.x = fullData.x; returnedValue.data = new char*[fullData.x];
            for (int j = 0; j < fullData.x; ++j) returnedValue.data[j] = duplicateString(fullData.data[i][j]);
            returnedValue.fields_count = fullData.x; returnedValue.fields = new char*[fullData.x];
            for (int j = 0; j < fullData.x; ++j) returnedValue.fields[j] = duplicateString(fullData.fields[j]);
            deleteDataContainer2D(fullData); return returnedValue;
        }
    }
    returnedValue.error = 1; /* Key not found */ deleteDataContainer2D(fullData); return returnedValue;
}

dataContainer1D queryField(const std::string& filenameWithExtension, const char* field) {
    if (!field) { dataContainer1D errVal; errVal.error = 1; std::cerr << "Error: Null field for queryField." << std::endl; return errVal; }
    dataContainer2D fullData = getData(filenameWithExtension);
    if (fullData.error) { dataContainer1D rv; rv.error = 1; deleteDataContainer2D(fullData); return rv; }
    dataContainer1D result = getFieldValues(fullData, field);
    deleteDataContainer2D(fullData); return result;
}

dataContainer2D queryFieldStrict(const std::string& filenameWithExtension, const char* field, const char* key) {
    if (!field || !key) { dataContainer2D ev; ev.error = 1; std::cerr << "Error: Null field/key for queryFieldStrict." << std::endl; return ev; }
    dataContainer2D fullData = getData(filenameWithExtension);
    if (fullData.error) { dataContainer2D rv; rv.error = 1; deleteDataContainer2D(fullData); return rv; }
    dataContainer2D result = filterDataContainer(fullData, field, key);
    deleteDataContainer2D(fullData); return result;
}


// --- Data Write Functions ---
bool openCsvFileForWrite(const std::string& filenameWithExtension, std::ofstream& outFileStream, std::ios_base::openmode mode) {
    fs::path dataDir;
    try {
        fs::path currentPath = fs::current_path();
        if (currentPath.has_parent_path()) {
            dataDir = currentPath.parent_path() / "data";
            if (!fs::exists(dataDir) && currentPath.parent_path().has_parent_path()) {
                 dataDir = currentPath.parent_path().parent_path() / "data";
            }
        } else dataDir = currentPath / "data";
        
        if (!fs::exists(dataDir)) {
            fs::path alternativeDataDir = fs::current_path() / "data";
            if (fs::exists(alternativeDataDir) && fs::is_directory(alternativeDataDir)) dataDir = alternativeDataDir;
            else {
                dataDir = currentPath.has_parent_path() ? currentPath.parent_path() / "data" : currentPath / "data";
                if(!fs::exists(dataDir)) {
                    std::cout << "Info: Data directory (" << dataDir.string() << ") not found, attempting to create." << std::endl;
                    if (!fs::create_directories(dataDir)) {
                        std::cerr << "Error: Could not create data directory: " << dataDir.string() << std::endl; return false;
                    }
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error (write path): " << e.what() << std::endl; return false;
    }
    fs::path filePath = dataDir / filenameWithExtension;
    outFileStream.open(filePath, mode);
    if (!outFileStream.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filePath.string() << std::endl; return false;
    }
    return true;
}

int writeData(const std::string& filenameWithExtension, const dataContainer2D& containerToWrite) {
    if (containerToWrite.error) { std::cerr << "Error: Input for writeData has error." << std::endl; return 1; }
    std::ofstream outFile;
    if (!openCsvFileForWrite(filenameWithExtension, outFile, std::ios::out | std::ios::trunc)) return 1;

    if (containerToWrite.x > 0 && containerToWrite.fields) {
        for (int i = 0; i < containerToWrite.x; ++i) {
            if (containerToWrite.fields[i]) outFile << containerToWrite.fields[i];
            if (i < containerToWrite.x - 1) outFile << ",";
        }
        outFile << "\n";
    }
    if (containerToWrite.y > 0 && containerToWrite.data) {
        for (int i = 0; i < containerToWrite.y; ++i) {
            if (containerToWrite.data[i]) {
                for (int j = 0; j < containerToWrite.x; ++j) {
                    if (containerToWrite.data[i][j]) outFile << containerToWrite.data[i][j];
                    if (j < containerToWrite.x - 1) outFile << ",";
                }
                outFile << "\n";
            }
        }
    }
    outFile.close(); return 0;
}

int writeNewDataRow(const std::string& filenameWithExtension, int numInputs, const char* inputs[]) {
    if (!inputs || numInputs <= 0) { std::cerr << "Error: No inputs for writeNewDataRow." << std::endl; return 1; }
    for (int i = 0; i < numInputs; ++i) {
        if (inputs[i] && strchr(inputs[i], ',')) {
            std::cerr << "Warning (writeNewDataRow): Input '" << inputs[i] << "' contains comma. Output may be malformed without quoting." << std::endl;
        }
    }
    std::ofstream outFile;
    if (!openCsvFileForWrite(filenameWithExtension, outFile, std::ios::out | std::ios::app)) return 1;
    for (int i = 0; i < numInputs; ++i) {
        if (inputs[i]) outFile << inputs[i];
        if (i < numInputs - 1) outFile << ",";
    }
    outFile << "\n"; outFile.close(); return 0;
}

int updateDataByUniqueKey(const std::string& filenameWithExtension, const char* uniqueKeyToUpdate, int uniqueKeyColumnIndex, int numColsInNewData, const char* newDataRow[]) {
    if (!uniqueKeyToUpdate || !newDataRow || uniqueKeyColumnIndex < 0) { std::cerr << "Error: Invalid args for updateDataByUniqueKey." << std::endl; return 1;}

    dataContainer2D currentData = getData(filenameWithExtension);
    if (currentData.error && currentData.y == 0) { std::cerr << "Error: File empty/unreadable for update." << std::endl; deleteDataContainer2D(currentData); return 1; }
    if (currentData.error) { std::cerr << "Error: File read error for update." << std::endl; deleteDataContainer2D(currentData); return 1;}
    if (uniqueKeyColumnIndex >= currentData.x) { std::cerr << "Error: Key index out of bounds." << std::endl; deleteDataContainer2D(currentData); return 1; }
    if (numColsInNewData != currentData.x) { std::cerr << "Error: New data column count mismatch." << std::endl; deleteDataContainer2D(currentData); return 1;}

    bool updated = false;
    for (int i = 0; i < currentData.y; ++i) {
        if (currentData.data[i] && currentData.data[i][uniqueKeyColumnIndex] && strcmp(currentData.data[i][uniqueKeyColumnIndex], uniqueKeyToUpdate) == 0) {
            for (int j = 0; j < currentData.x; ++j) {
                delete[] currentData.data[i][j];
                currentData.data[i][j] = duplicateString(newDataRow[j]);
            }
            updated = true; // Assuming unique key, but could continue if multiple updates allowed
        }
    }
    if (!updated) { std::cerr << "Info: Key '" << uniqueKeyToUpdate << "' not found for update." << std::endl; deleteDataContainer2D(currentData); return 2; }
    int writeResult = writeData(filenameWithExtension, currentData);
    deleteDataContainer2D(currentData); return writeResult;
}

int updateDataBySecondKey(const std::string& filenameWithExtension, const char* primaryKey, int primaryKeyIndex, const char* secondaryKey, int secondaryKeyIndex, int numColsInNewData, const char* newDataRow[]) {
    if (!primaryKey || !secondaryKey || !newDataRow || primaryKeyIndex < 0 || secondaryKeyIndex < 0) { std::cerr << "Error: Invalid args for updateDataBySecondKey." << std::endl; return 1; }

    dataContainer2D currentData = getData(filenameWithExtension);
    if (currentData.error && currentData.y == 0) { std::cerr << "Error: File empty/unreadable for update (second key)." << std::endl; deleteDataContainer2D(currentData); return 1;}
    if (currentData.error) { std::cerr << "Error: File read error for update (second key)." << std::endl; deleteDataContainer2D(currentData); return 1; }
    if (primaryKeyIndex >= currentData.x || secondaryKeyIndex >= currentData.x) { std::cerr << "Error: Key index out of bounds (second key)." << std::endl; deleteDataContainer2D(currentData); return 1; }
    if (numColsInNewData != currentData.x) { std::cerr << "Error: New data column count mismatch (second key)." << std::endl; deleteDataContainer2D(currentData); return 1; }

    bool updated = false;
    for (int i = 0; i < currentData.y; ++i) {
        if (currentData.data[i] && currentData.data[i][primaryKeyIndex] && strcmp(currentData.data[i][primaryKeyIndex], primaryKey) == 0 &&
            currentData.data[i][secondaryKeyIndex] && strcmp(currentData.data[i][secondaryKeyIndex], secondaryKey) == 0) {
            for (int j = 0; j < currentData.x; ++j) {
                delete[] currentData.data[i][j];
                currentData.data[i][j] = duplicateString(newDataRow[j]);
            }
            updated = true;
        }
    }
    if (!updated) { std::cerr << "Info: Record matching keys not found for update (second key)." << std::endl; deleteDataContainer2D(currentData); return 2;}
    int writeResult = writeData(filenameWithExtension, currentData);
    deleteDataContainer2D(currentData); return writeResult;
}

int deleteByKey(const std::string& filenameWithExtension, const char* keyToDelete, int keyColumnIndex) {
    if (!keyToDelete || keyColumnIndex < 0) { std::cerr << "Error: Invalid args for deleteByKey." << std::endl; return 1; }

    dataContainer2D originalData = getData(filenameWithExtension);
    if (originalData.error && originalData.y == 0) { std::cerr << "Error: File empty/unreadable for delete." << std::endl; deleteDataContainer2D(originalData); return 1; }
    if (originalData.error) { std::cerr << "Error: File read error for delete." << std::endl; deleteDataContainer2D(originalData); return 1; }
    if (keyColumnIndex >= originalData.x) { std::cerr << "Error: Key index out of bounds for delete." << std::endl; deleteDataContainer2D(originalData); return 1; }

    dataContainer2D modifiedData;
    modifiedData.x = originalData.x;
    if (originalData.x > 0 && originalData.fields) {
        modifiedData.fields = new char*[modifiedData.x];
        for (int i = 0; i < modifiedData.x; ++i) modifiedData.fields[i] = duplicateString(originalData.fields[i]);
    } else modifiedData.fields = nullptr;


    char*** keptRowsData = (originalData.y > 0) ? new char**[originalData.y] : nullptr;
    int keptRowCount = 0; bool deletionOccurred = false;

    for (int i = 0; i < originalData.y; ++i) {
        if (originalData.data[i] && originalData.data[i][keyColumnIndex] && strcmp(originalData.data[i][keyColumnIndex], keyToDelete) == 0) {
            for (int j = 0; j < originalData.x; ++j) delete[] originalData.data[i][j];
            delete[] originalData.data[i]; originalData.data[i] = nullptr;
            deletionOccurred = true;
        } else {
            if (originalData.data[i] && keptRowsData) keptRowsData[keptRowCount++] = originalData.data[i]; // Transfer pointer
            if (originalData.data[i]) originalData.data[i] = nullptr; // Avoid double free by originalData cleanup
        }
    }
    
    modifiedData.y = keptRowCount;
    if (keptRowCount > 0 && keptRowsData) {
        modifiedData.data = new char**[keptRowCount];
        for(int i=0; i < keptRowCount; ++i) modifiedData.data[i] = keptRowsData[i];
    } else modifiedData.data = nullptr;
    delete[] keptRowsData;

    if(originalData.fields) delete[] originalData.fields; originalData.fields = nullptr; 
    if(originalData.data) delete[] originalData.data; originalData.data = nullptr;

    if (!deletionOccurred) {
        std::cerr << "Info: Key '" << keyToDelete << "' not found for deletion." << std::endl;
        deleteDataContainer2D(modifiedData); return 2;
    }
    int writeResult = writeData(filenameWithExtension, modifiedData);
    deleteDataContainer2D(modifiedData); return writeResult;
}

int deleteBySecondKey(const std::string& filenameWithExtension, const char* primaryKey, int primaryKeyIndex, const char* secondaryKey, int secondaryKeyIndex) {
    if (!primaryKey || !secondaryKey || primaryKeyIndex < 0 || secondaryKeyIndex < 0) { std::cerr << "Error: Invalid args for deleteBySecondKey." << std::endl; return 1;}

    dataContainer2D originalData = getData(filenameWithExtension);
    if (originalData.error && originalData.y == 0) { std::cerr << "Error: File empty/unreadable for delete (second key)." << std::endl; deleteDataContainer2D(originalData); return 1;}
    if (originalData.error) { std::cerr << "Error: File read error for delete (second key)." << std::endl; deleteDataContainer2D(originalData); return 1;}
    if (primaryKeyIndex >= originalData.x || secondaryKeyIndex >= originalData.x) { std::cerr << "Error: Key index out of bounds (second key)." << std::endl; deleteDataContainer2D(originalData); return 1;}

    dataContainer2D modifiedData;
    modifiedData.x = originalData.x;
    if (originalData.x > 0 && originalData.fields) {
        modifiedData.fields = new char*[modifiedData.x];
        for (int i = 0; i < modifiedData.x; ++i) modifiedData.fields[i] = duplicateString(originalData.fields[i]);
    } else modifiedData.fields = nullptr;

    char*** keptRowsData = (originalData.y > 0) ? new char**[originalData.y] : nullptr;
    int keptRowCount = 0; bool deletionOccurred = false;

    for (int i = 0; i < originalData.y; ++i) {
        bool pMatch = originalData.data[i] && originalData.data[i][primaryKeyIndex] && strcmp(originalData.data[i][primaryKeyIndex], primaryKey) == 0;
        bool sMatch = originalData.data[i] && originalData.data[i][secondaryKeyIndex] && strcmp(originalData.data[i][secondaryKeyIndex], secondaryKey) == 0;
        if (pMatch && sMatch) {
            for (int j = 0; j < originalData.x; ++j) delete[] originalData.data[i][j];
            delete[] originalData.data[i]; originalData.data[i] = nullptr;
            deletionOccurred = true;
        } else {
            if (originalData.data[i] && keptRowsData) keptRowsData[keptRowCount++] = originalData.data[i];
            if (originalData.data[i]) originalData.data[i] = nullptr;
        }
    }

    modifiedData.y = keptRowCount;
    if (keptRowCount > 0 && keptRowsData) {
        modifiedData.data = new char**[keptRowCount];
        for(int i=0; i < keptRowCount; ++i) modifiedData.data[i] = keptRowsData[i];
    } else modifiedData.data = nullptr;
    delete[] keptRowsData;

    if(originalData.fields) delete[] originalData.fields; originalData.fields = nullptr;
    if(originalData.data) delete[] originalData.data; originalData.data = nullptr;

    if (!deletionOccurred) {
        std::cerr << "Info: Record matching keys not found for deletion (second key)." << std::endl;
        deleteDataContainer2D(modifiedData); return 2;
    }
    int writeResult = writeData(filenameWithExtension, modifiedData);
    deleteDataContainer2D(modifiedData); return writeResult;
}

dataContainer2D SortByFieldName(dataContainer2D data, const char* fieldName, bool ascending) {
    using namespace CsvToolkit;

    dataContainer2D sorted;
    sorted.error = 1;

    if (data.error || !data.fields || data.x == 0 || data.y == 0) {
        std::cerr << "Error: Invalid data container.\n";
        return sorted;
    }

    // Find column index
    int fieldIndex = -1;
    for (int i = 0; i < data.x; ++i) {
        if (strcmp(data.fields[i], fieldName) == 0) {
            fieldIndex = i;
            break;
        }
    }
    if (fieldIndex == -1) {
        std::cerr << "Error: Field '" << fieldName << "' not found.\n";
        return sorted;
    }

    // Copy field names
    sorted.x = data.x;
    sorted.y = data.y;
    sorted.fields = new char*[sorted.x];
    for (int i = 0; i < sorted.x; ++i)
        sorted.fields[i] = duplicateString(data.fields[i]);

    // Copy data
    sorted.data = new char**[sorted.y];
    for (int i = 0; i < sorted.y; ++i) {
        sorted.data[i] = new char*[sorted.x];
        for (int j = 0; j < sorted.x; ++j) {
            sorted.data[i][j] = duplicateString(data.data[i][j]);
        }
    }

    // Bubble sort (lexical)
    for (int i = 0; i < sorted.y - 1; ++i) {
        for (int j = 0; j < sorted.y - i - 1; ++j) {
            const char* a = sorted.data[j][fieldIndex];
            const char* b = sorted.data[j + 1][fieldIndex];

            bool condition = ascending ? (strcmp(a, b) > 0) : (strcmp(a, b) < 0);
            if (condition) {
                char** temp = sorted.data[j];
                sorted.data[j] = sorted.data[j + 1];
                sorted.data[j + 1] = temp;
            }
        }
    }

    sorted.error = 0;
    return sorted;
}

dataContainer2D sortByTwoFields(dataContainer2D data,const char* primaryField,const char* secondaryField,bool ascending) {
    using namespace CsvToolkit;

    dataContainer2D sorted;
    sorted.error = 1;

    if (data.error || !data.fields || data.x == 0 || data.y == 0) {
        std::cerr << "Error: Invalid data container.\n";
        return sorted;
    }

    // Find primary and secondary column indices
    int primaryIdx = -1, secondaryIdx = -1;
    for (int i = 0; i < data.x; ++i) {
        if (strcmp(data.fields[i], primaryField) == 0) primaryIdx = i;
        if (strcmp(data.fields[i], secondaryField) == 0) secondaryIdx = i;
    }

    if (primaryIdx == -1 || secondaryIdx == -1) {
        std::cerr << "Error: One or both fields not found.\n";
        return sorted;
    }

    // Copy fields
    sorted.x = data.x;
    sorted.y = data.y;
    sorted.fields = new char*[sorted.x];
    for (int i = 0; i < sorted.x; ++i)
        sorted.fields[i] = duplicateString(data.fields[i]);

    // Copy data
    sorted.data = new char**[sorted.y];
    for (int i = 0; i < sorted.y; ++i) {
        sorted.data[i] = new char*[sorted.x];
        for (int j = 0; j < sorted.x; ++j)
            sorted.data[i][j] = duplicateString(data.data[i][j]);
    }

    // Bubble sort with primary + secondary criteria
    for (int i = 0; i < sorted.y - 1; ++i) {
        for (int j = 0; j < sorted.y - i - 1; ++j) {
            const char* a1 = sorted.data[j][primaryIdx];
            const char* b1 = sorted.data[j + 1][primaryIdx];
            const char* a2 = sorted.data[j][secondaryIdx];
            const char* b2 = sorted.data[j + 1][secondaryIdx];

            bool swapNeeded = false;
            int cmpPrimary = strcmp(a1, b1);
            int cmpSecondary = strcmp(a2, b2);

            if (ascending) {
                if (cmpPrimary > 0) swapNeeded = true;
                else if (cmpPrimary == 0 && cmpSecondary > 0) swapNeeded = true;
            } else {
                if (cmpPrimary < 0) swapNeeded = true;
                else if (cmpPrimary == 0 && cmpSecondary < 0) swapNeeded = true;
            }

            if (swapNeeded) {
                char** temp = sorted.data[j];
                sorted.data[j] = sorted.data[j + 1];
                sorted.data[j + 1] = temp;
            }
        }
    }

    sorted.error = 0;
    return sorted;
}

int updateFieldByKey(const std::string& filename, const char* key, const char* fieldName, const char* newValue) {
    dataContainer2D data = getData(filename);
    if (data.error || data.y == 0) {
        std::cerr << "Error loading file or no records.\n";
        return 1;
    }

    int keyIndex = 0; // assumes the key is the first column
    int fieldIndex = -1;

    // Find the index of the field to update
    for (int i = 0; i < data.x; ++i) {
        if (data.fields[i] && strcmp(data.fields[i], fieldName) == 0) {
            fieldIndex = i;
            break;
        }
    }

    if (fieldIndex == -1) {
        std::cerr << "Field '" << fieldName << "' not found.\n";
        deleteDataContainer2D(data);
        return 2;
    }

    bool found = false;
    for (int i = 0; i < data.y; ++i) {
        if (data.data[i][keyIndex] && strcmp(data.data[i][keyIndex], key) == 0) {
            delete[] data.data[i][fieldIndex]; // delete old value
            data.data[i][fieldIndex] = duplicateString(newValue);
            found = true;
            break;
        }
    }

    if (!found) {
        std::cerr << "Key '" << key << "' not found.\n";
        deleteDataContainer2D(data);
        return 3;
    }

    int writeResult = writeData(filename, data);
    deleteDataContainer2D(data);
    return writeResult;
}
int writeRow(const std::string& filenameWithExtension, int numCols, char** rowData) {
        if (!rowData || numCols <= 0) {
            std::cerr << "Error: Invalid arguments for writeRow.\n";
            return 1;
        }

        std::ofstream outFile;
        if (!openCsvFileForWrite(filenameWithExtension, outFile, std::ios::out | std::ios::app)) {
            return 1;
        }

        for (int i = 0; i < numCols; ++i) {
            if (rowData[i]) outFile << rowData[i];
            if (i < numCols - 1) outFile << ",";
        }
        outFile << "\n";
        outFile.close();
        return 0;
    }

dataContainer2D Search1FieldValue(const std::string& filenameWithExtension, const char* fieldName, const char* dataToMatch) {
    dataContainer2D fullData = getData(filenameWithExtension);
    dataContainer2D result;
    result.error = 1;

    if (fullData.error || !fieldName || !dataToMatch) {
        std::cerr << "Search1FieldValue: Error loading data or invalid arguments.\n";
        deleteDataContainer2D(fullData);
        return result;
    }

    // Find the index of the requested field
    int fieldIndex = -1;
    for (int i = 0; i < fullData.x; ++i) {
        if (strcmp(fullData.fields[i], fieldName) == 0) {
            fieldIndex = i;
            break;
        }
    }

    if (fieldIndex == -1) {
        std::cerr << "Search1FieldValue: Field '" << fieldName << "' not found.\n";
        deleteDataContainer2D(fullData);
        return result;
    }

    // Prepare result container
    result.x = fullData.x;
    result.fields = new char*[result.x];
    for (int i = 0; i < result.x; ++i)
        result.fields[i] = duplicateString(fullData.fields[i]);

    result.data = new char**[fullData.y];
    result.y = 0;

    for (int i = 0; i < fullData.y; ++i) {
        if (strcmp(fullData.data[i][fieldIndex], dataToMatch) == 0) {
            result.data[result.y] = new char*[result.x];
            for (int j = 0; j < result.x; ++j)
                result.data[result.y][j] = duplicateString(fullData.data[i][j]);
            result.y++;
        }
    }

    result.error = 0;
    deleteDataContainer2D(fullData);
    return result;
}

dataContainer2D Search2FieldValue(const std::string& file, const char* field1, const char* key1, const char* field2, const char* key2) {
    dataContainer2D full = getData(file);
    if (full.error) { dataContainer2D empty; empty.error = 1; return empty; }

    int index1 = -1, index2 = -1;
    for (int i = 0; i < full.x; ++i) {
        if (strcmp(full.fields[i], field1) == 0) index1 = i;
        if (strcmp(full.fields[i], field2) == 0) index2 = i;
    }
    if (index1 == -1 || index2 == -1) {
        deleteDataContainer2D(full);
        dataContainer2D empty; empty.error = 1; return empty;
    }

    dataContainer2D result;
    result.x = full.x;
    result.y = 0;
    result.fields = new char*[full.x];
    for (int i = 0; i < full.x; ++i)
        result.fields[i] = duplicateString(full.fields[i]);

    result.data = new char**[full.y];

    for (int i = 0; i < full.y; ++i) {
        if (strcmp(full.data[i][index1], key1) == 0 && strcmp(full.data[i][index2], key2) == 0) {
            result.data[result.y] = new char*[result.x];
            for (int j = 0; j < result.x; ++j)
                result.data[result.y][j] = duplicateString(full.data[i][j]);
            result.y++;
        }
    }

    deleteDataContainer2D(full);
    return result;
}

} // namespace CsvToolkit

#endif // CSV_TOOLKIT_HPP