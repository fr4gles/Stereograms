#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//    qDebug() << QImageReader::supportedImageFormats ();
    wczytano = false;
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->setSizeIncrement(ui->centralWidget->width(),ui->centralWidget->height());

    setElementsDisabled(true);

    connect(&_stereogramGenerator,SIGNAL(setStatusBarLabel(QString)),this,SLOT(onStatusBarChanged(QString)));

    onStatusBarChanged("");

    ui->pushButton_Koniec->setIcon(QIcon(":new/icon/exit.png"));
    ui->pushButton_Open->setIcon(QIcon(":new/icon/Open.png"));
    ui->pushButton_Save->setIcon(QIcon(":new/icon/Save.png"));
    ui->pushButton->setIcon(QIcon(":new/icon/back.png"));
    ui->pushButton_wg_Menu->setIcon(QIcon(":new/icon/back.png"));
    ui->pushButton_2_end_menu->setIcon(QIcon(":new/icon/back.png"));
    ui->pushButton_scen_menu->setIcon(QIcon(":new/icon/back.png"));
    ui->pushButton_scen2_menu->setIcon(QIcon(":new/icon/back.png"));
    ui->pushButton_Generate->setIcon(QIcon(":new/icon/stereo.png"));
    ui->pushButton_Games->setIcon(QIcon(":new/icon/play.png"));
    ui->pushButton_wg_Graj->setIcon(QIcon(":new/icon/play.png"));
    ui->pushButton_Oprogramie->setIcon(QIcon(":new/icon/about.png"));
    ui->pushButton_Scenariusz->setIcon(QIcon(":new/icon/magic.png"));
    ui->pushButton_game_Ok->setIcon(QIcon(":new/icon/tick.png"));
    ui->pushButton_scen2_ok->setIcon(QIcon(":new/icon/tick.png"));
    ui->pushButton_scen_add->setIcon(QIcon(":new/icon/add.png"));
    ui->pushButton_scen2_add->setIcon(QIcon(":new/icon/add.png"));
    ui->pushButton_scen_dalej->setIcon(QIcon(":new/icon/next.png"));
    ui->pushButton_oprogramie_menu->setIcon(QIcon(":new/icon/back.png"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setElementsDisabled(bool disabled)
{
    ui->label_DPI->setDisabled(disabled);
    ui->label_rozdzielczosc->setDisabled(disabled);
    ui->label_RozstawOczu->setDisabled(disabled);

    ui->pushButton_Save->setDisabled(disabled);

    ui->checkBox->setDisabled(disabled);
    ui->checkBox_keepAspectRatio->setDisabled(disabled);
    ui->checkBox_dopasujDoOkna->setDisabled(disabled);

    ui->comboBox->setDisabled(disabled);
    ui->comboBox_DPI->setDisabled(disabled);
    ui->comboBox_rozdzielczosc->setDisabled(disabled);
    ui->comboBox_RozstawOczu->setDisabled(disabled);

    ui->checkBox_wkleslosc->setDisabled(disabled);
}

void MainWindow::on_pushButton_Generate_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    onStatusBarChanged(QString("Wybierz plik aby wygenerowac."));
}

void MainWindow::openFile()
{
    if(wczytano)
        delete imageOrg;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Obrazki w skali szaroœci (*.jpg *.png *.bmp);;Wszystkie pliki (*.*)"));
    try
    {
        if (!fileName.isEmpty())
        {
           wczytano = wczytajPlik(fileName);
           setElementsDisabled(false);
        }
    }
    catch(std::exception &e)
    {
        qDebug() << "Blad openFile() ";
    }

}

void MainWindow::saveFile()
{
    imageToSave = imageOrg;

    StereogramGenerator *generatedToSave = new StereogramGenerator();
    generatedToSave->changeDefault(ui->comboBox_DPI->currentIndex(),ui->comboBox_RozstawOczu->currentIndex());
    generatedToSave->setImage(imageToSave);
    generatedToSave->generate(ui->comboBox_DPI->currentIndex(),ui->comboBox->currentIndex(),ui->checkBox->checkState(),ui->comboBox_rozdzielczosc->currentIndex(),ui->checkBox_keepAspectRatio->checkState());
    imageToSave = generatedToSave->getImage();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Zapisz stereogram jako obrazek..."), QDir::homePath(), tr("JPG (*.jpg);;PNG(*.png);;BMP (*.bmp)"));

    if(fileName.endsWith(".png"))
        imageToSave->save(fileName, "PNG");
    else if(fileName.endsWith(".bmp"))
        imageToSave->save(fileName, "BMP");
    else
        imageToSave->save(fileName, "JPG");

    delete generatedToSave;

    if(fileName.isEmpty())
        onStatusBarChanged(QString("Anulowano"));
    else
        onStatusBarChanged(QString("Zapisano. | ")+fileName);
}

void MainWindow::update(bool dopasuj)
{
    if(dopasuj)
    {
        scaledImage = imageCpy->scaled(ui->label->width(),ui->label->height(),Qt::KeepAspectRatio);
        ui->label->setPixmap(QPixmap::fromImage(scaledImage,Qt::AutoColor));
        scaledImage = imageCpy->scaled(ui->label_game->width(),ui->label_game->height(),Qt::KeepAspectRatio);
        ui->label_game->setPixmap(QPixmap::fromImage(scaledImage,Qt::AutoColor));
    }
    else
    {
        scaledImage = imageCpy->scaled(imageCpy->size(),Qt::KeepAspectRatio);
        ui->label->setPixmap(QPixmap::fromImage(scaledImage,Qt::AutoColor));
    }
}

bool MainWindow::wczytajPlik(QString fileName)
{
    QFile file(fileName);

    imageOrg = new QImage(fileName);
    imageCpy = imageOrg;

    setLabel_info(imageCpy->width(),imageCpy->height(),file.size(),imageCpy->allGray());

    _stereogramGenerator.setImage(imageCpy);
    _stereogramGenerator.generate(ui->checkBox_wkleslosc->checkState(),ui->comboBox->currentIndex(),ui->checkBox->checkState());

    imageCpy = _stereogramGenerator.getImage();

    scaledImage = imageCpy->scaled(ui->label->width(),ui->label->height(),Qt::KeepAspectRatio);
    ui->label->setPixmap(QPixmap::fromImage(scaledImage,Qt::AutoColor));

    return true;
}

void MainWindow::setLabel_info(int w, int h, float size, bool allgray)
{
    QString t;
    t.append("Wczytano obraz:\n");
    t.append(QString("  rozdzielczosc: ").toUtf8());
    t.append(QString("%1").arg(w));
    t.append("x");
    t.append(QString("%1").arg(h));
    t.append("\n");
    t.append("  rozmiar pliku: ");
    QString tmp; tmp.sprintf("%.2f",(size/1024)/1024);
    t.append(tmp);
    t.append(" MB\n");
    t.append(QString("  obrazek w skali szarosci: ").toUtf8());

    switch (allgray)
    {
        case 1:
            t.append(QString("tak"));
            break;
        default:
            t.append(QString("nie"));
    }

    ui->label_info->clear();
    ui->label_info->setText(t.toUtf8());
}

void MainWindow::onStatusBarChanged(QString tmp)
{
    ui->statusBar->showMessage(tmp);
}

// --------------------

// -------EVENT'Y-------
void MainWindow::resizeEvent(QResizeEvent *)
{
ui->stackedWidget->setSizeIncrement(ui->centralWidget->width(),ui->centralWidget->height());
    if(wczytano || (ui->stackedWidget->currentIndex()==3))
    {

        update(ui->checkBox_dopasujDoOkna->isChecked());
    }
}

// --------------------

void MainWindow::on_checkBox_clicked()
{
    _stereogramGenerator.generate(ui->checkBox_wkleslosc->checkState(),ui->comboBox->currentIndex(),ui->checkBox->checkState());
    imageCpy = _stereogramGenerator.getImage();
    update(ui->checkBox_dopasujDoOkna->isChecked());
}

void MainWindow::on_pushButton_Open_clicked()
{
    openFile();
}

void MainWindow::on_pushButton_Koniec_clicked()
{
    exit(1);
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    onStatusBarChanged("");
}

/** @brief Wybor zakladki 'Gra' z menu glownego programu.
 */

void MainWindow::on_pushButton_Games_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    QStringList listaGier;
    QStringListModel *model;


    QFile file("gry/lista.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {}

    QTextStream in(&file);

    while (!in.atEnd()) {
        listaGier.append(in.readLine());

         }
     model = new QStringListModel(listaGier);
     ui->listView->setModel(model);
     file.close();
     onStatusBarChanged("Wybierz gre w ktora chcesz zagrac");
}

void MainWindow::on_pushButton_Save_clicked()
{
    saveFile();
}



void MainWindow::on_pushButton_wg_Graj_clicked()
{
    QModelIndex wybor = ui->listView->currentIndex();
    if(wybor.isValid())
        loadGame(wybor.data().toString());
}



void MainWindow::on_pushButton_wg_Menu_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    onStatusBarChanged(QString("Witaj w programie Stereogramiksy."));
}


