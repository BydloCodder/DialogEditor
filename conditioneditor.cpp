#include "conditioneditor.h"
#include "ui_conditioneditor.h"


ConditionEditor::ConditionEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConditionEditor)
{
    ui->setupUi(this);
    this->setWindowTitle("Condition Editor");
}

ConditionEditor::~ConditionEditor()
{
    delete ui;
}

void ConditionEditor::represent()
{
    ui->operation->setCurrentText(condition->op);
    ui->var->setText(condition->var);
    ui->value->setText(condition->value);
    ui->cast_checkbox->setChecked(!condition->cast.isEmpty());
    if (!condition->cast.isEmpty())
        ui->cast_combobox->setCurrentText(condition->cast);
    ui->cond_edit_box->setEnabled(!condition->logical());
    ui->data_box->setEnabled(condition->logical());
    ui->data->setEnabled(condition->logical());
    ui->data->clear();
    foreach (auto c, condition->data) {
        ui->data->addItem(c.toString());
    }

    ready = true;
}

void ConditionEditor::on_operation_currentTextChanged(const QString &arg1)
{
    if (ready) {
        condition->op = arg1;
        represent();
    }
}


void ConditionEditor::on_var_textChanged(const QString &arg1)
{
    if (ready)
        condition->var = arg1;
}


void ConditionEditor::on_value_textChanged(const QString &arg1)
{
    if (ready)
        condition->value = arg1;
}


void ConditionEditor::on_cast_checkbox_toggled(bool checked)
{
    if (ready) {
        ui->cast_combobox->setEnabled(checked);
        if (checked)
            condition->cast = ui->cast_combobox->currentText();
        else
            condition->cast = "";
    }
}


void ConditionEditor::on_cast_combobox_currentTextChanged(const QString &arg1)
{
    if (ready) {
        condition->cast = arg1;
    }
}


void ConditionEditor::on_pushButton_clicked()
{
    condition->data.append(Event::Condition());
    represent();
}


void ConditionEditor::on_pushButton_2_clicked()
{
    int row = ui->data->currentRow();
    if (row >= 0 && condition->data.count() > 0) {
        condition->data.removeAt(row);
        delete ui->data->takeItem(row);
    }
}


void ConditionEditor::on_data_itemDoubleClicked(QListWidgetItem *)
{
    int row = ui->data->currentRow();
    auto dialog = new ConditionEditor();
    dialog->condition = &condition->data[row];
    dialog->represent();
    dialog->exec();
    represent();
}

