#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QPrinter>
#include <QTextDocument>
#include <QTextStream>
#include <QVBoxLayout>
#include <QPieSeries>
#include <QChart>
#include <QChartView>
#include "email.h"
using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //lors du l'excution

    //livreur
    ui->tableView_livreur->setModel(l.afficher());
ui->id_livreur->hide();

    QIntValidator *intValidator = new QIntValidator(0, 100000000, this); // Only allows integers from 0 to 100000
ui->cin_livreur->setValidator(intValidator);
ui->telephone_livreur->setValidator(intValidator);
ui->salaire_livreur->setValidator(intValidator);
ui->anne_vehicule->setValidator(intValidator);

QRegularExpression charRegex("[a-zA-Z]*");
QRegularExpressionValidator* charValidator = new QRegularExpressionValidator(charRegex, this);
ui->nom_livreur->setValidator(charValidator);
ui->prenom_livreur->setValidator(charValidator);



    //vehicule
ui->tableView_vehicule->setModel(v.afficher());
ui->id_vehicule->hide();
remplir_comboBox_Livreur();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_bt_livreur_to_vehicule_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_bt_vehicule_to_livreur_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

}

void MainWindow::on_bt_ajouter_livreur_clicked()
{
    //Etape1:recuperation des donné d'apres ui !
    QString cin=ui->cin_livreur->text();
    QString nom=ui->nom_livreur->text();
    QString prenom=ui->prenom_livreur->text();
    QString email=ui->email_livreur->text();
    QString telephone=ui->telephone_livreur->text();
    int salaire=ui->salaire_livreur->text().toInt();
    QString sexe=ui->sexe_livreur->currentText();
    QDate naissance=ui->naissance_livreur->date();

    //etape2:Controle de saisie
if(telephone==""|| email==""||nom==""||prenom==""|| cin==""||salaire==0)
{
    QMessageBox::warning(nullptr, QObject::tr(""),
                QObject::tr("Tu dois remplir tous les champs "), QMessageBox::Cancel);
    return;
}
if(telephone.length()!=8)
{
    QMessageBox::warning(nullptr, QObject::tr(""),
                QObject::tr("Le numero doit etre 8 chiffre"), QMessageBox::Cancel);
    return;

}

QRegularExpression emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");//aa@aa.aa
// Check if the email is valid
if (!emailRegex.match(email).hasMatch()) {
    QMessageBox::critical(nullptr, QObject::tr(""),
                          QObject::tr("L'adresse email est invalide"), QMessageBox::Cancel);
    return;
}

QDate eighteenYearsAgo = QDate::currentDate().addYears(-18);

if (naissance > eighteenYearsAgo) {
    QMessageBox::critical(nullptr, QObject::tr(""),
                          QObject::tr("Tu dois être +18"), QMessageBox::Cancel);
    return;
}

    //etape3:appel du fonction ajout !!
livreurs l(cin,nom,prenom,telephone,email,salaire,sexe,naissance);
bool test=l.ajouter();
    //etape4: test Si true show message , else show error
if(test)
{
    QMessageBox::information(nullptr, QObject::tr(""),
                QObject::tr("Ajout avec succées"), QMessageBox::Cancel);

ui->tableView_livreur->setModel(l.afficher());

ui->cin_livreur->clear();
ui->nom_livreur->clear();
ui->prenom_livreur->clear();
ui->telephone_livreur->clear();
ui->email_livreur->clear();
ui->sexe_livreur->setCurrentIndex(0);
ui->salaire_livreur->clear();
ui->naissance_livreur->clear();

remplir_comboBox_Livreur();
}
else
{
    QMessageBox::critical(nullptr, QObject::tr(""),
                QObject::tr("Ajout echoué"), QMessageBox::Cancel);
}

}

void MainWindow::on_bt_supprimer_livreur_clicked()
{
    int id_livreur=ui->id_supprimer_livreur->text().toInt();
if(l.idExists(id_livreur))
{
    bool test=l.supprimer(id_livreur);
    if(test)
    {
        QMessageBox::information(nullptr, QObject::tr(""),
                    QObject::tr("Suppression avec succées"), QMessageBox::Cancel);
        ui->id_supprimer_livreur->clear();
        ui->tableView_livreur->setModel(l.afficher());
        remplir_comboBox_Livreur();
        addToHistory("Suppression du livreur", QString::number(id_livreur));

    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr(""),
                    QObject::tr("Suppression echoué"), QMessageBox::Cancel);
    }
}
else
{
    QMessageBox::warning(nullptr, QObject::tr(""),
                QObject::tr("Le id n'existe pas"), QMessageBox::Cancel);
    return;
}

}


