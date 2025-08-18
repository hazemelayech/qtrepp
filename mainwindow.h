#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vehicules.h"
#include "livreurs.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_bt_livreur_to_vehicule_clicked();

    void on_bt_vehicule_to_livreur_clicked();

    void on_bt_ajouter_livreur_clicked();

    void on_bt_supprimer_livreur_clicked();

    void on_bt_modifier_livreur_clicked();

    void on_tableView_livreur_clicked(const QModelIndex &index);

    void on_bt_trier_livreur_clicked();

    void on_lineEdit_chercher_livreur_textChanged(const QString &text);

    void on_bt_pdf_livreur_clicked();

    void on_bt_stat_livreur_clicked();

private:
    Ui::MainWindow *ui;
vehicule v;
livreurs l;
};
#endif // MAINWINDOW_H
