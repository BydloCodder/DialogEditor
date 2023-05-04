#ifndef BACKGROUNDEDITDIALOG_H
#define BACKGROUNDEDITDIALOG_H

#include <QDialog>
#include <QHash>
#include "datatypes.h"

namespace Ui {
class BackgroundEditDialog;
}

class BackgroundEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BackgroundEditDialog(QWidget *parent = nullptr);
    ~BackgroundEditDialog();
    void setBase(QHash<QString, QString> *backgrounds, QHash<QString, QString> *videos);
    void setEvent(Event *e) {this->e = e;}
    void represent();


private slots:
    void on_imageCheckbox_toggled(bool checked);

    void on_image_combobox_currentTextChanged(const QString &arg1);

    void on_video_checkbox_toggled(bool checked);

    void on_video_combobox_currentTextChanged(const QString &arg1);

    void on_fade_combobox_toggled(bool checked);

    void on_fade_valueChanged(double arg1);

    void on_clickable_checkbox_stateChanged(int arg1);

    void on_background_box_toggled(bool arg1);

private:
    Ui::BackgroundEditDialog *ui;

    Event *e;
    QHash<QString, QString> *backgrounds, *videos;
    bool ready = false;
};

#endif // BACKGROUNDEDITDIALOG_H
