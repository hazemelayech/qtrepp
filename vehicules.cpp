#include "vehicules.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>

vehicule::vehicule()
{
    id_vehicule = 0;
    marque = "";
    modele = "";
    annee = 0;
    matricule = "";
    id_livreur = 0;
}

vehicule::vehicule(int id_vehicule, QString marque, QString modele, int annee, QString matricule, int id_livreur)
{
    this->id_vehicule = id_vehicule;
    this->marque = marque;
    this->modele = modele;
    this->annee = annee;
    this->matricule = matricule;
    this->id_livreur = id_livreur;
}
vehicule::vehicule( QString marque, QString modele, int annee, QString matricule, int id_livreur)
{
    this->marque = marque;
    this->modele = modele;
    this->annee = annee;
    this->matricule = matricule;
    this->id_livreur = id_livreur;
}

bool vehicule::ajouter()
{
    QSqlQuery query;

    query.prepare("INSERT INTO VEHICULES (Marque, Modele, Annee, Matricule, ID_Livreur) "
                  "VALUES (:Marque, :Modele, :Annee, :Matricule, :ID_Livreur)");

    query.bindValue(":Marque", marque);
    query.bindValue(":Modele", modele);
    query.bindValue(":Annee", annee);
    query.bindValue(":Matricule", matricule);
    query.bindValue(":ID_Livreur", id_livreur);

return query.exec();


}

bool vehicule::modifier()
{
    QSqlQuery query;

    query.prepare("UPDATE VEHICULES SET Marque = :Marque, Modele = :Modele, Annee = :Annee, "
                  "Matricule = :Matricule ,ID_LIVREUR=:ID_LIVREUR WHERE ID_Vehicule = :ID_Vehicule");

    query.bindValue(":ID_Vehicule", id_vehicule);
    query.bindValue(":Marque", marque);
    query.bindValue(":Modele", modele);
    query.bindValue(":Annee", annee);
    query.bindValue(":Matricule", matricule);
    query.bindValue(":ID_LIVREUR", id_livreur);

    return query.exec();
}

bool vehicule::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM VEHICULES WHERE ID_Vehicule = :id");
    query.bindValue(":id", id);
    return query.exec();
}

QSqlQueryModel* vehicule::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT V.ID_VEHICULE, V.MARQUE, V.MODELE, V.ANNEE, V.MATRICULE, V.ID_LIVREUR, L.nom, L.prenom FROM VEHICULES V "
                    "JOIN LIVREURS L ON L.ID_LIVREUR = V.ID_LIVREUR");
    return model;
}

QSqlQueryModel* vehicule::chercher(QString column, QString text)//id_vehicule  BMW , MW
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM VEHICULES WHERE " + column + " LIKE '%" + text + "%'");
    return model;
}

QSqlQueryModel* vehicule::tri(QString column, QString choix)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM VEHICULES ORDER BY " + column + " " + choix);
    return model;
}


bool vehicule::idExists(int id)
{
QSqlQuery query;
query.prepare("SELECT COUNT(*) FROM VEHICULES WHERE ID_VEHICULE = :id");
query.bindValue(":id", id);

if (query.exec() && query.next()) {
    int count = query.value(0).toInt();
    return count > 0;
}

return false;
}

QMap<QString, int> vehicule::statistiquesParMarque() {
    QMap<QString, int> MarqueStats;

    QSqlQuery query;
    query.prepare("SELECT MARQUE, COUNT(*) as count FROM VEHICULES GROUP BY MARQUE");

    if (query.exec()) {
        while (query.next()) {
            QString Marque = query.value(0).toString();
            int count = query.value(1).toInt();
            MarqueStats[Marque] = count;
        }
    }

    return MarqueStats;
}
