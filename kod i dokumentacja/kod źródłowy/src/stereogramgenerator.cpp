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
    /// czyszczenie pamiêci jeœli obrazek jakiœ obrazek zosta³ ju¿ wczytany
    if(_wygenerowano)
    {
        delete _imageGeneratedStereogram;
    }

    /// tworzenie kopii oryginalnego obrazka wczytanego do generatora
    _imageCopy = _imageToGenerate;

    /// ustawienie odpowiedzialne za to czy generowany obrazek ma zachowaæ swoje proporcje b¹dŸ nie
    Qt::AspectRatioMode aspectModel;
    if(keepAspectRatio!=0)
        aspectModel = Qt::KeepAspectRatio;
    else
        aspectModel = Qt::IgnoreAspectRatio;

    /// swotch odpowiadaj¹cy rozmiar w jakim dany obrazek bêdzie generowany
    /// standardowo obrazek generowany ma rozmiar oko³o 1280x720
    /// reszta opcji u¿ywna dopiero przy wywo³aniu opcji zapisu stereogramu
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

    /// przypisanie zmiennym u¿ywanym w algorytmie generowania stereogramu, szerokoœci i wysokoœci obrazka
    _widthOfImage_X = _imageCopy->width();
    _heightOfImage_Y = _imageCopy->height();

    /// zarezerwowanie pamiêci dla nowego ( na razie pustego obrazka ), który w przysz³oœci bêdzie stereogramem
    _imageGeneratedStereogram = new QImage(_widthOfImage_X,_heightOfImage_Y,QImage::Format_RGB32);

    /// zarezwerwowanie pamiêci dla tablicy dwuwymiarowej[szerokoœæ][wykoœæ]
    /// - w której bêd¹ przechowywane "g³êbokoœci" kolejnych pixeli
    double **imageDepth = 0;
    imageDepth = new double*[_widthOfImage_X];
    for(int i=0;i<_widthOfImage_X;++i)
        imageDepth[i] = new double[_heightOfImage_Y];

    /// wywo³anie funkcji obliczaj¹cej "g³êbokoœæ" pixeli na skali szaroœci ( czarny 1, bia³y 0 ), im ciemniejszy tym bli¿ej nas
    calculateImageDepth(imageDepth,convex);

    /// wys³anie do status bara, w g³ownym oknie aplikacji, wiadomoœci
    emit setStatusBarLabel("Generowanie");

    /// pocz¹tek algorytmu generuj¹cego stereogramy
    /// algorytm zaczerpniêty z "Displaying 3D Images: Algorithms for Single Image Random Dot Stereograms"
    /// autorstwa: Harold W. Thimbleby, Stuart Inglis, Ian H. Witten
    /// poni¿ej implementacja algorytmu
    ///
    /// przechodzimy po wszystkich pixelach, po pixelach poziomych
    /// tak aby zastosowaæ algorytm do wszystkich linii oddzielnie
    /// x,y wspó³rzêdne danego punktu ( pixela )
    for(int y=0;y<_heightOfImage_Y;++y)
    {
        /// tablica kolorów danych pixeli
        unsigned int colorOfPixel[_widthOfImage_X];

        /// tablica pixeli, które wskazuj¹ na te pixele
        /// które maj¹ mieæ wymuszony dany kolor ( czarny lub bia³y )
        /// [ wszystko w zale¿noœci od tego, czy posiadamy informacjê na temat "g³êbokoœci" danego pixela ]
        int samePixels[_heightOfImage_Y];

        /// punkt - przechowuje informacje na temat rozdzielenia
        /// dwóch identycznych pixeli odpowiadaj¹cych rodzieleniu "stereo" tych punktów
        /// @note rozdzielenie "stereo" efekt wizualny - wykorzystywany do generowania stereogramów
        int stereoSeparationOfPoint;

        /// "lewy" i "prawy" pixel
        /// musz¹ byæ tego samego koloru
        /// - odpowiadaj¹ pixelowi widocznemu przez lewe i analogicznie prawe oko
        int leftEye, rightEye;

        /// inicjalizacja tablicy pixeli samePixels
        /// pocz¹tkowo przypisujemy pixelom ich w³asne wartoœci
        for(int x=0;x<_widthOfImage_X;++x)
            samePixels[x]=x;

        ///
        for(int x=0;x<_widthOfImage_X;++x)
        {
            /// przypisanie - rozdzielenie dwóch punktów
            stereoSeparationOfPoint = separateSomething(imageDepth[x][y]);

            /// przypisanie wartoœci do zmiennych przechowuj¹cych
            /// informacje o pixelu lewym i prawym
            /// - pixel lewy jak i pixel prawy musz¹ byæ takie same ( albo przynajmniej powinny takie byæ ... )
            leftEye = x - stereoSeparationOfPoint/2;
            rightEye = leftEye + stereoSeparationOfPoint;

            /// dalsza czêœæ algorytmu
            /// - usuwanie niewidocznych powierzchni
            if(0 <= leftEye && rightEye < _widthOfImage_X)
            {
                /// zmienna odpowiedzialana za usuwanie niewidocznych ( ukrytych ) powie¿chni
                int isVisible;

                /// zmienna tymczasowa, porz¹dkuj¹ca kod i alorytm
                /// [ bêdziemy sprawdzaæ pixele x+tmp i x-tmp - czy nie s¹ zakryte, b¹dŸ takie same ]
                int tmp=1;

                /// zmienna przechowuj¹ca wartoœæ odleg³oœci od "œciany" stereogramu
                /// zale¿nie od wartoœci imageDepth - czyli g³êbi na skali szaroœci
                float rayOfImageDepthAtPoint;

                /// pêtla odpowiadaj¹ca za przejœcie i usuniêcie wszystkich ukrytych powierzni
                do
                {
                    /// implementacja wzoru wyliczaj¹cego odleg³oœci ( promieñ )
                    rayOfImageDepthAtPoint = imageDepth[x][y] + 2*(2 - _depthOfField*imageDepth[x][y])*tmp/(_depthOfField*_eyeSeparation);

                    /// jeœli dany pixel jest zas³oniêty to isVisible = 0
                    isVisible = imageDepth[x-tmp][y] < rayOfImageDepthAtPoint && imageDepth[x+tmp][y] < rayOfImageDepthAtPoint;

                    /// postinkrementacja tmp
                    /// - bêdziemy sprawdzaæ dla kolejnych s¹siednich pixeliczy siê pokrywaj¹
                    tmp++;
                }
                while (isVisible && rayOfImageDepthAtPoint < 1);

                /// sprawdzamy czy pixele s¹ takie same - jeœli s¹ widoczne
                if (isVisible)
                {
                    int left_N_right = samePixels[leftEye];
                    while (left_N_right != leftEye && left_N_right != rightEye)
                    {
                        /// nastêpnie porównujemy pixele
                        /// do momentu a¿ samePixels[leftEye] = leftEye
                        /// lub samePixels[leftEye] = rightEye
                        /// jeœli nie s¹ - zamieniamy je miejscami
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

                /// odnotowujemy fakt, ¿e pixele s¹ takie same
                samePixels[leftEye] = rightEye;
                }
            }
        }

        /// koñcowa czêœæ algorytmu
        /// ostatnia pêtla po ktorej odpowiednim pixelom
        /// zostaje nadany kolor
        for (int x=_widthOfImage_X-1 ; x>= 0 ; x--)
        {
            /// losowo nadajemy im wartoœci
            /// zamiarem algorytmu jest uzyskanie koloru czarnego lub bia³ego
            /// dok³aniej dwóch ró¿nych kontrastowych kolorów
            /// - czarny powinien zostaæ
            if (samePixels[x] == x) colorOfPixel[x] = qrand()&1;
            else colorOfPixel[x] = colorOfPixel[samePixels[x]];

            /// switch odpowiedzialny za nadanie innego koloru stereogramowi ( ni¿ bia³y )
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

        /// wys³anie za ka¿dym przebiegiem pêtli postêp jaki zosta³ wykonany
        /// do statusbara na g³ownym oknie
        emit setStatusBarLabel(QString("Generowanie: ")+QString("%1").arg((y*100)/(_heightOfImage_Y-1))+QString("%"));
    }

    /// po skoñczeniu algorytmu, przes³¹nie do g³ownego okna postêp jaki zosta³ wykonany
    /// oraz rozdzielczoœc wygenerowanego stereogramu
    emit setStatusBarLabel(QString("Generowanie: 100% | Wygenerowano obrazek: ")+QString("%1").arg(_widthOfImage_X)+QString("x")+QString("%1").arg(_heightOfImage_Y));

    /// jeœli u¿ytkownik zaznaczy³ tak¹ opcjê to stereogramy s¹ generowane z pomocniczymi kropkami
    /// dok³adniejszy opis znajduje siê w funkcji
    if(circles)
        drawCirclesOnImage(_widthOfImage_X,_heightOfImage_Y);

    /// czyszczenie pamiêci - usuwanie tymczasowej tablicy pixeli
    for(int i=0;i<_widthOfImage_X;++i)
        delete [] imageDepth[i];
    delete [] imageDepth;

    /// po 1 wygenerowaniu obrazka waroœæ _wygerowano zmienia siê na true
    /// pomaga to zapobiec póŸniejszym wyciekom pamiêci
    _wygenerowano = true;
}

void StereogramGenerator::calculateImageDepth(double **imageDepth, int convex)
{
    /// tworzenie tymczasowej zmiennej, której wartoœæ bêdzie zale¿eæ od tego czy
    /// generowane wartoœci maj¹ byæ dla stereogramu wypuk³ego ( normalnie widoczny stereogram )
    /// czy wklês³ego ( gdzie kolor bia³y jest bli¿ej nas a czarny dalej )
    float tmp;
    if(convex)
        tmp = 1.0;
    else
        tmp=-1.0;

    /// przejœcie po wszystkich pixelach na szeroœci
    /// dla ka¿dego, przejœcie przez wszystkie pionowe pixele
    /// nastêpnie wype³nienie tablicy dwuwymiarowej imageDepth[x][y] - wartoœciami danego pixela ( w skali szaroœci )
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
    /// rysowanie za pomoc¹ QPainter'a na obrazku _imageGeneratedStereogram
    /// pomocniczych czarnych kropek ( opcjonalnie, dla u¿ytkownika )
    /// czarne kropki s¹ w zadanej odleg³oœci od siebie - tak aby z dwóch ( u¿ytkownik programu zobaczy³ trzy )
    QPainter p;
    p.begin(_imageGeneratedStereogram);
    p.setBrush(QBrush(Qt::black));
    p.setPen(Qt::black);
    p.drawEllipse(_widthOfImage_X/2-_farOfDots/2,_heightOfImage_Y*16/19,20,20);
    p.drawEllipse(_widthOfImage_X/2+_farOfDots/2,_heightOfImage_Y*16/19,20,20);
    p.end();
}
