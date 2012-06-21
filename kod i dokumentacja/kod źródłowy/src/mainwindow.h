#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QStringListModel>
#include <QImageReader>

#include "stereogramgenerator.h"

/**
 * Ui - namespace - do u¿ywania obiektów stworzonych za pomoc¹ Designer'a w QtCreatorze
 * @see mainwindow.ui
 * @see MainWindow
 */
namespace Ui
{
    class MainWindow;
}

/**
 * Main Window - G³ówna Klasa .
 * Klasa, kontroluj¹ca wszelkie dzia³ania aplikacji.
 * @note MainWindow musi dzia³aæ.
 * @file mainwindow.h
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:

    /**
     * MainWindow - rêczne dodawanie/ustawianie elementów.
     * Konstruktor klasy MainWindow
     */
    explicit MainWindow(QWidget *parent = 0);

    /**
     * Destruktor klasy MainWindow
     */
    ~MainWindow();

private slots:

    /** @brief funkcja odpowiadaj¹ca za wœwietlenie na labelu informaji o wczytanym obrazku
      * @return void
     */
    void setLabel_info(int w, int h, float size, bool allgray);

    /** @brief funkcja odpowiadaj¹ca za wœwietlenie na status barze aktulanych informacji/podpowiedzi
      * @return void
     */
    void onStatusBarChanged(QString tmp);

    /** @brief funkcja ob³usuj¹ca przycisk przenosz¹cy u¿ytkownika do odpowiedniej generowania stereogramu
      * @return void
     */
    void on_pushButton_Generate_clicked();

    /** @brief funkcja ob³usuj¹ca przycisk otwieraj¹cy OpenFileDialog
      * @return void
     */
    void on_pushButton_Open_clicked();

    /** @brief funkcja ob³usuj¹ca przycisk zamykaj¹cy program
      * @return void
     */
    void on_pushButton_Koniec_clicked();

    /** @brief funkcja ob³usuj¹ca przycisk powracaj¹cy do g³ownego menu z zak³adki generowanie stereogramu
      * @return void
     */
    void on_pushButton_clicked();

    /** @brief Wybor zakladki 'Gra' z menu glownego programu.
      * @return void
     */
    void on_pushButton_Games_clicked();

    /** @brief funkcja ob³usuj¹ca przycisk otwieraj¹cy SaveFileDialog - zapisuje obrazek
      * @return void
     */
    void on_pushButton_Save_clicked();

    /** @brief W zakladce wyboru gier wybieramy gre w ktora chcemy zagrac.
      * @return void
     */
    void on_pushButton_wg_Graj_clicked();

    /** @brief Powrot do menu glownego z zakladki wybory gier.
     */
    void on_pushButton_wg_Menu_clicked();

    /** @brief Akcja na klikniecie odpowiedzi do gry.
      * @return void
     */
    void on_pushButton_game_Ok_clicked();

    /** @brief funkcja ob³usuj¹ca przycisk powracaj¹cy do g³ownego menu z zak³adki z wynikami gry
      * @return void
     */
    void on_pushButton_2_end_menu_clicked();

    /** @brief funkcja ob³usuj¹ca check odpowiedzialny za wygenerowanie stereogramu z pomocniczymi kropkami
      * @return void
     */
    void on_checkBox_clicked();

    /** @brief Wybor zakladki 'Stworz scenariusz' w menu glownym.
      * @return void
     */
    void on_pushButton_Scenariusz_clicked();

    /** @brief Powrot do menu z zakladki scenariusz.
      * @return void
     */
    void on_pushButton_scen_menu_clicked();

    /** @brief Przycisk akcji dodajacy obrazek do scenariusza.
      * @return void
     */
    void on_pushButton_scen_add_clicked();

    /** @brief Dodanie wszystkich obrazkow i przejscie do kolejnego etapu.
      * @return void
     */
    void on_pushButton_scen_dalej_clicked();

    /** @brief Powrot do menu z zakladki stworzenia scenariusza.
      * @return void
     */
    void on_pushButton_scen2_menu_clicked();

    /** @brief Dodanie hasla do gry przy tworzeniu scenariusza.
      * @return void
     */
    void on_pushButton_scen2_add_clicked();

    /** @brief Zakonczenie tworzenia scenariusza i zapis do pliku.
      * @return void
     */
    void on_pushButton_scen2_ok_clicked();

    /** @brief funkcja ob³usuj¹ca combobox odpowiedzialny za zmianê kolorów na stereogramie
      * @return void
     */
    void on_comboBox_activated(int index);

    /** @brief Wybor zakladki 'o programie' z menu glownego.
      * @return void
     */
    void on_pushButton_Oprogramie_clicked();

    /** @brief Powrot do menu z zakladki o programie.
      * @return void
     */
    void on_pushButton_oprogramie_menu_clicked();

    /** @brief funkcja ob³usuj¹ca combobox odpowiedzialny za zmianê DPI generowanego stereogramu
      * @return void
     */
    void on_comboBox_DPI_activated(int index);

    /** @brief funkcja ob³usuj¹ca combobox odpowiedzialny za rozstawu oczu generowanego stereogramu
      * @return void
     */
    void on_comboBox_RozstawOczu_activated(int index);

    /** @brief funkcja ob³usuj¹ca checkbox odpowiedzialny za zmianê "wklês³oœci" stereogramu - wiêcej w dokumentacji.
      * @return void
     */
    void on_checkBox_wkleslosc_clicked();

    /** @brief funkcja ob³usuj¹ca checkbox odpowiedzialny za to czy obrazek ma siê dopasowywaæ do okna czy te¿ pozostaæ w oryginalnym kszta³cie
      * @return void
     */
    void on_checkBox_dopasujDoOkna_clicked();

