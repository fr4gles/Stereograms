#include "stereogramgenerator.h"

StereogramGenerator::StereogramGenerator(QObject *parent) :
    QObject(parent)
{
    // uzywac
    qDebug() << "Konstruktor klasy: SteogramGenerator()";
    setDefault(96,2.7);
}


StereogramGenerator::~StereogramGenerator()
{
    if(_wygenerowano)
    {
        delete _imageGeneratedStereogram;
    }
}

void StereogramGenerator::setDefault(int DPI, double distanceBetweenEyes)
{
    _DPI                    = DPI;
    _distanceBetweenEyes    = distanceBetweenEyes; // odleglosc obiektow
    _eyeSeparation          = roundSomething(_distanceBetweenEyes*_DPI);
    _depthOfField           = (1.0/3.0);
    _farOfDots              = separateSomething(0);
    _wygenerowano           = false;
}

void StereogramGenerator::changeDefault(int currentDPI, int currentDistanceBetweenEyes)
{
    switch(currentDPI)
    {
        case 0:     _DPI = 76;      break;
        case 1:     _DPI = 96;      break;
        case 2:     _DPI = 150;     break;
        default:    _DPI = 96;      break;
    }

    switch(currentDistanceBetweenEyes)
    {
        case 0:     _distanceBetweenEyes = 2.5;     break;
        case 1:     _distanceBetweenEyes = 2.6;     break;
        case 2:     _distanceBetweenEyes = 2.7;     break;
        default:    _distanceBetweenEyes = 2.755;   break;
    }

    _eyeSeparation          = roundSomething(_distanceBetweenEyes*_DPI);
    _depthOfField           = (1.0/3.0);
    _farOfDots              = separateSomething(0);
}

int StereogramGenerator::roundSomething(double something)
{
    return ((int)((something)+0.5));
}

int StereogramGenerator::separateSomething(double something)
{
    return roundSomething((1-_depthOfField*something)*_eyeSeparation/(2-_depthOfField*something));
}

