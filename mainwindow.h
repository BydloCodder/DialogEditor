#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHash>
#include <QVector>
#include <QJsonObject>
#include "datatypes.h"
#include "dictionaryview.h"
#include "charactersview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void loadConfig();
    void saveConfig();

    QJsonObject readJson(QString filename);
    QHash<QString, QString> loadDictionary(QString filename, QString res);
    void saveDictionary(QString filename, QHash<QString, QString> &dict, QString res);
    void getIndexes(QVector<Event *> events);
    void represent();
    ~MainWindow();

private slots:
    void on_res_button_clicked();

    void on_timeline_load_clicked();

    void on_backgrounds_load_clicked();

    void on_sounds_load_clicked();

    void on_videos_load_clicked();

    void on_characters_load_clicked();

    void on_backgrounds_edit_clicked();

    void on_sounds_edit_clicked();

    void on_videos_edit_clicked();

    void on_characters_edit_clicked();

    void on_groupBox_toggled(bool arg1);

    void on_timeline_id_editingFinished();

    void on_timeline_name_editingFinished();

    void on_pushButton_16_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

private:

    bool firstLoad = true;
    Timeline timeline;
    QHash<QString, QString> backgrounds, sounds, videos;
    Characters characters;
    DictionaryView *backgroundsWidget = 0, *soundsWidget = 0, *videosWidget = 0;
    CharactersView *charactersWidget = 0;
    QStringList idList;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