void MainWindow::on_bt_modifier_livreur_clicked()
{
    //Etape1:recuperation des donné d'apres ui !
    QString cin=ui->cin_livreur->text();
    QString nom=ui->nom_livreur->text();
    QString prenom=ui->prenom_livreur->text();
    QString email=ui->email_livreur->text();
    QString telephone=ui->telephone_livreur->text();
    int salaire=ui->salaire_livreur->text().toInt();
    QString sexe=ui->sexe_livreur->currentText();
    QDate naissance=ui->naissance_livreur->date();
    int id_livreur=ui->id_livreur->text().toInt();
    //etape2:Controle de saisie
if(telephone==""|| email==""||nom==""||prenom==""|| cin==""||salaire==0)
{
    QMessageBox::warning(nullptr, QObject::tr(""),
                QObject::tr("Tu dois remplir tous les champs "), QMessageBox::Cancel);
    return;
}
if(telephone.length()!=8)
{
    QMessageBox::warning(nullptr, QObject::tr(""),
                QObject::tr("Le numero doit etre 8 chiffre"), QMessageBox::Cancel);
    return;

}

QRegularExpression emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");//aa@aa.aa
// Check if the email is valid
if (!emailRegex.match(email).hasMatch()) {
    QMessageBox::critical(nullptr, QObject::tr(""),
                          QObject::tr("L'adresse email est invalide"), QMessageBox::Cancel);
    return;
}

QDate eighteenYearsAgo = QDate::currentDate().addYears(-18);

if (naissance > eighteenYearsAgo) {
    QMessageBox::critical(nullptr, QObject::tr(""),
                          QObject::tr("Tu dois être +18"), QMessageBox::Cancel);
    return;
}

    //etape3:appel du fonction ajout !!
livreurs l(id_livreur,cin,nom,prenom,telephone,email,salaire,sexe,naissance);
bool test=l.modifier();
    //etape4: test Si true show message , else show error
if(test)
{
    QMessageBox::information(nullptr, QObject::tr(""),
                QObject::tr("Modification avec succées"), QMessageBox::Cancel);

ui->tableView_livreur->setModel(l.afficher());

ui->cin_livreur->clear();
ui->nom_livreur->clear();
ui->prenom_livreur->clear();
ui->telephone_livreur->clear();
ui->email_livreur->clear();
ui->sexe_livreur->setCurrentIndex(0);
ui->salaire_livreur->clear();
ui->naissance_livreur->clear();

addToHistory("Mise à jour du livreur", QString::number(id_livreur));

}
else
{
    QMessageBox::critical(nullptr, QObject::tr(""),
                QObject::tr("Modification echoué"), QMessageBox::Cancel);
}

}

void MainWindow::on_tableView_livreur_clicked(const QModelIndex &index)
{
    //etape1: numero du ligne ! clicked !
    QAbstractItemModel* model = ui->tableView_livreur->model();

        int row = index.row();
    //etape2:  table du format matrice , recuperation du donne
        QString id_livreur = model->data(model->index(row, 0)).toString();
        QString cin = model->data(model->index(row, 1)).toString();
        QString nom = model->data(model->index(row, 2)).toString();
        QString prenom = model->data(model->index(row, 3)).toString();
        QString telephone = model->data(model->index(row, 4)).toString();
        QString email = model->data(model->index(row, 5)).toString();
        QString salaire = model->data(model->index(row, 6)).toString();
        QString sexe = model->data(model->index(row, 7)).toString();
        QDate naissance = model->data(model->index(row, 8)).toDate();

        //etape3: insertion dans ui  ( lineEdit , comboBox)

        ui->id_livreur->setText(id_livreur);
        ui->cin_livreur->setText(cin);
        ui->nom_livreur->setText(nom);
        ui->prenom_livreur->setText(prenom);
        ui->telephone_livreur->setText(telephone);
        ui->email_livreur->setText(email);
        ui->salaire_livreur->setText(salaire);
        ui->sexe_livreur->setCurrentText(sexe);
        ui->naissance_livreur->setDate(naissance);

}

