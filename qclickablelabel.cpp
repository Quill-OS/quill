#include "qclickablelabel.h"

QClickableLabel::QClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {

}

QClickableLabel::~QClickableLabel() {}

void QClickableLabel::mousePressEvent(QMouseEvent * event) {
    emit clicked();
    QClickableLabel::setStyleSheet("color: white; background-color: black; border-radius: 10px; padding: 20px");
}

void QClickableLabel::mouseReleaseEvent(QMouseEvent * event) {
    emit unclicked();
    QClickableLabel::setStyleSheet("color: black; background-color: white; border-radius: 10px; padding: 20px");
}
