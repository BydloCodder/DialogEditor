#ifndef DICTIONARYVIEW_H
#define DICTIONARYVIEW_H

#include <QWidget>
#include <QHash>
#include <QDialog>

namespace Ui {
class DictionaryView;
}

class DictionaryView : public QDialog
{
    Q_OBJECT

public:
    explicit DictionaryView(QWidget *parent = nullptr);
    void mapDict();
    ~DictionaryView();
    QHash<QString, QString> *dict = 0;

    bool addFiles = false;

    void setColumnNames(QString key, QString value);
signals:
    void editingFinished();

private slots:

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::DictionaryView *ui;
};

#endif // DICTIONARYVIEW_H
