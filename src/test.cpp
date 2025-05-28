#include <iostream>
#include "File_exe.hpp"

using namespace CsvToolkit;
using namespace std;

int main() {

    const string filename = "tournament_stages.csv";

    dataContainer2D tournamentData = getData(filename);

    clearTerminal();

    cout << "\nCSV Data from: " << filename << endl;
    
    displayTabulatedData(tournamentData);

    deleteDataContainer2D(tournamentData);

    return 0;
}