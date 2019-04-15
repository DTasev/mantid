#ifndef IALGORITHMPROGRESSWIDGET_H
#define IALGORITHMPROGRESSWIDGET_H
class QProgressBar;

namespace MantidQt {
namespace MantidWidgets {
class IAlgorithmProgressWidget {
public:
  IAlgorithmProgressWidget() = default;
  ~IAlgorithmProgressWidget() = default;
  virtual QProgressBar *progressBar() const = 0;
  virtual void algorithmStarted() = 0;
  virtual void algorithmEnded() = 0;
  virtual void showDetailsDialog() = 0;
};

} // namespace MantidWidgets
} // namespace MantidQt

#endif // IALGORITHMPROGRESSWIDGET_H
