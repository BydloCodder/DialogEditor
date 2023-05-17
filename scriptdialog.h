#ifndef SCRIPTDIALOG_H
#define SCRIPTDIALOG_H

#include <QDialog>
#include "datatypes.h"

namespace Ui {
class ScriptDialog;
}

class ScriptDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScriptDialog(QWidget *parent = nullptr);
    ~ScriptDialog();
    Event *e = 0;
    Event::Choice *c = 0;

    QString get() {
        if (e) return e->script;
        if (c) return c->condition;
        return "";
    }

    void set(QString s) {
        if (e) e->script = s;
        if (c) c->condition = s;
    }

    void represent();
    bool ready = false;

private slots:
    void on_groupBox_toggled(bool arg1);

    void on_script_textChanged();

private:
    Ui::ScriptDialog *ui;
};

#endif // SCRIPTDIALOG_H
