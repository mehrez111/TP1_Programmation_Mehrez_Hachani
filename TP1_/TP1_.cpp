#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

class AnalyseurDeFichier {
private:
    vector<string> mots;

    void triParInsertion() {
        for (size_t i = 1; i < mots.size(); ++i) {
            string temp = mots[i];
            int j = i - 1;
            while (j >= 0 && mots[j] > temp) {
                mots[j + 1] = mots[j];
                --j;
            }
            mots[j + 1] = temp;
        }
    }

    bool rechercherRecursivement(int debut, int fin, const string& cible) {
        if (debut > fin) return false;
        int milieu = debut + (fin - debut) / 2;

        if (mots[milieu] == cible) return true;
        if (cible < mots[milieu])
            return rechercherRecursivement(debut, milieu - 1, cible);
        else
            return rechercherRecursivement(milieu + 1, fin, cible);
    }

public:
    bool chargerFichier(const string& nomFichier) {
        cout << "Tentative d'ouverture du fichier : [" << nomFichier << "]" << endl;

        ifstream fichier(nomFichier);
        if (!fichier.is_open()) {
            cerr << "❌ Erreur : impossible d'ouvrir le fichier [" << nomFichier << "]" << endl;
            return false;
        }

        mots.clear();
        string ligne, mot;
        while (getline(fichier, ligne)) {
            stringstream ss(ligne);
            while (ss >> mot) {
                mots.push_back(mot);
            }
        }

        fichier.close();
        cout << "✅ Fichier chargé avec succès." << endl;
        return true;
    }

    void trierEtSauvegarder(const string& nomFichierOriginal) {
        triParInsertion();

        string baseNom = nomFichierOriginal;
        size_t point = nomFichierOriginal.find('.');
        if (point != string::npos)
            baseNom = nomFichierOriginal.substr(0, point);

        string nomFichierFinal = baseNom + "_Trie.txt";
        cout << "📝 Tentative de création du fichier : " << nomFichierFinal << endl;

        ofstream sortie(nomFichierFinal);
        if (!sortie.is_open()) {
            cerr << "❌ Erreur lors de la création du fichier trié [" << nomFichierFinal << "]" << endl;
            return;
        }

        for (const auto& mot : mots) {
            sortie << mot << endl;
        }

        sortie.close();
        cout << "✅ Fichier trié enregistré : " << nomFichierFinal << endl;
    }

    bool TrouverMot(string motATrouver) {
        return rechercherRecursivement(0, mots.size() - 1, motATrouver);
    }
};

int main() {
    AnalyseurDeFichier analyseur;
    string nomFichierTexte;

    cout << "Nom du fichier à analyser (ex: Texte.txt) : ";
    cin >> nomFichierTexte;

    if (!analyseur.chargerFichier(nomFichierTexte)) {
        return 1;
    }

    analyseur.trierEtSauvegarder(nomFichierTexte);

    string motRecherche;
    while (true) {
        cout << "\nTape un mot à chercher (ou 'exit' pour quitter) : ";
        cin >> motRecherche;

        if (motRecherche == "exit") break;

        if (analyseur.TrouverMot(motRecherche)) {
            cout << "✅ Le mot \"" << motRecherche << "\" a été trouvé." << endl;
        }
        else {
            cout << "❌ Le mot \"" << motRecherche << "\" est introuvable." << endl;
        }
    }

    return 0;
}
