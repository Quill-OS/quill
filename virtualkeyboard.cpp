#include "virtualkeyboard.h"
#include "ui_virtualkeyboard.h"

#include <QTimer>
#include <QDebug>

virtualkeyboard::virtualkeyboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::virtualkeyboard)
{   
    ui->setupUi(this);
    shift = false;

    ui->n1->setProperty("type", "borderless");
    ui->n2->setProperty("type", "borderless");
    ui->n3->setProperty("type", "borderless");
    ui->n4->setProperty("type", "borderless");
    ui->n5->setProperty("type", "borderless");
    ui->n6->setProperty("type", "borderless");
    ui->n7->setProperty("type", "borderless");
    ui->n8->setProperty("type", "borderless");
    ui->n9->setProperty("type", "borderless");
    ui->n0->setProperty("type", "borderless");

    ui->la->setProperty("type", "borderless");
    ui->lb->setProperty("type", "borderless");
    ui->lc->setProperty("type", "borderless");
    ui->ld->setProperty("type", "borderless");
    ui->le->setProperty("type", "borderless");
    ui->lf->setProperty("type", "borderless");
    ui->lg->setProperty("type", "borderless");
    ui->lh->setProperty("type", "borderless");
    ui->li->setProperty("type", "borderless");
    ui->lj->setProperty("type", "borderless");
    ui->lk->setProperty("type", "borderless");
    ui->ll->setProperty("type", "borderless");
    ui->lm->setProperty("type", "borderless");
    ui->ln->setProperty("type", "borderless");
    ui->lo->setProperty("type", "borderless");
    ui->lp->setProperty("type", "borderless");
    ui->lq->setProperty("type", "borderless");
    ui->lr->setProperty("type", "borderless");
    ui->ls->setProperty("type", "borderless");
    ui->lt->setProperty("type", "borderless");
    ui->lu->setProperty("type", "borderless");
    ui->lv->setProperty("type", "borderless");
    ui->lw->setProperty("type", "borderless");
    ui->lx->setProperty("type", "borderless");
    ui->ly->setProperty("type", "borderless");
    ui->lz->setProperty("type", "borderless");

    ui->eraseBtn->setProperty("type", "borderless");
    ui->shiftBtn->setProperty("type", "borderless");
    ui->spt->setProperty("type", "borderless");
    ui->sat->setProperty("type", "borderless");

    ui->n1->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->n2->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->n3->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->n4->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->n5->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->n6->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->n7->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->n8->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->n9->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->n0->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");

    ui->la->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->lb->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->lc->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->ld->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->le->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->lf->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->lg->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->lh->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->li->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->lj->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->lk->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->ll->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->lm->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->ln->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->lo->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->lp->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->lq->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->lr->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->ls->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->lt->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->lu->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->lv->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->lw->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->lx->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->ly->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->lz->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");

    ui->eraseBtn->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->shiftBtn->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->spt->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->sat->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 13px");
    ui->spaceBtn->setStyleSheet("font-weight: bold; font-size: 9pt; padding: 10px; border: 1px solid black");

    ui->lineEdit->setStyleSheet("border: 3px solid black");
}

virtualkeyboard::~virtualkeyboard()
{
    delete ui;
}

