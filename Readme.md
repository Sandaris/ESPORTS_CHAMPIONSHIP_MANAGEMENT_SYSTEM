# CSV Toolkit Library Function Reference

## Data Structures

### `dataContainer2D`
Stores 2D CSV data with headers and rows.
```cpp
struct dataContainer2D {
    int error;        // Error flag (0 = success, 1 = error)
    char** fields;    // Column headers
    char*** data;     // 2D data array [row][column]
    int y;           // Number of data rows
    int x;           // Number of columns
};
```

### `dataContainer1D`
Stores 1D data (single column or row).
```cpp
struct dataContainer1D {
    int error;           // Error flag
    char** fields;       // Field names
    int fields_count;    // Number of field names
    char** data;         // 1D data array
    int x;              // Number of elements
};
```

## Memory Management Functions

### `deleteDataContainer2D(dataContainer2D& container)`
Safely deallocates all memory used by a 2D container.
```cpp
dataContainer2D data = getData("employees.csv");
// ... use data ...
deleteDataContainer2D(data); // Clean up memory
```

### `deleteDataContainer1D(dataContainer1D& container)`
Safely deallocates all memory used by a 1D container.
```cpp
dataContainer1D fieldData = queryField("employees.csv", "name");
// ... use fieldData ...
deleteDataContainer1D(fieldData); // Clean up memory
```

## Core CSV Parsing Functions

### `parseCsvRow(const std::string& line, char delimiter, int expectedFields, int& actualTokenCount)`
Parses a single CSV row into an array of strings.
```cpp
std::string csvLine = "John,Doe,30,Engineer";
int tokenCount;
char** tokens = parseCsvRow(csvLine, ',', 4, tokenCount);
// tokens[0] = "John", tokens[1] = "Doe", etc.
```

### `getData(const std::string& filenameWithExtension)`
Loads complete CSV file into a 2D data container.
```cpp
dataContainer2D employees = getData("employees.csv");
if (!employees.error) {
    // Successfully loaded CSV data
    std::cout << "Loaded " << employees.y << " rows with " << employees.x << " columns\n";
}
```

## Data Manipulation Functions

### `concatDataContainer(const dataContainer2D& data1, const dataContainer2D& data2, const char* keyField1, const char* keyField2)`
Joins two datasets based on matching key fields (like SQL JOIN).
```cpp
dataContainer2D employees = getData("employees.csv");
dataContainer2D departments = getData("departments.csv");
dataContainer2D joined = concatDataContainer(employees, departments, "dept_id", "id");
// Joins employees with departments where employees.dept_id = departments.id
```

### `filterDataContainer(const dataContainer2D& data, const char* field, const char* key)`
Filters data to rows where specified field matches the key value.
```cpp
dataContainer2D allEmployees = getData("employees.csv");
dataContainer2D engineers = filterDataContainer(allEmployees, "position", "Engineer");
// Returns only rows where position = "Engineer"
```

### `getFieldValues(const dataContainer2D& data, const char* field)`
Extracts all values from a specific column.
```cpp
dataContainer2D employees = getData("employees.csv");
dataContainer1D names = getFieldValues(employees, "name");
// Returns 1D container with all employee names
```

### `shortenDataContainer(const dataContainer2D& data, const char* wantedFields[], int numFields)`
Creates a subset with only specified columns.
```cpp
dataContainer2D fullData = getData("employees.csv");
const char* wantedCols[] = {"name", "salary", "department"};
dataContainer2D subset = shortenDataContainer(fullData, wantedCols, 3);
// Returns data with only name, salary, and department columns
```

## Display Functions

### `displayTabulatedData(const dataContainer2D& data)`
Prints data in a formatted table with borders.
```cpp
dataContainer2D employees = getData("employees.csv");
displayTabulatedData(employees);
// Outputs:
// |-------|-------|-------|
// | Name  | Age   | Dept  |
// |-------|-------|-------|
// | John  | 30    | IT    |
// |-------|-------|-------|
```

### `displayUnorderedOptions(const char* header, const char* options[], int noOptions)`
Displays a list of options in a bordered box.
```cpp
const char* menuItems[] = {"Add Employee", "View Employees", "Delete Employee"};
displayUnorderedOptions("Employee Management", menuItems, 3);
```

### `displayOptions(const char* header, const char* options[], int noOptions, bool freeModified = true)`
Displays numbered options for menu selection.
```cpp
const char* actions[] = {"Create", "Read", "Update", "Delete"};
displayOptions("CRUD Operations", actions, 4);
// Outputs:
// 1. Create
// 2. Read
// 3. Update
// 4. Delete
```

### `displayMenu(const char* header, const char* options[], int noOptions)`
Interactive menu that returns user's choice.
```cpp
const char* choices[] = {"Save", "Load", "Exit"};
int selection = displayMenu("File Operations", choices, 3);
// Returns 1, 2, or 3 based on user input
```

### `displaySystemMessage(const char* message, int waitTimeInSeconds)`
Shows a message, waits, then clears screen.
```cpp
displaySystemMessage("Data saved successfully!", 2);
// Shows message for 2 seconds, then clears
```

## User Input Functions

### `getString(const char* prompt)`
Gets string input from user with prompt.
```cpp
char* name = getString("Enter employee name: ");
// User types: "John Smith"
// name = "John Smith"
delete[] name; // Remember to free memory
```

