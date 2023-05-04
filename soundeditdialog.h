#ifndef SOUNDEDITDIALOG_H
#define SOUNDEDITDIALOG_H

#include <QDialog>
#include <QHash>
#include "datatypes.h"

namespace Ui {
class SoundEditDialog;
}

class SoundEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SoundEditDialog(QWidget *parent = nullptr);
    void setBase(QHash<QString, QString> *sounds);
    void setEvent(Event *e) {this->e = e;}
    void represent();
    ~SoundEditDialog();

private slots:
    void on_playBox_toggled(bool arg1);

    void on_stopBox_toggled(bool arg1);

    void on_stopsound_channel_editingFinished();

    void on_loop_stateChanged(int arg1);

    void on_fade_valueChanged(double arg1);

    void on_fade_checkbox_toggled(bool checked);

    void on_volume_sliderReleased();

    void on_bus_editingFinished();

    void on_sound_currentTextChanged(const QString &arg1);

    void on_playsound_channel_editingFinished();

private:
    Ui::SoundEditDialog *ui;
    Event *e;
    QHash<QString, QString> *sounds;
    bool ready = false;
};

#endif // SOUNDEDITDIALOG_H
