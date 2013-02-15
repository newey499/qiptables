#ifndef FORMDLGRULESET_H
#define FORMDLGRULESET_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include "rulesetsqltablemodel.h"
#include "formcfgruleset.h"
#include "formdlgrulesnippet.h"

class FormCfgRuleset;

namespace Ui {
class FormDlgRuleset;
}

class FormDlgRuleset : public QDialog
{
    Q_OBJECT

public:
    explicit FormDlgRuleset(int opCode, FormCfgRuleset *parent);
    ~FormDlgRuleset();

public slots:

    virtual void slotSave();
    virtual void slotCancel();

protected:

    FormCfgRuleset *formRuleset;
    int id;
    QString name;
    QString rules;
    int opcode;

    bool isDefaultRuleset();
    bool validateData();
    bool writeRow();

protected slots:

    virtual void buttonsEnabled(bool enabled);
    virtual void dataChanged();


private:
    Ui::FormDlgRuleset *ui;
};

#endif // FORMDLGRULESET_H
