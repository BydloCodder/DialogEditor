#ifndef EVENTLISTITEM_H
#define EVENTLISTITEM_H

#include <QWidget>
#include <QHash>
#include <QStringList>
#include "datatypes.h"

namespace Ui {
class EventListItem;
}

class EventListItem : public QWidget
{
    Q_OBJECT

public:
    explicit EventListItem(QWidget *parent = nullptr);

    void setBase(QHash<QString, QString> *backgrounds, QHash<QString, QString> *sounds,  QHash<QString, QString> *videos, Characters * characters, QStringList *idList);
    void setEvent(Event *e) {this->e = e;}
    void represent();
    ~EventListItem();

private slots:
    void on_characterCombobox_toggled(bool checked);

    void on_jumpCombobox_toggled(bool checked);

    void on_idCombobox_toggled(bool checked);

    void on_backgroundCombobox_clicked();

    void on_soundButton_clicked();

    void on_backgroundCombobox_toggled(bool arg1);

    void on_characters_currentTextChanged(const QString &arg1);

    void on_jumpto_currentTextChanged(const QString &arg1);

    void on_id_editingFinished();

    void on_text_textChanged();

    void on_statsButton_clicked();

    void on_choicesButton_clicked();

    void on_timer_checkbox_toggled(bool checked);

    void on_id_returnPressed();

    void on_timer_valueChanged(double arg1);

    void on_checkBox_toggled(bool checked);

private:
    Ui::EventListItem *ui;

    Event *e;
    bool ready = false;

    QHash<QString, QString> * backgrounds, *sounds, *videos;
    Characters* characters;
    QStringList * idList;

};

#endif // EVENTLISTITEM_H