void MainWindow::on_bt_trier_livreur_clicked()
{
    QString choix=ui->comboBox_choix_livreur->currentText();
    QString ordre=ui->comboBox_ordre_livreur->currentText();

    ui->tableView_livreur->setModel(l.tri(choix,ordre));
}

void MainWindow::on_lineEdit_chercher_livreur_textChanged(const QString &text)
{
    QString choix=ui->comboBox_choix_livreur->currentText();

ui->tableView_livreur->setModel(l.chercher(choix,text));
}

void MainWindow::on_bt_pdf_livreur_clicked()
{
    QString strStream;
    QTextStream out(&strStream);

    const int rowCount = ui->tableView_livreur->model()->rowCount();
    const int columnCount = ui->tableView_livreur->model()->columnCount();

    out << "<html>\n"
           "<head>\n"
           "<meta Content=\"Text/html; charset=Windows-1251\">\n"
           "<title>%1</title>\n"
           "<style>\n"
           "table {\n"
           "    width: 100%;\n"
           "    border-collapse: collapse;\n"
           "}\n"
           "th, td {\n"
           "    padding: 8px;\n"
           "    text-align: left;\n"
           "    border-bottom: 1px solid #ddd;\n"
           "}\n"
           "tr:nth-child(even) {\n"
           "    background-color: #f2f2f2;\n"
           "}\n"
           "</style>\n"
           "</head>\n"
           "<body bgcolor=#ffffff link=#5000A0>\n"
           "<center> <H1>Liste des Livreurs</H1></center><br/><br/>\n"
           "<img src=\"path/to/your/image.jpg\" alt=\"Description of image\" style=\"max-width: 100%; height: auto;\">\n"
           "<table>\n";

    // headers
    out << "<thead><tr bgcolor=#f0f0f0> <th>Numero</th>";
    for (int column = 0; column < columnCount; column++)
    {
        if (!ui->tableView_livreur->isColumnHidden(column))
        {
            out << QString("<th>%1</th>").arg(ui->tableView_livreur->model()->headerData(column, Qt::Horizontal).toString());
        }
    }
    out << "</tr></thead>\n";

    // data table
    for (int row = 0; row < rowCount; row++)
    {
        out << "<tr> <td>" << row + 1 << "</td>";
        for (int column = 0; column < columnCount; column++)
        {
            if (!ui->tableView_livreur->isColumnHidden(column))
            {
                QString data = ui->tableView_livreur->model()->data(ui->tableView_livreur->model()->index(row, column)).toString().simplified();
                out << QString("<td>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
            }
        }
        out << "</tr>\n";
    }



    QString fileName = QFileDialog::getSaveFileName((QWidget *)0, "Sauvegarder en PDF", QString(), "*.pdf");
    if (QFileInfo(fileName).suffix().isEmpty())
    {
        fileName.append(".pdf");
    }

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPrinter::A4);
    printer.setOutputFileName(fileName);

    QTextDocument doc;
    doc.setHtml(strStream);
    doc.print(&printer);

}

void MainWindow::on_bt_stat_livreur_clicked()
{
        QMap<QString, int> stats = l.statistiquesParSexe();

        QPieSeries *series = new QPieSeries();
        for (auto it = stats.begin(); it != stats.end(); ++it) {
            series->append(it.key(), it.value());
        }

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Statistiques des Livreurs par Sexe");

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        QDialog *chartDialog = new QDialog(this);
        chartDialog->setWindowTitle("Graphique des Statistiques");
        chartDialog->setFixedSize(480, 240);
        QVBoxLayout *layout = new QVBoxLayout();
        layout->addWidget(chartView);
        chartDialog->setLayout(layout);

        chartDialog->exec();
}

void MainWindow::remplir_comboBox_Livreur()
{
    ui->livreur_vehicule->clear();

QList<int> liste=l.ListIdLivreur();

for(int i=0 ;i<liste.length();i++)
{
    ui->livreur_vehicule->addItem(QString::number(liste[i]));
}


}

