#ifndef CONDITIONEDITOR_H
#define CONDITIONEDITOR_H

#include <QDialog>
#include "datatypes.h"
#include <QListWidgetItem>

namespace Ui {
class ConditionEditor;
}

class ConditionEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ConditionEditor(QWidget *parent = nullptr);
    ~ConditionEditor();

    void represent();

    Event::Condition *condition;

private slots:
    void on_operation_currentTextChanged(const QString &arg1);

    void on_var_textChanged(const QString &arg1);

    void on_value_textChanged(const QString &arg1);

    void on_cast_checkbox_toggled(bool checked);

    void on_cast_combobox_currentTextChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_data_itemDoubleClicked(QListWidgetItem *);

private:
    Ui::ConditionEditor *ui;
    bool ready = false;
};

#endif // CONDITIONEDITOR_H
