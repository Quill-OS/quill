#include <QJsonDocument>
#include <QJsonObject>

#include "qclickablelabel.h"

QClickableLabel::QClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {

}

QClickableLabel::~QClickableLabel() {}

void QClickableLabel::mousePressEvent(QMouseEvent * event) {
    emit clicked();
    if(objectName() == "pageNumberLabel") {
        QClickableLabel::setStyleSheet("color: white; background-color: black; border-radius: 10px; padding-left: 10px; padding-right: 10px");
    }
    else {
        QClickableLabel::setStyleSheet("color: white; background-color: black; border-radius: 10px; padding: 10px");
    }
}

void QClickableLabel::mouseReleaseEvent(QMouseEvent * event) {
    emit unclicked();
    emit bookID(objectName().toInt());
    emit bookPath(QJsonDocument::fromJson(objectName().toUtf8()).object()["BookPath"].toString());
    if(objectName() == "pageNumberLabel") {
        QClickableLabel::setStyleSheet("color: black; background-color: white; border-radius: 10px; padding-left: 10px; padding-right: 10px");
    }
    else {
        QClickableLabel::setStyleSheet("color: black; background-color: white; border-radius: 10px; padding: 10px");
    }
}