private:
    /**
     * update - odœwie¿aj¹ca okienko - dopasowuje rozmiar wczytanego, a nastêpnie wygenerowanego pliku ( stereogramu )
     * @param dopasuj - czy obrazek ma byæ dopasowany do okna ( domyœlan wartoœæ - tak )
     * @see openFile();
     * @see saveFile();
     * @see resizeEvent();
     * @see wczytajPlik();
     * @return void
     */
    void update(bool dopasuj=true);

    /**
     * openFile - okienko dialogowe do wyboru pliku.
     * @see wczytajPlik();
     * @see update();
     * @see saveFile();
     * @see resizeEvent();
     * @return void
     */
    void openFile();

    /**
     * saveFile - zapisuje plik ( stereogram ) - w okreœlonej rozdzielczoœci, wybranym rozszerzeniem oraz innymi ustawieniami
     * @see update();
     * @see saveFile();
     * @see resizeEvent();
     * @see wczytajPlik();
     * @return void
     */
    void saveFile();

    /**
     * wczytajPlik - metoda wczytuj¹ca obrazek
     * @param fileName - string nazwy pliku.
     * @see update();
     * @see saveFile();
     * @see resizeEvent();
     * @see wczytajPlik();
     * @return bool
     */
    bool wczytajPlik(QString fileName);

    /**
     * resizeEvent - event odpowiadaj¹cy za updatowanie rozmiaru obrazka
     * @param event - rozszerzenie obrazka
     * @see update();
     * @see saveFile();
     * @see resizeEvent();
     * @see wczytajPlik();
     * @return void
     */
    void resizeEvent(QResizeEvent *);


    /**
     * setElementsDisabled - pozwala na ³atwe "w³¹czanie" lub "wy³¹czanie" elementów
     * @param disabled - czy dane elementy maj¹ byæ w³¹czone b¹dŸ nie
     * @see update();
     * @see saveFile();
     * @see resizeEvent();
     * @see wczytajPlik();
     * @return void
     */
    void setElementsDisabled(bool disabled);

    /**
      * Zmienna *ui - pozwala siê odwo³ywaæ do obiektów stworzonych przez Designer'a
      * Zmienna prywatna.
      */
    Ui::MainWindow *ui;

    /**
      * Zmienna *fileName - wczytuje œcie¿kê dostêpu do pliku oraz jego nazwê.
      * Zmienna prywatna.
      */
    QString *fileName;

    /**
      * Zmienna *imageOrg - wczytany oryginalny obrazek
      * Zmienna prywatna.
      */
    QImage *imageOrg;

    /**
      * Zmienna *imageCpy - na pocz¹tku przechowuje kopiê imageOrg a potem sam stereogram.
      * Zmienna prywatna.
      */
    QImage *imageCpy;

    /**
      * Zmienna *imageToSave - przechowuje obrazek gotowy do zapisania.
      * Zmienna prywatna.
      */
    QImage *imageToSave;

    /**
      * Zmienna scaledImage - przechowuje kopiê imageCpy - odpowiednio dopasowan¹ do rozmiaru okna
      * Zmienna prywatna.
      */
    QImage scaledImage;

    /**
      * Zmienna wczytano - na wejœciu false, po wygenerowaniu true | pomaga w optymalizacji zu¿ycia pamiêci
      * Zmienna prywatna.
      */
    bool wczytano;


    /**
      * Zmienna _stereogramGenerator - obiekt klasy StereogramGenerator - odpowiedzialny za obs³ugê generowania stereogramu
      * Zmienna prywatna.
      */
    StereogramGenerator _stereogramGenerator;



    /* do gry */
    /**
      * Zmienna tytulGra - przechowuje nazwe gry
      * Zmienna prywatna.
      */
    QString tytulGra;
    /**
      * Zmienna ileObrazkowGra - przechowuje liczbe obrazkow w grze
      * Zmienna prywatna.
      */
    int ileObrazkowGra;
    /**
      * Zmienna adresObrazkaGra - przechowuje adres do obrazka w grze
      * Zmienna prywatna.
      */
    QStringList adresObrazkaGra;
    /**
      * Zmienna hasloObrazkaGra - przechowuje odpowiedz do obrazka w grze
      * Zmienna prywatna.
      */
    QStringList hasloObrazkaGra;
    /**
      * Zmienna punktyGra - przechowuje ilosc zgromadzonych punktów w grze
      * Zmienna prywatna.
      */
    int  punktyGra;
    /**
      * Zmienna ktoryObrazekGra - przechowuje aktualny obrazek do wyswietlenia w grze
      * Zmienna prywatna.
      */
    int ktoryObrazekGra;
    /* koniec do gry */
    /* generator scenariuszy */
    /**
      * Zmienna fileListScene - przechowuje adresy obrazkow w generatorze scenariuszy
      * Zmienna prywatna.
      */
    QStringList fileListScen;
    /**
      * Zmienna haslaListScene - przechowuje odpowiedzi do obrazkow w generatorze scenariuszy
      * Zmienna prywatna.
      */
    QStringList haslaListScen;
    /*koniec generator scenariuszy */


    /* funkcje do gry */
    /** @brief Wczytanie parametrow gry.
     *
     *  @param gra Nazwa gry w ktora chcemy zagrac.
     * @see statsGame();
     * @see showGame();
     * @see endGame();
     * @return void
     */
    void loadGame(QString);
    /** @brief Uaktulnienie statystyk  oraz punktow gry.
     *
     *  @param wygrana Sprawdza czy odpowiedz jest prawidlowa.
     * @return void
     */
    void statsGame(int);
    /** @brief Wyswietlenie obrazka w grze.
      * @see endGame();
      * @return void
     */
    void showGame();
    /** @brief Wyswietla zakladke podsumowania gry.
      * @return void
     */
    void endGame();

};

#endif // MAINWINDOW_H
