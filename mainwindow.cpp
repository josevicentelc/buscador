#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    buscador = new Buscador("indice", ui->comboBox->currentIndex());
    cout << "Indice recuperado"<<endl;
    updateLabels();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Indexar(){

}


void MainWindow::on_botonIndexar_clicked()
{
  string dir = ui->lineEdit_2->text().toStdString();

  ui->botonIndexar->setEnabled(false);

  cout << "Indexando..."<<endl;
    buscador->IndexarDirectorio(ui->lineEdit_2->text().toStdString());
    ui->botonIndexar->setEnabled(true);
    updateLabels();
  cout << "Fin de la indexación..."<<endl;
}

void MainWindow::on_cargarIndice_clicked()
{
    cout << "Cargando el indice"<<endl;
        buscador = new Buscador("indice", ui->comboBox->currentIndex());
        updateLabels();
    cout << "Indice cargado"<<endl;
}

void MainWindow::updateLabels(){
    ui->numDocumentos->setText(QString::fromStdString(to_string(buscador->NumDocsIndexados())));
    ui->numPalabras->setText(QString::fromStdString(to_string(buscador->NumPalIndexadas())));
}

void MainWindow::on_ButonBorrarIndice_clicked()
{
    buscador->VaciarIndice();
    updateLabels();
}





void MainWindow::on_buscar_clicked()
{
    string query = ui->Q->text().toStdString();
    buscador->IndexarPregunta(query);


    string salida;
    salida += "busqueda para: \n";
    salida += query + "\n\n";

    cout << "buscando"<<endl;
    buscador->Buscar(10);
    cout << "fin de busqueda"<<endl;
    for (int i=0;i<10;i++){
        salida += buscador->getResult(i);
        //ui->lista->additem("asd");
        salida += ".\n";
    }
    ui->textEdit->setText(QString::fromStdString(salida));


}

void MainWindow::on_GuardarIndice_clicked()
{
    buscador->GuardarIndexacion();
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if (ui->comboBox->currentIndex() == 0)
        buscador->setStopWords("StopWordsEspanyol.txt");
    else
        buscador->setStopWords("StopWordsIngles.txt");

}

void MainWindow::on_textEdit_textChanged()
{

}

void MainWindow::on_algoritmo_currentIndexChanged(const QString &arg1)
{
        buscador->CambiarFormulaSimilitud(ui->algoritmo->currentIndex());
}

void MainWindow::on_buscarPrecision_clicked()
{
    int numResultados = 10;
    buscador->Buscar(ui->textDirQuerys->text().toStdString(), numResultados, ui->pregInicio->value(), ui->pregFin->value());

    buscador->ImprimirResultadoBusqueda((ui->pregFin->value()-ui->pregInicio->value()+1)*numResultados, ui->textFicSalida->text().toStdString());

}
