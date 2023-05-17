#include "scriptdialog.h"
#include "ui_scriptdialog.h"
#include <QFontMetricsF>

ScriptDialog::ScriptDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScriptDialog)
{
    ui->setupUi(this);
    setWindowTitle("Script");
    QString defaultText = "func start(dialog, event):\n";
    defaultText += "\tpass";
    defaultText +="\nfunc condition(dialog, event):\n";
    defaultText +="\treturn true";
    ui->script->setPlainText(defaultText);

    QFontMetricsF fm (ui->script->font());
    auto stopWidth = 4 * fm.horizontalAdvance(" ");
    ui->script->setTabStopDistance(stopWidth);
}

ScriptDialog::~ScriptDialog()
{
    delete ui;
}

void ScriptDialog::represent()
{
    if (get().isEmpty()) {
        ui->groupBox->setChecked(false);
    } else {
        ui->groupBox->setChecked(true);
        ui->script->setPlainText(get());
    }
    ready = true;
}

void ScriptDialog::on_groupBox_toggled(bool arg1)
{
    if (arg1) {
        set(ui->script->toPlainText());
    } else
        set("");
}


void ScriptDialog::on_script_textChanged()
{
    if (ready)
        set(ui->script->toPlainText());
}

