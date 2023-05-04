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

private:
    Ui::CharactersView *ui;
};

#endif // CHARACTERSVIEW_H
