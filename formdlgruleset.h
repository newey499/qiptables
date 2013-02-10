#ifndef FORMDLGRULESET_H
#define FORMDLGRULESET_H

#include <QDialog>

namespace Ui {
class FormDlgRuleset;
}

class FormDlgRuleset : public QDialog
{
    Q_OBJECT
    
public:
    explicit FormDlgRuleset(QWidget *parent = 0);
    ~FormDlgRuleset();
    
private:
    Ui::FormDlgRuleset *ui;
};

#endif // FORMDLGRULESET_H
