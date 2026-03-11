#include <iostream>       
#include <fstream>        
#include <string>         
#include "QGramIndex.hpp" 

using namespace std;     // Verwendet den Standardnamensraum

int main(int argc, const char* argv[]) {
    // Überprüft die Anzahl der Argumente
    if (argc != 3) {
        cerr << "Korrekte Nutzung: ./aufgabe5_main <GENOME FILE> <QUERY>" << endl;
        return 1;  // Beendet das Programm mit Fehlercode, wenn Argumente fehlen
    }

    string filename = argv[1];  // Speichert den Dateinamen aus den Argumenten
    ifstream filestream(filename);  // Öffnet einen Stream zur Datei
    if (!filestream) {
        cerr << "Fehler beim Öffnen der Datei: " << filename << endl;
        return 1;  // Beendet das Programm, wenn Datei nicht geöffnet werden kann
    }

    string genome;  // Deklariert einen String für das Genom
    getline(filestream, genome);  // Liest das Genom aus der Datei
    filestream.close();  // Schließt den Dateistream
    cout << "Genom: " << genome << endl;  // Gibt das gelesene Genom aus

    string query = argv[2];  // Speichert die Suchanfrage aus den Argumenten
    QGramIndex index(genome, query.length());  // Erstellt ein QGramIndex-Objekt

    // Ruft die Methode getHits auf, um die Treffer für die Suchanfrage zu finden
    vector<uint32_t> hits = index.getHits(index.hash(query));
    cout << "Treffer für '" << query << "': ";
    // Gibt jeden Treffer aus
    for (uint32_t hit : hits) {
        cout << hit << " ";
    }
    cout << endl;  

    return 0;  
}