void MainWindow::on_bt_ajouter_vehicule_clicked()
{
    QString marque=ui->marque_vehicule->text();
    QString modele=ui->modele_vehicule->text();
    QString matricule=ui->matricule_vehicule->text();
    int annee=ui->anne_vehicule->text().toInt();
    int id_livreur=ui->livreur_vehicule->currentText().toInt();

    if(marque=="" || modele==""||matricule ==""||annee==0)
    {
        QMessageBox::critical(nullptr, QObject::tr(""),
                    QObject::tr("Tu dois remplir tous les champs "), QMessageBox::Cancel);
        return;
    }

    int TenYearsAgo = QDate::currentDate().addYears(-10).year();

    if (annee < TenYearsAgo) {
        QMessageBox::critical(nullptr, QObject::tr(""),
                              QObject::tr("La date de fabiraction du vehicule doit etre maximum 10 anneé"), QMessageBox::Cancel);
        return;
    }

    QRegularExpression MatriculeRegex(R"(^\d{3}TUN\d{1,4}$)");

    if (!MatriculeRegex.match(matricule).hasMatch()) {
        QMessageBox::critical(nullptr, QObject::tr(""),
                              QObject::tr("La matricule doit etre du cetter format ###TUN####"), QMessageBox::Cancel);
        return;
    }

    vehicule v(marque,modele,annee,matricule,id_livreur);
bool test=v.ajouter();

if(test)
{
    QMessageBox::information(nullptr, QObject::tr(""),
                          QObject::tr("Ajout avec succées"), QMessageBox::Cancel);
    ui->tableView_vehicule->setModel(v.afficher());

    ui->marque_vehicule->clear();
    ui->matricule_vehicule->clear();
    ui->anne_vehicule->clear();
    ui->modele_vehicule->clear();

    QString email=l.ChercherEmailById(id_livreur);
    QString message = "Bonjour,\n\n"
                      "Je vous écris pour vous informer que vous avez été assigné à un véhicule.\n\n"
                      "Détails du véhicule :\n"
                      "Marque : " + marque + "\n"
                      "Modèle : " + modele + "\n"
                      "Matricule : " + matricule + "\n"
                      "Année : " + QString::number(annee) + "\n\n"
                      "Si vous avez des questions ou besoin de plus d'informations, n'hésitez pas à me contacter.\n\n"
                      "Cordialement,\n";

    mailer::sendEmail(email,"Ajout du vehicule",message );
}
else
{
    QMessageBox::critical(nullptr, QObject::tr(""),
                          QObject::tr("Ajout échoué"), QMessageBox::Cancel);
}
}



void MainWindow::on_bt_modifier_vehicule_clicked()
{
    QString marque=ui->marque_vehicule->text();
    QString modele=ui->modele_vehicule->text();
    QString matricule=ui->matricule_vehicule->text();
    int annee=ui->anne_vehicule->text().toInt();
    int id_livreur=ui->livreur_vehicule->currentText().toInt();
    int id_vehicule=ui->id_vehicule->text().toInt();

    if(marque=="" || modele==""||matricule ==""||annee==0)
    {
        QMessageBox::critical(nullptr, QObject::tr(""),
                    QObject::tr("Tu dois remplir tous les champs "), QMessageBox::Cancel);
        return;
    }

    int TenYearsAgo = QDate::currentDate().addYears(-10).year();

    if (annee < TenYearsAgo) {
        QMessageBox::critical(nullptr, QObject::tr(""),
                              QObject::tr("La date de fabiraction du vehicule doit etre maximum 10 anneé"), QMessageBox::Cancel);
        return;
    }

    QRegularExpression MatriculeRegex(R"(^\d{3}TUN\d{1,4}$)");

    if (!MatriculeRegex.match(matricule).hasMatch()) {
        QMessageBox::critical(nullptr, QObject::tr(""),
                              QObject::tr("La matricule doit etre du cetter format ###TUN####"), QMessageBox::Cancel);
        return;
    }

    vehicule v(id_vehicule,marque,modele,annee,matricule,id_livreur);
bool test=v.modifier();

if(test)
{
    QMessageBox::information(nullptr, QObject::tr(""),
                          QObject::tr("Modification avec succées"), QMessageBox::Cancel);
    ui->tableView_vehicule->setModel(v.afficher());

    ui->marque_vehicule->clear();
    ui->matricule_vehicule->clear();
    ui->anne_vehicule->clear();
    ui->modele_vehicule->clear();

    QString email=l.ChercherEmailById(id_livreur);
    QString message = "Bonjour,\n\n"
                      "Je vous écris pour vous informer que vous avez été assigné à un véhicule.\n\n"
                      "Détails du véhicule :\n"
                      "Marque : " + marque + "\n"
                      "Modèle : " + modele + "\n"
                      "Matricule : " + matricule + "\n"
                      "Année : " + QString::number(annee) + "\n\n"
                      "Si vous avez des questions ou besoin de plus d'informations, n'hésitez pas à me contacter.\n\n"
                      "Cordialement,\n";

    mailer::sendEmail(email,"Modification du vehicule",message );
}
else
{
    QMessageBox::critical(nullptr, QObject::tr(""),
                          QObject::tr("Modification échoué"), QMessageBox::Cancel);
}
}


