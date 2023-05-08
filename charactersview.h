#ifndef CHARACTERSVIEW_H
#define CHARACTERSVIEW_H

#include <QWidget>
#include "datatypes.h"
#include <QDialog>

namespace Ui {
class CharactersView;
}

class CharactersView : public QDialog
{
    Q_OBJECT

public:
    explicit CharactersView(QWidget *parent = nullptr);
    void mapDict();
    ~CharactersView();

    Characters * dict;

signals:
    void editingFinished();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_persons_list_currentTextChanged(const QString &currentText);

    void on_moods_list_currentTextChanged(const QString &currentText);

    void on_mood_name_returnPressed();

    void on_person_name_returnPressed();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_mood_file_returnPressed();

    void on_pushButton_7_clicked();

    void on_openfile_button_clicked();

private:
    Ui::CharactersView *ui;
};

#endif // CHARACTERSVIEW_H
