#include "generateimage.h"
#include <mainwindow.h>
#include <ImageMagick-7/Magick++.h>
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <iostream>
#include <locale>
#include <codecvt>
#include <string>
GenerateImage::GenerateImage(double rDistortion,double rWidth,double rHeight) {

    setDistortion(rDistortion);
    setWidth(rWidth);
    setHeight(rHeight);
}

double GenerateImage::getWidth() {
    return this->width;
}

double GenerateImage::getHeight() {
    return this->height;
}

double GenerateImage::getDistortion() {
    return this->distortion;
}

void GenerateImage::setWidth(double rWidth) {
    this->width = rWidth;
}

void GenerateImage::setHeight(double rHeight) {
    this->height = rHeight;
}

void GenerateImage::setDistortion(double rDistortion) {
    this->distortion = rDistortion;
}

void GenerateImage::inputImage(const QStringList& filePathes){
    Magick::Image image;
    try {
        for(const auto& filePath: filePathes) {
            image.read(filePath.toStdString());
            this->processImages.push_back(image);
        }
    }
    catch(std::exception &error_ ) {
        qDebug() << "Caught exception: " << error_.what() << '\n';
    }
}

void GenerateImage::generateImage() {
    for(auto& image : this->processImages) {
        qDebug() << "processing\n";
        std::string fileName = image.baseFilename();
        int i = fileName.size() - 1;
        while(fileName[i] != '/') {
            i--;
        }
        fileName = fileName.substr(i,fileName.size()-i);
        /*
        QString textForProcessing =   QString("FROM fairest creatures we desire increase,")
                                    + QString("That thereby beauty's rose might never die,")
                                    + QString("But as the riper should by time decease,")
                                    + QString("His tender heir might bear his memory:")
                                    + QString("But thou, contracted to thine own bright eyes,")
                                    + QString("Feed'st thy light'st flame with self-substantial fuel,")
                                    + QString("Making a famine where abundance lies,")
                                    + QString("Thyself thy foe, to thy sweet self too cruel.")
                                    + QString("Thou that art now the world's fresh ornament")
                                    + QString("And only herald to the gaudy spring,")
                                    + QString("Within thine own bud buriest thy content")
                                    + QString("And, tender churl, makest waste in niggarding.")
                                    + QString("Pity the world, or else this glutton be,");

        */
        std::string textForProcessing = u8"русский";
        GenerateImage::placeText(image,textForProcessing).write("/home/prise/testImages/"+ fileName);
    }
}

Magick::Image GenerateImage::placeText(Magick::Image& modificate, const std::string& text) {
//    modificate.font("Quirlycues");
    modificate.fontWeight(1);
    modificate.fontPointsize(22);
    modificate.strokeWidth(0.5);
    modificate.strokeColor("#00202F");

    int i = 0;
    int j = 1;
    double firstDerivative = 0;
    double currentHeight = 3;


    const auto tempText = text;
    QByteArray byteArray(tempText.c_str(), tempText.length());
    qDebug() << byteArray.toHex() << "\n\n\n";
    for(const auto& x : byteArray) {
        qDebug() << QString("0x%1").arg(static_cast<int>(x), 0, 16) << '\n';
    }
    std::cout << tempText << '\n';
    for(int p = 0; p < tempText.size(); p+=2) {
        firstDerivative += randomizeCoords(currentHeight);
        currentHeight += firstDerivative;
        if(static_cast<unsigned>(70+11*i) >= modificate.baseColumns()) {
            i = 0;
            if(tempText[p] == '.' ||  tempText[p] == '/' || tempText[p] == '\\') {
                continue;
            }
            j++;
        }
        modificate.draw(Magick::DrawableText(30+i*11,70*j+currentHeight,tempText.substr(p,2),"UTF-8"));
        qDebug() << (60+11*i) % modificate.baseColumns() << " " << currentHeight;
        i++;
    }

    qDebug() << QString::fromStdString(modificate.format())<< '\n';
    return modificate;

}

double GenerateImage::randomizeCoords(const double& y) {
    std::srand(std::time(NULL));
    return -tl::sgn(y)*std::pow(std::abs(y), std::rand()%2);
}

bool GenerateImage::extractImage(Magick::Image* readyPicture, QString fullPath) {
    if(readyPicture != nullptr) {
        readyPicture->write(fullPath.toStdString());
        return true;
    } else {
        return false;
    }
}

//    modificate.font("Quirlycues");
//    modificate.strokeWidth(2);
//    modificate.draw(Magick::DrawableStrokeColor("blue"));
//    modificate.draw(Magick::DrawableFillColor("blue"));
//    modificate.draw(Magick::DrawableFont("Celestina",Magick::AnyStyle,400,Magick::NormalStretch));