void StereogramGenerator::generate(int convex, int color, bool circles, int size, int keepAspectRatio)
{
    /// czyszczenie pami�ci je�li obrazek jaki� obrazek zosta� ju� wczytany
    if(_wygenerowano)
    {
        delete _imageGeneratedStereogram;
    }

    /// tworzenie kopii oryginalnego obrazka wczytanego do generatora
    _imageCopy = _imageToGenerate;

    /// ustawienie odpowiedzialne za to czy generowany obrazek ma zachowa� swoje proporcje b�d� nie
    Qt::AspectRatioMode aspectModel;
    if(keepAspectRatio!=0)
        aspectModel = Qt::KeepAspectRatio;
    else
        aspectModel = Qt::IgnoreAspectRatio;

    /// swotch odpowiadaj�cy rozmiar w jakim dany obrazek b�dzie generowany
    /// standardowo obrazek generowany ma rozmiar oko�o 1280x720
    /// reszta opcji u�ywna dopiero przy wywo�aniu opcji zapisu stereogramu
    switch(size)
    {
        case 0:     *_imageCopy = _imageToGenerate->scaled(QSize(800,600),aspectModel);         break;
        case 1:     *_imageCopy = _imageToGenerate->scaled(QSize(1024,768),aspectModel);        break;
        case 2:     *_imageCopy = _imageToGenerate->scaled(1280,720,aspectModel);               break;
        case 3:     *_imageCopy = _imageToGenerate->scaled(1920,1080,aspectModel);              break;
        case 4:
            if(_imageToGenerate->height() >= 720)
                *_imageCopy = _imageToGenerate->scaledToHeight(720);
            else if(_imageToGenerate->width()>= 1280)
                *_imageCopy = _imageToGenerate->scaledToWidth(1280);
        break;
        default:    _imageCopy = _imageToGenerate;      break;
    }

    /// przypisanie zmiennym u�ywanym w algorytmie generowania stereogramu, szeroko�ci i wysoko�ci obrazka
    _widthOfImage_X = _imageCopy->width();
    _heightOfImage_Y = _imageCopy->height();

    /// zarezerwowanie pami�ci dla nowego ( na razie pustego obrazka ), kt�ry w przysz�o�ci b�dzie stereogramem
    _imageGeneratedStereogram = new QImage(_widthOfImage_X,_heightOfImage_Y,QImage::Format_RGB32);

    /// zarezwerwowanie pami�ci dla tablicy dwuwymiarowej[szeroko��][wyko��]
    /// - w kt�rej b�d� przechowywane "g��boko�ci" kolejnych pixeli
    double **imageDepth = 0;
    imageDepth = new double*[_widthOfImage_X];
    for(int i=0;i<_widthOfImage_X;++i)
        imageDepth[i] = new double[_heightOfImage_Y];

    /// wywo�anie funkcji obliczaj�cej "g��boko��" pixeli na skali szaro�ci ( czarny 1, bia�y 0 ), im ciemniejszy tym bli�ej nas
    calculateImageDepth(imageDepth,convex);

    /// wys�anie do status bara, w g�ownym oknie aplikacji, wiadomo�ci
    emit setStatusBarLabel("Generowanie");

    /// pocz�tek algorytmu generuj�cego stereogramy
    /// algorytm zaczerpni�ty z "Displaying 3D Images: Algorithms for Single Image Random Dot Stereograms"
    /// autorstwa: Harold W. Thimbleby, Stuart Inglis, Ian H. Witten
    /// poni�ej implementacja algorytmu
    ///
    /// przechodzimy po wszystkich pixelach, po pixelach poziomych
    /// tak aby zastosowa� algorytm do wszystkich linii oddzielnie
    /// x,y wsp�rz�dne danego punktu ( pixela )
    for(int y=0;y<_heightOfImage_Y;++y)
    {
        /// tablica kolor�w danych pixeli
        unsigned int colorOfPixel[_widthOfImage_X];

        /// tablica pixeli, kt�re wskazuj� na te pixele
        /// kt�re maj� mie� wymuszony dany kolor ( czarny lub bia�y )
        /// [ wszystko w zale�no�ci od tego, czy posiadamy informacj� na temat "g��boko�ci" danego pixela ]
        int samePixels[_heightOfImage_Y];

        /// punkt - przechowuje informacje na temat rozdzielenia
        /// dw�ch identycznych pixeli odpowiadaj�cych rodzieleniu "stereo" tych punkt�w
        /// @note rozdzielenie "stereo" efekt wizualny - wykorzystywany do generowania stereogram�w
        int stereoSeparationOfPoint;

        /// "lewy" i "prawy" pixel
        /// musz� by� tego samego koloru
        /// - odpowiadaj� pixelowi widocznemu przez lewe i analogicznie prawe oko
        int leftEye, rightEye;

        /// inicjalizacja tablicy pixeli samePixels
        /// pocz�tkowo przypisujemy pixelom ich w�asne warto�ci
        for(int x=0;x<_widthOfImage_X;++x)
            samePixels[x]=x;

        ///
        for(int x=0;x<_widthOfImage_X;++x)
        {
            /// przypisanie - rozdzielenie dw�ch punkt�w
            stereoSeparationOfPoint = separateSomething(imageDepth[x][y]);

            /// przypisanie warto�ci do zmiennych przechowuj�cych
            /// informacje o pixelu lewym i prawym
            /// - pixel lewy jak i pixel prawy musz� by� takie same ( albo przynajmniej powinny takie by� ... )
            leftEye = x - stereoSeparationOfPoint/2;
            rightEye = leftEye + stereoSeparationOfPoint;

            /// dalsza cz�� algorytmu
            /// - usuwanie niewidocznych powierzchni
            if(0 <= leftEye && rightEye < _widthOfImage_X)
            {
                /// zmienna odpowiedzialana za usuwanie niewidocznych ( ukrytych ) powie�chni
                int isVisible;

                /// zmienna tymczasowa, porz�dkuj�ca kod i alorytm
                /// [ b�dziemy sprawdza� pixele x+tmp i x-tmp - czy nie s� zakryte, b�d� takie same ]
                int tmp=1;

                /// zmienna przechowuj�ca warto�� odleg�o�ci od "�ciany" stereogramu
                /// zale�nie od warto�ci imageDepth - czyli g��bi na skali szaro�ci
                float rayOfImageDepthAtPoint;

                /// p�tla odpowiadaj�ca za przej�cie i usuni�cie wszystkich ukrytych powierzni
                do
                {
                    /// implementacja wzoru wyliczaj�cego odleg�o�ci ( promie� )
                    rayOfImageDepthAtPoint = imageDepth[x][y] + 2*(2 - _depthOfField*imageDepth[x][y])*tmp/(_depthOfField*_eyeSeparation);

                    /// je�li dany pixel jest zas�oni�ty to isVisible = 0
                    isVisible = imageDepth[x-tmp][y] < rayOfImageDepthAtPoint && imageDepth[x+tmp][y] < rayOfImageDepthAtPoint;

                    /// postinkrementacja tmp
                    /// - b�dziemy sprawdza� dla kolejnych s�siednich pixeliczy si� pokrywaj�
                    tmp++;
                }
                while (isVisible && rayOfImageDepthAtPoint < 1);

                /// sprawdzamy czy pixele s� takie same - je�li s� widoczne
                if (isVisible)
                {
                    int left_N_right = samePixels[leftEye];
                    while (left_N_right != leftEye && left_N_right != rightEye)
                    {
                        /// nast�pnie por�wnujemy pixele
                        /// do momentu a� samePixels[leftEye] = leftEye
                        /// lub samePixels[leftEye] = rightEye
                        /// je�li nie s� - zamieniamy je miejscami
                        if (left_N_right > rightEye)
                        {
                            leftEye = left_N_right;
                            left_N_right = samePixels[leftEye];
                        }
                        else
                        {
                            samePixels[leftEye] = rightEye;
                            leftEye = rightEye;
                            left_N_right = samePixels[leftEye];
                            rightEye = left_N_right;
                        }
                    }

                /// odnotowujemy fakt, �e pixele s� takie same
                samePixels[leftEye] = rightEye;
                }
            }
        }

        /// ko�cowa cz�� algorytmu
        /// ostatnia p�tla po ktorej odpowiednim pixelom
        /// zostaje nadany kolor
        for (int x=_widthOfImage_X-1 ; x>= 0 ; x--)
        {
            /// losowo nadajemy im warto�ci
            /// zamiarem algorytmu jest uzyskanie koloru czarnego lub bia�ego
            /// dok�aniej dw�ch r�nych kontrastowych kolor�w
            /// - czarny powinien zosta�
            if (samePixels[x] == x) colorOfPixel[x] = qrand()&1;
            else colorOfPixel[x] = colorOfPixel[samePixels[x]];

            /// switch odpowiedzialny za nadanie innego koloru stereogramowi ( ni� bia�y )
            /// - poprzez wybranie odpowiedniej wartosci color przez uzytkowika
            switch (color)
            {
                case 0:     _imageGeneratedStereogram->setPixel(x, y, colorOfPixel[x]*16775930); /* White */         break;
                case 1:     _imageGeneratedStereogram->setPixel(x, y, colorOfPixel[x]*14210000); /* Gray */          break;
                case 2:     _imageGeneratedStereogram->setPixel(x, y, colorOfPixel[x]*11500000); /* Purple */        break;
                case 3:     _imageGeneratedStereogram->setPixel(x, y, colorOfPixel[x]*11800000); /* Violet */        break;
                case 4:     _imageGeneratedStereogram->setPixel(x, y, colorOfPixel[x]*14245000); /* Pink */          break;
                case 5:     _imageGeneratedStereogram->setPixel(x, y, colorOfPixel[x]*14300000); /* Scarlet */       break;
                case 6:     _imageGeneratedStereogram->setPixel(x, y, colorOfPixel[x]*16000000); /* Red */           break;
                case 7:     _imageGeneratedStereogram->setPixel(x, y, colorOfPixel[x]*14250000); /* Orange */        break;
                case 8:     _imageGeneratedStereogram->setPixel(x, y, colorOfPixel[x]*9000000); /* Brown */          break;
                case 9:     _imageGeneratedStereogram->setPixel(x, y, colorOfPixel[x]*14272000); /* Yellow */        break;
                case 10:    _imageGeneratedStereogram->setPixel(x, y, colorOfPixel[x]*9300000); /* Pistachio */      break;
                case 11:    _imageGeneratedStereogram->setPixel(x, y, colorOfPixel[x]*125000); /* Green */           break;
                case 12:    _imageGeneratedStereogram->setPixel(x, y, colorOfPixel[x]*1830000); /* Sea green */      break;
                case 13:    _imageGeneratedStereogram->setPixel(x, y, colorOfPixel[x]*1900000); /* Sky blue */       break;
                case 14:    _imageGeneratedStereogram->setPixel(x, y, colorOfPixel[x]*300000); /* Indigo */          break;
                case 15:    _imageGeneratedStereogram->setPixel(x, y, colorOfPixel[x]*255); /* Blue */               break;
                case 16:    _imageGeneratedStereogram->setPixel(x, y, colorOfPixel[x]*16000000/_imageCopy->width()*x); /* Multi-color columns */ break;
                case 17:    _imageGeneratedStereogram->setPixel(x, y, colorOfPixel[x]*16000000/_imageCopy->height()*y); /* Multi-color rows */   break;
                default:    _imageGeneratedStereogram->setPixel(x, y, colorOfPixel[x]*16775930); /* White */         break;
            }
        }

        /// wys�anie za ka�dym przebiegiem p�tli post�p jaki zosta� wykonany
        /// do statusbara na g�ownym oknie
        emit setStatusBarLabel(QString("Generowanie: ")+QString("%1").arg((y*100)/(_heightOfImage_Y-1))+QString("%"));
    }

    /// po sko�czeniu algorytmu, przes��nie do g�ownego okna post�p jaki zosta� wykonany
    /// oraz rozdzielczo�c wygenerowanego stereogramu
    emit setStatusBarLabel(QString("Generowanie: 100% | Wygenerowano obrazek: ")+QString("%1").arg(_widthOfImage_X)+QString("x")+QString("%1").arg(_heightOfImage_Y));

    /// je�li u�ytkownik zaznaczy� tak� opcj� to stereogramy s� generowane z pomocniczymi kropkami
    /// dok�adniejszy opis znajduje si� w funkcji
    if(circles)
        drawCirclesOnImage(_widthOfImage_X,_heightOfImage_Y);

    /// czyszczenie pami�ci - usuwanie tymczasowej tablicy pixeli
    for(int i=0;i<_widthOfImage_X;++i)
        delete [] imageDepth[i];
    delete [] imageDepth;

    /// po 1 wygenerowaniu obrazka waro�� _wygerowano zmienia si� na true
    /// pomaga to zapobiec p�niejszym wyciekom pami�ci
    _wygenerowano = true;
}

