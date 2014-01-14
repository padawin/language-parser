#include "LecteurPhraseAvecArbre.h"

#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
LecteurPhraseAvecArbre::LecteurPhraseAvecArbre(char nomFich[]) :
	ls(nomFich), ts() {
}


void LecteurPhraseAvecArbre::generCode() { // procédure appelant la procédure de traduction de arbre.cc
	cout << "#include <stdlib.h>"<< endl;
	cout << "#include <stdio.h>" << endl;
	cout << "#include <iostream>" << endl;
	cout << "#include <string.h>"<< endl;
	cout << "using namespace std;"<< endl;
	cout << endl <<"int main { " << endl;
		ts.getValeur();
		arbre->afficherInstAlgo();
		cout << "}" << endl;
}
////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecArbre::analyse() {
	arbre=programme();
	cout << "Syntaxe correcte." << endl;
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::programme() {
	// <programme> ::= debut <seq_inst> fin FIN_FICHIER

	sauterSymCour("debut");
	Noeud* si = seqInst();
	sauterSymCour("fin");
	testerSymCour("<FINDEFICHIER>");
	return si;
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::seqInst() {
	// <seqInst> ::= <inst> ; { <inst> ; }

	NoeudSeqInst* si = new NoeudSeqInst();
	do {
		si->ajouteInstruction(inst());
		sauterSymCour(";");
	} while (ls.getSymCour()=="<VARIABLE>" || ls.getSymCour()=="si"
			|| ls.getSymCour()=="tantque" || ls.getSymCour()=="repeter"
			|| ls.getSymCour()=="ecrire" || ls.getSymCour()=="lire"
			 || ls.getSymCour()=="pour" || ls.getSymCour() == "selon");
	// tant que le symbole courant est un debut possible d'instruction...
	return si;
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::inst() {
	// <inst> ::= <affectation> | <inst_condi>
	if (ls.getSymCour()=="si")
		return instSi();
	else if (ls.getSymCour()=="tantque")
		return instTq();
	else if (ls.getSymCour()=="repeter")
		return instRepeter();
	else if (ls.getSymCour()=="<VARIABLE>")
		return affectation();
	else if (ls.getSymCour()=="ecrire")
		return instEcrire();
	
	else if (ls.getSymCour()=="lire")
	return instLire();
	else if (ls.getSymCour()=="pour")
		return instPour();
	else if (ls.getSymCour()=="selon")
		return instSwitch();
	else
	{
		erreur("<inst>");
		return NULL;
	}

}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::affectation() {
	// <affectation> ::= <variable> = <expression>

	testerSymCour("<VARIABLE>");
	Noeud* var = ts.chercheAjoute(ls.getSymCour());
	ls.suivant();
	sauterSymCour("=");
	Noeud* exp = expression();
	return new NoeudAffectation(var,exp);
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::expression() {
	// <expression> ::= <terme> { <opAdd> <terme> }

	Noeud* term = terme();
	while (ls.getSymCour()=="+" || ls.getSymCour()=="-") {
				Symbole operateur = opAdd(); // on stocke le symbole de l'opérateur
				Noeud* termDroit=terme(); // lecture de l'operande droit
				term = new NoeudOperateurBinaire(operateur,term,termDroit); // const. du noeud
			}
			return term;
		}

		////////////////////////////////////////////////////////////////////////////////
		Noeud* LecteurPhraseAvecArbre::facteur() {
			// <facteur> ::= <entier>  |  <variable>  |  - <facteur>  |  ( <expression> )

			Noeud* fact=NULL;

			if (ls.getSymCour()=="<VARIABLE>" || ls.getSymCour()=="<ENTIER>") {
				fact=ts.chercheAjoute(ls.getSymCour());
				ls.suivant();
			} else if (ls.getSymCour()=="-") {
				ls.suivant();
				// on représente le moins unaire (- facteur) par une soustractin binaire (0 - facteur)
				fact = new NoeudOperateurBinaire(Symbole("-"), ts.chercheAjoute(Symbole("0")), facteur());
			} else if (ls.getSymCour()=="(") {
				ls.suivant();
				fact=expression();
				sauterSymCour(")");
			} else
				erreur("<facteur>");
			return fact;
		}

		////////////////////////////////////////////////////////////////////////////////
		Symbole LecteurPhraseAvecArbre::opBinaire() {
			// <opBinaire> ::= + | - | *  | /
			Symbole operateur;
			if (ls.getSymCour()=="+" || ls.getSymCour()=="-" || ls.getSymCour()
					=="*" || ls.getSymCour()=="/") {
				operateur=ls.getSymCour();
				ls.suivant();
			} else
				erreur("<opBinaire>");
			return operateur;
		}

		////////////////////////////////////////////////////////////////////////////////
		void LecteurPhraseAvecArbre::testerSymCour(char * ch) {
			if (ls.getSymCour() != ch) {
				cout << endl << "-------- Erreur ligne " << ls.getLigne()
						<< " - Colonne " << ls.getColonne() << endl
						<< "   Attendu : " << ch << endl << "   Trouve  : "
						<< ls.getSymCour() << endl << endl;
				exit(0); // plus tard, on levera une exception
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		void LecteurPhraseAvecArbre::sauterSymCour(char * ch) {
			testerSymCour(ch);
			ls.suivant();
		}

		////////////////////////////////////////////////////////////////////////////////
		void LecteurPhraseAvecArbre::erreur(char mess[]) {
			cout << endl << "-------- Erreur ligne " << ls.getLigne()
					<< " - Colonne " << ls.getColonne() << endl
					<< "   Attendu : " << mess << endl << "   Trouve  : "
					<< ls.getSymCour() << endl << endl;
			exit(0); // plus tard, on levera une exception
		}
		///////////////////////////////////////////
		Noeud* LecteurPhraseAvecArbre::terme() { // <terme> ::=<facteur> { <opMult> <facteur> }

			Noeud* term=facteur();
			while (ls.getSymCour()=="*"|| ls.getSymCour()=="/") {
				Symbole operateur = opMult(); // on stocke le symbole de l'opérateur
				Noeud* termDroit=facteur(); // lecture de l'operande droit
				term = new NoeudOperateurBinaire(operateur,term,termDroit); // const. du noeud
			}
			return term;

		}
		///////////////////////////////////////////////////////
		Symbole LecteurPhraseAvecArbre::opAdd() { // <opAdd> ::= + | -

			Symbole operateur;
			if (ls.getSymCour()=="+" || ls.getSymCour()=="-") {
				operateur=ls.getSymCour();
				ls.suivant();
			} else
				erreur("<opAdd>");
			return operateur;

		}
		//////////////////////////////////////////////////////////
		Symbole LecteurPhraseAvecArbre::opMult() { // <opMult> ::= * | /

			Symbole operateur;
			if (ls.getSymCour()=="*" || ls.getSymCour()=="/") {
				operateur=ls.getSymCour();
				ls.suivant();
			} else
				erreur("<opMult>");
			return operateur;

		}
		//////////////////////////////////////////////////	
		Noeud* LecteurPhraseAvecArbre::expBool() { // <expBool> ::= <relation> {<opBool> <relation> }

			Noeud* expBool=relation();
			while (ls.getSymCour()=="et"|| ls.getSymCour()=="ou") {
				Symbole operateur = opBool(); // on stocke le symbole de l'opérateur
				Noeud* expBoolDroit=relation(); // lecture de l'operande droit
				expBool = new NoeudOperateurBinaire(operateur,expBool,expBoolDroit); // const. du noeud
			}
			return expBool;

		}
		//////////////////////////////////////////////////
		Symbole LecteurPhraseAvecArbre::opBool() { // <opBool> ::= et | ou
			Symbole operateur;
			if (ls.getSymCour()=="et" || ls.getSymCour()=="ou") {
				operateur=ls.getSymCour();
				ls.suivant();
			} else
				erreur("<opBool>");
			return operateur;

		}
		///////////////////////////////////////////
		Noeud* LecteurPhraseAvecArbre::relation() {
			// <relation> ::= <expression> { <opRel> <expression>}
			Noeud* relation=expression();
			while (ls.getSymCour()=="=="|| ls.getSymCour()=="!="
					|| ls.getSymCour() =="<=" ||ls.getSymCour()==">="
					|| ls.getSymCour()=="<" ||ls.getSymCour()==">") {
				Symbole operateur = opRel(); // on stocke le symbole de l'opérateur
				Noeud* relationDroit=expression(); // lecture de l'operande droit
				relation = new NoeudOperateurBinaire(operateur,relation,relationDroit); // const. du noeud
			}
			return relation;

		}

		///////////////////////////////////////////
		Symbole LecteurPhraseAvecArbre::opRel() {
			// <opRel> ::= == | != | < | <= | > | >= 

			Symbole operateur;
			if (ls.getSymCour()=="=="|| ls.getSymCour()=="!="
					|| ls.getSymCour()=="<=" ||ls.getSymCour()==">="
					|| ls.getSymCour()=="<" ||ls.getSymCour() ==">") {
				operateur=ls.getSymCour();
				ls.suivant();
			} else
				erreur("<opRel>");
			return operateur;
		}
		///////////////////////////////////////////
		Symbole LecteurPhraseAvecArbre::opUnaire() {
			// <opUnaire> ::= - | non
			Symbole operateur;
			if (ls.getSymCour()=="-"|| ls.getSymCour()=="non") {
				operateur=ls.getSymCour();
				ls.suivant();
			} else
				erreur("<opUnaire>");
			return operateur;

		}
		///////////////////////////////////////////
		Noeud* LecteurPhraseAvecArbre::instSi() {
			// si (<expBool> ) <seqInst> {sinonsi (<expBool>) <seqInst> }
			//	[sinon <seqInst> ] finsi

			NoeudInstSi* instSi= new NoeudInstSi();

			sauterSymCour("si");
			sauterSymCour("(");

			Noeud* condition=expBool();
			sauterSymCour(")");
			Noeud* inst = seqInst();
			instSi->ajouteInstruction(condition, inst);

			while (ls.getSymCour()=="sinonsi") {

				sauterSymCour("sinonsi");
				sauterSymCour("(");
				Noeud* condition= expBool();
				sauterSymCour(")");
				Noeud* inst= seqInst();

				instSi->ajouteInstruction(condition, inst);
			}

			if (ls.getSymCour()=="sinon") {
				sauterSymCour("sinon");
				Noeud* inst= seqInst();

				instSi->ajouteInstruction(NULL,inst); // Else : pas de condition
			}
			sauterSymCour("finsi");

			return instSi;

		}

		///////////////////////////////////////////
		Noeud* LecteurPhraseAvecArbre::instTq() {
			// tantque ( <expBool> ) <seqInst> fintantque 


			sauterSymCour("tantque");
			sauterSymCour("(");

			Noeud* condition=expBool();
			sauterSymCour(")");
			Noeud* inst = seqInst();
			NoeudInstTq* instTq= new NoeudInstTq(condition,inst);
			sauterSymCour("fintantque");
			return instTq;

		}
		///////////////////////////////////////////
		Noeud* LecteurPhraseAvecArbre::instRepeter() {
			// repeter <seqInst> jusqua ( <expBool> )
			sauterSymCour("repeter");
			Noeud* inst = seqInst();
			sauterSymCour("jusqua");
			sauterSymCour("(");
			Noeud* condition=expBool();
			sauterSymCour(")");
			NoeudInstRepeter* instRpt=new NoeudInstRepeter(condition, inst);
			return instRpt;
		}
		///////////////////////////////////////////
		Noeud* LecteurPhraseAvecArbre::instLire() {
			// lire ( <variable> )
			sauterSymCour("lire");
			sauterSymCour("(");
			testerSymCour("<VARIABLE>"); // on teste si variable
			Noeud* var = ts.chercheAjoute(ls.getSymCour());
			ls.suivant();
			sauterSymCour(")");
			NoeudInstLire* instL= new NoeudInstLire(var);
			return instL;
		}
		///////////////////////////////////////////
		Noeud* LecteurPhraseAvecArbre::instEcrire() {
			// ecrire ( <expression> | <chaine> )


			sauterSymCour("ecrire");
			sauterSymCour("(");
			Noeud* exp=NULL;
			Noeud* ch=NULL;
			if (ls.getSymCour() == "<CHAINE>") { //  on regarde si chaine 
				
				ch=ts.chercheAjoute(ls.getSymCour()); 
				ls.suivant();
			}
	
			else // ou expression
				exp=expression();
			
				sauterSymCour(")");
				NoeudInstEcrire* instE= new NoeudInstEcrire(exp,ch);
				return instE;
			
		}

		///////////////////////////////////////////
		Noeud* LecteurPhraseAvecArbre::instPour() {
			//  pour (affectation ; expBool ; affectation) <seqinst> finpour;
			
			
			sauterSymCour("pour");
			sauterSymCour("(");
			Noeud* aff=affectation();
			sauterSymCour(";");
			Noeud* cond=expBool();	
			sauterSymCour(";");
			Noeud* incr= affectation();
			sauterSymCour(")");	
			
			Noeud* inst = seqInst();
			
			sauterSymCour("finpour");
			
			
			return new NoeudInstPour(aff,cond,incr,inst);	
		}
		
		///////////////////////////////////////////
		Noeud* LecteurPhraseAvecArbre::instSwitch() {
		// switch(variable) { case constante: <seqinst> break;};
		
		
		sauterSymCour("selon");
		sauterSymCour("(");
		Noeud* variable= NULL;
		testerSymCour("<VARIABLE>");// on teste si le symbole courant est une variable
		variable= ts.chercheAjoute(ls.getSymCour()); // ajoute la variable à la table des symboles si inexistant dedans
		ls.suivant();
		sauterSymCour(")");
		
		NoeudInstSwitch* instS= new NoeudInstSwitch(variable);
		
		sauterSymCour("{");
		while (ls.getSymCour() == "cas") {
			sauterSymCour("cas");
			testerSymCour("<ENTIER>"); // teste bien que c'est un entier
			Noeud* num= facteur();
			sauterSymCour(":");
			Noeud* inst= seqInst();
			instS->ajouteInstruction(inst,num); // ajout d'une instruction à chaque tableau 
			sauterSymCour("stop");
			sauterSymCour(";");
			}
				// On suppose qu'il y a tjs un break.. 
			
			
		if (ls.getSymCour() == "defaut")
		{
			sauterSymCour("defaut");
			sauterSymCour(":");
			if(ls.getSymCour() != "stop")
			{	
				Noeud* inst= seqInst();
				instS->ajouteInstruction(inst,NULL); // si default pas de numéro
			}
			else
				instS->ajouteInstruction(NULL,NULL); // si default et pas d'instruction 
			sauterSymCour("stop");
			sauterSymCour(";");
		}	
		
		sauterSymCour("}");
		return instS;
		}

