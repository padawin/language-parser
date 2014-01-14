#include "LecteurSymbole.h"
#include <ctype.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
LecteurSymbole::LecteurSymbole(char nom_fich[]) :
	lc(nom_fich), symCour("") {
	suivant(); // pour lire le premier symbole
}

////////////////////////////////////////////////////////////////////////////////
void LecteurSymbole::suivant() {
	
	 do
		{
		 sauterCommentaire();
		 sauterSeparateurs();
		} while (lc.getCarCour()== '#');
			

	// on est maintenant positionne sur le premier caractère d'un symbole
	ligne=lc.getLigne();
	colonne=lc.getColonne();
	symCour=Symbole(motSuivant()); // on reconstruit symCour avec le nouveau mot lu

}

////////////////////////////////////////////////////////////////////////////////
void LecteurSymbole::sauterSeparateurs() {
	while (lc.getCarCour()==' '  || 
		   lc.getCarCour()=='\t' || 
		   lc.getCarCour()=='\r' || 
		   lc.getCarCour()=='\n')
		lc.suivant();
}
//////////////////////////////////////////////////////////////////
void LecteurSymbole::sauterCommentaire() {

	
	if (lc.getCarCour() == '#')
	{
		lc.suivant();
		if (lc.getCarCour() == '*')
		{
		do
			{
				lc.suivant();
			} while (lc.getCarCour() != '*' );
			do
			{
				lc.suivant();
			} while (lc.getCarCour() != '#' );
		}
		else {
		do
		{
			lc.suivant();
		} while (lc.getCarCour() != '\n' &&
			   lc.getCarCour() !='\r');
		 }
		
	}	
	
}
////////////////////////////////////////////////////////////////////////////////
string LecteurSymbole::motSuivant() {
	static string  s;
	s="";
	
	if (isdigit(lc.getCarCour()))
		// c'est le début d'un entier
		do {
			s=s+lc.getCarCour();
			lc.suivant();
		} while (isdigit(lc.getCarCour()));
	
	else if (lc.getCarCour()== '=' || 
			lc.getCarCour()== '!' || 
			lc.getCarCour()== '<' ||
			lc.getCarCour()== '>' )
	{
			s=s+lc.getCarCour();
			lc.suivant();
			
			if (lc.getCarCour()== '=')
			{
				s=s+lc.getCarCour();
				lc.suivant();
			}
	}	
		
	else if (lc.getCarCour() == '"')
		{ 
			s= "\"";
				do {
					lc.suivant();
					s=s+lc.getCarCour();
				
				} while (lc.getCarCour() != '"' && 
							 lc.getCarCour() !='\n' &&
							 lc.getCarCour() !='\r' &&
							 lc.getCarCour() != EOF);
			
			if (lc.getCarCour() != '"')
			{
				cout << "Erreur: votre chaine n'est pas terminée ! "<< endl;
				exit(0);
			}
			lc.suivant();
		}
	
	else if (isalpha(lc.getCarCour()))
		// c'est le début d'un mot
		do {
			s=s+lc.getCarCour();
			lc.suivant();
		} while (isalpha(lc.getCarCour())  || 
				 isdigit(lc.getCarCour()) || 
				 lc.getCarCour()=='_');
	
	else if (lc.getCarCour()!=EOF)
	// c'est un caractere special
	{
		s=lc.getCarCour();
		lc.suivant();
	}
	
	
	
	return s;
}