void StereogramGenerator::calculateImageDepth(double **imageDepth, int convex)
{
    /// tworzenie tymczasowej zmiennej, kt�rej warto�� b�dzie zale�e� od tego czy
    /// generowane warto�ci maj� by� dla stereogramu wypuk�ego ( normalnie widoczny stereogram )
    /// czy wkl�s�ego ( gdzie kolor bia�y jest bli�ej nas a czarny dalej )
    float tmp;
    if(convex)
        tmp = 1.0;
    else
        tmp=-1.0;

    /// przej�cie po wszystkich pixelach na szero�ci
    /// dla ka�dego, przej�cie przez wszystkie pionowe pixele
    /// nast�pnie wype�nienie tablicy dwuwymiarowej imageDepth[x][y] - warto�ciami danego pixela ( w skali szaro�ci )
    for(int x=0;x<_imageCopy->width();++x)
    {
        for(int y=0;y<_imageCopy->height();++y)
        {
            imageDepth[x][y] = ((double)(convex - (qGray(_imageCopy->pixel(x,y))/255.0)*tmp));
        }
    }
}

void StereogramGenerator::drawCirclesOnImage(int _widthOfImage_X, int _heightOfImage_Y)
{
    /// rysowanie za pomoc� QPainter'a na obrazku _imageGeneratedStereogram
    /// pomocniczych czarnych kropek ( opcjonalnie, dla u�ytkownika )
    /// czarne kropki s� w zadanej odleg�o�ci od siebie - tak aby z dw�ch ( u�ytkownik programu zobaczy� trzy )
    QPainter p;
    p.begin(_imageGeneratedStereogram);
    p.setBrush(QBrush(Qt::black));
    p.setPen(Qt::black);
    p.drawEllipse(_widthOfImage_X/2-_farOfDots/2,_heightOfImage_Y*16/19,20,20);
    p.drawEllipse(_widthOfImage_X/2+_farOfDots/2,_heightOfImage_Y*16/19,20,20);
    p.end();
}
