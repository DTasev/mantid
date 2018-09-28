#ifndef RECOVERYFAILUREVIEW_H
#define RECOVERYFAILUREVIEW_H

#include "ProjectRecoveryPresenter.h"
#include <QDialog>
#include <QWidget>

namespace Ui {
class RecoveryFailure;
}

class RecoveryFailureView : public QDialog {
  Q_OBJECT

public:
  explicit RecoveryFailureView(
      QWidget *parent = 0,
      ProjectRecoveryPresenter *presenter = nullptr);
  ~RecoveryFailureView();
  void reject() override;

private slots:
  void onClickLastCheckpoint();
  void onClickSelectedCheckpoint();
  void onClickOpenSelectedInScriptWindow();
  void onClickStartMantidNormally();

private:
  void addDataToTable(Ui::RecoveryFailure *ui);

  Ui::RecoveryFailure *ui;
  ProjectRecoveryPresenter *m_presenter;
};

#endif // RECOVERYFAILUREVIEW_H
