// Contient toutes les spécifications de classes nécessaires
// pour représenter l'arbre abstrait
#ifndef ARBRE_H_
#define ARBRE_H_

#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

#include "Symbole.h"

////////////////////////////////////////////////////////////////////////////////
class Noeud {
	// classe abstraite dont dériveront toutes les classes servant à représenter l'arbre abstrait
	// Remarque : la classe ne contient aucun constructeur
public:
	virtual int getValeur() =0; // méthode pure (non implémentée) qui rend la classe abstraite
	virtual void afficher(unsigned short indentation=0) {
		cout << setw(4*indentation) << " ";
	}
	// pour afficher un noeud avec un décalage (indentation) proportionnel à son niveau dans l'arbre

	virtual void afficherInstAlgo (){
		
	}
	// affiche la séquence d'instruction sous forme de langage de programmation (C++)
	virtual ~Noeud() {
	} // présence d'un destructeur virtuel conseillée dans les classes abstraites
};

////////////////////////////////////////////////////////////////////////////////
class NoeudSeqInst : public Noeud {
	// classe pour représenter un noeud "sequence d'instruction"
	// qui a autant de fils que d'instructions dans la séquence
public:
	NoeudSeqInst(); // construit une séquence d'instruction vide
	~NoeudSeqInst() {
	} // à cause du destructeur virtuel de la classe Noeud
	int getValeur(); // évalue chaque instruction de la séquence
	void afficher(unsigned short indentation=0); // affiche la séquence d'instructions
	void ajouteInstruction(Noeud* instruction); // ajoute une instruction à la séquence
	void afficherInstAlgo ();// affiche la séquence d'instruction sous forme de langage de programmation (C++)
	
private:
	vector<Noeud *> tabInst; // pour stocker les instructions de la séquence
};

////////////////////////////////////////////////////////////////////////////////
class NoeudAffectation : public Noeud {
	// classe pour représenter un noeud "affectation"
	// composé de 2 fils : la variable et l'expression qu'on lui affecte
public:
	NoeudAffectation(Noeud* variable, Noeud* expression); // construit une affectation
	~NoeudAffectation() {
	} // à cause du destructeur virtuel de la classe Noeud
	int getValeur(); // évalue l'expression et affecte sa valeur à la variable
	void afficher(unsigned short indentation=0); // affiche l'affectation
	void afficherInstAlgo ();// affiche la séquence d'instruction sous forme de langage de programmation (C++)
	
private:
	Noeud* variable;
	Noeud* expression;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudOperateurBinaire : public Noeud {
	// classe pour représenter un noeud "opération arithmétique" composé d'un opérateur (+-*/)
	// et de 2 fils : l'opérande gauche et l'opérande droit
public:
	NoeudOperateurBinaire(Symbole operateur, Noeud* operandeGauche,
			Noeud* operandeDroit);
	// construit une opération binaire : operandeGauche operateur OperandeDroit
	~NoeudOperateurBinaire() {
	} // à cause du destructeur virtuel de la classe Noeud
	int getValeur(); // évalue l'operande gauche, l'operande droit et applique l'opérateur
	void afficher(unsigned short indentation=0); // affiche l'opération
	void afficherInstAlgo ();// affiche la séquence d'instruction sous forme de langage de programmation (C++)
	
private:
	Symbole operateur;
	Noeud* operandeGauche;
	Noeud* operandeDroit;
};

//////////////////////////////////////////////////////////////////////////
class NoeudInstSi : public Noeud {

public:

	NoeudInstSi(); // construit une séquence d'instruction vide
	~NoeudInstSi() {
	} // à cause du destructeur virtuel de la classe Noeud
	int getValeur(); // évalue chaque instruction de la séquence
	void afficher(unsigned short indentation=0); // affiche la séquence d'instructions
	void ajouteInstruction(Noeud* instruction, Noeud* Condition); // ajoute une instruction à la séquence
	void afficherInstAlgo ();// affiche la séquence d'instruction sous forme de langage de programmation (C++)

private:
	vector<Noeud *> tabCond; // pour stocker les condition de la séquence (si, sinonsi ..)
	vector<Noeud *> tabSeqInst; // pour stocker les instructions de la séquence


};
////////////////////////////////////////////////////
class NoeudInstTq : public Noeud {
public:
	NoeudInstTq(Noeud* cond, Noeud* seqInst); // construit une séquence d'instruction vide
	~NoeudInstTq() {
	} // à cause du destructeur virtuel de la classe Noeud
	int getValeur(); // évalue chaque instruction de la séquence
	void afficher(unsigned short indentation=0); // affiche la séquence d'instructions
	void afficherInstAlgo ();// affiche la séquence d'instruction sous forme de langage de programmation (C++)

private:
	Noeud* cond;
	Noeud* seqInst;
};

////////////////////////////////////////
class NoeudInstRepeter : public Noeud {

public:
	NoeudInstRepeter(Noeud* expBool, Noeud* seqInst); // construit une séquence d'instruction vide
	~NoeudInstRepeter() {
	} // à cause du destructeur virtuel de la classe Noeud
	int getValeur(); // évalue chaque instruction de la séquence
	void afficher(unsigned short indentation=0); // affiche la séquence d'instructions
	void afficherInstAlgo ();// affiche la séquence d'instruction sous forme de langage de programmation (C++)
	
private:
	Noeud* expBool;
	Noeud* seqInst;
};

/////////////////////////////////////////////
class NoeudInstLire : public Noeud {

public:
	NoeudInstLire(Noeud* variable); // construit une séquence d'instruction vide
	~NoeudInstLire() {
	} // à cause du destructeur virtuel de la classe Noeud
	int getValeur(); // évalue chaque instruction de la séquence
	void afficher(unsigned short indentation=0); // affiche la séquence d'instructions
	void afficherInstAlgo ();// affiche la séquence d'instruction sous forme de langage de programmation (C++)
private:
	Noeud* variable;
};

////////////////////////////////////////////
class NoeudInstEcrire : public Noeud {

public:
	NoeudInstEcrire(Noeud* exp, Noeud* ch); // construit une séquence d'instruction vide
	~NoeudInstEcrire() {
	} // à cause du destructeur virtuel de la classe Noeud
	int getValeur(); // évalue chaque instruction de la séquence
	void afficher(unsigned short indentation=0); // affiche la séquence d'instructions
	void afficherInstAlgo ();// affiche la séquence d'instruction sous forme de langage de programmation (C++)

private:
	Noeud* exp;
	Noeud* ch;
};
////////////////////////////////////////////
class NoeudInstPour : public Noeud {

public:

	NoeudInstPour(Noeud* aff,Noeud* cond,Noeud* incr, Noeud* seqinst); // construit une séquence d'instruction vide
	~NoeudInstPour() {
	} // à cause du destructeur virtuel de la classe Noeud
	int getValeur(); // évalue chaque instruction de la séquence
	void afficher(unsigned short indentation=0); // affiche la séquence d'instructions
	void afficherInstAlgo ();// affiche la séquence d'instruction sous forme de langage de programmation (C++)

private:
	
	Noeud* aff; // affectation dans le pour
	Noeud* cond; // condition dans le pour
	Noeud* incr;// incrémentation dans le pour
	Noeud* seqinst;// sequence d'instruction 
};

////////////////////////////////////////////
class NoeudInstSwitch : public Noeud {

public:
	NoeudInstSwitch(Noeud* variable); // construit une séquence d'instruction vide
	~NoeudInstSwitch() {
	} // à cause du destructeur virtuel de la classe Noeud
	int getValeur(); // évalue chaque instruction de la séquence
	void ajouteInstruction(Noeud* instruction,Noeud* num);
	void afficher(unsigned short indentation=0); // affiche la séquence d'instructions
	void afficherInstAlgo ();// affiche la séquence d'instruction sous forme de langage de programmation (C++)

private:
	
	
	Noeud* variable;
	vector<Noeud*> tabCase;// pour stocker l'entier correspondant à chaque "case"
	vector<Noeud *> tabInst; //pour stocker les instructions de la séquence
};

#endif /* ARBRE_H_ */
