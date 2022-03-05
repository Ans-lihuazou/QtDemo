#ifndef FINDFORM_H
#define FINDFORM_H

#include <QWidget>

namespace Ui {
class FindForm;
}

class FindForm : public QWidget
{
    Q_OBJECT

public:
    explicit FindForm(QWidget *parent = nullptr);
    ~FindForm();

private:
    Ui::FindForm *ui;
};

#endif // FINDFORM_H