### `getInt(const char* prompt)`
Gets integer input with validation.
```cpp
int age = getInt("Enter age: ");
// Validates input is a valid integer
// Returns -1 on error
```

### `getFloat(const char* prompt)`
Gets float input with validation.
```cpp
float salary = getFloat("Enter salary: ");
// Validates input is a valid float
// Returns -1.0f on error
```

## Query Functions (Data Reading)

### `queryKey(const std::string& filenameWithExtension, const char* key)`
Finds row where first column matches the key.
```cpp
dataContainer1D employee = queryKey("employees.csv", "EMP001");
// Returns entire row for employee with ID "EMP001"
```

### `queryField(const std::string& filenameWithExtension, const char* field)`
Extracts all values from specified column.
```cpp
dataContainer1D salaries = queryField("employees.csv", "salary");
// Returns all salary values from the file
```

### `queryFieldStrict(const std::string& filenameWithExtension, const char* field, const char* key)`
Returns all rows where specified field matches key.
```cpp
dataContainer2D managers = queryFieldStrict("employees.csv", "position", "Manager");
// Returns all rows where position = "Manager"
```

## Data Writing Functions

### `openCsvFileForWrite(const std::string& filename, std::ofstream& outFileStream, std::ios_base::openmode mode)`
Opens CSV file for writing with proper path handling.
```cpp
std::ofstream file;
if (openCsvFileForWrite("output.csv", file, std::ios::out)) {
    file << "Name,Age,Department\n";
    file << "John,30,IT\n";
    file.close();
}
```

### `writeData(const std::string& filename, const dataContainer2D& containerToWrite)`
Writes entire 2D container to CSV file.
```cpp
dataContainer2D modifiedData = filterDataContainer(originalData, "active", "true");
int result = writeData("active_employees.csv", modifiedData);
// result = 0 on success, 1 on error
```

### `writeNewDataRow(const std::string& filename, int numInputs, const char* inputs[])`
Appends new row to existing CSV file.
```cpp
const char* newEmployee[] = {"Jane", "Smith", "25", "Marketing"};
int result = writeNewDataRow("employees.csv", 4, newEmployee);
// Appends new employee to file
```

## Data Update Functions

### `updateDataByUniqueKey(filename, keyToUpdate, keyColumnIndex, numCols, newDataRow[])`
Updates row based on unique key match.
```cpp
const char* updatedData[] = {"John", "Doe", "31", "Senior Engineer"};
int result = updateDataByUniqueKey("employees.csv", "EMP001", 0, 4, updatedData);
// Updates employee EMP001's information
```

### `updateDataBySecondKey(filename, primaryKey, primaryIndex, secondaryKey, secondaryIndex, numCols, newDataRow[])`
Updates row using two key fields for matching.
```cpp
const char* newData[] = {"John", "Doe", "32", "Team Lead"};
int result = updateDataBySecondKey("employees.csv", "John", 0, "Doe", 1, 4, newData);
// Updates row where first_name="John" AND last_name="Doe"
```

## Data Deletion Functions

### `deleteByKey(const std::string& filename, const char* keyToDelete, int keyColumnIndex)`
Deletes rows matching key in specified column.
```cpp
int result = deleteByKey("employees.csv", "EMP001", 0);
// Deletes employee with ID "EMP001"
// Returns: 0=success, 1=error, 2=key not found
```

### `deleteBySecondKey(filename, primaryKey, primaryIndex, secondaryKey, secondaryIndex)`
Deletes rows matching both key conditions.
```cpp
int result = deleteBySecondKey("employees.csv", "John", 0, "Temp", 3);
// Deletes rows where name="John" AND status="Temp"
```

## Utility Functions

### `clearTerminal()`
Clears the terminal screen using ANSI codes.
```cpp
clearTerminal(); // Clears screen and moves cursor to top-left
```

## Complete Usage Example

```cpp
#include "CSV_TOOLKIT_HPP"
using namespace CsvToolkit;

int main() {
    // Load data
    dataContainer2D employees = getData("employees.csv");
    if (employees.error) {
        std::cerr << "Failed to load data\n";
        return 1;
    }
    
    // Display all data
    displayTabulatedData(employees);
    
    // Filter for managers
    dataContainer2D managers = filterDataContainer(employees, "position", "Manager");
    std::cout << "Found " << managers.y << " managers\n";
    
    // Add new employee
    const char* newEmp[] = {"Alice", "Johnson", "28", "Developer"};
    writeNewDataRow("employees.csv", 4, newEmp);
    
    // Update existing employee
    const char* updated[] = {"Bob", "Smith", "35", "Senior Developer"};
    updateDataByUniqueKey("employees.csv", "EMP002", 0, 4, updated);
    
    // Clean up memory
    deleteDataContainer2D(employees);
    deleteDataContainer2D(managers);
    
    return 0;
}
```

## Important Notes

1. **File Location**: CSV files are expected in a `data/` directory relative to the executable
2. **Memory Management**: Always call `deleteDataContainer2D()` or `deleteDataContainer1D()` to prevent memory leaks
3. **Error Handling**: Check the `error` field in containers before using data
4. **String Memory**: Functions like `getString()` return dynamically allocated strings that need `delete[]`
5. **CSV Format**: Assumes comma-separated values with proper escaping for quotes
6. **Path Handling**: Uses C++17 filesystem features for robust path management