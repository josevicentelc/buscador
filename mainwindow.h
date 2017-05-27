#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Buscador.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //void on_pushButton_3_clicked();

    void on_botonIndexar_clicked();

    void on_cargarIndice_clicked();

    void on_ButonBorrarIndice_clicked();

    void on_buscar_clicked();

    void on_GuardarIndice_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_textEdit_textChanged();

    void on_algoritmo_currentIndexChanged(const QString &arg1);

    void on_buscarPrecision_clicked();

private:
    Ui::MainWindow *ui;
    Buscador* buscador;
    void Indexar();
    void updateLabels();


};

#endif // MAINWINDOW_H