void MainWindow::loadGame(QString gra)
{
    adresObrazkaGra.clear();
    hasloObrazkaGra.clear();
    ui->stackedWidget->setCurrentIndex(3);
    QFile file("gry/opisy/"+gra+".txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {}
    QTextStream in(&file);
    tytulGra = in.readLine();
    ileObrazkowGra = in.readLine().toInt();

    for(int i=0;i<ileObrazkowGra;i++){
        adresObrazkaGra.append(in.readLine());
    }
    for(int i=0;i<ileObrazkowGra;i++){
        hasloObrazkaGra.append(in.readLine());
    }
    punktyGra=ktoryObrazekGra=0;
    ui->label_game_status->setText("");
    ui->lineEdit_game_haslo->setText("");
    statsGame(0);
    showGame();
    onStatusBarChanged("Wybrales " + tytulGra + ". Co widzisz na pierwszym obrazku?");
    ui->stackedWidget->setCurrentIndex(3);
}



void MainWindow::statsGame(int wygrana)
{
    if(wygrana==1) {
        punktyGra+=10;
        ui->label_game_status->setText("Dobra odpowiedz");
        } else {
        if(ktoryObrazekGra!=0) ui->label_game_status->setText("Zla odpowiedz");
        }
    ui->lineEdit_game_nazwa->setText(tytulGra);
    ui->lineEdit_game_punkty->setText(QString::number(punktyGra)+"/"+QString::number(ileObrazkowGra*10));
}



void MainWindow::showGame()
{
//    StereogramGenerator _stereogramGenerator;

    imageOrg = new QImage(adresObrazkaGra.at(ktoryObrazekGra));
    imageCpy = imageOrg;
    //imageOrg->load(fileName);

    //qDebug() << " 1 " << imageCpy->width() << " " << imageCpy->height();
    qDebug() << adresObrazkaGra.at(ktoryObrazekGra);

    _stereogramGenerator.setImage(imageCpy);

    _stereogramGenerator.generate(1);

    imageCpy = _stereogramGenerator.getImage();

    scaledImage = imageCpy->scaled(ui->label_game->width(),ui->label_game->height(),Qt::KeepAspectRatio);
    ui->label_game->setPixmap(QPixmap::fromImage(scaledImage,Qt::AutoColor));
}



void MainWindow::on_pushButton_game_Ok_clicked()
{
    int odpowiedz=0;
    if(ui->lineEdit_game_haslo->text()==hasloObrazkaGra.at(ktoryObrazekGra)) odpowiedz=1;

    statsGame(odpowiedz);
    ktoryObrazekGra++;

    if(ktoryObrazekGra<ileObrazkowGra){
        showGame();
        if(odpowiedz==1) onStatusBarChanged("Twoje odpowiedz byla prawidlowa. Co widzisz na kolejnym obrazku?");
        if(odpowiedz==0) onStatusBarChanged("Twoje odpowiedz byla zla. Co widzisz na kolejnym obrazku?");
        } else
            {
            if(odpowiedz==1) onStatusBarChanged("Twoje odpowiedz byla prawidlowa. Gra skonczona");
            if(odpowiedz==0) onStatusBarChanged("Twoje odpowiedz byla zla. Gra skonczona");
            endGame();
            }

}



void MainWindow::endGame(){
    ui->stackedWidget->setCurrentIndex(4);
    ui->lcdNumber_end_punkty->display(punktyGra);


}

void MainWindow::on_pushButton_2_end_menu_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    onStatusBarChanged("");
}


