#ifndef LECTEURPHRASEAVECTABLE_H_
#define LECTEURPHRASEAVECTABLE_H_

#include "LecteurSymbole.h"
#include "TableSymboles.h"

class LecteurPhraseAvecTable {
public:
	LecteurPhraseAvecTable(char nomFich[]);	// Construit un lecteur de phrase pour vérifier
	                                        // la syntaxe du programme dans le fichier nomFich

	void analyse();   // Si le contenu du fichier est conforme à la grammaire,
	                  //   cette méthode se termine normalement et affiche un message "Syntaxe correcte".
                          //   la table des symboles (ts) aura été construite
	                  // Sinon, le programme sera interrompu (exit).
	inline TableSymboles getTs ()   { return ts; } // accesseur

private:
    LecteurSymbole ls;  // le lecteur de symboles utilisé pour analyser le fichier
    TableSymboles  ts;  // la table des symboles valués

    // implémentation de la grammaire
    void programme();   //   <programme> ::= debut <seqInst> fin <EOF>
        void seqInst();	    //     <seqIns>  ::= <inst> ; { <inst> ; }
        void inst();	    //        <inst> ::= <affectation>
        void affectation();	// <affectation> ::= <variable> = <expression>
        void expression();	//  <expression> ::= <facteur> { <opBinaire> <facteur> }
        void facteur();     //     <facteur> ::= <entier>  |  <variable>  |  - <facteur>  |  ( <expression> )
        void opBinaire();   //   <opBinaire> ::= + | - | *  | /
        void terme();       // <terme> ::=<facteur> { <opMult> <facteur> }
        void opAdd();       // <opAdd> ::= + | -
        void opMult();      // <opMult> ::= * | /
        void expBool();     // <expBool> ::= <relation> {<opBool> <relation> }
        void opBool();      // <opBool> ::= et | ou
        void relation();    // <relation> ::= <expression> { <opRel> <expression>}
        void opRel();       // <opRel> ::= == | != | < | <= | > | >=
        void opUnaire();    // <opUnaire> ::= - | non
        void instSi();      // si (<expBool> ) <seqInst> {sinonsi (<expBool>) <seqInst> }
           					//	[sinon <seqInst> ] finsi
        void instTq();      // tantque ( <expBool> ) <seqInst> fintantque
        void instRepeter(); // repeter <seqInst> jusqua ( <expBool> )
        void instLire();	// lire ( <variable> )
        void instEcrire();	// ecrire ( <expression> | <chaine> )
        void instPour() ;
        void instSwitch();
        // outils pour se simplifier l'analyse syntaxique
        void testerSymCour (char *ch);  // si symbole courant != ch, erreur : on arrete le programme, sinon rien
        void sauterSymCour (char *ch);  // si symbole courant == ch, on passe au symbole suivant, sinon erreur : on arrete
        void erreur (char mess[]);      // affiche les message d'erreur mess et arrete le programme
};

#endif /* LECTEURPHRASEAVECTABLE_H_ */