void virtualkeyboard::on_spaceBtn_clicked()
{
    ui->lineEdit->insert(" ");
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_eraseBtn_clicked()
{
    ui->lineEdit->backspace();
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_spt_clicked()
{
    ui->lineEdit->insert(".");
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_sat_clicked()
{
    ui->lineEdit->insert("@");
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_n1_clicked()
{
    ui->lineEdit->insert("1");
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_n2_clicked()
{
    ui->lineEdit->insert("2");
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_n3_clicked()
{
    ui->lineEdit->insert("3");
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_n4_clicked()
{
    ui->lineEdit->insert("4");
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_n5_clicked()
{
    ui->lineEdit->insert("5");
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_n6_clicked()
{
    ui->lineEdit->insert("6");
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_n7_clicked()
{
    ui->lineEdit->insert("7");
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_n8_clicked()
{
    ui->lineEdit->insert("8");
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_n9_clicked()
{
    ui->lineEdit->insert("9");
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_n0_clicked()
{
    ui->lineEdit->insert("0");
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_lq_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("Q");
    }
    else {
        ui->lineEdit->insert("q");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_lw_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("W");
    }
    else {
        ui->lineEdit->insert("w");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_le_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("E");
    }
    else {
        ui->lineEdit->insert("e");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_lr_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("R");
    }
    else {
        ui->lineEdit->insert("r");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_lt_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("T");
    }
    else {
        ui->lineEdit->insert("t");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_ly_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("Y");
    }
    else {
        ui->lineEdit->insert("y");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_lu_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("U");
    }
    else {
        ui->lineEdit->insert("u");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_li_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("I");
    }
    else {
        ui->lineEdit->insert("i");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_lo_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("O");
    }
    else {
        ui->lineEdit->insert("o");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_lp_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("P");
    }
    else {
        ui->lineEdit->insert("p");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_la_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("A");
    }
    else {
        ui->lineEdit->insert("a");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_ls_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("S");
    }
    else {
        ui->lineEdit->insert("s");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_ld_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("D");
    }
    else {
        ui->lineEdit->insert("d");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_lf_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("F");
    }
    else {
        ui->lineEdit->insert("f");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_lg_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("G");
    }
    else {
        ui->lineEdit->insert("g");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_lh_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("H");
    }
    else {
        ui->lineEdit->insert("h");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_lj_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("J");
    }
    else {
        ui->lineEdit->insert("j");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_lk_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("K");
    }
    else {
        ui->lineEdit->insert("k");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_ll_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("L");
    }
    else {
        ui->lineEdit->insert("l");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_lz_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("Z");
    }
    else {
        ui->lineEdit->insert("z");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_lx_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("X");
    }
    else {
        ui->lineEdit->insert("x");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_lc_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("C");
    }
    else {
        ui->lineEdit->insert("c");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_lv_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("V");
    }
    else {
        ui->lineEdit->insert("v");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_lb_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("B");
    }
    else {
        ui->lineEdit->insert("b");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_ln_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("N");
    }
    else {
        ui->lineEdit->insert("n");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_lm_clicked()
{
    if(shift == true) {
        ui->lineEdit->insert("M");
    }
    else {
        ui->lineEdit->insert("m");
    }
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}

void virtualkeyboard::on_shiftBtn_clicked()
{
    if(shift == true) {
        shift = false;
    }
    else {
        shift = true;
    }
    reverseKeys();
}

void virtualkeyboard::reverseKeys() {
    if(shift == true) {
        ui->shiftBtn->setText("⇪");

        ui->la->setText("A");
        ui->lb->setText("B");
        ui->lc->setText("C");
        ui->ld->setText("D");
        ui->le->setText("E");
        ui->lf->setText("F");
        ui->lg->setText("G");
        ui->lh->setText("H");
        ui->li->setText("I");
        ui->lj->setText("J");
        ui->lk->setText("K");
        ui->ll->setText("L");
        ui->lm->setText("M");
        ui->ln->setText("N");
        ui->lo->setText("O");
        ui->lp->setText("P");
        ui->lq->setText("Q");
        ui->lr->setText("R");
        ui->ls->setText("S");
        ui->lt->setText("T");
        ui->lu->setText("U");
        ui->lv->setText("V");
        ui->lw->setText("W");
        ui->lx->setText("X");
        ui->ly->setText("Y");
        ui->lz->setText("Z");
    }
    else {
        ui->shiftBtn->setText("⇧");

        ui->la->setText("a");
        ui->lb->setText("b");
        ui->lc->setText("c");
        ui->ld->setText("d");
        ui->le->setText("e");
        ui->lf->setText("f");
        ui->lg->setText("g");
        ui->lh->setText("h");
        ui->li->setText("i");
        ui->lj->setText("j");
        ui->lk->setText("k");
        ui->ll->setText("l");
        ui->lm->setText("m");
        ui->ln->setText("n");
        ui->lo->setText("o");
        ui->lp->setText("p");
        ui->lq->setText("q");
        ui->lr->setText("r");
        ui->ls->setText("s");
        ui->lt->setText("t");
        ui->lu->setText("u");
        ui->lv->setText("v");
        ui->lw->setText("w");
        ui->lx->setText("x");
        ui->ly->setText("y");
        ui->lz->setText("z");
    }
    QTimer::singleShot(1000, this, SLOT(adjust_size_function()));
}

void virtualkeyboard::adjust_size_function() {
    emit adjust_size();
}

void virtualkeyboard::clearLineEdit() {
    ui->lineEdit->clear();
    QString text = ui->lineEdit->text();
    global::keyboard::keyboardText = text;
}
