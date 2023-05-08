#ifndef PERSONEDITOR_H
#define PERSONEDITOR_H

#include <QDialog>
#include "datatypes.h"

namespace Ui {
class PersonEditor;
}

class PersonEditor : public QDialog
{
    Q_OBJECT

public:
    explicit PersonEditor(QWidget *parent = nullptr);
    void represent(QString person = "left");
    void show(QString arg1);
    void setBase(Characters *c);
    ~PersonEditor();

    Event *e = 0;

private slots:
    void on_comboBox_4_currentTextChanged(const QString &arg1);

    void on_operation_currentTextChanged(const QString &arg1);

    void on_person_currentTextChanged(const QString &arg1);

    void on_mood_currentTextChanged(const QString &arg1);

    void on_appear_checkbox_toggled(bool checked);

    void on_appear_combobox_currentTextChanged(const QString &arg1);

    void on_appear_time_valueChanged(double arg1);

    void on_appear_backwards_toggled(bool checked);

    void on_fade_checkbox_toggled(bool checked);

    void on_fade_time_valueChanged(double arg1);

    void on_fade_backwards_toggled(bool checked);

private:
    Ui::PersonEditor *ui;
    Characters* c;
};

#endif // PERSONEDITOR_H
