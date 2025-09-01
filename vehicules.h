#ifndef VEHICULE_H
#define VEHICULE_H

#include <QString>
#include <QSqlQueryModel>
#include <QSqlQuery>

class vehicule
{
public:
    vehicule();
    vehicule(int id_vehicule, QString marque, QString modele, int annee, QString matricule, int id_livreur);
    vehicule(QString marque, QString modele, int annee, QString matricule, int id_livreur);

    // Getters et Setters
    int getIdVehicule() const { return id_vehicule; }
    void setIdVehicule(int id_vehicule) { this->id_vehicule = id_vehicule; }

    QString getMarque() const { return marque; }
    void setMarque(QString marque) { this->marque = marque; }

    QString getModele() const { return modele; }
    void setModele(QString modele) { this->modele = modele; }

    int getAnnee() const { return annee; }
    void setAnnee(int annee) { this->annee = annee; }

    QString getMatricule() const { return matricule; }
    void setMatricule(QString matricule) { this->matricule = matricule; }

    int getIdLivreur() const { return id_livreur; }
    void setIdLivreur(int id_livreur) { this->id_livreur = id_livreur; }

    // CRUD
    bool ajouter();
    bool modifier();
    bool supprimer(int id);
    QSqlQueryModel* afficher();
    bool idExists(int id);

    //metier
    QSqlQueryModel* chercher(QString column, QString text);
    QSqlQueryModel* tri(QString column, QString choix);
    QMap<QString, int> statistiquesParMarque();

private:
    int id_vehicule;
    QString marque;
    QString modele;
    int annee;
    QString matricule;
    int id_livreur;
};

#endif // VEHICULE_H
