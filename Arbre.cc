#include "Arbre.h"
#include "Symbole.h"
#include "SymboleValue.h"
#include <typeinfo>
#include <string.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
// NoeudSeqInst
////////////////////////////////////////////////////////////////////////////////

NoeudSeqInst::NoeudSeqInst() :
    tabInst() {}

int NoeudSeqInst::getValeur() {
    int valeur=0;
    for (unsigned int i=0; i<tabInst.size(); i++)
        valeur = tabInst[i]->getValeur(); // on evalue chaque instruction de la séquence
    return valeur; // par convention, resultat = valeur de la derniere instruction
}

void NoeudSeqInst::afficher(unsigned short indentation) {
    Noeud::afficher(indentation);
    cout << "Noeud - Sequence de " << tabInst.size() << " instruction(s)"
            << endl;
    for (unsigned int i=0; i<tabInst.size(); i++)
        tabInst[i]->afficher(indentation+1); // on affiche les fils en augmentant l'indentation
}

void NoeudSeqInst::ajouteInstruction(Noeud* instruction) {
    tabInst.push_back(instruction);
}

void NoeudSeqInst::afficherInstAlgo() {
    // affiche la séquence d'instruction sous forme de langage de programmation (C++)
    //Noeud::afficherInstAlgo();

    Noeud::afficherInstAlgo();

    for (unsigned int i=0; i<tabInst.size(); i++) {

        cout <<"\t";
        tabInst[i]->afficherInstAlgo();
        //if ((typeid(*tabInst[i]).name())!= (typeid(NoeudInstPour).name()) && (typeid(*tabInst[i]).name())!= (typeid(NoeudInstTq).name()) && (typeid(*tabInst[i]).name())!= (typeid(NoeudInstSi).name()) && (typeid(*tabInst[i]).name())!= (typeid(NoeudInstSwitch).name())&& (typeid(*tabInst[i]).name())!= (typeid(NoeudInstRepeter).name())&& (typeid(*tabInst[i]).name())!= (typeid(NoeudInstEcrire).name()))
        if ((typeid(*tabInst[i]).name())== (typeid(NoeudAffectation).name()))
        cout << ";" <<endl;

    }


}

////////////////////////////////////////////////////////////////////////////////
// NoeudAffectation
////////////////////////////////////////////////////////////////////////////////

NoeudAffectation::NoeudAffectation(Noeud* variable, Noeud* expression) {
    this->variable=variable;
    this->expression=expression;
}

int NoeudAffectation::getValeur() {
    int valeur = expression->getValeur(); // on évalue l'expression
    ((SymboleValue*)variable)->setValeur(valeur); // on affecte la variable
    return valeur; // par convention, une affectation a pour valeur la valeur affectée
}

void NoeudAffectation::afficher(unsigned short indentation) {
    Noeud::afficher(indentation);
    cout << "Noeud - Affectation" << endl;
    variable->afficher(indentation+1); // on affiche variable et expression
    expression->afficher(indentation+1); // en augmentant l'indentation
}

void NoeudAffectation::afficherInstAlgo() {
    // affiche la séquence d'instruction sous forme de langage de programmation (C++)


    Noeud::afficherInstAlgo();

    variable->afficherInstAlgo(); // affichage de la variable = expression
    cout << " = ";
    expression->afficherInstAlgo();



}

////////////////////////////////////////////////////////////////////////////////
// NoeudOperateurBinaire
////////////////////////////////////////////////////////////////////////////////

NoeudOperateurBinaire::NoeudOperateurBinaire(Symbole operateur,
        Noeud* operandeGauche, Noeud* operandeDroit) {
    this->operateur=operateur;
    this->operandeGauche=operandeGauche;
    this->operandeDroit=operandeDroit;
}

