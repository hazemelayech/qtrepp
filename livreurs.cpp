#include "livreurs.h"
#include <QString>
#include <QSqlQuery>
#include <QDate>
#include <QSqlQueryModel>

livreurs::livreurs()
{
    id_livreur = 0;
    cin = "";
    nom = "";
    prenom = "";
    telephone = "";
    email = "";
    salaire = 0;
    sexe = "";
    date_naissance = QDate::currentDate();
}

livreurs::livreurs(int id_livreur, QString cin, QString nom, QString prenom, QString telephone, QString email, int salaire, QString sexe, QDate date_naissance)
{
    this->id_livreur = id_livreur;
    this->cin = cin;
    this->nom = nom;
    this->prenom = prenom;
    this->telephone = telephone;
    this->email = email;
    this->salaire = salaire;
    this->sexe = sexe;
    this->date_naissance = date_naissance;
}

livreurs::livreurs( QString cin, QString nom, QString prenom, QString telephone, QString email, int salaire, QString sexe, QDate date_naissance)
{
    this->cin = cin;
    this->nom = nom;
    this->prenom = prenom;
    this->telephone = telephone;
    this->email = email;
    this->salaire = salaire;
    this->sexe = sexe;
    this->date_naissance = date_naissance;
}

bool livreurs::ajouter()
{
    QSqlQuery query;

    query.prepare("INSERT INTO LIVREURS (CIN, Nom, Prenom, Telephone, Email, SALAIRE, SEXE, DATE_DE_NAISSANCE) "
                  "VALUES (:CIN, :Nom, :Prenom, :Telephone, :Email, :SALAIRE, :SEXE, :DATE_DE_NAISSANCE)");

    query.bindValue(":CIN", cin);
    query.bindValue(":Nom", nom);
    query.bindValue(":Prenom", prenom);
    query.bindValue(":Telephone", telephone);
    query.bindValue(":Email", email);
    query.bindValue(":SALAIRE", salaire);
    query.bindValue(":SEXE", sexe);
    query.bindValue(":DATE_DE_NAISSANCE", date_naissance);

    return query.exec();

}

bool livreurs::modifier()
{

        QSqlQuery query;

        query.prepare("UPDATE LIVREURS SET CIN = :CIN, Nom = :Nom, Prenom = :Prenom, Telephone = :Telephone,"
                      " Email = :Email, SALAIRE = :SALAIRE, SEXE = :SEXE,"
                      " DATE_DE_NAISSANCE = :DATE_DE_NAISSANCE WHERE ID_Livreur = :ID_Livreur");
        query.bindValue(":ID_Livreur", id_livreur);
        query.bindValue(":CIN", cin);
        query.bindValue(":Nom", nom);
        query.bindValue(":Prenom", prenom);
        query.bindValue(":Telephone", telephone);
        query.bindValue(":Email", email);
        query.bindValue(":SALAIRE", salaire);
        query.bindValue(":SEXE", sexe);
        query.bindValue(":DATE_DE_NAISSANCE", date_naissance);

        return query.exec();

}


bool livreurs::supprimer(int id)
{
    QSqlQuery query;
query.prepare("DELETE FROM LIVREURS WHERE ID_LIVREUR=:id");
query.bindValue(":id",id);
return query.exec();
}

QSqlQueryModel* livreurs::afficher()
{
  QSqlQueryModel* model=new QSqlQueryModel();
  model->setQuery("SELECT * FROM LIVREURS");
  return model;



}

QSqlQueryModel* livreurs::chercher(QString column, QString text)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM LIVREURS WHERE " + column + " LIKE '%" + text + "%'");
    return model;
}

QSqlQueryModel* livreurs::tri(QString column, QString choix)//(ID_LIVREUR ASC)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM LIVREURS ORDER BY " + column + " " + choix);
    return model;
}


int livreurs::countStatusCommandesDunLivreurSpecifique(QString status,int id_livreur)
{
    int count = 0;

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM COMMANDES  "
                  "WHERE ID_LIVREUR = :id AND STATUS = :status");

    if (query.exec() && query.next()) {
        count = query.value(0).toInt();
    }

    return count;
}


QMap<QString, int> livreurs::statistiquesParSexe() {
    QMap<QString, int> SexeStats;

    QSqlQuery query;
    query.prepare("SELECT SEXE, COUNT(*) as count FROM LIVREURS GROUP BY SEXE");
//"HOMME" 2
//"FEMME" 0
    //list=QMap
    if (query.exec()) {
        while (query.next()) {
            QString sexe = query.value(0).toString();
            int count = query.value(1).toInt();
            SexeStats[sexe] = count;
        }
    }

    return SexeStats;
}
int livreurs::chercherIdLivreurDapreFullName(QString livreur_FullName)
{
    int id_livreur = 0;
    QSqlQuery query;

    // Specify the table name in the SELECT statement
    query.prepare("SELECT ID_LIVREUR FROM LIVREURS WHERE prenom || ' ' || nom = :livreur_Fullname");
    query.bindValue(":livreur_Fullname", livreur_FullName);

    if (query.exec() && query.next())
    {
        id_livreur = query.value(0).toInt();
    }

    return id_livreur;
}

bool livreurs::idExists(int id)
{
QSqlQuery query;
query.prepare("SELECT COUNT(*) FROM LIVREURS WHERE ID_LIVREUR = :id");
query.bindValue(":id", id);

if (query.exec() && query.next()) {
    int count = query.value(0).toInt();
    return count > 0;
}

return false;
}

QList<int> livreurs::ListIdLivreur(){
QList<int> Liste;

QSqlQuery query;
query.prepare("SELECT ID_LIVREUR FROM LIVREURS ");
if(query.exec())
{
    while(query.next())
    {
        Liste.append(query.value(0).toInt());
    }
}
return Liste;

}


QString livreurs::ChercherEmailById(int id_livreur)
{
    QString email = "";
    QSqlQuery query;

    // Specify the table name in the SELECT statement
    query.prepare("SELECT EMAIL FROM LIVREURS WHERE id_livreur = :id_livreur");
    query.bindValue(":id_livreur", id_livreur);

    if (query.exec() && query.next())
    {
        email = query.value(0).toString();
    }

    return email;
}
