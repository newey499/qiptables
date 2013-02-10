#ifndef FORMDLGRULESET_H
#define FORMDLGRULESET_H

#include <QDialog>
#include <QVariant>

#include "formcfgruleset.h"


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

protected slots:

    virtual void buttonsEnabled(bool enabled);
    virtual void dataChanged();


private:
    Ui::FormDlgRuleset *ui;
};

#endif // FORMDLGRULESET_H