void MainWindow::on_tableView_vehicule_clicked(const QModelIndex &index)
{
    //etape1: numero du ligne ! clicked !
    QAbstractItemModel* model = ui->tableView_vehicule->model();

        int row = index.row();
    //etape2:  table du format matrice , recuperation du donne
        QString id_vehicule = model->data(model->index(row, 0)).toString();
        QString marque = model->data(model->index(row, 1)).toString();
        QString modele = model->data(model->index(row, 2)).toString();
        QString anne = model->data(model->index(row, 3)).toString();
        QString matricule = model->data(model->index(row, 4)).toString();
        QString id_livreur = model->data(model->index(row, 5)).toString();

        //etape3: insertion dans ui  ( lineEdit , comboBox)

        ui->id_vehicule->setText(id_vehicule);
        ui->marque_vehicule->setText(marque);
        ui->modele_vehicule->setText(modele);
        ui->anne_vehicule->setText(anne);
        ui->matricule_vehicule->setText(matricule);
        ui->livreur_vehicule->setCurrentText(id_livreur);

}

void MainWindow::on_bt_supprimer_vehicule_clicked()
{
    int id_vehicule=ui->id_supprimer_vehicule->text().toInt();
if(v.idExists(id_vehicule))
{
    bool test=v.supprimer(id_vehicule);
    if(test)
    {
        QMessageBox::information(nullptr, QObject::tr(""),
                    QObject::tr("Suppression avec succées"), QMessageBox::Cancel);
        ui->id_supprimer_vehicule->clear();
        ui->tableView_vehicule->setModel(v.afficher());
        remplir_comboBox_Livreur();

    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr(""),
                    QObject::tr("Suppression echoué"), QMessageBox::Cancel);
    }
}
else
{
    QMessageBox::warning(nullptr, QObject::tr(""),
                QObject::tr("Le id n'existe pas"), QMessageBox::Cancel);
    return;
}

}

void MainWindow::on_bt_trier_vehicule_clicked()
{
    QString choix=ui->comboBox_choix_vehicule->currentText();
    QString ordre=ui->comboBox_ordre_vehicule->currentText();
    ui->tableView_vehicule->setModel(v.tri(choix,ordre));

}

void MainWindow::on_lineEdit_chercher_vehicule_textChanged(const QString &text)
{
    QString choix=ui->comboBox_choix_vehicule->currentText();
    ui->tableView_vehicule->setModel(v.chercher(choix,text));

}