void MainWindow::on_pushButton_Scenariusz_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
    ui->pushButton_scen_dalej->setEnabled(false);
    fileListScen.clear();
    haslaListScen.clear();
    QStringListModel *model;
    model = new QStringListModel(fileListScen);
    ui->listView_scen_file->setModel(model);
    ui->listView_scen2_hasla->setModel(model);
    ui->lineEdit_scen2_haslo->setText("");
    ui->lineEdit_scen2_tytul->setText("");
    onStatusBarChanged("Dodaj obrazki do swojej gry.");
}



void MainWindow::on_pushButton_scen_menu_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    onStatusBarChanged("");
}



void MainWindow::on_pushButton_scen_add_clicked()
{
     QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Graphic files (*.jpg *.png *.bmp);;All files (*.*)"));

     QStringListModel *model;
     fileListScen.append(fileName);
     model = new QStringListModel(fileListScen);
     ui->listView_scen_file->setModel(model);
     if(!fileName.isEmpty()) { ui->pushButton_scen_dalej->setEnabled(true); onStatusBarChanged("Obrazek dodany! Chcesz dodac kolejny?");}
}


void MainWindow::on_pushButton_scen_dalej_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    int ilefile = fileListScen.count();
    int ilehaslo = haslaListScen.count();
    ui->lineEdit_scen2_ile->setText(QString::number(ilefile-ilehaslo));
    ui->pushButton_scen2_ok->setEnabled(false);
    ui->lineEdit_scen2_haslo->setText("");
    onStatusBarChanged("A teraz ustaw tytul i dodaj hasla do swojej gry.");
}


