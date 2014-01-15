#ifndef LECTEURPHRASEAVECARBRE_H_
#define LECTEURPHRASEAVECARBRE_H_

#include "Symbole.h"
#include "LecteurSymbole.h"
#include "TableSymboles.h"
#include "Arbre.h"

class LecteurPhraseAvecArbre {
public:
	LecteurPhraseAvecArbre(char nomFich[]); // Construit un lecteur de phrase pour interpreter
	//  le programme dans le fichier nomFich

	void analyse(); // Si le contenu du fichier est conforme à la grammaire,
	//   cette méthode se termine normalement et affiche un message "Syntaxe correcte".
	//   la table des symboles (ts) et l'arbre abstrait (arbre) auront été construits
	// Sinon, le programme sera interrompu (exit).

	inline TableSymboles getTs() {
		return ts;
	} // accesseur
	inline Noeud* getArbre() {
		return arbre;
	} // accesseur
	void generCode();

private:
	LecteurSymbole ls; // le lecteur de symboles utilisé pour analyser le fichier
	TableSymboles ts; // la table des symboles valués
	Noeud* arbre; // l'arbre abstrait

	// implémentation de la grammaire
	Noeud* programme(); //   <programme> ::= debut <seqInst> fin FIN_FICHIER
	Noeud* seqInst(); //     <seqInst> ::= <inst> ; { <inst> ; }
	Noeud* inst(); //        <inst> ::= <affectation>
	Noeud* affectation(); // <affectation> ::= <variable> = <expression>
	Noeud* expression(); //  <expression> ::= <facteur> { <opBinaire> <facteur> }
	Noeud* facteur(); //     <facteur> ::= <entier>  |  <variable>  |  - <facteur>  |  ( <expression> )
	Symbole opBinaire(); //   <opBinaire> ::= + | - | *  | /
	Noeud* terme(); // <terme> ::=<facteur> { <opMult> <facteur> }
	Symbole opAdd(); // <opAdd> ::= + | -
	Symbole opMult(); // <opMult> ::= * | /
	Noeud* expBool(); // <expBool> ::= <relation> {<opBool> <relation> }
	Symbole opBool(); // <opBool> ::= et | ou
	Noeud* relation(); // <relation> ::= <expression> { <opRel> <expression>}
	Symbole opRel(); // <opRel> ::= == | != | < | <= | > | >=
	Symbole opUnaire(); // <opUnaire> ::= - | non
	Noeud* instSi(); // si (<expBool> ) <seqInst> {sinonsi (<expBool>) <seqInst> }
					//	[sinon <seqInst> ] finsi
	Noeud* instTq(); // tantque ( <expBool> ) <seqInst> fintantque
	Noeud* instRepeter(); // repeter <seqInst> jusqua ( <expBool> )
	Noeud* instLire(); // lire ( <variable> )
	Noeud* instEcrire(); // ecrire ( <expression> | <chaine> )
	Noeud* instPour(); // pour
	Noeud* instSwitch(); //
	// outils pour se simplifier l'analyse syntaxique
	void testerSymCour(const char *ch); // si symbole courant != ch, erreur : on arrete le programme, sinon rien
	void sauterSymCour(const char *ch); // si symbole courant == ch, on passe au symbole suivant, sinon erreur : on arrete
	void erreur(const char mess[]); // affiche les message d'erreur mess et arrete le programme

};

#endif /* LECTEURPHRASEAVECARBRE_H_ */
