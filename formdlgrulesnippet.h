#ifndef FORMDLGRULESNIPPET_H
#define FORMDLGRULESNIPPET_H

#include <QDialog>
#include <QPointer>
#include <QVariant>

#include "formcfgrulesnippets.h"

class FormCfgRuleSnippets;

namespace Ui {
class FormDlgRuleSnippet;
}

class FormDlgRuleSnippet : public QDialog
{
    Q_OBJECT

public:
    explicit FormDlgRuleSnippet(int opCode, FormCfgRuleSnippets *parent = 0);
    ~FormDlgRuleSnippet();

protected:

    QPointer<FormCfgRuleSnippets> formCfgRuleSnippets;
    int opCode;

    bool validateData();
    bool writeRow();

    // row contents
    int id;
    QString name;
    QString snippets;

protected slots:

    virtual void dataChanged();
    virtual void slotBtnSave();
    virtual void slotBtnCancel();

    virtual void buttonsEnabled(bool enabled);


private:

    Ui::FormDlgRuleSnippet *ui;

};

#endif // FORMDLGRULESNIPPET_H
