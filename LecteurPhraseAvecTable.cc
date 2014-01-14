#include "LecteurPhraseAvecTable.h"

#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
LecteurPhraseAvecTable::LecteurPhraseAvecTable(char nomFich[]) :
	ls(nomFich), ts() {
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::analyse() {
	programme();
	cout << "Syntaxe correcte." << endl;
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::programme() {
	// <programme> ::= debut <seqInst> fin <EOF>

	sauterSymCour("debut");
	seqInst();
	sauterSymCour("fin");
	testerSymCour("<FINDEFICHIER>");
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::seqInst() {
	// <seqIns> ::= <inst> ; { <inst> ; }

	do {
		inst();
		sauterSymCour(";");

	} while (ls.getSymCour()=="<VARIABLE>" || ls.getSymCour()=="si"
			|| ls.getSymCour()=="tantque" || ls.getSymCour()=="repeter"
			|| ls.getSymCour()=="ecrire" || ls.getSymCour()=="lire" ||
			ls.getSymCour() =="pour" || ls.getSymCour() == "switch");

	// tant que le symbole courant est un debut possible d'instruction...


}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::inst() {
	// <inst> ::= <affectation>

	if (ls.getSymCour()=="si")
		instSi();
	else if (ls.getSymCour()=="tantque")
		instTq();
	else if (ls.getSymCour()=="repeter")
		instRepeter();
	else if (ls.getSymCour()=="<VARIABLE>") {
		ts.chercheAjoute(ls.getSymCour());
		affectation();
	} else if (ls.getSymCour()=="ecrire")
		instEcrire();
	else if (ls.getSymCour()=="lire")
		instLire();
	else if (ls.getSymCour()=="pour")
		instPour();
	else if (ls.getSymCour() == "switch")
		instSwitch();
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::affectation() {
	// <affectation> ::= <variable> = <expression>

	//sauterSymCour("<VARIABLE>");
	testerSymCour("<VARIABLE>");
	ts.chercheAjoute(ls.getSymCour());
	ls.suivant();
	sauterSymCour("=");
	expression();
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::expression() {
	// <expression> ::= <terme> { <opAdd> <terme> }

	terme();
	while (ls.getSymCour()=="+" || ls.getSymCour()=="-" ){
		opAdd();
		terme();
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::facteur() {
	// <facteur> ::= <entier>  |  <variable>  |  - <facteur>  |  ( <expression> )

	if (ls.getSymCour()=="<VARIABLE>" || ls.getSymCour()=="<ENTIER>") {
		ts.chercheAjoute(ls.getSymCour());
		ls.suivant();
	} else if (ls.getSymCour()=="-") {
		ls.suivant();
		facteur();
	} else if (ls.getSymCour()=="(") {
		ls.suivant();
		expression();
		sauterSymCour(")");
	} else
		erreur("<facteur>");
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::testerSymCour(char * ch) {
	if (ls.getSymCour() != ch) {
		cout << endl << "-------- Erreur ligne " << ls.getLigne()
				<< " - Colonne " << ls.getColonne() << endl << "   Attendu : "
				<< ch << endl << "   Trouve  : " << ls.getSymCour() << endl
				<< endl;
		exit(0); // plus tard, on levera une exception
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::sauterSymCour(char * ch) {
	testerSymCour(ch);
	ls.suivant();
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::erreur(char mess[]) {
	cout << endl << "-------- Erreur ligne " << ls.getLigne() << " - Colonne "
			<< ls.getColonne() << endl << "   Attendu : " << mess << endl
			<< "   Trouve  : " << ls.getSymCour() << endl << endl;
	exit(0); // plus tard, on levera une exception
}

/////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::instSi() { //si (<expBool> ) <seqInst> {sinonsi (<expBool>) <seqInst> } [sinon <seqInst> ] finsi

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
	}

	else
		erreur("<instSi>");
}

////////////////////////////////////////////////////////////////////////////////      

void LecteurPhraseAvecTable::instTq() { //tantque ( <expBool> ) <seqInst> fintantque 

	sauterSymCour("tantque");

	if (ls.getSymCour() == "(") {

		sauterSymCour("(");
		expBool();
		sauterSymCour(")");
		seqInst();
		sauterSymCour("fintantque");
	}

	else
		erreur("<instTq>");
}

////////////////////////////////////////////////////////////////////////////////    
void LecteurPhraseAvecTable::instRepeter() {// repeter <seqInst> jusqua ( <expBool> )
	sauterSymCour("repeter");
	seqInst();
	sauterSymCour("jusqua");

	if (ls.getSymCour() == "(") {

		sauterSymCour("(");
		expBool();
		sauterSymCour(")");
	} else
		erreur("<instRepeter>");

}
////////////////////////////////////////////////////////////////////////////////    
void LecteurPhraseAvecTable::instLire() { // lire ( <variable> )

	sauterSymCour("lire");
	sauterSymCour("(");

	if (ls.getSymCour()== "<VARIABLE>")
	{
		ts.chercheAjoute(ls.getSymCour());
		ls.suivant();
	}
	else
		erreur("<instLire>");

	sauterSymCour(")");

}

////////////////////////////////////////////////////////////////////////////////    
void LecteurPhraseAvecTable::instEcrire() { // ecrire ( <expression> | <chaine> )


	sauterSymCour("ecrire");
	sauterSymCour("(");

	if (ls.getSymCour() == "<CHAINE>") {
		ts.chercheAjoute(ls.getSymCour());
		ls.suivant();
	} else
		expression();

	sauterSymCour(")");

}

////////////////////////////////////////////////////////////////////////////////

void LecteurPhraseAvecTable::expBool() {
	// <expBool> ::= <relation> {<opBool> <relation> }

	relation();
	while (ls.getSymCour()=="et" || ls.getSymCour()=="ou") {
		opBool();
		relation();
	}

}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::opBool() {
	// <opBool> ::= et | ou

	if (ls.getSymCour()=="et" || ls.getSymCour()=="ou")
		ls.suivant();
	else
		erreur("<opBool>");

}
//////////////////////////////////////////////////////////////////////////////// 
void LecteurPhraseAvecTable::relation() {
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
void LecteurPhraseAvecTable::opRel() {
	// <opRel> ::= == | != | < | <= | > | >= 

	if (ls.getSymCour()=="==" || ls.getSymCour()=="!=" || ls.getSymCour()=="<"
			|| ls.getSymCour()=="<=" || ls.getSymCour()==">" || ls.getSymCour()
			==">=")
		ls.suivant();
	else
		erreur("<opRel>");

}
/////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::terme() {
	// <terme> ::=<facteur> { <opMult> facteur }

	facteur();
	while (ls.getSymCour()=="*" || ls.getSymCour()=="/") {
		opMult();
		facteur();
	}

}
//////////////////////////////////////////////////
void LecteurPhraseAvecTable::opAdd() {
	// <opAdd> ::= + | -

	if (ls.getSymCour()=="+" || ls.getSymCour()=="-")
		ls.suivant();
	else
		erreur("<opAdd>");

}
////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::opMult() {
	// <opMult> ::= * | /

	if (ls.getSymCour()=="*" || ls.getSymCour()=="/")
		ls.suivant();
	else
		erreur("<opMult>");

}

////////////////////////////////////////////////////////////////////////////////   
void LecteurPhraseAvecTable::opUnaire() {
	// <opUnaire> ::= - | non

	if (ls.getSymCour()=="-" || ls.getSymCour()=="non")

		ls.suivant();
	else
		erreur("<opUnaire>");

}
////////////////////////////////////////////////////////////////////////////////   
void LecteurPhraseAvecTable::instPour() { 

sauterSymCour("pour");


	sauterSymCour("(");
	affectation();
	sauterSymCour(";");
	expBool();
	sauterSymCour(";");
	affectation();
	sauterSymCour(")");
	seqInst();
	sauterSymCour("finpour");


	
}
////////////////////////////////////////////////////////////////////////////////   
void LecteurPhraseAvecTable::instSwitch() { 
sauterSymCour("switch");
	
	
		sauterSymCour("(");
		testerSymCour("<VARIABLE>");
		ts.chercheAjoute(ls.getSymCour());
		ls.suivant();
		sauterSymCour(")");
		sauterSymCour("{");
		while (ls.getSymCour() == "case") {
		
			sauterSymCour("case");
			
			testerSymCour("<ENTIER>"); // test si entier
			ts.chercheAjoute(ls.getSymCour()); // ajoute à la table des symboles si inexistant de dedans
			ls.suivant();
			
			sauterSymCour(":");
			seqInst();
			
			sauterSymCour("break");
			sauterSymCour(";");
		}
	
		if(ls.getSymCour() == "default") {
			sauterSymCour("default");
			sauterSymCour(":");
			if (ls.getSymCour() != "break") // si break-> pas d'instruction donc pas de seqInst()
			seqInst();
			sauterSymCour("break");
			sauterSymCour(";");
		}
		sauterSymCour("}");
}