int NoeudOperateurBinaire::getValeur() {
    int valeur=0;
    int og=operandeGauche->getValeur();
    int od=operandeDroit->getValeur();
    if (this->operateur=="+")
        valeur = og+od;
    else if (this->operateur=="-")
        valeur = og-od;
    else if (this->operateur=="*")
        valeur = og*od;
    else if (this->operateur=="==")
        valeur = og==od;
    else if (this->operateur=="<")
        valeur = og<od;
    else if (this->operateur==">")
        valeur = og>od;
    else if (this->operateur=="<=")
        valeur = og<=od;
    else if (this->operateur==">=")
        valeur = og>=od;
    else if (this->operateur=="!=")
        valeur = og!=od;
    else if (this->operateur=="et")
        valeur= og&&od;
    else if (this->operateur=="ou")
        valeur= og||od;
    else if (this->operateur=="/") {
        if (od!=0)
            valeur = og/od;
        else {
            cout << "Erreur pendant l'interpretation : division par zero"
                    << endl;
            exit(0); // plus tard on levera une exception
        }
    } else {
        cout << "Erreur pendant l'interpretation : opérateur binaire inconnu"
                << endl;
        exit(0); // plus tard on levera une exception
    }

    return valeur;
}

void NoeudOperateurBinaire::afficher(unsigned short indentation) {
    Noeud::afficher(indentation);
    cout << "Noeud - Operateur Binaire \"" << this->operateur.getChaine()
            << "\" applique a : " << endl;
    operandeGauche->afficher(indentation+1); // on affiche fils gauche et fils droit
    operandeDroit->afficher(indentation+1); // en augmentant l'indentation


}

void NoeudOperateurBinaire::afficherInstAlgo() {
    // affiche la séquence d'instruction sous forme de langage de programmation (C++)


    Noeud::afficherInstAlgo();

    operandeGauche->afficherInstAlgo();
    cout<<this->operateur.getChaine();
    operandeDroit->afficherInstAlgo();

}
///////////////////////////////////////////////
//Noeud instruction si
//////////////////////////////////////////////////////////////////////////
NoeudInstSi::NoeudInstSi() :
    tabCond(), tabSeqInst() {

}

int NoeudInstSi::getValeur() {

    unsigned int i=0;
    while (i< tabCond.size()-1 && !tabCond[i]->getValeur() )
        i++;
    if (tabCond[i]==NULL || tabCond[i]->getValeur())
        tabSeqInst[i]->getValeur();
    return 0;
}

void NoeudInstSi::afficher(unsigned short indentation) {
    Noeud::afficher(indentation);
    cout << "Noeud - Instruction Si de " << tabCond.size() << " condition(s)"
            << endl;
    for (unsigned int i=0; i<tabCond.size(); i++) {
        if (tabCond[i]!=NULL)
            tabCond[i]->afficher(indentation+1);// on affiche les fils en augmentant l'indentation
        else {
            Noeud::afficher(indentation+1);
            cout << "sinon" << endl;
        }
        tabSeqInst[i]->afficher(indentation+1);
    }
}

void NoeudInstSi::ajouteInstruction(Noeud* condition, Noeud* instruction) {

    tabCond.push_back(condition);
    tabSeqInst.push_back(instruction);

}

void NoeudInstSi::afficherInstAlgo() {
    // affiche la séquence d'instruction sous forme de langage de programmation (C++)

    //Noeud::afficherInstAlgo();

    cout <<endl<<"if ( ";
    tabCond[0]->afficherInstAlgo(); // on affiche la condition du if
    cout << " ) {" << endl;

    tabSeqInst[0]->afficherInstAlgo();// séquence d'instruction du if
    cout << endl <<"}" << endl;

    for (unsigned int i=1; i<tabCond.size(); i++) {
        if (tabCond[i]!=NULL) {
            cout << "else if ( ";

            tabCond[i]->afficherInstAlgo(); // condition du else if
            cout << " ) "<< endl;
        } else {

            cout << "else" << endl;
        }

        tabSeqInst[i]->afficherInstAlgo(); // séquences d'instruction des else if ou else

    }

}
///////////////////////////////////////////////
//Noeud instruction tq
//////////////////////////////////////////////////////////////////////////
NoeudInstTq::NoeudInstTq(Noeud* cond, Noeud* seqInst) {

    this->cond=cond;
    this->seqInst=seqInst;

}

int NoeudInstTq::getValeur() {

    while (cond->getValeur() )
        seqInst->getValeur();
    return 0;
}

void NoeudInstTq::afficher(unsigned short indentation) {

    Noeud::afficher(indentation);
    cout << "Noeud - InstTq" << endl;
    cond->afficher(indentation+1); // on affiche condition et instruction
    seqInst->afficher(indentation+1); // en augmentant l'indentation
}

