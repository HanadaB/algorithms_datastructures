#include "BLAST_Neighborhood.hpp" // Einbinden der Header-Datei für die BLAST_Neighborhood-Klasse.
#include <array>                  // Einbinden der Standardbibliothek für Array-Datenstrukturen.
#include <math.h>                 // Einbinden der Mathematik-Bibliothek für mathematische Funktionen.
#include <algorithm>              // Einbinden der Bibliothek für Algorithmen wie Sortierfunktionen.
#include <omp.h>                  // Einbinden der OpenMP-Bibliothek für parallele Programmierung.

// Definition der Methode generateNeighborhood der Klasse BLAST_Neighborhood.
std::vector<NHResult> BLAST_Neighborhood::generateNeighborhood(const std::string& query,
                                                               const ScoreMatrix& matrix,
                                                               const int word_size,
                                                               const int score_threshold,
                                                               const int threads) {
    // Überprüfung der Anzahl der Threads.
    if (threads < 1) {
        throw std::invalid_argument("Die Anzahl der Threads muss >= 1 sein.");
    }
    // Überprüfung der Wortgröße.
    if (word_size < 1) {
        throw std::invalid_argument("Die Wortgröße muss >= 1 sein.");
    }
    // Rückgabe eines leeren Vektors, wenn die Wortgröße größer als die Länge der Abfrage ist.
    if (word_size > static_cast<int>(query.length())) {
        return {};
    }

    // Initialisierung eines Vektors für die Anfangswörter der Abfrage.
    std::vector<std::string> initial_words;
    initial_words.resize(query.size() - word_size + 1);

    // Erzeugen der Anfangswörter aus der Abfragesequenz.
    for (int i = 0; i < static_cast<int>(query.size() - word_size + 1); i++) {
        initial_words[i] = query.substr(i, word_size);
    }

    // Initialisierung des Ergebnisvektors.
    std::vector<NHResult> result;
    result.resize(query.size() - word_size + 1);

    // Definition des Alphabets.
    std::array<char, 20> Sigma = { 'A', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'Y' };

    // Parallele Berechnung mithilfe von OpenMP, falls mehr als ein Thread verwendet wird.
    #pragma omp parallel if(threads > 1) num_threads(threads)
    #pragma omp for
    for (int i = 0; i < static_cast<int>(initial_words.size()); i++) {
        // Initialisierung des Vektors für Nachbarn.
        std::vector< std::pair <std::string, int>> neighbors;
        // Initialisierung des Kandidatenvektors.
        std::vector<std::string> candidates(1, "");

        // Iteration über jede Position im Wort.
        for(int x = 0; x < word_size; x++) {
            // Erweitern des Kandidatenvektors, falls nicht das letzte Zeichen erreicht ist.
            if (x != word_size - 1) {
                candidates.resize(static_cast<size_t>(pow(20, x + 1)));
            }
            // Erstellen einer temporären Kopie der Kandidatenliste.
            std::vector<std::string> temp = candidates;

            // Iteration über die Kandidatenliste.
            for (int y = 0; y < static_cast<int>(pow(20, x)); y++) {
                // Falls es das letzte Zeichen ist, werden endgültige Kandidaten generiert.
                if (x == word_size - 1) {
                    // Iteration über jedes Zeichen im Alphabet.
                    for (char AS : Sigma) {
                        int score = 0; // Initialisierung des Scores.
                        // Erzeugen eines Kandidaten.
                        std::string candidate = candidates[y] + AS;
                        // Berechnen des Scores für den Kandidaten.
                        for (int j = 0; j < word_size; j++) {
                            score += matrix.score(initial_words[i][j], candidate[j]);
                        }
                        // Hinzufügen des Kandidaten, falls der Score über dem Schwellenwert liegt.
                        if (score >= score_threshold) {
                            neighbors.push_back(std::make_pair(candidate, score));
                        }
                    }
                } else {
                    // Für andere Zeichenpositionen, neue Kandidaten generieren.
                    for (int z = 0; z < 20; z++) {
                        candidates[y * 20 + z] = temp[y] + Sigma[z];
                    }
                }
            }
        }

        // Sortieren der Nachbarn alphabetisch.
        std::sort(neighbors.begin(), neighbors.end());

        // Speichern des Infixes und der Nachbarn im Ergebnisvektor.
        result[i].infix = initial_words[i];
        result[i].neighbors = neighbors;
    }

    // Rückgabe des Ergebnisvektors.
    return result;
}
