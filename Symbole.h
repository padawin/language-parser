#ifndef SYMBOLE_H_
#define SYMBOLE_H_

#include <iostream>
#include <vector>
#include <string>
using namespace std;

#define FICHIER_MOTS_CLE "motsCles.txt"

class Symbole {
public:
	enum categorieSymbole { MOTCLE, VARIABLE, ENTIER, INDEFINI, FINDEFICHIER, CHAINE };
	Symbole(string s=string("")); // construit le symbole à partir de la chaine (string) ch lue par le lecteur de symbole dans un fichier
	Symbole(const char *ch);             // construit le symbole à partir d'une chaine (char*) passée en paramètre
	bool operator == (const char * ch) ; // pour tester l'égalité entre le symbole et une chaîne
	inline bool operator != (const char * ch) { return ! (*this == ch); } // pour tester la différence...
	inline string getChaine()           { return this->chaine;}     // accesseur

	friend ostream & operator <<(ostream & cout, Symbole symb);     // fonction amie pour pouvoir afficher un symbole sur cout

private:
	string           chaine;              // chaine du symbole
	categorieSymbole categorie;           // categorie du symbole (voir type énuméré ci-dessus)
	static bool      isMotCle(string s);  // méthode de classe : renvoie vrai si s est un mot clé du langage
};

#endif /* SYMBOLE_H_ */