void MainWindow::on_pushButton_scen2_menu_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    onStatusBarChanged("");
}


void MainWindow::on_pushButton_scen2_add_clicked()
{
    QStringListModel *model;
    QString haslo = ui->lineEdit_scen2_haslo->text();
    if(haslo!=""){
    haslaListScen.append(haslo);
    model = new QStringListModel(haslaListScen);
    ui->listView_scen2_hasla->setModel(model);
    int ilefile = fileListScen.count();
    int ilehaslo = haslaListScen.count();
    ui->lineEdit_scen2_ile->setText(QString::number(ilefile-ilehaslo));
    if(ilefile-ilehaslo==0){
        ui->pushButton_scen2_add->setEnabled(false);
        ui->pushButton_scen2_ok->setEnabled(true);
        }
    ui->lineEdit_scen2_haslo->setText("");
    onStatusBarChanged("Haslo dodane.");
    }

}


void MainWindow::on_pushButton_scen2_ok_clicked()
{
    QFile file("gry/lista.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Blad otwarcia pliku lista.txt";
        }

    QTextStream out(&file);
    QString tytul = ui->lineEdit_scen2_tytul->text();
    out <<  tytul << endl;
    file.close();

    QFile file2( "gry/opisy/"+ui->lineEdit_scen2_tytul->text()+".txt" );
    if(!file2.open(QIODevice::WriteOnly | QIODevice::Text)){ qDebug() << "Blad otwarcia pliku nowego"; }
    QTextStream out2(&file2);
    tytul=ui->lineEdit_scen2_tytul->text();
    out2 << tytul << "\n";
    int ilefile = fileListScen.count();
    out2 << ilefile << "\n";
    for(int i=0;i<ilefile;i++){
        out2 << fileListScen.at(i) << "\n";
        }
    for(int i=0;i<ilefile;i++){
        qDebug() << haslaListScen.at(i);
        out2 << haslaListScen.at(i) << "\n";
        }

    file2.close();
    ui->stackedWidget->setCurrentIndex(0);
    onStatusBarChanged("Scenariusz stworzony. Mozesz grac!");
}

void MainWindow::on_comboBox_activated(int index)
{
    _stereogramGenerator.generate(1,index,ui->checkBox->checkState());
    imageCpy = _stereogramGenerator.getImage();
    update(ui->checkBox_dopasujDoOkna->isChecked());
}


void MainWindow::on_pushButton_Oprogramie_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
    onStatusBarChanged("O pogramie...");
}


void MainWindow::on_pushButton_oprogramie_menu_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    onStatusBarChanged("");
}

void MainWindow::on_comboBox_DPI_activated(int index)
{
    _stereogramGenerator.changeDefault(index,ui->comboBox_RozstawOczu->currentIndex());
    _stereogramGenerator.generate(ui->checkBox_wkleslosc->checkState(),ui->comboBox->currentIndex(),ui->checkBox->checkState());
    imageCpy = _stereogramGenerator.getImage();
    update(ui->checkBox_dopasujDoOkna->isChecked());
}

void MainWindow::on_comboBox_RozstawOczu_activated(int index)
{
    _stereogramGenerator.changeDefault(ui->comboBox_DPI->currentIndex(),index);
    _stereogramGenerator.generate(ui->checkBox_wkleslosc->checkState(),ui->comboBox->currentIndex(),ui->checkBox->checkState());
    imageCpy = _stereogramGenerator.getImage();
    update(ui->checkBox_dopasujDoOkna->isChecked());
}

void MainWindow::on_checkBox_wkleslosc_clicked()
{
    _stereogramGenerator.generate(ui->checkBox_wkleslosc->checkState(),ui->comboBox->currentIndex(),ui->checkBox->checkState());
    imageCpy = _stereogramGenerator.getImage();
    update(ui->checkBox_dopasujDoOkna->isChecked());
}


void MainWindow::on_checkBox_dopasujDoOkna_clicked()
{
    update(ui->checkBox_dopasujDoOkna->isChecked());
}
