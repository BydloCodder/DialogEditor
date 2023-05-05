#ifndef CHOICESEDITDIALOG_H
#define CHOICESEDITDIALOG_H

#include <QDialog>
#include "datatypes.h"

namespace Ui {
class ChoicesEditDialog;
}

class ChoicesEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChoicesEditDialog(QWidget *parent = nullptr);
    ~ChoicesEditDialog();
    void setBase(QHash<QString, QString> *backgrounds, QHash<QString, QString> *sounds,  QHash<QString, QString> *videos, Characters * characters, QStringList *idList);

    void represent();

    QVector<Event::Choice*> *choices;

private slots:
    void on_choices_list_currentRowChanged(int currentRow);

    void on_pushButton_clicked();

    void on_choices_text_returnPressed();

    void on_pushButton_2_clicked();

    void on_addEventButton_clicked();

    void on_removeEventButton_clicked();

    void on_choices_list_currentTextChanged(const QString &currentText);

    void on_condition_combobox_toggled(bool checked);

private:
    Ui::ChoicesEditDialog *ui;
    bool ready = false;
    QHash<QString, QString> * backgrounds, *sounds, *videos;
    Characters* characters;
    QStringList * idList;
};

#endif // CHOICESEDITDIALOG_H