void NoeudInstTq::afficherInstAlgo() {
    // affiche la séquence d'instruction sous forme de langage de programmation (C++)

    Noeud::afficherInstAlgo();

    cout <<endl<< "while ( ";
    cond->afficherInstAlgo(); //    affichage condition du while
    cout << " )  {" << endl;
    seqInst->afficherInstAlgo();//  affichage séquence d'instruction du while
    cout << endl << " } "<< endl;

}
///////////////////////////////////////////////
//Noeud instruction repeter
//////////////////////////////////////////////////////////////////////////
NoeudInstRepeter::NoeudInstRepeter(Noeud* expBool, Noeud* seqInst) {

    this->expBool=expBool;
    this->seqInst=seqInst;

}

int NoeudInstRepeter::getValeur() {
    do {
        seqInst->getValeur();
    } while (!expBool->getValeur() );

    return 0;
}

void NoeudInstRepeter::afficher(unsigned short indentation) {
    Noeud::afficher(indentation);
    cout << "Noeud - InstRepeter" << endl;
    expBool->afficher(indentation+1); // on affiche condition et instruction
    seqInst->afficher(indentation+1); // en augmentant l'indentation
}

void NoeudInstRepeter::afficherInstAlgo() {
    // affiche la séquence d'instruction sous forme de langage de programmation (C++)
    //Noeud::afficherInstAlgo();
    //Noeud::afficherInstAlgo();
    cout <<endl<< "do { " << endl;
    seqInst->afficherInstAlgo(); // afichage de la séquence du do ... while
    cout <<endl<< "} while ( !";
    expBool->afficherInstAlgo(); // affichage de la condition du while
    cout << " )"<< endl;

}
///////////////////////////////////////////////
//Noeud instruction lire
//////////////////////////////////////////////////////////////////////////
NoeudInstLire::NoeudInstLire(Noeud* variable) {
    this->variable=variable;
}

int NoeudInstLire::getValeur() {
    int valeur;
    cout << "tapez un entier svp..."<< endl;
    cin >> valeur;
    ((SymboleValue*)variable)->setValeur(valeur);

    return valeur;
}

void NoeudInstLire::afficher(unsigned short indentation) {
    Noeud::afficher(indentation);
    cout << "Noeud - InstLire" << endl;
    variable->afficher(indentation+1);
}

void NoeudInstLire::afficherInstAlgo() {
    // affiche la séquence d'instruction sous forme de langage de programmation (C++)

    Noeud::afficherInstAlgo();
    cout <<"cin >> ";
    variable->afficherInstAlgo();
    cout  <<";"<< endl;
}
///////////////////////////////////////////////
//Noeud instruction ecrire
//////////////////////////////////////////////////////////////////////////
NoeudInstEcrire::NoeudInstEcrire(Noeud* exp, Noeud* ch) {
    this->exp=exp;
    this->ch=ch;

}

/*int NoeudInstEcrire::getValeur() {

 string nom;
 nom = typeid(*contenu).name();
 if (nom == "SymboleValue" && *((SymboleValue*)contenu)=="<CHAINE>")

 cout << ((SymboleValue*)contenu)->getChaine();

 else  {
 cout << contenu->getValeur() << endl;
 }

 return 0;

 }*/

int NoeudInstEcrire::getValeur() {
    if (exp!=NULL)
        cout << exp->getValeur() << endl;
    else
        cout << ((SymboleValue*)ch)->getChaine().substr(1, ((SymboleValue*)ch)->getChaine().size()-2) << endl;
    return 0;
}

void NoeudInstEcrire::afficher(unsigned short indentation) {
    Noeud::afficher(indentation);
    cout << "Noeud - InstEcrire" << endl;

    if (exp!=NULL)
        exp->afficher(indentation+1); // on affiche condition et instruction

    else
        ch->afficher(indentation+1); // on affiche condition et instruction
}

