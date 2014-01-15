#include "LecteurPhraseSimple.h"
#include <stdlib.h>

#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
LecteurPhraseSimple::LecteurPhraseSimple(char nomFich[]) :
	ls(nomFich) {
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::analyse() {
	programme();
	cout << "Syntaxe correcte." << endl;
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::programme() {
	// <programme> ::= debut <seqInst> fin FIN_FICHIER

	sauterSymCour("debut");
	seqInst();
	sauterSymCour("fin");
	testerSymCour("<FINDEFICHIER>");
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::seqInst() {
	// <seqInst> ::= <inst> ; { <inst> ; }

	do {
		inst();



		sauterSymCour(";");

	} while (ls.getSymCour()=="<VARIABLE>" || ls.getSymCour()=="si"
			|| ls.getSymCour()=="tantque" || ls.getSymCour()=="repeter"
			|| ls.getSymCour()=="ecrire" || ls.getSymCour()=="lire"||
			ls.getSymCour()=="pour" || ls.getSymCour() == "switch");

	// tant que le symbole courant est un debut possible d'instruction...


}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::inst() {
	// <inst> ::= <affectation> | <instSi> | <instTq> | <instRepeter> | <instPour>

	if (ls.getSymCour()=="si")
		instSi();
	else if (ls.getSymCour()=="tantque")
		instTq();
	else if (ls.getSymCour()=="repeter")
		instRepeter();
	else if (ls.getSymCour()=="<VARIABLE>")
		affectation();
	else if (ls.getSymCour()=="ecrire")
		instEcrire();
	else if (ls.getSymCour()=="lire")
		instLire();
	else if (ls.getSymCour()=="pour")
		instPour();
	else if (ls.getSymCour() == "switch")
		instSwitch();
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::affectation() {
	// <affectation> ::= <variable> = <expression>

	sauterSymCour("<VARIABLE>");
	sauterSymCour("=");
	expression();
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::expression() {
	// <expression> ::= <terme> { <opAdd> <terme> }

	terme();
	while (ls.getSymCour()=="+" || ls.getSymCour()=="-") {
		opAdd();
		terme();
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::facteur() {
	// <facteur> ::= <entier>  |  <variable>  | <opUnaire> <expBool> |  ( <expBool> )

	if (ls.getSymCour()=="<VARIABLE>" || ls.getSymCour()=="<ENTIER>")
		ls.suivant();
	else if (ls.getSymCour()=="-" || ls.getSymCour()=="non") {
		ls.suivant();
		opUnaire();
		expBool();
	} else if (ls.getSymCour()=="(") {
		ls.suivant();
		expBool();
		sauterSymCour(")");
	} else
		erreur("<facteur>");
}

////////////////////////////////////////////////////////////////////////////////
/*void LecteurPhraseSimple::opBinaire()
 // <opBinaire> ::= + | - | *  | /
 {
 if (ls.getSymCour()=="+" || ls.getSymCour()=="-" ||
 ls.getSymCour()=="*" || ls.getSymCour()=="/")
 ls.suivant();
 else
 erreur("<opBinaire>");
 }
 */
////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::testerSymCour(const char * ch) {
	if (ls.getSymCour() != ch) {
		cout << endl << "-------- Erreur ligne " << ls.getLigne()
				<< " - Colonne " << ls.getColonne() << endl << "   Attendu : "
				<< ch << endl << "   Trouve  : " << ls.getSymCour() << endl
				<< endl;
		exit(0); // plus tard, on levera une exception
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::sauterSymCour(const char * ch) {
	testerSymCour(ch);
	ls.suivant();
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::erreur(const char mess[]) {
	cout << endl << "-------- Erreur ligne " << ls.getLigne() << " - Colonne "
			<< ls.getColonne() << endl << "   Attendu : " << mess << endl
			<< "   Trouve  : " << ls.getSymCour() << endl << endl;
	exit(0); // plus tard, on levera une exception
}

///////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::terme() {
	// <terme> ::=<facteur> { <opMult> facteur }

	facteur();
	while (ls.getSymCour()=="*" || ls.getSymCour()=="/") {
		opMult();
		facteur();
	}

}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::opAdd() {
	// <opAdd> ::= + | -

	if (ls.getSymCour()=="+" || ls.getSymCour()=="-")
		ls.suivant();
	else
		erreur("<opAdd>");

}
////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::opMult() {
	// <opMult> ::= * | /

	if (ls.getSymCour()=="*" || ls.getSymCour()=="/")
		ls.suivant();
	else
		erreur("<opMult>");

}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::expBool() {
	// <expBool> ::= <relation> {<opBool> <relation> }

	relation();
	while (ls.getSymCour()=="et" || ls.getSymCour()=="ou") {
		opBool();
		relation();
	}

}
////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::opBool() {
	// <opBool> ::= et | ou

	if (ls.getSymCour()=="et" || ls.getSymCour()=="ou")
		ls.suivant();
	else
		erreur("<opBool>");

}
////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::relation() {
	// <relation> ::= <expression> { <opRel> <expression>}

	expression();
	while (ls.getSymCour()=="==" || ls.getSymCour()=="!=" || ls.getSymCour()
			=="<" || ls.getSymCour()=="<=" || ls.getSymCour()==">"
			|| ls.getSymCour()==">=") {
		opRel();
		expression();
	}

}
////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::opRel() {
	// <opRel> ::= == | != | < | <= | > | >=

	if (ls.getSymCour()=="==" || ls.getSymCour()=="!=" || ls.getSymCour()=="<"
			|| ls.getSymCour()=="<=" || ls.getSymCour()==">" || ls.getSymCour()
			==">=")
		ls.suivant();
	else
		erreur("<opRel>");

}
////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::opUnaire() {
	// <opUnaire> ::= - | non

	if (ls.getSymCour()=="-" || ls.getSymCour()=="non")

		ls.suivant();
	else
		erreur("<opUnaire>");

}

////////////////////////////////////////////////////////////////////////////////

void LecteurPhraseSimple::instSi() { //si (<expBool> ) <seqInst> {sinonsi (<expBool>) <seqInst> } [sinon <seqInst> ] finsi

	sauterSymCour("si");

	if (ls.getSymCour() == "(") {


		sauterSymCour("(");

		expBool();

		sauterSymCour(")");

		seqInst();

		while (ls.getSymCour() == "sinonsi") {
			ls.suivant();

			sauterSymCour("(");
			expBool();
			sauterSymCour(")");
			seqInst();
		}

		if (ls.getSymCour()=="sinon") {
			sauterSymCour("sinon");
			seqInst();
		}

		sauterSymCour("finsi");


	} else
		erreur("<instSi>");
}

////////////////////////////////////////////////////////////////////////////////

void LecteurPhraseSimple::instTq() { //tantque ( <expBool> ) <seqInst> fintantque

	sauterSymCour("tantque");

	if (ls.getSymCour() == "(") {


		sauterSymCour("(");
		expBool();
		sauterSymCour(")");
		seqInst();
		sauterSymCour("fintantque");


	} else
		erreur("<instTq>");
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::instRepeter() {// repeter <seqInst> jusqua ( <expBool> )
	sauterSymCour("repeter");
	seqInst();

	sauterSymCour("jusqua");

	if (ls.getSymCour() == "(")
		{


		sauterSymCour("(");
		expBool();
		sauterSymCour(")");
	}

	else
		erreur("<instRepeter>");

}
////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::instLire() { // lire ( <variable> )

	sauterSymCour("lire");

	sauterSymCour("(");

	if (ls.getSymCour()== "<VARIABLE>")

		ls.suivant();

	else
		erreur("<instLire>");

	sauterSymCour(")");

}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::instEcrire() { // ecrire ( <expression> | <chaine> )


	sauterSymCour("ecrire");
	sauterSymCour("(");

	if (ls.getSymCour() == "<CHAINE>")
	ls.suivant();

	else
		expression();

	sauterSymCour(")");



}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::instPour() {


	sauterSymCour("pour");

	if (ls.getSymCour() == "(") {
		sauterSymCour("(");
	affectation();
	sauterSymCour(";");
	expBool();
	sauterSymCour(";");
	affectation();
	sauterSymCour(")");

	seqInst();


	}

	else
		erreur("<instPour>");

	sauterSymCour("finpour");
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::instSwitch() {
	sauterSymCour("switch");

	if (ls.getSymCour() == "(") {
		sauterSymCour("(");
		ls.suivant();
		sauterSymCour(")");
		sauterSymCour("{");
		while (ls.getSymCour() == "case") {

			sauterSymCour("case");
			testerSymCour("<ENTIER>");
			ls.suivant();
			sauterSymCour(":");
			seqInst();
			sauterSymCour("break");
			sauterSymCour(";");
		}

		if(ls.getSymCour() == "default") {
			sauterSymCour("default");
			sauterSymCour(":");
			if (ls.getSymCour() != "break")
			seqInst();
			sauterSymCour("break");
			sauterSymCour(";");
		}
		sauterSymCour("}");
	}

	else

		erreur("<instSwitch>");



}