void MainWindow::on_bt_pdf_vehicule_clicked()
{
    QString strStream;
    QTextStream out(&strStream);

    const int rowCount = ui->tableView_vehicule->model()->rowCount();
    const int columnCount = ui->tableView_vehicule->model()->columnCount();

    out << "<html>\n"
           "<head>\n"
           "<meta Content=\"Text/html; charset=Windows-1251\">\n"
           "<title>%1</title>\n"
           "<style>\n"
           "table {\n"
           "    width: 100%;\n"
           "    border-collapse: collapse;\n"
           "}\n"
           "th, td {\n"
           "    padding: 8px;\n"
           "    text-align: left;\n"
           "    border-bottom: 1px solid #ddd;\n"
           "}\n"
           "tr:nth-child(even) {\n"
           "    background-color: #f2f2f2;\n"
           "}\n"
           "</style>\n"
           "</head>\n"
           "<body bgcolor=#ffffff link=#5000A0>\n"
           "<center> <H1>Liste des Vehicules</H1></center><br/><br/>\n"
           "<img src=\"path/to/your/image.jpg\" alt=\"Description of image\" style=\"max-width: 100%; height: auto;\">\n"
           "<table>\n";

    // headers
    out << "<thead><tr bgcolor=#f0f0f0> <th>Numero</th>";
    for (int column = 0; column < columnCount; column++)
    {
        if (!ui->tableView_vehicule->isColumnHidden(column))
        {
            out << QString("<th>%1</th>").arg(ui->tableView_vehicule->model()->headerData(column, Qt::Horizontal).toString());
        }
    }
    out << "</tr></thead>\n";

    // data table
    for (int row = 0; row < rowCount; row++)
    {
        out << "<tr> <td>" << row + 1 << "</td>";
        for (int column = 0; column < columnCount; column++)
        {
            if (!ui->tableView_vehicule->isColumnHidden(column))
            {
                QString data = ui->tableView_vehicule->model()->data(ui->tableView_vehicule->model()->index(row, column)).toString().simplified();
                out << QString("<td>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
            }
        }
        out << "</tr>\n";
    }



    QString fileName = QFileDialog::getSaveFileName((QWidget *)0, "Sauvegarder en PDF", QString(), "*.pdf");
    if (QFileInfo(fileName).suffix().isEmpty())
    {
        fileName.append(".pdf");
    }

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPrinter::A4);
    printer.setOutputFileName(fileName);

    QTextDocument doc;
    doc.setHtml(strStream);
    doc.print(&printer);

}

void MainWindow::on_bt_stat_vehicule_clicked()
{
        QMap<QString, int> stats = v.statistiquesParMarque();

        QPieSeries *series = new QPieSeries();
        for (auto it = stats.begin(); it != stats.end(); ++it) {
            series->append(it.key(), it.value());
        }

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Statistiques des vehicules par Marque");

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        QDialog *chartDialog = new QDialog(this);
        chartDialog->setWindowTitle("Graphique des Statistiques");
        chartDialog->setFixedSize(480, 240);
        QVBoxLayout *layout = new QVBoxLayout();
        layout->addWidget(chartView);
        chartDialog->setLayout(layout);

        chartDialog->exec();
}



void MainWindow::addToHistory(const QString &action, const QString &serviceName)
{
    // Chemin du fichier historique
    QString filePath = "/Users/DELL/Desktop/github/historique.txt";
    QFile file(filePath);

    // Ouvrir le fichier en mode ajout
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        return;
    }

    // Créer un flux texte pour écrire dans le fichier
    QTextStream out(&file);

    // Obtenir la date et l'heure actuelles
    QDateTime currentDateTime = QDateTime::currentDateTime();

    // Écrire l'action dans le fichier historique avec la date et l'heure
    out << currentDateTime.toString("yyyy-MM-dd hh:mm:ss") << " - " << action;
    if (!serviceName.isEmpty()) {
        out << " : " << serviceName;
    }
    out << "\n";

    // Fermer le fichier
    file.close();
}

void MainWindow::on_bt_historique_clicked()
{
    QString filePath = "/Users/DELL/Desktop/github/historique.txt";
    QFile file(filePath);

    // Vérifier si le fichier peut être ouvert en mode lecture
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier d'historique.");
        return;
    }

    // Lire tout le contenu du fichier historique
    QTextStream in(&file);
    QString historyContent = in.readAll();

    // Afficher le contenu dans une boîte de dialogue ou une zone de texte
    QMessageBox::information(this, "Historique des actions", historyContent);

    // Fermer le fichier
    file.close();
}

void MainWindow::on_bt_tableau_2_clicked()
{
    ui->tableView_vehicule->setModel(v.afficher());
}

void MainWindow::on_bt_tableau_clicked()
{
    ui->tableView_livreur->setModel(l.afficher());
}