void NoeudInstEcrire::afficherInstAlgo() {
    // affiche la séquence d'instruction sous forme de langage de programmation (C++)
    //Noeud::afficherInstAlgo();
    Noeud::afficherInstAlgo();
    cout << "cout << ";
    if (exp!=NULL)

        exp->afficherInstAlgo(); // affichage de l'expression
    else {

        ch->afficherInstAlgo();//affichage de la chaine

    }

    cout <<";"<<endl;

}
///////////////////////////////////////////////
//Noeud instruction pour
//////////////////////////////////////////////////////////////////////////
NoeudInstPour::NoeudInstPour(Noeud* aff, Noeud* cond, Noeud* incr,
        Noeud* seqinst) {
    this->aff=aff;
    this->cond=cond;
    this->incr=incr;
    this->seqinst=seqinst;
}

int NoeudInstPour::getValeur() {
    for (aff->getValeur(); cond->getValeur(); incr->getValeur())
        seqinst->getValeur();
    return 0;
}

void NoeudInstPour::afficher(unsigned short indentation) {
    Noeud::afficher(indentation);
    cout << "Noeud - InstPour" << endl;

    incr->afficher(indentation+1);//on affiche l'incrémentation
    aff->afficher(indentation+1);// on affiche l'affectation de départ
    cond->afficher(indentation+1); // on affiche condition
    seqinst->afficher(indentation+1);// et instruction
}

void NoeudInstPour::afficherInstAlgo() {
    // affiche la séquence d'instruction sous forme de langage de programmation (C++)
    //Noeud::afficherInstAlgo();
    Noeud::afficherInstAlgo();

    cout << endl<<"for ( ";
    aff->afficherInstAlgo();//  affichage de l'affectation (1) dans le for
    cout <<";";
    cond->afficherInstAlgo();// Affichage de la condition dans le for (2)
    cout <<";";
    incr->afficherInstAlgo();// affichage de l'incrémentation dans le for (3)
    cout << ") { " << endl;
    seqinst->afficherInstAlgo();// affichage de la séquence d'instruction du for
    cout <<endl<< " } " << endl;

}

///////////////////////////////////////////////
//Noeud instruction switch
//////////////////////////////////////////////////////////////////////////
NoeudInstSwitch::NoeudInstSwitch(Noeud* variable) :
    tabCase(), tabInst() {
    this->variable= variable;
    //this->fact2=fact2;
}

int NoeudInstSwitch::getValeur() {
    unsigned int i=0;

    int valeur=variable->getValeur();
    ((SymboleValue*)variable)->setValeur(valeur);

    while (i< tabCase.size()-1 && tabCase[i]->getValeur() != valeur)
        i++;
    if (tabCase[i]==NULL ||tabCase[i]->getValeur() == valeur)
    {
        if (tabInst[i] != NULL)
        tabInst[i]->getValeur();
    }
    return 0;
}

void NoeudInstSwitch::afficher(unsigned short indentation) {
    Noeud::afficher(indentation);
    cout << "Noeud - InstSwitch" << endl;
    for (unsigned int i=0; i<tabCase.size(); i++) {
        cout << "case n° ";
        if (tabCase[i] != NULL) // tabCase=null --> default, pas de numéro dans le vecteur
            tabCase[i]->afficher(indentation+1); // on affiche n° du case
        else
            cout << "default";
            cout << endl;
        if (tabInst[i] != NULL) // tabInst = null --> si aucune instruction , direct break
        tabInst[i]->afficher(indentation+1); //et instruction
    }
}

void NoeudInstSwitch::ajouteInstruction(Noeud* instruction, Noeud* num) {

    tabInst.push_back(instruction);//ajoute l'instruction à la fin du tableau
    tabCase.push_back(num);// ajoute le num du case à la fin du tableau
}

void NoeudInstSwitch::afficherInstAlgo() {
    // affiche la séquence d'instruction sous forme de langage de programmation (C++)

    Noeud::afficherInstAlgo();
    cout <<endl<< "switch (";
    variable->afficherInstAlgo();
    cout << ") {" << endl;

    for (unsigned int i=0; i<tabCase.size(); i++) {

        if (tabCase[i] != NULL) { // idem que procédure précédente
            cout << "case  ";
            tabCase[i]->afficherInstAlgo(); // on affiche n° du case

        } else

            cout << "default ";
            cout << ": ";
            if (tabInst[i] != NULL)// idem procédure précédente
                tabInst[i]->afficherInstAlgo(); //et instruction
            cout << "break; "<< endl;
    }
    cout << "}" << endl;
}
